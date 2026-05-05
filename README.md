# HOW TO RUN IT
## Pre-configure

First of all, create a folder build, in root dir. Then go inside and run 
```bash
cmake ..
```
This will download the raylib. After that, run 
```bash
make raylib
```
to compile the raylib lib.

## Compile the game

To compile the game just in the root dir run 
```bash
make
```
This will create a exec game. 
TO run it write 
```bash
./game
```
For clean build you can destroy the build dir with 
```bash
make clean
```