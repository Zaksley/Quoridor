#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>


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
    void * handle;
    handle = dlopen("./libplayer.so", RTLD_LAZY);
    
    //void (*print_name) (const char *);  
    char* (*get_name)(); 

    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

     

    dlerror(); // Clear any existing error 

    
    //*(void **) (&print_name) = dlsym(handle, "get_print_name");
    *(void **) (&get_name) = dlsym(handle, "get_player_name");

    //print_name("Jean"); 
    printf("%s\n", get_name());

    dlclose(handle);
    
}

int main()
{
    get_functions();
    printf("It was a test\n");
    return 1; 
}