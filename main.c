#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1000

/* Very simple hash function */
size_t simple_hash(const char* s) {
    size_t sum, i;
    sum = i = 0;
    while (s[i] != '\0') {
        sum = sum + s[i];
        ++i;
    }
    return sum;
}

int main(int argc, char* argv[]) {
    const char* hash_table[BUF_SIZE];
    memset(hash_table, 0, (sizeof hash_table[0]) * BUF_SIZE);

    /* Check command line arguments */
    if (argc < 2) {
        fprintf(stderr, "usage: %s FILENAME ...\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Get current working directory name */
    char* dname = NULL;
    if ((dname = getcwd(dname, 0)) == NULL) {
        fprintf(stderr, "error getting current working directory\n");
        return EXIT_FAILURE;
    }

    /* Open current working directory */
    DIR* dp;
    if ((dp = opendir(dname)) == NULL) {
        fprintf(stderr, "error opening %s\n", dname);
        free(dname);
        return EXIT_FAILURE;
    }

    /* Read current working directory and hash filenames */
    struct dirent* e;
    const char* e_name;
    errno = 0; // set before readdir
    while ((e = readdir(dp)) != NULL) {
        e_name = e->d_name;
        hash_table[simple_hash(e_name) % BUF_SIZE] = e_name;
    }
    if (e == NULL && errno) {
        perror("readdir");
        free(dname);
        return EXIT_FAILURE;
    }

    /* Print table entry if any */
    int fname;
    for (int i = 1; i < argc; ++i) {
        fname = simple_hash(argv[i]) % BUF_SIZE;
        if (hash_table[fname] != NULL) {
            printf("%s\n", hash_table[fname]);
        } else {
            printf("no entry\n");
        }
    } 

    free(dname);
    return EXIT_SUCCESS;
}
