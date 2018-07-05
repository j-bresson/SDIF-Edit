/*
 *  GUIComponent.h
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
#ifndef GUI_COMPONENT_H
#define GUI_COMPONENT_H

#include "include.h"
#include "GUI.h"


#define TYPE_BUTTON 1
#define TYPE_LABEL 2
#define TYPE_PANEL 3
#define TYPE_MOUSECTRL 4
//#define TYPE_SPECIAL 5


class GUIComponent {

    private:
	int x;
	int y;
	int w;
	int h;
	int ID;
	int type;
	string text;
	
	void* font;
	bool active; 
	bool selected; 
	float v1, v2;
		
    public:

	
	static bool select;

	GUIComponent(int type, int n, string str):type(type),ID(n),text(str){
	    x = y = w = h = 0;
		v1 = v2 = 0;
	    active = true;
		selected = false;
	    font = GUI::getDefaultFont();
	};
	
	GUIComponent(int type, int n, string str, void * f):type(type),ID(n),text(str),font(f){
	    x = y = w = h = 0;
		v1 = v2 = 0;
		selected = false;
	    active = true;
	};
	
	GUIComponent(){};
	virtual ~GUIComponent(){};
	
	void set(int X,int Y,int W,int H);
	
	bool inside(int X,int Y);

	int getID();
	int x1();
	int y1();
	int width();
	int height();
	int x2();
	int y2();
	int textPosX();
	int textPosY();
	string getText();
	void setActive(bool b);
	void setSelected(bool b);
	bool isActive();
	void setText(string str);
	void setv1(float v);
	void setv2(float v);
	
	    
	void writeText();
	
	void draw();
};

#endif


