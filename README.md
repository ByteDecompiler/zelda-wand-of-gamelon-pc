# Zelda: The Wand of Gamelon PC Port

```diff
- WARNING! -
This repository doesn't include any game data. To play you need your own copy of the game, as a disc image.
```

This is a **PC port** of ***Zelda: The Wand of Gamelon*** ((Philips CD-i, 1993)), built from the [Zelda: The Wand of Gamelon decompilation](https://github.com/ByteDecompiler/zelda-wand-of-gamelon). The game's own C code runs natively on Windows, and the CD-i's hardware ((video planes, sound, disc playback, the pointer)) is rewritten on top of SDL3. It's not an emulator: there is no CD-i BIOS and no 68000 CPU, except for a tiny interpreter for the enemy graphics, which are stored on the disc as 68000 code.

The game runs on the same engine as *Link: The Faces of Evil*, and so does this port: its the same CD-i stand-in as the [Link PC port](https://github.com/ByteDecompiler/link-faces-of-evil-pc), with the Zelda decomp's code in it.

Its still early. It plays the Philips logo, the title, the menu, the cutscenes and the world map, you can walk around the rooms and fight, and the game's own saves work. Expect bugs.

## Supported discs

All five releases work, with the same exe:

* English: `Zelda - The Wand of Gamelon (Europe).bin` and `Zelda - The Wand of Gamelon (USA).bin`
* French: `Zelda - The Wand of Gamelon (France).bin`
* German: `Zelda - Der Zauberstab von Gamelon (Germany).bin`
* Dutch: `Zelda - De Toverstaf van Gamelon (Netherlands).bin`

The French, German and Dutch versions are seperate builds of the game's program, so each one has its own exe: `zelda_fr.exe`, `zelda_de.exe` and `zelda_nl.exe`. You don't have to pick: `zelda.exe` looks at your disc and starts the right one, as long as they're next to it.

## Playing

1. Build it ((see below)) or get the release zip.
2. Put your disc image next to `zelda.exe` or in a `disc` folder beside it. It has to be a raw image with 2352-byte sectors, the usual `.bin`/`.cue` rip. You can also drag the `.bin` onto `zelda.exe`.
3. Run `zelda.exe`.

Controls:

- Move: arrow keys or WASD, a gamepad's d-pad or left stick, or the mouse
- Button 1 ((sword, select)): Z, Space, J or Enter, gamepad A, left mouse button
- Button 2 ((item, the game menu on the map)): X, Ctrl or K, gamepad B or X, right mouse button
- Esc ((or Start)): the settings menu, the game pauses while its open
- P: pause
- Tab ((or the right shoulder button)): hold to fast-forward
- F5: quick save, F9: quick load, F6 and F7: pick the quick save slot ((1 to 9))
- F12: screenshot

Every key and gamepad button can be changed in the menu, under Controls.

Display:

- F2: pixels. Sharp ((crisp pixels that all come out the same size, the default)), nearest ((plain blocky pixels)) or smooth ((blurred, like a TV))
- F3: shape. TV ((4:3, like on a CD-i, where the pixels are a bit taller than wide)) or square pixels
- F4: whole-number scaling. With square pixels and this on, every pixel is exactly the same size on screen
- F11 or Alt+Enter: fullscreen

The menu also has vsync ((on by default, it stops the picture tearing. The game still runs at the CD-i's speed of 59.94 frames a second, on any screen)), scanlines, a CRT look ((a curved screen that gets darker at the edges, and the red, green and blue stripes of a TV, each can be turned on seperately)), the music, effects and voice volumes, pausing when the window isn't in front, and filling the 2 black lines the CD-i leaves at the top of the screen. All settings are saved in `zelda_port.ini` next to `zelda.exe`, and the window remembers its size and place. The mouse pointer hides itself when you're not using the mouse.

Saves go in a `saves` folder next to `zelda.exe`: the game's own saves ((`zelda1.nvri` to `zelda3.nvri`)), and the quick saves ((`quick1.state` to `quick9.state`)). The menu shows the slot and when it was saved. A quick save only loads into the same build of the exe.

Screenshots go in a `screenshots` folder, as PNGs of the picture the way you see it ((with the scanlines and CRT look if they're on)).

## Play as Link

If you also have *Link: The Faces of Evil*, you can play Zelda's game as Link. Put his disc image next to `zelda.exe` or in the `disc` folder too, or write where it is in `zelda_port.ini` as `link_disc = <path to the .bin>`. Then pick "Hero: Link" in the menu. You can switch back and forth at any time, and it works with every version of Zelda's game.

Both games run on the same engine and the heroes are made the same way: 24 pictures in the same order for the same poses, with the same animations and the same colours. So the port reads Link's pictures and his hit areas off his disc and swaps them in for Zelda's. Nothing else changes: the cutscenes, the voices and the story are still Zelda's.

## Building

You need WSL ((or Linux)) with:

1. `make` and Python 3
2. The 32-bit MinGW compiler: `sudo apt install gcc-mingw-w64-i686`
3. SDL3's MinGW files ((`SDL3-devel-<version>-mingw.tar.gz` from the SDL releases)) unpacked into `third_party/`, so that `third_party/SDL3-3.4.18/i686-w64-mingw32` exists.

Then run `make`. If all goes well you get `zelda.exe`, `zelda_fr.exe`, `zelda_de.exe`, `zelda_nl.exe` and `SDL3.dll` in `build/pc/`.

`make release` makes `build/release/zelda-wand-of-gamelon-pc.zip` to share: the exes without their debug info, `SDL3.dll`, this README and SDL's license.

The port is 32-bit on purpose: the game keeps addresses in 32-bit longs all over the place, just like it did on the CD-i.

## How it works

- `game/` has the decomp's C code with a few changes for the PC.
- `port/` is the stand-in for the CD-i:
  - `main.c` runs the game in its own thread and sends it signals, like the CD-i's interrupts. It also picks the exe for your disc's version
  - `mem.c` sets up the game's memory and its globals, with their starting values taken from the program on your disc
  - `video.c` does what the CD-i's video chip does with the game's display tables ((two planes, CLUT7, RL7 and DYUV pictures, fades))
  - `play.c` streams the game's real-time files off the disc, `disc.c` reads the image
  - `audio.c` decodes the CD-i's ADPCM sound
  - `gap.c` has C versions of the game's hand-written assembly ((sprite and font drawing))
  - `m68k.c` runs the compiled enemy sprites
  - `logo.c` plays the Philips logo ((on the CD-i a seperate program, cdi_bumper))
  - `display.c`, `menu.c` and `settings.c` are the window, the settings menu and `zelda_port.ini`
  - `state.c` is the quick save
  - `hero.c` is the "play as Link" mod
- `tools/gen_globals.py` lays out the globals at their original offsets, for each version.

The CD-i is big-endian and the PC isn't. Data from the disc stays big-endian in memory, and the structs that look at it are marked `BIG_ENDIAN_DATA` so the compiler reads them the right way round.

## Project Structure

```
WandOfGamelon_port
├── config: the decomp's symbol lists (where each global lives, and where they moved in fr, de and nl)
├── game
│   ├── include: the decomp's headers
│   └── src: the decomp's C code
├── port: the CD-i stand-in
├── third_party: SDL3 (not included)
└── tools: build helpers
```
