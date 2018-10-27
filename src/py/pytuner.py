#!/usr/bin/python
#

import curses
import serial
import json

log_rows = 12
status_space = 20

def main_window(y,x):
    win1_rows = y - log_rows
    win1 = stdscr.subwin(win1_rows,0,0,0)
    win1.box()
    win1.addstr(1,1,"Press q to quit")
    win1.bkgd(' ', curses.color_pair(1))
    win1.refresh()
    return win1

def log_window(y,x):
    log_row = y - log_rows
    log_col = 0

    win2 = stdscr.subwin(log_rows, x, log_row, log_col)
    win2.box()
    win2.addstr(1,1,"log..", curses.color_pair(1))
    win2.bkgd(' ', curses.color_pair(2))
    win2.refresh()
    return win2

log_buf = []
def log_display(win, l):
    log_buf.append(l)
    if (len(log_buf) > (log_rows-2)):
      log_buf.pop(0)
    win.clear()
    win.box()
    for i in range(0,len(log_buf)):
      win.addstr(i+1,1,log_buf[i],curses.color_pair(1))
    win.bkgd(' ', curses.color_pair(2))
    win.refresh()

current_status = {}
def status_display(win, status):
    win.clear()
    win.box()
    current_status.update(status)
    itms = sorted(current_status.items())
    for i in range(0,len(current_status)):
      s = "%10s:%9s" % (itms[i][0], str(itms[i][1]))
      win.addstr(1 + i/2, 1 + (i%2)*status_space, s, curses.color_pair(1))
    
    win.bkgd(' ', curses.color_pair(1))
    win.refresh()


if __name__ == "__main__":
    
    port = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
    print port.name
    
    NO_KEY = -1

    stdscr = curses.initscr()
    stdscr.nodelay(True)
    stdscr.keypad(True)
    stdscr.refresh()

    curses.noecho()
    curses.curs_set(0)

    # Colors
    if curses.has_colors():
        curses.start_color()
        curses.init_pair(1, curses.COLOR_WHITE, curses.COLOR_BLUE)
        curses.init_pair(2, curses.COLOR_WHITE, curses.COLOR_RED)

    y, x = stdscr.getmaxyx()
    mw = main_window(y, x)
    lw = log_window(y, x)

    key = NO_KEY
    while key != ord('q'):
        key = stdscr.getch()

        if key == curses.KEY_RESIZE:
            stdscr.erase()
            y, x = stdscr.getmaxyx()
            mw = main_window(y, x)
            lw = log_window(y, x)

        stdscr.refresh()

        l = port.readline()
        if len(l) > 2:
          try:
            j = json.loads(l)
            if 'log' in j:
              log_display(lw,j['log']['msg'])
            if 'config' in j:
              log_display(lw,"local: config received")
            if 'status' in j:
              status_display(mw,j['status'])
          except ValueError as v:
              None
              #log_display(lw,l)
        
    curses.endwin()

