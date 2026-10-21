#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>

/* ============================================================
                        BINARY TETRIS
   ============================================================ */

#define TW 10
#define TH 20

static const int TETRIS_SHAPES[7][4][4][4] = {
    /* I */ {
        {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
        {{0,0,1,0},{0,0,1,0},{0,0,1,0},{0,0,1,0}},
        {{0,0,0,0},{0,0,0,0},{1,1,1,1},{0,0,0,0}},
        {{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0}} },
    /* O */ {
        {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
        {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
        {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
        {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}} },
    /* T */ {
        {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{0,1,0,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}},
        {{0,0,0,0},{1,1,1,0},{0,1,0,0},{0,0,0,0}},
        {{0,1,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}} },
    /* S */ {
        {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
        {{0,1,0,0},{0,1,1,0},{0,0,1,0},{0,0,0,0}},
        {{0,0,0,0},{0,1,1,0},{1,1,0,0},{0,0,0,0}},
        {{1,0,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}} },
    /* Z */ {
        {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{0,0,1,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}},
        {{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}},
        {{0,1,0,0},{1,1,0,0},{1,0,0,0},{0,0,0,0}} },
    /* J */ {
        {{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{0,1,1,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}},
        {{0,0,0,0},{1,1,1,0},{0,0,1,0},{0,0,0,0}},
        {{0,1,0,0},{0,1,0,0},{1,1,0,0},{0,0,0,0}} },
    /* L */ {
        {{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{0,1,0,0},{0,1,0,0},{0,1,1,0},{0,0,0,0}},
        {{0,0,0,0},{1,1,1,0},{1,0,0,0},{0,0,0,0}},
        {{1,1,0,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}} }
};

static long long now_ms(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000LL + tv.tv_usec / 1000;
}

static int read_char_timeout(int ms) {
    struct timeval tv;
    tv.tv_sec  = ms / 1000;
    tv.tv_usec = (ms % 1000) * 1000;
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0) {
        char c;
        if (read(STDIN_FILENO, &c, 1) == 1) return (unsigned char)c;
    }
    return -1;
}

static int t_collide(int board[TH][TW], const int shape[4][4], int px, int py) {
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            if (shape[y][x]) {
                int bx = px + x, by = py + y;
                if (bx < 0 || bx >= TW) return 1;
                if (by >= TH) return 1;
                if (by >= 0 && board[by][bx]) return 1;
            }
    return 0;
}

static void t_lock(int board[TH][TW], const int shape[4][4], int px, int py) {
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            if (shape[y][x]) {
                int bx = px + x, by = py + y;
                if (bx >= 0 && bx < TW && by >= 0 && by < TH)
                    board[by][bx] = 1;
            }
}

static int t_clear_lines(int board[TH][TW]) {
    int cleared = 0;
    for (int y = TH - 1; y >= 0; ) {
        int full = 1;
        for (int x = 0; x < TW; x++)
            if (!board[y][x]) { full = 0; break; }
        if (full) {
            for (int yy = y; yy > 0; yy--)
                memcpy(board[yy], board[yy - 1], sizeof(board[yy]));
            memset(board[0], 0, sizeof(board[0]));
            cleared++;
        } else {
            y--;
        }
    }
    return cleared;
}

static void play_tetris(void) {
    struct termios orig, raw;
    tcgetattr(STDIN_FILENO, &orig);
    raw = orig;
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_iflag &= ~(IXON | ICRNL);
    raw.c_cc[VMIN]  = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    printf("\x1b[?1049h\x1b[?25l\x1b[2J\x1b[H");
    fflush(stdout);

    srand((unsigned)time(NULL));

    int board[TH][TW];
    memset(board, 0, sizeof(board));

    int score = 0, game_over = 0, running = 1;
    int cur_type = rand() % 7, cur_rot = 0;
    int cur_x = 3, cur_y = 0;

    long long last_drop = now_ms();
    long long drop_interval = 800;

    while (running && !game_over) {
        int c;
        while ((c = read_char_timeout(0)) >= 0) {
            if (c == 27) {
                int c2 = read_char_timeout(20);
                if (c2 == '[') {
                    int c3 = read_char_timeout(20);
                    if (c3 == 'A') {
                        int nr = (cur_rot + 1) % 4;
                        if (!t_collide(board, TETRIS_SHAPES[cur_type][nr], cur_x, cur_y))
                            cur_rot = nr;
                    } else if (c3 == 'B') {
                        if (!t_collide(board, TETRIS_SHAPES[cur_type][cur_rot], cur_x, cur_y + 1))
                            cur_y++;
                    } else if (c3 == 'C') {
                        if (!t_collide(board, TETRIS_SHAPES[cur_type][cur_rot], cur_x + 1, cur_y))
                            cur_x++;
                    } else if (c3 == 'D') {
                        if (!t_collide(board, TETRIS_SHAPES[cur_type][cur_rot], cur_x - 1, cur_y))
                            cur_x--;
                    }
                } else if (c2 < 0) {
                    running = 0;
                }
            } else if (c == 'q' || c == 'Q') {
                running = 0;
            } else if (c == ' ') {
                while (!t_collide(board, TETRIS_SHAPES[cur_type][cur_rot], cur_x, cur_y + 1))
                    cur_y++;
            }
        }
        if (!running) break;

        long long now = now_ms();
        if (now - last_drop >= drop_interval) {
            last_drop = now;
            const int (*shape)[4] = TETRIS_SHAPES[cur_type][cur_rot];
            if (!t_collide(board, shape, cur_x, cur_y + 1)) {
                cur_y++;
            } else {
                t_lock(board, shape, cur_x, cur_y);
                int lines = t_clear_lines(board);
                if (lines > 0) {
                    score += (lines == 1) ? 100
                           : (lines == 2) ? 300
                           : (lines == 3) ? 500 : 800;
                    if (drop_interval > 120) drop_interval -= lines * 40;
                }
                cur_type = rand() % 7;
                cur_rot  = 0;
                cur_x    = 3;
                cur_y    = 0;
                if (t_collide(board, TETRIS_SHAPES[cur_type][0], cur_x, cur_y))
                    game_over = 1;
            }
        }

        /* --- render --- */
        printf("\x1b[H");
        printf("========== BINARY TETRIS ==========\n");
        printf("Score: %-6d  Speed: %lldms\n\n", score, drop_interval);
        printf("+----------+\n");

        const int (*shape)[4] = TETRIS_SHAPES[cur_type][cur_rot];
        for (int y = 0; y < TH; y++) {
            printf("|");
            for (int x = 0; x < TW; x++) {
                int sy = y - cur_y, sx = x - cur_x;
                int falling = (sy >= 0 && sy < 4 && sx >= 0 && sx < 4 && shape[sy][sx]);
                if (falling) {
                    char ch = ((sy + sx) & 1) ? '1' : '0';
                    printf("\x1b[1;32m%c\x1b[0m", ch);
                } else if (board[y][x]) {
                    char ch = ((x + y) & 1) ? '1' : '0';
                    printf("\x1b[36m%c\x1b[0m", ch);
                } else {
                    printf(" ");
                }
            }
            printf("|\n");
        }
        printf("+----------+\n");
        printf("\nArrows: move/rotate | SPACE: drop | Q: quit\n");
        fflush(stdout);

        usleep(16000);
    }

    printf("\x1b[2J\x1b[H");
    if (game_over) printf("========== GAME OVER ==========\n\n");
    else           printf("========== QUIT ==========\n\n");
    printf("Score: %d\n\n", score);
    printf("Press any key to return to menu...\n");
    fflush(stdout);

    tcflush(STDIN_FILENO, TCIFLUSH);
    char dummy;
    while (read(STDIN_FILENO, &dummy, 1) != 1) usleep(50000);
    tcflush(STDIN_FILENO, TCIFLUSH);

    printf("\x1b[?25h\x1b[?1049l");
    fflush(stdout);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig);
}

/* ============================================================
                        MAIN PROGRAM
   ============================================================ */

int main(void) {
    int choice;
    int mode;
    int decimal;
    char binary[65];
    int result;
    int score;
    char answer[65];

    while (1) {

        printf(
        "============================================================\n"
        "             +----------------------------+\n"
        "=============|                            |=============\n"
        "=============|   +--------------------+   |=============\n"
        "=============|   |                    |   |=============\n"
        "=============|   |                    |   |=============\n"
        "=============|   |                    |   |=============\n"
        "=============|   +--------------------+   |=============\n"
        "=============|                            |=============\n"
        "             +----------------------------+\n"
        "============================================================\n\n"
        );
        printf("1. What is electricity?\n");
        printf("2. What is binary code\n");
        printf("3. Binary -> Decimal and Decimal -> Binary\n");
        printf("4. What is transistor\n");
        printf("5. Logic gates (AND, OR, NOT, XOR)\n");
        printf("6. How memory stores bits\n");
        printf("7. How a CPU calculates (Half Adder & Full Adder)\n");
        printf("8. How two signals create everything\n");
        printf("9. How programs work\n");
        printf("10. Binary Playground\n");
        printf("11. Final Challenge\n");
        printf("12. Binary Tetris\n");
        printf("0. Exit\n\n");

        printf("Choose: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf(
                "       +-----------+\n"
                "       |  BATTERY  |\n"
                "       +-----------+\n"
                "         |       |\n"
                "         |       |\n"
                "         |       |\n"
                "         +-------+\n\n"
                );

                printf("WHAT IS ELECTRICITY?\n\n");

                printf("Electricity is the movement of tiny particles\n");
                printf("called electrons through a material.\n\n");

                printf("You cannot see electricity directly,\n");
                printf("but you can see its effects.\n\n");

                printf("Examples:\n");
                printf("- A lamp lights up.\n");
                printf("- A speaker plays sound.\n");
                printf("- A computer starts working.\n\n");

                printf("Think of electricity like water flowing\n");
                printf("through a pipe.\n\n");

                printf("Water          Electricity\n");
                printf("-------------------------------\n");
                printf("Water          Electrons\n");
                printf("Flow           Electric current\n");
                printf("Valve          Switch\n\n");

                printf("Every processor works because electricity\n");
                printf("moves through billions of tiny transistors.\n");

                break;

            case 2:
                printf(
                "0000000000000000000000000        111111111\n"
                "0000000000000000000000000      11111111111\n"
                "0000111111111111111110000          11111\n"
                "0001111111111111111111000          11111\n"
                "0011111000000000011111100          11111\n"
                "0111110000000000001111110          11111\n"
                "0111100000000000000111110          11111\n"
                "1111100000000000000111111          11111\n"
                "1111100000000000000111111          11111\n"
                "1111100000000000000111111          11111\n"
                "1111100000000000000111111          11111\n"
                "1111100000000000000111111          11111\n"
                "1111100000000000000111111          11111\n"
                "0111100000000000000111110          11111\n"
                "0111110000000000001111110          11111\n"
                "0011111000000000011111100          11111\n"
                "0001111111111111111111000          11111\n"
                "0000111111111111111110000          11111\n"
                "0000000000000000000000000      111111111111111\n"
                "0000000000000000000000000      111111111111111\n\n"
                );
                printf("\nBinary code is a way to store information using only 0 and 1.\n");
                break;

            case 3:
                printf(
                "101101        45\n"
                "1101          13\n"
                "100000        32\n"
                "\n"
                "      <---->\n"
                "\n"
                "45  -> 101101\n"
                "13  -> 1101\n"
                "32  -> 100000\n\n"
                );

                printf("HOW TO CONVERT BINARY TO DECIMAL\n\n");

                printf("Example: 101101\n\n");

                printf("Write powers of two:\n");
                printf("32 16 8 4 2 1\n");
                printf(" 1  0 1 1 0 1\n\n");

                printf("Add only columns containing 1:\n");
                printf("32 + 8 + 4 + 1 = 45\n\n");

                printf("101101(binary) = 45(decimal)\n\n");

                printf("----------------------------------------\n\n");

                printf("HOW TO CONVERT DECIMAL TO BINARY\n\n");

                printf("Example: 45\n\n");

                printf("45 / 2 = 22 remainder 1\n");
                printf("22 / 2 = 11 remainder 0\n");
                printf("11 / 2 =  5 remainder 1\n");
                printf(" 5 / 2 =  2 remainder 1\n");
                printf(" 2 / 2 =  1 remainder 0\n");
                printf(" 1 / 2 =  0 remainder 1\n\n");

                printf("Read the remainders from bottom to top:\n");
                printf("101101(binary)\n");

                break;

            case 4:
                printf(
                " _____\n"
                "|     |\n"
                "|_____|\n"
                " |   |\n"
                " |   |\n"
                " |   |\n"
                " |   |\n"
                );
                printf("\nA transistor is a tiny electronic switch. It can be ON (1) or OFF (0).\n");
                break;

            case 5:
                printf(
                "      A ----\\\n"
                "             )---- OUT\n"
                "      B ----/\n\n"
                );

                printf("LOGIC GATES\n\n");

                printf("Logic gates are built from transistors.\n");
                printf("Every CPU contains billions of them.\n\n");

                printf("----------------------------------------\n");
                printf("AND gate\n\n");

                printf("A B | OUT\n");
                printf("--------\n");
                printf("0 0 | 0\n");
                printf("0 1 | 0\n");
                printf("1 0 | 0\n");
                printf("1 1 | 1\n\n");

                printf("The output is 1 only if BOTH inputs are 1.\n\n");

                printf("----------------------------------------\n");
                printf("OR gate\n\n");

                printf("A B | OUT\n");
                printf("--------\n");
                printf("0 0 | 0\n");
                printf("0 1 | 1\n");
                printf("1 0 | 1\n");
                printf("1 1 | 1\n\n");

                printf("The output is 1 if ANY input is 1.\n\n");

                printf("----------------------------------------\n");
                printf("NOT gate\n\n");

                printf("A | OUT\n");
                printf("-------\n");
                printf("0 | 1\n");
                printf("1 | 0\n\n");

                printf("The NOT gate reverses the signal.\n\n");

                printf("----------------------------------------\n");
                printf("XOR gate\n\n");

                printf("A B | OUT\n");
                printf("--------\n");
                printf("0 0 | 0\n");
                printf("0 1 | 1\n");
                printf("1 0 | 1\n");
                printf("1 1 | 0\n\n");

                printf("XOR outputs 1 only when the inputs are different.\n\n");

                printf("These four gates are enough to build\n");
                printf("calculators, memory, and entire processors.\n");

                break;

            case 6:
                printf(
                "+---+---+---+---+---+---+---+---+\n"
                "| 1 | 0 | 1 | 1 | 0 | 0 | 1 | 0 |\n"
                "+---+---+---+---+---+---+---+---+\n\n"
                );

                printf("HOW MEMORY STORES BITS\n\n");

                printf("A computer stores information as bits.\n");
                printf("Each bit can contain only one value:\n\n");

                printf("0 = OFF\n");
                printf("1 = ON\n\n");

                printf("Eight bits form one byte.\n\n");

                printf("Example byte:\n\n");

                printf("10110010\n\n");

                printf("Every byte can store a number from\n");
                printf("0 to 255.\n\n");

                printf("----------------------------------------\n\n");

                printf("Memory is made from billions of tiny\n");
                printf("electronic cells.\n\n");

                printf("Each cell stores exactly ONE bit.\n\n");

                printf("Cell      Value\n");
                printf("-----------------\n");
                printf("[ ]  ->    0\n");
                printf("[#]  ->    1\n\n");

                printf("----------------------------------------\n\n");

                printf("Many bits together create:\n\n");

                printf("Bits -> Bytes -> Kilobytes -> Megabytes\n");
                printf("      -> Gigabytes -> Terabytes\n\n");

                printf("Everything in a computer is stored this way:\n");
                printf("- Text\n");
                printf("- Pictures\n");
                printf("- Music\n");
                printf("- Videos\n");
                printf("- Programs\n\n");

                printf("Memory keeps these bits until they\n");
                printf("are changed or deleted.\n");

                break;

            case 7:
                printf(
                "      HALF ADDER\n\n"
                " A ----\\\n"
                "        XOR ---- SUM\n"
                " B ----/\n"
                "\n"
                " A ----\\\n"
                "        AND ---- CARRY\n"
                " B ----/\n\n"
                );

                printf("HOW A CPU CALCULATES\n\n");

                printf("A processor performs calculations using\n");
                printf("logic gates.\n\n");

                printf("The simplest calculator is called a\n");
                printf("Half Adder.\n\n");

                printf("----------------------------------------\n\n");

                printf("Half Adder Truth Table\n\n");

                printf("A B | SUM CARRY\n");
                printf("----------------\n");
                printf("0 0 |  0    0\n");
                printf("0 1 |  1    0\n");
                printf("1 0 |  1    0\n");
                printf("1 1 |  0    1\n\n");

                printf("SUM comes from an XOR gate.\n");
                printf("CARRY comes from an AND gate.\n\n");

                printf("----------------------------------------\n\n");

                printf("Example:\n\n");

                printf("1 + 1\n\n");

                printf("SUM   = 0\n");
                printf("CARRY = 1\n\n");

                printf("Result = 10 (binary)\n\n");

                printf("----------------------------------------\n\n");

                printf("A Full Adder is built from two Half Adders.\n\n");

                printf(
                "        FULL ADDER\n\n"
                " A ----\\\n"
                "        HALF ----\\\n"
                " B ----/          \\\n"
                "                  HALF ---- SUM\n"
                "Cin --------------/\n\n"
                "Carry -----------------------> OUT\n\n"
                );

                printf("A Full Adder also receives a Carry Input\n");
                printf("from the previous calculation.\n\n");

                printf("By connecting millions of Full Adders,\n");
                printf("a CPU can add very large numbers.\n\n");

                printf("Every calculator and every processor\n");
                printf("uses this basic idea.\n");

                break;

            case 8:
                printf(
                "+------------+     ♪♫♪      27 + 15 = 42\n"
                "|\\          /|                x * y = z\n"
                "| \\        / |                2^8 = 256\n"
                "|  \\______/  |\n"
                "|  /      \\  |\n"
                "| /        \\ |\n"
                "|/__________\\|\n"
                "+------------+\n"
                );
                printf("\nEvery image, sound and calculation is ultimately represented as billions of 0s and 1s.\n");
                break;

            case 9:
                printf(
                "      Source Code\n"
                "           |\n"
                "           v\n"
                "   +----------------+\n"
                "   |   Compiler     |\n"
                "   +----------------+\n"
                "           |\n"
                "           v\n"
                "10110010100101100101...\n"
                "           |\n"
                "           v\n"
                "      Processor\n\n"
                );

                printf("HOW PROGRAMS WORK\n\n");

                printf("A program is just a list of instructions.\n\n");

                printf("Example in C:\n\n");

                printf("printf(\"Hello\");\n\n");

                printf("The processor cannot understand C code.\n");
                printf("It only understands machine code.\n\n");

                printf("----------------------------------------\n\n");

                printf("A compiler translates your code into\n");
                printf("binary instructions.\n\n");

                printf("Source code\n");
                printf("     |\n");
                printf("     v\n");
                printf("Compiler\n");
                printf("     |\n");
                printf("     v\n");
                printf("Machine code\n");
                printf("     |\n");
                printf("     v\n");
                printf("CPU executes instructions\n\n");

                printf("----------------------------------------\n\n");

                printf("Example machine code:\n\n");

                printf("10110100\n");
                printf("00101011\n");
                printf("11100010\n");
                printf("01001100\n\n");

                printf("These numbers tell the processor exactly\n");
                printf("what to do.\n\n");

                printf("Some instructions:\n");
                printf("- Add numbers\n");
                printf("- Copy memory\n");
                printf("- Compare values\n");
                printf("- Jump to another instruction\n");
                printf("- Print characters\n\n");

                printf("A processor simply executes one\n");
                printf("instruction after another millions or\n");
                printf("billions of times every second.\n\n");

                printf("Games, browsers and operating systems\n");
                printf("are all just enormous collections of\n");
                printf("machine instructions.\n");

                break;

            case 10:

            while (1)
            {
                int temp;
                int bits[64];
                int count = 0;
                int i;

                printf(
                "=========================================\n"
                "         BINARY PLAYGROUND\n"
                "=========================================\n\n");

                printf("Enter a decimal number (-1 to return): ");

                if (scanf("%d", &decimal) != 1)
                {
                    printf("\nIncorrect symbol.\n\n");

                    while (getchar() != '\n');

                    continue;
                }

                if (decimal == -1)
                    break;

                temp = decimal;

                if (temp == 0)
                {
                    printf("\nBinary: 0\n\n");
                    continue;
                }

                while (temp > 0)
                {
                    bits[count] = temp % 2;
                    temp = temp / 2;
                    count++;
                }

                printf("\nBinary: ");

                for (i = count - 1; i >= 0; i--)
                {
                    printf("%d", bits[i]);
                }

                printf("\n\n");
            }

            break;

            case 11:

            score = 0;

            printf("========== FINAL CHALLENGE ==========\n\n");

            printf("Question 1\n");
            printf("Convert 45 to binary:\n> ");
            scanf("%64s", answer);

            if (strcmp(answer, "101101") == 0)
            {
                printf("Correct!\n\n");
                score++;
            }
            else
            {
                printf("Wrong. Correct answer: 101101\n\n");
            }

            printf("Question 2\n");
            printf("Convert 1101 to decimal:\n> ");
            scanf("%d", &result);

            if (result == 13)
            {
                printf("Correct!\n\n");
                score++;
            }
            else
            {
                printf("Wrong. Correct answer: 13\n\n");
            }

            printf("Question 3\n");
            printf("1 AND 1 = ?\n> ");
            scanf("%d", &result);

            if (result == 1)
            {
                printf("Correct!\n\n");
                score++;
            }
            else
            {
                printf("Wrong.\n\n");
            }

            printf("Question 4\n");
            printf("1 XOR 1 = ?\n> ");
            scanf("%d", &result);

            if (result == 0)
            {
                printf("Correct!\n\n");
                score++;
            }
            else
            {
                printf("Wrong.\n\n");
            }

            printf("Question 5\n");
            printf("How many bits are in one byte?\n> ");
            scanf("%d", &result);

            if (result == 8)
            {
                printf("Correct!\n\n");
                score++;
            }
            else
            {
                printf("Wrong.\n\n");
            }

            printf("=====================================\n");
            printf("Your score: %d / 5\n", score);

            if (score == 5)
                printf("Excellent!\n");

            else if (score >= 3)
                printf("Good job!\n");

            else
                printf("Try again!\n");

            break;

            case 12:
                play_tetris();
                continue;   /* terminal уже восстановлен — не ждём двойной Enter */

            case 0:
                printf(
                " __  __ _    _ ______ ______ _____ _   _  _____ \n"
                "|  \\/  | |  | |  ____|  ____|_   _| \\ | |/ ____|\n"
                "| \\  / | |  | | |__  | |__    | | |  \\| | (___  \n"
                "| |\\/| | |  | |  __| |  __|   | | | . ` |\\___ \\ \n"
                "| |  | | |__| | |    | |     _| |_| |\\  |____) |\n"
                "|_|  |_|\\____/|_|    |_|    |_____|_| \\_|_____/ \n"
                "\n"
                " _____  _____   ____  _____  _    _  _____ _______ _____ ____  _   _  _____\n"
                "|  __ \\|  __ \\ / __ \\|  __ \\| |  | |/ ____|__   __|_   _/ __ \\| \\ | |/ ____|\n"
                "| |__) | |__) | |  | | |  | | |  | | |       | |    | || |  | |  \\| | (___  \n"
                "|  ___/|  _  /| |  | | |  | | |  | | |       | |    | || |  | | . ` |\\___ \\ \n"
                "| |    | | \\ \\| |__| | |__| | |__| | |____   | |   _| || |__| | |\\  |____) |\n"
                "|_|    |_|  \\_\\\\____/|_____/ \\____/ \\_____|  |_|  |_____\\____/|_| \\_|_____/ \n"
                "\n"
                );
                printf("\nGoodbye! Developed by Muffins Productions\n");
                return 0;

            default:
                printf("\nUnknown option.\n");
        }

        printf("\nPress Enter to return to menu...");
        getchar(); // съедает Enter после scanf
        getchar(); // ждет, пока пользователь нажмет Enter
    }
}
