This is a __fork__ of the original hollyhock launcher by The6P4C (https://github.com/Stellaris-code/hollyhock-fork/).
My intention was to modify it a little. I changed the following things:
- BIN:       I added the ability to load prograrms from binary files instead of .hhk files, the extrypoint is the start of the file and the load location is somwhere in the file,
             look at the linker.ld file in the app_template folder.
- EXEC003:   I added the ability to run programs located in program variables. This was the function of the old "LoaderIIb", which is now obsolete.
             I didn't want to miss the feel of typing in programs byte per byte by hand (only if you want to) and sending them over the serial cable.
- Exam mode: The Launcher is disabled in exam mode to prevent cheating. (Even though I would recommend going back to the original firmware for exams.) 
             The loader checks the color of the battery logo to test, if the exam mode is on or off.
- Strings:   I changed a few texts in the OS, the Imaginary Unit button used to load the program was renamed to 'Hollyhock-Launcher'. The version text (usually "classpad II")
             Was changed to "hollyhock" by The6P4C, I changed it to "hollyhock-2".
- Backslash: I will change the backslash character from the symbol `¥` to the symbol `\`.

# DISCLAIMER
- I AM NOT RESPONSIBLE FOR ANY DAMAGE YOU MAY DO TO YOUR fx-CP400.  
- I AM NOT LIABLE IF YOU BRICK OR OTHERWISE DAMAGE YOUR CALCULATOR.  
- CASIO WILL NOT FIX YOUR CALCULATOR IF IT SHOWS EVIDENCE OF TAMPERING, I.E. FLASHING WITH MODIFIED FIRMWARE IMAGES OR MESSING WITH DEBUG MENUS.  
- I WILL NOT FIX YOUR CALCULATOR.  
- IF YOU CANNOT FIX YOUR CALCULATOR YOURSELF, DO NOT USE THIS TOOLKIT.  
- **BY USING THESE TOOLS, YOU AGREE TO TAKE FULL RESPONSIBILITY FOR ANY DAMAGE THAT MAY OCCUR.**  
- **YOU HAVE BEEN WARNED**

# Installation (simple)
This is how you install hollyhock-2 onto your calculator:
- download the newest prebuild release of hollyhock-2: https://github.com/SnailMath/hollyhock-2/releases
- copy the folder Snail2021 from the .zip file in the release onto the desktop.
- open the folder Snail2021 on the desktor and double click on Snail2021.exe
- follow the instructions on screen:
  - Download and install ResourceHacker (scroll down, click on _EXE install_): http://angusj.com/resourcehacker/
  - Download the official updater version 02.01.2 (click the green download button): https://tiplanet.org/forum/archives_voir.php?id=1044960
  - Follow the instructions in the updater, but don't connect the calculator when it asks you to.
- at this point, return back to the first windown (The Snail2021.exe)
- click enter until it asks you to connect the calculator.
- You can close the updater window now.
- When you click enter the last time in the Snail2021.exe window, the updater will open again, but this time it is the modified version.
- Connect the calculator via USB.
- On the calculator hold down the `EXP`, `^` and `Clear` keys. With those keys pressed, momentarily press the RESET button on the back of the calculator. Keep the three front buttons pressed until the screen displays the `OS ERROR` message. Don't worry, there is no actual error, this is just the update mode.
- On the computer, Click on OK in the updater. 
- __DO NOT TOUCH__ the calculator until the update is finished.
- When the updater is finished, unplug it and plug it in again
- Select USB Storage and copy the file run.bin and all .hhk and .bin files (from the folder Snail2021) directly onto the calculator.
- On the computer, click on eject (probably on the bottom right) and eject the calculator.

To start a program, go to the Menu, go to the right, select System, click on the gear on the top left and click on `Imaginary Unit` (I will probably rename this to 'Start hollyhock' or 'Start program' soon.)

Select the program and click OK.


# hollyhock
An SDK to write your own applications for the CASIO fx-CP400 graphics calculator.

This is by no means a finished project. Like the disclaimer says, use at your own risk! Breaking changes are not out of the question, and should be 100% expected.

I (@The6P4C) have burned out *hard* on this project, and won't be maintaining it any further. These tools, patches and SDK work only for OS v02.01.2000.0000. The tools are easily updated, but the patches and SDK are **much** more difficult (and half the reason I'm so burned out).

~~Please contact @ps100000 regarding this project.~~ If you have any questions, you can visit our discord https://discord.gg/y8Mr8tD or comment on YouTube.

## User Guides
1. [Install hollyhock-2](#installation-simple)
2. [Using add-on software](doc/user/using.md)
3. [Developing applications](doc/user/developing.md)
4. [Writing programs without a PC](doc/user/exec.md)
5. [Patching your firmware (from source)](doc/user/patching.md) (complicated, you do not need this, just scroll up to section [Install](#installation-simple)

