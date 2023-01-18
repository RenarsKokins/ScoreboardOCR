<p align="center">
  <img src="/scoreboardocr/graphics/logo.png" />
</p>

## About ScoreboardOCR

ScoreboardOCR is a program made for sports game scoreboard recognition. That means that you can get real-time scoreboard data on your computer only using a camera which is pointed at real life scoreboard. ScoreboardOCR will recognize digits/numbers in selection which the user has specified and save them to a file ~10-15 times a second.

## How it works

ScoreboardOCR uses a custom SVM (source vector machine) to recognize digits. This SVM can be trained by you, if necessary. For example, the current SVM provided in this project is trained only on 7-segment displays and probably won't work on numbers which are in a different font.

## How to use it

*Video tutorial is coming soon...*

1. Build ScoreboardOCR from source or get the executable (more info about installation is below),
2. Run the program,
3. Check if SVM file is loaded (so that we can recognize numbers) by clicking "settings" and checking if "Is SVM loaded" is "yes". If not, set path to SVM file, click OK, then "settings" again,
4. Select a capture device from dropdown menu (if no devices are shown, check if camera(-s) can be accessed from a different program),
5. Click start capture in "capture" tab and you should see the camera output,
6. Click "Mark edges" and mark/click four corners of the real life scoreboard,
7. Adjust filters (threshold, erode, italic, etc.) so that scoreboard numbers are clearly visible and in white color, while everything else is black,
8. Click "output" tab and make sure output file path, name and format is set,
9. Click "recognition" tab and press "add selection",
10. Enter the selection name and its format,
11. Using your mouse, draw a box over the numbers you want to detect,
12. Check the output file and if it has some output, everything is working!

## Installation

### Windows

You can either get the compiled executable (easier) or build the program from source (harder).

#### Build from source

1. Get QT creator/QT6 framework on your computer,
2. Get OpenCV source, build OpenCV,
3. Add OPENCV_SDK_DIR system variable (with path to opencv folder, where you built it to),
4. Build ScoreboardOCR project,
5. If no errors occur, you are done!

#### Executable for Windows

Coming soon!

### Linux

Right now it is only possible to build from source.

1. Get QT creator/QT6 framework on your computer (``sudo pacman -S qt6-base`` or ``sudo apt install qt6-base-dev``),
2. Get OpenCV (``sudo pacman -S opencv`` or build from [source](https://docs.opencv.org/4.x/d7/d9f/tutorial_linux_install.html)),
3. Get ``pkg-config`` package (``sudo pacman -S pkg-config`` or ``sudo apt-get install pkg-config``),
4. Build ScoreboardOCR project,
5. If no errors occur, you are done!

