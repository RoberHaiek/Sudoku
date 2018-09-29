# Sudoku

This is an advanced game of Sudoku I've made in C,
The game only works on Linux through the terminal, though I'm working on Android Studio to Implement the game on android phones as an app.
To activate the game:
  - Open a terminal on Linux (The easiest way to open a terminal is to use the key combination of Ctrl + Alt + T).
  - Go to the directory of the project (Must know how to use simple linux terminal commands to do so).
  - Type "make" ,and press the Enter key on the keyboard
  - Type "./sudoku"
And now you're in the game.

To play the game you need to enter a command for every step you take (enter name of command [followed by parameters if the command has some] and Enter,but first you must choose a game mode, either solve or edit (will be explained below) and THEN you can use the other commands, here are all the commands:
  - solve X : loads a game with the name "X" you've already editted and saved in the same folder to play it without changing the initial   bame board loaded.
  - edit X : loads a game with the name "X" you've already editted and saved in the same folder to edit it, the parameter X here is optional, if no parameter is supplied, the program enters edit mode with an empty board.
  - mark_errors X : this command is only available in solve mode. Sets X to 0 (doesnt mark the erroneous cells with an asterisk "*" next to it), or 1 (to mark them).
  - print_board : prints the board to the screen.
  - set X Y Z : sets the cell in the X's row, Y's column, to Z.
  - validate : validates is the current board is solvable or not.
  - generate X Y : this command is only available in edit mode when the board is empty. Generates a puzzle by randomly filling X cells (cannot be larger than 25) with random legal values (changing this parameter gives a more distinct board everytime), solves the board completely, then clearing all but Y cells to work with.
  - undo : undoes the last step done.
  - redo : redoes the last step undone.
  - save X : saves the game in the current folder with the name X.
  - hint X Y : this command is only available in solve mode. Gives a hint to the cell <X,Y>.
  - num_solutions : gives the number of possible solutions for the current board (the emptier the board, the more time it takes).
  - autofill : this command is only available in solve mode. Automatically fills all cells containing a single legal value in this exact moment.
  - reset : undoes all the moves reverting the board to its original loaded state.
  - exit : exits the game.
Additional information:
- A fixed cell in the board is marked with a dot "." next to the number in solve mode, fixed cells cannot be changed.
- The game uses another application called Gurobi to solve the board using integer linear programming.
- The game uses an exhaustive backtracking algorithm to calculate the number of solutions.

Enjoy!
