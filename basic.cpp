#include <bitset>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

using std::vector;
using std::string;

string convertBin(int& bitSize, int& count) {
	std::bitset<32>		x(count);	
	// x is converted into a string below
	std::string binString = x.to_string<char,std::string::traits_type,std::string::allocator_type>();
	// the second number, the length of the string must be the same as in bitset<32>
	binString = binString.substr(32-bitSize, 32);
	//std::cout << binString << std::endl;
	return binString;
}

vector< vector<int> > getCoordinates(const int& size) {
	// this only works if I only use squares
	int oneSide = int(sqrt (double(size)));
	// uses vectors because they can be returned
	// arrays using pointers may be better because they are faster
	vector<vector<int> > coordinates(2, vector<int>(9, 0));
	for (int i = 0; i<size; i++) {
		coordinates[0][i] = i/oneSide;
		coordinates[1][i] = i%oneSide;
		std::cout << coordinates[0][i] << "  " << coordinates[1][i] << std::endl;
	}
	return coordinates;
}

int main() {
	int size = 511; // 511 is the max combinations for a 3x3 square of pixels
	// this finds the length the binary number should be
	int squareSize = log(size)/log(2) + 1; 
	vector<vector<int> > coordinates(getCoordinates(squareSize));
	std::cout << "working...";
	string binString[size];
	for (int i = 0; i<size; i++) {
		binString[i] = convertBin(squareSize, i);
	}
	std::cout << binString[0];
		// should this be appended to an array?
	return 0;
}