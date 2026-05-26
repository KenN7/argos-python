counter = 0


def init():
    pass


def controlstep():
    global counter
    counter += 1

    if counter < 70:
        robot.wheels.set_speed(5, 5)
    elif counter == 70:
        robot.wheels.set_speed(0, 0)
        robot.gripper.lock()
    elif counter < 120:
        robot.wheels.set_speed(-5, -5)
    elif counter < 170:
        robot.gripper.unlock()
        robot.wheels.set_speed(-5, -5)
    else:
        robot.wheels.set_speed(0, 0)


def reset():
    global counter
    counter = 0


def destroy():
    pass
