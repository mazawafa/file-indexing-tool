#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // getcwd()

int main(int argc, char* argv[]) { 
    char* dname = NULL;
    
    if ((dname = getcwd(dname, 0)) == NULL) {
        fprintf(stderr, "error getting current working directory\n");
        return EXIT_FAILURE;
    }

    printf("%s\n", dname);
    free(dname); 
    return EXIT_SUCCESS;
}
