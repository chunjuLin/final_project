#include"mbed.h"
#include "bbcar.h"


Ticker servo_ticker;
Ticker encoder_ticker;
PwmOut pin5(D5), pin6(D6);
DigitalIn encoder(D11);
volatile int steps;
volatile int last;

BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx
BBCar car(pin5, pin6, servo_ticker);

char x;
int num[4] = {0, 0, 0, 0};
int i= 0, start = 0;
char pre;
int type;
int state = 0;
int coordination[10] = {0, 0, 0, 0, -400, -400, -400, -400, -400, -400};

void encoder_control() {
   int value = encoder;
   if (!last && value) steps++;
   last = value;
}

int main(){
    encoder_ticker.attach(&encoder_control, 10ms);
    pc.set_baud(9600);
    uart.set_baud(9600);
   while(1){
        //printf("hi\n");
        if(uart.readable()){
            char recv[1];
            uart.read(recv, sizeof(recv));
            //pc.write(recv, sizeof(recv));
            x = *recv;
            printf("%c ",x);
            if(start == 1 && x!= '.'){
                num[i] = int(x)-48;
               // printf("num[%d] = %d\n", i, num[i]);
                i++;
            }
            if(x == ':'){
                start = 1;
                switch(pre){
                    case '1': type = 0; break;
                    case '2': type = 1; break;
                    case '3': type = 2; break;
                    case '4': type = 3; break;
                    case '5': type = 4; break;
                    case '6': type = 5; break;
                    case '7': type = 6; break;
                    case '8': type = 7; break;
                    case '9': type = 8; break;
                    case 'a': type = 9; break;
                    default: type = 0; break; 
                }
            }
            if(x == '.'){
                start = 0;
                if(num[0] != -3){
                    if(i == 1) coordination[type] = num[0];
                    else if(i == 2) coordination[type] = 10*num[0] + num[1];
                    else if(i == 3) coordination[type] = 100*num[0] + 10*num[1] + num[2];
                    //printf("%d\n",i);
                    //printf("num: %d %d %d\n", num[0], num[1], num[2]);
                    i = 0;
                    printf("\nC%d: %d\n",type,coordination[type]);
                    num[0] = 0; num[1] = 0; num[2] = 0; num[3] = 0;
                }
                else{
                    if(i == 2) coordination[type] = num[1];
                    else if(i == 3) coordination[type] = 10*num[1] + num[2];
                    else if(i == 4) coordination[type] = 100*num[1] + 10*num[2] + num[3];
                    coordination[type] = coordination[type] *(-1);
                    //printf("%d\n",i);
                    //printf("num: %d %d %d\n", num[0], num[1], num[2]);
                    i = 0;
                    printf("\nC%d: %d\n",type,coordination[type]);
                    num[0] = 0; num[1] = 0; num[2] = 0; num[3] = 0;
                }
            }
            //printf("%c ",x1);
            pre = x;
            //printf("66666: %d\n",coordination[6]);
            int xd = (coordination[0] + coordination[1])/2;
            if(coordination[6] > -5 && state == 0) state = 1; 
            if(state == 0){
                if(xd >= 65) car.turn(15,0.1);
                else if(xd <= -55) car.turn(15,-0.1);
                else car.goStraight(20);
            }
            if(state == 1){
                steps = 0;
                last = 0;
                car.turn(40,-0.1);
                ThisThread::sleep_for(2000ms);
                car.stop();
                ThisThread::sleep_for(3000ms);
                steps = 0;
                last = 0;
                car.turn(40,0.4);
                while(steps*6.5*3.14/32 < 80) {
                    printf("encoder2 = %d\r\n", steps);
                    ThisThread::sleep_for(100ms);
                }
                car.stop();
                ThisThread::sleep_for(3000ms);
                state = 2;
            }
                // ThisThread::sleep_for(3000ms);
                // steps = 0;
                // last = 0;
                // car.goStraight(100);
                // while(steps*6.5*3.14/32 < 10) {
                //     //printf("encoder1 = %d\r\n", steps);
                //     ThisThread::sleep_for(100ms);
                // }
                // car.stop();
                // ThisThread::sleep_for(3000ms);
                // steps = 0;
                // last = 0;
                // car.turn(200,-0.1);
                // while(steps*6.5*3.14/32 < 7) {
                //     printf("encoder2 = %d\r\n", steps);
                //     ThisThread::sleep_for(100ms);
                // }
                // car.stop();
                // ThisThread::sleep_for(3000ms);
                // steps = 0;
                // last = 0;
                // car.goStraight(100);
                // while(steps*6.5*3.14/32 < 10) {
                //     printf("encoder3 = %d\r\n", steps);
                //     ThisThread::sleep_for(100ms);
                // }
                // car.stop();
                // ThisThread::sleep_for(3000ms);
                // steps = 0;
                // last = 0;
                // car.turn(200,0.1);
                // while(steps*6.5*3.14/32 < 7) {
                //     printf("encoder2 = %d\r\n", steps);
                //     ThisThread::sleep_for(100ms);
                // }
                // car.stop();
                // ThisThread::sleep_for(3000ms);
                // steps = 0;
                // last = 0;
                // car.goStraight(100);
                // while(steps*6.5*3.14/32 < 10) {
                //     printf("encoder3 = %d\r\n", steps);
                //     ThisThread::sleep_for(100ms);
                // }
                // car.stop();
      }
   }
}