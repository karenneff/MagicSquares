//Name: Karen Neff
//Class: Algorithms
//Date: 10/17/17
//Title: MagicSquares.cpp
//Description: Program to find all magic squares
//of size nxn, not counting rotations or reflections.
//Note: This program requires C++11 or higher.


#define N 4 //problem size
#include <iostream>
#include <fstream>
#include <thread>

using namespace std;

int correctSum;
ofstream outfile;
int currentcount = 0;
bool printMutex = false;

void assignFirst(int*);
void assign(int, int*, int);
void assignBasic(int, int*, int);
void autoRow(int, int*, int);
void lowerLeftCornerA(int*, int);
void lowerLeftCornerB(int*, int);
void lowerLeftCornerC(int*, int);
void lowerLeftCornerD(int*, int);
void lowerRightCornerA(int*);
void lowerRightCornerB(int*);
void lowerRightCornerC(int*);
void lowerRightCornerD(int*);
void autoColBasic(int, int*);
void finalizeSize3(int*);
void finalizeSize4(int*);
void autoColFinal(int*);
void swap(int*, int, int);
int find(int, int*, int);
void print(int* A);

int main() 
{
	char blah;
	int* A = new int[N * N];
	correctSum = N * (N * N + 1) / 2;
	for (int i = 0; i < N * N; i++)
		A[i] = i + 1; //initialize array with numbers 1 to n2 in order
	outfile.open("MagicSquares.txt");
	assignFirst(A); //find all possible magic squares
	outfile.close();
	cout << "Finished with " << currentcount << " successes.";
	cin >> blah;
	return 0;
}

//Assign different numbers to the top-left corner, 
//and generate a new thread for each one
void assignFirst(int*A) {
	thread** series = new thread*[N * N];
	for (int ii = 4; ii <= N * N; ii++) //start at 4; avoid rotational repeats
	{
		int* B = new int[N * N];
		for (int j = 0; j < N*N; j++)
			B[j] = A[j]; //copy array so it can be reused
		swap(B, 0, ii - 1); //generate all possible arrangements
		int lessThanFirst = B[0] - 1;
		series[ii - 1] = new thread(assign, 2, B, lessThanFirst);
	}
	for (int ii = 4; ii <= N * N; ii++) //start at 4; avoid rotational repeats
	{
		series[ii - 1]->join();
	}
}

//figure out which assign function to use
void assign(int x, int*A, int cornersLeft) {
	if (x > N * (N - 2))  //second-to-last row
		lowerLeftCornerA(A, cornersLeft);
	else if (x % N == 0)	//new row has been completed
		autoRow(x, A, cornersLeft);
	else 
		assignBasic(x, A, cornersLeft);
}

//Early in the process; next cell could have any remaining number
void assignBasic(int x, int*A, int cornersLeft) {
	for (int i = x - 1; i < N*N; i++) {
		int newLeft = cornersLeft;
		if (A[i] < A[0]) {
			newLeft--;
		}
		if (x < N && newLeft < 3) continue;
		else if (newLeft < 2) continue;
		swap(A, x - 1, i); //generate all possible arrangements
		assign(x + 1, A, newLeft);
		swap(A, x - 1, i);
	}
}

//Last number in a row; can be calculated based on what is there already
void autoRow(int x, int*A, int cornersLeft) { 
	int sum = 0;
	for (int ii = x - N; ii < x - 1; ii++)
		sum += A[ii];
	int needed = correctSum - sum;
	if (x == N) {  //upper right corner
		if (needed > A[0]) return;
	}
	else if (needed < A[0] && cornersLeft == 2)return;
	int index = find(x, A, needed);
	if (index == -1) return;
	int newLeft = cornersLeft;
	swap(A, index, x - 1);
	if (A[x - 1] < A[0]) newLeft--;
	assign(x + 1, A, newLeft);
	swap(A, index, x - 1);
}

