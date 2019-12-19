from distutils.core import setup, Extension

simu_module = Extension ('simu', sources=['packing.c', 'sim.c', 'method.c', 'libsim.c'])

setup (name='simu', description='traffic simulator', ext_modules=[simu_module])
