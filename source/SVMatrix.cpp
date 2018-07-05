/*
 *  SVMatrix.cpp
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

#include "SVMatrix.h"

SVMatrix::SVMatrix(SdifSignature _sig, char* _name){
    
    sig = _sig;
    name = new char[strlen(_name)];
    strcpy(name,_name);
    
    nbTimes = 0;
    beginTime = endTime = -1;
    nbCol = nbRowMax = 0;
    dataType = (SdifDataTypeET) 0x0000;  // default value : will be assigned at first occurence of the matrix 

}

SVMatrix::~SVMatrix(){
    
}

int SVMatrix::setMatrix(int c, int r, SdifMatrixTypeT* MatrixType, SdifDataTypeET dt, SdifFloat8 time, SdiffPosT pos, SdiffPosT fpos){
    
    // If it's the first occurence of the matrix..
    if(nbTimes==0){
	nbCol = c;
	nbRowMax = r;
	dataType = dt;
	colnames = new char*[nbCol];
	// Get the names of the column using given SdifMatrixTypeT
	SdifColumnDefT *ColDef;
	for(int n = 1; n <= nbCol; n++){
	    colnames[n-1]=new char[50];
	    ColDef = SdifMatrixTypeGetNthColumnDef(MatrixType, n);
	    if(ColDef == NULL) sprintf(colnames[n-1],"column %i",n);
	    else strcpy(colnames[n-1],ColDef->Name);
	    // SdifMatrixTypeGetColumnName(MatrixType, n) ...	    
	}
	
	beginTime = endTime = time;
	
    } else {
	// Check validity of the new data
	if(dt != dataType || c != nbCol){
	    cout << "!!ERROR!! Matrix " << name << ": data read doesn't correspond with previous data."<<endl;
	    cout << dt << "-" << dataType << "    " << c << "-" << nbCol << endl;
	    return -1;
	}
	// update general datas for the matrix
	if(time<beginTime) beginTime=time;
	if(time>endTime) endTime = time;
	if(r>nbRowMax) nbRowMax = r;
    }
    
    // In any case: add a new occurence with corresponding informations
    nbTimes+=1;
    matPropertiesT mp = {time,pos,fpos,r};
    matOcc.push_back(mp);
    
    return 1;
}


// ACCESSORS //

char* SVMatrix::getSigText() {
    return SdifSignatureToString(sig);
}

SdifDataTypeET SVMatrix::getDataType(){
    return dataType;
} 

SdifSignature SVMatrix::getMatSignature() {
    return sig;
}

char* SVMatrix::getName() {
    return name;
}

char* SVMatrix::getColName(int num) {
    return colnames[num];
}

int SVMatrix::getNbRowMax(){ 
    return nbRowMax; 
}

int SVMatrix::getNbCol() { 
    return nbCol; 
}

int SVMatrix::getNbTimes() { 
    return nbTimes; 
}

SdiffPosT SVMatrix::getPosition(int i){ 
    return matOcc[i].pos; 
}

SdiffPosT SVMatrix::getFramePosition(int i){ 
    return matOcc[i].framepos; 
}

int SVMatrix::getNbElem(int i){ 
    return matOcc[i].nbElem; 
}

int SVMatrix::getNbPoints(){
    int n = 0;
    for(int i = 0; i<nbTimes; i++) n += matOcc[i].nbElem * nbCol;
    return n;
}

float SVMatrix::getTime(int i){ 
    return matOcc[i].time; 
}

SdifFloat8 SVMatrix::getBeginTime() {
    return beginTime; 
}

SdifFloat8 SVMatrix::getEndTime() {
    return endTime; 
}


void SVMatrix::setName(char* nm) {
    name = nm;
}