//Actual lower-left corner
void lowerLeftCornerA(int*A, int cornersLeft) {
	for (int ii = N * (N - 2); ii < N * N; ii++) {
		if (A[ii] > A[0] || A[ii] > A[N - 1]) continue;
		swap(A, ii, N * (N - 1));
		lowerLeftCornerB(A, cornersLeft - 1);
		swap(A, ii, N * (N - 1));
	}
}
//Square just above lower-left corner; calculate as autoCol
void lowerLeftCornerB(int*A, int cornersLeft) {
	int sum = 0;
	for (int ii = 0; ii < N * (N - 2); ii += N)
		sum += A[ii];
	sum += A[N * (N - 1)];
	int needed = correctSum - sum;
	int newLeft = cornersLeft;
	if (needed < A[0]) newLeft--;
	if (newLeft <= 0)return;
	int index = find(N * (N - 2) + 1, A, needed);
	if (index == -1 || index == N * (N - 1)) return;
	swap(A, index, N * (N - 2));
	lowerLeftCornerC(A, newLeft);
	swap(A, index, N * (N - 2));
}
//Square above-right of lower-left corner; calculate based on diagonal
void lowerLeftCornerC(int*A, int cornersLeft) {
	int sum = 0;
	for (int ii = N - 1; ii < N * (N - 2); ii += (N - 1))
		sum += A[ii];
	sum += A[N * (N - 1)];
	int needed = correctSum - sum;
	int newLeft = cornersLeft;
	if (needed < A[0]) newLeft--;
	if (newLeft <= 0)return;
	int index = find(N * (N - 2) + 2, A, needed);
	if (index == -1 || index % N == 0) return;
	swap(A, index, N * (N - 2) + 1);
	lowerLeftCornerD(A, newLeft);
	swap(A, index, N * (N - 2) + 1);
}
//Square just to the right of lower-left corner; calculate as autoCol
void lowerLeftCornerD(int*A, int cornersLeft) {
	int sum = 0;
	for (int ii = 1; ii < N * (N - 1); ii += N)
		sum += A[ii];
	int needed = correctSum - sum;
	int newLeft = cornersLeft;
	if (needed < A[0]) newLeft--;
	if (newLeft <= 0)return;
	int index = find(N * (N - 2) + 3, A, needed);
	if (index == -1 || index % N == 0) return;
	swap(A, index, N * (N - 1) + 1);
	if (N == 3)
		finalizeSize3(A);
	else
		lowerRightCornerA(A);
	swap(A, index, N * (N - 1) + 1);
}

//Actual lower-right corner
void lowerRightCornerA(int*A) {
	for (int ii = N * (N - 2) + 2; ii < N * N; ii++) {
		if (ii % N < 2) continue; //skip lower-left corner area
		if (A[ii] > A[0]) continue;
		swap(A, ii, N * N - 1);
		lowerRightCornerB(A);
		swap(A, ii, N * N - 1);
	}
}
//Square just above lower-right corner; calculate as autoCol
void lowerRightCornerB(int*A) {
	int sum = 0;
	for (int ii = N - 1; ii < N * (N - 2); ii += N)
		sum += A[ii];
	sum += A[N * N - 1];
	int needed = correctSum - sum;
	int index = find(N * (N - 2) + 3, A, needed);
	if (index == -1 || index == N * N - 1) return;
	if (index % N < 2) return;
	swap(A, index, N * N - N - 1);
	lowerRightCornerC(A);
	swap(A, index, N * N - N - 1);
}
//Square above-left of lower-right corner; calculate based on diagonal
void lowerRightCornerC(int*A) {
	int sum = 0;
	for (int ii = 0; ii < N * (N - 2); ii += (N + 1))
		sum += A[ii];
	sum += A[N * N - 1];
	int needed = correctSum - sum;
	int index = find(N * (N - 2) + 3, A, needed);
	if (index % N < 2 || index % N == N - 1) return; //includes index == -1
	swap(A, index, N * N - N - 2);
	if (N == 4)
		finalizeSize4(A);
	else
		lowerRightCornerD(A);
	swap(A, index, N * N - N - 2);
}
//Square just to the left of lower-right corner; calculate as autoCol
void lowerRightCornerD(int*A) {
	int sum = 0;
	for (int ii = 1; ii < N * (N - 1); ii += N)
		sum += A[ii];
	int needed = correctSum - sum;
	int index = find(N * (N - 2) + 3, A, needed);
	if (index % N < 2 || index % N == N - 1) return; //includes index == -1
	if (index == N * N - N - 2) return;
	swap(A, index, N * N - 2);
	autoColBasic(N * (N - 2) + 3, A);
	swap(A, index, N * N - 2);
}

