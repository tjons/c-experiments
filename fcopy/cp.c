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


void help() {
	printf("usage: _cp <options> [SOURCE] [TARGET]\n");
	printf("	options: [-R -v]\n");
	printf("	-R: recursive\n");
	printf("	-v: verbose\n");
	exit(1);
}

int copyfile(char *from, char *to)
{
		FILE *source = fopen(from, "r");
		if (source == NULL) {
			perror("source file not found");
			return 2;
		}
		FILE *target = fopen(to, "w");
		if (target == NULL) {
			perror("error creating target file");
			return 1;
		}

		int c;

		while ((c = fgetc(source)) != EOF) {
			fprintf(target, "%c", c);	
		}

		fclose(source);
		fclose(target);

        return 0;
}

bool abspath(char *path)
{
    return path[0] == '/';
}

int main(int argc, char *argv[])
{
	if (argc < 3) {
		// we expect exactly two arguments. anything else should
		// send the user to help.
		help();
	}
    char *from = "";
    char *to = "";
    bool isRecursive, isVerbose;
    
    int i;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-R") == 0)
                isRecursive = true;
        else if (strcmp(argv[i], "-v") == 0)
                isVerbose = true;
        else if (strlen(from) == 0)
                from = argv[i];
        else if (strlen(to) == 0)
                to = argv[i];
        else
                help();
    }

	struct stat sb;
	int errn = lstat(from, &sb);
	if (errn != 0) {
		exit(1);
	}

    if (S_ISDIR(sb.st_mode)) {
        struct stat target_dir;
        if (stat(to, &target_dir) == -1) {
            mkdir(to, sb.st_mode);
        }

		DIR *s_dir = opendir(from);
		if (s_dir != NULL) {
			// copy directory, file-by-file, from source to target
            struct dirent *f;
            struct stat s;
            while (f = readdir(s_dir)) {
                printf("%s\n", f->d_name);
                if (strcmp(f->d_name, ".") == 0 || strcmp(f->d_name, "..") == 0) continue;

                char fn[strlen(from)+strlen(f->d_name)];
                strcpy(fn, from);
                strcat(fn, f->d_name);
                printf("%s\n", fn);
                if (lstat(fn, &s) == -1) {
                        perror("could not stat %s");
                        continue;
                }

                if (S_ISDIR(s.st_mode)) {
                    // construct path, recursively handle directory
                    continue;
                }

                if (S_ISREG(s.st_mode)) {
                    char from_path[strlen(from)+strlen(f->d_name)];
                    strcpy(from_path, from);
                    strcat(from_path, f->d_name);

                    char to_path[strlen(to)+1+strlen(f->d_name)];
                    strcpy(to_path, to);
                    strcat(to_path, "/");
                    strcat(to_path, f->d_name);

                    if (copyfile(from_path, to_path) != 0) exit(1);
                }
            }
		}
	} else if (copyfile(from, to) != 0) exit(1);
		
	return 0;
}

