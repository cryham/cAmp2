# cAmp2

**WIP** reimplementation of my Audio Player [Crystal AMP (cAmp)](http://cryham.tuxfamily.org/portfolio/2010_camp/) for GNU/Linux (and Windows).  
Currently needs building from sources.  

# New features

* Gui with options in few own windows (for find, tab, etc.)
* All program keys bindable
* Visualization themes with options and sliders in Gui

# To do
## restore old

* move track, selection (critical), insert at cursor
* hotkeys (how?)
* threads for spectrogram, insert files, etc.
* use shaders for visualizations

## add new

* queue (set tab, add to, keys)
* rename files, dirs
* skins, xml


# Using

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

I'm testing on Debian 11.  
Building with CMake in Qt Creator is easy.  
Building in VSCodium / VS Code is possible too, with some more effort. Needs C++ and CMake extensions at least.
