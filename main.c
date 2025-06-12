#include <stdbool.h>
#include <stdio.h>
#include <termios.h>
#include <time.h>

#include "EasyPIO.h"
// #include "header.h"

static struct termios orig_termios;

void reset_terminal() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios); }
void enable_raw_mode();
int kbHit();
void turnOff();
int delay(int);
void disp_binary(int);
// extern void carga_bateria();
// extern void ambulancia();
void autoFantastico();
void showLed(unsigned char);
void menu();
void keys();

const unsigned char led[] = {14, 15, 18, 23, 24, 25, 8, 7};
int delayTime[]           = {100000, 100000, 100000, 100000};

char getch() {
    char ch;
    read(STDIN_FILENO, &ch, 1);
    return ch;
}

int main() {
    menu();
    return 0;
}

void keys() {
    enable_raw_mode();
    printf("Program running! Use arrow keys to control speed, 'q' to quit.\n");
    while (1) {
        if (kbHit()) {
            char c = getch();
            if (c == 27) {
                if (kbHit()) {
                    char next = getch();
                    if (next == '[' && kbHit()) {
                        char arrow = getch();
                        switch (arrow) {
                            case 'A':
                                printf("\rSpeed increased, arrow UP (%c)  ",
                                       arrow);
                                fflush(stdout);
                                break;
                            case 'B':
                                printf("\rSpeed decreased, arrow DOWN (%c)",
                                       arrow);
                                fflush(stdout);
                                break;
                        }
                    }
                }
            } else {
                if (c == 'q' || c == 'Q') {
                    printf("\rExit                   ");
                    fflush(stdout);
                    break;
                } else {
                    printf("\rYou pressed: %c (ASCII: %d)", c, c);
                    fflush(stdout);
                }
            }
        }
    }
    reset_terminal();
    return;
}

void menu() {
    int opcion;
    printf("Ingrese un numero\n");
    while (1) {
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                autoFantastico();
                return;
            // case 2:
            //     ambulancia();
            //     return;
            case 3:
                // carga_bateria();
                return;
            default:
                printf("Ingrese un numero válido\n");
        }
    }
}

void autoFantastico() {
    printf("Autofantastico!\n");
    unsigned char output;
    while (1) {
        output = 0x80;  // 80 hex == 128 decimal
        // ledShow(output);
        for (int i = 0; i < 8; i++) {
            disp_binary(output);
            output = output >> 1;
            if (delay(1000) == 0) {
                return;
                // Call turnoff function
            }
        }

        // output = 0x2;
        // for (int i = 0; i < 6; i++) {
        //     // ledShow(output);
        //     disp_binary(output);
        //     output = output << 1;
        //     if (delay(0) == 0) {
        //         return;
        //     }
        // }
    };
    return;
}

void disp_binary(int i) {
    for (int t = 128; t > 0; t = t / 2)
        if (i == t)  // si i y t son iguales
            printf("* ");
        else
            printf("_ ");
    fflush(stdout);
    printf("\r");
}

void showLed(unsigned char output) {
    for (int j = 0; j < 8; j++) {
        digitalWrite(led[j], (output >> j) & 1);
    };
    return;
}

int delay(int d) {
    int milli_seconds = 1000 * d;
    enable_raw_mode();
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds) {
        if (kbHit()) {
            char c = getchar();
            printf("You pressed: %d	 \n", c);
            return 0;
        }
    }
    reset_terminal();
    return 1;
}

void enable_raw_mode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(reset_terminal);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);  // Disable echo and canonical mode
    raw.c_cc[VMIN]  = 0;              // Return immediately
    raw.c_cc[VTIME] = 0;              // No timeout

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

/*
 * Returns 1 if a key has been pressed (and is waiting to read),
 * 0 otherwise. Doesn’t block.
 */
int kbHit() {
    struct timeval tv = {0L, 0L};
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    // select: check if stdin has data to read
    return select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) > 0;
}

void turnOff() {
    unsigned char off = 0x0;
    showLed(off);
}
