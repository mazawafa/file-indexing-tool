#include <dirent.h> // readdir()
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // getcwd()

int main(int argc, char* argv[]) {
    char* dname = NULL;

    if ((dname = getcwd(dname, 0)) == NULL) {
        fprintf(stderr, "error getting current working directory\n");
        return EXIT_FAILURE;
    }

    DIR* dp;
    if ((dp = opendir(dname)) == NULL) {
        fprintf(stderr, "error opening %s\n", dname);
        free(dname);
        return EXIT_FAILURE;
    }

    struct dirent* ent;
    errno = 0;
    while ((ent = readdir(dp)) != NULL) {
        printf("%s\n", ent->d_name);
    }
    if (ent == NULL && errno) {
        perror("readdir");
        free(dname);
        return EXIT_FAILURE;
    }

    free(dname);
    return EXIT_SUCCESS;
}
