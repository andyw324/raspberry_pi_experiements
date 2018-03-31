#!/usr/bin/python

import RPi.GPIO as GPIO
import time


class HumidTemp():

        def __init__(self, pin):
                self.pin = pin
                self.data = []

        def dht11_init(self):
                j = 0

                GPIO.setmode(GPIO.BOARD)

                time.sleep(1)

                GPIO.setup(self.pin, GPIO.OUT)

                GPIO.output(self.pin, GPIO.LOW)
                time.sleep(0.02)
                GPIO.output(self.pin, GPIO.HIGH)

                GPIO.setup(self.pin, GPIO.IN)

                while GPIO.input(self.pin) == GPIO.LOW:
                        continue

                while GPIO.input(self.pin) == GPIO.HIGH:
                        continue

                while j < 40:
                        k = 0
                        while GPIO.input(self.pin) == GPIO.LOW:
                                continue

                        while GPIO.input(self.pin) == GPIO.HIGH:
                                k += 1
                                if k > 100:
                                        break

                        if k < 8:
                                self.data.append(0)
                        else:
                                self.data.append(1)

                        j += 1

                print("sensor is working.")
                #print (self.data)

                humidity_bit = self.data[0:8]
                humidity_point_bit = self.data[8:16]
                temperature_bit = self.data[16:24]
                temperature_point_bit = self.data[24:32]
                check_bit = self.data[32:40]

                humidity = 0
                humidity_point = 0
                temperature = 0
                temperature_point = 0
                check = 0

                for i in range(8):
                        humidity += humidity_bit[i] * 2 ** (7 - i)
                        humidity_point += humidity_point_bit[i] * 2 ** (7 - i)
                        temperature += temperature_bit[i] * 2 ** (7 - i)
                        temperature_point += temperature_point_bit[i] * 2 ** (7 - i)
                        check += check_bit[i] * 2 ** (7 - i)

                tmp = humidity + humidity_point + temperature + temperature_point                
                if check == tmp:
                        self.temp = temperature
                        self.humid = humidity
                        print("check passed")
                else:
                        self.temp = None
                        self.humid = None
                        print("check failed")
        def getTemp(self):
                self.dht11_init()
                self.cleanup()
                return self.temp
        def getHumid(self):
                self.dht11_init()
                self.cleanup()
                return self.humid

        def cleanup(self):
                GPIO.cleanup()
