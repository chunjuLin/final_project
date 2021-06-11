import serial
import time

# XBee setting
serdev = '/dev/ttyUSB0'
s = serial.Serial(serdev, 9600)

print("start !!")
while True:
    print("state 0")
    time.sleep(10)
    print("state 1")
    time.sleep(5)
    print("state 2")
    # send RPC to remote
    # s.write("hi\r\n".encode())
    message = s.read(10)
    print(message.decode())
    print("\n")
    # time.sleep(1)
    
s.close()