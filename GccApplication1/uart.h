/*
 * uart.h
 *
 * Created: 2024-06-18 오후 3:41:09
 *  Author: kijun
 */ 


#ifndef UART_H_
#define UART_H_

//#define UBRR 103

void USART0_NUM(int num);
void USART0_TX_vect(unsigned char data);
void UART_init();



#endif /* UART_H_ */