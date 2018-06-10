# Flappy-Bird-StarWarsVersion
C++ Project for CS 179.14A (SPECIAL TOPICS IN MULTIMEDIA: PC AND CONSOLE GAME DEVELOPMENT I)

**Data driven** - Text.txt edits the spawn info of the ships 
* Format: (time-between-ship-spawns direction-of-ship lowest-y-coordinate-of-ship-spawn highest-y-coordinate-of-ship-spawn speed-of-ship)
* Ex. 1000 left 200 800 30 (which means the ships will spawn 1 second after the other, moving from right to left. The ships will only spawn between the y-coordinates 200 and 800 at a y velocity of 30)

**Executable file**: FlappyWars

In order to execute the game, download all files into one folder. Open the folder in a command prompt/terminal and run the executable (input "FlappyWars" for windows, "./FlappyWars" for mac/linux).

**Notes**

* Game only ends when the player goes too high or too low
* When player gets damaged, it doesn't end the game. It simply shows player is damaged.
* Player can be damaged an infinite number of times
* After a game over, the game simply closes. In order to play again, the player must simply rerun the game.
