/*
 * Item.c
 *
 * Created: 2024-05-27 오전 10:21:15
 *  Author: kijun
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "Item.h"

void Select_Item(char item){
	PORTC = (PORTC & 0xF0) | item;
	_delay_ms(100);
}