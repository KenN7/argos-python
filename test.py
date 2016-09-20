#/usr/bin/python

def controlstep():
    #print("step")
    robot.wheels(1,5)

def init():
    robot.logprint("init")

def destroy():
    robot.logprint("destroy")

def reset():
    robot.logprint("reset")

