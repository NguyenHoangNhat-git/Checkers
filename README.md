# Checkers game

A checkers game i made back in 11th grade with C++ and OpenGL

Note:

- The game was made in Visual Studio so to modify the project, Visual Studio is required. Go to `\4\4.sln`

## Usage

Download the file `Release.zip`, extract it and run the `4.exe` file (the .exe file has to stay in it Release folder to it to work)

## Game play

### Rules

- Yellow moves first
- Each turn, click on the square of the unit once and click on the square where you want it to go (unit only moves 1 block horizontally)
- Units can jump over an enemy to kill them
- After reaching the end of the other side, a unit has UpgradedMove, and can move with whatever stride it wants (still horizontally)

### How it looks

<img src="README imgs/game play.png" alt="Logo" width="400">

### Info for each move from terminal

- The Got Position give integer position on the board (first one for pick the unit, second one for where it goes)

The `stride` and `UnitStride` are only for developers
<img src="README imgs/terminal.png" alt="Logo" width="400">
