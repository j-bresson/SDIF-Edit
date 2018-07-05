/*
 *  SDIFDescriptor.cpp
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

#include "SDIFDescriptor.h"

//using namespace std;
//using std::cout;

/*
#ifdef __MACOSX_CORE__
#include <fstream.h>
#else 
#include <fstream>
#endif
using namespace std;
*/

SDIFDescriptor::SDIFDescriptor(const char* fName) {
    nbStreams = 0;
    nbMatrices = 0;
    fileName = fName; 
    file = NULL;
}


void SDIFDescriptor::addStream(SVStream str) {
    streams.push_back(str);
    nbStreams++;
}

void SDIFDescriptor::addMatrix(SVMatrix mat) {
    matrices.push_back(mat);
    nbMatrices++;
}

int SDIFDescriptor::getNbStreams() {
    return nbStreams;
}

int SDIFDescriptor::getNbMatrices() {
    return nbMatrices;
}

int SDIFDescriptor::getNbViewPoints() {
    return viewpoints.size();
}

viewpoint* SDIFDescriptor::getViewPoint(int n){
    return &viewpoints[n];
}

SVStream* SDIFDescriptor::getStream(int num) {
    return &streams[num] ;
}

SVMatrix* SDIFDescriptor::getMatrix(int num) {
    return &matrices[num] ;
}
/*
 * Reads data of matrix defined by SVMatrix mtr and writes it in "data"
* Note: SVMatrix contains information on file position of each occurence of the matrix
* and related frame in the file.
*/
int SDIFDescriptor::getMatrixData(matrixValuesT* data, SVMatrix* mtr){
   
    cout << "READING MATRIX DATA..." << endl;
    int nf, row, col;
    int err;
    size_t bytesread = 0;
    SdiffPosT pos,posF;
    
    // Number of occurence of the matrix
    int nbtimes = mtr->getNbTimes();

    for(nf=0; nf<nbtimes; nf++) {
	
	// Positions of the matrix and the related frame in file
	pos = mtr->getPosition(nf);
	posF = mtr->getFramePosition(nf);
	
	// Go to frame position and read frame header
	// (required by Sdif lib before to read a matrix header int file)
	err = SdiffSetPos(file->Stream, &posF);
	if(err==-1){
	    cout << "Error SetPos !!!" << endl;
	    return err;
	}
	SdifFGetSignature(file, &bytesread);
	bytesread += SdifFReadFrameHeader (file);
	
	// Go to matrix position and read matrix header 	
	err = SdiffSetPos(file->Stream, &pos);
	if(err==-1){
	    cout << "Error SetPos !!!" << endl;
	    return err;
	}
	bytesread += SdifFReadMatrixHeader (file);
	
	SdifSignature	sigM   = SdifFCurrMatrixSignature (file);
	SdifInt4	nrows = SdifFCurrNbRow (file);
	SdifInt4	ncols = SdifFCurrNbCol (file);
	SdifDataTypeET	type  = SdifFCurrDataType (file);
	
	if(sigM != mtr->getMatSignature() || nrows != mtr->getNbElem(nf) || ncols!=mtr->getNbCol() || type != mtr->getDataType()){
	    cout << "error matrix read doesn't correspond with SVMatrix description" << endl; 
	    return -1;
	}
	
	float value;
	for (row = 0; row < nrows; row++){
	    bytesread += SdifFReadOneRow (file);
	    for (col = 1; col <= ncols; col++){
		value = SdifFCurrOneRowCol (file, col);
		data[nf].values[row][col] = value;
	    }
	}
	
	if (SdifFLastError (file)){
	    cout << "error reading SDIF file" << endl;
	    return -1;
	}
    }
    return 0;
    
}



int SDIFDescriptor::writeMatrixData(matrixValuesT* data, SVMatrix* mtr){
        cout << "Writing DATA using SDIF 3.8.0" << endl;
        int nf, row, col;
	int err;
	SdiffPosT pos;
	size_t bytes = 0;
	    // Get nuber of occurence of the matrix to write
	int nbtimes = mtr->getNbTimes();
	// General information for matrix headers
	SdifDataTypeET dt = mtr->getDataType();
	int nc = mtr->getNbCol();
    SdifSignature sig = mtr->getMatSignature();
    int nr;	// number of rows : can change in each occurence of the matrix
    
    for(nf=0; nf<nbtimes; nf++) {
	
	// get data 
	nr = data[nf].dimension;
	//float matrixData[nc*nr];
	float* matrixData = new float[nc*nr];
	
	for(row=0;row<nr;row++)
	    for(col=0;col<nc;col++)
		matrixData[row*nc+col]=data[nf].values[row][col+1];

	// go to matrix position
	pos = mtr->getPosition(nf);
	err = SdiffSetPos(file->Stream, &pos);
	if(err==-1){
	    cout << "Error SetPos !!!" << endl;
	    return err;
	}
	// Write data in file
	bytes = SdifFWriteMatrix(file,sig,dt,nr,nc,matrixData);

    }

    cout << "... Saved in " << fileName << endl;
    return 0;
    
}

