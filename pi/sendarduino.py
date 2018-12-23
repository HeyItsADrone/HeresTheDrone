import serial
import sys
import time



def main():
    ard = serial.Serial('/dev/ttyACM0',9600)
    ard.flush()
    while True:
        whatToSend = input("what would you like to send the arduino?")
        ard.write(bytes(whatToSend.encode('ascii')))
        print ("Python value sent: ")
        print (whatToSend)
        
      
    
    
    
if __name__== "__main__":
    main()