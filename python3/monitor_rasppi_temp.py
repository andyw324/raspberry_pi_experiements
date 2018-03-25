import i2c_lcd1602
import time
import RPi.GPIO as GPIO
from subprocess import Popen, PIPE

screen = i2c_lcd1602.Screen(bus=1, addr=0x3f, cols=16, rows=2)

def destory():
   screen.disable_backlight()
   screen.display_init()
   screen.clear()

def loop():
    while True:
        temp = Popen(['vcgencmd', 'measure_temp'], \
                     stdout=PIPE, \
                     universal_newlines=True).stdout.readline().rstrip()

        cpuutil = Popen("mpstat 1 1 | grep 'Average' | awk '{print 100-$12}'", \
                    shell=True, \
                    stdout=PIPE, \
                    universal_newlines=True).stdout.readline().rstrip()
        
        print(temp.rstrip())
        print('Load=' + str(round(float(cpuutil),2)) + '%')
        
        screen.clear()
        screen.cursorTo(0, 0)
        screen.println(temp)
##        t = ds18b20.ds18b20Read()
##        t = round(t, 2)
##        m = '%f' %t
##        m = m[:5]
        screen.cursorTo(1, 0)
        screen.println('Load=' + str(round(float(cpuutil),2)) + '%')
        
        time.sleep(5)
##        screen.command(0x1,1.0)

if __name__ == '__main__':
   print('starting...')
##   line = " www.adeept.com "
   screen.enable_backlight()
   time.sleep(1)
   screen.display_init()
   screen.clear()
   try:
      loop()
   except KeyboardInterrupt:
      destory()
