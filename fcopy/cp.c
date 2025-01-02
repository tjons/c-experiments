#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <strings.h>
#define bool int
#define false 0
#define true 1

void help() {
	printf("usage: tycp <options> [SOURCE] [TARGET]\n");
	printf("	options: [-R -v]\n");
	printf("	-R: recursive\n");
	printf("	-v: verbose\n");
	exit(1);
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
	int errn = lstat(argv[1], &sb);
	if (errn != 0) {
		exit(1);
	}

    if (S_ISDIR(sb.st_mode)) {
		DIR *s_dir = opendir(argv[1]);
		if (s_dir != NULL) {
			// copy directory from source to target
			
		}
	} else {
		FILE *source = fopen(argv[1], "r");
		if (source == NULL) {
			printf("%s not found.", argv[1]);
			exit(1);
		}
		FILE *target = fopen(argv[2], "w");
		if (target == NULL) {
			printf("error creating %s", argv[2]);
			exit(1);
		}

		int c;

		while ((c = fgetc(source)) != EOF) {
			fprintf(target, "%c", c);	
		}

		fclose(source);
		fclose(target);
	}
		
	return 0;
}

