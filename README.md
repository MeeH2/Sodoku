# Sodoku
Sudoku Solution Validator Readme

This program is a multi-threaded Sudoku solution validator written in C language by Sarmad Hashmi. The program uses 27 threads to determine if the given Sudoku puzzle solution is valid. The threads are categorized into 9 threads for each 3x3 subsection, 9 threads for the 9 columns, and 9 threads for the 9 rows.

How it works

The program defines a 9x9 Sudoku puzzle solution in a 2D array. The program then creates 27 threads and assigns each thread with a specific 3x3 subsection, row or column in the puzzle. Each thread is responsible for checking if the corresponding region of the puzzle they were assigned to is valid. If valid, the thread updates its index in a global array to 1 indicating that the corresponding region in the puzzle is valid.

The program then waits for all threads to complete their execution and checks if all entries in the valid array have been set to 1. If yes, the solution is valid. If not, the solution is invalid.

Functions

The program consists of three functions:

isColumnValid: Determines if the numbers 1-9 only appear once in a column.
isRowValid: Determines if the numbers 1-9 only appear once in a row.
is3x3Valid: Determines if the numbers 1-9 only appear once in a 3x3 subsection.
Each function is called by a thread and receives a parameter struct that indicates which row or column or 3x3 subsection of the Sudoku puzzle it is responsible for checking.

Usage

The Sudoku puzzle solution is stored in a 2D array named sudoku defined at the beginning of the program. By default, a Sudoku solution is already defined in the program.

To use your own Sudoku puzzle solution, replace the values of the sudoku array with your own Sudoku puzzle values. Make sure to define your own Sudoku puzzle in the same format as the default one.

You can also use a text file that contains a 9x9 Sudoku puzzle solution as input. Make sure to format the text file so that each row is in a new line, and each value in a row is separated by a space. The program will read the text file and populate the sudoku array with the values.

Dependencies

This program requires the pthread library to be installed in your system. You can install it by running the following command on a terminal window:

arduino
Copy code
sudo apt-get install libpthread-stubs0-dev
Conclusion

The multi-threaded Sudoku solution validator program is a useful tool to check the validity of a given Sudoku puzzle solution. The program is scalable and can handle larger Sudoku puzzles by simply increasing the number of threads assigned to each region of the puzzle.
