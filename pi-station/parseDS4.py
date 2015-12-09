#!/usr/bin/env python2

import sys
import serial
import StringIO

report = """ Report dump
    left_analog_x: 120
    left_analog_y: 124
    right_analog_x: 129
    right_analog_y: 125
    l2_analog: 0
    r2_analog: 0
    dpad_up: False
    dpad_down: False
    dpad_left: False
    dpad_right: False
    button_cross: False
    button_circle: False
    button_square: False
    button_triangle: False
    button_l1: False
    button_l2: False
    button_l3: False
    button_r1: False
    button_r2: False
    button_r3: False
    button_share: False
    button_options: False
    button_trackpad: False
    button_ps: False
    motion_y: 3
    motion_x: -1
    motion_z: -3
    orientation_roll: 211
    orientation_yaw: 8079
    orientation_pitch: 2033
    trackpad_touch0_id: 99
    trackpad_touch0_active: False
    trackpad_touch0_x: 1554
    trackpad_touch0_y: 224
    trackpad_touch1_id: 90
    trackpad_touch1_active: False
    trackpad_touch1_x: 102
    trackpad_touch1_y: 644
    timestamp: 54
    battery: 11
    plug_usb: True
    plug_audio: False
    plug_mic: False
 Report dump"""

""" The analog sticks map to different control parameters based on flight Mode 1,2,3, or 4 """
throttle_LUT = ['right_analog_y', 'left_analog_y', 'right_analog_y', 'left_analog_y']
pitch_LUT 	 = ['left_analog_y', 'right_analog_y', 'left_analog_y', 'right_analog_y']
roll_LUT 	 = ['right_analog_x', 'right_analog_x', 'left_analog_x', 'left_analog_x']
yaw_LUT 	 = ['left_analog_x', 'left_analog_x', 'right_analog_x', 'right_analog_x']

START = 255
STOP  = 254
MODE  = 3

STICK_ZERO = 127
DEADBAND = 10

def main():
	packet = [START, 0, STICK_ZERO, STICK_ZERO, STICK_ZERO, STOP]
	ids = ['dummy_id', 'r2_analog', pitch_LUT[MODE-1], roll_LUT[MODE-1], yaw_LUT[MODE-1]]
	
	ser = serial.Serial('/dev/ttyUSB0', 57600, parity=serial.PARITY_EVEN)
# 	ser = serial.Serial('COM9', 57600, parity=serial.PARITY_EVEN)
	
# 	buf = StringIO.StringIO(report)
# 	for line in buf:
	for line in sys.stdin:
		""" New report """
		if 'Report dump' in line:
			if abs(packet[2] - STICK_ZERO) < DEADBAND:
				packet[2] = STICK_ZERO
			if abs(packet[3] - STICK_ZERO) < DEADBAND:
				packet[3] = STICK_ZERO
			if abs(packet[4] - STICK_ZERO) < DEADBAND:
				packet[4] = STICK_ZERO
			ser.write(bytearray(packet))
			print packet
		for index, val in enumerate(ids):
			if val in line:
				if index == 2:
					packet[index] = int(int(filter(str.isdigit, str.lstrip(line, ' r2'))) / 255.0 * 253.0 * -1.0 + 253.0)
				else:
					packet[index] = int(int(filter(str.isdigit, str.lstrip(line, ' r2'))) / 255.0 * 253.0)
					
		if 'button_cross' in line:
			if 'True' in line:
				packet = [0, 0, STICK_ZERO, STICK_ZERO, STICK_ZERO, 0]
				ser.write(bytearray(packet))
				exit()
			
if __name__ == '__main__':
	main()


