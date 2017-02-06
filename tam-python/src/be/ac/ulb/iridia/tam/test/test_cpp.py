from random import random

def param_test(param):
    return param ** 2

def func_test():
    return random()

class Test:

    def __init__(self):
        self.a = 10
        self.b = 2
   
    def rand(self):
        return random()

    def param_test_class(self, param):
        return self.a * param + self.b
