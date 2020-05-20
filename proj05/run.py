#!/bin/env python3
import os
import sys
import subprocess

original = sys.stdout
f = open("output.csv", "w")
# redirect to file
sys.stdout = f
print("number of trials,megatrials per second,probability")
sys.stdout = original

for t in [16*1024, 32*1024, 64*1024, 128*1024, 256*1024, 512*1024, 1000*1024]:
    for blck in [16,32,64,128]:
        print("NUMT=%d" %t)
        sys.stdout = f
        cmd = "/usr/local/apps/cuda/cuda-10.1/bin/nvcc -o proj05 main.cu -DNUMTRIALS=%d -DBLOCKSIZE=%d" % (t,
				blck)
        os.system(cmd)
        cmd = "./proj05"
        print(subprocess.run([cmd], stdout=subprocess.PIPE).stdout.decode('utf-8'), end = "")
        # restore stdout
        sys.stdout = original
f.close()
