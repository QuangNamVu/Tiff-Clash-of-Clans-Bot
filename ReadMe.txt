******************************************
* About 
******************************************

A bot that automatically raids bases in Clash of Clans. Runs  
object recognition on sprites in the game to find gold mines, 
and simulates mouse clicks inside an android emulator to 
attack and deploy troops.

Author: Ivan Chen

******************************************
* Dependencies 
******************************************

Tessaract OCR 
v3.02
https://github.com/tesseract-ocr/tesseract/wiki/Downloads

OpenCV
v2.4.13
http://opencv.org/downloads.html

Nox App Player
v0.10.0.4321
https://www.bignox.com/

Clash of Clans APK
v8.709.2
https://play.google.com/store/apps/details?id=com.supercell.clashofclans

******************************************
* Additional Notes 
******************************************

- The dependencies are the versions used for testing. May 
  work with other version numbers but not guaranteed. All the 
  libraries are chosen to compile with Microsoft Visual C++ 2010.

- Nox App Player must be running in 1280x720 resolution. 
- Tested with ASUS_T00J device.
- Must be using in DirectX rendering mode, not OpenGL.
- Make sure keyboard shortcut for "Enlarge Screen" is "Up"
- Make sure keyboard shortcut for "Reduce Screen" is "Down"

- Clash of Clans should be installed in Nox. When starting the bot, 
  the game should be open on the home screen. Army should be all 
  goblins, nothing else.