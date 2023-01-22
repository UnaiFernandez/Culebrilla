#ifndef _uart_h
#define _uart_h

typedef struct uart{
    int fd;
    char * port;
    char * baudrate;
    struct termios config;
}uart_t;

int uart_config(uart_t uart);
int uart_read(int fd, char * buffer);
int uart_write(int fd, char * buffer);
void uart_flush(int fd, char * buff);
void parse_data(char * data);
void *uart_read_func(void * param);
void *uart_write_func(void * param);

#endif
