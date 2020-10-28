
#ifndef SERIAL_H
#define SERIAL_H

int uart_open(const char *port);

int uart_open_no_block(const char *port);

void uart_close(int fd);

int uart_set(int fd, int speed, int flow_ctrl = 0, int databits = 8, int stopbits = 1, int parity = 'N');

int uart_recv(int fd, char *rcv_buf, int data_len);

int uart_send(int fd, char *send_buf, int data_len);

int uart_recv_no_block(int fd, char *rcv_buf, int data_len);

#endif // !SERIAL_H
