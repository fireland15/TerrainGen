/*----------------------------------------------------------------------------------------*
/*	FI_Graphics.h
/*----------------------------------------------------------------------------------------*
/*	Forrest Ireland
/*	April 6, 2015
/*----------------------------------------------------------------------------------------*
/*	The major include header for access to the FI_Graphics library
/*----------------------------------------------------------------------------------------*/

#pragma once

#ifndef FI_GRAPHICS
#define FI_GRAPHICS

#define _CRT_SECURE_NO_WARNINGS // Disable warnings for using fopen instead of fopen_s

/*----------------------------------------------------------------------------------------*
/*	Library Includes 
/*----------------------------------------------------------------------------------------*/

#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <time.h>
#include <cassert>

#include <GL\glew.h>
#include <GL\glut.h>
#include <GL\GL.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>
//#include <glm\gtc\matrix_projection.hpp>

/*----------------------------------------------------------------------------------------*
/*	Library Definitions
/*----------------------------------------------------------------------------------------*/

#define FI_EPSILON			0.00001
#define DEG2RAD(x)			x * (3.141592f/180.0f)
#define TARGET_FRAMERATE	30.0f
#define DELTA_TIME			1.0f / TARGET_FRAMERATE

#define VERBOSE_1
#define VERBOSE_2

#define SHADER_PATH "FI_Graphics\\shaders\\"
#define MODEL_PATH	"FI_Graphics\\models\\"
#define TEX_PATH	"FI_Graphics\\textures\\"
#define SCRIPT_PATH "FI_Graphics\\scripts\\"

#define Print(x)		std::cout << x << std::endl
#define PrintVec3(v)	std::cout << std::setprecision(3) << "<" << v.x << ", " << v.y << ", " << v.z << ">" << std::endl

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

#define GL_CHECK_ERRORS \
{ \
	int err = glGetError(); \
	if (err != 0) \
		{   std::cout << "OpenGL Error: " << err << "\n\tLine: " << __LINE__ << "\n\tFile: " << __FILE__ << std::endl; } \
}

/*----------------------------------------------------------------------------------------*
/*	Library typedefs
/*----------------------------------------------------------------------------------------*/

typedef std::vector<char> VC;
typedef std::vector<int> VI;
typedef std::vector<float> VF;
typedef std::vector<glm::vec2> V_vec2;
typedef std::vector<glm::vec3> V_vec3;
typedef std::vector<glm::vec4> V_vec4;

typedef std::vector<VC> VVC;
typedef std::vector<VI> VVI;
typedef std::vector<VF> VVF;
typedef std::vector<V_vec2> VV_vec2;
typedef std::vector<V_vec3> VV_vec3;
typedef std::vector<V_vec4> VV_vec4;

typedef glm::vec2 point2;
typedef glm::vec2 color2;
//typedef glm::vec2 vec2;

typedef glm::vec3 point3;
typedef glm::vec3 color3;
//typedef glm::vec3 vec3;

typedef glm::vec4 point4;
typedef glm::vec4 color4;
//typedef glm::vec4 vec4;

/*----------------------------------------------------------------------------------------*
/*	Shader Compiler
/*----------------------------------------------------------------------------------------*/



#endif // FI_GRAPHICS
