#ifndef UART_H
#define UART_H

#include <xc.h>
#include <stdio.h>
#include <stdbool.h>

#define STR_MAX 100

// 全域變數宣告 (讓 main 可以存取)
extern char uart_buffer[STR_MAX];
extern int uart_buffer_size;

void uart_initialize(void);

void uart_clear_buffer(void);
void uart_read(void);
int uart_get_string(char *str); // 在中斷中呼叫

// 覆寫 putch 讓 printf 運作
void putch(char data);

#endif