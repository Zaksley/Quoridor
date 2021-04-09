GSL_PATH ?= /net/ens/renault/save/gsl-2.6/install
DIR = src
BIN = 
TEST_DIR = tests
TEST_BIN = test_graph_func test_graph_struct

CC ?= cc
CFLAGS = -Wall -Wextra -std=c99 -g -lgcov -I${GSL_PATH}/include
LDFLAGS = -L${GSL_PATH}/lib -ldl
LIBS = -lgsl -lgslcblas -lm

################ Compilation rules #################

all: clean build test

build: ${BIN}

test: build ${TEST_BIN}

alltests: build test
	@for e in ${BIN}; do \
	echo $${e}; ./$${e}; \
	done
	@for e in ${TEST_BIN}; do \
	echo $${e}; ./$${e}; \
	done

install: player_move_random player_random
	${CC} -rdynamic -o install/server ${DIR}/main.c graph_modif.o -ldl ${LIBS} ${CFLAGS} ${LDFLAGS};
#./install/server [-m] [-t] ./install/alltests

################## Binary objects ##################

utils.o : ${DIR}/utils.h ${DIR}/utils.c
	${CC} -fPIC ${CFLAGS} ${DIR}/utils.c -c

graph_modif.o: ${DIR}/graph_modif.h ${DIR}/graph_modif.c
	${CC} -fPIC ${CFLAGS} ${DIR}/graph_modif.c -c


##################### Players ######################

player_random: graph_modif.o utils.o
	${CC} -fPIC -c ${CFLAGS} ${DIR}/player_random.c;
	${CC} -shared -nostartfiles -o install/libplayer_random.so player_random.o graph_modif.o utils.o;

player_move_random: graph_modif.o utils.o
	${CC} -fPIC -c ${CFLAGS} ${DIR}/player_move_random.c;
	${CC} -shared -nostartfiles -o install/libplayer_move_random.so player_move_random.o graph_modif.o utils.o;
###################### Tests #######################

clean:
	rm -f *.o ${BIN}/*.so *~ ${TEST_BIN}

test_graph_struct: graph_modif.o
	${CC} graph_modif.o ${TEST_DIR}/test_graph_struct.c -o $@ ${CFLAGS} ${LDFLAGS} ${LIBS}

test_graph_func: graph_modif.o
	${CC} graph_modif.o ${TEST_DIR}/test_graph_fonc.c -o $@ ${CFLAGS} ${LDFLAGS} ${LIBS}
