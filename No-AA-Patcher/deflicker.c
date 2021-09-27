#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* Credits for these values: NoobletCheese @ GBATemp 
* Source: https://gbatemp.net/threads/possible-to-disable-the-wiis-de-flicker-filter.477163/page-2#post-9432766 */
const char ptr_old_progSf[] = { 0x08, 0x08, 0x0a, 0x0c, 0x0a, 0x08, 0x08 }; // Ntsc480ProgSoft
const char ptr_old_progAa[] = { 0x05, 0x08, 0x0c, 0x10, 0x0c, 0x08, 0x04 }; // Ntsc480ProgAa
const char ptr_old_mkwii1[] = { 0x07, 0x07, 0x0c, 0x0c, 0x0c, 0x07, 0x07 }; // Mario Kart Wii
const char ptr_old_mkwii2[] = { 0x05, 0x05, 0x0f, 0x0e, 0x0f, 0x05, 0x05 }; // Mario Kart Wii
const char ptr_old_galaxy[] = { 0x20, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00 }; // Super Mario Galaxy 1 / 2
const char ptr_new_nofilt[] = { 0x00, 0x00, 0x15, 0x16, 0x15, 0x00, 0x00 }; // Desired filter pattern (no filter)
const char ptr_preceding[] = { 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06 }; // Pattern that preceeds the filter pattern

void applyDeflickerPatch(char* buffer, size_t bufsize)
{
	char* position = 0;

	// I, EvaX, ...
	for (int i = 0; i < bufsize - sizeof(ptr_new_nofilt); i++)
	{
		position = (char*)buffer + i;

		if (memcmp(position, ptr_old_progSf, 7) == 0 && memcmp(position - 7, ptr_preceding, 7) == 0)
		{
			memcpy(position, ptr_new_nofilt, 7);
		}
		else if (memcmp(position, ptr_old_progAa, 7) == 0 && memcmp(position - 7, ptr_preceding, 7) == 0)
		{
			memcpy(position, ptr_new_nofilt, 7);
		}
		else if (memcmp(position, ptr_old_mkwii1, 7) == 0 && memcmp(position - 7, ptr_preceding, 7) == 0)
		{
			memcpy(position, ptr_new_nofilt, 7);
		}
		else if (memcmp(position, ptr_old_mkwii2, 7) == 0 && memcmp(position - 7, ptr_preceding, 7) == 0)
		{
			memcpy(position, ptr_new_nofilt, 7);
		}
		else if (memcmp(position, ptr_old_galaxy, 7) == 0 && memcmp(position - 7, ptr_preceding, 7) == 0)
		{
			memcpy(position, ptr_new_nofilt, 7);
		}
	}
}
