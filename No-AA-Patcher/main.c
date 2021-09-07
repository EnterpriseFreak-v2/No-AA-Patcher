#define _CRT_SECURE_NO_WARNINGS // I don't care if you consider normal fopen unsafe, Visual Studio.
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "deflicker.h"
#include "dithering.h"

char deflicker, dithering;

int parseArgv(char* argv[])
{
	int current = 1;
	int parsed = 0;

	while (argv[current] != NULL)
	{
		if (strncmp(argv[current], "--", 2) != 0)
		{
			break;
		}

		if (strcmp(argv[current], "--noaa") == 0)
		{
			deflicker = 1;
			parsed++;
		}

		if (strcmp(argv[current], "--nodither") == 0)
		{
			dithering = 1;
			parsed++;
		}

		current++;
	}

	return parsed;
}

int main(int argc, char* argv[])
{
	FILE *input, *output;
	char* dolbuf;
	size_t dolsize;
	
	if (argc < 3)
	{
		printf("Not enough arguments provided!\n");
		printf("Usage: No-AA-Patcher.exe --<patch> <main.dol> <patched.dol>\n");
		printf("Patches:\n");
		printf("--noaa: Removes the deflicker filter.\n");
		printf("--nodither: Removes the dithering filter.\n");
		return 1;
	}

	if (!parseArgv(argv))
	{
		printf("No valid patch specified, please specify at least one patch!\n");
		return 1;
	}

	if (!(input = fopen(argv[argc - 2], "rb")))
	{
		printf("Unable to access file '%s' for reading!\n", argv[argc - 2]);
		return 1;
	}

	fseek(input, 0, SEEK_END);
	dolsize = ftell(input);
	fseek(input, 0, SEEK_SET);

	if (!(output = fopen(argv[argc - 1], "wb")))
	{
		fclose(input);
		printf("Unable to access file '%s' for writing!\n", argv[argc - 1]);
		return 1;
	}

	dolbuf = (char*)malloc(dolsize);

	if (!dolbuf)
	{
		fclose(input);
		fclose(output);
		printf("Unable to allocate enough memory for the patching process!\n");
		return 1;
	}

	fread(dolbuf, dolsize, 1, input);

	if (deflicker)
	{
		applyDeflickerPatch(dolbuf, dolsize);
	}

	if (dithering)
	{
		applyDitheringPatch(dolbuf, dolsize);
	}

	fwrite(dolbuf, dolsize, 1, output);
	printf("And we're done, have a great day!\n");
	fclose(input);
	fclose(output);
	free(dolbuf);
	return 0;
}
