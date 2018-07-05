/*
 *  SplineSurface.cpp
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

#include <math.h>
#include "SplineSurface.h"


SplineSurface::SplineSurface(int resI, int resJ, int ti, int tj, int ni, int nj){
    
    RESOLUTIONI = resI;
    RESOLUTIONJ = resJ;
    TI = ti;
    TJ = tj;
    NI = ni;
    NJ = nj;
    
    pts = new XYZ*[NI+1];
    for(int i = 0; i<=NI; i++) pts[i] = new XYZ[NJ+1];
    
    surface = NULL;
    
    i1 = j1 = 0.2;
    i2 = j2 = 0.4;
    i3 = j3 = 0.6;
    i4 = j4 = 0.8;

    setPoints();
}

SplineSurface::~SplineSurface(){
    
    delete[] pts;
    delete[] surface;

}


/*
 Calculate the blending value for the spline recursively
 If the numerator and denominator are 0 the result is 0
 */
double SplineSurface::SplineBlend(int k,int t,int *u,double v){
    double value;
    if (t == 1) {
	if ((u[k] <= v) && (v < u[k+1]))
	    value = 1;
	else
	    value = 0;
    } else {
	if ((u[k+t-1] == u[k]) && (u[k+t] == u[k+1]))
	    value = 0;
	else if (u[k+t-1] == u[k])
	    value = (u[k+t] - v) / (u[k+t] - u[k+1]) * SplineBlend(k+1,t-1,u,v);
	else if (u[k+t] == u[k+1])
	    value = (v - u[k]) / (u[k+t-1] - u[k]) * SplineBlend(k,t-1,u,v);
	else
	    value = (v - u[k]) / (u[k+t-1] - u[k]) * SplineBlend(k,t-1,u,v) +
		(u[k+t] - v) / (u[k+t] - u[k+1]) * SplineBlend(k+1,t-1,u,v);
    }
    return(value);
}


/*
 Figure out the knots, u[]
	There are n + t + 1 knots
 */
void SplineSurface::SplineKnots(int *u,int n,int t){
    int j;
    for (j=0;j<=n+t;j++) {
	if (j < t)
	    u[j] = 0;
	else if (j <= n)
	    u[j] = j - t + 1;
	else if (j > n)
	    u[j] = n - t + 2;
    }
}

void SplineSurface::setPoints(){
    //cout << "SET POINTS" << endl;
    int i,j;
    for (j=0;j<=NJ;j++) {
	pts[0][j].x = 0;
	pts[NI][j].x = 1;
	pts[1][j].x = i1;
	pts[2][j].x = i2;
	pts[3][j].x = 0.5;
	pts[4][j].x = i3;
	pts[5][j].x = i4;
    }
    for (i=0;i<=NI;i++) {	    
	pts[i][0].y = 0;
	pts[i][NJ].y = 1;
	pts[i][1].y = j1;
	pts[i][2].y = j2;
	pts[i][3].y = 0.5;
	pts[i][4].y = j3;
	pts[i][5].y = j4;
    }
    for (i=0;i<=NI;i++) {
	for (j=0;j<=NJ;j++) {
	    if(i>=2 && i<=4 && j>=2 && j<=4)  pts[i][j].z = 1;
	    else pts[i][j].z = 0;
	}
    }
    pts = pts;
}

