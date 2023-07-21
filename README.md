## About cAmp2

Reimplementation of my Audio Player [Crystal AMP (cAmp)](https://cryham.tuxfamily.org/portfolio/2010_camp/) for GNU/Linux (and Windows).  

Website with some more description of [cAmp2 here](https://cryham.tuxfamily.org/portfolio/2020-camp2/).  
Screenshots [gallery here](https://photos.app.goo.gl/xKwuNdeLnh1d5NAW9).

Currently needs building from sources.  
Mostly working well, with few new things, but still some missing.


## Features

### New

* Gui with options in many own windows (for find, view, tab, playlist etc.)
* All program keys bindable, filterable list
* Visualization themes with options and sliders in Gui
* Queue tab (set tab as Queue Alt-E, add to queue key: E)

### To do
#### restore old

* move track, selection (critical), insert at cursor
* hotkeys (how?)
* threads for spectrogram, insert files, etc.
* use shaders for visualizations (not very needed)

#### add new

* rename files, dirs
* skins, xml


## Using

Libraries used:
* for Audio: [bass](https://www.un4seen.com/bass.html) (NOT FOSS)
* for Graphics: [SFML](https://www.sfml-dev.org/) 2.5.1

and also included in `libs/`:
* for Gui: [ImGui](https://github.com/ocornut/imgui) 1.82 and [ImGui-SFML](https://github.com/eliasdaler/imgui-sfml) 2.2
* [TinyXML-2](https://github.com/leethomason/tinyxml2)
* [Native File Dialog](https://github.com/mlabbe/nativefiledialog)


## Building
Uses C++17 and CMake to build.

Basic setup (on Debian like):
```
sudo apt-get install g++ git cmake make binutils
```
Extras:
```
sudo apt-get install gdb clangd
```
Then install SFML:
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

I'm testing on Debian 11 only.  
Building with CMake in Qt Creator is easy.  
Building in VSCodium / VS Code is possible too, with some more effort. Needs C++ and CMake extensions at least.
