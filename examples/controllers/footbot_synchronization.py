import random

COUNT_MAX = 100
counter = 0


def init():
    global counter
    counter = random.randint(0, COUNT_MAX)
    robot.colored_blob_omnidirectional_camera.enable()


def controlstep():
    global counter
    blobs = robot.colored_blob_omnidirectional_camera.get_readings()
    someone_flashed = any(color == [255, 0, 0] for color, _angle, _distance in blobs)

    if someone_flashed:
        counter += counter // 10
    else:
        counter += 1

    if counter > COUNT_MAX:
        robot.leds.set_all_colors("red")
        counter = 0
    else:
        robot.leds.set_all_colors("black")


def reset():
    global counter
    counter = random.randint(0, COUNT_MAX)


def destroy():
    pass
