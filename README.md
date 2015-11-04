palm keyboard adapter
=====================
This code is the software and 3D printable hardware for using an ATMega32U4-based Arduino-compatible device to adapt a Palm Portable Keyboard (PPK) to function as a USB HID.  The "Think Outside(TM) Hardware and Electronics Reference for the Palm(R) and Stowaway(TM) Portable Keyboards Revision 1.1" document was used as a reference, which I found [here](http://www.splorp.com/pdf/stowawayhwref.pdf).  For more information about my work with the PPK, see [my writeup](http://www.cy384.com/projects/palm-keyboard.html).

![ppk demo](http://i.imgur.com/oBWkVBG.jpg)

parts list
----------
* Pro Micro Arduino-compatible device ([Sparkfun](https://www.sparkfun.com/products/12640) or [eBay](http://www.ebay.com/sch/i.html?_nkw=pro%20micro%20atmega32u4)).
* 10k ohm 1/4W resistor
* ~26 AWG solid core wire (I got mine from a dead ethernet cable)
* 3D printed top part (either the V or III version, see below, print it yourself or grab one on [Shapeways](http://www.shapeways.com/designer/cy384))
* 3D printed bottom part (same for both, print it yourself or grab one on [Shapeways](http://www.shapeways.com/designer/cy384))

hardware variants
-----------------
![ppk headers](http://i.imgur.com/w6m0yrR.jpg)

There are at least two physically incompatible variants of the PPK: one for the Palm III series (black plastic, p/n P10713U), and one for the Palm V series (dark grey metal, p/n 3C10439).  Electronically, they're identical, but we'll need to wire them up a bit differently.

3d printed parts
----------------
The OpenSCAD designs print quickly on my cheap printer and have a reasonable friction fit with my PPKs.  `pro-micro-socket.scad` has the pieces you'll want to print.  `palm-V-adapter.scad` and `palm-III-adapter.scad` specify only the headers, in case you want to include them in other projects or need to adjust the fit for your printer.

assembly and wiring
-------------------
![wiring pic](http://i.imgur.com/rbRO9p4.jpg)

As per the spec, the pinout on the PPK (viewed from front of the keyboard, keep in mind how the adapter fits onto the keyboard to wire correctly):

	[NC] [VCC] [RXD] [RTS] [NC] [NC] [DCD] [NC] [NC] [GND]

Cut your wire into ~50mm sections, leaving ~10mm of insulation on one end (you'll need five of these).  Bend the wire at the end of the insulated part, thread the exposed part through the appropriate hole in the printed header, and push the insulated part into the other side of the header to anchor it.

![palm v assembled top pic](http://i.imgur.com/3fpSvYz.jpg)

For the Palm V adapter, thread the wires into the Pro Micro as follows:

	VCC -> 7
	RXD -> 6
	RTS -> 5
	DCD -> 4
	GND -> 2

DO NOT solder or trim pin 6 yet.  Solder pins 2, 4, 5, and 7.  Clip the extra wire off of those pins.  Now, use the protruding wire to connect pin 6 to pin 8 (solder pin 8 now, but not 6).  Trim and solder one end of the 10K ohm resistor into pin 15, and wrap the other end onto pin 6.  Solder and trim up your pin 6 mess.  You should now be done and ready to program.

![palm III assembled top pic](http://i.imgur.com/UUuMWqe.jpg)

For the Palm III adapter, thread the wires into the Pro Micro as follows:

	VCC -> 2
	RXD -> 3
	RTS -> 4
	DCD -> 5
	GND -> 6

DO NOT solder or trim pin 3 yet.  Solder pins 2, 4, 5, and 6.  Clip the extra wire off of those pins.  Now, use the protruding wire to connect pin 3 to pin 8 (solder pin 8 now, but not 3).  Trim and solder one end of the 10K ohm resistor into pin 15, and wrap the other end onto pin 3.  Solder and trim up your pin 3 mess.  You should now be done and ready to program.

programming
-----------
Open up `arduino_ppk_usb.ino` and edit PPK_VERSION to match your keyboard (5 for V, or 3 for III).  Set the device to Arduino Leonardo in the IDE, plug in your cable, and hit upload.  If it can't find a device, you may need to reset it first by grounding the RST pin.

Unplug/replug the cable and it should be fully functional.

Once you're sure it's working, stick the 3D printed cover piece on using glue, tape, [friction welding with a spare bit of plastic filament](http://hackaday.com/2014/12/30/3d-printing-technique-friction-welding/), or nothing.

key mapping
-----------
I've mapped the keys according to the spec and what's printed on the keyboard, excepting the keys that don't map logically to anything on a standard keyboard (e.g. the ones for starting Palm OS apps).  A few additions: Fn+Tab for Escape, Fn+number keys for F1-F10, Fn+- for F11, Fn+= for F12, Fn+up for page up, Fn+down for page down, Fn+left for home, and Fn+right for end.  Unmapped Fn keypresses fall through to the normal mapping.  Cmd is mapped to super (aka Windows/Apple key).  Adding to or modifying the mapping is straightforward, just edit the config_keymap and config_fnkeymap functions.

caveats and weirdness
---------------------
* Due to the way I implemented the Fn key, it can have slightly weird behavior if you press a normal key, then Fn, then another key, then release them in a certain order.  This could be fixed by keeping a list or something of keypresses sent.  The matrix only guarantees 2KRO, so you're pushing your luck anyway.
* I believe I properly implemented management of the low power mode, but found that the keyboard fell asleep regardless, so instead this code just reboots the keyboard periodically, as per the TIMEOUT value.
* I have only tested on p/n 3C10439 and p/n P10713U.
* Only some pins on my Pro Micros work with the software serial library, so some of the wiring is a bit weird (maybe that's what I deserve for using cheap knockoffs).

license
-------
This code is covered by the included BSD 3-clause license (see LICENSE file).  Any patches, bug reports, or other commments are greatly appreciated.
