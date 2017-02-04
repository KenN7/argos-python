import LedColor from LedColor
import TAMInterface from TAMInterface
import ControllerInterface from ControllerInterface

import threading

import logging
import time

def synchronized(lock_name):
    """
    Synchronized method decorator,
    based on http://theorangeduck.com/page/synchronized-python
    It works similarly to the Java @synchronized decorator, 
    but requires a lock_name (tip: use "lock").

    The class must have a "lock" attribute (tip: self.lock = threading.RLock())
    """ 
    def decorator(method):
			
        def synced_method(self, *args, **kws):
            lock = getattr(self, lock_name)
            with lock:
                return method(self, *args, **kws)
                
        return synced_method
		
    return decorator

def get_date_in_ms():
    """
    Returns
    int 
        number of milliseconds since 1970/01/01
    """
    return int(round(time.time() * 1000))

class TAM(TAMInterface):
    """
    This class represents a single TAM in the experiment.
    It holds the basic information such as Xbee-related data (addresses, id etc) and
    the state of the TAM.
    
    The state of a TAM is defined as the current color of the RGB LEDs, and if a
    robot is present in the TAM or not.
    
    The class also additionally stores the voltage of the TAM, which is not considered to be
    part of the state of the TAM in the sense of the controller.
    
    Note: everything in this class MUST be synchronized to render it thread-safe. 
    The synchronization is done through the method decorator above,
    and by adding a "lock" attribute to the class
    """

    # Setup logger
    logging.basicConfig(level=logging.INFO)
    logger = logging.getLogger(__name__)

    def __init__(self, id, coordinator, address64):
        """
        Constructor of the TAM. A TAM is created in two cases:
         1) the coordinator receives a heartbeat of an unknown TAM
         2) a unknown TAM replies to the node discover command

        Parameters
        -------------
        id: str
            id of the TAM
        coordinator: coordinator
            coordinator
        address64: XBeeAddress64
            64bit Xbee address of the TAM
        """
        logger.info("TAM construction")
        # 64bit address of the TAM in the mesh network.
        self.address64 = address64
        # coordinator that handles this TAM
        self.coordinator = coordinator

        # Timestamp that describes when the coordinator has seen the TAM for the first time.
        self.first_seen_timestamp = get_date_in_ms()
        # Timestamp that describes when the coordinator has seen the TAM for the last time.
        self.last_seen_timestamp = 0

        # LedColor class that describes the color of the RGB LEDs of the TAM.
        self.led_color = None
        # Timestamp of last update of led color.
        self.led_color_last_updated = 0

        # Flag that indicates if a robot is present as reported by the TAM.
        self.robot_present = False
        # Timestamp of last update of robot presence.
        self.robot_present_last_updated = 0
        # The data the robot sent to the TAM.
        self.robot_data = 0
        # Timestamp of last update of robot data.
        self.robot_data_last_updated = 0

        # Voltage as double value as reported by the TAM. Should be >= 3.2V.
        self.voltage = 0
    
        # Xbee network id. Up to 20 characters. In case of the TAM, this must be "TAMXX" with XX being a 2-digit number
        self.set_id(id)

        # Timer task that handles the timeout of the SET_LEDS command. Used to detect timed-out packets.
        self.set_leds_cmd_timeout_task = None

        # Timer task that handles the timeout of the WRITE_ROBOT command. Used to detect timed-out packets.
        self.write_robot_cmd_timeout_task = None

        # The ControllerInterface of the TAM, as set by the user.
        self.controller_interface = None


    # ***************************************************************************
    #
    # Methods below implement TAM interface
    #
    # ***************************************************************************

    @synchronized("lock")
    def get_led_color(self):
        """
        Returns the color of the RGB LEDs of the TAM as currently known.

        Returns
        -------------
        LedColor
            object reflecting the 24bit color, or null if TAM didn't report status yet
        """
        return self.led_color

    def set_led_color(self, led_color):
        """
        Sets a new LED color.
        Update is ignored if LED color does not change.

        Parameters
        -------------
        led_color: LedColor
            object reflecting the 24bit color
        """
        self.led_color = led_color

    @synchronized("lock")
    def is_robot_present(self):
        """
        Returns
        -------------
        bool:
            true iff there is currently a robot in the TAM.
        """
        return self.robot_present

    @synchronized("lock")
    def get_id(self):
        """
        Returns the id of the TAM.
        - it's TAMXX with XX being a 2-digit unique integer; or
        - it's the last 5 characters of the 64bit address if the id hasn't been resolved yet
        id of TAM as String 5 characters long.
        
        Returns
        -------------
        str
            id of TAM as str of 5 characters.
        """
        return self.id

    @synchronized("lock")
    def get_robot_data_received(self):
        """
        Returns
        -------------
        int:
            Returns the data received from the robot currently in the TAM.
        """
        if not self.is_robot_present():
            self.robot_data = 0
        return self.robot_data

    @synchronized("lock")
    def set_robot_data_to_send(self, robot_data):
        """
        Sets a value for the robotData.
        Update is ignored if data did not change. 

        Parameters
        -------------
        robot_data: int
            new robot_data value
        """
        self.coordinator.send_write_robot_command(self, robot_data)

    @synchronized("lock")
    def get_controller(self):
        """
        Returns
        -------------
        ControllerInterface:
            the user-defined controller of the TAM.
        """"
        return self.controller


    @synchronized("lock")
    def set_controller(self, controller):
        """
        Sets the user-defined controller of the TAM.
        The controller should define a step() function that controls the behavior of the TAM.

        Parameters
        -------------
        controller ControllerInterface:
            the user-defined controller of the TAM.
        """"
        self.controller = controller

     # ***************************************************************************
     #
     # Methods below are only used by the coordinator
     #
     # ***************************************************************************

     @synchronized("lock")
     def get_led_color_last_updated(self):
         """
         Returns
         -------------
         int
            the timestamp of the last update of the LED color.
         """
         return self.led_color_last_updated

    @synchronized("lock")
    def update_led_color(self, led_color):
        """
        Sets a new LED color and updates the timestamp accordingly.
        Update is ignored if LED color does not change

        Parameters
        -------------
        led_color: LedColor
            LedColor object reflecting the 24bit color
        """

        # check if we're actually trying to set a new color
        # ignore this check if we never received an update
        if (self.led_color is None) or (not self.led_color.equals(led_color) or (self.led_color_last_updated == 0)):
            self.led_color = led_color
            self.get_led_color_last_updated = get_date_in_ms()

    @synchronized("lock")
    def get_robot_present_last_updated(self):
        """
        Returns
        -------------
        int 
            timestamp of the last update of the robot_present flag.
        """
        return self.robot_present_last_updated

    @synchronized("lock")
    def update_robot_present(self, robot_present):
        """
        Sets a value for the robot_present flag and updates the timestamp accordingly.
        Update is ignored if flag did not change

        Parameters
        -------------
        robot_present: bool
            true iff the robot is present in the TAM
        """
        if self.robot_present != robot_present:
            self.robot_present = robot_present
            self.robot_present_last_updated = get_date_in_ms()

    @synchronized("lock")
    def set_id(self, id):
        """
        Sets the id of te TAM. TAM ids are always 5 characters:
         - it's TAMXX with XX being a 2-digit unique integer; or
         - it's the last 5 characters of the 64bit address if the id hasn't been resolved yet
        Updated using data from a node discovery request.

        Parameters
        --------------
        id: str
            id of tam if resolved already, else null
        """
        if id is None:
            address = str(self.get_address64())
            self.id = address[-5: ]
        self.id = id

    @synchronized("lock")
    def get_address64(self):
        """
        Returns
        -------------
        XBeeAddress64
            the 64bit Xbee network address of the TAM.
        """
        return self.address64

    @synchronized("lock")
    def get_first_seen_timestamp(self):
        """
        Returns
        --------------
        int 
            the timestamp of the first time the coordinator has seen this TAM on the network.
        """
        return self.first_seen_timestamp

    @synchronized("lock")
    def get_last_seen_timestamp(self):
        """
        Returns
        --------------
        int 
            the timestamp of the last time the coordinator has seen this TAM on the network.
        """
        return self.lastseen_timestamp

    @synchronized("lock")
    def update_last_seen_timestamp(self):
        """
        Updates the timestamp of the last time the coordinator has seen this TAM on the network.
        Sets timestamp to current time.
        """
        self.last_seen_timestamp = get_date_in_ms()

    @synchronized("lock")
    def get_voltage(self):
        """
        Returns
        -------------
        float:
            the voltage of the TAM.
        """
        return self.voltage
    
    @synchronized("lock")
    def update_voltage(self, data1, data2):
        """
        Sets the voltage from two bytes, as reported by the TAM in the status update packet.
        The TAM reports the voltage as 16bit unsigned int, multiplied by 1000.
        That is, a voltage of 3.2V gets reported as 3200, split up into two bytes.
        
        E.g. to set a voltage of 3.2V, use update_voltage(128,12) as bin(3200) = 00001100 10000000 

        Parameters
        --------------
        data1: int
            LSB of the voltage
        data2: int
            MSB of the voltage
        """
        self.voltage = ((data1 & 0xff) + ((data2 & 0xff) << 8)) / 1000.0

    # /**
    # * Returns the current timer task that handles the timeout of the SET_LEDS command.
    # * Return null if no unacknowledged command has been sent.
    # * @return current timer task or null
    # */
    # protected synchronized TimerTask getSetLedsCmdTimeoutTask()
    # {
    #     return setLedsCmdTimeoutTask;
    # }

    # /**
    #  * Sets the current timer task that handles the timeout of the SET_LEDS command.
    #  * Set null last command sent has been acknowledged.
    #  * @param setLedsCmdTimeoutTask  timer task for timeout or null
    #  */
    # protected synchronized void setSetLedsCmdTimeoutTask(TimerTask setLedsCmdTimeoutTask)
    # {
        # this.setLedsCmdTimeoutTask = setLedsCmdTimeoutTask;
    # }
    # 
    # /**
    #  * Returns the current timer task that handles the timeout of the WRITE_ROBOT command.
    #  * Return null if no unacknowledged command has been sent.
    #  * @return current timer task or null
    #  */
    # protected synchronized TimerTask getWriteRobotCmdTimeoutTask()
    # {
        # return writeRobotCmdTimeoutTask;
    # }

    # /**
    #  * Sets the current timer task that handles the timeout of the WRITE_ROBOT command.
    #  * Set null last command sent has been acknowledged.
    #  * @param writeRobotCmdTimeoutTask  timer task for timeout or null
    #  */
    # protected synchronized void setWriteRobotCmdTimeoutTask(TimerTask writeRobotCmdTimeoutTask)
    # {
        # this.writeRobotCmdTimeoutTask = writeRobotCmdTimeoutTask;
    # }

    def update_robot_data(self, robot_data):
        """
        Sets a value for the robotData and updates the timestamp accordingly.
        Update is ignored if data did not change.
        """
        if (self.robot_data != round) or (self.robot_data_last_updated == 0):
            self.robot_data = robot_data
            self.robot_data_last_updated = get_date_in_ms()

    @synchronized("lock")
    def get_robot_data_last_updated(self):
        """
        Returns
        ------------
        int
            the the timestamp of the last update of the robot_data value.
        """
        return self.robot_data_last_updated

    def __str__(self):
        """
        Returns a string representation of this TAM.
        
        Returns
        -------------
        string
            representation of this TAM.
        """
        return "TAM{" +
                "id='" + str(self.id) + '\'' + \
                ", address64=" + str(self.address64) + \
                ", voltage=" + str(self.voltage) + \
                "V, ledColor=(" + str(self.ledColor) + \
                "), ledColorLastUpdated=" + str(self.ledColorLastUpdated) + \
                ", robotPresent=" + str(self.robotPresent) + \
                ", robotPresentLastUpdated=" + str(self.robotPresentLastUpdated) + \
                ", robotData=" + str(self.robotData) + \
                ", robotDataLastUpdate=" + str(self.robotDataLastUpdated)