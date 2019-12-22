from simu import *

sim_start ()
n = 0
chg_sig (0, 0)
chg_sig (1, 0)
chg_sig (2, 0)
chg_sig (3, 0)
chg_sig (4, 0)

set_fs_func (0, 1, 2)
set_fs_func (4, 1, 2)
set_fs_func (1, 0, 2)
set_fs_func (3, 0, 2)

while True:
    a = input ()
    print (a)

    if a == "e":
        break
    elif a == "c":
        n = n^1
        for i in range(0, 5):
            chg_sig (i, n)
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
    show_sm ()

print ("Bye")
sim_stop ()