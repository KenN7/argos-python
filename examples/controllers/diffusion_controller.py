# import math

# # PARAMETERS:

# # Maximum tolerance for the angle between
# # the robot heading direction and
# # the closest obstacle detected. 
# alpha = 7.5 * math.pi / 180
# # Maximum tolerance for the proximity reading between
# # the robot and the closest obstacle.
# # The proximity reading is 0 when nothing is detected
# # and grows exponentially to 1 when the obstacle is
# # touching the robot.
# delta = 0.1
# # Wheel speed.
# wheel_velocity = 5
# # Angle tolerance range to go straight.
# # It is set to [-alpha,alpha]. 
# go_straight_angle_range = [-alpha, alpha]
a = 1
def init():
    robot.logprint("started diffusion1_python")

def controlstep():
    grobot.logprint("started diffusion1_python")
def reset():
    robot.logprint("reset")

def destroy():
    robot.logprint("destroy")