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

for t in [16*1024, 32*1024, 64*1024, 128*1024, 256*1024, 512*1024, 1000*1024]:
    for blck in [16,32,64,128]:
        print("NUMT=%d" %t)
        sys.stdout = f
        cms = f"{cudaNVCC} -o proj05 CUDAmonteCarlo.cu -DNUMTRIALS={trial} -DBLOCKSIZE={blockSize}"
        os.system(cmd)
        cmd = "./proj05"
        print(subprocess.run([cmd], stdout=subprocess.PIPE).stdout.decode('utf-8'), end = "")
        # restore stdout
        sys.stdout = original
f.close()
