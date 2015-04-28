#!/bin/bash

capURL="http://www.cse.unr.edu/~mircea/Teaching/cs485_685/Programming/PA4/Data/cap"
pixelURL="http://www.cse.unr.edu/~mircea/Teaching/cs485_685/Programming/PA4/Data/pixel"
worldURL="http://www.cse.unr.edu/~mircea/Teaching/cs485_685/Programming/PA4/Data/world"

for i in $(seq -f "%02g" 1 15)
do
	wget $capURL$i.bmp
	wget $pixelURL$i.txt
	wget $worldURL$i.txt
done
