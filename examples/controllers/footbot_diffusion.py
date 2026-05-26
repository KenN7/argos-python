import math

ALPHA = math.radians(7.5)
DELTA = 0.1
WHEEL_SPEED = 5.0


def init():
    pass


def controlstep():
    # Average the proximity readings into one obstacle vector.
    x = 0.0
    y = 0.0
    readings = robot.proximity.get_readings()

    for reading in readings:
        angle = reading.angle.value()
        x += reading.value * math.cos(angle)
        y += reading.value * math.sin(angle)

    length = math.hypot(x, y) / max(1, len(readings))
    angle = math.atan2(y, x)

    if -ALPHA <= angle <= ALPHA and length < DELTA:
        robot.wheels.set_speed(WHEEL_SPEED, WHEEL_SPEED)
    elif angle > 0:
        robot.wheels.set_speed(WHEEL_SPEED, 0)
    else:
        robot.wheels.set_speed(0, WHEEL_SPEED)


def reset():
    pass


def destroy():
    pass
