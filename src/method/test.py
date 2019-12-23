from traf_node import *
from time import *
import math
import numpy as np
import json
import threading
# 0 ahead
# 1 rear
# 2 left
# 3 right

path = np.zeros(4)
temp = np.zeros(4)
nowJson = {}
nowJson['ahead'] = 25
nowJson['rear'] = 25
nowJson['left'] = 25
nowJson['right'] = 25
val = 0
d = 0
f_stop = threading.Event()
time = 10
def f(f_stop):
    ar = nowJson['ahead'] + nowJson['rear']
    rl = nowJson['left'] + nowJson['right']
    if(ar > rl):
       ar,rl=rl,ar
    t = ar / (ar + rl)
    
    print(100 * t)
    print(100- 100 * t)
    if not f_stop.is_set():
        threading.Timer(time, f, [f_stop]).start()
def poisson(k,l,m):
    l1 = 3/5
    l2 = 0
    l3 = 0
    p = (l) ** k * np.exp(-(l)) / math.factorial(k)
    return p*m

def run ():
    start_server()
    f(f_stop)
    print(path)
    while True:

        r = recv_d (0)
        if(r != ""):
            r = json.loads(r)
            temp[r['path']] = r['data']
            print("recv:")
            print("")
            print(r)
            print(temp)
            print("")
            print("end")
        tempJson = {}
        tempJson['ahead'] = 0
        tempJson['rear'] = 0
        tempJson['left'] = 0
        tempJson['right'] = 0
        c = 0
        for p in path:
            v = p + temp[c]
            aps = poisson(0, 3/5, v)
            rl = (v - aps) / 2;
            if(c == 0):
                tempJson['rear'] += aps
                tempJson['left'] += rl
                tempJson['right'] += rl
            elif(c == 1):
                tempJson['ahead'] += aps
                tempJson['left'] += rl
                tempJson['right'] += rl
            elif(c == 2):
                tempJson['right'] += aps
                tempJson['ahead'] += rl
                tempJson['rear'] += rl
            elif(c == 3):
                tempJson['left'] += aps
                tempJson['ahead'] += rl
                tempJson['rear'] += rl
            c += 1
        nowJson['ahead'] = tempJson['ahead']
        nowJson['rear'] = tempJson['rear']
        nowJson['left'] = tempJson['left']
        nowJson['right'] = tempJson['right']
        sJson = {}
        sJson['path'] = 1
        sJson['data'] = tempJson['ahead']
        print(sJson)
        print("")
        print(tempJson)
        print("")
        print("")
        send_d(0, json.dumps(sJson))
        input()
    id = stop_server()
    print (id)
def setValue():
    print("0:ahead")
    print("1:rear")
    print("2:left")
    print("3:right")
    d = int(input())
    val = int(input())
    path[d] = val
def main():
    while True:
        print("1:setValue")
        print("2:run")
        print("3:exit")
        sel = int(input())
        if(sel == 1):
            setValue()
        elif(sel == 2):
            run()
        elif(sel == 3):
            break
main()


'''
def run ():
    start_server()
    while True:
        a = input()
        print ("py msg: " + a)
        send_d (0, a)
        if a == "exit":
            break
'''       

'''
        sJson['path'] = 0
        sJson['data'] = tempJson['rear']
        send_d(1, json.dumps(sJson))
        sJson['path'] = 3
        sJson['data'] = tempJson['left']
        send_d(2, json.dumps(sJson))
        sJson['path'] = 2
        sJson['data'] = tempJson['right']
        send_d(3, json.dumps(sJson))
        if a == "exit":
            break
''' 
