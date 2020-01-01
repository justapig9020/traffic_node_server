import paho.mqtt.client as mqtt
import numpy as np
import threading
import time
import array
import json
import matplotlib.pyplot as plt
from simu import *
table={}
distM={}
	


plt.ion()
#plt.figure()
fig , ax = plt.subplots()
fig.subplots_adjust(hspace=0.4, wspace=0.4)
#ax1 = fig.add_subplot(2,2,1)
#ax2 = fig.add_subplot(2,2,2)
tk = [0]
tk2=[0]
t_now = 0
m = [0]
mk = [0]
m2 = [0]
mk2 = [0]
client = mqtt.Client()
subTopic = "traffic/server"
pubTopic = "traffic/client/192.168.4.20"
c_ar = 0
c_rl = 0
count = 0
def on_connect(client, userdata, flags, rc):
    print("Server connected")
    client.subscribe("traffic/server")

def on_message(client, userdata, msg):
    global t_now
    #client.publish(pubTopic, (msg.payload.decode('utf-8')))
    msgTopic = msg.topic
    msgcont = json.loads(msg.payload.decode('utf-8'))
    #distM[0] = msgcont
    print(msgcont['sig'])
    if(msgcont['ip'] == "192.168.4.83"):
        if(msgcont['sig'] == 9):
            #plt.clf()
            plt.subplot(2, 1, 1)
            #plt.clf()
            #plt.figure(1)
            t_now += 1
            tk.append(t_now)
            print(c_ar + c_rl)
            if((c_ar + c_rl) == 0):
                m.append(0)
            else:
                m.append(c_ar / (c_ar + c_rl))

            if((msgcont['ar'] + msgcont['rl']) == 0):
                ave = 0
            else:
                ave = msgcont['ar'] / (msgcont['ar'] + msgcont['rl'])
            mk.append(ave)
            #m.append(t_now)
            plt.plot(tk,m, '-', color='RosyBrown', label='Cosine')
            plt.plot(tk,mk, '-', color='SteelBlue', label='Sine')
            plt.legend(['traffic','signal'])
            plt.xlabel("time", fontsize=12)
            plt.ylabel("rate", fontsize=12)
            plt.pause(0.01)
    if(msgcont['ip'] == "192.168.4.240"):
        if(msgcont['sig'] == 9):
            #plt.clf()
            plt.subplot(2, 1, 2)
            #plt.clf()
            t_now += 1
            tk2.append(t_now)
            print(c_ar + c_rl)
            if((c_ar + c_rl) == 0):
                m2.append(0)
            else:
                m2.append(c_ar / (c_ar + c_rl))

            if((msgcont['ar'] + msgcont['rl']) == 0):
                ave = 0
            else:
                ave = msgcont['ar'] / (msgcont['ar'] + msgcont['rl'])
            mk2.append(ave)
            #m.append(t_now)
            plt.plot(tk2,m2, '-', color='RosyBrown', label='Cosine')
            plt.plot(tk2,mk2, '-', color='SteelBlue', label='Sine')
            plt.legend(['traffic','signal'])
            plt.xlabel("time", fontsize=12)
            plt.ylabel("rate", fontsize=12)
            plt.pause(0.01)

    if(msgcont['sig'] != 9):
        for i in table:
            if(table[i] == msgcont['ip']):
                if(msgcont['sig'] == 0):
                    chg_sig(i,1)
                    #print("")
                elif(msgcont['sig'] == 1):
                    #print("")
                    chg_sig(i,0)
    print(msgTopic +" "+ json.dumps(msgcont))

def con_Init():
    global client
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect("192.168.4.18", 1883)
def con_start():
    global client
    client.loop_forever()

def sim_Init():
    chg_sig (0, 0)
    chg_sig (1, 0)
    chg_sig (2, 0)
    chg_sig (3, 0)
    chg_sig (4, 0)
    chg_sig (5, 0)
    chg_sig (6,0)
    chg_sig (7,0)
