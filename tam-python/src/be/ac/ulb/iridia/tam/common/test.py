from LedColor import LedColor
import threading

l = LedColor(red_channel_value = 23, green_channel_value = 23, blue_channel_value = 23)

l2 = LedColor(value=int("10111000101110001011100000000", 2))

print(l == l2)
print(l)
print(l2)

def synchronized(lock_name):
    """
    Synchronized method decorator,
    based on http://theorangeduck.com/page/synchronized-python
    It works similarly to the Java @synchronized decorator, 
    but requires a lock_name (tip: use "lock").
    """ 
    def decorator(method):
			
        def synced_method(self, *args, **kws):
            lock = getattr(self, lock_name)
            with lock:
                return method(self, *args, **kws)
                
        return synced_method
		
    return decorator


class T:
    def __init__(self):
        self.lock = threading.RLock()
    
    @synchronized("lock")
    def asd(self):
        return 0

t = T()
print(t.asd())

def update_voltage(data1, data2):
    """
    Sets the voltage from two bytes, as reported by the TAM in the status update packet.
    The TAM reports the voltage as 16bit unsigned int, multiplied by 1000.
    That is, a voltage of 3.2V gets reported as 3200, split up into two bytes.
    
    E.g. to set a voltage of 3.2V, use update_voltage(128,12)
    Parameters
    --------------
    data1: int
        LSB of the voltage
    data2: int
        MSB of the voltage
    """
    return ((data1 & 0xff) + ((data2 & 0xff) << 8)) / 1000.0

print("voltage: ", update_voltage(128, 12))