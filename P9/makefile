#Francisco García Mora 48780988N
OPTS = -Wall -O3 -std=c++0x

maxsum-bb: maxsum-bb.cc
	g++ ${OPTS} -o maxsum-bb maxsum-bb.cc	 

filter: filter.cc
	g++ ${OPTS} -o filter filter.cc	 

all: maxsum-bb filter

tar: maxsum-bb.cc makefile
	tar cvfz 48780988N.tar.gz maxsum-bb.cc makefile

clean: 
	rm maxsum-bb 48780988N.tar.gz filter
