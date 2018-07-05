/*
 *  SVStream.h
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

#ifndef SV_STREAM_H
#define SV_STREAM_H

#include "SVMatrix.h"
#include "Vector.h"

/*
 * Description of a stream in a SDIF File.
 * A stream is a succession of frames of the same type (same signature) and with the same stream ID.
 * So two diferent types of frame with the same streamID will be considered as two diferent streams. 
 * This class contains general informations about the stream (signature, ID, begin and end times,...) ,
 * and a vector with the matrix descriptions (SVMatrix) of all th matrices included in it.
 */
class SVStream {

private :
    SdifUInt4 idStream;		// Stream ID
    SdifSignature frameSign;	// Signature of the frames of the stream
    SdifFloat8 begin;		// Time of first frame of the stream
    SdifFloat8 end;		// Time of last frame of the stream
    int nbFrames;		// Number of frames in the stream
    
    SdifUInt4 nbmat;		// Number of matrices in the stream :
    Vector<SVMatrix> matrices;	// Vector containing the matrices
    
    // Adds a matrix in the stream
    void addMatrix(SVMatrix m);

    // Note: Matrices are defined in SDIF types definition
    // but some of them may not appear in frames of this stream.
    // Anyway they will be stored in SVStream with empty values.
    
public :
    
    // Creates a new stream description
    // and considers there's one only frame in it. 
    // The following frames will be notified with oneMoreFrame().
    // (so first frame doesn't need to be notified) 
    SVStream(SdifUInt4 idS, SdifSignature sig, SdifFloat8 time, SdifFrameTypeT *FrameType);
    
    SVStream() {};
    ~SVStream() {};
    
    // Notifies a new frame in the stream
    void oneMoreFrame(SdifFloat8 t);
    
    // Returns stream ID
    SdifUInt4 getId();
    // Returns the name of the frames from the stream
    char* getName();
    // Returns the signature of the frames from the stream
    SdifSignature getFrameSignature();
    // Returns number of matrix contained in each frame of the stream
    unsigned int getNbMat();
    // Returns nuber of frames in the stream
    int getNbFrames();
    // Returns time of the first frame of the stream
    SdifFloat8 getTimeBegin();
    // Returns time of the last frame of the stream
    SdifFloat8 getTimeEnd();
    
    // Returns matrix number num from the matrices vector (NULL if not found)
    SVMatrix* getMatrix(unsigned int num);
    // Returns matrix with signature sig (NULL if not found)
    SVMatrix* getMatrixFromSignature(SdifSignature sig);
    
};

#endif