void SDIFDescriptor::removeAllVP(int MatNum){
	int i = 0;
    while(i<viewpoints.size()){
	if(viewpoints[i].matrixNum == MatNum){
		std::cout << "remove " << viewpoints[i].name << std::endl; 
	    viewpoints.remove(i);
	}
	else i++;
    }
}

void SDIFDescriptor::addVP(viewpoint vp){
    viewpoints.push_back(vp);
	std::cout << "added " << vp.name << std::endl;
}


void SDIFDescriptor::writeViewPointsFile(){
    
	using namespace std;
	char* vpFileName = new char[strlen(fileName)+4];
    sprintf(vpFileName,"%s.vpt",fileName);
    
	ofstream outfile;
    outfile.open(vpFileName, ofstream::out);
    for(int i = 0; i<viewpoints.size(); i++){
		outfile << viewpoints[i].matrixNum << " ";
		outfile << viewpoints[i].xparam << " ";
		outfile << viewpoints[i].field  << " ";
		outfile << viewpoints[i].colorParam << " ";
		outfile << viewpoints[i].colorLowT << " " << viewpoints[i].colorHighT << " ";
		outfile << viewpoints[i].posx << " " << viewpoints[i].posy << " " << viewpoints[i].posz << " ";
		outfile << viewpoints[i].scaleT << " " << viewpoints[i].scaleE << " " << viewpoints[i].scaleV << " ";
		outfile << viewpoints[i].ax << " " << viewpoints[i].ay << " " ;
		outfile << "#" << viewpoints[i].name;
		outfile << endl;
    }
    outfile.close();
    
    if(viewpoints.size()==0){
		std::cout << "No viewpoints saved" << endl;
	remove(vpFileName);
    } else {
		std::cout << "View points saved in " << vpFileName << endl;	
    }
}

void SDIFDescriptor::loadViewPoints(){
	using namespace std;
    ifstream infile;
    char* vpFileName = new char[strlen(fileName)+4];
     sprintf(vpFileName,"%s.vpt",fileName);
     char line[1024];
     int matNum,xp,field, colorp;
     float colorl, colorh, posx,posy,posz,sct,sce,scv,ax,ay;
     infile.open(vpFileName, ifstream::in);
     if(infile.is_open()){
	 infile.getline(line,1024,'#');
	 while(!infile.eof() && !infile.fail()){
	     sscanf(line,"%d %d %d %d %f %f %f %f %f %f %f %f %f %f",&matNum,&xp,&field,&colorp,
	     &colorl, &colorh,
	     &posx,&posy,&posz,
	     &sct,&sce,&scv,
	     &ax,&ay);
		infile.getline(line,1024);
		viewpoint vp;
		vp.matrixNum = matNum;
		vp.xparam = xp;
		vp.field = field;
		vp.colorParam = colorp;
		vp.colorLowT = colorl;
		vp.colorHighT = colorh;
		vp.posx = posx;
		vp.posy = posy;
		vp.posz = posz;
		vp.scaleT = sct;
		vp.scaleE = sce;
		vp.scaleV = scv;
		vp.ax = ax;
		vp.ay = ay;
		vp.name = line;
		viewpoints.push_back(vp);
		if(infile.eof()) std::cout << "eof" << std::endl;
		if(infile.fail()) std::cout << "fail" << std::endl;
	    infile.getline(line,1024,'#');
	 }
     } else {
		 std::cout << "No viewponts file found" << std::endl;
     }
     infile.close();
}

