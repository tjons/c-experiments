#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *f = fopen("file.txt", "w");
	if (f == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}

	const char *text = "Write this to the file";
	fprintf(f, "Some text: %s\n", text);

	fclose(f);
}
