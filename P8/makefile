#Francisco García Mora 48780988N
OPTS = -Wall -O3 -std=c++0x

maxsum-bt: maxsum-bt.cc
	g++ ${OPTS} -o maxsum-bt maxsum-bt.cc	 

all: maxsum-bt

tar: maxsum-bt.cc makefile
	tar cvfz 48780988N.tar.gz maxsum-bt.cc makefile

clean: 
	rm maxsum-bt 48780988N.tar.gz
