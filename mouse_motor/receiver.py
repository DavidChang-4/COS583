import serial, time, pyautogui

# Speed up PyAutoGUI
pyautogui.PAUSE = 0 # remove the default 0.1 s pause
pyautogui.FAILSAFE = False

ser = serial.Serial("/dev/cu.usbserial-1130",
                    baudrate=115200,  # match the Arduino
                    timeout=0.01)     # only wait 10 ms for a line
time.sleep(2)

try:
    while True:
        if ser.in_waiting:
            line = ser.readline().decode(errors="ignore").strip()
            key = line[0]
            print(line)
            if key == 'A':
                val = int(line[2:])
                pyautogui.moveRel(0, val)
            elif key == 'B':
                val = int(line[2:])
                pyautogui.moveRel(val, 0)
            elif key == 'C':
                pyautogui.click() # Left click
            elif key == 'D':
                pyautogui.hotkey('command', 'c') # Copy
                time.sleep(0.05)
            elif key == 'E':
                pyautogui.hotkey('command', 'v') # Paste
except KeyboardInterrupt:
    pass
finally:
    ser.close()