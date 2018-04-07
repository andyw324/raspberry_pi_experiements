import time
import i2c_lcd1602

screen = i2c_lcd1602.Screen(bus=1, addr=0x3f, cols=16, rows=2)

line1 = 'Some test text'
line2 = 'some more test text'


def destory():
    screen.disable_backlight()
    screen.display_init()
    screen.clear()

def init():
    screen.enable_backlight()
    time.sleep(1)
    screen.display_init()
    screen.clear()

def loop():

    while True:
        line1_list = list(line1)
        line2_list = list(line2)
        for i in range(len(line1)):
            line1_text = line1[i:] + line1[:i]
            line2_text = line2[i:] + line2[:i]
            screen.cursorTo(0, 0)
            screen.println(line1_text[:16])
            screen.cursorTo(1, 0)
            screen.println(line2_text[:16])
            time.sleep(0.5)

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
    
    try:
      init()
      loop()
    except KeyboardInterrupt:
      destory()
