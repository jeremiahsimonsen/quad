#!/usr/bin/env python2

import sys
import serial
import time

""" Report format """
""" Report dump
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

""" The analog sticks map to different control parameters based on flight Mode 1,2,3, or 4 """
throttle_LUT = ['right_analog_y', 'left_analog_y', 'right_analog_y', 'left_analog_y']
pitch_LUT 	 = ['left_analog_y', 'right_analog_y', 'left_analog_y', 'right_analog_y']
roll_LUT 	 = ['right_analog_x', 'right_analog_x', 'left_analog_x', 'left_analog_x']
yaw_LUT 	 = ['left_analog_x', 'left_analog_x', 'right_analog_x', 'right_analog_x']

""" Comms protocol constants """
START = 255
STOP  = 254

""" Controller behavior constants """
MODE  = 3
STICK_ZERO = 127
DEADBAND = 10

""" Special command constants """
FLY = 1
DEMO = 2
DEMO_MOTOR_TOGGLE = 3

packet = []
ids = ['dummy_id', 'r2_analog', pitch_LUT[MODE-1], roll_LUT[MODE-1], yaw_LUT[MODE-1]]

ser = serial.Serial('/dev/ttyUSB0', 57600, parity=serial.PARITY_EVEN)

""" Get a complete controller status report """
def getReport():
	lines = []
	
	""" Wait for the start of a new report dump """
	l = sys.stdin.readline()
	while not 'Report dump' in l:
		if 'Disconnected' in l:
			print 'Error: Controller has been disconnected'
			exit()
		l = sys.stdin.readline()

	""" Read until start of next report dump """
	l = sys.stdin.readline()	
	while not 'Report dump' in l:
		if 'Disconnected' in l:
			print 'Error: Controller has been disconnected'
			exit()
		l = sys.stdin.readline()
		lines.append(l)
	
	return lines

""" Wireless control is locked out until the password is entered correctly.
	The password is L1, R1, touchpad"""
def passwordProtect():
	btn1 = False
	btn2 = False
	btn3 = False
	error = False
	""" Buttons haven't been pressed correctly """
	while btn1 != True and btn2 != True and btn3 != True:
		print 'Start of password'
		btn1 = False
		btn2 = False
		btn3 = False
		error = False
		""" Wait for the first button """
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
		
		""" Wait for the second button """
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
				continue
		print 'Second button correct'
		time.sleep(1)
		report = getReport()
		""" TODO: Start over if second button wrong """

		""" Wait for the third button """
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
				continue
		print 'Third button correct'
		time.sleep(1)
		""" TODO: Start over if third button wrong """

""" Remote control for flight mode"""
def fly():
	""" Tell the DC9000 it should enter flight mode """
	print 'Entering flight mode'
	packet = [FLY, FLY, FLY, FLY, FLY, FLY]
	ser.write(bytearray(packet))
	
	packet = [START, 0, STICK_ZERO, STICK_ZERO, STICK_ZERO, STOP]
	
	""" Read controller status """
	for line in sys.stdin:
		""" Handle controller disconnect gracefully """
		if 'Disconnected' in line:
			print 'Error: Controller has been disconnected'
			packet = [0, 0, STICK_ZERO, STICK_ZERO, STICK_ZERO, 0]
			ser.write(bytearray(packet))
			exit()
		""" New report so send RC packet """
		if 'Report dump' in line:
			""" Clip joysticks to deadband to eliminate noise """
			if abs(packet[2] - STICK_ZERO) < DEADBAND:
				packet[2] = STICK_ZERO
			if abs(packet[3] - STICK_ZERO) < DEADBAND:
				packet[3] = STICK_ZERO
			if abs(packet[4] - STICK_ZERO) < DEADBAND:
				packet[4] = STICK_ZERO
			ser.write(bytearray(packet))
			print packet
		""" Search for control parameters """
		for index, val in enumerate(ids):
			if val in line:
				""" Have to reverse the pitch axis. Rescale so 254,255 are reserved """
				if index == 2:
					packet[index] = int(int(filter(str.isdigit, str.lstrip(line, ' r2'))) / 255.0 * 253.0 * -1.0 + 253.0)
				else:
					packet[index] = int(int(filter(str.isdigit, str.lstrip(line, ' r2'))) / 255.0 * 253.0)
					
		""" Send packet that causes quad to abort """
		if 'button_cross' in line:
			if 'True' in line:
				packet = [0, 0, STICK_ZERO, STICK_ZERO, STICK_ZERO, 0]
				ser.write(bytearray(packet))
				exit()

""" Remote control for contract demo mode. Toggles motors enabled/disabled """
def demo():
	print 'Entering demo mode'
	packet = [DEMO, DEMO, DEMO, DEMO, DEMO, DEMO]
	ser.write(bytearray(packet))
	for line in sys.stdin:
		if 'button_cross' in line and 'True' in line:
			packet = [DEMO_MOTOR_TOGGLE, DEMO_MOTOR_TOGGLE, DEMO_MOTOR_TOGGLE, DEMO_MOTOR_TOGGLE, DEMO_MOTOR_TOGGLE, DEMO_MOTOR_TOGGLE]
			ser.write(bytearray(packet))
		if 'Disconnected' in line:
			print 'Error: Controller has been disconnected'
			packet = [0, 0, STICK_ZERO, STICK_ZERO, STICK_ZERO, 0]
			ser.write(bytearray(packet))
			exit()

""" Run everything """
def main():
	print 'Death Chopper 9000'
	time.sleep(1)
	
	""" Password protection """
	print 'Please enter the password'
	passwordProtect()
	
	print 'Press a button: triangle for flight, circle for contract demo'
	
	""" Select operation mode """
	op_mode = -1
	while op_mode < 0:
		line = sys.stdin.readline()
		if 'Disconnected' in line:
			print 'Error: Controller has been disconnected'
			packet = [0, 0, STICK_ZERO, STICK_ZERO, STICK_ZERO, 0]
			ser.write(bytearray(packet))
			exit()
		if 'button_triangle' in line and 'True' in line:
			op_mode = FLY
			fly()
		elif 'button_circle' in line and 'True' in line:
			op_mode = DEMO
			demo()
		else:
			line = sys.stdin.readline()
	

			
if __name__ == '__main__':
	main()


