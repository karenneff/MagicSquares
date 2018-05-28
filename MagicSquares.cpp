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
void assign(int, int*, int);
void check(int*);
bool checkFirstDiagonal(int*);

void swap(int* A, int pos1, int pos2) {
	if (pos1 == pos2)return;
	int temp = A[pos2];
	A[pos2] = A[pos1];
	A[pos1] = temp;
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

void autoRow(int x, int*A, int cornersLeft) { //last number in a row
	int sum = 0;
	for (int ii = x - N; ii < x - 1; ii++) 
		sum += A[ii];
	int needed = correctSum - sum;
	int index = -1;
	for (int ii = x - 1; ii < N*N; ii++) {
		if (A[ii] == needed) {
			index = ii;
			break;
		}
	}
	if (index == -1) return;
	if (x == N) {  //upper right corner
		if (A[index] > A[0]) return;
	}
	else if (A[index] < A[0] && cornersLeft == 2)return;
	int newLeft = cornersLeft;
	swap(A, index, x - 1);
	if (A[x - 1] < A[0]) newLeft--;
	assign(x + 1, A, newLeft);
	swap(A, index, x - 1);
}

void autoCol(int x, int*A, int cornersLeft) {
	if (x == N*N) { //complete; accept or reject
		if (A[0] > A[N*N - 1]) check(A); //avoid rotational or reflective repeats
		return;
	}
	int sum = 0;
	for (int ii = x - 1 - N; ii >= 0; ii -= N)
		sum += A[ii];
	int needed = correctSum - sum;
	int index = -1;
	for (int ii = x - 1; ii < N*N; ii++) {
		if (A[ii] == needed) {
			index = ii;
			break;
		}
	}
	if (index == -1) return;

	if (x == N*(N - 1) + 1) {  //lower left corner
		if (A[index] > A[N - 1]) return;
		if (A[index] > A[0]) return;
		swap(A, index, x - 1);
		if (!checkFirstDiagonal(A)) {
			swap(A, index, x - 1);
			return;
		}
		autoCol(x + 1, A, cornersLeft - 1);
		swap(A, index, x - 1);
		return;
	}

	//not a corner
	if (A[index] < A[0] && cornersLeft == 1) return;
	int newLeft = cornersLeft;
	swap(A, index, x - 1);	
	if (A[x - 1] < A[0]) newLeft--;
	autoCol(x + 1, A, newLeft);
	swap(A, index, x - 1);
}

bool checkFirstDiagonal(int* A) {
	int sum = 0;
	int x = N - 1;
	//check first diagonal
	for (int ii = 0; ii < N; ii++)
		sum += A[x + ii * (N - 1)];
	return sum == correctSum;
}

void check(int* A) {
	int sum = 0;
	//check other diagonal
	for (int ii = 0; ii < N; ii++)
		sum += A[ii * (N + 1)];
	if (sum == correctSum) print(A); //found a magic square; print so it can be read
}

void assign(int x, int*A, int cornersLeft) {
	if (x % N == 0) {	//new row has been completed
		autoRow(x, A, cornersLeft); return;
	}
	else if (x > N * (N - 1)) {  //last row
		autoCol(x, A, cornersLeft); return;
	}
	for (int i = x-1; i < N*N; i++) {
		int newLeft = cornersLeft;
		if (A[i] < A[0])newLeft -= 1;
		if (x < N && newLeft < 3) return;
		else if (x < N * (N - 1) && newLeft < 2) return;
		else if (newLeft < 1) return;
		swap(A, x - 1, i); //generate all possible arrangements
		assign(x + 1, A, newLeft);
		swap(A, x - 1, i);
	}
}

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

int main() 
{
	char blah;
	int* A = new int[N * N];
	correctSum = N * (N * N + 1) / 2;
	for (int i = 0; i < N * N; i++)
		A[i] = i + 1; //initialize array with numbers 1 to n2 in order
	outfile.open("MagicSquares.txt");
	assignFirst(A);
	outfile.close();
	cout << "Finished with " << currentcount << " successes.";
	cin >> blah;
	return 0;
}