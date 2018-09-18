# MagicSquares

This is a program that I wrote as an assignment for Algorithms class at UCCS. It finds all magic squares of size N, which is defined at the start of the program.

A magic square, as defined for this project, is an NxN matrix that contains the numbers 1 through N<sup>2</sup> arranged such that the numbers in each row, column, and main diagonal have the same sum. The smallest magic square is 3 by 3 and contains the numbers 1 through 9, as follows:

8 1 6 <br>
3 5 7 <br>
4 9 2

This problem is difficult to solve; its time complexity is O((N<sup>2</sup>)!). I've set the problem size to 4 here, and on my personal laptop, which has two cores, the program runs for about one minute. You can set the problem size to 3 to get the answer shown above. If you set it to 5, the program may not find all the magic squares in this lifetime, but it should find at least one of them within eight hours. (Results vary by hardware, of course.)

My program contains the following optimizations to cut down on the execution time:

- The program does not count rotations or reflections of the same magic square. To enforce this, it checks to make sure that the upper left corner cell contains a larger number than any of the other corner cells, and that the upper right cell has a larger number than the lower left cell.
- As it assigns numbers, the program keeps track of how many valid corner numbers are left. For instance, if the upper left corner contains a 5, valid corner numbers are 1, 2, 3, and 4. If at any time it discovers that the number of valid corner numbers is the same as the number of corners left to assign, it will not assign any more corner numbers to non-corner cells.
- After assigning the second-to-last number in each row, the program will compute the last number for the row and assign it to the last cell rather than trying each available number. It does the same for each column. If the correct number is unavailable, the program will immediately stop pursuing the entire current branch of permutations. 
For instance, for a 4x4 magic square, the program may assign the numbers 16, 15, and 14 to the first three cells. To add to the correct sum, the last number in the row would have to be -11, which is not available, so the program immediately tries a different number for the third cell.
- The program creates several threads to perform its calculations; this speeds up the program on a multicore machine. Note that this program uses the C++ &lt;thread&gt; </pre> library, so C++11 or higher is required.
  
Results are printed to a text file; an example is posted. Each line represents one square; the first row from left to right is listed first, followed by the second row and so on. Yours may vary due to different execution time of different threads, but it should contain all the same results in some order.

This program and its documentation are available as examples of my work and are intended to be used for informational purposes only. Please do not copy or use this code for any other purpose without my permission.

## Update 9/14/2018

Today's enhancement offers a big boost in efficiency due to improvements to the autoCol method. Previously, the program would wait until the entire second-to-last row was filled before assigning any numbers to the bottom row. Now, its autoColBasic method will assign a number to the second-to-last square in a column and then immediately calculate the required number for the last square. This means that the last two rows of the magic square are filled together.

Implementing the new method caused me to lose a few of the existing optimizations, so more fiddling is to come.

## Update 9/17/2018

I've added another two enhancements for the bottom two rows. My program now assigns all four of the cells in the bottom-left corner as a unit. It does the same for the lower-right corner.

First, the program assigns a number to the cell in the very lower-left corner of the magic square. Since all but the last two rows are filled, the cell above this one will complete a column, so its contents can be calculated rather than permuted. Similarly, the cell above and to the right of the corner cell will complete a diagonal, so its contents can also be calculated. Once this cell is filled, the cell just below it, i.e. the cell just to the right of the corner cell, will complete the second column. For the lower-right corner, the program repeats this process in mirror-image.

This enhancement raised three possibilities for finishing the magic square:

- If N = 3, there is only one column left after assigning the lower-left four squares. Instead of assigning all four lower right squares, the program simply calculates the correct contents for the last two cells and skips or prints accordingly.

- If N = 4, both corners are assigned, but there is nothing more to be assigned after that. Instead the program checks whether the last two rows have the correct sum. If they do, it prints the finished square.

- If N >= 5, there is at least one column in between the corner four-square groups. These columns are assigned using the autoColBasic method applied in the last update.

After completing these corner enhancements, the program can find all 880 4x4 magic squares in about 4 seconds. Results vary by hardware, of course.
