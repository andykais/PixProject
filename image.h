#include <cassert>
#include <vector>
#include <iostream>

class Color {
public:
	Color(unsigned char red=255, unsigned char green=255, unsigned char blue=255, unsigned char alpha=255) 
		: R(red),G(green),B(blue), A(alpha) {}
	bool isWhite() const {return R==255 && G==255 && B==255;}
	unsigned char R,G,B,A;
};

class Image {
public:
	//image format is RGBA
	Image() : width(0), height(0) {}
  Image(const Image &image) { 
    copy_helper(image); }
  const Image& operator=(const Image &image) { 
    if (this != &image)
      copy_helper(image);
    return *this; }
	void Allocate(int resolution) {
		width = resolution;
	  height =resolution;
	  data.clear();
		data.resize(width*height);
		setAllPixels(Color(255,255,255,255));
	}
	//ACCESSORS
  int Width() const { return width; }
  int Height() const { return height; }
	const Color getPixel(int r, int c) const {
		return data[c*width + r]; }
	std::vector<unsigned char> getOutput() {
		assert(width !=0);
		std::vector<unsigned char> out;
		out.resize(width * height * 4);
		for (int i=0; i<width*height; ++i) {
			int x=i*4;
			out[x]   = data[i].R;
			out[x+1] = data[i].G;
			out[x+2] = data[i].B;
			out[x+3] = data[i].A;
		}
		return out;
	}

	//MODIFIERS
	void setPixel(int r, int c, const Color& value) {
		assert(r >= 0 && r < width);
    assert(c >= 0 && c < height);
    data[c*width + r] = value; }
  void setAllPixels(const Color& value) {
  	for (int i=0; i<width*height; ++i)
  		data[i] = value;
  }
private:
	void copy_helper(const Image &image) {
		width = image.Width();
		height = image.Height();
		data.resize(width*height);
		for (int r=0; r<width; ++r)
		for (int c=0; c<height; ++c) {
			data[c*width + r] = image.getPixel(r,c);
		}
	}
	int width;
	int height;
	std::vector<Color> data;
};