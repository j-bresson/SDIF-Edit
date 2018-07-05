/*
 *  ViewPointsWindow.cpp
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

#include "ViewPointsWindow.h"

ViewPointsWindow::ViewPointsWindow(SDIFViewerWindow *w){
    width = 200;
    height = 600;
    font = GLUT_BITMAP_HELVETICA_10;
    parent = w;
    
    exitButton = new GUIComponent(TYPE_BUTTON,0,"X",GLUT_BITMAP_8_BY_13);
    vpLabel = new GUIComponent(TYPE_LABEL,0,"Viewpoints :",font);
    noVpLabel = new GUIComponent(TYPE_LABEL,0,"no viewpoints saved",font);
    goButton = new GUIComponent(TYPE_BUTTON,0,"Go!",font);
    removeButton = new GUIComponent(TYPE_BUTTON,0,"Remove",font);
    saveButton = new GUIComponent(TYPE_BUTTON,0,"Save current view point",font);
    nameLabel = new GUIComponent(TYPE_LABEL,0,"Name :",font);
    inputLabel = new GUIComponent(TYPE_LABEL,0,"",font);
    okButton = new GUIComponent(TYPE_BUTTON,0,"Ok",font);
    cancelButton = new GUIComponent(TYPE_BUTTON,0,"Cancel",font);

#if defined(GLUTWMCLOSEFUNC)
    exitButton->setActive(false);
#endif
    
    saving = false;
    selectedVP = -1;
    typedName = "";
    
    updateViewPoints();
    setComponents();
    
    windowTitle = "View Points";
    createWindow();
    glClearColor(GUI::getPanelColor().r,GUI::getPanelColor().g,GUI::getPanelColor().b,1.0);
    glutHideWindow();
}


void ViewPointsWindow::setComponents(){
    
    
    int pos = 1;
    int space = 20;
    
    height = 200+nbVP*space;
    
    
    exitButton->set(width-20,4,15,15);
    vpLabel->set(20,space*pos++,0,0);
    noVpLabel->set(40,pos*space,0,0);
    for(int i = 0; i<nbVP; i++){
	viewPointsButtons[i]->set(40,pos*space,100,space);
	pos++;
    }
    pos++;
    goButton->set(20,pos*space,60,20);
    removeButton->set(110,pos*space,60,20);
    pos++;pos++;
    saveButton->set(20,pos*space,150,20);
    pos++;pos++;
    //if(saving){
	nameLabel->set(20,pos*space,0,0);
	inputLabel->set(60,pos*space,0,0);
	inputLabel->setSelected(true);
	pos++;
	okButton->set(20,pos*space,60,20);
	cancelButton->set(110,pos*space,60,20);
    //}
    pos++;pos++;
}

void ViewPointsWindow::updateViewPoints(){
    
    nbVP = parent->viewpoints.size();
    viewPointsButtons = new GUIComponent*[nbVP];
    for(int i = 0; i<nbVP; i++){
	viewPointsButtons[i] = new GUIComponent(TYPE_LABEL,i,parent->viewpoints[i].name,font);
    }
    
}


ViewPointsWindow::~ViewPointsWindow(){

    glutDestroyWindow(ID);
    
    delete exitButton;
    delete vpLabel;
    delete noVpLabel;
    if(viewPointsButtons != NULL) delete[] viewPointsButtons;
    delete goButton;
    delete removeButton;
    delete saveButton;
    delete nameLabel;
    delete inputLabel;
    delete okButton;
    delete cancelButton;    
    
}

void ViewPointsWindow::close(){
    hideWindow();
}

void ViewPointsWindow::reshape(int w, int h){
    glutReshapeWindow(width,height);
    //width = w;
    //height = h;
    //setComponents();
}

void ViewPointsWindow::mousePressed(int mouse, int state, int x,int y){
    if(state == GLUT_DOWN){
	if (exitButton->inside(x,y)){
	    close();
	} else if(goButton->inside(x,y) && nbVP>0){
	    goToVP(selectedVP);
	} else if(removeButton->inside(x,y) && nbVP>0){
	    removeVP(selectedVP);
	} else if(saveButton->inside(x,y)){
	    activeSave();
	} else if(cancelButton->inside(x,y) && saving){
	    desactiveSave();
	} else if(okButton->inside(x,y) && saving){
	    saveVP();
	} else {
	    for(int i = 0; i<nbVP; i++){
			viewPointsButtons[i]->setSelected(false);
			if(viewPointsButtons[i]->inside(x,y)){
				selectedVP = viewPointsButtons[i]->getID();
				viewPointsButtons[i]->setSelected(true);
				}
			}
		} 
    }
    glutPostRedisplay();
}

void ViewPointsWindow::removeVP(int vp){
    if(vp >=0 && vp < nbVP){
	parent->viewpoints.remove(vp);
    }
    selectedVP = -1;
    updateViewPoints();
    parent->dataModified = true;
    setComponents();
    reshape(width,height);
}

void ViewPointsWindow::goToVP(int vp){
    if(vp >=0 && vp < nbVP){
	viewpoint viewp = parent->viewpoints[vp];
	parent->xParam = viewp.xparam ;
	parent->selectField(viewp.field);
	parent->posX = viewp.posx;
	parent->posY[parent->selectedField-1] = viewp.posy;
	parent->posZ = viewp.posz ;
	parent->scaleT = viewp.scaleT ;
	parent->scaleE = viewp.scaleE ;
	parent->scaleV[parent->selectedField-1] = viewp.scaleV;
	parent->aX = viewp.ax ;
	parent->aY = viewp.ay ;
	parent->setColorParam(viewp.colorParam);
	parent->lowTh = viewp.colorLowT;
	parent->highTh = viewp.colorHighT;
    }
    selectedVP = -1;
    parent->draw();
}

void ViewPointsWindow::activeSave(){
    saving = true;
    setComponents();
}

void ViewPointsWindow::desactiveSave(){
    saving = false;
    typedName = "";
    setComponents();
}

void ViewPointsWindow::saveVP(){
    if(typedName!=""){
		viewpoint vp;
		vp.matrixNum = parent->selectedMatrixNum;
		vp.xparam = parent->xParam;
		vp.field = parent->selectedField;
		vp.colorParam = parent->colorParam;
		vp.colorLowT = parent->lowTh;
		vp.colorHighT = parent->highTh;
		vp.posx = parent->posX;
		vp.posy = parent->posY[parent->selectedField-1];
		vp.posz = parent->posZ;
		vp.scaleT = parent->scaleT;
		vp.scaleE = parent->scaleE;
		vp.scaleV = parent->scaleV[parent->selectedField-1];
		vp.ax = parent->aX;
		vp.ay = parent->aY;
		vp.name = typedName;
		parent->viewpoints.push_back(vp);
    }
    saving = false;
    typedName = "";
    updateViewPoints();
    parent->dataModified = true;
    setComponents();
    reshape(width,height);
}

void ViewPointsWindow::draw(){
    glutSetWindow(ID);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    orthoProj();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    if(exitButton->isActive()) exitButton->draw();
    vpLabel->draw();
    
    if(nbVP==0){
	noVpLabel->draw();
    } else { 
	for(int i = 0; i < nbVP; i++){
	    viewPointsButtons[i]->draw();
	}
	goButton->draw();
	removeButton->draw();
    }
    
    saveButton->draw();
    
    if(saving){
		nameLabel->draw();
		inputLabel->setText(typedName);
		inputLabel->draw();
		okButton->draw();
		cancelButton->draw();
    }
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glutSwapBuffers();
}

void ViewPointsWindow::keyPressed(unsigned char key, int x,int y){
    if(saving){
	if(key == 127) {
	    if(typedName.length() > 0 ) typedName.erase(typedName.length()-1,1);
	} else if(key == 13){
	    saveVP();
	} else {
	    if(typedName.length() < 20 ) typedName += key;
	}
    } else if(selectedVP >= 0){
	if(key == 13){
	    goToVP(selectedVP);
	} else if(key == 127) {
	    removeVP(selectedVP);
	}
    }
    glutPostRedisplay();
}


