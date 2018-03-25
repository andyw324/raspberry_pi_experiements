import i2c_lcd1602_test
import time
import RPi.GPIO as GPIO
from subprocess import Popen, PIPE

screen = i2c_lcd1602_test.lcd(0x3f, 1)

##temp = Popen(['vcgencmd', 'measure_temp'], stdout=PIPE, universal_newlines=True).stdout.readline()
##print(temp.rstrip())
##
##cpuutil = Popen("mpstat 1 1 | grep 'Average' | awk '{print 100-$12}'", \
##                shell=True, \
##                stdout=PIPE, \
##                universal_newlines=True).stdout.readline().rstrip()
##
##print('Load=' + cpuutil + '%')


def destory():
   screen.lcd_clear()
##   GPIO.cleanup()

def loop():
    while True:
        temp = Popen(['vcgencmd', 'measure_temp'], \
                     stdout=PIPE, \
                     universal_newlines=True).stdout.readline().rstrip()
##        print(temp.rstrip())

        cpuutil = Popen("mpstat 1 1 | grep 'Average' | awk '{print 100-$12}'", \
                    shell=True, \
                    stdout=PIPE, \
                    universal_newlines=True).stdout.readline().rstrip()

##        print('Load=' + cpuutil + '%')
##        screen.cursorTo(0, 0)
        screen.lcd_puts(temp,1)
##        t = ds18b20.ds18b20Read()
##        t = round(t, 2)
##        m = '%f' %t
##        m = m[:5]
##        screen.cursorTo(1, 0)
        screen.lcd_puts(cpuutil,2)
        screen.lcd_clear()
        time.sleep(1)      

if __name__ == '__main__':
   print('starting...')
##   line = " www.adeept.com "
   screen.lcd_backlight(1)
   screen.lcd_clear()
   try:
      loop()
   except KeyboardInterrupt:
      destory()
