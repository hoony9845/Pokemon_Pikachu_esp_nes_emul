ESP32-NESEMU, a Nintendo Entertainment System emulator for the ESP32
====================================================================

original:  https://github.com/MittisBootloop/esp32_nesemu_wemosmini

video: https://youtu.be/8eZadN7_EwI

Warning
-------

This is a proof-of-concept and not an official application note. As such, this code is entirely unsupported by Espressif.


Compiling
---------

1) You can compile this code with espressifs esp-idf (ESP32) 
branch: release/v3.2
commit: 9ae3df7ec27496d351938542c6c5390ea802fc06

2) You can use prebuilt images from prebuilt_images/

Keys
----------

Up, Down, Right, Left, Select, Start, B, A 

meun key : Select + Start


Menu (Select + Start)
---------

Save State (Left Key)
Load State (Right Key)
Volume (Up/Down Key)
Turbo A key (A key)
Turbo B key (B key)


ROM
---

This includes no Roms. You'll have to flash your own Roms and modify the roms.txt according to your needs.
Don't change the Layout from roms.txt, otherwise it could happen, that the menu will not load.
Description on how to change the roms.txt and where to place the Roms are in the file itself.

You can flash up to 14 Roms.
4x up to 100KB
2x up to 132KB
4x up to 260KB
1x up to 296KB, 388KB, 516KB, 772KB

Flash the Roms with the flashrom.sh script, you'll need to add an argument for flash adress(0x12345) and one for the 
file you want to flash: ./flashrom.sh 0xTargetAdress RomName.nes
Flash the roms.txt with the flashtxt.sh script: ./flashtxt roms.txt

If you flash, for example, only to Rom-Partition 2, you'll have to give Rom1 in roms.txt anyhow an Name (a place-maker)
otherwise the menu couldn't load the right partition.

Copyright
---------

Code in this repository is Copyright (C) 2016 Espressif Systems, licensed under the Apache License 2.0 as described in the file LICENSE. Code in the
components/nofrendo is Copyright (c) 1998-2000 Matthew Conte (matt@conte.com) and licensed under the GPLv2.
