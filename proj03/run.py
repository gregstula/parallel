#!/bin/env python
import os
import sys
import subprocess

original = sys.stdout
f = open("output.csv", "w")
# redirect to file
sys.stdout = f
print("month,rain,temp,height,deer")
sys.stdout = original
sys.stdout = f
cmd = "g++ main.cpp -o proj01 -lm -fopenmp"
os.system(cmd)
cmd = "./proj01"
print(subprocess.run([cmd], stdout=subprocess.PIPE).stdout.decode('utf-8'), end = "")
# restore stdout
sys.stdout = original
f.close()
