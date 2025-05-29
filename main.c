#include <stdio.h>

void autofantastico();
void menu();

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
      autofantastico();
      return;
    default:
      printf("Ingrese un numero válido\n");
    }
  }
}

void autofantastico() {
  printf("Autofantastico!\n");
  return;
}
