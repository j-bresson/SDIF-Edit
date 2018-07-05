/*
 *  ViewPointsWindow.h
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

#ifndef VIEW_POINTS_WINDOW_H
#define VIEW_POINTS_WINDOW_H

#include "include.h"
#include "Window.h"
#include "GUI.h"

class SDIFViewerWindow;

class ViewPointsWindow : public Window {

private:
    
    void *font;
    SDIFViewerWindow *parent;
	int nbVP;
	int selectedVP;
	//int vpListPtr;
	//int vpListSize;
	
	GUIComponent *exitButton;
	GUIComponent *vpLabel;
	GUIComponent *noVpLabel;
	GUIComponent **viewPointsButtons;
	GUIComponent *goButton;
	GUIComponent *removeButton;
	GUIComponent *saveButton;
	GUIComponent *nameLabel;
	GUIComponent *inputLabel;
	GUIComponent *okButton;
	GUIComponent *cancelButton;
	
	bool saving;
	string typedName;
	
	void setComponents();
	void removeVP(int vp);
	void goToVP(int vp);
	void activeSave();
	void desactiveSave();
	void saveVP();
	
	
public:
	
	ViewPointsWindow(SDIFViewerWindow *w);
	~ViewPointsWindow();
	
	void updateViewPoints();
	
	void close();
	
	void draw();
	void reshape(int w, int h);
	void mousePressed(int mouse, int state, int x, int y);
	void mouseMoved(int x, int y){};
	void keyPressed( unsigned char key, int x, int y );
	void specialPressed( int a_keys, int x, int y ){};
};

#include "SDIFViewerWindow.h"
#endif
