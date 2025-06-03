#include <stdbool.h>
#include <stdio.h>
#include <termios.h>

#include "EasyPIO.h"

struct termios modifyTerminalConfig(void);
void turnOff();
void restoreTerminalConfig(struct termios);
bool keyHit(int);
int delay(int);
void disp_binary(int);
void ledShow(unsigned char);
void autoFantastico();
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
            default:
                printf("Ingrese un numero válido\n");
        }
    }
}

void autoFantastico() {
    printf("Autofantastico!\n");
    while (1) {
        unsigned char output = 0x80;
        for (int i = 0; i < 8; i++) {
            // ledShow(output);
            disp_binary(output);
            output = output >> 1;
        };
        output = 0x2;
        for (int i = 0; i < 6; i++) {
            // ledShow(output);
            disp_binary(output);
            output = output << 1;
            if (delay(0) == 0) {
                turnOff();
                return;
            }
        }
    }
    return;
}

void disp_binary(int i) {
    int t;
    for (t = 128; t > 0; t = t / 2)
        if (i & t)
            printf("1 ");
        else
            printf("0 ");
    fflush(stdout);
    printf("\r");
}

void ledShow(unsigned char output) {
    for (int j = 0; j < 8; j++) {
        digitalWrite(led[j], (output >> j) & 1);
    };
    return;
}

int delay(int index) {
    int i;
    for (i = delayTime[index]; i > 0; --i) {
        if (keyHit(index)) {
            return 0;
        }
    }
    return 1;
}

bool keyHit(int index) {
    struct termios oldattr = modifyTerminalConfig();
    int ch, oldf;

    // setea el archivo descriptor del input estandar a que no se bloquee
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    // intenta leer un caracter de un input estandar
    ch = getchar();

    // cuando se presiona U
    if (ch == 117) {  // ASCII para u
        if (delayTime[index] > 1000) {
            delayTime[index] = delayTime[index] - 1000;
        }
    }

    // cuando se presiona D
    if (ch == 100) {  // ASCII para d
        delayTime[index] = delayTime[index] + 1000;
    }

    restoreTerminalConfig(oldattr);

    // Restablece el archivo a modo descriptor
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    // si se presiona escape devuelve 1
    if (ch == 27) {
        ungetc(ch, stdin);
        return 1;
    }

    // si escape no se presiona devuelve 0
    return 0;
}

void restoreTerminalConfig(struct termios oldattr) {
    // restablece los atributos originales a la terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
}

void turnOff() {
    unsigned char off = 0x0;
    ledShow(off);
}

struct termios modifyTerminalConfig(void) {
    struct termios oldattr, newattr;

    // obtiene los atributos de la terminal
    tcgetattr(STDIN_FILENO, &oldattr);

    // copia los atributos ya existenes a los nuevos
    newattr = oldattr;

    // desabilita el modo canonico y echo
    newattr.c_lflag &= ~(ICANON | ECHO);

    // aplica los nuevos atributos a la terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);

    return oldattr;
}
