#!/usr/bin/python

import RPi.GPIO as GPIO
import time
import i2c_lcd1602
from gpiozero import ButtonBoard

pins = [18]
eggs = ButtonBoard(18,23,24,27,12,6,17,16,20,21,22,13)
reed = ButtonBoard(5,pull_up=False)
go = ButtonBoard(19)
ReedBCM = 5
line_1_display_text = "   HAPPY   "
line_2_display_text = "Easter Egg Hunt!"
##skip_intro = False
screen = i2c_lcd1602.Screen(bus=1, addr=0x3f, cols=16, rows=2)
##GPIO.setup(ReedBCM, GPIO.IN, pull_up_down=GPIO.PUD_UP)
##GPIO.setmode(GPIO.BOARD)

def init_EasterHunt():

##    time.sleep(1)
    
    line_1_display_text = "   Welcome to   "
    line_2_display_text = "Easter Egg Hunt!"
    screen.cursorTo(0, 0)
    screen.println(line_1_display_text)
    screen.cursorTo(1, 0)
    screen.println(line_2_display_text)
    ##time.sleep(5)

##    for i in range(len(pins)):
##        GPIO.setup(pins[i], GPIO.IN)
##        time.sleep(1)

    screen.enable_backlight()
    time.sleep(1)
    screen.display_init()
    screen.clear()
    

    # Text to display if successful


def destory():
    screen.disable_backlight()
    screen.display_init()
    screen.clear()
    GPIO.cleanup()


def loop():
    skip_intro = False
    while True:
##        print(eggs.value)
##        print(GPIO.input(ReedBCM))
##        if GPIO.input(ReedBCM) == GPIO.HIGH:
        
        if sum(reed.value) == 1:
            line_1_display_list = list("This one is a...")
            line_2_display_list = list("  Special Egg!  ")
        elif sum(eggs.value) == 0 and not skip_intro:
            line_1_display_list = list(" Welcome to the ")
            line_2_display_list = list("Easter Egg Hunt!")
            print(go.value[0])
            if go.value[0] == True:
                skip_intro = True
        elif sum(eggs.value) == 0 and skip_intro:
##            time.sleep(5)
            line1 = "Find all 12 eggs to reveal the surprise   "
            line2 = "Use the Minecraft map to find the eggs   "            
            for i in range(len(line1)):
                line1_text = line1[i:] + line1[:i]
                line2_text = line2[i:] + line2[:i]
                screen.cursorTo(0, 0)
                screen.println(line1_text[:16])
                screen.cursorTo(1, 0)
                screen.println(line2_text[:16])
                if sum(eggs.value) > 0:
                    break
                time.sleep(0.2)
##                print(go.value)
        elif sum(eggs.value) == 12:
##            line_1_display_list = list("-----HAPPY -----")
##            line_2_display_list = list("-----EASTER-----")
            line1 = "There is an even bigger surprise - Use the Reed sensor to find which egg contains the surprise   "
            line2 = "<--- Reed Sensor"
            line1_list = list(line1)
##            line2_list = list(line2)
            for i in range(len(line1)):
                line1_text = line1[i:] + line1[:i]
                line2_text = line2[i:] + line2[:i]
                screen.cursorTo(0, 0)
                screen.println(line1_text[:16])
                screen.cursorTo(1, 0)
                screen.println(line2)
                time.sleep(0.2)
                if sum(reed.value) == 1:
                    break
                
        else:
            line_1_display_list = list("-----000000-----")
            line_2_display_list = list("-----000000-----")
                
            for i in range(len(eggs.value)):
                if i < 6:
                    line_1_display_list[i+5] = str(int(eggs.value[i]))
                else:
                    line_2_display_list[(i%6)+5] = str(int(eggs.value[i]))
    ##            if eggs[i] == 1:
    ##                print("Egg " + str(i) + ": Present")
    ##            else:
    ##                print("Egg " + str(i) + ": Not Present")

        screen.cursorTo(0, 0)
        screen.println(''.join(line_1_display_list))
        screen.cursorTo(1, 0)
        screen.println(''.join(line_2_display_list))

        time.sleep(1)

if __name__ == '__main__':
##   import argparse
##   parser = argparse.ArgumentParser(description='Monitor Raspberry PI temps and post to Power BI')
##   parser.add_argument('--cover_on', metavar='bool', \
##                       required=False, default='N/K', \
##                       help='Is the cover on the raspberry pi case (default: N/K)')
##   parser.add_argument('--heatsink_on', metavar='bool', \
##                       required=False, default='N/K', \
##                       help='Is the heatsink on the raspberry pi processor (default: N/K)')
##   args = parser.parse_args()
   print('starting...')
   init_EasterHunt()
   try:
      loop()
   except KeyboardInterrupt:
      destory()
