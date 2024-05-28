/*
 * Item.h
 *
 * Created: 2024-05-27 오전 10:02:12
 *  Author: kijun
 */ 


#ifndef ITEM_H_
#define ITEM_H_

//Demux
#define ITEM_NONE		0x03	//0000 0011
#define ITEM_SERVO		0x02	//0000 0010
#define ITEM_SPEAKER	0x0E	//0000 1110
#define ITEM_LED_RED	0x09	//0000 1001
#define ITEM_LED_GREEN	0x0D	//0000 1101
#define ITEM_LED_BLUE	0x01	//0000 0001

void Select_Item(char item);



#endif /* ITEM_H_ */