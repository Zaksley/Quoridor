GSL_PATH ?= /net/ens/renault/save/gsl-2.6/install
DIR = src
TEST_DIR = tests
BIN = 
TEST_BIN = test_graph_shape test_graph_fonc test_graph_struct

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

install: player player_random
	${CC} -rdynamic -o install/server ${DIR}/main.c graph_modif.o -ldl ${LIBS};
	

	#./install/server [-m] [-t] ./install/alltests


player: graph_modif.o utils.o
	${CC} -fPIC -c ${DIR}/player.c;
	${CC} -shared -nostartfiles -o install/libplayer.so player.o graph_modif.o utils.o;
		


player_random: graph_modif.o utils.o
	${CC} -fPIC -c ${DIR}/player_random.c;
	${CC} -shared -nostartfiles -o install/libplayer_random.so player_random.o graph_modif.o utils.o;
		

clean:
	rm -f *.o ${BIN}/*.so *~ ${TEST_BIN}

utils.o : ${DIR}/utils.h ${DIR}/utils.c
	${CC} -fPIC ${CFLAGS} ${DIR}/utils.c -c

graph_modif.o: ${DIR}/graph_modif.h ${DIR}/graph_modif.c
	${CC} -fPIC ${CFLAGS} ${DIR}/graph_modif.c -c

test_graph_shape: graph_modif.o
	${CC} graph_modif.o ${TEST_DIR}/test_graph_shape.c -o $@ ${CFLAGS} ${LDFLAGS} ${LIBS}

test_graph_struct: graph_modif.o
	${CC} graph_modif.o ${TEST_DIR}/test_graph_struct.c -o $@ ${CFLAGS} ${LDFLAGS} ${LIBS}

test_graph_fonc: graph_modif.o
	${CC} graph_modif.o ${TEST_DIR}/test_graph_fonc.c -o $@ ${CFLAGS} ${LDFLAGS} ${LIBS}
