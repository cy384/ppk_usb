palm keyboard adapter
=====================
This code is the software and 3D printable hardware for using an ATMega32U4-based Arduino-compatible device to adapt a Palm Portable Keyboard (PPK) to function as a USB HID.  The "Think Outside(TM) Hardware and Electronics Reference for the Palm(R) and Stowaway(TM) Portable Keyboards Revision 1.1" document was used as a reference, which I found [here](http://www.splorp.com/pdf/stowawayhwref.pdf).  For more information about my work with the PPK, see [my writeup](http://www.cy384.com/projects/palm-keyboard.html).

![ppk demo](http://i.imgur.com/oBWkVBG.jpg)

hardware variants
-----------------
![ppk headers](http://i.imgur.com/w6m0yrR.jpg)
There are at least two physically incompatible variants of the PPK: one for the Palm III series (black plastic, p/n P10713U), and one for the Palm V series (dark grey metal, p/n 3C10439).  `palm-III-adapter.scad` fits the Palm III PPKs, and `palm-V-adapter.scad` fits the Palm V PPKs.

3d printed parts
----------------
The OpenSCAD designs print quickly on my cheap printer and have a reasonable friction fit with my PPKs.  Depending on your printer, you may need to adjust some of the smaller dimensions.

wiring
------
As per the spec linked above, the pinout of the header on the PPK (viewed from front/above):

	[NC] [VCC] [RXD] [RTS] [NC] [NC] [DCD] [NC] [NC] [GND]

The Arduino pins are #define'd at the top of the file, feel free to change them to suit your hardware.

The only external component you will need is an approximately 10K ohm pull-down resistor on the receive (RXD) line.

key mapping
-----------
I've mapped the keys according to the spec and what's printed on the keyboard, excepting the keys that don't map logically to anything on a standard keyboard (e.g. the ones for starting Palm OS apps).  A few additions: Fn+1 for Escape, Fn+up for page up, Fn+down for page down, Fn+left for home, and Fn+right for end.  Unmapped Fn keypresses fall through to the normal mapping.  Cmd is mapped to super (aka Windows/Apple key).  Adding to or modifying the mapping is straightforward, just edit the config_keymap and config_fnkeymap functions.

caveats
-------
* Due to the way I implemented the Fn key, it can have slightly weird behavior if you press a normal key, then Fn, then another key, then release them in a certain order.  This could be fixed by keeping a list or something of keypresses sent.
* I believe I properly implemented management of the low power mode, but found that the keyboard fell asleep regardless, so instead this code just reboots the keyboard periodically, as per the TIMEOUT value.
* I have only tested on p/n 3C10439 and p/n P10713U.

license
-------
This code is covered by the included BSD 3-clause license (see LICENSE file).  Any patches, bug reports, or other commments are greatly appreciated.
