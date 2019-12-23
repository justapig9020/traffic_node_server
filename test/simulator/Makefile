PYLIB = simu.cpython-37m-darwin.so
DIR = ../../src/simulator/
CONF = conf/

all: $(CONF) $(PYLIB)

$(CONF):
	cp -rf $(DIR)$(CONF) $(CONF)

$(PYLIB):
	make -C $(DIR)
	cp $(DIR)$(PYLIB) ./

run: all
	python3 traffic_sim.py

clean:
	rm -rf $(CONF) $(PYLIB)
