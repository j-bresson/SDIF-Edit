/*
 *  SVStream.cpp
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

#include "SVStream.h"

SVStream::SVStream(SdifUInt4 id, SdifSignature sig, SdifFloat8 t, SdifFrameTypeT *FrameType){
    
    idStream = id;
    frameSign = sig;
    nbFrames = 1;
    begin = t;
    end = t;
    
    // Fill matrices vector with data from given SdifFrameTypeT
    
    // Number of components (matrices)
    SdifUInt4 nc = FrameType->NbComponent;
    // SdifUInt4 ncu = FrameType->NbComponentUse; 	// Components created by the user
    //cout << getName() << "   " << "nbComp: " << nc << " - NbCompUse: " << ncu << endl;
    
    nbmat = nc;
    SdifComponentT *Comp ;
    for(unsigned int i = 1; i <= nbmat; i++){
	// Get description of each matrix
	// In Sdif, indices of n Components are (1..n)
	Comp = SdifFrameTypeGetNthComponent(FrameType, i);
	char* name;
	if(Comp == NULL) sprintf(name, "matrix %i",i);
	else name = Comp->Name;
	SdifSignature sigMat = Comp->MtrxS;
	// Create a new matrix descriptor and add it to matrices vector
	addMatrix(SVMatrix(sigMat,name));
    }
}

void SVStream::addMatrix(SVMatrix m){
    matrices.push_back(m);
}

void SVStream::oneMoreFrame(SdifFloat8 t){
    // update number of frames in the stream
    nbFrames++;
    // update begin and end times of the stream
    if(t<begin) begin=t;
    else if(t>end) end =t;
}



SdifUInt4 SVStream::getId(){
    return idStream;
}

char* SVStream::getName(){
    // Return the Frame signature as his name
    return SdifSignatureToString(frameSign);
} 


unsigned int SVStream::getNbMat(){
    return nbmat;
}

SVMatrix* SVStream::getMatrix(unsigned int num) {
    if(num<nbmat) return &matrices[num];
    else return NULL;
}

SVMatrix* SVStream::getMatrixFromSignature(SdifSignature sig) {
    for(unsigned int i = 0; i< nbmat; i++){
	if(matrices[i].getMatSignature() == sig)
	    return &matrices[i];
    }
    return NULL; 
}

SdifSignature SVStream::getFrameSignature() {
    return frameSign;
}

int SVStream::getNbFrames(){
    return nbFrames;
}


SdifFloat8 SVStream::getTimeBegin(){
    return begin;
}

SdifFloat8 SVStream::getTimeEnd(){
    return end;
}


