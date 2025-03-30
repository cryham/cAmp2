## About cAmp2

Reimplementation of my Audio Player [Crystal AMP (cAmp)](https://cryham.org/portfolio/2009-crystal-amp/) for GNU/Linux (and Windows).

Website with some more description of [cAmp2 here](https://cryham.org/portfolio/2020-camp2/).
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

* refresh dir F6, or whole playlist, find missing file after rename
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

Basic setup (on Debian-based):
```
sudo apt-get install g++ git cmake make binutils
```
Extras (not needed to build):
```
sudo apt-get install gdb clangd clang
```
Then install SFML and Gtk3:
```
sudo apt-get install libsfml-dev libgtk-3-dev
```

Now inside some folder, let's call it `dev/`.  
To setup bass, download [bass24-linux.zip](https://www.un4seen.com/download.php?bass24-linux) (clicked Linux [there](https://www.un4seen.com/])).  
I unpacked it into `dev/bass/` subdir, then created `x64` dir like so: `dev/bass/src/x64/` and copied `libbass.so` there, from `/bass/libs/x86_64/`.  
These paths can be changed in `CMakeLists.txt`, top under `#  bass`

To get sources, back inside `dev/`:
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

Then to start, just run the binary `cAmp2`.

I tested on Debian 11 and 12 only.  

Building with CMake in Qt Creator is easy.  
Building in VSCodium / VS Code is possible too, with some more effort.  
More info in my [C++ guide](https://cryham.org/cpp-guide//#IDEs) under IDEs.
