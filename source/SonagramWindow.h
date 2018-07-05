/*
 *  Sonagram.h
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

#ifndef SONAGRAM_WINDOW_H
#define SONAGRAM_WINDOW_H

#include "include.h"
#include "Window.h"
#include "GUI.h"

class SDIFViewerWindow;

class SonagramWindow : public Window {

private:

    void *font;
	SDIFViewerWindow *parent;
	
	GUIComponent *titleLabel;
	GUIComponent *exitButton;
	GUIComponent * timeLabel;
	GUIComponent * timeVLabel;
	GUIComponent * timeMinLabel;
	GUIComponent * timeMaxLabel;
	GUIComponent * xLabel;
	GUIComponent * xVLabel;
	GUIComponent * xMinLabel;
	GUIComponent * xMaxLabel;
	GUIComponent * sonagramme;
	
	int mainParam;
	int xParam;
	float tValue;
	float xValue;
	bool mouseDown;
	
	void setComponents();
	void calculateValues(int x,int y);
	void changeMainParam();
	void changeXParam();

public:
	
	SonagramWindow(SDIFViewerWindow *w);
	~SonagramWindow();
	
	void close();
	
	void draw();
	void reshape(int w, int h);
	void mousePressed(int mouse, int state, int x, int y);
	void mouseMoved(int x, int y);
	void keyPressed( unsigned char key, int x, int y ){};
	void specialPressed( int a_keys, int x, int y ){};

};

#include "SDIFViewerWindow.h"
#endif
