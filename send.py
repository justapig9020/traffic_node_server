from traf_node import *
from time import *

def run ():
    start_server()
    while True:
        a = int (input())
        send_d (0, a)
        if a == -1:
            break
        
    id = stop_server()
    print (id)

run ()
