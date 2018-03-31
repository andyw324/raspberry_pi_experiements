import i2c_lcd1602
import dht11_func
import segmentdisp
import time
import RPi.GPIO as GPIO
from subprocess import Popen, PIPE
import post_to_pbi
import os.path

GPIO.setmode(GPIO.BOARD)

screen = i2c_lcd1602.Screen(bus=1, addr=0x3f, cols=16, rows=2)

segment = segmentdisp.Segment()

uri = 'pbi api url'

startTime = time.strftime("%Y-%m-%dT%H:%M:%S", time.gmtime())
if not os.path.isfile("./rasppi_temps.log"):
   OutFile = open("rasppi_temps.log","w")
   OutFile.write('timestamp, cpu_temp, cpu_util, cpu_freq, amb_temp, amb_humid, heatsink, cover, starttime\n')
   OutFile.close()
   

def destory():
   screen.disable_backlight()
   screen.display_init()
   screen.clear()
   segment.clear()

def loop(cover_on,heatsink_on):
   with open('rasppi_temps.log','a+') as OutFile:
      while True:
         temp = Popen(['vcgencmd', 'measure_temp'], \
                     stdout=PIPE, \
                     universal_newlines=True).stdout.readline().rstrip()

         cpuutil = Popen("mpstat 1 1 | grep 'Average' | awk '{print 100-$12}'", \
                    shell=True, \
                    stdout=PIPE, \
                    universal_newlines=True).stdout.readline().rstrip()

         cpuclock = Popen(['vcgencmd', 'measure_clock arm'], \
                     stdout=PIPE, \
                     universal_newlines=True).stdout.readline().rstrip()
              
         print(temp)
         print('Load=' + str(round(float(cpuutil),2)) + '%')
         print(cpuclock)

         screen.clear()
         screen.cursorTo(0, 0)
         screen.println(temp)
   ##        t = ds18b20.ds18b20Read()
   ##        t = round(t, 2)
   ##        m = '%f' %t
   ##        m = m[:5]
         screen.cursorTo(1, 0)
         screen.println('Load=' + str(round(float(cpuutil),2)) + '%')
         
         dht_out = dht11_func.dht_GetReadings(40)
         if dht_out[0] == None:
            segment.numberDisplay_null()
            amb_temp = 'null'
            amb_humid = 'null'
         else:
            segment.numberDisplay_temp(dht_out[0])
            print('Abient Temp: ' + str(dht_out[0]) + 'C')
            print('Humidity: ' + str(dht_out[1]) + '%')
            amb_temp = dht_out[0]
            amb_humid = dht_out[1]

         ts = time.strftime("%Y-%m-%dT%H:%M:%S", time.gmtime())
         
         OutFile.write(ts + ',' +\
                          str(temp[5:-2]) + ',' +\
                          str(cpuutil) + ',' +\
                          str(cpuclock[14:]) + ',' +\
                          str(amb_temp) + ',' +\
                          str(amb_humid) + ',' +\
                          heatsink_on + ',' +\
                          cover_on + ',' +\
                          startTime +'\n')

         post_to_pbi.post(uri,\
                          ts,\
                          temp[5:-2],\
                          cpuutil,\
                          cpuclock[14:],\
                          amb_temp,\
                          amb_humid,\
                          heatsink_on,\
                          cover_on,\
                          startTime)

##         time.sleep(2)
   ##        screen.command(0x1,1.0)

if __name__ == '__main__':
   import argparse
   parser = argparse.ArgumentParser(description='Monitor Raspberry PI temps and post to Power BI')
   parser.add_argument('--cover_on', metavar='bool', \
                       required=False, default='N/K', \
                       help='Is the cover on the raspberry pi case (default: N/K)')
   parser.add_argument('--heatsink_on', metavar='bool', \
                       required=False, default='N/K', \
                       help='Is the heatsink on the raspberry pi processor (default: N/K)')
   args = parser.parse_args()
   
   print('starting...')
   screen.enable_backlight()
   time.sleep(1)
   screen.display_init()
   screen.clear()
   try:
      loop(cover_on=args.cover_on, heatsink_on=args.heatsink_on)
   except KeyboardInterrupt:
      destory()
