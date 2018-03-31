#!/usr/bin/env python
import ADC0832
import time
import led

def init():
	ADC0832.setup()

def loop():
    while True:
        res = ADC0832.getResult() - 80
        if res < 0:
            res = 0
        if res > 100:
            res = 100
        if res < 10:
            led.led('on')
        else:
            led.led('off')
        print ('res = %d' % res)
        
        time.sleep(0.2)

if __name__ == '__main__':
	init()
	try:
		loop()
	except KeyboardInterrupt: 
		ADC0832.destroy()
		print ('The end !')
