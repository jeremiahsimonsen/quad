# Death Chopper 9000 Visualization
# Jeremiah Simonsen
# Dec 16, 2015

# This presents a more pleasant visual interface for displaying the measured
# quantities for contract specifications. The Death Chopper 9000 is represented as
# a box and reflects the angle of the quadcopter. Measured roll, pitch, height, and
# voltage are also shown separately.

# Based on:
# Pololu MinIMU-9 + Arduino AHRS (Attitude and Heading Reference System)

# MinIMU9ArduinoAHRS is based on sf9domahrs by Doug Weibel and Jose Julio:
# http://code.google.com/p/sf9domahrs/

# sf9domahrs is based on ArduIMU v1.5 by Jordi Munoz and William Premerlani, Jose
# Julio and Doug Weibel:
# http://code.google.com/p/ardu-imu/

# MinIMU9ArduinoAHRS is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by the
# Free Software Foundation, either version 3 of the License, or (at your option)
# any later version.

# MinIMU9ArduinoAHRS is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for
# more details.

# You should have received a copy of the GNU Lesser General Public License along
# with MinIMU9ArduinoAHRS. If not, see <http://www.gnu.org/licenses/>.

################################################################################

# This is a 3D visualization program based on "Test for Razor 9DOF IMU" 
# by Jose Julio, copyright 2009.

# This script needs VPython, pyserial and pywin modules

# First Install Python 2.6.4 (Python 2.7 also works)
# Install pywin from http://sourceforge.net/projects/pywin32/
# Install pyserial from http://sourceforge.net/projects/pyserial/files/
# Install VPython from http://vpython.org/contents/download_windows.html

from visual import *
import serial
import string
import math

from time import time

# Degrees to radians
grad2rad = 3.141592/180.0

# Open the XBee serial port
ser = serial.Serial(port='COM9',baudrate=57600, parity=serial.PARITY_EVEN)

# Main scene
scene=display(title="Death Chopper 9000 Visualization")
scene.range=(1.2,1.2,1.2)
scene.forward = (1,0,-0.25)
scene.up=(0,0,1)

# Second scene (Roll, Pitch, Height, Voltage)
scene2 = display(title='Death Chopper 9000 Visualization',x=0, y=0, width=500, height=200,center=(0,0,0), background=(0,0,0))
scene2.range=(1,1.2,1)
scene.width=500
scene.y=200

scene2.select()
# Roll, Pitch, Height, Voltage
# Make 'lines' using cylinder objects for a visual representation
cil_roll = cylinder(pos=(-0.7,0,0),axis=(0.2,0,0),radius=0.01,color=color.red)
cil_roll2 = cylinder(pos=(-0.7,0,0),axis=(-0.2,0,0),radius=0.01,color=color.red)
cil_pitch = cylinder(pos=(-0.2,0,0),axis=(0.2,0,0),radius=0.01,color=color.green)
cil_pitch2 = cylinder(pos=(-0.2,0,0),axis=(-0.2,0,0),radius=0.01,color=color.green)
cil_height = cylinder(pos=(0.1,-0.2,0),axis=(0.4,0,0),radius=0.01,color=color.yellow)
cil_voltage = cylinder(pos=(0.6,-0.2,0),axis=(0.4,0,0),radius=0.01,color=color.cyan)

# Roll,Pitch,Height,Voltage labels for numerical representation
label(pos=(-0.7,0.3,0),text="Roll",box=0,opacity=0)
label(pos=(-0.2,0.3,0),text="Pitch",box=0,opacity=0)
label(pos=(0.3,0.3,0),text="Height",box=0,opacity=0)
label(pos=(0.8,0.3,0),text="Voltage",box=0,opacity=0)

# Labels to display the measured quantities
L1 = label(pos=(-0.7,0.22,0),text="-",box=0,opacity=0)
L2 = label(pos=(-0.2,0.22,0),text="-",box=0,opacity=0)
L3 = label(pos=(0.3,0.22,0),text="-",box=0,opacity=0)
L4 = label(pos=(0.8,0.22,0),text="-",box=0,opacity=0)

# Main scene objects
scene.select()
# Reference axis (x,y,z)
arrow(color=color.green,axis=(1,0,0), shaftwidth=0.02, fixedwidth=1)
arrow(color=color.green,axis=(0,-1,0), shaftwidth=0.02 , fixedwidth=1)
arrow(color=color.green,axis=(0,0,-1), shaftwidth=0.02, fixedwidth=1)
# labels
label(pos=(1,0,0),text="X",box=0,opacity=0)
label(pos=(0,-1,0),text="Y",box=0,opacity=0)
label(pos=(0,0,-1),text="Z",box=0,opacity=0)
# IMU object
platform = box(length=1, height=0.05, width=1, color=color.blue)
p_line = box(length=1,height=0.08,width=0.1,color=color.yellow)
plat_arrow = arrow(color=color.green,axis=(1,0,0), shaftwidth=0.06, fixedwidth=1)

# Open log file
f = open("Serial"+str(time())+".txt", 'w')

roll=0
pitch=0
yaw=0
height = 0
voltage = 0
nums = [0.0,0.0,0.0,0.0]
while 1:
    # Get a new packet
    line = ser.readline()
    print line
    f.write(line)                     # Write to the output log file
    try:
        nums = map(float,line.split())
    except:
        print 'Invalid line'

    # Parse packet
    pitch_deg = nums[0]
    pitch = -nums[0]*grad2rad
    roll_deg = nums[1]
    roll = nums[1]*grad2rad
    height_raw = nums[2]
    height = -0.2 + (nums[2] / 80)*0.4
    voltage_raw = nums[3]
    voltage = -0.2 + (nums[3] / 12.6)*0.4

    # Calculate new axis vectors and update objects
    axis=(cos(pitch)*cos(yaw),-cos(pitch)*sin(yaw),sin(pitch)) 
    up=(sin(roll)*sin(yaw)+cos(roll)*sin(pitch)*cos(yaw),sin(roll)*cos(yaw)-cos(roll)*sin(pitch)*sin(yaw),-cos(roll)*cos(pitch))
    platform.axis=axis
    platform.up=up
    platform.length=1.0
    platform.width=0.65
    plat_arrow.axis=axis
    plat_arrow.up=up
    plat_arrow.length=0.8
    p_line.axis=axis
    p_line.up=up
    cil_roll.axis=(0.2*cos(roll),0.2*sin(roll),0)
    cil_roll2.axis=(-0.2*cos(roll),-0.2*sin(roll),0)
    cil_pitch.axis=(0.2*cos(pitch),0.2*sin(pitch),0)
    cil_pitch2.axis=(-0.2*cos(pitch),-0.2*sin(pitch),0)
    cil_height.pos=(0.1,height,0)
    cil_voltage.pos=(0.6,voltage,0)

    # Display quantities
    L1.text = str(roll_deg)
    L2.text = str(pitch_deg)
    L3.text = str(height_raw)
    L4.text = str(voltage_raw)
ser.close
f.close
