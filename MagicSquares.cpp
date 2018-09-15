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
void autoColBasic(int, int*);
void autoColFinal(int*);
bool checkFirstDiagonal(int*);
bool checkSecondDiagonal(int*);
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
		autoColBasic(x, A);
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
		if (index == -1) continue;
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

//The last column to be assigned.
//Second-to-last cell in the column is calculated as auto-row.
//If successful, last cell need not be checked.
void autoColFinal(int* A){
	if (!checkFirstDiagonal(A)) return;
	if (A[N * (N - 1)] > A[0]) return;
	if (A[N * (N - 1)] > A[N - 1]) return;
	int top = N * (N - 1) - 1;
	int bottom = top + N;
	int sum = 0;
	for (int ii = top - N + 1; ii < top; ii++)
		sum += A[ii];
	int needed = correctSum - sum;
	bool swapNeeded = false;
	if (A[bottom] == needed) {
		swapNeeded = true;
	}
	else if (A[top] != needed) { 
		return; 
	}
	if (swapNeeded)swap(A, top, bottom);
	if (checkSecondDiagonal(A) && A[bottom] < A[0]) print(A);
	if (swapNeeded)swap(A, top, bottom);
}

bool checkFirstDiagonal(int* A) {
	int sum = 0;
	int x = N - 1;
	for (int ii = 0; ii < N; ii++)
		sum += A[x + ii * (N - 1)];
	return sum == correctSum;
}
bool checkSecondDiagonal(int*A) {
	int sum = 0;
	for (int ii = 0; ii < N; ii++)
		sum += A[ii * (N + 1)];
	return sum == correctSum;
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