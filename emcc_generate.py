from __future__ import print_function  # Only needed for Python 2
import glob, os

f = open('emcc_build.bat', 'w')
f.write('mkdir -p webbuild\n')
f.write('rm -r webbuild/*\n')
f.write('emcc ')
f.write('-O3 ')

os.chdir("src")
for file in glob.glob("*.cpp"):
    f.write('src/' + file + " ")

f.write('-I include -s WASM=1 -s DISABLE_EXCEPTION_CATCHING=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s ALLOW_MEMORY_GROWTH=1 --use-preload-plugins --preload-file resources -o webbuild/main.html --std=c++14 -DCHAISCRIPT_NO_THREADS')

f.close() 
