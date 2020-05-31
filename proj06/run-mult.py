#!/bin/env python
import os
import sys
import subprocess

original = sys.stdout
f = open("output-mult.csv", "w")
# redirect to file
sys.stdout = f
print("threads,NUMNODES,average_area,performance")
sys.stdout = original

for loc in [1,2,4,6,8,10,12,14,16]:
    for glob in [10,100,1000,10000]:
        print("NUMT=%d" %t)
        sys.stdout = f
        cmd = "g++ -DLOCAL_SIZE=%d -DGLOBAL_SIZE=%d -o mult mult.cpp
        /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp" % (loc, glob)
        os.system(cmd)
        cmd = "./proj01"
        print(subprocess.run([cmd], stdout=subprocess.PIPE).stdout.decode('utf-8'), end = "")
        # restore stdout
        sys.stdout = original

f.close()
