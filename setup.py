from distutils.core import setup, Extension

traf_node_module = Extension ('traf_node', sources=['packing.c', 'node.c', 'syn.c', 'server.c', 'shmctl.c'])

setup (name='traf_node', description='node of traffic contral system', ext_modules=[traf_node_module])