void SplineSurface::calculateSplineSurface(){
    
    int* knotsI = new int[NI+TI+1];
    int* knotsJ = new int[NJ+TJ+1];
    
    int i,j,ki,kj;
    double intervalI,incrementI;
    double intervalJ,incrementJ;
    double bi,bj;
    
    surface = new XYZ*[RESOLUTIONI];
    for(i = 0;i<RESOLUTIONI; i++){
	surface[i] = new XYZ[RESOLUTIONJ];
    }
          
    /* Step size along the curve */
    incrementI = (NI - TI + 2) / ((double)RESOLUTIONI - 1);
    incrementJ = (NJ - TJ + 2) / ((double)RESOLUTIONJ - 1);
    /* Calculate the knots */
    SplineKnots(knotsI,NI,TI);
    SplineKnots(knotsJ,NJ,TJ);
    intervalI = 0;
    for (i=0;i<RESOLUTIONI-1;i++) {
	intervalJ = 0;
	for (j=0;j<RESOLUTIONJ-1;j++) {
	    surface[i][j].x = 0;
	    surface[i][j].y = 0;
	    surface[i][j].z = 0;
	    for (ki=0;ki<=NI;ki++) {
		for (kj=0;kj<=NJ;kj++) {
		    bi = SplineBlend(ki,TI,knotsI,intervalI);
		    bj = SplineBlend(kj,TJ,knotsJ,intervalJ);
		    surface[i][j].x += (pts[ki][kj].x * bi * bj);
		    surface[i][j].y += (pts[ki][kj].y * bi * bj);
		    surface[i][j].z += (pts[ki][kj].z * bi * bj);
		}
	    }
	    intervalJ += incrementJ;
	}
	intervalI += incrementI;
    }
    
    
    intervalI = 0;
    for (i=0;i<RESOLUTIONI-1;i++) {
	surface[i][RESOLUTIONJ-1].x = 0;
	surface[i][RESOLUTIONJ-1].y = 0;
	surface[i][RESOLUTIONJ-1].z = 0;
	for (ki=0;ki<=NI;ki++) {
	    bi = SplineBlend(ki,TI,knotsI,intervalI);
	    surface[i][RESOLUTIONJ-1].x += (pts[ki][NJ].x * bi);
	    surface[i][RESOLUTIONJ-1].y += (pts[ki][NJ].y * bi);
	    surface[i][RESOLUTIONJ-1].z += (pts[ki][NJ].z * bi);
	}
	intervalI += incrementI;
    }
    surface[i][RESOLUTIONJ-1] = pts[NI][NJ];
    intervalJ = 0;
    for (j=0;j<RESOLUTIONJ-1;j++) {
	surface[RESOLUTIONI-1][j].x = 0;
	surface[RESOLUTIONI-1][j].y = 0;
	surface[RESOLUTIONI-1][j].z = 0;
	for (kj=0;kj<=NJ;kj++) {
	    bj = SplineBlend(kj,TJ,knotsJ,intervalJ);
	    surface[RESOLUTIONI-1][j].x += (pts[NI][kj].x * bj);
	    surface[RESOLUTIONI-1][j].y += (pts[NI][kj].y * bj);
	    surface[RESOLUTIONI-1][j].z += (pts[NI][kj].z * bj);
	}
	intervalJ += incrementJ;
    }
    surface[RESOLUTIONI-1][j] = pts[NI][NJ];

    /*
    for(i = 0; i<RESOLUTIONI; i++){
	for(j = 0; j<RESOLUTIONJ; j++){
	    cout << surface[i][j].x << " ";
	}
	cout << endl;
    }
    cout << endl;
    for(i = 0; i<RESOLUTIONI; i++){
	for(j = 0; j<RESOLUTIONJ; j++){
	    cout << surface[i][j].y << " ";
	}
	cout << endl;
    }
    cout << endl << endl;
    */
}

// A ameliorer...

float SplineSurface::findSplineAlt(float i, float j){

    float distMin = FLT_MAX;
    float dist;
    int im,jm;
    for(int I = 0; I< RESOLUTIONI; I++){
	for(int J = 0; J< RESOLUTIONJ; J++){
	    dist = sqrt((i-surface[I][J].x)*(i-surface[I][J].x)+(j-surface[I][J].y)*(j-surface[I][J].y));
	    if(dist < distMin){
		distMin = dist;
		im = I;
		jm = J;
	    }
	}
    }
    
    return surface[im][jm].z;

}

