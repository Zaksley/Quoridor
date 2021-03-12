GSL_PATH ?= /net/ens/renault/save/gsl-2.6/install
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


player: player.c
	${CC} -fPIC -c $@;
	${CC} -shared -nostartfiles -o libplayer.so player.o;
	${CC} -rdynamic -o main main.c -ldl;	

clean:
	rm -f *.o ${BIN} *~
