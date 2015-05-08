#include "FI_Graphics.h"
#include "FI_Bitmap.h"
#include <glm\gtx\vector_angle.hpp>

//#define MD
#define DS
//#define SAVETOTXT
#define SAVETOBMP

float p_factor = 0.2f;
int seed_range = 20;
int seed_min = -10;
int divisions = 8;
int mapsize = pow(2, divisions); // Measured in squares/tiles
unsigned int max_height = 255;

V_vec3 vertices;
V_vec2 uv_coords;
V_vec3 faces;
VVF hmap;

enum rel_pos { center, top, bottom, left, right };

void CalculateNormals() {

}

void WriteHmapToOBJFile(std::string filename) {
	CalculateNormals();

	std::ofstream obj;
	obj.open(filename);
	// write to obj file here
	obj.close();
}


float get_midpoint(float distance, float average) {
	int direction = (int)(2 * (rand() % 2 - 0.5));
	float random = rand() % RAND_MAX / (float)RAND_MAX;
	float p = direction * distance * random * p_factor;
	return p + average;
}

float get_center_midpoint(const point3& A, const point3& B, float height) {
	float dx = A.x - B.x;
	float dy = A.y - B.y;
	float distance = sqrt(dx*dx + dy*dy);
	int direction = (int)(2 * (rand() % 2 - 0.5));
	float random = rand() % RAND_MAX / (float)RAND_MAX;
	float p = direction * distance * random * p_factor + height;
	return p * height;
}

void diamond_square(VVF& map, int mapsize, int divs) {
	// Seed the corners of the map
	map[0][0] = rand() % seed_range + seed_min;
	map[0][mapsize] = rand() % seed_range + seed_min;
	map[mapsize][0] = rand() % seed_range + seed_min;
	map[mapsize][mapsize] = rand() % seed_range + seed_min;

	for (int i = 0; i < divs; i++) {
		int x = (mapsize) / (int)pow(2, i);
		int d = (x + 1) / 2;
		Print(x << ", " << d);

		// Square Step, looks at squares and creates midpoints fo those squares, makes diamonds
		for (int j = 0; j < mapsize / x; j++) {
			for (int k = 0; k < mapsize / x; k++) {
				map[j * x + d][k * x + d] = get_midpoint((float)x * (float)sqrt(2), (map[j * x][k * x] + map[j * x][k * x + x] + map[j * x + x][k * x] + map[j * x + x][k * x + x]) / 4.0f);
				//				PrintVec3(map[j * x + d][k * x + d]);
			}
		}
		// Diamond Step, looks at diamonds and generates midpoints, makes squares
		// Horizontal Diamonds first
		for (int j = 0; j < (mapsize / x) + 1; j++) {
			for (int k = 0; k < mapsize / x; k++) {

				if (j * x - d < 0) {
					map[j * x][k * x + d] = get_midpoint((float)x * 0.5f, (map[j * x][k * x] + map[j * x][k * x + x] + map[j * x + d][k * x + d]) / 3.0f);
				}
				else if (j * x + d > mapsize) {
					map[j * x][k * x + d] = get_midpoint((float)x * 0.5f, (map[j * x][k * x] + map[j * x][k * x + x] + map[j * x - d][k * x + d]) / 3.0f);
				}
				else {
					map[j * x][k * x + d] = get_midpoint((float)x * 0.5f, (map[j * x][k * x] + map[j * x - d][k * x + d] + map[j * x + d][k * x + d] + map[j * x][k * x + x]) / 4.0f);
				}
				//PrintVec3(map[j * x][k * x + d]);
			}
		}

		for (int j = 0; j < mapsize / x; j++) {
			for (int k = 0; k < (mapsize / x) + 1; k++) {

				if (k * x - d < 0) {
					map[j * x + d][k * x] = get_midpoint((float)x * 0.5f, (map[j * x][k * x] + map[j * x + x][k * x] + map[j * x + d][k * x + d]) / 3.0f);
				}
				else if (k * x + d > mapsize) {
					map[j * x + d][k * x] = get_midpoint((float)x * 0.5f, (map[j * x][k * x] + map[j * x + x][k * x] + map[j * x + d][k * x - d]) / 3.0f);
				}
				else {
					map[j * x + d][k * x] = get_midpoint((float)x * 0.5f, (map[j * x][k * x] + map[j * x + d][k * x + d] + map[j * x + x][k * x] + map[j * x + d][k * x + d]) / 4.0f);
				}
				// PrintVec3(map[j * x + d][k * x]);
			}
		}
	}
	return;
}

