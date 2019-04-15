from __future__ import print_function
import glob, os, sys

projdir = "projects/samples/basic"
if len(sys.argv) > 1:
	projdir = sys.argv[1]

f = open('emcc_build.bat', 'w')
f.write('mkdir -p webbuild\n')
f.write('rm -r webbuild/*\n')

# EMCC_DEBUG
#f.write('set EMCC_DEBUG=2')

f.write('emcc ')
f.write('-O3 ')

# Ugly! fix later:
os.chdir("Game/src")
for file in glob.glob("*.cpp"):
    f.write('Game/src/' + file + " ")

os.chdir("../../Retro3D/src")
for file in glob.glob("*.cpp"):
    f.write('Retro3D/src/' + file + " ")
for file in glob.glob("**/*.cpp"):
    f.write('Retro3D/src/' + file + " ")
for file in glob.glob("**/**/*.cpp"):
    f.write('Retro3D/src/' + file + " ")
for file in glob.glob("**/**/**/*.cpp"):
    f.write('Retro3D/src/' + file + " ")
for file in glob.glob("**/**/**/**/*.cpp"):
    f.write('Retro3D/src/' + file + " ")

# ALLOW_MEMORY_GROWTH=0 seems to cause a crash with SDL & TTF
f.write('-I include -I Retro3D/src -s WASM=1 -s DISABLE_EXCEPTION_CATCHING=0 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s ALLOW_MEMORY_GROWTH=0 -s TOTAL_MEMORY=167772160 --use-preload-plugins --preload-file ' + projdir + '/resources@resources -o webbuild/main.html --std=c++14 -DCHAISCRIPT_NO_THREADS -DCHAISCRIPT_NO_THREADS_WARNING')

# Debug parameters
#f.write('-s ASSERTIONS=1 -s SAFE_HEAP=1 -s SAFE_HEAP_LOG=1 -s STACK_OVERFLOW_CHECK=2')

f.close() 