def table_Init():
    global distM
    fp = open('conf/ip_table', "r")
    count = int(fp.readline())
    for i in range(0,count):
        line = fp.readline()
        table[i] = line.replace("\n","")
        distM[i] = ""
    fp.close()
    print(distM)
    print(table)
def node_all_run():
    r = {}
    r['open'] = 4
    for i in table:
        client.publish("traffic/client/"+ table[i], json.dumps(r))
def node_all_stop():
    r = {}
    r['open'] = 5
    for i in table:
        client.publish("traffic/client/"+ table[i], json.dumps(r))
def run():
    while True:
        sim_update()
        node_update()
        print("table 5",get_sig(5))
        show_sm()
        time.sleep(0.1)
def node_update():
    global c_ar
    global c_rl
    edge = {}
    edge['open'] = 3
    edge['path'] = {}
    for i in table:
        edge['ip'] = table[i]
        for o in range(0,4):
            edge['path'][o] = get_cont(i, o)
        client.publish("traffic/client/"+ table[i], json.dumps(edge))
    c_ar += get_cont(2, 0)+get_cont(2, 1)
    c_rl += get_cont(2, 2)+get_cont(2, 3)
if __name__ == "__main__":
    table_Init()
    sim_start()
    sim_Init()
    con_Init()
    t = threading.Thread(target = con_start)
    t.start()
    plt.clf()
    #for i in range(1,3):
    plt.subplot(2, 1, 1)
    plt.subplot(2,1,2)
        #plt.text(0.5, 0.5, str((2, 2, i)), fontsize=18, ha='center')
    #plt.plot(tk,m, '-', color='RosyBrown', label='Cosine')
    #plt.plot(t,m, '.', color='SteelBlue', label='Sine')
    plt.pause(0.01)
    #distM[0]= "{'test':1}"
    #print(distM)

    print("main_start")
    while True:
        a = input()
        if(a == '1'):
            print("open 1")
            r = {}
            r['open'] = 0
            client.publish(pubTopic, json.dumps(r))
        elif(a == 'a'):
            print("open 1")
            r = {}
            r['open'] = 0
            for i in range(0,2):#table:
                print(table[i])
                client.publish("traffic/client/"+ table[i], json.dumps(r))
        elif(a == '2'):
            print("open 2")
            r = {}
            r['open'] = 2
            print('path')
            b = input()
            r['path'] = b
            b = input()
            r['pathData'] = b
            client.publish(pubTopic, json.dumps(r))
        elif(a == 'u'):
            node_update()
        elif(a == 'stop'):
            print(5)
            r ={}
            r['open'] = 5
            for i in table:
                client.publish("traffic/client/"+ table[i], json.dumps(r))
        elif(a == 'run'):
            print("system start")
            node_all_run()
            t2 = threading.Thread(target = run)
            t2.start()
        elif(a == 'exit'):
            break
        elif(a == 'stop'):
            sim_stop()
            node_all_stop()
        elif(a == 'print'):
            for i in range(2000):
                plt.clf()

                t_now += 1
                tk.append(t_now)
                print(c_ar + c_rl)
                if((c_ar + c_rl) == 0):
                    m.append(0)
                else:
                    m.append(c_ar / (c_ar + c_rl))
                #m.append(t_now)
                plt.plot(tk,m, '-', color='RosyBrown', label='Cosine')
                #plt.plot(t,m, '.', color='SteelBlue', label='Sine')
                plt.pause(0.01)

            
    
'''
while True:
    a = input ()
    sim_update ()
    show_sm ()

'''
'''
    print (a)

    if a == "e":
        break
    elif a == "c":
        n = n^1
        chg_sig (2, n)
    elif a == "s":
        for i in range(0,5):
            print ("node " + str(i) + " sig: " + str(get_sig(i)))
        continue
    elif a == "o":
        for i in range(0,5):
            for o in range(0,4):
                print ("node " + str(i) + "edge " + str(o) + " cont: " + str(get_cont(i, o)))
        continue
    sim_update ()
'''
