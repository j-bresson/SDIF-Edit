/*
 *  SVMatrix.h
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

#ifndef SV_MATRIX_H
#define SV_MATRIX_H

#include "include.h" 

#include "Vector.h"


// structure to store relevant matrix information into the SVMatrix matrix description
typedef struct matProperties matPropertiesT;
struct matProperties {
    float time;		// time of the corresponding frame
    SdiffPosT pos;	// position of the matrix in file
    SdiffPosT framepos; // position of the frame in file
    int nbElem;		// number of elements (= nb rows)
};


/*
* Description of a matrix in a Sdif File
* The matrix is supposed to appear in frames from a particular stream
* This class stores general informations about this matrix (signature, structure,...) , 
* and informations related to each occurence of it (times, positions,...) 
*/
class SVMatrix {

private :
	SdifSignature sig;	// Signature of the matrix
	SdifInt4 nbRowMax;	// Maximal number of rows (elements of the matrix) (can change in each occurence of the matrix) 
	SdifInt4 nbCol;		// Number of columns (fields of the matrix) (supposed to be constant)
	SdifDataTypeET dataType;// SdifDataType of the data 
	char* name;		// Name of the matrix
	char ** colnames;	// Table with names of each column (field)
	int nbTimes;		// Number of occurence of the matrix in its stream 
	SdifFloat8 beginTime;	// Time of first occurence
	SdifFloat8 endTime;	// Time of last occurence
	
	Vector<matPropertiesT> matOcc; // Vector containing proerties of each occurences of the matrix
	
public :
	//SVMatrix(SdifSignature sig, SdifInt4 r, SdifInt4 c, SdifDataTypeET dt, char** names);
	SVMatrix(SdifSignature _sig, char* _name);
	SVMatrix(){};
	~SVMatrix();
	
	// Returns SdifDataType of the matrix
	SdifDataTypeET getDataType(); 
	// Returns signature of the matrix as a string
	char* getSigText();
	// Returns name of the matrix
	char* getName();
	// Sets name of the matrix
	void setName(char* nm);
	// Retruns name of column (field) number num
	char* getColName(int num);
	// Return signature of the matrix as a SdifSignature
	SdifSignature getMatSignature();
	// Returns maximal number of rows of the matrix
	int getNbRowMax();
	// Returns number of columns (fields) of the matrix
	int getNbCol();
	// Returns number of occurences of the matrix in his stream
	int getNbTimes();
	// Returns matrix header position in file of occurence i of the matrix
	SdiffPosT getPosition(int i);
	// Returns relative frame header position in file of occurence i of the matrix
	SdiffPosT getFramePosition(int i);
	// Returns number of elements (rows) of occurence i of the matrix
	int getNbElem(int i);
	// Returns total number of points from the matrix in the whole file
	int getNbPoints();
	// Returns time of occurence i of the matrix
	float getTime(int i);
	// Returns time of first occurence of the matrix
	SdifFloat8 getBeginTime();
	// Returns time of last occurence of the matrix
	SdifFloat8 getEndTime();
	
	// Notify a new occurence of the matrix in the SDIF file
	int setMatrix(int c, int r, SdifMatrixTypeT *MatrixType, SdifDataTypeET dt, SdifFloat8 time, SdiffPosT pos, SdiffPosT fpos );
};

#endif

    


