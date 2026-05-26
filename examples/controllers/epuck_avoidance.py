WHEEL_SPEED = 5.0


def init():
    pass


def controlstep():
    readings = robot.epuck_proximity.get_readings()
    front_indices = [0, 1, 7, 6]
    strongest = max(front_indices, key=lambda i: readings[i].value)

    if readings[strongest].value > 0.0:
        if strongest in (0, 1):
            robot.epuck_wheels.set_speed(WHEEL_SPEED, 0)
        else:
            robot.epuck_wheels.set_speed(0, WHEEL_SPEED)
    else:
        robot.epuck_wheels.set_speed(WHEEL_SPEED, WHEEL_SPEED)


def reset():
    pass


def destroy():
    pass
