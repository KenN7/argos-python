#!/usr/bin/env python3
global robot, environment

def init():
    pass

def draw_in_world():
    pass

def draw_in_robot():
    environment.qt_draw.circle([0,0,0.010], [], 0.300, 'red', True)

def destroy():
    print('Closing the QT window')