/*
float SplineSurface::findSplineAlt(float i, float j){
    //float distMin = FLT_MAX;
    //float dist;
    int im = -1;
    int jm = -1;
    float dist1, dist2, dist3, dist4, alt;
    //dist = sqrt((i-surface[I][J].x)*(i-surface[I][J].x)+(j-surface[I][J].y)*(j-surface[I][J].y));
    for(int I = 0; I< RESOLUTIONI && im==-1; I++){
	if(surface[I][0].x>=i) im = I;
    }
    for(int J = 0; J< RESOLUTIONJ && jm==-1; J++){
	if(surface[0][J].y>=j) jm = J;
    }
    
    
    if(im == RESOLUTIONI-1 && jm == RESOLUTIONJ-1){
	cout << "1" << endl;
	alt = surface[im][jm].z;
    } else if(im == RESOLUTIONI-1){
    cout << "2" << endl;
	dist1 = sqrt((i-surface[im][jm].x)*(i-surface[im][jm].x)+(j-surface[im][jm].y)*(j-surface[im][jm].y));
	dist3 = sqrt((i-surface[im][jm+1].x)*(i-surface[im][jm+1].x)+(j-surface[im][jm+1].y)*(j-surface[im][jm+1].y));
	alt = (dist1*surface[im][jm].z + dist3*surface[im][jm+1].z)/(dist1+dist3);
    } else if(jm == RESOLUTIONJ-1){
	cout << "3" << endl;
	dist1 = sqrt((i-surface[im][jm].x)*(i-surface[im][jm].x)+(j-surface[im][jm].y)*(j-surface[im][jm].y));
	dist2 = sqrt((i-surface[im+1][jm].x)*(i-surface[im+1][jm].x)+(j-surface[im+1][jm].y)*(j-surface[im+1][jm].y));
	alt = (dist1*surface[im][jm].z+dist2*surface[im+1][jm].z)/(dist1+dist2);
    } else {
	cout << "4" << endl;
	dist1 = sqrt((i-surface[im][jm].x)*(i-surface[im][jm].x)+(j-surface[im][jm].y)*(j-surface[im][jm].y));
	dist2 = sqrt((i-surface[im+1][jm].x)*(i-surface[im+1][jm].x)+(j-surface[im+1][jm].y)*(j-surface[im+1][jm].y));
	dist3 = sqrt((i-surface[im][jm+1].x)*(i-surface[im][jm+1].x)+(j-surface[im][jm+1].y)*(j-surface[im][jm+1].y));
	dist4 = sqrt((i-surface[im+1][jm+1].x)*(i-surface[im+1][jm+1].x)+(j-surface[im+1][jm+1].y)*(j-surface[im+1][jm+1].y));
	alt = (dist1*surface[im][jm].z +dist2*surface[im+1][jm].z +dist3*surface[im][jm+1].z + dist4*surface[im+1][jm+1].z)/(dist1+dist2+dist3+dist4);
    }
    return alt;

}
*/

int SplineSurface::getResolutionI(){
    return RESOLUTIONI;
}

int SplineSurface::getResolutionJ(){
    return RESOLUTIONJ;
}

int SplineSurface::getTI(){
    return TI;
}

int SplineSurface::getTJ(){
    return TJ;
}

void SplineSurface::setResolutionI(int res){
    if(res >= 3) RESOLUTIONI = res;
    calculateSplineSurface();
}

void SplineSurface::setResolutionJ(int res){
    if(res >= 3) RESOLUTIONJ = res;
    calculateSplineSurface();
}

void SplineSurface::setTI(int deg){
    if(deg > 0 && deg < 8) TI = deg;
    calculateSplineSurface();
}

void SplineSurface::setTJ(int deg){
    if(deg > 0 && deg < 8) TJ = deg;
    calculateSplineSurface();
}


float SplineSurface::getI1(){ return i1; }
float SplineSurface::getI2(){ return i2; }
float SplineSurface::getI3(){ return i3; }
float SplineSurface::getI4(){ return i4; }
float SplineSurface::getJ1(){ return j1; }
float SplineSurface::getJ2(){ return j2; }
float SplineSurface::getJ3(){ return j3; }
float SplineSurface::getJ4(){ return j4; }


void SplineSurface::setI1(float v){
    if(v>=0 && v<=i2){
	i1 = v;
	setPoints();
    }
}
void SplineSurface::setI2(float v){
    if(v>=i1 && v<=0.5){
	i2 = v;
	setPoints();
    }
}
void SplineSurface::setI3(float v){
    if(v>=0.5 && v<=i4){
	i3 = v;
	setPoints();
    }
}
void SplineSurface::setI4(float v){
    if(v>=i3 && v<=1){
	i4 = v;
	setPoints();
    }
}

void SplineSurface::setJ1(float v){
    if(v>=0 && v<=j2) {
	j1 = v;
	setPoints();
    }
}
void SplineSurface::setJ2(float v){
    if(v>=j1 && v<=0.5){
	j2 = v;
	setPoints();
    }
}
void SplineSurface::setJ3(float v){
    if(v>=0.5 && v<=j4) {
	j3 = v;
	setPoints();
    }
}
void SplineSurface::setJ4(float v){
    if(v>=j3 && v<=1){
	j4 = v;
	setPoints();
    }
}
