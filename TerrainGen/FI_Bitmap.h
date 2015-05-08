/*----------------------------------------------------------------------------------------*
/*	FI_Bitmap.h
/*----------------------------------------------------------------------------------------*
/*	Forrest Ireland
/*	March 20, 2015
/*----------------------------------------------------------------------------------------*
/*	Header File for the Bitmap24 class. This class provides functionality for loading, 
/*	modifying, and saving 24bit bitmap images.
/*----------------------------------------------------------------------------------------*/

#ifndef FI_BITMAP
#define FI_BITMAP

#include "FI_Graphics.h"

namespace fli {
	class Bitmap24 {
	private:
		int w, h;
	public:
		unsigned char* image;

	public:
		Bitmap24();
		Bitmap24(int _w, int _h);
		Bitmap24(unsigned char r, unsigned char g, unsigned char b, int _w, int _h);
		Bitmap24(glm::vec3& color, int _w, int _h);
		Bitmap24(unsigned char* _image, int _w, int _h);
		Bitmap24(std::string filename);
		~Bitmap24();

		unsigned char& operator* ();
		glm::vec3 operator () (int x, int y);

		void setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);
		void setPixel(int x, int y, const color3& color);
		glm::vec3 getPixel(int x, int y);

		int width() { return w; }
		int height() { return h; }

		bool loadbmp(std::string filename);
		bool savebmp(std::string filename);
	};
}
#endif