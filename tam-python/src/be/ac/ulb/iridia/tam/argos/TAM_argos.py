from TAMInterface import TAMInterface
from LedColor import LedColor
from ControllerInterface import ControllerInterface

class TAM(TAMInterface):

    def __init__(self, id):
        self.led_color = LedColor(red_channel_value = 8, green_channel_value = 16, blue_channel_value = 32)
        self.id = id
        self.robot_presence = False
        self.data_to_send = 0
        self.data_received = 0
        self.controller = None

    def get_led_color(self):
        return self.led_color

    def set_led_color(led_color):
        self.led_color = led_color

    def update_robot_present(robot_presence_bool):
        self.robot_presence = robot_presence_bool

    def is_robot_present(self):
        return self.is_robot_present

    def get_id(self):
        return self.id

    def set_robot_data_to_send(self, robot_data):
        """
        Sets new data to send; 
        They must be contained in a byte;

        Parameters
        -------------
        robot_data: int
            The new data to send.
        """
        self.data_to_send = robot_data

    def get_robot_data_to_send(self):
        return self.get_robot_data_to_send

    def set_robot_data_received(self, data_received):
        """
        Sets new data to be received; 
        They must be contained in a byte;

        Parameters
        -------------
        data_received: int
            The new data to be received.
        """
        self.data_received = data_received

    def get_robot_data_received(self):
        return self.data_received

    def get_controller(self):
        return self.controller

    def set_controller(self, controller):
        """
        Sets a new controller.

        Parameters
        -------------
        controller: ControllerInterface
            The new controller that will be set.
        """
        print("set controller: ", controller)
        self.controller = controller

    def __str__(self):
        """
        Returns a string representation of this TAM.
        
        Returns
        -------------
        string
            representation of this TAM.
        """
        return str("TAM{" 
                + "id='" + str(self.id) + '\'' \
                + ", led_color=(" + str(self.led_color) \
                + "), robot_present=" + str(self.robot_presence) \
                + ", data_to_send=" + str(self.data_to_send) \
                + ", data_received=" + str(self.data_received) \
        + ", controller=" + str(self.controller)) + "}"
