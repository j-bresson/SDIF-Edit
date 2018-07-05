/*
 *  SliceViewWindow.h
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

#ifndef SLICE_VIEW_WINDOW_H
#define SLICE_VIEW_WINDOW_H

#define MOUSE_OFF 0
#define MOUSE_X 1
#define MOUSE_TIME 2

#include "include.h"
#include "Window.h"
#include "GUI.h"

class SDIFViewerWindow;

class SliceViewWindow : public Window {
    private:
    
	void *font;
	int clic;
	
	int *xPts;
	int *tPts;
	
	SDIFViewerWindow *parent;

	GUIComponent *titleLabel;
	GUIComponent *exitButton;
	GUIComponent *timeSlicePanel;
	GUIComponent *xSlicePanel;
	GUIComponent *timeLabel;
	GUIComponent *timeMinLabel;
	GUIComponent *timeMaxLabel;
	GUIComponent *xLabel;
	GUIComponent *xMinLabel;
	GUIComponent *xMaxLabel;

	void setComponents();
    public:
    
	SliceViewWindow(SDIFViewerWindow *w);
	~SliceViewWindow();
	
	void close();
	void timeEdit(int x,int y);
	void xEdit(int x,int y);
	void draw();
	void reshape(int w, int h);
	void mousePressed(int mouse, int state, int x, int y);
	void mouseMoved(int x, int y);
	void keyPressed( unsigned char key, int x, int y ){};
	void specialPressed( int a_keys, int x, int y );
};


#include "SDIFViewerWindow.h"

#endif

