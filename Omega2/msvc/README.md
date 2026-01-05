\### These are additional files to allow Omega2 to be built using Visual Studio 2022



You need to download the latest SDL2 Dev package from github



https://github.com/libsdl-org/SDL/releases/tag/release-2.32.8



SDL2-devel-2.32.8-VC.zip



You need to put the contents of the 'include' folder into



msvc/include/SDL2



You need to put the contents of the 'lib' folder into



msvc/lib/SDL2



After this you will be able to build and run the 64bit Debug build of Omega2 in Visual Studio 2022 using the .sln file.



Options that need to be changed to get other configurations working are



Configuration Properties->Advanced->Character Set: Use Multi-Byte Character Set

C/C++->General->Additional Include Directories: include

C/C++->Preprocessor: add the following INLINE=static inline;\_CRT\_SECURE\_NO\_WARNINGS

C/C++->Code Generation->Struct Member Alignment: 1 Byte (/Zp1)

C/C++->Advances->Forced Include File: compat.h

Linker->Input: add the following lib\\SDL2\\x64\\sdl2.lib;lib\\sdl2\\x64\\sdl2main.lib

Linker->System->Stack Reserve Size: 13417728



