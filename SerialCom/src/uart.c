#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <termios.h>
#include <string.h>
#include <semaphore.h>
#include <curses.h>
#include "../include/uart.h"
#include "../include/defines.h"
#include "../include/logfile.h"


char buff [BUFFSIZE];
int log_enable = 1;
int dataarr[4];


/*
 * Function to configure serial port
 * 
 * Baudrate: B115200
 * Bit no.: 8
 * Parity: No
 * Stop bit: Yes
 */
int uart_config(uart_t uart){

    //Open /dev/ttyACM0
    if((uart.fd = open(uart.port, O_RDWR | O_NOCTTY | O_NDELAY)) < 0){
        perror("[!] ERROR: ");
        return -1;
    }

    //Check if its a tty
    if(!isatty(uart.fd)){
        perror("[!] ERROR (not tty): ");
        return -1;
    }

    //copy the config to uart.config
    if(tcgetattr(uart.fd, &uart.config) < 0){
        perror("[!] ERROR (get attr): ");
        return -1;
    }

    uart.config.c_cflag |= atoi(uart.baudrate);	//baudrate
                                                //uart.config.c_cflag |= B115200;
    uart.config.c_iflag |= IGNPAR;		//no parity
    uart.config.c_cflag |= CS8;			//8 bit


    uart.config.c_cflag |= CREAD;		//Enable receiver
    uart.config.c_lflag &= ~(ICANON);		//Enable canonical mode

    uart.config.c_cc[VTIME] = 0;
    uart.config.c_cc[VMIN] = 1;

    if(tcsetattr(uart.fd, TCSAFLUSH, &uart.config) < 0){
        perror("[!] ERROR (set attr): ");
        return -1;
    }

    printf("[OK] Configuration succeedded!\n");
    usleep(100);
    return uart.fd;

}

/*
 * Function to read from serial port
 */
int uart_read (int fd, char * buff){
    return read(fd, buff, BUFFSIZE);
}

/*
 * Function to write in the serial port
 */
int uart_write (int fd, char * buffer){
    return write(fd, buffer, sizeof(buffer));
}


/*
 * Function to flush serial port
 */
void uart_flush(int fd, char * buffer){
    int c;
    while((c = read(fd, buffer, sizeof(buffer)) != EOF));
}

void parse_data(char * data){
    char * token;
    int d, i = 0;
    //int dataarr [3];
    token = strtok(data, ",");
    while (token != NULL) {
        d = atoi(token);
        dataarr[i] = d;
        //printf("%d\r\n", dataarr[i]);
        i++;
        token = strtok(NULL, ",");
    }
}

/*
 * Thread function
 *
 * It configures the serial port and reads data from it
 */
void *uart_read_func (void * param){
    uart_t *uart;
    uart = (uart_t*)param;
    char data[BUFFSIZE];
    char * ctl;
    sleep(1);
    while(1){
        //read from /dev/ttyACM0
        if(uart_read(uart->fd, buff) > 0){
            //memcpy(buff+45, uart->port, 12*sizeof(char));
            //memcpy(buff+84, uart->baudrate, 6*sizeof(char));


            system("clear");
            strcpy(data, buff);
            parse_data(data);
            if(dataarr[3] == 0){
                ctl = "TECLAS";
            }
            else {
                ctl = "JOYSTICK";
            }
            //printf ("%s\r\n", buff);
            //printf("     STATUS       |      GAME      |\r\nPort:              | Points:    %d |\r\nBaud:       	   | x:         %d |\r\nControl: uart      | y:     %d      |\r\n", dataarr[0], dataarr[1], dataarr[2]);
            printf("\033[1;32m-------------------------- [GAME] --------------------------\033[0m\r\n\r\n\033[1;34mPoints:\033[0m %d              \033[1;34mx:\033[0m %d               \033[1;34my:\033[0m  %d          \r\n\r\n\033[1;32m-------------------------- [INFO] --------------------------\033[0m\r\n\033[1;34m-Port:\033[0m %s\r\n\033[1;34m-Baudrate: \033[0m%s\r\n\033[1;34m-Control: \033[0m%s\r\n\033[0;31m[!] Press q to exit.\r\n", dataarr[0],dataarr[1],dataarr[2], uart->port, uart->baudrate, ctl);
        }
        sem_post(&sem);
        usleep (delay);
    }

    sem_post(&sem);
    //Disable log
    log_enable = 0;

    close(uart->fd);
    return 0;
}


/*
 * Thread function
 *
 * It configures the serial port and reads data from it
 */
void *uart_write_func (void * param){
    uart_t *uart;
    uart = (uart_t*)param;
    char t[3];

    sleep(1);
    while(1){
        initscr();
        cbreak();
        noecho();
        usleep(delay);
        keypad(stdscr, TRUE);

        switch (getch()) {
            case 'a':
                sprintf(t, "le");
                uart_write(uart->fd, t);
                break;
            case 's':
                sprintf(t, "do");
                uart_write(uart->fd, t);
                break;
            case 'd':
                sprintf(t, "ri");
                uart_write(uart->fd, t);
                break;
            case 'w':
                sprintf(t, "up");
                uart_write(uart->fd, t);
                break;
            case 'q':
                endwin();
                exit(0);
                break;

        }
        endwin();
    }

    exit(0);
    close(uart->fd);
}
