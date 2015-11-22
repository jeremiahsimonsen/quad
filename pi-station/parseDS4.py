#!/usr/bin/env python2

import sys
import serial
import time

def main():
	ser = serial.Serial('/dev/ttyUSB0', 57600, parity=serial.PARITY_EVEN)
#	ser.write('Hello world')

	ids = ['left_analog_x','left_analog_y','right_analog_x','right_analog_y']
	sticks = [0,0,0,0]
	thr = 0

	for line in sys.stdin:
		if 'r2_analog' in line:
			thr = int(filter(str.isdigit, str.lstrip(line,' r2')))
			print thr

		for index,val in enumerate(ids):
			if val in line:
				sticks[index] = int(filter(str.isdigit, line))
#		print sticks


		if 'Report dump' in line:
#			txBytes = bytearray(sticks)
			txBytes = bytearray([thr, thr, thr, thr])
			ser.write(txBytes)
		
'''
		if 'left_analog_x' in line:
			left_joystick_x = int(filter(str.isdigit, line))
			print left_joystick_x,
		elif 'left_analog_y' in line:
			left_joystick_y = int(filter(str.isdigit, line))
			print left_joystick_y,
		elif 'right_analog_x' in line:
			right_joystick_x = int(filter(str.isdigit, line))
			print right_joystick_x,
		elif 'right_analog_y' in line:
			right_joystick_y = int(filter(str.isdigit, line))
			print right_joystick_y
		txBytes = bytearray([left_joystick_x, left_joystick_y, right_joystick_x, right_joystick_y])
		ser.write(txBytes)
'''
if __name__ == '__main__':
	main()
