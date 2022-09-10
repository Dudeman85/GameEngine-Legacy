# Johdanto pelimoottoriohjelmointiin

## Building
### Linux
Prerequirements (Ubuntu):
```bash
sudo apt-get install -y build-essential cmake libx11-dev xorg-dev
sudo apt-get install -y freeglut3-dev freeglut3-dev libudev-dev libxcursor-dev libxi-dev libxinerama-dev libxrandr-dev libfreetype6-dev libopenal-dev libvorbis-dev libflac-dev 
```
Building:
```bash
mkdir build
cd build
cmake ../
cmake --build .
```
