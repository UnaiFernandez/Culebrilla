#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include "../include/defines.h"
#include "../include/logfile.h"

/*
 * Function to store the data read in a logfile
 */
void *logfile_func( void * param){

    int fd, i = 0;
    char sep[136] = "\n###################()##################\n";

    sleep(1);
    //Abrir el log.txt
    if((fd = open("log.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0){
        perror("[!] ERROR, opening log file:");
        exit(EXIT_FAILURE);
    }

    //Escribir el log.txt
    while(log_enable){
        i++;
        sem_wait(&sem);
                        
        sprintf(sep, "\n-----------------------------------------\n###################(%d)##################\n-----------------------------------------\n", i);
        write(fd, buff, sizeof(buff));
        write(fd, sep, sizeof(sep));
        usleep(delay);
    }
    return 0;
}