//void midpoint_displacement(const point3& ll, const point3& lr, const point3& ul, const point3& ur) {
//	if ((lr.x - ll.x) > 1) {
//		point3 midpts[5];
//
//		midpts[center].x = (float)((int)(ur.x + ll.x) / 2);
//		midpts[center].y = (float)((int)(ur.y + ll.y) / 2);
//		midpts[center].z = get_center_midpoint(ur, ll, (ur.z + ll.z + ul.z + lr.z) / 4.0f);
////		std::cout << "Center: " << PrintVec3(midpts[center]) << std::endl;
//
//		midpts[top].x = midpts[center].x;
//		midpts[top].y = ul.y;
//		midpts[top].z = get_midpoint(ul.x - ur.x, (ul.z + ur.z) / 2.0f);
////		std::cout << "top: " << PrintVec3(midpts[top]) << std::endl;
//		midpts[bottom].x = midpts[center].x;
//		midpts[bottom].y = ll.y;
//		midpts[bottom].z = get_midpoint(ll.x - lr.x, (ll.z + lr.z) / 2.0f);
////		std::cout << "bottom: " << PrintVec3(midpts[bottom]) << std::endl;
//		midpts[left].x = ll.x;
//		midpts[left].y = midpts[center].y;
//		midpts[left].z = get_midpoint(ll.x - ul.x, (ul.z + ll.z) / 2.0f);
////		std::cout << "left: " << PrintVec3(midpts[left]) << std::endl;
//		midpts[right].x = lr.x;
//		midpts[right].y = midpts[center].y;
//		midpts[right].z = get_midpoint(lr.x - ur.x, (ur.z + lr.z) / 2.0f);
////		std::cout << "right: " << PrintVec3(midpts[right]) << std::endl;
//
//		for (int i = 0; i < 5; i++) {
//			if (hmap[(int)midpts[i].x][(int)midpts[i].y].z == -1.0f) {
//				hmap[(int)midpts[i].x][(int)midpts[i].y] = midpts[i];
//			}
//			else {
//				midpts[i] = hmap[(int)midpts[i].x][(int)midpts[i].y];
//			}
//		}
//
//		midpoint_displacement(ll, hmap[(int)midpts[bottom].x][(int)midpts[bottom].y], hmap[(int)midpts[left].x][(int)midpts[left].y], hmap[(int)midpts[center].x][(int)midpts[center].y]);
//		midpoint_displacement(hmap[(int)midpts[bottom].x][(int)midpts[bottom].y], lr, hmap[(int)midpts[center].x][(int)midpts[center].y], hmap[(int)midpts[right].x][(int)midpts[right].y]);
//		midpoint_displacement(hmap[(int)midpts[left].x][(int)midpts[left].y], hmap[(int)midpts[center].x][(int)midpts[center].y], ul, hmap[(int)midpts[top].x][(int)midpts[top].y]);
//		midpoint_displacement(hmap[(int)midpts[center].x][(int)midpts[center].y], hmap[(int)midpts[right].x][(int)midpts[right].y], hmap[(int)midpts[top].x][(int)midpts[top].y], ur);
//	}
//	return;
//}

void normalize_map(VVF& map) {
	float min = 10000;
	for (unsigned int i = 0; i < map.size(); i++) {
		for (unsigned int j = 0; j < map[i].size(); j++) {
			if (map[i][j] < min)
				min = map[i][j];
		}
	}

	min = 0 - min;

	for (unsigned int i = 0; i < map.size(); i++) {
		for (unsigned int j = 0; j < map[i].size(); j++) {
			map[i][j] += min;
		}
	}

	float max = 0;
	for (unsigned int i = 0; i < map.size(); i++) {
		for (unsigned int j = 0; j < map[i].size(); j++) {
			if (map[i][j] > max)
				max = map[i][j];
		}
	}

	for (unsigned int i = 0; i < map.size(); i++) {
		for (unsigned int j = 0; j < map[i].size(); j++) {
			map[i][j] /= max;
		}
	}
}

