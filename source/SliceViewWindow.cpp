/*
 *  SliceViewWindow.cpp
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

#include "SliceViewWindow.h"


SliceViewWindow::SliceViewWindow(SDIFViewerWindow *w){
    
    width = 500;
    height = 400;
    font = GLUT_BITMAP_HELVETICA_10;
    
    parent = w;
    clic = MOUSE_OFF;
			             
    titleLabel = new GUIComponent(TYPE_LABEL,0,"",GLUT_BITMAP_9_BY_15);
    exitButton = new GUIComponent(TYPE_BUTTON,0,"X",GLUT_BITMAP_8_BY_13);
    timeSlicePanel = new GUIComponent(TYPE_SPECIAL,0,"",NULL);
    xSlicePanel = new GUIComponent(TYPE_SPECIAL,0,"",NULL);
    timeLabel = new GUIComponent(TYPE_LABEL,0,"time",font);
    timeMinLabel = new GUIComponent(TYPE_LABEL,0,"",font);
    timeMaxLabel = new GUIComponent(TYPE_LABEL,0,"",font);
    xLabel = new GUIComponent(TYPE_LABEL,0,"",font);
    xMinLabel = new GUIComponent(TYPE_LABEL,0,"",font);
    xMaxLabel = new GUIComponent(TYPE_LABEL,0,"",font);

#if defined(GLUTWMCLOSEFUNC)
    exitButton->setActive(false);
#endif
    
    setComponents();

    windowTitle = "2D Views";
    createWindow();
    glClearColor(GUI::getPanelColor().r,GUI::getPanelColor().g,GUI::getPanelColor().b,1.0);
    glutHideWindow();
    
}


void SliceViewWindow::setComponents(){
    titleLabel->set(10,20,0,0);
    exitButton->set(width-20,4,15,15);
    
    int panelH = (height-120)/2;
    int panelW = width - 80;
    timeSlicePanel->set(20,40,panelW,panelH);
    xSlicePanel->set(20,80+panelH,panelW,panelH);

    timeLabel->set(panelW/2,panelH+50,0,0);
    timeMinLabel->set(20,panelH+50,0,0);
    timeMaxLabel->set(panelW,panelH+50,0,0);
    
    xLabel->set(panelW/2,2*panelH+90,0,0);
    xMinLabel->set(20,2*panelH+90,0,0);
    xMaxLabel->set(panelW,2*panelH+90,0,0);
    
    
}


SliceViewWindow::~SliceViewWindow(){
    
    glutDestroyWindow(ID);
    
    delete titleLabel;
    delete exitButton;
    delete timeSlicePanel;
    delete xSlicePanel;
    delete timeLabel;
    delete timeMinLabel;
    delete timeMaxLabel;
    delete xLabel;
    delete xMinLabel;
    delete xMaxLabel;    

    
}



void SliceViewWindow::reshape(int w, int h){
    width = w;
    height = h;
    setComponents();
}

void SliceViewWindow::specialPressed( int a_keys, int x, int y ){
    int m = glutGetModifiers();
    switch ( a_keys ) {
	case GLUT_KEY_UP:
	    if(m == GLUT_ACTIVE_CTRL) parent->selectedPtFwd();
	    break;
	case GLUT_KEY_DOWN:
	    if(m == GLUT_ACTIVE_CTRL) parent->selectedPtBwd();
	    break;
	case GLUT_KEY_LEFT:
	    if(m == GLUT_ACTIVE_CTRL) parent->selectedPtL();
	    break;
	case GLUT_KEY_RIGHT:
	    if(m == GLUT_ACTIVE_CTRL) parent->selectedPtR();
	    break;
	case GLUT_KEY_HOME:
	    if(parent->selectedField-1 <= 0) parent->selectField(parent->field);
	    else parent->selectField(parent->selectedField - 1);
	    break;
	case GLUT_KEY_END:
	    parent->selectField((parent->selectedField)%(parent->field) + 1);
	    break;
	default:
	    break;
    }
    glutPostRedisplay();
}

void SliceViewWindow::mousePressed(int mouse, int state, int x,int y){
    if(state == GLUT_DOWN){
	if(xSlicePanel->inside(x,y)){
	    clic = MOUSE_X;
	    xEdit(x-xSlicePanel->x1(),xSlicePanel->y2()-y);
	}
	else if(timeSlicePanel->inside(x,y)){
	    clic = MOUSE_TIME;
	    timeEdit(x-timeSlicePanel->x1(),timeSlicePanel->y2()-y);
	}
	if(exitButton->inside(x,y)){
	    close();
	}
    } else if(state == GLUT_UP){
	clic = MOUSE_OFF;
    }
    glutPostRedisplay();
}

void SliceViewWindow::mouseMoved(int x, int y){
    if(xSlicePanel->inside(x,y) && clic==MOUSE_X) xEdit(x-xSlicePanel->x1(),xSlicePanel->y2()-y);
    else if(timeSlicePanel->inside(x,y) && clic==MOUSE_TIME) timeEdit(x-timeSlicePanel->x1(),timeSlicePanel->y2()-y);
    glutPostRedisplay();
}

void SliceViewWindow::timeEdit(int x,int y){
    float dyn = parent->max - parent->min;
    float val = parent->min + (y/(float) timeSlicePanel->height()) * dyn;
    int i = 0;
    while(i<parent->tps && tPts[i]!=x) i++;
    while(i<parent->tps && tPts[i]==x){
	parent->data[i].values[parent->selectedElem][parent->selectedField] = val;
	i++;
    }
    parent->dataModified = true;
}

void SliceViewWindow::xEdit(int x,int y){

    float dyn = parent->max - parent->min;
    int t = parent->selectedT;
    float val = parent->min + (y/(float)xSlicePanel->height()) * dyn;
    int i = 0;
    while(i<parent->data[t].dimension && xPts[i]!=x) i++;
    while(i<parent->data[t].dimension && xPts[i]==x){
	parent->data[t].values[i][parent->selectedField] = val;
	i++;
    }
     parent->dataModified = true;
}

void SliceViewWindow::close(){
    hideWindow();
}

void SliceViewWindow::draw(){
    
    glutSetWindow(ID);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,width,height);
    // TEXTE :
    // switch to projection mode
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    orthoProj();
       
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        
    if(exitButton->isActive()) exitButton->draw();
    
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
    glVertex2d(timeSlicePanel->x1(),timeSlicePanel->y1());
    glVertex2d(timeSlicePanel->x1(),timeSlicePanel->y2());
    glVertex2d(timeSlicePanel->x2(),timeSlicePanel->y2());
    glVertex2d(timeSlicePanel->x2(),timeSlicePanel->y1());
    
    glVertex2d(xSlicePanel->x1(),xSlicePanel->y1());
    glVertex2d(xSlicePanel->x1(),xSlicePanel->y2());
    glVertex2d(xSlicePanel->x2(),xSlicePanel->y2());
    glVertex2d(xSlicePanel->x2(),xSlicePanel->y1());
    glEnd();

    int f = parent->selectedField;
    if(f>0) { // <==> some values in the viewer
	
	int t = parent->selectedT;
	int e = parent->selectedElem;
	int xp = parent->xParam;
	tPts = new int[parent->tps];
	xPts = new int[parent->data[t].dimension];
	
	titleLabel->setText(parent->fieldNames[f]);
	titleLabel->draw();
	
	char str[128];
	timeLabel->draw();
	sprintf(str,"%.4f",parent->data[0].time);
	timeMinLabel->setText(str);
	timeMinLabel->draw();
	sprintf(str,"%.4f",parent->data[parent->tps-1].time);
	timeMaxLabel->setText(str);
	timeMaxLabel->draw();
	
	xLabel->setText(parent->fieldNames[xp]);
	xLabel->draw();
	sprintf(str,"%.4f",parent->data[t].values[0][xp]);
	xMinLabel->setText(str);
	xMinLabel->draw();
	if(parent->data[t].dimension > 0) 
	    sprintf(str,"%.4f",parent->data[t].values[parent->data[t].dimension-1][xp]);
	xMaxLabel->setText(str);
	xMaxLabel->draw();
	
	// Curves
	float dyn = parent->max-parent->min;
	
	glColor3f(0,0,0);
	int i;
	int ox = timeSlicePanel->x1();
	int oy = timeSlicePanel->y2();
	float cx = timeSlicePanel->width()/(float) (parent->data[parent->tps-1].time-parent->data[0].time);
	float cy = 1;
	if(dyn != 0) cy = timeSlicePanel->height()/dyn;
	glBegin(GL_LINE_STRIP);
	for(i = 0; i<parent->tps; i++){
	    if(e<parent->data[i].dimension){
		glVertex2d(ox+(parent->data[i].time-parent->data[0].time)*cx,oy-(parent->data[i].values[e][f]-parent->min)*cy);
		tPts[i] = (int) ((parent->data[i].time-parent->data[0].time)*cx);
	    }
	}
	glEnd();
	glColor3f(0.5,0.2,0.2);
	glBegin(GL_LINE_LOOP);
	glVertex2d(ox+(parent->data[t].time-parent->data[0].time)*cx-2,oy-(parent->data[t].values[e][f]-parent->min)*cy-2);
	glVertex2d(ox+(parent->data[t].time-parent->data[0].time)*cx+2,oy-(parent->data[t].values[e][f]-parent->min)*cy-2);
	glVertex2d(ox+(parent->data[t].time-parent->data[0].time)*cx+2,oy-(parent->data[t].values[e][f]-parent->min)*cy+2);
	glVertex2d(ox+(parent->data[t].time-parent->data[0].time)*cx-2,oy-(parent->data[t].values[e][f]-parent->min)*cy+2);
	glEnd();
	
	
	ox = xSlicePanel->x1();
	oy = xSlicePanel->y2();
	if(parent->data[t].dimension > 0) 
	cx = xSlicePanel->width()/
	    (float) (parent->data[t].values[parent->data[t].dimension-1][xp]-parent->data[t].values[0][xp]);
	else cx = 1;
	if(dyn != 0) cy = xSlicePanel->height()/dyn;
	glColor3f(0,0,0);
	glBegin(GL_LINE_STRIP);
	for(i = 0; i<parent->data[t].dimension; i++){
	    	glVertex2d(ox+(parent->data[t].values[i][xp]-parent->data[t].values[0][xp])*cx,oy-(parent->data[t].values[i][f]-parent->min)*cy);
		xPts[i] = (int) ((parent->data[t].values[i][xp]-parent->data[t].values[0][xp])*cx);
	}
	glEnd();
	glColor3f(0.5,0.2,0.2);
	glBegin(GL_LINE_LOOP);
	glVertex2d(ox+(parent->data[t].values[e][xp]-parent->data[t].values[0][xp])*cx-2,oy-(parent->data[t].values[e][f]-parent->min)*cy-2);
	glVertex2d(ox+(parent->data[t].values[e][xp]-parent->data[t].values[0][xp])*cx+2,oy-(parent->data[t].values[e][f]-parent->min)*cy-2);
	glVertex2d(ox+(parent->data[t].values[e][xp]-parent->data[t].values[0][xp])*cx+2,oy-(parent->data[t].values[e][f]-parent->min)*cy+2);
	glVertex2d(ox+(parent->data[t].values[e][xp]-parent->data[t].values[0][xp])*cx-2,oy-(parent->data[t].values[e][f]-parent->min)*cy+2);
	glEnd();
    }
    
    glPopMatrix();
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    
    glutSwapBuffers();
    
}

