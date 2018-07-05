/*
 *  SonagramWindow.cpp
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

#include "SonagramWindow.h"

SonagramWindow::SonagramWindow(SDIFViewerWindow *w){

    width = 500;
    height = 420;
    font = GLUT_BITMAP_HELVETICA_10;
    parent = w;
    	
    titleLabel = new GUIComponent(TYPE_BUTTON,0,"",GLUT_BITMAP_9_BY_15);
    exitButton = new GUIComponent(TYPE_BUTTON,0,"X",GLUT_BITMAP_8_BY_13);
    timeLabel = new GUIComponent(TYPE_LABEL,0,"time",font);
    timeVLabel = new GUIComponent(TYPE_LABEL,0,"",font);
    timeMinLabel = new GUIComponent(TYPE_LABEL,0,"",font);
    timeMaxLabel = new GUIComponent(TYPE_LABEL,0,"",font);
    xLabel = new GUIComponent(TYPE_BUTTON,0,"",font);
    xVLabel = new GUIComponent(TYPE_BUTTON,0,"",font);
    xMinLabel = new GUIComponent(TYPE_LABEL,0,"",font);
    xMaxLabel = new GUIComponent(TYPE_LABEL,0,"",font);
    sonagramme = new GUIComponent(TYPE_SPECIAL,0,"",NULL);

#if defined(GLUTWMCLOSEFUNC)
    exitButton->setActive(false);
#endif

    mouseDown = false;
    xValue = 0;
    tValue = 0;
    
    setComponents();
    windowTitle = "Sonagram";
    createWindow();
    glClearColor(GUI::getPanelColor().r,GUI::getPanelColor().g,GUI::getPanelColor().b,1.0);
    glutHideWindow();

}

void SonagramWindow::setComponents(){
    
    int sonaW = width-120;
    int sonaH = height-100;

    titleLabel->set(width/2,20,0,0);
    exitButton->set(width-20,4,15,15);
    
    sonagramme->set(100,40,sonaW,sonaH);

    timeLabel->set(100+sonaW/2,sonaH+60,0,0);
    timeVLabel->set(100+sonaW/2,sonaH+80,0,0);
    timeMinLabel->set(100,sonaH+60,0,0);
    timeMaxLabel->set(100+sonaW-30,sonaH+60,0,0);

    xLabel->set(10,40+sonaH/2,0,0);
    xVLabel->set(10,60+sonaH/2,0,0);
    xMaxLabel->set(30,50,0,0);
    xMinLabel->set(30,40+sonaH,0,0);

}

SonagramWindow::~SonagramWindow(){

    glutDestroyWindow(ID);
    
    delete titleLabel;
    delete exitButton;
    delete timeLabel;
    delete timeVLabel;
    delete timeMinLabel;
    delete timeMaxLabel;
    delete xLabel;
    delete xVLabel;
    delete xMinLabel;
    delete xMaxLabel;
    delete sonagramme;
    
}

void SonagramWindow::reshape(int w, int h){
    width = w;
    height = h;
    setComponents();
}

void SonagramWindow::mousePressed(int mouse, int state, int x,int y){
    if(state == GLUT_DOWN){
	if(exitButton->inside(x,y)){
	    close();
	} else if(titleLabel->inside(x,y)){
	    changeMainParam();
	} else if(xLabel->inside(x,y)){
	    changeXParam();
	} else if(sonagramme->inside(x,y)){
	    calculateValues(x,y);
	    mouseDown = true;
	}
    } else if(state = GLUT_UP){
	mouseDown = false;
    }
    glutPostRedisplay();
}


void SonagramWindow::mouseMoved(int x,int y){
    
    if(sonagramme->inside(x,y)){
	calculateValues(x,y);
	glutPostRedisplay();
    }
    
}

void SonagramWindow::close(){
    hideWindow();
}

void SonagramWindow::changeMainParam(){

}

void SonagramWindow::changeXParam(){

}

void SonagramWindow::calculateValues(int x, int y){
    tValue = ((x-sonagramme->x1())/(float)(sonagramme->width()) )*(parent->maxTime()-parent->minTime()) + parent->minTime(); 
    xValue = ((sonagramme->y2()-y)/(float)(sonagramme->height()) )*(parent->xmax-parent->xmin) + parent->xmin;
}

void SonagramWindow::draw(){
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
    
    int f = parent->selectedField;
    if(f>0) { // <==> some values in the viewer
	int xp = parent->xParam;
	titleLabel->setText(parent->fieldNames[f]);
	titleLabel->draw();
		
	glColor3f(0,0,0);
	glBegin(GL_QUADS);
	glVertex2d(sonagramme->x1(),sonagramme->y1());
	glVertex2d(sonagramme->x1(),sonagramme->y2());
	glVertex2d(sonagramme->x2(),sonagramme->y2());
	glVertex2d(sonagramme->x2(),sonagramme->y1());
	glEnd();
	
	float ox = sonagramme->x1();
	float oy = sonagramme->y2();
	float minT = parent->minTime();
	float maxT = parent->maxTime();
	float cx = sonagramme->width()/(float)(maxT-minT);
	float cy = sonagramme->height()/(float)(parent->xmax-parent->xmin);
	float v;
	if(parent->max==parent->min) v = 0.5;
	
	if(parent->SDIF_DATA == PARTIAL_DATA){
	
	    for(int part = parent->beginT; part<=parent->endT; part++){
		float time = parent->data[part].time;
		int nbPts = parent->data[part].dimension-1;
		float xRef = parent->data[part].values[0][parent->xParam];
		float valRef = parent->data[part].values[0][parent->selectedField];
		float onset,deltaX,deltaV;
		int pt;
		float v;
		glBegin(GL_LINE_STRIP);
		for(pt = 0; pt<nbPts; pt++){
		    onset = parent->data[part].values[pt+1][1];
		    deltaX = parent->data[part].values[pt+1][parent->xParam];
		    deltaV = parent->data[part].values[pt+1][parent->selectedField];
		    if(parent->max!=parent->min) v = (valRef+deltaV-parent->min)/
						    (parent->max-parent->min);
		    if(v<=parent->lowTh) glColor3f(parent->minColor.r,parent->minColor.g,parent->minColor.b);
		    else if(v>=parent->highTh) glColor3f(parent->maxColor.r,parent->maxColor.g,parent->maxColor.b);
		    else {
			v = (v-parent->lowTh)/(parent->highTh-parent->lowTh);
			glColor3f((1-v)*parent->minColor.r+v*parent->maxColor.r, 
				    (1-v)*parent->minColor.g+v*parent->maxColor.g, 
				    (1-v)*parent->minColor.b+v*parent->maxColor.b);
		    }
		    glVertex2d(ox+(time+onset-minT)*cx,oy-(xRef+deltaX-parent->xmin)*cy);
		}
		glEnd();
	    }

	} else {
    
	for(int t = parent->beginT; t<parent->endT; t++){
	
	    glBegin(GL_QUAD_STRIP);
	    glColor3f(parent->bgColor.r,parent->bgColor.g,parent->bgColor.b);
	    glVertex2d(ox+(parent->data[t].time-parent->data[parent->beginT].time)*cx,oy);
	    glVertex2d(ox+(parent->data[t+1].time-parent->data[parent->beginT].time)*cx,oy);
	    
	    if(parent->data[t+1].dimension>0){
		for(int e=0; e<parent->data[t].dimension; e++){
		    if(parent->max != parent->min) v = (parent->data[t].values[e][f]-parent->min)/(parent->max-parent->min);
		    if(v<=parent->lowTh) glColor3f(parent->minColor.r,parent->minColor.g,parent->minColor.b);
		    else if(v>=parent->highTh) glColor3f(parent->maxColor.r,parent->maxColor.g,parent->maxColor.b);
		    else {
			v = (v-parent->lowTh)/(parent->highTh-parent->lowTh);
			glColor3f((1-v)*parent->minColor.r+v*parent->maxColor.r,
				    (1-v)*parent->minColor.g+v*parent->maxColor.g,
				    (1-v)*parent->minColor.b+v*parent->maxColor.b);
		    }
		    glVertex2d(ox+(parent->data[t].time-parent->data[parent->beginT].time)*cx,
				oy-(parent->data[t].values[e][xp]-parent->xmin)*cy);
		    int e2;
		    if(e<parent->data[t+1].dimension) e2 = e;
		    else e2 = parent->data[t+1].dimension-1;
		    if(parent->max != parent->min) v = (parent->data[t+1].values[e2][f]-parent->min)/(parent->max-parent->min);
		    if(v<=parent->lowTh) glColor3f(parent->minColor.r,parent->minColor.g,parent->minColor.b);
		    else if(v>=parent->highTh) glColor3f(parent->maxColor.r,parent->maxColor.g,parent->maxColor.b);
		    else {
			v = (v-parent->lowTh)/(parent->highTh-parent->lowTh);
			glColor3f((1-v)*parent->minColor.r+v*parent->maxColor.r,
				    (1-v)*parent->minColor.g+v*parent->maxColor.g,
				    (1-v)*parent->minColor.b+v*parent->maxColor.b);
		    }
		    glVertex2d(ox+(parent->data[t+1].time-parent->data[parent->beginT].time)*cx,
				oy-(parent->data[t+1].values[e2][xp]-parent->xmin)*cy);
		}
	    }
	    glColor3f(parent->bgColor.r,parent->bgColor.g,parent->bgColor.b);
	    glVertex2d(ox+(parent->data[t].time-parent->data[parent->beginT].time)*cx,oy-sonagramme->height());
	    glVertex2d(ox+(parent->data[t+1].time-parent->data[parent->beginT].time)*cx,oy-sonagramme->height());
	    glEnd();
	}
    
	//glVertex2d(ox+(parent->data[i].time-parent->data[0].time)*cx,oy-(parent->data[i].values[e][f]-parent->min)*cy);
	
	}
	
	
	char str[24];
	timeLabel->draw();
	if(mouseDown){
	    sprintf(str,"[%.4f]",tValue);
	    timeVLabel->setText(str);
	    timeVLabel->draw();
	}
	sprintf(str,"%.4f",parent->data[parent->beginT].time);
	timeMinLabel->setText(str);
	timeMinLabel->draw();
	sprintf(str,"%.4f",parent->data[parent->endT].time);
	timeMaxLabel->setText(str);
	timeMaxLabel->draw();
	xLabel->setText(parent->fieldNames[xp]);
	xLabel->draw();
	
	
	if(mouseDown){
	    if(parent->fieldPrintFormat(parent->xParam)==MC_FORMAT)
		sprintf(str,"[%d mc]",SDIFViewerWindow::FreqToMidicents(xValue));
	    else
		sprintf(str,"[%.4f]",xValue);
	    xVLabel->setText(str);
	    xVLabel->draw();
	}
	if(parent->fieldPrintFormat(parent->xParam)==MC_FORMAT)
	    sprintf(str,"%d mc",SDIFViewerWindow::FreqToMidicents(parent->xmin));
	else
	    sprintf(str,"%.4f",parent->xmin);
	xMinLabel->setText(str);
	xMinLabel->draw();
	if(parent->fieldPrintFormat(parent->xParam)==MC_FORMAT)
	    sprintf(str,"%d mc",SDIFViewerWindow::FreqToMidicents(parent->xmax));
	else
	    sprintf(str,"%.4f",parent->xmax);
	xMaxLabel->setText(str);
	xMaxLabel->draw();
	
		
    }	
    	
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glutSwapBuffers();
}
