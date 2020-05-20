#!/bin/bash
#SBATCH -J Project05
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH --gres=gpu:1
#SBATCH -o proj05.out
#SBATCH -e proj05.err
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=stulag@oregonstate.edu
python3 run.py
