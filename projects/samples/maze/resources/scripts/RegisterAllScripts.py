import glob, os
from fnmatch import fnmatch

import glob, os

f = open('ScriptFiles.txt', 'w')

pattern = "*.chai"

for (root, dirs, files) in os.walk(os.curdir):
	path = root.split(os.sep)
	for file in files:
		if (fnmatch(file, pattern)):
			f.write((os.path.join(root, file)).replace(os.curdir + "\\","").replace("\\","//"))
			f.write("\n")
			
			