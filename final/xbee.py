import serial
import time

# XBee setting
serdev = '/dev/ttyUSB0'
s = serial.Serial(serdev, 9600)

print("start !!")
while True:
    # send RPC to remote
    # s.write("hi\r\n".encode())
    message = s.read(7)
    print(message.decode())
    #print("\n")
    # time.sleep(1)
    
s.close()
