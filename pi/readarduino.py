import serial
import time

def main():
    ard = serial.Serial('/dev/ttyACM0',9600)
    ard.flush()
    while True:
        if(ard.inWaiting()>0):
            myData = ard.readline()
            print ("Message from arduino")
            print(myData.decode('ascii'))
        time.sleep(1)

if __name__== "__main__":
    main()