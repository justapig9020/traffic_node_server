from traf_node import *
from time import *

def run ():
    start_server()
    while True:
        a = input()
        print ("py msg: " + a)
        send_d (0, a)
        if a == "exit":
            break
        
    id = stop_server()
    print (id)

run ()
