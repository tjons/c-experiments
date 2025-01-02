#include <stdlib.h>
#include <stdio.h>
#define LEN 500

int main()
{
	FILE *f;
	f = fopen("file.txt", "r");
	if (f == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}

	char contents[LEN];

	int n;
	while ((contents[n++] = fgetc(f)) != EOF)
		;

	printf("file says %s", contents);
}
