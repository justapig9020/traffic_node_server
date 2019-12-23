import RPi.GPIO as GPIO
import time
 
class SIG ():
    def __init__(self):
        self.rl = 5
        self.gl = 6
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self.rl, GPIO.OUT)
        GPIO.setup(self.gl, GPIO.OUT)
 
    def sig(self, s):
        if s==0:
            GPIO.output(self.gl, GPIO.HIGH)
            GPIO.output(self.rl, GPIO.LOW)
        elif s==1:
            GPIO.output(self.rl, GPIO.HIGH)
            GPIO.output(self.gl, GPIO.LOW)
