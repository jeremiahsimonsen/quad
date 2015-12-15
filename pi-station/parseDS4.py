#!/usr/bin/env python2

import sys
import serial
import time
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
    button_circle: True
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
buf = StringIO.StringIO(report)

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

DEMO = 1
FLY = 2

packet = []
ids = ['dummy_id', 'r2_analog', pitch_LUT[MODE-1], roll_LUT[MODE-1], yaw_LUT[MODE-1]]

# ser = serial.Serial('/dev/ttyUSB0', 57600, parity=serial.PARITY_EVEN)
# 	ser = serial.Serial('COM9', 57600, parity=serial.PARITY_EVEN)

def getReport():
	lines = []
	
	l = sys.stdin.readline()
	while not 'Report dump' in l:
		l = sys.stdin.readline()

	l = sys.stdin.readline()	
	while not 'Report dump' in l:
		l = sys.stdin.readline()
		lines.append(l)
	
	return lines

def passwordProtect():
	btn1 = False
	btn2 = False
	btn3 = False
	error = False
	while btn1 != True and btn2 != True and btn3 != True:
		print 'Start of password'
		btn1 = False
		btn2 = False
		btn3 = False
		error = False
		while btn1 == False:
			report = getReport()
			for line in report:
				if 'button' in line and not 'l1' in line and 'True' in line:
					print 'Error'
					break
				elif 'button_l1' in line and 'True' in line:
					btn1 = True
		print 'First button correct'
		time.sleep(1)
		report = getReport()
		
		while btn2 == False:
			report = getReport()
			for line in report:
				if 'button' in line and not 'r1' in line and 'True' in line:
					print 'Error'
					error = True
					break
				elif 'button_r1' in line and 'True' in line:
					btn2 = True
			if error == True:
				break
		if error == True:
			continue
		print 'Second button correct'
		time.sleep(1)
		report = getReport()

		while btn3 == False:
			report = getReport()
			for line in report:
				if 'button' in line and not 'trackpad' in line and 'True' in line:
					print 'Error'
					error = True
					break
				elif 'button_trackpad' in line and 'True' in line:
					btn3 = True
			if error == True:
				break
		if error == True:
			continue
		print 'Third button correct'
		time.sleep(1)

def fly():
	print 'Entering flight mode'
	packet = [FLY, FLY, FLY, FLY, FLY, FLY]
# 	ser.write(bytearray(packet))
	
	packet = [START, 0, STICK_ZERO, STICK_ZERO, STICK_ZERO, STOP]
	
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
# 			ser.write(bytearray(packet))
			print packet
		for index, val in enumerate(ids):
			if val in line:
				if index == 2:
					packet[index] = int(int(filter(str.isdigit, str.lstrip(line, ' r2'))) / 255.0 * 253.0 * -1.0 + 253.0)
				else:
					packet[index] = int(int(filter(str.isdigit, str.lstrip(line, ' r2'))) / 255.0 * 253.0)
					
		""" Send packet that causes quad to abort """
		if 'button_cross' in line:
			if 'True' in line:
				packet = [0, 0, STICK_ZERO, STICK_ZERO, STICK_ZERO, 0]
# 				ser.write(bytearray(packet))
				exit()

def demo():
	print 'Entering demo mode'
	packet = [DEMO, DEMO, DEMO, DEMO, DEMO, DEMO]
# 	ser.write(bytearray(packet))
	for line in sys.stdin:
		pass

def main():
	print 'Death Chopper 9000'
	time.sleep(1)
	
	print 'Please enter the password'
	passwordProtect()
	
	print 'Press a button: triangle for flight, circle for contract demo'
	
	op_mode = -1
	while op_mode < 0:
 		line = sys.stdin.readline()
#		line = buf.readline()
		if 'button_triangle' in line and 'True' in line:
			op_mode = FLY
			fly()
		elif 'button_circle' in line and 'True' in line:
			op_mode = DEMO
			demo()
		else:
 			line = sys.stdin.readline()
#			line = buf.readline()
	

			
if __name__ == '__main__':
	main()


