/*
 *  GUI.h
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

#ifndef _GUI_H_
#define _GUI_H_

#include "include.h"

#define TYPE_BUTTON 1
#define TYPE_LABEL 2
#define TYPE_PANEL 3
#define TYPE_SPECIAL 4

#include "include.h"


/*
#include "Button.h"
#include "Panel.h"
#include "Label.h"
#include "Group.h"
#include "MouseController.h"
*/


class GUI {

private:
    static color guiTextColor;
    static color guiSelectedTextColor;
    static color guiButtonColor;
    static color guiBgColor;
    static color guiBorderColor;
    static void* defaultFont;

    public:
	static void setButtonColor(color c);
    static void setPanelColor(color c);
    static void setTextColor(color c);
    static void setSelectedTextColor(color c);
    static void setBorderColor(color c);
    static void setDefaultFont(void* font);
    static color getButtonColor();
    static color getPanelColor();
    static color getTextColor();
    static color getBorderColor();
    static color getSelectedTextColor();
    static void* getDefaultFont();
    };


#include "GUIComponent.h"

/*
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

public:
	static bool select;
    
	GUIComponent(int n, string str):ID(n),text(str){
		x = y = w = h = 0;
		active = true;
		font = GUI::getDefaultFont();
    };
	
    GUIComponent(int n, string str, void * f):ID(n),text(str),font(f){
		x = y = w = h = 0;
		active = true;
    };

    GUIComponent(){};
    
    virtual ~GUIComponent();
    
    void set(int X,int Y,int W,int H);
    bool inside(int X,int Y);
    int getId();
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
    bool isActive();
    void setText(string str);
    void writeText();

    virtual void draw() {};

};
*/

/*

class Button : public GUIComponent {

    private:
    
    public:

    Button(int n, string str){
		super(n,str);
    };
    
    Button(int n, string str, void * f){
		super(n,str,f);
    };
    
	Button(){};
    
    ~Button();
	
	void draw(){};
};


class Panel : public GUIComponent {
    private:
    
    public:
    
    
    Panel(int n, string str){
	//super(n,str);
    };
    
    Panel(int n, string str, void * f){
	//this(n,str,f);
    };
    
    void draw();
};


class Label : public GUIComponent {
    private:
    
    public:
    
    
    Label(int n, string str){
	//super(n,str);
    };
    
    Label(int n, string str, void * f){
	//super(n,str,f);
    };
    
    
    void draw();
};

class Group {
    private:
    public:
};

class MouseControler : public GUIComponent {
    private:
    public:
};
*/


#endif
