#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>


/*
*   Contains every special function of a player
*/
struct player_func
{
    char* (*get_name)();
    struct move_t (*player_play) (struct move_t);
}; 

/*
* Get functions from a special player with a dynamic lib.so 
* 
* @ Param : Need 2 names of the players (libs)
* @ Precond - 2 players (librairies)
* @ Return - Nothing
*/

// TODO => Take 2 arguments (lib1 & lib2) to get all functions
void get_functions()
{

    struct player_func player1; 

    void * handle;
    handle = dlopen("./libplayer.so", RTLD_LAZY);

    char* (*get_name)(); 
    struct move_t (*player_play) (struct move_t); 

    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    dlerror(); // Clear any existing error 

    *(void **) (&get_name) = dlsym(handle, "get_player_name");
    *(void **) (&player_play) = dlsym(handle, "play"); 

    printf("%s\n", get_name());

    player1.get_name = get_name; 
    player1.player_play = player_play;

    dlclose(handle);
    
}

int main()
{
    get_functions();
    printf("It was a test\n");
    return 1; 
}