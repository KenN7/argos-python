#Install orderedset with pip
from orderedset import OrderedSet
from queue import Queue

from ExperimentInterface import ExperimentInterface
from LedColor import LedColor

import logging

import threading
import time

import re


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



class Coordinator:
    """
    This is the main class of the coordinator. This class communicates with the Xbee module
    and schedules all the different tasks that the coordinator has to do. These tasks include:

     - discover new nodes by Xbee command (regularly)
     - resolve unknown nodes by node discovery (only when unknown node was encountered)
     - request the current signal strength from the local Xbee module (regularly)
     - send packets from the send queue (regularly)
     - call the step() function of the controllers of all known TAMs (regularly)
  
    Additionally, this class starts two packet listeners:

     1) takes care of normal network packets that have been received and the status
        responses to the packets sent by us
     2) takes care of the responses to AT commands issued by the coordinator
  
    The most important packet is the status report of the TAM. The TAM sends its status when:

     - battery voltage is under BATT_VOLTAGE_THRESHOLD
     - every HEARTBEAT_INTERVAL_SEC seconds
     - every time something changed in status (robot left or arrived)
     - as response to a command of the coordinator (eg, SET_LEDS command)
  
    After initializing the coordinator with the parameters of the Xbee's serial port,
    the user's main class MUST call the start() method in order to schedule all the
    tasks described above and start the packet listeners. After completing these tasks,
    the coordinator goes into an infinite loop that can be interrupted by calling
    setShutdownRequested(true).
    """

    # Setup logger
    logging.basicConfig(level=logging.INFO)
    logger = logging.getLogger(__name__)


    # intervals of various tasks that the coordinator performs regularly
    SEND_QUEUE_INTERVAL =         10 # send all items in the queue every 10ms
    CHECK_DB_INTERVAL   =  60 * 1000 # check signal strength every minute
    TRIGGER_ND_INTERVAL = 600 * 1000 # trigger node discovery every 10 minutes
    STEP_INTERVAL =               10 # step experiment and all TAMs every 10ms

    # prefix for usb serial ports on linux
    PORT_DEVICE_PREFIX = "/dev/ttyUSB"

    # packet types of TAM->Coordinator (TC)
    PACKET_TYPE_TC_CURRENT_STATE = 0

    # packet types of Coordinator->TAM (CT)
    PACKET_TYPE_CT_SET_LEDS = 10
    PACKET_TYPE_CT_WRITE_ROBOT = 12
    PACKET_TYPE_CT_SHUTDOWN = 13

    # timeout for the SET_LEDS command in seconds
    SET_LEDS_CMD_TIMEOUT = 3
    # timeout for the WRITE_ROBOT_CMD_TIMEOUT command in seconds
    WRITE_ROBOT_CMD_TIMEOUT = 5 # TODO set to a realistic value

    # list of known coordinator nodes, will be added to blacklist directly
   COORDINATOR_ADDRESSES = [
            "0x00,0x13,0xa2,0x00,0x40,0x8c,0x04,0xa4",  # coordinator 0
            "0x00,0x13,0xa2,0x00,0x40,0x8c,0x04,0xc5",  # coordinator 1
            "0x00,0x13,0xa2,0x00,0x40,0x8d,0x6e,0xb4",  # coordinator 2
            "0x00,0x13,0xa2,0x00,0x40,0x8d,0x6e,0xa2"   # coordinator 3
   ]


    # // main Xbee object used to communicate with the attached Xbee module
    # private XBee xbee;
    

    # // timer used to schedule reoccurring tasks of the coordinator and the controllers of all TAMs
    # private Timer timer;

   

    def __init__(self, device = None):
        """
        Creates the coordinator and initializes all variables.
        This constructor uses the current time to initialize the pseudo-random number generator.
        
        Parameters
        --------------
        device: String
            serial device used to access the Xbee module, specify null to use auto-detection
        """
        # hash map that stores all currently known TAMs by using the 64bit address of the TAM as key
        self.list_of_TAMS = {}
        # a list of nodes on the network that are not TAMs
        self.address_blacklist = OrderedSet(COORDINATOR_ADDRESSES)
        # queue of requests that should be sent to different TAMs
        self.send_request_queue = Queue()
        # serial device used to access the Xbee module
        self.device = device
        # experiment attached to this coordinator
        self.experiment = None

        # Initialize flags

        # flags that designates if the coordinator should terminate
        self.shutdown_requested = False
        # signal strength of the attached Xbee module
        self.signal_strength = 0
        # flag that designates if a node discovery has been requested
        self.node_discovery_requested = False
        # flag that designates if a node discovery is currently running
        self.node_discovery_running = False

        #self.xbee = XBee()

        # create timer used to schedule all tasks done by the coordinator
        # this.timer = new Timer

        self.setup_shutdown_handlers()

        # Required to have synchronized methods
        self.lock = threading.RLock()

    def setup_shutdown_handlers(self):
        """
        TODO
        Catch interrupt signals and terminate the application gracefully.
        Note: the java version doesnt currently work.
        """
        pass


    def get_experiment(self):
        """
        Returns
        --------------
        ExperimentInterface
           The current experiment attached to the coordinator.
        """
        return self.experiment

    def set_experiment(self, experiment):
        """   
        Sets the current experiment attached to the coordinator.
        The experiment is used to attach controllers to newly discovered TAMs.
        
        Parameters
        --------------
        experiment: ExperimentInterface
            object that implements ExperimentInterface
        """
        self.experiment = experiment

    @synchronized("lock")
    def get_signal_strength(self):
        """
        Returns
        --------------
        int 
            Returns the signal strength, in dB, of the attached local Xbee module
        """
        return self.signal_strength
    
    @synchronized("lock")
    def set_signal_strength(self, signal_strength):
        """
        Sets the signal strength of the attached local Xbee module.
        Requested by AT command DB, parsed in @see ATCommandPacketListener
        
        Parameters
        --------------
        signalStrength: int
            signal strength as reported by Xbee module.
        """
        self.signal_strength = signal_strength

    @synchronized("lock")
    def is_node_discovery_requested(self):
        """
        Returns
        --------------
        bool
            true iff a node discovery has been requested.
        """
        return self.node_discovery_requested

    @synchronized("lock")
    def set_node_discovery_requested(self, node_discovery_requested):
        """
        Requests a node discovery.
        The coordinator will send a request for node discovery if
        there is none running already.

        Parameters
        --------------
        nodeDiscoveryRequested: bool
            true if a node discovery is requested.
        """
        if self.is_node_discovery_running():
            logger.debug("Warning: Node discovery is currently running!")
            return
        self.node_discovery_requested = node_discovery_requested

    
    @synchronized("lock")
    def is_node_discovery_running(self):
        """
        Returns
        --------------
        bool
            true iff node discovery is currently running
        """
        return self.node_discovery_running

    @synchronized("lock")
    def set_node_discovery_running(self, node_discovery_running):
        """
        Sets if there's currently a node discovery running. Used to prevent the
        coordinator to send multiple, overlapping requests for node discovery.
        This is set to true when the coordinator sends a node discovery request.
        This is set to false when the node discovery request times out.
        Parameters
        --------------
        node_discovery_running: bool
            true iff node discovery is currently running
        """
        self.node_discovery_running = node_discovery_running

    @synchronized("lock")
    def is_shutdown_requested(self):
        """ 
        Has a shutdown be requested? Used to terminate the coordinator.

        Returns
        --------------
        bool
            true iff user requested shutdown of coordinator
        """
        return self.shutdown_requested

    @synchronized("lock")
    def set_shutdown_requested(self, shutdown_requested):
        """
        Set shutdown flag. Used to terminate the coordinator.

        Parameters
        --------------
        shutdown_requested: bool
            true iff the coordinator should be terminated
        """
        self.shutdown_requested = shutdown_requested

    
    def update_discovered_TAMS(self, id, address64):
        """
        Method that is triggered on the discovery of a (partially) unknown TAM.
        The coordinator can discover new TAMs in two ways:
         1) by receiving a status update of an unknown TAM @see TAMResponsePacketListener
         2) by receiving a reply of an (partially) unknown TAM to the node discovery @see ATCommandPacketListener
        In first case, the new TAM is added to the list of TAMs.
        In the second case, the TAM might be partially known already due to a status update.
        Only the second case can provide the id of the TAM.

        Parameters
        --------------
        id: string
            id of the TAM (null if yet unknown)
        address64: XBeeAddress64
             64bit address of the TAM
        """
        tam = None

        # convert address to string, which is the key to the map of all known TAMs
        address = str(address64)

        # ignore blacklisted TAMs
        if address in self.address_blacklist:
            return

        # check if this is a newly discovered node that is not a TAM
        pattern = re.compile("TAM[0-9][1-9]")
        if (id != None) and (pattern.match(id) is None):
            # if it's not a TAM, remove and blacklist
            del list_of_TAMS[address]
            address_blacklist.add(address)

        # check if we know this TAM already
        if address in list_of_TAMS:
            # if we have that TAM already, update it (can't change addresses!)
            tam = list_of_TAMS[address]
            tam.set_id(id)
            tam.update_last_seen_timestamp()
            logger.info("TAM " + tam.get_id() + " is already recorded in the database, record updated.")
        else:
            # tam is completely unknown, so create a new object and stick it into the database
            tam = TAM(id, self, address64)
            list_of_TAMS[address] = tam
            logger.info("Added TAM with address " + address + " to database.")

        # if we haven't attached a controller to this TAM yet, do it now
        # (if we have the id already)
        if (id is None) and (tam.get_controller is None):
            self.get_experiment.attach_TAM_controller(tam)

    @synchronized("lock")
    def send_set_leds_command(tam, led_color):
        """
        Sends a SET_LEDS command to the specified TAM.
        The request is ignored if we sent the command already (and we haven't had
        a response from the TAM yet) and if the command does not change actually the LEDs.
        
        Parameters
        -------------
        tam: TAM 
            TAM to send command to
        led_color: LedColor
            new color of leds to send
        """

        # check if we've sent already a SET_LEDS command
        if tam.get_set_leds_cmd_timeout_task() is not None:
            logger.debug("Not sending SET_LEDS command again as we sent one already.");
            return
        
        # check if we're trying to send a command with a color that is already set
        # ignore this check if we haven't gotten an update yet (because color is invalid on init)
        if tam.get_led_color_last_updated() != 0 and tam.get_led_color().equals(led_color)):        
            logger.debug("Not sending SET_LEDS command again as leds are set already to requested values.");
            return
        
        # after above checks, we are sure that we want to change the leds to different values and
        # that we haven't tried to do this before

        # create an array of arbitrary data to send
        payload[] = {
            PACKET_TYPE_CT_SET_LEDS,
            ledColor.get_red_channel_value(),
            ledColor.get_green_channel_value(),
            ledColor.get_blue_channel_value()
            }

        # prepare the request and append it to the queue of

        # // prepare the request and append it to the queue of
        # DMTxRequest request = new DMTxRequest(tam.getAddress64(), payload);

        # // create a timer task that handles the timeout of the SET_LEDS command
        # TimerTask setLedsCmdTimeoutTask = new TimerTask()
        # {
        #     public void run()
        #     {
        #         log.warn("SET_LED command sent to " + tam.getId() + " timed out!");
        #         tam.setSetLedsCmdTimeoutTask(null);
        #     }
        # };
        # tam.setSetLedsCmdTimeoutTask(setLedsCmdTimeoutTask);

        # // append request to the queue and schedule timer
        # log.debug("Sending SET_LEDS command to " + tam.getId() + " value (" + ledColor + ")");
        # sendRequestQueue.add(request);
        # getTimer().schedule(setLedsCmdTimeoutTask, SET_LEDS_CMD_TIMEOUT * 1000);


    @synchronized("lock")
    def send_write_robot_command(self, tam, value):
        """
        Sends a WRITE_ROBOT command to the specified TAM.
        The request is ignored if we sent the command already (and we haven't had
        a response from the TAM yet).

        Parameters
        --------------
        tam: TAM 
            TAM to send command to
        value: int 
            value to send to the robot
        """
        # TODO
        pass
    
    @synchronized("lock")
    def send_shutdown_command_to_all_TAMs(self):
        """
        Sends the shutdown command to all TAMs.
        TAMs need to be switched off and on after receiving this command.
        Commands are send iteratively and synchronously (that is, method blocks until completion).
        Note: after sending this command, you have to remove power from the TAM completely before starting up again
        
        Throws XBeeException on communication error
        """
        # TODO
        pass

    @synchronized("lock")
    def send_switch_off_leds_command_to_all_TAMs(self):
        """
        Sends a command to all TAMs that switches of the LEDs.
        Commands are send iteratively and synchronously (that is, method blocks until completion).
        
        Throws XBeeException on communication error
        """
        # TODO
        pass

    def start(self):
        """
        Primary method of the coordinator.
        This method schedules all regularly occurring tasks (controller step functions, node discovery requests,
        send requests in the queue etc) and starts the packet listeners that parse incoming packets.
        Note: this method does not return until set_shutdown_requested(true) is called
        
        Throws Exception on Xbee communication failure
        """
        # TODO
        pass