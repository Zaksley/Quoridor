GSL_PATH ?= /net/ens/renault/save/gsl-2.6/install
DIR = src
TEST_DIR = tests
BIN = 
TEST_BIN = test_graph_shape

CC ?= cc
CFLAGS = -Wall -Wextra -std=c99 -g -lgcov -I${GSL_PATH}/include
LDFLAGS = -L${GSL_PATH}/lib -ldl
LIBS = -lgsl -lgslcblas -lm

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


player: graph_modif.o utils.o
	${CC} -fPIC -c ${DIR}/player.c;
	${CC} -shared -nostartfiles -o install/libplayer.so player.o graph_modif.o utils.o;
	${CC} -rdynamic -o main ${DIR}/main.c graph_modif.o -ldl ${LIBS};	

clean:
	rm -f *.o ${BIN}/*.so *~ ${TEST_BIN}

utils.o : graph_modif.o
	${CC} -fPIC ${CFLAGS} ${DIR}/utils.c graph_modif.o -c

graph_modif.o:
	${CC} -fPIC ${CFLAGS} ${DIR}/graph_modif.c -c

test_graph_shape: graph_modif.o
	${CC} graph_modif.o ${TEST_DIR}/test_graph_shape.c -o $@ ${CFLAGS} ${LDFLAGS} ${LIBS}
