from traf_node import *
from time import *

def run ():
    start_server ()
    while True:
        sleep (1)
        a = recv_d (0)
        print (a)
        if a == "exit":
            break
    ret = stop_server ()
    print (ret)

run ()
