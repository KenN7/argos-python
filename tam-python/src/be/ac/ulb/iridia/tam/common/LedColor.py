class LedColor: 
    """
    Led color class.
    This class is used to store the state of the RGB leds of the TAMs.
    The color is represented internally by a long for ease of use and speed.
    Uses bit operations to set and get specific channels of the color.
    
    Note: while this appears to be 32bit RGBA, the alpha channel is ignore making this a 24bit color space
    """

    # mask for all channels
    CHANNEL_MASK_ALL   = 0xffffff00
    # mask for red channel
    CHANNEL_MASK_RED   = 0xff000000
    # mask for green channel
    CHANNEL_MASK_GREEN = 0x00ff0000
    # mask for blue channel
    CHANNEL_MASK_BLUE  = 0x0000ff00

    def __init__(self, value = 0, red_channel_value = None, green_channel_value = None, blue_channel_value = None):
        """
        Initialize the led, optionally by passing a color 
        as a single integer variable or as RGB components. 

        Parameters
        -------------
        value: int 
            int value to set (lower 8 bits are alpha channel which is ignored)
        red_channel_value: int
        green_channel_value: int
        blue_channel_value: int
            int value for each channel (only the lower 8 bits are used)
        """
        self.value = 0
        if (red_channel_value is not None and green_channel_value is not None and blue_channel_value is not None):
            self.set_red_channel_value(red_channel_value)
            self.set_green_channel_value(green_channel_value)
            self.set_blue_channel_value(blue_channel_value)
        elif value is not None:
            print("LedColor:",  value)
            self.value = value

    def get_value(self):
        """
        Returns the current value as a single int variable

        Returns
        -------------
        value: int
        """
        return self.value

    def set_value(self, value):
        """
        Sets self.value using a single int variable

        Parameters
        -------------
        value: int
            int value to set (lower 8 bits are alpha channel which is ignored)
        """
        self.value = value


    def get_red_channel_value(self):
        """
        Returns the value of the red channel of this color.
        Returns
        -------------
        int 
            value of red channel
        """
        return ((self.value & self.CHANNEL_MASK_RED) >> 24)
    
    def get_green_channel_value(self):
        """
        Returns the value of the green channel of this color.
        Returns
        -------------
        int 
            value of green channel
        """
        return ((self.value & self.CHANNEL_MASK_GREEN) >> 16)
    
    def get_blue_channel_value(self):
        """
        Returns the value of the blue channel of this color.
        Returns
        -------------
        int 
            value of blue channel
        """
        return ((self.value & self.CHANNEL_MASK_BLUE) >> 8)

    
    def set_red_channel_value(self, red_channel_value):
        """
        Sets the value of the red channel of this color separately.

        Parameters
        -------------
        red_channel_value: int
            value of the red channel; only the lowest 8 bits are considered.
        """
        self.value = (self.value & (~self.CHANNEL_MASK_RED + 1)) ^ (red_channel_value << 24)
   
    def set_green_channel_value(self, green_channel_value):
        """
        Sets the value of the green channel of this color separately.

        Parameters
        -------------
        green_channel_value: int
            value of the green channel; only the lowest 8 bits are considered.
        """
        self.value = (self.value & (~self.CHANNEL_MASK_GREEN + 1)) ^ (green_channel_value << 16)

    def set_blue_channel_value(self, blue_channel_value):
        """
        Sets the value of the blue channel of this color separately.

        Parameters
        -------------
        blue_channel_value: int
            value of the blue channel; only the lowest 8 bits are considered.
        """
        self.value = (self.value & (~self.CHANNEL_MASK_BLUE + 1)) ^ (blue_channel_value << 8)

    def __eq__(self, other):
        """
        Check if two Leds have the same color

        Parameters
        -------------
        other: LedColor
            the other color that is considered in the comparison.


        Returns
        -------------
        bool
            True iff the two colors are the same.
        """
        if isinstance(other, self.__class__):
            # ignore LSB for comparison (ignore ALPHA channel)
            return (self.value & self.CHANNEL_MASK_ALL) == (other.get_value() & self.CHANNEL_MASK_ALL)
        return NotImplemented


    def __str__(self):
        """
        Returns a string representation of this LedColor.
        
        Returns
        -------------
        string
            representation of this LedColor.
        """
        return "R=" + str(self.get_red_channel_value()) \
                + ",G=" + str(self.get_green_channel_value()) \
                + ",B=" + str(self.get_blue_channel_value())