void normalize_map_to_max_height(VVF& map) {
	float min = 10000;
	for (unsigned int i = 0; i < map.size(); i++) {
		for (unsigned int j = 0; j < map[i].size(); j++) {
			if (map[i][j] < min)
				min = map[i][j];
		}
	}

	min = 0 - min;

	for (unsigned int i = 0; i < map.size(); i++) {
		for (unsigned int j = 0; j < map[i].size(); j++) {
			map[i][j] += min;
		}
	}

	for (unsigned int i = 0; i < map.size(); i++) {
		for (unsigned int j = 0; j < map[i].size(); j++) {
			if (map[i][j] > max_height)
				map[i][j] = (float)max_height;
		}
	}

	for (unsigned int i = 0; i < map.size(); i++) {
		for (unsigned int j = 0; j < map[i].size(); j++) {
			map[i][j] /= (float)max_height;
		}
	}
}

int main() {
	srand((unsigned int)time(NULL));


	hmap.resize(mapsize + 1);
	for (unsigned int i = 0; i < hmap.size(); i++) {
		hmap[i].resize(mapsize + 1);
	}

	for (unsigned int i = 0; i < hmap.size(); i++) {
		for (unsigned int j = 0; j < hmap[i].size(); j++) {
			hmap[i][j] = -10000.0f;
		}
	}

#ifdef MD

	hmap[0][0] = point3(0, 0, rand() % max_seed);
	hmap[0][mapsize] = point3(0, mapsize, rand() % max_seed);
	hmap[mapsize][0] = point3(mapsize, 0, rand() % max_seed);
	hmap[mapsize][mapsize] = point3(mapsize, mapsize, rand() % max_seed);

	midpoint_displacement(hmap[0][0], hmap[mapsize][0], hmap[0][mapsize], hmap[mapsize][mapsize]);
#endif

#ifdef DS
	diamond_square(hmap, mapsize, divisions);

#endif
	WriteHmapToOBJFile("hmap.obj");
	normalize_map(hmap);
	//normalize_map_to_max_height(hmap);

	for (unsigned int i = 0; i < hmap.size(); i++) {
		for (unsigned int j = 0; j < hmap[i].size(); j++) {
			hmap[i][j] *= max_height + 1;
		}
	}

#ifdef SAVETOTXT
	std::ofstream file;
	file.open("map.txt");
	file.precision(3);

	for (unsigned int i = 0; i < hmap.size(); i++) {
		for (unsigned int j = 0; j < hmap[i].size(); j++) {
			file << std::fixed << std::setw(8) << hmap[i][j] << " ";
		}
		file << std::endl;
	}

	file.close();
#endif

#ifdef SAVETOBMP

	fli::Bitmap24 bmp(mapsize + 1, mapsize + 1);
	for (unsigned int i = 0; i < hmap.size(); i++) {
		for (unsigned int j = 0; j < hmap[i].size(); j++) {
			int h = (int)hmap[i][j];
			//Print(h);
			char rgb[3];
			rgb[2] = (h >> 16) & 0xFF;
			rgb[1] = (h >> 8) & 0xFF;
			rgb[0] = h & 0xFF;
			bmp.setPixel(i, j, rgb[0], rgb[1], rgb[2]);
		}
	}

#ifdef MD
	time_t now;
	char filename[26];

	filename[0] = '\0';

	now = time(NULL);

	if (now != -1)
	{
		strftime(filename, 26, "%Y.%m.%d %H%M%S MD.bmp", gmtime(&now));
	}
	Print(filename);
	bmp.savebmp(filename);
#endif

#ifdef DS
	time_t now;
	char filename[26];

	filename[0] = '\0';

	now = time(NULL);

	if (now != -1)
	{
		strftime(filename, 26, "%Y.%m.%d %H%M%S DS.bmp", gmtime(&now));
	}
	Print(filename);
	bmp.savebmp(filename);
#endif

#endif
	Print(sizeof(glm::vec3));
	return 0;
}
