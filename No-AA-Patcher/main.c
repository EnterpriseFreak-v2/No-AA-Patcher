#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>

char pattern[] = {0x08, 0x08, 0x0a, 0x0c, 0x0a, 0x08, 0x08, 0x00};
char pattern_new[] = {0x00, 0x00, 0x15, 0x16, 0x15, 0x00, 0x00, 0x00};

int main(int argc, char* argv[])
{
	uint64_t numItems = 0;
	uint64_t*	items;

	size_t maindolsize = 0;
	uint8_t* maindolbuff;
	FILE* dol;

	dol = fopen(argv[1], "rb");
	
	if (!dol)
	{
		printf("Unable to open '%s', aborting!\n", argv[1]);
		return 1;
	}

	fseek(dol, 0, SEEK_END);
	maindolsize = ftell(dol);
	rewind(dol);

	maindolbuff = malloc(maindolsize);

	if (!maindolbuff)
	{
		printf("Unable to allocate memory buffer, aborting! (Is your system low on memory?)\n");
		fclose(dol);
		return 1;
	}

	fread(maindolbuff, maindolsize, 1, dol);
	fclose(dol);

	for (int i = 0; i < maindolsize - 8; i++)
	{
		char* loc = (char*)maindolbuff + i;

		if (memcmp(loc, pattern, 8) == 0)
		{
			memcpy(maindolbuff + i, pattern_new, 8);
			printf("%lx\n", maindolbuff + i);
		}
		
	}

	FILE* out = fopen("patched.dol", "wb");
	fwrite(maindolbuff, maindolsize, 1, out);
	fclose(out);


	return 0;
}