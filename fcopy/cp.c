#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <strings.h>
#include <errno.h>
#define bool int
#define false 0
#define true 1

bool verbose;

void help()
{
    printf("usage: _cp <options> [SOURCE] [TARGET]\n");
    printf("	options: [-R -v]\n");
    printf("	-R: recursive\n");
    printf("	-v: verbose\n");
    exit(1);
}

void write_verbose(char *message)
{
    if (verbose)
        printf("%s\n", message);

    return;
}

// copyfile will copy the file located at the `from` path to the
// `to` path, creating the file if it does not exist. Currently,
// this method does not copy the mode of the original file.
// A non-zero exit code indicates failure.
//    `2` indicates that the source file does not exist or could not be opened
//    `1` indicates that the target file could not be opened
// If the function is successful, it will return `0`.
// TODO: copy mode of original file
int copyfile(char *from, char *to)
{
    // open source for reading. we do not need write permissions to
    // copy from source.
    // TODO: what happens if we don't have permissions to read?
    FILE *source = fopen(from, "r");
    if (source == NULL)
    {
        perror("source file not found");
        return 2;
    }

    // open target for writing.
    // TODO: what happens if we don't have permissions to write?
    FILE *target = fopen(to, "w");
    if (target == NULL)
    {
        perror("error creating target file");
        return 1;
    }

    int c;
    // copy each character from source to target.
    while ((c = fgetc(source)) != EOF)
    {
        fprintf(target, "%c", c);
    }

    // TODO: can we error or fail here?
    fclose(source);
    fclose(target);

    return 0;
}

int copydir(char *from_path, struct stat *from_stat, char *to_path)
{
    struct stat target_dir;
    if (stat(to_path, &target_dir) == -1)
    {
        mkdir(to_path, from_stat->st_mode);
    }

    DIR *s_dir = opendir(from_path);
    if (s_dir != NULL)
    {
        // copy directory, file-by-file, from source to target
        struct dirent *f;
        struct stat s;
        while ((f = readdir(s_dir)))
        {
            if (strcmp(f->d_name, ".") == 0 || strcmp(f->d_name, "..") == 0)
                continue;

            char fn[strlen(from_path) + 1 + strlen(f->d_name) + 1];
            strcpy(fn, from_path);
            strcat(fn, "/");
            strcat(fn, f->d_name);
            printf("%s\n", fn);
            if (lstat(fn, &s) == -1)
            {
                perror("could not stat %s");
                continue;
            }

            if (S_ISDIR(s.st_mode))
            {
                char source_path[strlen(from_path) + strlen(f->d_name) + 1];
                strcpy(source_path, from_path);
                strcat(source_path, "/");
                strcat(source_path, f->d_name);

                char target_path[strlen(to_path) + 1 + strlen(f->d_name) + 1];
                strcpy(target_path, to_path);
                strcat(target_path, "/");
                strcat(target_path, f->d_name);

                if (copydir(source_path, &s, target_path) != 0) return 1;
            }

            if (S_ISREG(s.st_mode))
            {
                char source_path[strlen(from_path) + strlen(f->d_name) + 1];
                strcpy(source_path, from_path);
                strcat(source_path, "/");
                strcat(source_path, f->d_name);

                char target_path[strlen(to_path) + 1 + strlen(f->d_name) + 1];
                strcpy(target_path, to_path);
                strcat(target_path, "/");
                strcat(target_path, f->d_name);

                if (copyfile(source_path, target_path) != 0)
                    return 1;
            }
        }
    }

    return 0;
}

// abspath is a small helper to determine if a path is absolute,
// determined by checking if the first character in the string is
// a forward-slash.
bool abspath(char *path)
{
    return path[0] == '/';
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        // we expect exactly two arguments. anything else should
        // send the user to help.
        help();
    }
    char *from_path = "";
    char *to_path = "";
    bool recursive;

    // parse options and parameters. from_path and to_path are required,
    // -R (recursive) and -v (verbose) are optional. They may appear in
    // any order.
    int i;
    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-R") == 0)
            recursive = true;
        else if (strcmp(argv[i], "-v") == 0)
            verbose = true;
        else if (strlen(from_path) == 0)
            from_path = argv[i];
        else if (strlen(to_path) == 0)
            to_path = argv[i];
        else
            help();
    }

    if (from_path[strlen(from_path)-1] == '/') {
        from_path[strlen(from_path)-1] = '\0';
    }

    if (to_path[strlen(to_path)-1] == '/') {
        to_path[strlen(to_path)-1] = '\0';
    }

    struct stat from_stat;
    if (lstat(from_path, &from_stat) != 0)
    {
        exit(1);
    }

    if (S_ISDIR(from_stat.st_mode))
    {
        if (copydir(from_path, &from_stat, to_path) != 0)
            exit(1);
    }
    else if (copyfile(from_path, to_path) != 0)
        exit(1);

    return 0;
}
