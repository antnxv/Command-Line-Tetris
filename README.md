# 🕹 Command Line Tetris 🎮
An implementation of Classic Tetris that runs in your console window.

<img width="674" height="741" alt="preview" src="https://github.com/user-attachments/assets/5b1557a3-2d55-4a4d-96a0-5378d34f3d3d" />

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
