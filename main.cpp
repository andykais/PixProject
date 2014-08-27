#include <bitset>
#include <math.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include "lodepng.h"
#include "image.h"

using std::vector;
using std::string;

///////////////////////////////////////////////////////////////////////////
//////////////////////////BINARY CALCULATIONS/////////////////////////////
/////////////////////////////////////////////////////////////////////////

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
	vector<vector<int> > coordinates(2, vector<int>(size, 0));
	for (int i = 0; i<size; i++) {
		coordinates[0][i] = i/oneSide;
		coordinates[1][i] = i%oneSide;
		// if (i%oneSide==0 && i!=0)
		// 	std::cout << std::endl;
		// std::cout << '(' << coordinates[0][i] << ',' << coordinates[1][i] << ") ";
	}
	// std::cout << std::endl;
	return coordinates;
}

///////////////////////////////////////////////////////////////////////////
///////////////////////IMAGE ENCODING & EDITING///////////////////////////
/////////////////////////////////////////////////////////////////////////

void saveImage(char* aBin, std::vector<unsigned char>& png, int size) {
	//arranging the name for the file
  std::stringstream ss;
  ss << size;
	std::string length = ss.str();
	ss.str("");
	ss << 'x';
	std::string x = ss.str();
	ss.str("");
	ss << '_';
	std::string dash = ss.str();
  std::string ending = ".png";
  std::string filename = length + x + length + dash + aBin + ending;
  // const char *filename = stringFilename.c_str();
  //Encode the image
  unsigned error = lodepng::encode(filename, png, size, size);
  //if there's an error, display it
  if(error) std::cerr << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;

}

void drawImage(Image& image, const vector<vector<int> >& coordinates, 
	char* aBin) {
	Color black(0,0,0,255);
	Color white(255,255,255,255);
	int squareSize = coordinates[0].size();
	// std::string aBin = binString[index];
	std::cout << aBin << std::endl;
	for (int i=0; i<squareSize; ++i) {
		if (aBin[i] == '1') {
			image.setPixel(coordinates[0][i], coordinates[1][i], black);
		}
		else
			image.setPixel(coordinates[0][i], coordinates[1][i], white);
	}
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////ERROR CHECKING/////////////////////////////////
/////////////////////////////////////////////////////////////////////////

bool error(int argc, char* argv[]) {
	if (argc != 5) {
		std::cerr << "Usage: ./main.exe -index <#> -size <#>" << std::endl;
		std::cerr << "Usage: ./main.exe -percentage <#> -size <#>" << std::endl;
		return true;
	}
	int oneSide = atoi(argv[4]);
	int index = atoi(argv[2]);
	if (argv[1] != std::string("-index") && argv[1] != std::string("-percentage")) {
		std::cout << argv[1];
		std::cerr << "error: invalid command (valid commands: -index)" << std::endl;
		return true;
	}
	if (argv[1] == std::string("-percentage") && atoi(argv[2])>100 || atoi(argv[2])<0)  {
		std::cerr << "error: percentage needs to be between 0 and 100" << std::endl;
		return true;
	}
	int length = 0;
	while(argv[2][length] != 0 && argv[2][length] == '0' || argv[2][length] == '1') {
		length ++;
	}
	if (length != pow(oneSide, 2)) {
		std::cerr << "error: invalid index, needs to be " << pow(oneSide, 2);
		std::cerr << " digits consisted of 1s & 0s" << std::endl;
		return true;
	}
	return false;
}

int main(int argc, char* argv[]) {
	if (error(argc, argv)) return 1;
	char* index = argv[2];
	int oneSide = atoi(argv[4]);
	// this finds the length the binary number should be (number of pixels)
	int squareSize = pow(oneSide, 2); //==other calc log(size)/log(2) + 1;

	double size = pow(2, oneSide*oneSide) -1; // 511 is the max combinations for a 3x3 square of pixels
	// if (argv[1]==std::string("-percentage")) {
	// 	index = size*(index/100.0);
	// 	std::cout << index << std::endl;
	// }

	Image image;
	image.Allocate(oneSide);

	
	vector<vector<int> > coordinates(getCoordinates(squareSize));
	//only needed for all images solution
	// string binString[size];
	// for (int i = 0; i<size; i++) {
	// 	binString[i] = convertBin(squareSize, i);
	// }
	//cant use with large values (bigger than 10)
	// std::string aBin = convertBin(squareSize, index);
	// right now this only does one of the combinations for a size
	drawImage(image, coordinates, index);
	std::vector<unsigned char> png = image.getOutput();
	saveImage(index, png, oneSide);


	std::cout << "finished." << std::endl;
	return 0;
}

// notes:
// 2^(number of pixels) = total number of images