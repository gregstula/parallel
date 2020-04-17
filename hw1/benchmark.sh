#!/bin/bash
SINGLE=1
MULTI=4
TRIES=1000

g++ -Wall -std=c++11 -O1 -DNUMT=$SINGLE -DNUM_TRIES=$TRIES -fopenmp  main.cpp -o project0-single
./project0-single
read -r savg<results
echo
echo
g++ -Wall -std=c++11 -O1 -DNUMT=$MULTI -DNUM_TRIES=$TRIES -fopenmp  main.cpp -o project0-multi
./project0-multi
read -r mavg<results

echo
echo
echo "MULTAVG $mavg"
echo "SINGLAVG $savg"
echo

S=$(echo "print(${mavg}/${savg})" | python3)
echo "S is ${S}"

FP=$( echo "print( (4.0 / 3.0 ) * 1.0 - ( 1.0 - ${S} ) )" | python3)
echo
echo "FP IS ${FP}"
