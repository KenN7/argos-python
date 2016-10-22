import math

# PARAMETERS:

# Maximum tolerance for the angle between
# the robot heading direction and
# the closest obstacle detected. 
alpha = 7.5 * math.pi / 180
# Maximum tolerance for the proximity reading between
# the robot and the closest obstacle.
# The proximity reading is 0 when nothing is detected
# and grows exponentially to 1 when the obstacle is
# touching the robot.
delta = 0.1
# Wheel speed.
wheel_velocity = 5
# Angle tolerance range to go straight.
# It is set to [-alpha,alpha]. 
go_straight_angle_range = [-alpha, alpha]

def init():
    robot.logprint("started diffusion1_python")

def controlstep():
    global alpha, delta, wheel_velocity, go_straight_angle_range

    accumulator = {"tot_value": 0, "tot_angle": 0}
    # Get readings from the proximity sensors
    proximity_readings = robot.proximity()
    for proximity_reading_i in proximity_readings:
        accumulator["tot_value"] += proximity_reading_i.value / len(proximity_readings)
        accumulator["tot_angle"] += proximity_reading_i.angle.value() / len(proximity_readings)
    
    if (go_straight_angle_range[0] <= accumulator["tot_angle"] and accumulator["tot_angle"] <= go_straight_angle_range[1] and \
        accumulator["tot_value"] < delta):
        robot.wheels(wheel_velocity, wheel_velocity)
    else:
        if accumulator["tot_angle"] > 0:
            robot.wheels(wheel_velocity, 0)
        else:
            robot.wheels(0, wheel_velocity)

def reset():
	robot.logprint("reset")

def destroy():
	robot.logprint("destroy")