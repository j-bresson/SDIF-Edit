/*
 *  SplineEditorWindow.h
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

#ifndef SPLINE_EDITOR_WINDOW_H
#define SPLINE_EDITOR_WINDOW_H

#include "include.h"

#include "Window.h"
#include "SplineSurface.h"
#include "GUI.h"	

class SplineEditorWindow : public Window {

private:
    
    SplineSurface* surf;
   
    int curveH, curveW;
    void* font;	

    GUIComponent *iLabel;
    GUIComponent *jLabel; 

    GUIComponent *curvei_1;
    GUIComponent *curvei_2;
    GUIComponent *curvei_3;
    GUIComponent *curvei_4;
    GUIComponent *curvej_1;
    GUIComponent *curvej_2;
    GUIComponent *curvej_3;
    GUIComponent *curvej_4;

    GUIComponent *ti;
    GUIComponent *ti_p;
    GUIComponent *ti_m;
    GUIComponent *tj;
    GUIComponent *tj_p;
    GUIComponent *tj_m;

    GUIComponent *resi;
    GUIComponent *resi_p;
    GUIComponent *resi_m;
    GUIComponent *resj;
    GUIComponent *resj_p;
    GUIComponent *resj_m;

    GUIComponent *exitButton;
    
    int ctrlPt;
    int mouseX, mouseY;

    void setComponents();
           
public:
    
    SplineEditorWindow(SplineSurface *s, int w = 400, int h = 200);
    ~SplineEditorWindow();
    
    void close();

    void draw();
    void reshape(int w, int h);
    void mousePressed(int mouse, int state, int x, int y);
    void mouseMoved(int x, int y);
    void keyPressed( unsigned char key, int x, int y ){};
    void specialPressed( int a_keys, int x, int y ){};
        
};

#endif
