#include <dirent.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define HT_SIZE 1000

/* Very simple hash function */
unsigned int simple_hash(const char* s) {
    unsigned int sum, i;
    sum = i = 0;
    while (s[i] != '\0') {
        sum = sum + s[i];
        ++i;
    }
    return sum;
}

/* Set hash table array from directory content */
void set_ht_from_dir(DIR* dp, const char* ht[], size_t sz,
                     unsigned int (*hash_func)(const char*)) {
    struct dirent* e;

    errno = 0;
    while ((e = readdir(dp)) != NULL) {
        ht[(*hash_func)(e->d_name) % sz] = e->d_name;
    }

    /* Catch error */
    if (e == NULL && errno) {
        perror("readdir");
        exit(EXIT_FAILURE);
    }
}

void save_ht_to_file(const char* ht[], size_t sz, FILE* fp) {
    size_t i;
    uint8_t len;
    for (i = 0; i < sz; ++i) {
        if (ht[i] != NULL) {
            fwrite(&i, sizeof i, 1, fp);

            /* Add check for overflow */
            len = (uint8_t)strlen(ht[i]);
            fwrite(&len, sizeof len, 1, fp);
            fwrite(ht[i], 1, len, fp);
        }
    }
}

void print_hash_table(const char* ht[], size_t sz) {
    int wid = 10;

    size_t i;
    for (i = 0; i < sz; ++i) {
        if (ht[i] != NULL) {
            printf("%-*zu %s\n", wid, i, ht[i]);
        }
    }
}

void print_ent_in_hash(unsigned int (*hash_func)(const char*), const char* ht[],
                       size_t sz, const char* fname) {
    if (ht[(*hash_func)(fname) % sz] != NULL) {
        printf("%s\n", fname);
    } else {
        printf("no entry\n");
    }
}

int main(int argc, char* argv[]) {
    const char* const ht_fname = argv[1];    // File that contain hash table
    const char* ht[HT_SIZE];                 // Hash table array
    memset(ht, 0, (sizeof ht[0]) * HT_SIZE); // Set array elements to NULL

    /* Check command line arguments */
    if (argc < 3) {
        fprintf(stderr, "usage: %s HASHTABLE FILENAME...\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Get current working directory */
    char* dname = NULL;
    if ((dname = getcwd(dname, 0)) == NULL) {
        perror("getcwd");
        return EXIT_FAILURE;
    }

    /* Open directory */
    DIR* dp = opendir(dname);
    free(dname);
    if (dp == NULL) {
        perror("opendir");
        return EXIT_FAILURE;
    }

    /* Open file that contain hash table */
    FILE* ht_fp;
    if ((ht_fp = fopen(ht_fname, "rb+")) == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    // load_ht_from_file(ht_fp, ht, HT_SIZE);
    set_ht_from_dir(dp, ht, HT_SIZE, &simple_hash);
    closedir(dp);
    print_hash_table(ht, HT_SIZE);
    save_ht_to_file(ht, HT_SIZE, ht_fp);

    /* Print table entry if any */
    /* for (int i = 1; i < argc; ++i) {
        print_ent_in_hash(&simple_hash, hash_table, BUF_SIZE, argv[i]);
    } */

    fclose(ht_fp);
    return EXIT_SUCCESS;
}
