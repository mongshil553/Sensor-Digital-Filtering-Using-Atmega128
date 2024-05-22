#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

//블루투스 통신
void init_BT(void);
void BT_send(char msg);
char BT_Receive(); //#x. : Color, !xxx. : posX, *xxx. : posY
volatile char rdata; //read buffer from BT
volatile char rdatas[12];



#endif /* BLUETOOTH_H_ */