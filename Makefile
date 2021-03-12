GSL_PATH ?= /net/ens/renault/save/gsl-2.6/install
DIR = src
BIN = 
TEST_BIN = 

CC ?= cc
CFLAGS = -Wall -Wextra -std=c99 -g -lgcov

all: build test

build: ${BIN}

test: build ${TEST_BIN}

alltests: build test
	@for e in ${BIN}; do \
	echo $${e}; ./$${e}; \
	done
	@for e in ${TEST_BIN}; do \
	echo $${e}; ./$${e}; \
	done

install:
	./install/server [-m] [-t] ./install/alltests


player:
	${CC} -fPIC -c ${DIR}/player.c;
	${CC} -shared -nostartfiles -o install/libplayer.so player.o;
	${CC} -rdynamic -o main ${DIR}/main.c -ldl;	

clean:
	rm -f *.o ${BIN} *~ 
