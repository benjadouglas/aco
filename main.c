#include <stdbool.h>
#include <stdio.h>
#include <termios.h>
#include <time.h>

#include "EasyPIO.h"
#include "header.h"

static struct termios orig_termios;

void enable_raw_mode();
void disable_raw_mode();
void turnOff();
int keyHit();
int delay(int);
void disp_binary(int);
extern void carga_bateria();
extern void ambulancia();
void autoFantastico();
void ledShow(unsigned char);
void menu();

const unsigned char led[] = {14, 15, 18, 23, 24, 25, 8, 7};
int delayTime[]           = {100000, 100000, 100000, 100000};

int main() {
    menu();
    return 0;
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
            case 2:
                ambulancia();
                return;
            case 3:
                carga_bateria();
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

void ledShow(unsigned char output) {
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
        if (keyHit()) {
            char c = getchar();
            printf("You pressed: %d	 \n", c);
            return 0;
        }
    }
    disable_raw_mode();
    return 1;
}

void enable_raw_mode() {
    struct termios raw;
    // 1. Get current terminal attributes
    tcgetattr(STDIN_FILENO, &orig_termios);
    // 2. Copy to modify
    raw = orig_termios;
    // 3. Turn off canonical mode and echo:
    //    ICANON: disable line buffering
    //    ECHO: disable echo
    raw.c_lflag &= ~(ICANON | ECHO);
    //    TCSANOW: apply changes immediately
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void disable_raw_mode() { tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios); }

/*
 * Returns 1 if a key has been pressed (and is waiting to read),
 * 0 otherwise. Doesn’t block.
 */
int keyHit() {
    struct timeval tv = {0L, 0L};
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    // select: check if stdin has data to read
    return select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) > 0;
}
