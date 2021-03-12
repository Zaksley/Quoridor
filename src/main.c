#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>


/*
*   Contains every special function of a player
*/
struct player
{
    char* (*get_name)();
    struct move_t (*player_play) (struct move_t);
}; 

/*
* Get functions from a special player with a dynamic lib.so and stock it in a new struct
* 
* @ Param : Need 1 name of the player (lib)
* @ Precond - 1 player (librairies)
* @ Return - A player with initialized functions
*/

// TODO => Take 1 arguments (lib1) to get all functions
struct player get_functions()
{
        // Initialization 
    struct player player; 
    char* (*get_name)(); 
    struct move_t (*player_play) (struct move_t); 

        // Open the lib 
    void * handle;
    handle = dlopen("./libplayer.so", RTLD_LAZY);

    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    dlerror(); // Clear any existing error 


    *(void **) (&get_name) = dlsym(handle, "get_player_name");
    *(void **) (&player_play) = dlsym(handle, "play"); 
    player.get_name = get_name; 
    player.player_play = player_play;

    // dlclose(handle); - To close at the end ?

    return player; 
    
}

int main()
{
    struct player player1 = get_functions();
    struct player player2 = get_functions(); 

    printf("Nom du joueur 1 : %s\n", player1.get_name());
    printf("Nom du joueur 2 : %s\n", player2.get_name());

    return 1; 
}