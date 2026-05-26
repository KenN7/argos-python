import math

MAX_SPEED = 10.0
HARD_TURN = math.radians(90)
SOFT_TURN = math.radians(70)
NO_TURN = math.radians(10)
TARGET_DISTANCE = 75.0
GAIN = 1000.0
EXPONENT = 2.0

turning = "none"


def init():
    robot.colored_blob_omnidirectional_camera.enable()
    robot.leds.set_single_color(12, "red")


def controlstep():
    heading = _add(_vector_to_light(), _flocking_vector())
    _set_wheel_speeds(heading)


def reset():
    global turning
    turning = "none"
    robot.colored_blob_omnidirectional_camera.enable()
    robot.leds.set_single_color(12, "red")


def destroy():
    pass


def _vector_to_light():
    x = 0.0
    y = 0.0
    for reading in robot.light_sensor.get_readings():
        angle = reading.angle.value()
        x += reading.value * math.cos(angle)
        y += reading.value * math.sin(angle)

    length = math.hypot(x, y)
    if length == 0.0:
        return (0.0, 0.0)

    scale = 0.25 * MAX_SPEED / length
    return (x * scale, y * scale)


def _flocking_vector():
    x = 0.0
    y = 0.0
    seen = 0

    for color, angle, distance in robot.colored_blob_omnidirectional_camera.get_readings():
        if color != [255, 0, 0] or distance >= TARGET_DISTANCE * 1.8:
            continue
        force = _lennard_jones(distance)
        x += force * math.cos(angle)
        y += force * math.sin(angle)
        seen += 1

    if seen == 0:
        return (0.0, 0.0)

    x /= seen
    y /= seen
    length = math.hypot(x, y)
    if length > MAX_SPEED:
        scale = MAX_SPEED / length
        return (x * scale, y * scale)
    return (x, y)


def _lennard_jones(distance):
    if distance <= 0:
        return 0.0
    normalized = (TARGET_DISTANCE / distance) ** EXPONENT
    return -GAIN / distance * (normalized * normalized - normalized)


def _set_wheel_speeds(heading):
    global turning
    x, y = heading
    angle = math.atan2(y, x)
    speed = min(math.hypot(x, y), MAX_SPEED)

    if turning == "hard" and abs(angle) <= SOFT_TURN:
        turning = "soft"
    if turning == "soft":
        if abs(angle) > HARD_TURN:
            turning = "hard"
        elif abs(angle) <= NO_TURN:
            turning = "none"
    if turning == "none":
        if abs(angle) > HARD_TURN:
            turning = "hard"
        elif abs(angle) > NO_TURN:
            turning = "soft"

    if turning == "none":
        left = right = speed
    elif turning == "soft":
        factor = (HARD_TURN - abs(angle)) / HARD_TURN
        slow = speed * factor
        fast = speed + speed * (1.0 - factor)
        left, right = (slow, fast) if angle > 0 else (fast, slow)
    else:
        left, right = (-MAX_SPEED, MAX_SPEED) if angle > 0 else (MAX_SPEED, -MAX_SPEED)

    robot.wheels.set_speed(left, right)


def _add(a, b):
    return (a[0] + b[0], a[1] + b[1])
