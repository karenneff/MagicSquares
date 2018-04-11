# MagicSquares

This is a program that I wrote as an assignment for Algorithms class last semester. It finds all magic squares of size n, which is defined at the start of the program.

A magic square, as defined for this project, is an nxn matrix that contains the numbers 1 through n<sup>2</sup> arranged such that the numbers in each row, column, and main diagonal have the same sum. The smallest magic square is 3 by 3 and contains the numbers 1 through 9, as follows:

8 1 6 <br>
3 5 7 <br>
4 9 2

This problem is difficult to solve; its time complexity is O((n<sup>2</sup>)!). I've set the problem size to 4 here, and on my personal laptop, which has two cores, the program runs for about one minute. You can set the problem size to 3 to get the answer shown above. If you set it to 5, the program may not find all the magic squares in this lifetime, but it should find at least one of them within eight hours. (Results vary by hardware, of course.)

My program contains the following optimizations to cut down on the execution time:

- The program does not count rotations or reflections of the same magic square. To enforce this, it checks to make sure that the upper left corner cell contains a larger number than any of the other corner cells, and that the upper right cell has a larger number than the lower left cell.
- As it assigns numbers, the program keeps track of how many valid corner numbers are left. For instance, if the upper left corner contains a 5, valid corner numbers are 1, 2, 3, and 4. If at any time it discovers that the number of valid corner numbers is the same as the number of corners left to assign, it will not assign any more corner numbers to non-corner cells.
- After assigning the second-to-last number in each row, the program will compute the last number for the row and assign it to the last cell rather than trying each available number. It does the same for each column. If the correct number is unavailable, the program will immediately stop pursuing the entire current branch of permutations. 
For instance, for a 4x4 magic square, the program may assign the numbers 16, 15, and 14 to the first three cells. To add to the correct sum, the last number in the row would have to be -11, which is not available, so the program immediately tries a different number for the third cell.
- The program creates several threads to perform its calculations; this speeds up the program on a multicore machine. Note that this program uses the C++ &lt;thread&gt; </pre> library, so C++11 or higher is required.
  
Results are printed to a text file; an example is posted. Each line represents one square; the first row from left to right is listed first, followed by the second row and so on. Yours may vary due to different execution time of different threads, but it should contain all the same results in some order.

This program and its documentation are available as examples of my work and are intended to be used for informational purposes only. Please do not copy or use this code for any other purpose without my permission.
