# If serial busy, close serial monitor on py

import serial
import time
import pyautogui

# Serial info
arduino_port = "/dev/cu.usbserial-1130"  # Change this if needed
baud_rate = 9600

# Open Serial port
ser = serial.Serial(arduino_port, baud_rate, timeout=1)
time.sleep(2)

try:    
    while True:
        line = ser.readline().decode().strip()  # Read and decode line
        val = int(line[3:])
        if line[0] == '1': # motor 1
            pyautogui.move(val, 0, duration=0)
        elif line[0] == '2': # motor 2
            pyautogui.move(0, -val, duration=0)
except KeyboardInterrupt:
    print("Stopping...")
finally:
    ser.close()
