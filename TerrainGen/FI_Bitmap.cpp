/*----------------------------------------------------------------------------------------*
/*	FI_Bitmap.cpp
/*----------------------------------------------------------------------------------------*
/*	Forrest Ireland
/*	March 20, 2015
/*----------------------------------------------------------------------------------------*
/*	Source File for the Bitmap24 class. This class provides functionality for loading,
/*	modifying, and saving 24bit bitmap images.
/*----------------------------------------------------------------------------------------*/


#include "FI_Bitmap.h"

namespace fli {

/*----------------------------------------------------------------------------------------*
/*	Constructors
/*----------------------------------------------------------------------------------------*/
	Bitmap24::Bitmap24() {
		image = NULL;
	}

	Bitmap24::Bitmap24(int _w, int _h) : w(_w), h(_h){
		image = NULL;

		int c = 3 * w * h;
		image = new unsigned char[c];
		for (int i = 0; i < c; i++) {
			image[i] = 1;
		}
	}

	Bitmap24::Bitmap24(unsigned char r, unsigned char g, unsigned char b, int _w, int _h) : w(_w), h(_h) {
		image = NULL;
		int c = 3 * w * h;
		image = new unsigned char[c];
		for (int i = 0; i < c;) {
			image[i++] = r;
			image[i++] = g;
			image[i++] = b;
		}
	}

	Bitmap24::Bitmap24(unsigned char* _image, int _w, int _h) : w(_w), h(_h) {
		// TODO
	}

	Bitmap24::Bitmap24(glm::vec3& color, int _w, int _h) : w(_w), h(_h) {
		image = NULL;
		int c = 3 * w * h;
		image = new unsigned char[c];
		for (int i = 0; i < c;) {
			image[i++] = (unsigned char)(color.r * 255);
			image[i++] = (unsigned char)(color.g * 255);
			image[i++] = (unsigned char)(color.b * 255);
		}
	}

	Bitmap24::Bitmap24(std::string filename) {
		image = NULL;
        loadbmp(filename);
	}

	Bitmap24::~Bitmap24() {
		if (image)
			delete[] image;
	}

	/*----------------------------------------------------------------------------------------*
	/*	Operator Overloads
	/*----------------------------------------------------------------------------------------*/
	unsigned char& Bitmap24::operator* () {
		return *image;
	}

	glm::vec3 Bitmap24::operator () (int x, int y) {
		int index = 3 * (y * w + x);
		return glm::vec3(image[index + 0] / 255, image[index + 1] / 255, image[index + 2] / 255);
	}

	/*----------------------------------------------------------------------------------------*
	/*	Get/Set Functions
	/*----------------------------------------------------------------------------------------*/
	void Bitmap24::setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
		int index = 3 * (y * w + x);

		image[index + 0] = r;
		image[index + 1] = g;
		image[index + 2] = b;
	}

	void Bitmap24::setPixel(int x, int y, const color3& color) {
		int index = 3 * (y * w + x);

		image[index + 0] = (int)(color.r * 255);
		image[index + 1] = (int)(color.g * 255);
		image[index + 2] = (int)(color.b * 255);
	}

	glm::vec3 Bitmap24::getPixel(int x, int y) {
		int index = 3 * (y * w + x);
		return glm::vec3(image[index + 0] / 255, image[index + 1] / 255, image[index + 2] / 255);
	}

	/*----------------------------------------------------------------------------------------*
	/*	Load/Save Bitmap24
	/*----------------------------------------------------------------------------------------*/
	bool Bitmap24::loadbmp(std::string filename) {
		unsigned char bmpfileheader[14];
		unsigned char bmpinfoheader[40];
		memset(bmpfileheader, 0, sizeof(bmpfileheader));
		memset(bmpinfoheader, 0, sizeof(bmpinfoheader));

		FILE* f = NULL;
		f = fopen(filename.c_str(), "r");
		if (!f) {
#ifdef VERBOSE_2
			std::cout << "Error: Unable to open " << filename << "!" << std::endl;
#endif
			return false;
		}

		if (f) {
			fread(bmpfileheader, 1, 14, f);
			// Checks that the file is actually a Bitmap24 with leading chars 'BM'
			if (bmpfileheader[0] != 'B' || bmpfileheader[1] != 'M') {
#ifdef VERBOSE_2
				std::cout << "Error: Cannot open \"" << filename << "\"!" << std::endl;
#endif
				return false;
			}

#ifdef _WIN32
            
            fread(bmpinfoheader, 1, 40, f);
            w = 0;
            for (int i = 0; i < 4; i++) {
                w += (bmpinfoheader[4 + i]);
            }
            h = 0;
			for (int i = 0; i < 4; i++) {
				h += (bmpinfoheader[8 + i]);
			}
#endif
            
#ifdef __APPLE__
			fread(bmpinfoheader, 1, 40, f);
			w = 0;
			for (int i = 0; i < 4; i++) {
				w += (bmpinfoheader[7 - i]) << (24 - (i * 8));
			}
			h = 0;
			for (int i = 0; i < 4; i++) {
				h += (bmpinfoheader[11 - i]) << (24 - (i * 8));
			}
#endif
			image = new unsigned char[w * h * 3*sizeof(unsigned char)];
			fread(image, 1, 3 * w * h, f);
		}
		return true;
	}

	bool Bitmap24::savebmp(std::string filename) {
		FILE* f = NULL;
		unsigned char *img = NULL;
		int filesize = 54 + 3 * w * h;  //w is your image width, h is image height, both int
		if (img)
			free(img);
		img = (unsigned char *)malloc(3 * w*h);
		strncpy((char*)img, (char*)image, 3 * w * h);

		int x, y;

		for (int i = 0; i < w; i++)
		{
			for (int j = 0; j<h; j++)
			{
				x = i;
				y = (h - 1) - j;

				img[(x + y*w) * 3 + 2] = image[(x + y*w) * 3 + 0];
				img[(x + y*w) * 3 + 1] = image[(x + y*w) * 3 + 1];
				img[(x + y*w) * 3 + 0] = image[(x + y*w) * 3 + 2];
			}
		}

		unsigned char bmpfileheader[14] = { 'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0 };
		unsigned char bmpinfoheader[40] = { 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0 };
		unsigned char bmppad[3] = { 0, 0, 0 };

		bmpfileheader[2] = (unsigned char)(filesize);
		bmpfileheader[3] = (unsigned char)(filesize >> 8);
		bmpfileheader[4] = (unsigned char)(filesize >> 16);
		bmpfileheader[5] = (unsigned char)(filesize >> 24);

		bmpinfoheader[4] = (unsigned char)(w);
		bmpinfoheader[5] = (unsigned char)(w >> 8);
		bmpinfoheader[6] = (unsigned char)(w >> 16);
		bmpinfoheader[7] = (unsigned char)(w >> 24);
		bmpinfoheader[8] = (unsigned char)(h);
		bmpinfoheader[9] = (unsigned char)(h >> 8);
		bmpinfoheader[10] = (unsigned char)(h >> 16);
		bmpinfoheader[11] = (unsigned char)(h >> 24);

		f = fopen(filename.c_str(), "wb");
		if (f) {
			fwrite(bmpfileheader, 1, 14, f);
			fwrite(bmpinfoheader, 1, 40, f);
			for (int i = 0; i < h; i++)
			{
				fwrite(img + (w*(h - i - 1) * 3), 3, w, f);
				fwrite(bmppad, 1, (4 - (w * 3) % 4) % 4, f);
			}
			fclose(f);
			return true;
		}
		return false;
	}
}