from abc import ABCMeta
from abc import abstractmethod
import functools


@total_ordering
class TAMInterface(metaclass=ABCMeta):
    """
    This interface implements all methods required to manipulate and
    query the state of the TAM.
    """
   
    @abstractmethod 
    def get_led_color(self):
        """
        Returns the color of the RGB LEDs of the TAM as currently known.

        Returns
        -------------
        LedColor
            LedColor object reflecting the 24bit color, or null if TAM didn't report status yet.
        """
        return 

    @abstractmethod
    def set_led_color(self, led_color):
        """
        Sets a new LED color.
        Update is ignored if LED color does not change.

        Parameters
        -------------
        led_color: LedColor
            LedColor object reflecting the 24bit color
        """
        return 

    @abstractmethod
    def is_robot_present(self):
        """
        Returns true if there is currently a robot in the TAM.

        Returns
        -------------
        bool
            true if there is currently a robot in the TAM.
        """
        return 

    @abstractmethod
    def get_id(self):
        """    
        Returns the id of the TAM.
        - it's TAMXX with XX being a 2-digit unique integer; or
        - it's the last 5 characters of the 64bit address if the id hasn't been resolved yet
        
        Returns
        -------------
        string
            id of TAM as String 5 characters long.
        """
        return

    @abstractmethod
    def set_robot_data_to_send(self, robot_data):
        """
        Sets an integer value for the robot_data.
        Update is ignored if data did not change

        Parameters
        -------------
        robot_data: int
            new robot_data value
        """
        return

    @abstractmethod
    def get_robot_data_received(self):
        """
        Returns the data received from the robot currently in the TAM.

        Returns
        -------------
        int
            data received from the robot currently in the TAM.
        """
        return

    @abstractmethod
    def get_controller(self):
        """
        Returns the user-defined controller of the TAM.

        Returns
        -------------
        ControllerInterface
            controller of the TAM
        """
        return
    
    @abstractmethod
    def set_controller(self, controller):
        """
        Sets the user-defined controller of the TAM.
        See be.ac.ulb.iridia.tam.common.ControllerInterface
        
        Parameters
        -------------
        controller: ControllerInterface
            user-defined controller of the TAM
        """

    # @total_ordering creates automatically the other comparison operators, 
    # given __eq__ and another comparison operator (__gt__ in this case).
    
    def __eq__(self, other):
        """
        Check if two TAMs are the same, by comparing their ID.ABCMeta

        Parameters
        -------------
        other: TAMInterface
            the TAM that is compared.


        Returns
        -------------
        bool
            True iff the two TAMs have the same ID 
        """
        if isinstance(other, self.__class__):
            return self.get_id() == other.get_id()
        return NotImplemented

    def __gt__(self, other):
        """
        Check if this TAM is bigger than another, by comparing their ID.ABCMeta

        Parameters
        -------------
        other: TAMInterface
            the TAM that is compared.


        Returns
        -------------
        bool
            True iff this TAM has bigger ID than the other one.
        """
        if isinstance(other, self.__class__):
            return self.get_id() > other.get_id()
        return NotImplemented