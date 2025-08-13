# Schlem on Campus Extended Edition
## Summary
Schlem on campus is a doom/wolfenstein like game originally created as an INDA-project during spring 2025 on KTH. The game is somewhat inspired by mottagningen on Datasektionen. 
## Extended edition
[Dante](https://github.com/Elkatrash) aka `Elkatrash`, a member from the original INDA-project group chose to improve on the game during the summer of 2025 and has thus created the Extended Edition, a fork which consists mostly of bugfixes and code refactoring. 
## How to run the game
There is no game binary or executable in the repo you will have to compile the code on your own. For this you will need a C compiler since the game is entirely written in C. If you want to change options such as targetfps or fov you can change them in `include/config.h`. Note that you will need to recompile the game in order for the settings to change. 
### Compilation for Windows
* The easiest option is to open the repo in `vscode` and press `ctrl+shift+B`. The included `.vscode/tasks.json` should be able to compile it directly. 
* Another option is to open `cmd` in the repo directory and run this command `gcc -g -Wall src/main.c src/map.c src/enemy.c src/movement.c src/raycast.c src/sprites.c src/screen.c -Iinclude -Llib/Win64 -lraylib -lopengl32 -lgdi32 -lwinmm -o main.exe`

If you have trouble with your compiler for windows I recommend getting it from [here](https://winlibs.com). Download the latest `Win64` Zip archive and extract the mingw64 to somewhere (I put it in `c:\`). Then add the `mingw64/bin` directory to your `Path environment variable`. That's what worked best during the project.
### Compilation for Linux
There is a `build.sh` script that should download needed dependencies, build and run the game. If that doesn't work ask chatgpt. Here is a usefull prompt: `How do I compile a raylib game, written in c,  on (insert distro here). The source files are in src the headers are in include and the library binaries are in lib/Lin64.` 

## Credits
### Original code, maps and design from the INDA-Project
* [Dante](https://github.com/Elkatrash)
* [Linus](https://github.com/LinusBredin)
* [Ludvig](https://github.com/Ludvig850)

### Sprites and textures (using `ms-paint`)

* Pickups [Ludvig](https://github.com/Ludvig850)
* Other [Dante](https://github.com/Elkatrash)

### Third-Party libraries
This project uses [Raylib 5.5](https://github.com/raysan5/raylib/releases/tag/5.5) Copyright (c) 2013–2025 Ramon Santamaria under the [zlib/libpng License](Docs\RAYLIB_LICENSE.txt) 

