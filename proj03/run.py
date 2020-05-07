#!/bin/env python
import os
import sys
import subprocess

original = sys.stdout
f = open("output.csv", "w")
# redirect to file
sys.stdout = f
print("threads,NUMNODES,average_area,performance")
sys.stdout = original

for t in [1,2,4,6,8,10,12,14,16]:
    for nodes in [10,100,1000,10000]:
        print("NUMT=%d" %t)
        sys.stdout = f
        cmd = "g++ -DNUMTHREADS=%d main.cpp -o proj01 -lm -fopenmp" % (t, nodes)
        os.system(cmd)
        cmd = "./proj01"
        print(subprocess.run([cmd], stdout=subprocess.PIPE).stdout.decode('utf-8'), end = "")
        # restore stdout
        sys.stdout = original

f.close()
