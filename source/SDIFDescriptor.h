/*
 *  SdifDescriptor.h
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

#ifndef SDIF_DESCRIPTOR_H
#define SDIF_DESCRIPTOR_H

#include "include.h"

#include "SVStream.h"
#include "SVMatrix.h" 
#include "Vector.h"
//#include "Partial.h"



/**
* Interface with a SDIF archive.
* An instance of SdifDescriptor is defined with a file name, which makes it 
* related to only one particular Sdif file. 
* The vector "streams" contains file streams's description (SVStream class).
* Each stream description contains inner matrices descriptions (SVMatrix class).
*/
class SDIFDescriptor {

    private :
	
	SdifFileT* file;		// Sdif file 	
	const char* fileName;		// Name of of Sdif file
	int nbStreams;			// Number of streams in Sdif File
	int nbMatrices;			// Number of matrices in Sdif File
	Vector<SVStream> streams;	// Streams descriptions
	Vector<SVMatrix> matrices;	// Matrix descriptions
	Vector<viewpoint> viewpoints;

public :
    
	// Constructor
	SDIFDescriptor(const char* fName);
	// Destructor
	~SDIFDescriptor() {};
	// Returns the number of streams in the Sdif file
	int getNbStreams();
	// Returns the number of matrices in the Sdif file
	int getNbMatrices();
	// Returns file Name
	const char* getFileName();
	// Notify a new stream in the SdifFile
	void addStream(SVStream str);
	// Notify a new valid matrix in the SdifFile
	void addMatrix(SVMatrix mat);
	// Returns description of stream number num
	SVStream* getStream(int num);
	// Returns description of matrix number num
	SVMatrix* getMatrix(int num);
	// Return number of viewpoints
	int getNbViewPoints();
	// Returns a viewpoint
	viewpoint* getViewPoint(int num);
	// Create a table with all matrices streams of the file
	void createMatrixTable();
	
	// Reads and parse information of the Sdif File
	// Returns 0 if file correctly read, -1 otherwise
	int readSdifFile();
	// Gets data of a specified matrix from Sdif File into data structure
	// Returns 0 if matrix correctly read, -1 otherwise
	int getMatrixData(matrixValuesT* data, SVMatrix* mtr);
	// Writes matrix data from data structure into Sdif File
	// Returns 0 if matrix written, -1 otherwise
	int writeMatrixData(matrixValuesT* data, SVMatrix* mtr);
	// Closes Sdif File
	void closeSdifFile();

	void removeAllVP(int matrixNum);

	void addVP(viewpoint vp);

	void writeViewPointsFile();
	void loadViewPoints();
};

#endif

    


