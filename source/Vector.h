/*  Vector.h
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

#ifndef VECTOR_H
#define VECTOR_H

#include <cassert>
#include <iostream>

using namespace std;


template <class T> class Vector {
    int vsize; // number of elements stored
    int asize; // size of the array
    T *array;

    // Invariant: vsize <= asize

public:
    
    Vector() : vsize(0), asize(1), array(new T[asize]) {}
    
    virtual ~Vector() { delete[] array; }
    
    Vector(const Vector<T> & other) :
	vsize(other.vsize), asize(other.asize),
	array(new T[asize]) {
	for (int i = 0; i < vsize; i++)
            array[i] = other.array[i];
    }

    
    inline int size() const { return vsize; }

    T & operator[](int i) {
	assert(i <= size());
	return array[i];
    }

    void pop_back() { vsize--; }

    void push_back(T x) {
        //cout << "push A = " << asize << "  V = " << vsize << endl;
	if (vsize == asize) {
            asize *= 2;
            T *new_array = new T[asize];
            for (int i = 0; i < vsize; i++)
                new_array[i] = array[i];
            delete[] array;
            array = new_array;
        }
	array[vsize] = x;
	vsize++;
	//cout << "pushed A = " << asize << "  V = " << vsize << endl;
    }
    
    // Attention experimentation..
    void remove(int n) {
	//cout << "remove A = " << asize << "  V = " << vsize << endl;
	
		int i;
		
	if(n<vsize){
	    if(vsize==1){
		vsize = 0;
		asize = 1;
		array = new T[asize];
	    } else {
		//asize--;
		T *new_array = new T[asize];
		for (i = 0; i < n ; i++)
		    new_array[i] = array[i];
		for (i = n+1; i < vsize ; i++)
		    new_array[i-1] = array[i];
		delete[] array;
		array = new_array;
		vsize--;
	    }
	} else {
	    cout << n << " is out of array" << endl;
	}
	//cout << "removed A = " << asize << "  V = " << vsize << endl;
    }
    
    Vector<T> & operator=(const Vector<T> & other) {
	if (&other != this) {
            vsize = other.vsize;
            if (asize < vsize) {
                delete[] array;
                asize = other.asize;
                array = new T[asize];
            }
            for (int i = 0; i < vsize; i++)
                array[i] = other.array[i];
        }
        return *this;
    }
};

#endif