//Second-to-last row; choose a number for this cell,
//then calculate what must go in the cell below it
void autoColBasic(int x, int*A) {
	//the very last column
	if (x == N * (N - 1)) {
		autoColFinal(A);
		return;
	}
	int top = x - 1;
	int bottom = top + N;
	//first, the second-to-last square in the column...
	for (int i = top; i < N*N; i++) {
		while(i % N < top % N) {
			i++; 
		}
		int upperSquare = A[i];
		//...then, the last square in the column
		int sum = 0;
		for (int iter = top % N; iter < top; iter += N)
			sum += A[iter];
		sum += upperSquare;
		int needed = correctSum - sum;
		if (needed == upperSquare) continue;
		int index = find(x, A, needed);
		if (index % N >= N - 2) continue; //includes index == -1
		if (index % N < top % N) continue;
		//all conditions met; do the swap
		swap(A, top, i);
		if (index != top) swap(A, bottom, index);
		else swap(A, bottom, i);
		autoColBasic(x + 1, A);
		if (index != top) swap(A, bottom, index);
		else swap(A, bottom, i);
		swap(A, top, i);
	}
}

//If the problem size is 3, the lower right corner is treated differently.
void finalizeSize3(int* A) {
	int sum1 = A[0] + A[4];
	int sum2 = A[6] + A[7];
	if (sum1 != sum2) return;
	int needed = correctSum - sum1;
	if (needed > A[0]) return;
	if (A[8] == needed)
		print(A);
	else if (A[5] == needed) {
		swap(A, 5, 8);
		print(A);
		swap(A, 5, 8);
	}
}

//If the problem size is 4, the lower right corner is treated still differently.
void finalizeSize4(int* A) {
	int sum = 0;
	for (int ii = 8; ii < 12; ii++)
		sum += A[ii];
	if (sum == correctSum)
		print(A);
}

//The last column to be assigned.
//Second-to-last cell in the column is calculated as auto-row.
//If successful, last cell need not be checked.
void autoColFinal(int* A){
	int top = N * (N - 1) - 3;
	int bottom = top + N;
	int sum = 0;
	for (int ii = top - N + 3; ii < top; ii++)
		sum += A[ii];
	sum = sum + A[top + 1] + A[top + 2];
	int needed = correctSum - sum;
	bool swapNeeded = false;
	if (A[bottom] == needed) {
		swapNeeded = true;
	}
	else if (A[top] != needed) { 
		return; 
	}
	if (swapNeeded)swap(A, top, bottom);
	print(A);
	if (swapNeeded)swap(A, top, bottom);
}

void swap(int* A, int pos1, int pos2) {
	if (pos1 == pos2)return;
	int temp = A[pos2];
	A[pos2] = A[pos1];
	A[pos1] = temp;
}

int find(int x, int* A, int needed) {
	for (int ii = x - 1; ii < N*N; ii++) {
		if (A[ii] == needed) return ii;
	}
	return -1;
}

void print(int* A)
{
	while (printMutex);
	printMutex = 1;
	for (int ii = 0; ii < N * N; ii++)
		outfile << A[ii] << " ";
	outfile << endl;
	printMutex = 0;
	cout << "*";
	currentcount++;
}