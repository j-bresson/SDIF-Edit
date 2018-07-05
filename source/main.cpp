
/*
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


#include "EventManager.h"
#include "Preferences.h"
#include "GUI.h"

using namespace std;

int main (int argc, char** argv ){
    
    EventManager em;
	SdifGenInitCond("");
	
    //int preferenceFile = Preferences::readPreferencesFile();
    //if(preferenceFile == -1){
		// cout << "DEFAULT PREFS" << endl;
		// No preference file found
		color textColor = {0.84,0.82,0.54};
		color sTextColor = {1.0,1.0,1.0};
		color paneColor = {0.22,0.28,0.24};
		color buttonColor = {0.37,0.38,0.31};
		color borderColor = { 0.28,0.24,0.22};
		GUI::setButtonColor(buttonColor);
		GUI::setPanelColor(paneColor);
		GUI::setTextColor(textColor);
		GUI::setSelectedTextColor(sTextColor);
		GUI::setBorderColor(borderColor);
    /*} else {
		GUI::setButtonColor(Preferences::color1);
		GUI::setPanelColor(Preferences::color2);
		GUI::setTextColor(Preferences::color3);
		GUI::setSelectedTextColor(Preferences::color4);
		GUI::setBorderColor(Preferences::color5);
    }*/

    Window::start(&argc, argv);
    
   	cout << "SDIF-EDIT 1.5" << endl;
    int id;
    if(argc == 2)
    {
		id = EventManager::openSdifFile(argv[1]);
    // } else if(argc == 3){ id = EventManager::openSdifFileWithAudio(argv[1],argv[2]);
    } else {
		
		cout << "Please supply SDIF file as argument (/path-to-file/filename.sdif" << endl ; //<<  "arg 2:  volume:path:audioFile.aiff" << endl;
	    return -1;
	
	}
    	
    int ret = Window::mainLoop();  

}
