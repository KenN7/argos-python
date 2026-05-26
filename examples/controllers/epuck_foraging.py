import random


WHEEL_SPEED = 5.0
TURN_SPEED = 3.5
AVOID_THRESHOLD = 0.03
CLOSE_THRESHOLD = 0.18
REVERSE_STEPS = 5

turn_steps = 0
turn_direction = 1
reverse_steps = 0
escape_turn_steps = 0
escape_turn_direction = 1


def _set_leds(color):
    robot.epuck_leds.set_all_colors(color)


def init():
    global turn_steps, turn_direction
    global reverse_steps, escape_turn_steps, escape_turn_direction

    random.seed(robot.variables.get_id())
    turn_steps = 0
    turn_direction = 1
    reverse_steps = 0
    escape_turn_steps = 0
    escape_turn_direction = 1

    robot.variables.set_attribute("harvest", "0")
    robot.variables.set_attribute("carrying", "0")
    robot.variables.set_attribute("score", "0")
    _set_leds("black")


def _start_escape(left_front, right_front):
    global reverse_steps, escape_turn_steps, escape_turn_direction, turn_steps

    reverse_steps = REVERSE_STEPS
    escape_turn_steps = random.randint(8, 14)
    turn_steps = 0

    if abs(left_front - right_front) < 0.02:
        escape_turn_direction = random.choice((-1, 1))
    elif left_front > right_front:
        escape_turn_direction = 1
    else:
        escape_turn_direction = -1


def _escape():
    global reverse_steps, escape_turn_steps

    if reverse_steps > 0:
        reverse_steps -= 1
        robot.epuck_wheels.set_speed(-TURN_SPEED, -TURN_SPEED)
        return True

    if escape_turn_steps > 0:
        escape_turn_steps -= 1
        robot.epuck_wheels.set_speed(
            escape_turn_direction * TURN_SPEED,
            -escape_turn_direction * TURN_SPEED,
        )
        return True

    return False


def _avoid_obstacles():
    if _escape():
        return True

    readings = robot.epuck_proximity.get_readings()
    left_front = readings[0].value + readings[1].value
    right_front = readings[6].value + readings[7].value
    left_side = readings[2].value + readings[3].value
    right_side = readings[4].value + readings[5].value
    strongest_front = max(readings[i].value for i in (0, 1, 6, 7))

    if strongest_front > CLOSE_THRESHOLD or (left_front > 0.12 and right_front > 0.12):
        _start_escape(left_front, right_front)
        return _escape()

    left = left_front + 0.35 * left_side
    right = right_front + 0.35 * right_side

    if max(left, right) < AVOID_THRESHOLD:
        return False

    if left > right:
        robot.epuck_wheels.set_speed(WHEEL_SPEED, 0.35 * WHEEL_SPEED)
    else:
        robot.epuck_wheels.set_speed(0.35 * WHEEL_SPEED, WHEEL_SPEED)
    return True


def _wander():
    global turn_steps, turn_direction

    if turn_steps <= 0 and random.random() < 0.025:
        turn_steps = random.randint(6, 18)
        turn_direction = random.choice((-1, 1))

    if turn_steps > 0:
        turn_steps -= 1
        robot.epuck_wheels.set_speed(turn_direction * TURN_SPEED, -turn_direction * TURN_SPEED)
    else:
        robot.epuck_wheels.set_speed(WHEEL_SPEED, WHEEL_SPEED)


def controlstep():
    robot.variables.set_attribute("harvest", "0")

    if robot.variables.get_attribute("resource_here"):
        robot.epuck_wheels.set_speed(0, 0)
        robot.variables.set_attribute("harvest", "1")
        robot.variables.set_attribute("carrying", "1")
        _set_leds("green")
        return

    robot.variables.set_attribute("carrying", "0")
    _set_leds("black")
    if not _avoid_obstacles():
        _wander()


def reset():
    init()


def destroy():
    robot.epuck_wheels.set_speed(0, 0)
