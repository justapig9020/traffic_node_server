from simu import *

sim_start ()
n = 0
chg_sig (0, 0)
chg_sig (1, 1)
chg_sig (2, n)
chg_sig (3, n)
chg_sig (4, 0)
chg_sig (5, 0)
chg_sig (6, 1)
chg_sig (7, 0)

while True:
    a = input ()
    print (a)

    if a == "e":
        break
    elif a == "c":
        n = n^1
        chg_sig (2, n)
        chg_sig (3, n)
    elif a == "s":
        for i in range(0,8):
            print ("node " + str(i) + " sig: " + str(get_sig(i)))
        continue
    elif a == "o":
        for i in range(0,8):
            for o in range(0,4):
                print ("node " + str(i) + "edge " + str(o) + " cont: " + str(get_cont(i, o)))
        continue
    sim_update ()
    show_sm ()

print ("Bye")
sim_stop ()
