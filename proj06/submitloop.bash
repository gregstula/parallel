#!/bin/bash
#SBATCH -J Project06
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH --gres=gpu:1
#SBATCH -o proj06.out
#SBATCH -e proj06.err
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=stulag@oregonstate.edu
python3 run.py
