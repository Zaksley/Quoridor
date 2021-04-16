GSL_PATH ?= /net/ens/renault/save/gsl-2.6/install

DIR = src
BIN_DIR = install
TEST_DIR = tests

CC ?= cc
CFLAGS = -Wall -Wextra -std=c99 -g -lgcov -I${GSL_PATH}/include
LDFLAGS = -L${GSL_PATH}/lib -ldl
LIBS = -lgsl -lgslcblas -lm

################ Compilation rules #################

all: clean build

build: install test

test: test_graph_fonc.o test_graph_struct.o test_utils_func.o graph_modif.o utils.o
	${CC} --coverage ${CFLAGS} ${TEST_DIR}/alltests.c graph_modif.o utils.o test_graph_fonc.o test_graph_struct.o test_utils_func.o -o install/alltests  ${LDFLAGS} ${LIBS}

alltests: test
	LD_LIBRARY_PATH=${GSL_PATH}/lib ./install/alltests

install: player_move_random player_random
	${CC} -rdynamic -o install/server ${DIR}/main.c graph_modif.o utils.o -ldl ${LIBS} ${CFLAGS} ${LDFLAGS};
#./install/server [-m] [-t] ./install/alltests

################## Binary objects ##################

utils.o : ${DIR}/utils.h ${DIR}/utils.c
	${CC} -fPIC ${CFLAGS} ${DIR}/utils.c -c

graph_modif.o: ${DIR}/graph_modif.h ${DIR}/graph_modif.c
	${CC} -fPIC ${CFLAGS} ${DIR}/graph_modif.c -c

test_graph_struct.o:
	${CC} ${CFLAGS} ${LDFLAGS} ${LIBS} ${TEST_DIR}/test_graph_struct.c -c

test_graph_fonc.o:
	${CC} ${CFLAGS} ${LDFLAGS} ${LIBS} ${TEST_DIR}/test_graph_fonc.c -c

test_utils_func.o: 
	${CC} ${CFLAGS} ${LDFLAGS} ${LIBS} ${TEST_DIR}/test_utils_func.c -c

##################### Players ######################

player_random: graph_modif.o utils.o
	${CC} -fPIC -c ${CFLAGS} ${DIR}/player_random.c;
	${CC} -shared -nostartfiles -o install/libplayer_random.so player_random.o graph_modif.o utils.o;

player_move_random: graph_modif.o utils.o
	${CC} -fPIC -c ${CFLAGS} ${DIR}/player_move_random.c;
	${CC} -shared -nostartfiles -o install/libplayer_move_random.so player_move_random.o graph_modif.o utils.o;
###################### Tests #######################

clean:
	rm -f *.o ${BIN_DIR}/* *~
