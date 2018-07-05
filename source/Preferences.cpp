/*
 *  Preferences.cpp
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

#include "Preferences.h"

#include <fstream>

using namespace std;

color Preferences::color1;
color Preferences::color2;
color Preferences::color3;
color Preferences::color4;
color Preferences::color5;

int Preferences::readPreferencesFile(){

    /* test output
	ofstream outfile;
    char* fileName="./TTTEEESSSTTT.test";
    outfile.open(fileName, ofstream::out);
    outfile << "eeeeeeeeeeeeeeeeeeeeeeeeeeyyyyyyaaaaaa";
    outfile << endl;
    outfile.close();
	*/
	
	ifstream infile;
    const char* prefFileName = "./SdifEditPreferences.prf";
    char line[1024];
    infile.open(prefFileName, ifstream::in);
    if(infile.is_open()){
	// skip 2 premières lignes
	infile.getline(line,1024);
	infile.getline(line,1024);
	infile.getline(line,1024);
	sscanf(line,"%f %f %f",&color1.r,&color1.g,&color1.b);
	infile.getline(line,1024);
	sscanf(line,"%f %f %f",&color2.r,&color2.g,&color2.b);
	infile.getline(line,1024);
	sscanf(line,"%f %f %f",&color3.r,&color3.g,&color3.b);
	infile.getline(line,1024);
	sscanf(line,"%f %f %f",&color4.r,&color4.g,&color4.b);
	infile.getline(line,1024);
	sscanf(line,"%f %f %f",&color5.r,&color5.g,&color5.b);
	infile.close();
	return 0;
    } else {
	cout << "No preference file found" << endl;
	infile.close();
	return -1;
    }
}


