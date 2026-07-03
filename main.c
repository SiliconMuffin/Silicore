#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
                
            score=0;

            printf("========== FINAL CHALLENGE ==========\n\n");

            printf("Question 1\n");
            printf("Convert 45 to binary:\n> ");
            scanf("%64s",answer);

            if(strcmp(answer,"101101")==0)
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
            scanf("%d",&result);

            if(result==13)
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
            scanf("%d",&result);

            if(result==1)
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
            scanf("%d",&result);

            if(result==0)
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
            scanf("%d",&result);

            if(result==8)
            {
                printf("Correct!\n\n");
                score++;
            }
            else
            {
                printf("Wrong.\n\n");
            }

            printf("=====================================\n");
            printf("Your score: %d / 5\n",score);

            if(score==5)
                printf("Excellent!\n");

            else if(score>=3)
                printf("Good job!\n");

            else
                printf("Try again!\n");

            break;
                
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
