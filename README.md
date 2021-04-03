# cAmp2

**WIP** reimplementation of my old [cAmp Crystal Audio Player](http://cryham.tuxfamily.org/portfolio/2010_camp/) for GNU/Linux.

Using:
* for Audio: [bass](https://www.un4seen.com/bass.html) (NOT FOSS)
* for Graphics: [SFML](https://www.sfml-dev.org/) 2.5.1

Also using (included in `libs/`):
* for Gui: [ImGui](https://github.com/ocornut/imgui) 1.82 and [ImGui-SFML](https://github.com/eliasdaler/imgui-sfml) 2.2
* [TinyXML-2](https://github.com/leethomason/tinyxml2)
* [Native File Dialog](https://github.com/mlabbe/nativefiledialog)

# Building
Uses C++17 and CMake to build.

First to install SFML (on Debian like):
```
sudo apt-get install libsfml-dev
```
To get sources:
```
git clone https://github.com/cryham/cAmp2.git cAmp2
cd cAmp2
```
To build:
```
mkdir bin && cd bin
cmake ..
make -j
```

I'm testing on Debian 10  
and building with CMake in Qt Creator.  
With some more effort, building from VSCodium / VS Code is possible (needs C++ and CMake extensions at least).
