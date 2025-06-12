#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

void reset_terminal() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios); }

void enable_raw_mode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(reset_terminal);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);  // Disable echo and canonical mode
    raw.c_cc[VMIN]  = 0;              // Return immediately
    raw.c_cc[VTIME] = 0;              // No timeout

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int kbhit() {
    fd_set read_fds;
    struct timeval timeout;

    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);

    timeout.tv_sec  = 0;
    timeout.tv_usec = 0;

    return select(STDIN_FILENO + 1, &read_fds, NULL, NULL, &timeout) > 0;
}

char getch() {
    char ch;
    read(STDIN_FILENO, &ch, 1);
    return ch;
}

int main() {
    enable_raw_mode();

    int speed   = 5;
    int running = 1;

    printf("Program running! Use arrow keys to control speed, 'q' to quit.\n");
    printf("Speed: %d\n", speed);

    while (running) {
        // Check for key press
        if (kbhit()) {
            char key = getch();

            // Handle escape sequences for arrow keys
            if (key == 27) {  // ESC sequence
                if (kbhit()) {
                    char next = getch();
                    if (next == '[' && kbhit()) {
                        char arrow = getch();
                        switch (arrow) {
                            case 'A':  // Up arrow
                                speed++;
                                printf("\rSpeed increased: %d  ", speed);
                                fflush(stdout);
                                break;
                            case 'B':  // Down arrow
                                if (speed > 1)
                                    speed--;
                                printf("\rSpeed decreased: %d  ", speed);
                                fflush(stdout);
                                break;
                            case 'C':  // Right arrow
                                printf("\rRight arrow pressed  ");
                                fflush(stdout);
                                break;
                            case 'D':  // Left arrow
                                printf("\rLeft arrow pressed  ");
                                fflush(stdout);
                                break;
                        }
                    }
                }
            } else {
                switch (key) {
                    case 'q':
                    case 'Q':
                        running = 0;
                        break;
                    case '+':
                        speed++;
                        printf("\rSpeed: %d  ", speed);
                        fflush(stdout);
                        break;
                    case '-':
                        if (speed > 1)
                            speed--;
                        printf("\rSpeed: %d  ", speed);
                        fflush(stdout);
                        break;
                    default:
                        printf("\rKey pressed: %c (ASCII: %d)  ", key, key);
                        fflush(stdout);
                        break;
                }
            }
        }

        // Simulate work based on speed
        // You can adjust the sleep time based on speed
        // usleep(1000000 / speed); // Adjust timing based on speed
    }

    printf("\nProgram ended.\n");
    return 0;
}
