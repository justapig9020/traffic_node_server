from traf_node import *
from time import *
import paho.mqtt.client as mqtt
import math
import numpy as np
import json
import threading
import random
import time
# 0 ahead
# 1 rear
# 2 left
# 3 right

path = np.zeros(4)
pathgo = np.zeros(4)
temp = np.zeros(4)
nowJson = {}
nowJson['ahead'] = 0
nowJson['rear'] = 0
nowJson['left'] = 0
nowJson['right'] = 0
val = 0
d = 0
rl_time = 0
ar_time = 0
ar_count = 0
rl_count = 0
t_sel = 0
sig = 0
stop_id = 0
client = mqtt.Client()
subTopic = "traffic/client/192.168.43.137"
pubTopic = "traffic/server"

def on_connect(client, userdata, flags, rc):
    global subTopic
    print("Server connected")
    client.subscribe(subTopic)

def on_message(client, userdata, msg):
    #client.publish(pubTopic, (msg.payload.decode('utf-8')))
    msgTopic = msg.topic
    msgcont = json.loads(msg.payload.decode('utf-8'))
    print(msgTopic +" "+ json.dumps(msgcont))
    msg_sel(msgcont)
    #distM[0] = msgcont
    #print(method)
def set_V(data):
    global path
    path[int(data['path'])] = data['pathData']
    print("Set Value", data)
    print("now path ", path)
def set_edge(data):
    global path
    for i in data['path']:
        path[int(i)] = int(data['path'][i])
    #print("Set Value", data)
    #print("now path ", path)
def msg_sel(data):
    sel_fun = {
        0 : time_f,
        1 : run,
        2 : lambda value : set_V(value),
        3 : lambda value : set_edge(value),
        4 : run,
        5 : stop
    }

    sel_data = data['open']
    method = sel_fun.get(sel_data,"nothing")
    if(sel_data == 2):
        method(data)
    elif(sel_data == 3):
        method(data)
        time_f()
    elif(sel_data == 4):
        start_server()
        #method()
        t_run = threading.Thread(target = run)
        t_run.start()
    else:
        method()
def con_Init():
    global client
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect("192.168.43.137", 1883)
def con_start():
    global client
    client.loop_forever()
def stop():
    global stop_id
    global path
    global temp
    global nowJson
    global val
    global d
    global ar_time
    global rl_time
    global ar_count
    global rl_count
    global t_sel
    global sig
    stop_id = 1
    path = np.zeros(4)
    temp = np.zeros(4)
    nowJson = {}
    nowJson['ahead'] = 0
    nowJson['rear'] = 0
    nowJson['left'] = 0
    nowJson['right'] = 0
    val = 0
    d = 0
    rl_time = 0
    ar_time = 0
    ar_count = 0
    rl_count = 0
    t_sel = 0
    sig = 0
def time_f():
    global t_sel 
    global ar_count 
    global ar_time 
    global rl_count 
    global rl_time 
    global path
    global sig
    if(t_sel == 0):
        now_path_data = now_path()
        #ar = now_path_data['ahead'] + now_path_data['rear']
        #rl = now_path_data['left'] + now_path_data['right']
        ar = path[0] + path[1]
        rl = path[2] + path[3]
        print(ar , " " , rl)
        
        if(ar+rl == 0):
            ar_time = 50
            rl = 50
        else:
            if(ar > rl):
                ar,rl=rl,ar
            tm = ar / (ar + rl)
            if(tm >= 0.8):
                ar_time = 20
                rl_time = 80
            if(tm <= 0.2):
                ar_time = 80
                rl_time = 20
            else:
                ar_time = round(100 * tm)
                rl_time = round(100 - 100 * tm)
            print(tm)
            print(ar_time)
            print(rl_time)
        t_sel = 1
    elif(t_sel == 1):
        if(sig == 0):
            sys = {}
            sys['ip'] = "192.168.4.20"
            sys['sig'] = 0
            client.publish(pubTopic, json.dumps(sys))
            sig = 1
            #print(1)
        if(ar_count < ar_time):
            ar_count += 1
            print("now ar red time:", ar_time - ar_count)
        else:
            t_sel = 2
            ar_count = 0
    elif(t_sel == 2):
        if(sig == 1):
            sys = {}
            sys['ip'] = "192.168.4.20"
            sys['sig'] = 0
            client.publish(pubTopic, json.dumps(sys))
            sig = 0
            #print(0)
        if(rl_count < rl_time):
            rl_count += 1
            print("now rl red time:", rl_time - rl_count)
        else:
            t_sel = 0
            rl_count = 0
    print("path 0: " , path[0]," path 1: " , path[1]," path 2: " , path[2]," path 3: " , path[3])
    print("temp 0: " , temp[0]," temp 1: " , temp[1]," temp 2: " , temp[2]," temp 3: " , temp[3])
        #time.sleep(1)
def poisson(k,l,m):
    p = (l) ** k * np.exp(-(l)) / math.factorial(k)
    return p*m
def now_path():
    tempJson = {}
    tempJson['ahead'] = 0
    tempJson['rear'] = 0
    tempJson['left'] = 0
    tempJson['right'] = 0
    c = 0
    time.sleep(0.1)
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
    return tempJson
def run ():
    global t_sel 
    global path 
    global stop_id

    #t = threading.Thread(target = time_f)
    #t.start()
    print(path)
    while stop_id == 0:
        r = recv_d (0)
        if(r != ""):
            r = json.loads(r)
            #print(r)
            temp[r['path']] = r['data']
            print("recv:")
            #print("")
            #print(r)
            #print(temp)
            #print("")
            #print("end")
        tempJson = {}
        tempJson['ahead'] = 0
        tempJson['rear'] = 0
        tempJson['left'] = 0
        tempJson['right'] = 0
        tempJson = now_path()

        if(nowJson != tempJson):
            #print("temoJson",tempJson)
            nowJson['ahead'] = tempJson['ahead']
            nowJson['rear'] = tempJson['rear']
            nowJson['left'] = tempJson['left']
            nowJson['right'] = tempJson['right']
            sJson={}
            sJson['path'] = 0
            sJson['data'] = tempJson['rear']
            sJson['open'] = 9
            client.publish("traffic/client/192.168.43.79", json.dumps(sJson))
            
            #send_d(0, json.dumps(sJson))
            '''
            sJson['path'] = 0
            sJson['data'] = tempJson['rear']
            sJson['ptg'] = pathgo[1]
            send_d(1, json.dumps(sJson))
            sJson['path'] = 3
            sJson['data'] = tempJson['left']
            sJson['ptg'] = pathgo[2]
            send_d(2, json.dumps(sJson))
            sJson['path'] = 2
            sJson['data'] = tempJson['right']
            sJson['ptg'] = pathgo[3]
            send_d(3, json.dumps(sJson))
            '''
            #print("JSON",sJson)
            print("send")
            #print(tempJson)
            #print("")
            #print("")
        #input()
        time.sleep(0.1)
    id = stop_server()
    stop_id = 0
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
    con_Init()
    t = threading.Thread(target = con_start)
    t.start()
    while True:
        print("1:setValue")
        print("2:run")
        print("3:exit")
        sel = int(input())
        if(sel == 1):
            setValue()
        
        elif(sel == 2):
            start_server()
            run()
        elif(sel == 3):
            break
        elif(sel == 4):
            rr = {}
            rr["test"] = 1  
            client.publish("traffic/server",  json.dumps(rr))
        elif(sel == 5):
            stop_id = 1
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
