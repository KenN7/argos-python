from abc import ABCMeta
from abc import abstractmethod
import functools
from threading import Timer
import random


class ControllerInterface(metaclass=ABCMeta):
    """
    Interface for a user-defined controller that can be attached to a one
    or multiple TAMs. The controller can set the LED color of the TAM, check
    for the presence of a robot and read/write data from the robot using IRcom.

    This implementation provides some convenience methods, mostly a pseudo-random number generator
    and a timer/scheduler for scheduling tasks.
    
    See be.ac.ulb.iridia.tam.common.TAMInterface
    """

    def __init__(self, random_seed):
        """
        Initialize prng and timer. 

        Parameters
        -------------
        random_seed: int
            seed for the prng, set either constant or None (the system time will be used)
        """
        self.random_seed = random_seed
        random.seed(random_seed)
        #self.timer = Timer()

    @abstractmethod
    def reset(self):
        """
        Resets the controller.
        """
        random.seed(random_seed)
        #self.timer = Timer()

    @abstractmethod
    def step(self):
        """
        Step function of the controller. Called every Coordinator.STEP_INTERVAL milliseconds.
        """
        return

    def get_prng(self):
        """
        Returns the PRNG for this experiment.

        Returns
        -------------
        
            prng
        """
        return random.getstate()

    # def get_timer(self):
    #     """
    #     Returns the timer for this experiment.

    #     Returns
    #     -------------
        
    #         timer
    #     """
    #     return self.timer