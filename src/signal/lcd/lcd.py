import sys
import time
import smbus2
from RPLCD.i2c import CharLCD

def init():
    sys.modules['smbus'] = smbus2
    lcd = CharLCD('PCF8574', address=0x27, port=1, backlight_enabled=True)
    return lcd

def update(lcd, s):
    lcd.clear()
    lcd.cursor_pos = (0, 0)
    lcd.write_string(s)
