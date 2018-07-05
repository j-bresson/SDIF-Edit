/*
 *  include.h
 *
 *  SDIF-Edit: 3D vizualizer for SDIF files
 *  Copyright (C) 2003 Jean Bresson, IRCAM
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; see the file COPYING. If not, write to the
 *  Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef INCLUDE_H
#define INCLUDE_H

// General includes

#ifdef __MACOSX_CORE__

#include <iostream>
#include <fstream>
#include <math.h>

#else

#include <iostream>
#include <fstream>

#endif

#include <float.h>
#include <string>
using std::string;

// Special includes

#ifdef WIN32
#include <windows.h>
#include <math.h>
#endif

#ifndef M_PI
#define M_PI   3.14159265358979323846f  
#endif

#ifdef __MACOSX_CORE__
#include <unistd.h>
#endif


// Libs

#ifdef __MACOSX_CORE__

#include <sdif.h>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#else

#include <sdif.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#endif

// Structure to store one matrix data for the viewer:
// time of occurence,
// dimension (number of rows can change at each time),
// and correponding values
typedef struct matrixValues matrixValuesT;
struct matrixValues {
    float time;
    int dimension;
    float ** values;
};

// Color RGB
struct color {
    float r;
    float g;
    float b;
};

// Structure to store a set of user settings
// (positions, scales, colors,...)
struct viewpoint {
    int matrixNum;
    int xparam;
    int field;
    int colorParam;
    float colorLowT;
    float colorHighT;
    float posx;
    float posy;
    float posz;
    float scaleT;
    float scaleE;
    float scaleV;
    float ax;
    float ay;
    string name;
};


#endif
