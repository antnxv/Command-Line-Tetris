# 🕹 Command Line Tetris 🎮
An implementation of Classic Tetris that runs in your console window.

![tetris](https://user-images.githubusercontent.com/34826608/208236605-6fca4c80-bbea-4b07-a2bd-76149d8c662e.png)

### Dependencies
- gcc
- make
- ncurses-dev (`ncurses.h`)
- pthread (`pthread.h`)
- Permission to write file (stores highscore)

### Instructions
```bash
make
```
Builds tetris.out executable. If `make` is not available, see [makefile](makefile) for compile steps

```bash
./tetris.out
```
Launches a game of Tetris.  

### Gameplay
- Controls
  - W / Up Arrow: Rotate Piece Clockwise
  - A / Left Arrow: Move Piece Left
  - D / Right Arrow: Move Piece Right
  - S / Down Arrow: Soft Drop
  - Spacebar: Hard Drop
  - Q: Quit
- Endless
  - No implemented final level or ending (aside from loss)
- Highscore
  - Saves highscore locally to `user.dat`
