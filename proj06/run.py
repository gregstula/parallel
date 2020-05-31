#!/bin/env python3
import os
import sys
import subprocess

print("mult")
# mult
original = sys.stdout
f = open("output-mult.csv", "w")
# redirect to file
sys.stdout = f
print("Local Size,Global Size,Performance")
sys.stdout = original

for loc in [8, 16, 32, 64, 128, 256, 512]:
    for glob in [1024, 4096, 8192, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 3145728, 4194304, 5242880, 6291456, 7340032,8388608]:
        print("local size=%d" %loc)
        sys.stdout = f
        cmd = "g++ -DLOCAL_SIZE=%d -DGLOBAL_SIZE=%d -o mult mult.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp" % (loc, glob)
        os.system(cmd)
        cmd = "./mult"
        print(subprocess.run([cmd], stdout=subprocess.PIPE).stdout.decode('utf-8'), end = "")
        # restore stdout
        sys.stdout = original
f.close()

print("multadd")
# multadd
original = sys.stdout
f = open("output-multadd.csv", "w")
# redirect to file
sys.stdout = f
print("Local Size,Global Size,Performance")
sys.stdout = original

for loc in [8, 16, 32, 64, 128, 256, 512]:
    for glob in [1024, 4096, 8192, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 3145728, 4194304, 5242880, 6291456, 7340032,8388608]:
        print("local size=%d" %loc)
        sys.stdout = f
        cmd = "g++ -DLOCAL_SIZE=%d -DGLOBAL_SIZE=%d -o multadd multadd.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp" % (loc, glob)
        os.system(cmd)
        cmd = "./multadd"
        print(subprocess.run([cmd], stdout=subprocess.PIPE).stdout.decode('utf-8'), end = "")
        # restore stdout
        sys.stdout = original
f.close()

print("reduce")
#reduce
original = sys.stdout
f = open("output-reduce.csv", "w")
# redirect to file
sys.stdout = f
print("Local Size,Global Size,Performance")
sys.stdout = original

for loc in [8, 16, 32, 64, 128, 256, 512]:
    for glob in [1024, 4096, 8192, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 3145728, 4194304, 5242880, 6291456, 7340032,8388608]:
        print("local size=%d" %loc)
        sys.stdout = f
        cmd = "g++ -DLOCAL_SIZE=%d -DGLOBAL_SIZE=%d -o reduce reduce.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp" % (loc, glob)
        os.system(cmd)
        cmd = "./reduce"
        print(subprocess.run([cmd], stdout=subprocess.PIPE).stdout.decode('utf-8'), end = "")
        # restore stdout
        sys.stdout = original
f.close()
