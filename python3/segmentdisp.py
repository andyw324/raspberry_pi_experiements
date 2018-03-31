#!/usr/bin/env python

import RPi.GPIO as GPIO
import time

##DIO = 16
##CLK = 18
##STB = 22
##
##LSBFIRST = 0
##MSBFIRST = 1
##
##tmp = 0

##def gpioOutput(pin, value):
##       GPIO.setwarnings(value)
##       GPIO.setmode(GPIO.BOARD)
##       GPIO.setup(DIO, GPIO.OUT)
##       GPIO.output(pin, value)
##
##def gpioSetWarnings(value):
##       GPIO.setwarnings(value)
##
##def gpioSetMode():
##       GPIO.setmode(GPIO.BOARD)
##
##def gpioSetup(DIO):
##       GPIO.setup(DIO, GPIO.OUT)

class Segment():
        
        LSBFIRST = 0
        MSBFIRST = 1

        def __init__(self, dio = 16, clk = 18, stb = 22):
                self.dio = dio
                self.clk = clk
                self.stb = stb
                GPIO.setwarnings(False)
                GPIO.setmode(GPIO.BOARD)
                GPIO.setup(self.dio, GPIO.OUT)
                GPIO.setup(self.clk, GPIO.OUT)
                GPIO.setup(self.stb, GPIO.OUT)
                self.TM1638_init()
                        
        def _shiftOut(self, dataPin, clockPin, bitOrder, val):
                for i in range(8):
                        GPIO.setmode(GPIO.BOARD)
                        GPIO.setup(dataPin, GPIO.OUT)
                        if bitOrder == self.LSBFIRST:
                                GPIO.output(dataPin, val & (1 << i))
                        else:
                                GPIO.output(dataPin, val & (1 << (7 -i)))
                        GPIO.setup(clockPin, GPIO.OUT)
                        GPIO.output(clockPin, True)
                        time.sleep(0.000001)			
                        GPIO.output(clockPin, False)
                        time.sleep(0.000001)			
                
        def sendCommand(self, cmd):
                GPIO.setmode(GPIO.BOARD)
                GPIO.setup(self.stb, GPIO.OUT)
                GPIO.output(self.stb, False)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, cmd)
                GPIO.output(self.stb, True)

        def TM1638_init(self):
##                GPIO.setwarnings(False)
##                GPIO.setmode(GPIO.BOARD)
##                GPIO.setup(self.dio, GPIO.OUT)
##                GPIO.setup(self.clk, GPIO.OUT)
##                GPIO.setup(self.stb, GPIO.OUT)
                self.sendCommand(0x8f)
                self.clear()

        def numberDisplay(self, num):
                digits = [0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f]
                self.sendCommand(0x40)
                GPIO.setmode(GPIO.BOARD)
                GPIO.setup(self.stb, GPIO.OUT)
                GPIO.output(self.stb, False)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0xc0)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, digits[int(num/1000%10)])
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0x00)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, digits[int(num/100%10)])
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0x00)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, digits[int(num/10%10)])
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0x00)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, digits[int(num%10)])
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0x00)
                GPIO.output(self.stb, True)

        def numberDisplay_dec(self, num):
                digits = [0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f]
                integer = 0
                decimal = 0

                pro = int(num * 100)

                integer = int(pro / 100)
                decimal = int(pro % 100)

                self.sendCommand(0x40)
                GPIO.setmode(GPIO.BOARD)
                GPIO.setup(self.stb, GPIO.OUT)
                GPIO.output(self.stb, False)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0xc0)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, digits[int(integer/10)])
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0x00)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, digits[int(integer%10)] | 0x80)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0x00)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, digits[int(decimal/10)])
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0x00)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, digits[int(decimal%10)])
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0x00)
                GPIO.output(self.stb, True)

        def numberDisplay_temp(self, num):
                digits = [0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x39]
                integer = 0
                decimal = 0

                pro = int(round(num * 10,0))

                integer = int(pro / 10)
                decimal = int(pro % 10)

                self.sendCommand(0x40)
                GPIO.setmode(GPIO.BOARD)
                GPIO.setup(self.stb, GPIO.OUT)
                GPIO.output(self.stb, False)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0xc0)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, digits[int(integer/10)])
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0x00)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, digits[int(integer%10)] | 0x80)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0x00)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, digits[int(decimal)])
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0x00)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, digits[10])
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0x00)
                GPIO.output(self.stb, True)
                
        def numberDisplay_null(self):

                self.sendCommand(0x40)
                GPIO.setmode(GPIO.BOARD)
                GPIO.setup(self.stb, GPIO.OUT)
                GPIO.output(self.stb, False)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0xc0)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0x40)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0x00)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0x40)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0x00)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0x40)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0x00)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0x40)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0x00)
                GPIO.output(self.stb, True)
                
        def clear(self):

                self.sendCommand(0x40)
                GPIO.setmode(GPIO.BOARD)
                GPIO.setup(self.stb, GPIO.OUT)
                GPIO.output(self.stb, False)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0xc0)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0x00)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0x00)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0x00)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0x00)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0x00)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0x00)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0x00)
                self._shiftOut(self.dio, self.clk, self.LSBFIRST, 0x00)
                GPIO.output(self.stb, True)                
##try:
##	TM1638_init()
##	numberDisplay(1234)
##	time.sleep(4) # 4s
##	numberDisplay_dec(56.78)
##	time.sleep(4) # 4s
##	while True:
##		numberDisplay(tmp)
##		tmp += 1
##		if tmp > 9999:
##			tmp = 0
##		time.sleep(0.05)
##
##except KeyboardInterrupt:
##	GPIO.cleanup()
