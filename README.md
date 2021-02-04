# YetAnotherRacingGame

This is a simple 2D racing arcade created with open-source library SDL2.

To play the game, just copy the folder "SuperRacingPro" to your machine and run the file Racing.exe (sorry, this build is only for Windows users).

All the basic game engine functionality was built separately and linked as a static library called TGE.lib. In order to create your own 2D game, just add the "./include/TGE" and "./lib/TGE" folders to your project include and library paths, respectively. You will also need some DLLs from SDL library to run the programs, so add them to the folder in which your .exe file will be stored. There are millions of tutorials on how to link DLL to your project. TGE is just a very tiny layer on the SDL that provides you basic classes for game development.

Enjoy!
