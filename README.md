# Unplugged Deluxe

> This project is not affiliated with or endorsed by Harmonix, MTV Games, EA, or any other official entity associated with Rock Band. It is a fan-made utility for educational and personal use only.

This is a plugin for the Playstation Portable that allows for custom content to be utilized within the game Rock Band Unplugged.

## Plugin Installation

This plugin can be installed on both official hardware and emulators. However, installation on a PSP will require some extra steps and Custom Firmware (CFW) with plugin capabilities.

### PSP (Retail Hardware)

Firstly, make sure you are using an up-to-date revision of CFW. Next, you will need to install the plugin np loader mod. Next, download the PSP version of this plugin from the releases section. Inside of the archive, you will find two files: `UDBootsrap.prx` and `UnpluggedDeluxe.prx`. You will **ONLY** want to install the `UDBootstrap.prx` plugin on your system. Make sure to install the bootstrapper for the game id `ULUS1048`. The bootstrapper is used to load  `UnpluggedDeluxe.prx` into user-space. Make sure **BOTH** `prx` files are in the **SAME** directory on your system.

### PPSSPP (Emulator)

Simply create a folder named `UnpluggedDeluxe` in your PPSSPP plugin directory. Then, download the PPSSPP version of this plugin. Afterwards, drag-and-drop both `UnpluggedDeluxe.prx` and `plugin.ini` into the directory you just created.

## Custom Songs
For conversion of custom audio, please use [ATMTool](https://github.com/maodus/ATMTool), and for custom chart conversion, please use [mid2rbu](https://github.com/maodus/mid2rbu). Additionally, the installation of custom songs may corrupt your save file. Make sure to backup your save file before attempting to install custom songs.

Firstly, you will have to have atleast two DLC songs present on your system of choice. The first one has to be **YOURENOROCKNROLLFUN**. The second one can be any song, but I recommend using **CONSTANTMOTION**, more on that soon. DLC content can be found in `ms0:/PSP/GAME/ULUS1048/MUSIC/`.

After navigating to this directory, simply copy and paste any of your subdirectories and rename it to `ZSONGNAME`. The **Z** prefix is necessary because it is how the plugin recognizes custom content. All custom content **MUST** be prefixed with a **Z**.

 **CONSTANTMOTION** is recommended as the DLC of choice because it has the longest running time (06:57). Any standard DLC song shorter than the running time of the song you import will end abruptly.
 
  Once you have obtained a `ZSONGNAME.rbu` file and a `ZSONGNAME.ATM` file, place them into the new subdirectory you created. Make sure you delete the previous `.ATM` file from the old song you had originally copied. Your custom subdirectory should look something like this after following these steps:

<pre> <code> ZSONGNAME/
	├── TEXT/
	├── BEATMATCH.STR.EDAT
	├── JBxxxxxx.STR.EDAT
	├── JSxxxxxx.STR.EDAT
	├── ZSONGNAME.ATM
	├── ZSONGNAME.rbu
	└── ZSONGNAME_PREVIEW.AT3</code> </pre>

Obtaining a custom `_PREVIEW.AT3` file is optional, just make sure to rename the file with the prefix of your song. After that, launch the game and your custom song should be playable in the quickplay menu!

Work will be done to try and simplify the DLC requirements (ideally to just one), but for now, you must have **YOURENOROCKNROLLFUN** and preferably **CONSTANTMOTION**.

## Remarks

This rendition of Rock Band customs is in no way comparable to the quality of traditional customs. There are most notably issues in regards to memory constraints and phrase timings. This game is tremendously hard to reverse engineer and doing so is very time consuming.

Despite the shortcomings, I hope some of you can enjoy some custom content in Rock Band Unplugged for the first time since its release! I will try to keep improving this plugin, but doing so will take a lot of time. Thank you for understanding.

## Acknowledgements

This project would not have been possible without help from the following:

- MiloHax Discord server.
- Contributors of the RB3 decompilation effort.
