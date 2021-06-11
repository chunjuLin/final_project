#include"mbed.h"
#include "bbcar.h"


Ticker servo_ticker;
Ticker encoder_ticker;
PwmOut pin5(D5), pin6(D6);
BufferedSerial xbee(D10, D9);
DigitalIn encoder(D11);
volatile int steps;
volatile int last;
DigitalInOut ping(D12);
Timer t;
Thread thread1, thread2, thread3, thread4, thread5, thread6;
float val;

BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx
BBCar car(pin5, pin6, servo_ticker);

char x;
int num[4] = {0, 0, 0, 0};
int i= 0, start = 0;
char pre;
int type;
int state = 0, pre_state = 0;
int coordination[11] = {0, 0, 0, 0, -400, -400, -400, -400, -400, -400, 0};
char buffer[10];

void encoder_control() {
   int value = encoder;
   if (!last && value) steps++;
   last = value;
}
void state0(){
    while(1){
        printf("000\n");
        int xd = (coordination[0] + coordination[1])/2;
        if(state == 0){
            if(xd >= 65) car.turn(15,0.1);
            else if(xd <= -55) car.turn(15,-0.1);
            else car.goStraight(20);
            if(pre_state == 1){
               if(val < 10 && coordination[10] > 80) {state = 3;} 
            }
            if(pre_state == 2){
                if(coordination[6] > -5){ 
                    car.stop();
                    state = 6;
                }
            }
            //printf("coor6 = %d\n", coordination[6]);
        }
    }
}
void state1(){
    while(1){
        printf("111\n");
        if(state == 1){
            steps = 0;
            last = 0;
            car.turn(40,-0.1);
            //printf("first1\n");
            ThisThread::sleep_for(2000ms);
            //printf("first2\n");
            car.stop();
            ThisThread::sleep_for(3000ms);
            steps = 0;
            last = 0;
            car.turn(90,0.4);
            while(steps*6.5*3.14/32 < 90) {
                printf("encoder2 = %d\r\n", steps);
                ThisThread::sleep_for(100ms);
            }
            car.stop();
            ThisThread::sleep_for(3000ms);
            state = 0;
            pre_state = 1;
            //printf("state = %d\n",state);
        }
    }
}
void state2(){
    while(1){
        printf("222\n");
        if(state == 2){
            printf("gogogogogogogogoogogogogogog\n");
            int xd = (coordination[0] + coordination[1])/2;
            if(xd >= 65) car.turn(15,0.1);
            else if(xd <= -55) car.turn(15,-0.1);
            else car.goStraight(20);
            if(val < 4 && coordination[10] > 90) {state = 3; printf("state = %d\n",state);}
        }
    }
}
void state3(){
    while(1){
        printf("333\n");
        if(state == 3){
            steps = 0;
            last = 0;
            car.turn(40,-0.1);
            //printf("first1\n");
            ThisThread::sleep_for(2000ms);
            //printf("first2\n");
            car.stop();
            state = 0;
            pre_state = 2;
            coordination[6] = -400;
            //printf("state = %d\n",state);
        }
    }
}
void state4(){
    while(1){
        printf("444\n");
        int xd = (coordination[0] + coordination[1])/2;
        if(state == 4){
            if(xd >= 65) car.turn(15,0.1);
            else if(xd <= -55) car.turn(15,-0.1);
            else car.goStraight(20);
            state = 6;
            //printf("state = %d\n",state);
        }
    }
}
void PING(){
    while(1){
        ping.output();
        ping = 0; wait_us(200);
        ping = 1; wait_us(5);
        ping = 0; wait_us(5);

        ping.input();
        while(ping.read() == 0);
        t.start();
        while(ping.read() == 1);
        val = t.read()*17700.4f;
        sprintf(buffer, "%f", val);
        xbee.write(buffer, sizeof(buffer));
        //if(val <= 5) state++;
        printf("Ping = %lf\r\n", val);
        t.stop();
        t.reset(); 
    }
}

int main(){
    //xbee.write("+++",3);
    encoder_ticker.attach(&encoder_control, 10ms);
    pc.set_baud(9600);
    uart.set_baud(9600);
    xbee.set_baud(9600);
    thread1.start(state0);
    thread2.start(state1);
    //thread3.start(state2);
    thread4.start(state3);
   // thread5.start(state4);
    //thread6.start(PING);

    while(1){
        // switch(state){
        //     case 0: xbee.write("state 0",7); break;
        //     case 1: xbee.write("state 1",7); break;
        //     case 2: xbee.write("state 2",7); break;
        //     default: xbee.write("nothing",7);
        // }
        
        if(uart.readable()){
            HERE:
            //printf("state = %d\n", state);
            char recv[1];
            uart.read(recv, sizeof(recv));
            //pc.write(recv, sizeof(recv));
            pre = x;
            x = *recv;
            //printf("%c",x);
            if(start == 1 && x!= '.'){
                num[i] = int(x)-48;
                //printf(" %c", x);
                i++;
            }
            if(x == ':'){
                //printf("state = %d\n", state);
                xbee.write("+++",3);
                //printf("+++\n");
                start = 1;
                //printf("pre = %c", pre);
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
                    case 'b': type = 10; break;
                    default: type = 11; 
                }
                if(type == 11) goto HERE;
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
                    //printf("\nC%d: %d\n",type,coordination[type]);
                    printf("state = %d\n", state);
                    num[0] = 0; num[1] = 0; num[2] = 0; num[3] = 0;
                }
                else{
                    if(i == 2) coordination[type] = num[1];
                    else if(i == 3) coordination[type] = 10*num[1] + num[2];
                    else if(i == 4) coordination[type] = 100*num[1] + 10*num[2] + num[3];
                    coordination[type] = coordination[type] *(-1);
                    //printf("%d\n",i);
                    //printf("num: %d %d %d\n", num[1], num[2], num[3]);
                    i = 0;
                    printf("C%d: %d\n",type,coordination[type]);
                    printf("state = %d\n", state);
                    if(coordination[6] > -5 && state == 0){state = 1; printf("state = %d\n",state);}
                    num[0] = 0; num[1] = 0; num[2] = 0; num[3] = 0;
                }
                
            }
  
        }
   }
}