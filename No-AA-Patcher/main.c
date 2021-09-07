#define _CRT_SECURE_NO_WARNINGS // I don't care if you consider normal fopen unsafe, Visual Studio.

#include <stdio.h>
#include <string.h>
#include <malloc.h>

const char ptr_old[] = { 0x08, 0x08, 0x0a, 0x0c, 0x0a, 0x08, 0x08, 0x00 }; // NTSC / PAL progressive "soft" / "aa" rmode
const char ptr_new[] = { 0x00, 0x00, 0x15, 0x16, 0x15, 0x00, 0x00, 0x00 }; // NTSC / PAL progressive rmode

const char ptr_dithering[] = {0x3C, 0x80, 0xCC, 0x01, 0x38, 0xA0, 0x00, 0x61,
							  0x38, 0x00, 0x00, 0x00, 0x80, 0xC7, 0x02, 0x20,
							  0x50, 0x66, 0x17, 0x7A, 0x98, 0xA4, 0x80, 0x00, // The four bytes before this pattern determine if the game should apply dithering or not.
							  0x90, 0xC4, 0x80, 0x00, 0x90, 0xC7, 0x02, 0x20,
							  0xB0, 0x07, 0x00, 0x02, 0x4E, 0x80, 0x00, 0x20};
const char ptr_nodither[] = {0x48, 0x00, 0x00, 0x28};

char patchAA, patchDither;

// Parses argv.
int parseArgs(char* argv[])
{
	int parsed = 0;
	int current = 1;

	while(argv[current] != NULL)
	{
		if (strncmp(argv[current], "--", 2) != 0)
		{
			break;
		}

		if (strcmp(argv[current], "--noaa") == 0)
		{
			patchAA = 1;
			parsed++;
		}

		if (strcmp(argv[current], "--nodither") == 0)
		{
			patchDither = 1;
			parsed++;
		}

		current++;
	}

	return parsed;
}

int main(int argc, char* argv[])
{
	FILE *src, *dst;
	char *buf, *pos;
	size_t srcSize;

	// Make sure we've got a file to work with
	if (argc < 3)
	{
		printf("Not enough arguments provided!\nUsage: ./No-AA-Patcher <patch(es)> <source dol> <output dol>\n");
		printf("Patches:\n");
		printf("--noaa : Removes the anti-aliasing / flicker filter from the DOL.\n");
		printf("--nodither : Disables dithering in the DOL.");
		return 1;
	}

	if (!parseArgs(argv))
	{
		printf("No patch specfied! Run this program without any arguments to view a list of patches you can apply!\n");
		return 1;
	}

	// Are we able to open it?
	if (!(src = fopen(argv[argc-2], "rb")))
	{
		printf("Unable to open DOL file '%s'!\n", argv[argc-2]);
		return 1;
	}

	// Try to open the destination file, this can be patched.dol or a filename specified by the user.
	if (!(dst = fopen(argv[argc-1], "wb")))
	{
		fclose(src);
		printf("Unable to open output file '%s' for writing!\n", argv[argc-2]);
		return 1;
	}

	// Get the size of the source dol and try to allocate enough memory to load it entirely into memory.
	fseek(src, 0, SEEK_END);
	srcSize = ftell(src);
	fseek(src, 0, SEEK_SET);
	buf = (char*)malloc(srcSize);

	// Confirm that we've actually got a pointer to the memory we requested.
	if (!buf)
	{
		fclose(src);
		fclose(dst);
		printf("Unable to allocate %dK memory for processing!\n", (int)srcSize / 1024);
		return 1;
	}

	// Read the source file into our buffer
	fread(buf, srcSize, 1, src);

	// Search for the "soft" / "aa" rmode pattern and replace it, if we find it.
	for (int i = 0; i < srcSize - sizeof(ptr_dithering); i++)
	{
		pos = (char*)buf + i;

		// If wanted by the user replace the "soft" / "aa" rmode with the normal progressive one.
		if (patchAA)
		{
			if (memcmp(pos, ptr_old, 8) == 0)
			{
				memcpy(pos, ptr_new, 8);
			}
		}

	// If wanted by the user remove the dithering filter from the game.
		if (patchDither)
		{
			if (memcmp(pos, ptr_dithering, 40) == 0)
			{
				memcpy(pos - 4, ptr_nodither, 4);
			}
		}
	}

	// Write the modified dol which we currently have in memory to the disk
	fwrite(buf, srcSize, 1, dst);
	fflush(dst);

	// Clean up and release all resources we have used.
	fclose(src);
	fclose(dst);
	free(buf);

	printf("And we're done, have a great day!\n");
	return 0;
}
