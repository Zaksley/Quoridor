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

test: test_graph_fonc.o test_graph_struct.o test_graph_shape.o test_utils_func.o test_strategy_func.o graph_modif.o utils.o strategy.o
	${CC} --coverage ${CFLAGS} ${TEST_DIR}/alltests.c graph_modif.o utils.o strategy.o test_graph_fonc.o test_graph_struct.o test_graph_shape.o test_utils_func.o test_strategy_func.o -o install/alltests  ${LDFLAGS} ${LIBS}

alltests: test
	LD_LIBRARY_PATH=${GSL_PATH}/lib ./install/alltests

install: player_jerry player_morty player_rick_J19Z7 player_rick_DIII player_rick_C137 #player_rick_evil
	${CC} -rdynamic -o install/server ${DIR}/main.c graph_modif.o utils.o -ldl ${LIBS} ${CFLAGS} ${LDFLAGS};

################## Binary objects ##################

utils.o : ${DIR}/utils.h ${DIR}/utils.c
	${CC} -fPIC ${CFLAGS} ${DIR}/utils.c -c

strategy.o : ${DIR}/strategy.h ${DIR}/strategy.c
	${CC} -fPIC ${CFLAGS} ${DIR}/strategy.c -c

graph_modif.o: ${DIR}/graph_modif.h ${DIR}/graph_modif.c
	${CC} -fPIC ${CFLAGS} ${DIR}/graph_modif.c -c

test_graph_struct.o:
	${CC} ${CFLAGS} ${LDFLAGS} ${LIBS} ${TEST_DIR}/test_graph_struct.c -c

test_graph_fonc.o:
	${CC} ${CFLAGS} ${LDFLAGS} ${LIBS} ${TEST_DIR}/test_graph_fonc.c -c

test_graph_shape.o:
	${CC} ${CFLAGS} ${LDFLAGS} ${LIBS} ${TEST_DIR}/test_graph_shape.c -c

test_utils_func.o: 
	${CC} ${CFLAGS} ${LDFLAGS} ${LIBS} ${TEST_DIR}/test_utils_func.c -c

test_strategy_func.o: 
	${CC} ${CFLAGS} ${LDFLAGS} ${LIBS} ${TEST_DIR}/test_strategy_func.c -c

##################### Players ######################

player_morty: graph_modif.o utils.o strategy.o
	${CC} -fPIC -c ${CFLAGS} ${DIR}/player_morty.c;
	${CC} -shared -nostartfiles -o install/Morty.so player_morty.o graph_modif.o utils.o strategy.o;

player_jerry: graph_modif.o utils.o strategy.o 
	${CC} -fPIC -c ${CFLAGS} ${DIR}/player_jerry.c;
	${CC} -shared -nostartfiles -o install/Jerry.so player_jerry.o graph_modif.o utils.o strategy.o;

player_rick_J19Z7: graph_modif.o utils.o strategy.o
	${CC} -fPIC -c ${CFLAGS} ${DIR}/player_rick_J19Z7.c;
	${CC} -shared -nostartfiles -o install/Rick_J19Z7.so player_rick_J19Z7.o graph_modif.o utils.o strategy.o;

player_rick_DIII: graph_modif.o utils.o strategy.o
	${CC} -fPIC -c ${CFLAGS} ${DIR}/player_rick_DIII.c;
	${CC} -shared -nostartfiles -o install/Rick_DIII.so player_rick_DIII.o graph_modif.o utils.o strategy.o;

player_rick_C137: graph_modif.o utils.o strategy.o
	${CC} -fPIC -c ${CFLAGS} ${DIR}/player_rick_C137.c;
	${CC} -shared -nostartfiles -o install/Rick_C137.so player_rick_C137.o graph_modif.o utils.o strategy.o;

#player_rick_evil: graph_modif.o utils.o strategy.o
#	${CC} -fPIC -c ${CFLAGS} ${DIR}/player_rick_evil.c;
#	${CC} -shared -nostartfiles -o install/Evil_Rick.so player_rick_evil.o graph_modif.o utils.o strategy.o;

###################### Tests #######################

clean:
	rm -f *.o ${BIN_DIR}/* *~ *.gcda *.gcno
