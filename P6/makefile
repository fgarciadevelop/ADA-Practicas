#Francisco García Mora 48780988N
OPTS = -Wall -O3 -std=c++0x

maxsum: maxsum.cc
	g++ ${OPTS} -o maxsum maxsum.cc	 

all: maxsum

tar: maxsum.cc makefile
	tar cvfz 48780988N.tar.gz maxsum.cc makefile

clean: 
	rm maxsum 48780988N.tar.gz
