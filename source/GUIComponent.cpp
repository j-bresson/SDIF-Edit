/*
 *  GUIComponent.cpp
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

#include "GUI.h"


void GUIComponent::set(int X,int Y,int W,int H){
    x = X;
    y = Y;
    w = W;
    h = H;
}

bool GUIComponent::inside(int X,int Y){
    return (active && X>=x && X<=x+w && Y>=y && Y<=y+h);
}

int GUIComponent::getID(){
    return ID;
}

int GUIComponent::x1(){
    return x;
}
int GUIComponent::y1(){
    return y;
}

int GUIComponent::width(){
    return w;
}

int GUIComponent::height(){
    return h;
}

int GUIComponent::x2(){
    return x+w;
}

int GUIComponent::y2(){
    return y+h;
}

int GUIComponent::textPosX(){
    return x+5;
}

int GUIComponent::textPosY(){
    return y+h/2+3;
}

string GUIComponent::getText(){
    return text;
}

void GUIComponent::setText(string str){
    text = str;
}

void GUIComponent::setv2(float v){
    v2 = v;
}

void GUIComponent::setv1(float v){
    v1 = v;
}


void GUIComponent::writeText(){
    unsigned int c;
    glRasterPos2f(textPosX(), textPosY());
    for(c=0; c < text.length(); c++) {
	glutBitmapCharacter(font, text[c]);
    }
}


void GUIComponent::setActive(bool b){
    active = b;
}

void GUIComponent::setSelected(bool b){
    selected = b;
}

bool GUIComponent::isActive(){
    return active;
}


void GUIComponent::draw(){
    switch(type){
	case TYPE_BUTTON :
	    glColor3f(GUI::getButtonColor().r,GUI::getButtonColor().g,GUI::getButtonColor().b);
	    glBegin(GL_QUADS);
	    glVertex2d(x1(),y1());
	    glVertex2d(x1(),y2());
	    glVertex2d(x2(),y2());
	    glVertex2d(x2(),y1());
	    glEnd();
	    glColor3f(GUI::getBorderColor().r,GUI::getBorderColor().g,GUI::getBorderColor().b);
	    glBegin(GL_LINE_LOOP);
	    glVertex2d(x1(),y1());
	    glVertex2d(x1(),y2());
	    glVertex2d(x2(),y2());
	    glVertex2d(x2(),y1());
	    glEnd();
	    if(selected) glColor3f(GUI::getSelectedTextColor().r,GUI::getSelectedTextColor().g,GUI::getSelectedTextColor().b);
	    else glColor3f(GUI::getTextColor().r,GUI::getTextColor().g,GUI::getTextColor().b);
	    writeText();
	    break;
	case TYPE_PANEL :
	    glColor3f(GUI::getPanelColor().r,GUI::getPanelColor().g,GUI::getPanelColor().b);
	    glBegin(GL_QUADS);
	    glVertex2d(x1(),y1());
	    glVertex2d(x1(),y2());
	    glVertex2d(x2(),y2());
	    glVertex2d(x2(),y1());
	    glEnd();
	    break;
	case TYPE_LABEL :
	    if(selected) glColor3f(GUI::getSelectedTextColor().r,GUI::getSelectedTextColor().g,GUI::getSelectedTextColor().b);
	    else glColor3f(GUI::getTextColor().r,GUI::getTextColor().g,GUI::getTextColor().b);
	    writeText();
	    break;
	case TYPE_MOUSECTRL :
		glColor3f(GUI::getButtonColor().r,GUI::getButtonColor().g,GUI::getButtonColor().b);
		glBegin(GL_QUADS);
			glVertex2d(x1()+1+width()*v1,y1());
			glVertex2d(x1()+width()*v2,y1());
			glVertex2d(x1()+width()*v2,y2()-1);
			glVertex2d(x1()+1+width()*v1,y2()-1);
		glEnd();
		glColor3f(GUI::getTextColor().r,GUI::getTextColor().g,GUI::getTextColor().b);
		glBegin(GL_LINE_LOOP);
			glVertex2d(x1(),y1());
			glVertex2d(x2(),y1());
			glVertex2d(x2(),y2());
			glVertex2d(x1(),y2());
		glEnd();
		if(selected) glColor3f(GUI::getSelectedTextColor().r,GUI::getSelectedTextColor().g,GUI::getSelectedTextColor().b);
	    else glColor3f(GUI::getTextColor().r,GUI::getTextColor().g,GUI::getTextColor().b);
	    writeText();
		break;
	default : 
	    break;
    }

}


