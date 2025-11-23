#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#define BUF_SIZE 1000

/* Very simple hash function */
uint32_t simple_hash(const char* s) {
    uint32_t sum, i;
    sum = i = 0;
    while (s[i] != '\0') {
        sum = sum + s[i];
        ++i;
    }
    return sum;
}

DIR *opencwd() {
    char *dname = NULL;
    if ((dname = getcwd(dname, 0)) == NULL) {
        perror("getcwd");
        exit(EXIT_FAILURE);
    }

    DIR *dp;
    if ((dp = opendir(dname)) == NULL) {
        perror("opendir");
        free(dname);
        exit(EXIT_FAILURE);
    }
    free(dname);

    return dp;
}

void set_dir_hash_table(unsigned int (*hash_func)(const char *),
                        DIR* dp, const char *ht[], size_t sz) {
    struct dirent* e;
    const char* e_name;

    errno = 0;
    while ((e = readdir(dp)) != NULL) {
        e_name = e->d_name;
        ht[(*hash_func)(e_name) % sz] = e_name;
    }
    closedir(dp);
    
    /* Catch error */
    if (e == NULL && errno) {
        perror("readdir");
        exit(EXIT_FAILURE);
    }
}

void print_hash_table(const char *ht[], size_t sz) {
    int wid = 10;

    size_t i;
    for (i = 0; i < sz; ++i) {
        if (ht[i] != NULL) {
            printf("%-*zu %s\n", wid, i, ht[i]);
        }
    }
}

void print_ent_in_hash(unsigned int (*hash_func)(const char *),
                       const char *ht[], size_t sz, const char *fname) {
    if (ht[(*hash_func)(fname) % sz] != NULL) {
        printf("%s\n", fname);
    } else {
        printf("no entry\n");
    }
}

int main(int argc, char* argv[]) {
    const char* hash_fname = ".hash_table";
    const char* hash_table[BUF_SIZE];
    memset(hash_table, 0, (sizeof hash_table[0]) * BUF_SIZE);

    /* Check command line arguments */
    if (argc < 2) {
        fprintf(stderr, "usage: %s FILENAME ...\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Open current working directory */
    DIR *dp = opencwd();
    
    /* Open hash table file */
    FILE* hash_fp;
    if ((hash_fp = fopen(hash_fname, "rb+")) == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    set_dir_hash_table(&simple_hash, dp, hash_table, BUF_SIZE);
    // print_hash_table(hash_table, BUF_SIZE);

    /* Print table entry if any */
    for (int i = 1; i < argc; ++i) {
        print_ent_in_hash(&simple_hash, hash_table, BUF_SIZE, argv[i]);
    }

    return EXIT_SUCCESS;
}
