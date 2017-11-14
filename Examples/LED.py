import RPi.GPIO as GPIO
import time

on = True;
off = False;
led = 21;
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.setup(led,GPIO.OUT)
print("LED on")
GPIO.output(led,on)
time.sleep(5)
print("LED off")
GPIO.output(led,off)
