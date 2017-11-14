import threading, time
import RPi.GPIO as GPIO


def play_with_lights( lightName, lightPin, delay):
    count = 0
    GPIO.output(lightPin, False)
    
    while(count<5):
        print("%s: %s" % ( lightName, "ON"))
        GPIO.output(lightPin, True)
        time.sleep(delay)
        print("%s: %s" % ( lightName, "OFF "))
        GPIO.output(lightPin, False)
        time.sleep(delay)
        count += 1

# assign pins by GPIO number given on pinout sheet
pinOne = 21
pinTwo = 4
pinThree = 17
pinFour = 27

# Turn off warnings saying that the pin is being used by another process
# this seems to happen when the program is completed executing and you're running it again
GPIO.setwarnings(False)

#Set mode to GPIO.BCM 
GPIO.setmode(GPIO.BCM)
GPIO.setup(pinOne, GPIO.OUT)
GPIO.setup(pinTwo, GPIO.OUT)
GPIO.setup(pinThree, GPIO.OUT)
GPIO.setup(pinFour, GPIO.OUT)

threadone= threading.Thread(target=play_with_lights, args=( "Light GREEN", pinOne , 1))
threadtwo= threading.Thread(target=play_with_lights, args=( "Light RED", pinTwo , .9))
threadthree= threading.Thread(target=play_with_lights, args=( "Light BLUE", pinThree , .8))
threadfour = threading.Thread(target=play_with_lights, args=( "Light YELLOW", pinFour , .7))

# Three LEDS start until threadone is complete
threadone.start()
threadtwo.start()
threadthree.start()
threadfour.start()

# Once threadone is complete it will continue in the code and start threadtwo
threadone.join()

threadtwo.start()

threadtwo.join()
threadthree.join()
threadfour.join()

    
        
