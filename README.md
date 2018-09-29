# Sudoku

This is an advanced game of Sudoku I've made in C,
The game only works on Linux through the terminal, though I'm working on Android Studio to Implement the game on android phones as an app.
To activate the game:
  1- Open a terminal on Linux (The easiest way to open a terminal is to use the key combination of Ctrl + Alt + T).
  2- Go to the directory of the project (Must know how to use simple linux terminal commands to do so).
  3- Type "make" ,and press the Enter key on the keyboard
  4- Type "./sudoku"
And now you're in the game.

To play the game you need to enter a command for every step you take (enter name of command [followed by parameters if the command has some] and Enter,but first you must choose a game mode, either solve or edit (will be explained below) and THEN you can use the other commands, here are all the commands:
  1- solve X : loads a game with the name "X" you've already editted and saved in the same folder to play it without changing the initial   bame board loaded.
  2- edit X : loads a game with the name "X" you've already editted and saved in the same folder to edit it, the parameter X here is optional, if no parameter is supplied, the program enters edit mode with an empty board.
  3- mark_errors X : this command is only available in solve mode. Sets X to 0 (doesnt mark the erroneous cells with an asterisk "*" next to it), or 1 (to mark them).
  4- print_board : prints the board to the screen.
  5- set X Y Z : sets the cell in the X's row, Y's column, to Z.
  6- validate : validates is the current board is solvable or not.
  7- generate X Y : this command is only available in edit mode when the board is empty. Generates a puzzle by randomly filling X cells (cannot be larger than 25) with random legal values (changing this parameter gives a more distinct board everytime), solves the board completely, then clearing all but Y cells to work with.
  8- undo : undoes the last step done.
  9- redo : redoes the last step undone.
  10- save X : saves the game in the current folder with the name X.
  11- hint X Y : this command is only available in solve mode. Gives a hint to the cell <X,Y>.
  12- num_solutions : gives the number of possible solutions for the current board (the emptier the board, the more time it takes).
  13- autofill : this command is only available in solve mode. Automatically fills all cells containing a single legal value in this exact moment.
  14- reset : undoes all the moves reverting the board to its original loaded state.
  15- exit : exits the game.
  
- A fixed cell in the board is marked with a dot "." next to the number in solve mode, fixed cells cannot be changed.
- The game uses another application called Gurobi to solve the board using integer linear programming.
- The game uses an exhaustive backtracking algorithm to calculate the number of solutions.

Enjoy!
