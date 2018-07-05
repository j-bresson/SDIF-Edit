/*
 *  GUI.cpp
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

color GUI::guiButtonColor;
color GUI::guiTextColor;
color GUI::guiSelectedTextColor;
color GUI::guiBgColor;
color GUI::guiBorderColor;

void* GUI::defaultFont;


void GUI::setButtonColor(color c){
    guiButtonColor = c;
}
void GUI::setTextColor(color c){
    guiTextColor = c;
}
void GUI::setSelectedTextColor(color c){
    guiSelectedTextColor = c;
}
void GUI::setBorderColor(color c){
    guiBorderColor = c;
}
void GUI::setPanelColor(color c){
    guiBgColor = c;
}

void GUI::setDefaultFont(void* f){
    defaultFont = f;
}

color GUI::getButtonColor(){
    return guiButtonColor;
}
color GUI::getTextColor(){
    return guiTextColor;
}
color GUI::getSelectedTextColor(){
    return guiSelectedTextColor;
}
color GUI::getPanelColor(){
    return guiBgColor;
}

color GUI::getBorderColor(){
    return guiBorderColor;
}

void* GUI::getDefaultFont(){
    return defaultFont;
}