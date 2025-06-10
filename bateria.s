.text
.global carga_bateria
carga_bateria:

PUSH {R4, R5, R6, LR}

reset:
MOV R4, #1      // Inicializar con un solo LED encendido

main_loop:
MOV R0, R4
BL disp_binary  // Mostrar en pantalla

MOV R0, R4
BL ledShow      // Mostrar en LEDs

MOV R0, #3
BL delay
CMP R0, #0
BEQ turn_off    // Si delay retorna 0, apagar y salir

LSL R4, R4, #1  // Desplazar el patrón hacia la izquierda
CMP R4, #0x100  // Verificar si hemos llegado al final
BGE reset       // Si llegamos al final, reiniciar

B main_loop     // Continuar el ciclo principal

turn_off:
BL turnOff      // Apagar LEDs

POP {R4, R5, R6, PC}

.data
msg_carga:  .asciz "Carga de Bateria:\n"