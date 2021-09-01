#define _CRT_SECURE_NO_WARNINGS // I don't care if you consider normal fopen unsafe, Visual Studio.

#include <stdio.h>
#include <string.h>
#include <malloc.h>

const char ptr_old[] = { 0x08, 0x08, 0x0a, 0x0c, 0x0a, 0x08, 0x08, 0x00 }; // NTSC / PAL progressive "soft" / "aa" rmode
const char ptr_new[] = { 0x00, 0x00, 0x15, 0x16, 0x15, 0x00, 0x00, 0x00 }; // NTSC / PAL progressive rmode

int main(int argc, char* argv[])
{
	FILE *src, *dst;
	char *buf, *pos;
	size_t srcSize;

	// Make sure we've got a file to work with
	if (argc < 2)
	{
		printf("No source file specified.\nUsage: ./No-AA-Patcher <source dol> <output dol>\n");
		return 1;
	}

	// Are we able to open it?
	if (!(src = fopen(argv[1], "rb")))
	{
		printf("Unable to open DOL file '%s'!\n", argv[1]);
		return 1;
	}

	// Try to open the destination file, this can be patched.dol or a filename specified by the user.
	if (!(dst = fopen(argv[2] != NULL ? argv[2] : "patched.dol", "wb")))
	{
		fclose(src);
		printf("Unable to open output file '%s' for writing!\n", argv[2] != NULL ? argv[2] : "patched.dol");
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
	for (int i = 0; i < srcSize - sizeof(ptr_old); i++)
	{
		pos = (char*)buf + i;

		if (memcmp(pos, ptr_old, 8) == 0)
		{
			memcpy(pos, ptr_new, 8);
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
