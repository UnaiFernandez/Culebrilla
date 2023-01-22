#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <termios.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "../include/uart.h"
#include "../include/logfile.h"
#include "../include/defines.h"

sem_t sem;

int main(int argc, char ** argv){

    int c;
    char cont;
    int option_index = 0;
    static struct option long_options[] = {
        {"port",  required_argument, 0,  'p' },
        {"baud",  required_argument, 0,  'b' },
        {0,         0,         0,  0 }
    };
    pthread_t uart_read_th, uart_write_th, logfile_th;

    uart_t *uart;
    uart = malloc(sizeof(uart_t));

    printf("\033[1;32m");

    printf("  █████████                      ███            ████                                   \n "); 
    printf(" ███░░░░░███                    ░░░            ░░███                                   \n "); 
    printf("░███    ░░░   ██████  ████████  ████   ██████   ░███   ██████   ██████  █████████████  \n "); 
    printf("░░█████████  ███░░███░░███░░███░░███  ░░░░░███  ░███  ███░░███ ███░░███░░███░░███░░███ \n "); 
    printf(" ░░░░░░░░███░███████  ░███ ░░░  ░███   ███████  ░███ ░███ ░░░ ░███ ░███ ░███ ░███ ░███ \n "); 
    printf(" ███    ░███░███░░░   ░███      ░███  ███░░███  ░███ ░███  ███░███ ░███ ░███ ░███ ░███ \n "); 
    printf("░░█████████ ░░██████  █████     █████░░████████ █████░░██████ ░░██████  █████░███ █████\n ");
    printf(" ░░░░░░░░░   ░░░░░░  ░░░░░     ░░░░░  ░░░░░░░░ ░░░░░  ░░░░░░   ░░░░░░  ░░░░░ ░░░ ░░░░░ \n "); 

    printf("\n[*] Configuration:\n");
    printf("\033[0m");
    printf("\033[1;34m");
    while ((c = getopt_long (argc, argv, "p:b:", long_options, &option_index)) != -1){
        switch (c){
            case 'p':
                printf("\t-port: %s\n", optarg);
                uart->port = optarg;
                break;
            case 'b':
                printf("\t-baud: %s\n", optarg);
                uart->baudrate = optarg;
                break;
            case '?':
                printf("Unkown\n");
                break;
            default:
                abort ();
        }
    }
    printf("\033[1;32m");
    printf("[?] Do you want to continue? [y/n]");
    cont = getchar();

    if(cont == 'n'){
        printf("New Configuration:\n");
        printf("-----------------------------------\n");
        printf("\033[1;34m");
        printf("[!] Set port: ");
        scanf("%s", uart->port);
        printf("[!] Set baudrate: ");
        scanf("%s", uart->baudrate);
        printf("\033[1;32m");
    }

    //Configure serial port
    if((uart->fd = uart_config(*uart)) < 0){
        perror("[!] ERROR opening serial port:");
        exit(EXIT_FAILURE);
    }

    sleep(1);


    //Init semaphore
    sem_init(&sem, 0, 0);

    //Create threads
    printf("[*] Creating threads...\n");
    if(pthread_create(&uart_read_th, NULL, uart_read_func, (void *)uart) > 0){
        perror("[!] ERROR: creating uart thread.");
        return EXIT_FAILURE;
    }
    printf("[OK] uart_read_th created.\n");
    if(pthread_create(&uart_write_th, NULL, uart_write_func, (void *)uart) > 0){
        perror("[!] ERROR: creating logfile thread.");
        return EXIT_FAILURE;
    }
    printf("[OK] uart_write_th created.\n");
    if(pthread_create(&logfile_th, NULL, logfile_func, NULL) > 0){
        perror("[!] ERROR: creating logfile thread.");
        return EXIT_FAILURE;
    }
    printf("[OK] logfile_th created.\n");

    printf("\033[0m");

    //Join threads 
    pthread_join(uart_read_th, NULL);
    pthread_join(uart_write_th, NULL);
    pthread_join(logfile_th, NULL);

    //Destroy semaphore
    sem_destroy(&sem);
    return 0;
}