/*
 * Parse the Sdif File in order to get a complete description of its structure
* using SVStream and SVMatrix description classes.
*/
int SDIFDescriptor::readSdifFile(){
    // temporary objects
    SdifMatrixTypeT *MatrixType;
    SdifFrameTypeT *FrameType;
    SVStream* tmpStream;
    SVMatrix* tmpMat;
    SdiffPosT pos,posF;
    
    // Check file
    if (SdifCheckFileFormat (fileName))
	cout <<  "SDIF FILE OK" << endl;
    else return -1;
    
    // Open file in read/write mode once for all the session:
    // Later we may read or write matrices data in it
    file = SdifFOpen(fileName, eReadWriteFile);
    
    cout << "READING FILE..." << endl;
    size_t  bytesread = 0;
    int	eof = 0;	// End-of-file flag
    
    // Open file for reading and parse selection.
    SdifFReadGeneralHeader  (file); // Read file header
    SdifFReadAllASCIIChunks (file); // Read ASCII header info, such as name-value tables

	// Read all frames
	while (!eof  &&  SdifFLastError (file) == NULL) {
		// Get current frame position
		int err = SdiffGetPos(file->Stream,&posF);
		if(err==-1){
			cout << "Error GetPos !!!" << endl;
			return err;
		} else {
			posF-=sizeof(SdifSignature); // one step backward, since frame signature has been read yet 
		}
		
		bytesread += SdifFReadFrameHeader(file);
		if (!eof) {    
			// Access frame header information
			SdifFloat8      time     = SdifFCurrTime (file);
			SdifSignature   sig      = SdifFCurrFrameSignature (file);
			SdifUInt4       streamid = SdifFCurrID (file);
			SdifUInt4       nmatrix  = SdifFCurrNbMatrix (file);
			// check whether this stream is known from our descriptor or not:
			bool newStream = true;
			for(int i=0; i < nbStreams && newStream; i++){
				tmpStream = &streams[i];
				// For us a stream is defined with a stream id AND a frame signature
				if (tmpStream->getId() == streamid && tmpStream->getFrameSignature() == sig) {
					newStream = false;
					tmpStream->oneMoreFrame(time);
				}
			}

			// If the current frame is new add it to the file description
			if(newStream){
				FrameType = SdifTestFrameType(file,sig);
				addStream(SVStream(streamid,sig,time,FrameType));
				tmpStream = &streams[nbStreams-1];
			}
			// then, tmpStream points on the stream descriptor of the current frame
			// Read all matrices in this frame
			for (unsigned int m = 0; m < nmatrix; m++) {
				// Get matrix header position
				int err = SdiffGetPos(file->Stream,&pos);
				if(err==-1){
					cout << "Error GetPos !!!" << endl;
					return err;
				}
				// read matrix header
				bytesread += SdifFReadMatrixHeader(file);
		
				// Access matrix header information
				SdifSignature sig   = SdifFCurrMatrixSignature (file);
				SdifInt4 nrows = SdifFCurrNbRow (file);
				SdifInt4 ncols = SdifFCurrNbCol (file);
				SdifDataTypeET type  = SdifFCurrDataType (file);
		
				//MatrixType = SdifGetMatrixType(file->MatrixTypesTable,sig);
				MatrixType = SdifTestMatrixType (file, sig);
				// get the matrix description of the current matrix in the current stream descriptor
				tmpMat = tmpStream->getMatrixFromSignature(sig);
				if(tmpMat == NULL) return -1;
		
				// Set a new occurence of the matrix in the matrix descriptor
				// with matrix informations, time, frame and matrix positions.
				err = tmpMat->setMatrix(ncols,nrows,MatrixType,type,time,pos,posF);
				if(err == -1) return err;
				// for description we don't need to read matrix data
				bytesread += SdifFSkipMatrixData(file);
			}
			
			eof = SdifFGetSignature(file, &bytesread) == 4 ;  // eEof;
		}
	
		// Check for errors
		if (SdifFLastError (file)) {
			cout << "ERREUR" << endl;
			//exit (1);
			return -1;
		}
    }
	loadViewPoints();
    cout << "END READ FILE" << endl;
	return 0;
}

void SDIFDescriptor::createMatrixTable(){

    int i,j;
    SVStream *svs;
    SVMatrix *svm;
    for(i = 0; i<nbStreams; i++){
	svs = getStream(i);
	int n = svs->getNbMat();
	for(j=0; j<n;j++){
	    svm = svs->getMatrix(j);
	    if(svm->getNbTimes()>0){
		addMatrix(*svm);
	    }
	}
    }
}

const char* SDIFDescriptor::getFileName(){
    return fileName;
}


void SDIFDescriptor::closeSdifFile(){
    if(file != NULL) SdifFClose (file);
}
