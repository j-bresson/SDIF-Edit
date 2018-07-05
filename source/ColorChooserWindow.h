/*
 *  ColorChooserWindow.h
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

#ifndef COLOR_CHOOSER_WINDOW_H
#define COLOR_CHOOSER_WINDOW_H

#define CLICK_NONE 0
#define CLICK_LOW_TH 1
#define CLICK_HIGH_TH 2


#include "include.h"

#include "Window.h"
#include "GUI.h"

class SDIFViewerWindow;

class ColorChooserWindow : public Window {
    
    private:
	
	SDIFViewerWindow *parent;
	
	int mouseX, mouseY;
	int nbColors;
	int steps;
	int selectedColor;
	int colorField;
	
	void *font;
	int click;
	
	color* palette;

	color *minColor;
	color *maxColor;
	color *bgColor;
	color tmpMinColor;
	color tmpMaxColor;
	color tmpBgColor;
	
	float *lowTh;
	float *highTh;
	float tmpLow;
	float tmpHigh;
	
	GUIComponent **colorButtons;

	GUIComponent *selectedBgColor;
        GUIComponent *selectedMinColor;
        GUIComponent *selectedMaxColor;

	GUIComponent *bgLabel;
        GUIComponent *minLabel;
        GUIComponent *maxLabel;	

	GUIComponent *colorPreview;

	GUIComponent *applyButton;
	GUIComponent *exitButton;

	GUIComponent *chooseParamLabel;	
	GUIComponent **paramLabels;
	GUIComponent **paramButtons;
		
	void setComponents();
        void selectAction(int x,int y);

    public:

	ColorChooserWindow(color *bg, color *min, color *max, float *low, float *high, SDIFViewerWindow *sv);
	~ColorChooserWindow();
        
	void update();
	void reset();
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

