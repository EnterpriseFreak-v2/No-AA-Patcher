# No-AA-Patcher
Removes the anti-aliasing / softening filter from your favourite Wii games! (Oh yeah, the dithering too, if that's your thing)
Have you ever noticed how some Wii games just look horribly blury even playing using component cables or an HDMI mod? You can thank the "anti-aliasing" / softener filter of the Wii for that. The purpose of this filter is to prevent "hard" jumps when playing in an interlaced (480i, 576i) video mode by mashing together mutliple lines before outputting them. For some reason however, this filter is still active even playing while outputting a progressive 480p signal on a lot of games. No-AA-Patcher removes the calls to the "aa" / "soft" progressive rmode and replaces them with the "normal" progressive rmode resulting in a much cleaner image (look at the preview.png file, if you want to see the difference). 

## Ways to patch you game
There are two ways to patch your game. Method #1 is to patch the entire ISO/WBFS, method #2 is to just patch the main.dol and then load it using the alternate dol feature of for example USB Loader GX. The choice of what method to use is up to you. Both methods work great.

## Patching your game (Method 1)
Before you start, please make sure that you already have WIT (Wiimm's ISO Tools) installed, if not get it on [Wiimm's website](https://wit.wiimm.de/download.html).

*When using this method you'll have to replace the entire WBFS/ISO on your storage media with the patched one, if you want to patch the main.dol on-the-fly please skip this section and read __Patching your game (Method 2)__*
1) Unpack your game using WIT (``wit extract <your wbfs/iso> <folder>``)
2) Run No-AA-Patcher on the main.dol of the game to get a patched.dol file (``No-AA-Patcher.exe --noaa <folder\DATA\sys\main.dol> patched.dol``)
3) Replace the main.dol of the game with the patched.dol you've just got. (You got this one :D)
4) Repack the game using wit (``wit copy <folder> <patched.wbfs>``)
5) Replace your original ISO/WBFS with the one you've packed using wit.
6) Enjoy your game in much higher quality!


## Patching your game (Method 2)
Before you start, please make sure that you already have WIT (Wiimm's ISO Tools) installed, if not get it on [Wiimm's website](https://wit.wiimm.de/download.html).

1) Unpack your game using WIT (``wit extract <your wbfs/iso> <folder>``)
2) Run No-AA-Patcher on the main.dol of the game to get a patched.dol file (``No-AA-Patcher.exe -noaa <folder\DATA\sys\main.dol> patched.dol``)
3) Copy the patched.dol to the root of your SD card.
4) Rename the patched.dol on the SD Card to the ID6 of your game*
5) Boot USB Loader GX, click on the game, settings, Game Load and set Alternate DOL to "Load From SD/USB", then click on save.
6) Run the game, if you did everything right the game will now load with your patched DOL!
7) Enjoy your game in much higher quality!

* : Usually, the ID6 of the game is equal to the filename of the WBFS. 

## Removing dithering from the main.dol
Simply add "--nodither" after "--noaa" and before the path of the main.dol you'd like to patch. If you'd like to keep the deflicker filter and only remove dithering call the program without "--noaa". 
