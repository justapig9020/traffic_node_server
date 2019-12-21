from lcd.lcd import *
from sig.sig import *

if __name__ == '__main__':
    lcd = init ()
    s = SIG ()
    fs = 0
    
    s.sig (fs)
    while True:
        c = input ()
        update (lcd, c)

        fs = fs^1
        s.sig (fs)

        if c == "e":
            break
