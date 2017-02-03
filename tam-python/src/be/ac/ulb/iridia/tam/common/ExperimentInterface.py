from abc import ABCMeta
from abc import abstractmethod
import functools


class ExperimentInterface(metaclass=ABCMeta):
    """
    Interface for a "TAM experiment". This should be implemented by your main class.
    
    The coordinator uses the attach_TAM_controller() function to attach controllers to newly discovered TAMs.
    You can use the id of the TAM to attach specific controllers to specific TAMs,
    thereby giving them the different functionality.
    
    Your __main__() function should look similar to this:
    
      coordinator = new Coordinator()
      experiment = new YourExperimentClass()
      experiment.init()
      coordinator.set_experiment(experiment)
      coordinator.start()
    """

    @abstractmethod
    def __init__(self, random_seed):
        """
        Initialize prng and timer. 

        Parameters
        -------------
        random_seed: int
            seed for the prng, set either constant or None (the system time will be used)
        """

        # flag that signals if experiment is done
        self.finished = false
        # flag that signals if experiment is ready to start
        self.ready = true
        self.random_seed = random_seed
        random.seed(random_seed)
        #self.timer = Timer()

    @abstractmethod
    def reset(self):
        """
        Resets the experiment 
        """
        random.seed(random_seed)
        #self.timer = Timer()

    @abstractmethod
    def attach_TAM_controller(self, tam):
        """
        Called by the coordinator to attach controllers to newly discovered TAMs.
        You can use the id of the TAM to attach specific controllers
        to specific TAMs, thereby giving them the different functionality.

        Parameters
        -------------
        tam: TAMInterface
            TAM the coordinator requests a coordinator for.
        """
        return

    @abstractmethod
    def is_ready(self):
        """
        Checks whether the experiment is ready to start.

        Returns
        -------------
        bool
            true iff ready to start
        """
        return self.ready

    @abstractmethod
    def is_finished(self):
        """
        Checks whether the experiment should finish.

        Returns
        -------------
        bool
            true iff should finish
        """
        return self.finished

    @abstractmethod
    def set_ready(self):
        """
        Sets that experiment is ready to start.
        """
        self.ready = true

    @abstractmethod
    def set_finished(self):
        """
        Sets that the experiment should finish.
        """
        self.finished = true

    @abstractmethod
    def step(self):
        """
        Called by the coordinator on regular intervals.
        Can be used for management of TAMs etc.
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