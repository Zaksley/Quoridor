# Quoridor Project
==================

- [Project subject](https://www.labri.fr/perso/renault/working/teaching/projets/2020-21-S6-C-Quoridor.php)

- [Thor web page](https://thor.enseirb-matmeca.fr/ruby/projects/projetss6-quor)

Quoridor is a board game where two players confront by moving a pawn towards a goal or placing walls as obstacles for the other player. The first player to get to the goal wins.

This project implements a server running quoridor games, and some automated players using simple strategies.

# Authors
=========

_ENSEIRB-MATMECA Group n°128-30_

- M. Brassart 	<mathieu.brassart@enseirb-matmeca.fr>
- K. Dieu 		<killian.dieu@enseirb-matmeca.fr>
- L. Herau 		<perig.herau@enseirb-matmeca.fr>
- E. Medina 	<enzo.medina@enseirb-matmeca.fr>

# Installation & Usage
======================

## Local usage
--------------

1. Installation
	- Make sure to have the *GSL* library installed on the computer
	- Store its root path inside the global variable *GSL_PATH*
	Example : `export GSL_PATH=/usr/local`

	- Store the library path in another global variable *LD_LIBRARY_PATH*
	Example : `export LD_LIBRARY_PATH=/usr/local/lib`

	- Write `make build` in a terminal to build the project
	- Then use `make install` to copy executables in the *install/* folder


2. Usage
	- Use `./install/server -m [M] -t [T] [player1] <player2>` to start a game

	Options description :
		`-m`	int 		board size
		`-t`	char		board shape ('c':square, 't':torus, 'h':chopped, 's':snake)

	Input players must paths to dynamic libraries (*.so* files)
	First player is optionnal (if not given, Usain Bolt (default player 1) will play against Rick (default player 2))
	Second player is optionnal (if not given, player1 will play against Rick (default player 2))

	Example : `./install/server -m 10 -t 'c' ./install/libplayer1.so ./install/libplayer2.so`


3. Tests
	- Use `make test` to build the tests without running them
	- Use `make alltests` to build the tests and run them

	- To run the tests manually, execute `./install/alltests <-v>`
	- The `-v` option will enter verbose mode, and give extra test information


## External players usage
-------------------------

If you want to use external players, simply build them using another Quoridor player builder.
Then, copy them in the `install/` directory, and give them as arguments for the server.

In the same way, you can use the player shared libraries built by this project in any other Quoridor central server using the same data exchange format.