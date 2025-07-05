# Unplugged Deluxe

> This project is not affiliated with or endorsed by Harmonix, MTV Games, EA, or any other official entity associated with Rock Band. It is a fan-made utility for educational and personal use only.

This is a plugin for the Playstation Portable that allows for custom content to be utilized within the game Rock Band Unplugged. This plugin will only work for the US version of the game (ULUS10418). The CRC for the game's `.ISO` that has been used to develop this plugin is `FC17DAFF`.

## Plugin Installation

This plugin can be installed on both official hardware and emulators. However, installation on a PSP will require some extra steps and Custom Firmware (CFW) with plugin capabilities.

### PSP (Retail Hardware)

Firstly, make sure you are using an up-to-date revision of CFW. Next, you will need to install the plugin np loader mod. Next, download the PSP version of this plugin from the releases section. Inside of the archive, you will find two files: `UDBootsrap.prx` and `UnpluggedDeluxe.prx`. You will **ONLY** want to install the `UDBootstrap.prx` plugin on your system. Make sure to install the bootstrapper for the game id `ULUS1048`. The bootstrapper is used to load  `UnpluggedDeluxe.prx` into user-space. Make sure **BOTH** `prx` files are in the **SAME** directory on your system.

### PPSSPP (Emulator)

Simply create a folder named `UnpluggedDeluxe` in your PPSSPP plugin directory. Then, download the PPSSPP version of this plugin. Afterwards, drag-and-drop both `UnpluggedDeluxe.prx` and `plugin.ini` into the directory you just created.

## Custom Songs

### Custom Audio
Rock Band Unplugged utilizes custom encrypted audio in the form of `.ATM` files for playing a song's audio tracks within the game. Beneath the encryption, these `.ATM` files contain your standard 4 instrument tracks (drum, bass, guitar, and vocal), with the addition of an addtional audio track for miscellanious sound effects. I have written a tool that allows for the creation of this audio file format, so please follow the necessary steps found on [ATMTool's](https://github.com/maodus/ATMTool) repo. This procedure is mandatory for your custom song to have custom audio. After following the encryption and conversion processes listed on the ATMTool repo, you will have obtained a `.ATM` file that can be used in-game.

**Note**: This plugin patches code in Rock Band Unplugged to allow for the loading of any `.ATM` file. If you choose to only swap the audio of any song in game without the use of this plugin, please be aware that it may not work.

### Custom Charts/Metadata
If you want your song to have custom charts and metadata, I have written an additional tool, [mid2rbu](https://github.com/maodus/mid2rbu), for this process. Please install mid2rbu, and read the documentation found on the repo for the conversion process. The tool will allow you to convert any standard Fortnite Festival/Rock Band chart into a format that this plugin can recognize (a `.rbu` file). The `.rbu` file will contain song metadata and chart information for you custom song.

Please keep backups of your mid2rbu config and any midi tracks. The parser will be improved/updated frequently, and you will likely have to re-generate your custom song's charts.

### Custom Song Installation
> WARNING: The installation of custom songs may corrupt your save file. Make sure to backup your save file before attempting to install custom songs.

Firstly, you will have to have atleast two DLC songs present on your system of choice. The first one has to be **YOURENOROCKNROLLFUN**. The second one can be any song, but I recommend using **CONSTANTMOTION**, more on that soon. DLC content can be found in `ms0:/PSP/GAME/ULUS1048/MUSIC/`, or if you are emulating, `PPSSPP_INSTALL_DIR/PSP/GAME/ULUS1048/MUSIC/`.

After navigating to this directory, you will find a list of DLC subdirectories (assuming you have some) that may look something like this:

<pre> <code> PSP/GAME/ULUS1048/MUSIC/
	├── CONSTANTMOTION/
	├── .../
	└── YOURENOROCKNROLLFUN/</code> </pre>


The first step in the custom installation process will be to simply copy and paste any of your subdirectories (*preferably CONSTANTMOTION*) and rename it to `ZSONGNAME`. For example, if you were importing the song *Know Your Enemy*, you would name the folder `ZKNOWYOURENEMY`. The **Z** prefix is necessary because it is how the plugin recognizes custom content. All custom content **MUST** be prefixed with a **Z**.

 **CONSTANTMOTION** is recommended as the DLC of choice because it has the longest running time (06:57). Any standard DLC song shorter than the running time of the song you import will end abruptly.

After creating the new custom DLC subdirectory, you DLC folder should look something like this:

<pre> <code> PSP/GAME/ULUS1048/MUSIC/
	├── CONSTANTMOTION/
	├── .../
	├── YOURENOROCKNROLLFUN/
	└── ZSONGNAME/</code> </pre>
 
Now the rest of this installation process assumes you are in possession of a custom `.rbu` and `.ATM` file, if you do not have these, please the read the instructions on [custom audio](https://github.com/maodus/UnpluggedDeluxe#custom-audio) and [custom chart/metadata](https://github.com/maodus/UnpluggedDeluxe#custom-chartsmetadata) creation. Next, please make sure to rename your `.rbu` file and `.ATM` file to the exact same **uppercase** name as the DLC folder you created in the last step (i.e. `ZSONGNAME`). Once you have obtained a `ZSONGNAME.rbu` (chart/metadata) file and a `ZSONGNAME.ATM` (audio) file, place them into the new folder you just created. Make sure you delete the previous `.ATM` file from the old song you had originally copied. Your custom subdirectory should look something like this after following these steps:

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
