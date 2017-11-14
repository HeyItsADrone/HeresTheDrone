import threading, time

def print_time( threadName, delay):
    count = 0
    while count <5:
        time.sleep(delay)
        count += 1
        print("%s: %s" % ( threadName, time.ctime(time.time())))



threadone= threading.Thread(target=print_time, args=("Thread One", 1, ))
threadtwo= threading.Thread(target=print_time, args=("Thread Two", 2, ))
threadone.start()
threadtwo.start()

threadone.join()
threadtwo.join()

print("end")
 

