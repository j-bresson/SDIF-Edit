/*
 *  SplineSurface.h
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
 
#ifndef SPLINE_SURFACE_H
#define SPLINE_SURFACE_H

#include "include.h"

struct XYZ {
    float x; float y; float z;
};

class SplineSurface {

private :
    
    // surface resolution
    int RESOLUTIONI;
    int RESOLUTIONJ;
    
    // degree
    int TI;
    int TJ;
    
    // nb points
    int NI;
    int NJ;
    
    // control points grid
    XYZ **pts;
    
    // variable values for control points positioning (0 <= v <= 1)
    float i1, i2, i3, i4;
    float j1, j2, j3, j4;
    
    
    
    // compute knots
    void SplineKnots(int *u,int n,int t);
    
    // compute blending
    double SplineBlend(int k,int t,int *u,double v);
    
    // setting initial values for control points
    void setPoints();
    
public :
	
    // spline surface
    XYZ **surface;
    SplineSurface(int resI = 21, int resJ = 21, int ti = 3, int tj = 3, int ni = 6, int nj = 6);
    ~SplineSurface();
    // compute spline surface
    void calculateSplineSurface();
    float findSplineAlt(float i, float j);
    int getResolutionI();
    int getResolutionJ();
    int getTI();
    int getTJ();
    void setResolutionI(int res);
    void setResolutionJ(int res);
    void setTI(int deg);
    void setTJ(int deg);
    float getI1();
    float getI2();
    float getI3();
    float getI4();
    float getJ1();
    float getJ2();
    float getJ3();
    float getJ4();
    void setI1(float v);
    void setI2(float v);
    void setI3(float v);
    void setI4(float v);
    void setJ1(float v);
    void setJ2(float v);
    void setJ3(float v);
    void setJ4(float v);
    
};

#endif

