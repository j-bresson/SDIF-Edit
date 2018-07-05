/*
 *  SplineEditorWindow.cpp
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

#include "SplineEditorWindow.h"


SplineEditorWindow::SplineEditorWindow(SplineSurface *s, int w, int h){
    
    surf = s;
    width = w;
    height = h;
    
    curveW = 100;
    curveH = 50;
    
    ctrlPt = -1;
    
    font = GLUT_BITMAP_HELVETICA_10;

    iLabel = new GUIComponent(TYPE_LABEL,0,"t-profile",font);
    jLabel = new GUIComponent(TYPE_LABEL,0,"x-profile",font);

    curvei_1 = new GUIComponent(TYPE_SPECIAL,1,"",NULL);
    curvei_2 = new GUIComponent(TYPE_SPECIAL,2,"",NULL);
    curvei_3 = new GUIComponent(TYPE_SPECIAL,3,"",NULL);
    curvei_4 = new GUIComponent(TYPE_SPECIAL,4,"",NULL);
    curvej_1 = new GUIComponent(TYPE_SPECIAL,5,"",NULL);
    curvej_2 = new GUIComponent(TYPE_SPECIAL,6,"",NULL);
    curvej_3 = new GUIComponent(TYPE_SPECIAL,7,"",NULL);
    curvej_4 = new GUIComponent(TYPE_SPECIAL,8,"",NULL);
    
    ti = new GUIComponent(TYPE_LABEL,0,"Degree",font);
    ti_p = new GUIComponent(TYPE_BUTTON,0,"+",font);
    ti_m = new GUIComponent(TYPE_BUTTON,0,"-",font);
    tj = new GUIComponent(TYPE_LABEL,0,"Degree",font);
    tj_p = new GUIComponent(TYPE_BUTTON,0,"+",font);
    tj_m = new GUIComponent(TYPE_BUTTON,0,"-",font);

    resi = new GUIComponent(TYPE_LABEL,0,"Resolution",font);
    resi_p = new GUIComponent(TYPE_BUTTON,0,"+",font);
    resi_m = new GUIComponent(TYPE_BUTTON,0,"-",font);
    resj = new GUIComponent(TYPE_LABEL,0,"Resolution",font);
    resj_p = new GUIComponent(TYPE_BUTTON,0,"+",font);
    resj_m = new GUIComponent(TYPE_BUTTON,0,"-",font);
    exitButton = new GUIComponent(TYPE_BUTTON,0,"X",GLUT_BITMAP_8_BY_13);    

#if defined(GLUTWMCLOSEFUNC)
    exitButton->setActive(false);
#endif
    
    setComponents();
    
    windowTitle = "Spline Editor";
    createWindow();
    glClearColor(GUI::getPanelColor().r,GUI::getPanelColor().g,GUI::getPanelColor().b,1.0);
    glutHideWindow();
    
}


void SplineEditorWindow::setComponents(){
    
    curveH = (int) ((height-30)/2);
    curveW = (int) (0.7*width);
    int space = 20;
    
    curvei_1->set(10,10+curveH/2,curveW/2,curveH/2);
    curvei_2->set(10,10,curveW/2,curveH/2);
    curvei_3->set(10+curveW/2,10,curveW/2,curveH/2);
    curvei_4->set(10+curveW/2,10+curveH/2,curveW/2,curveH/2);
    curvej_1->set(10,20+3*curveH/2,curveW/2,curveH/2);
    curvej_2->set(10,20+curveH,curveW/2,curveH/2);
    curvej_3->set(10+curveW/2,20+curveH,curveW/2,curveH/2);
    curvej_4->set(10+curveW/2,20+3*curveH/2,curveW/2,curveH/2);

    
    int left = curveW +20;
    int textw = 60;
    
    iLabel->set(left,10,textw,20);
    jLabel->set(left,20+curveH,textw,20);
    
    
    ti->set(left,10+space,textw,space);
    ti_p->set(left+textw+20,10+space+1,15,8);
    ti_m->set(left+textw+20,10+space+11,15,8);
    resi->set(left,10+2*space,textw,space);
    resi_p->set(left+textw+20,10+2*space+1,15,8);
    resi_m->set(left+textw+20,10+2*space+11,15,8);

    tj->set(left,20+curveH+space,textw,space);
    tj_p->set(left+textw+20,20+curveH+space+1,15,8);
    tj_m->set(left+textw+20,20+curveH+space+11,15,8);    
    resj->set(left,20+curveH+2*space,textw,space);
    resj_p->set(left+textw+20,20+curveH+2*space+1,15,8);
    resj_m->set(left+textw+20,20+curveH+2*space+11,15,8);
    
    exitButton->set(width-22,2,15,15);
    
}


SplineEditorWindow::~SplineEditorWindow(){
    
    glutDestroyWindow(ID);
    
    delete iLabel;
    delete jLabel;
    delete curvei_1;
    delete curvei_2;
    delete curvei_3;
    delete curvei_4;
    delete curvej_1;
    delete curvej_2;
    delete curvej_3;
    delete curvej_4;
    delete ti;
    delete ti_p;
    delete ti_m;
    delete tj;
    delete tj_p;
    delete tj_m;
    delete resi;
    delete resi_p;
    delete resi_m;
    delete resj;
    delete resj_p;
    delete resj_m;
    delete exitButton;
    
}

void SplineEditorWindow::reshape(int w, int h){
    width = w;
    height = h;
    setComponents();
}

void SplineEditorWindow::mousePressed(int mouse, int state, int x,int y){
    
    if(state == GLUT_DOWN){
	ctrlPt = -1;
	mouseX = x;
	mouseY = y;
	if(exitButton->inside(x,y)){
	    close();
	} else if(ti_p->inside(x,y)){
	    surf->setTI(surf->getTI()+1);
	} else if(ti_m->inside(x,y)){
	    surf->setTI(surf->getTI()-1);
	} else if(tj_p->inside(x,y)){
	    surf->setTJ(surf->getTJ()+1);
	} else if(tj_m->inside(x,y)){
	    surf->setTJ(surf->getTJ()-1);
	} else if(resi_p->inside(x,y)){
	    surf->setResolutionI(surf->getResolutionI()+2);
	} else if(resi_m->inside(x,y)){
	    surf->setResolutionI(surf->getResolutionI()-2);
	} else if(resj_p->inside(x,y)){
	    surf->setResolutionJ(surf->getResolutionJ()+2);
	} else if(resj_m->inside(x,y)){
	    surf->setResolutionJ(surf->getResolutionJ()-2);
	} else if(curvei_1->inside(x,y)){
	    ctrlPt = curvei_1->getID();
	} else if(curvei_2->inside(x,y)){
	    ctrlPt = curvei_2->getID();
	} else if(curvei_3->inside(x,y)){
	    ctrlPt = curvei_3->getID();
	} else if(curvei_4->inside(x,y)){
	    ctrlPt = curvei_4->getID();
	} else if(curvej_1->inside(x,y)){
	    ctrlPt = curvej_1->getID();
	} else if(curvej_2->inside(x,y)){
	    ctrlPt = curvej_2->getID();
	} else if(curvej_3->inside(x,y)){
	    ctrlPt = curvej_3->getID();
	} else if(curvej_4->inside(x,y)){
	    ctrlPt = curvej_4->getID();
	}
    } else if(state == GLUT_UP){
	if(ctrlPt != -1){
	    surf->calculateSplineSurface();
	    ctrlPt = -1;
	}
    }
    glutPostRedisplay();
}

void SplineEditorWindow::mouseMoved(int x,int y){
    
    float dx = (x - mouseX)/(float) curveW;

    switch(ctrlPt){
	case -1:
	    break;
	case 1:
	    surf->setI1(surf->getI1()+dx);
	    break;
	case 2:
	    surf->setI2(surf->getI2()+dx);
	    break;
	case 3:
	    surf->setI3(surf->getI3()+dx);
	    break;
	case 4:
	    surf->setI4(surf->getI4()+dx);
	    break;
	case 5:
	    surf->setJ1(surf->getJ1()+dx);
	    break;
	case 6:
	    surf->setJ2(surf->getJ2()+dx);
	    break;
	case 7:
	    surf->setJ3(surf->getJ3()+dx);
	    break;
	case 8:
	    surf->setJ4(surf->getJ4()+dx);
	    break;
	default:
	    break;
    }
    
    mouseX = x;
    mouseY = y;
    glutPostRedisplay();
}

void SplineEditorWindow::close(){
    hideWindow();
}

void SplineEditorWindow::draw(){
    
    char str[50];
    
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
    glLoadIdentity();
    glColor3f(GUI::getButtonColor().r,GUI::getButtonColor().g,GUI::getButtonColor().b);
    glBegin(GL_QUADS);
	glVertex2d(curvei_2->x1(),curvei_2->y1());
	glVertex2d(curvei_3->x2(),curvei_3->y1());
	glVertex2d(curvei_4->x2(),curvei_4->y2());
	glVertex2d(curvei_1->x1(),curvei_1->y2());

	glVertex2d(curvej_2->x1(),curvej_2->y1());
	glVertex2d(curvej_3->x2(),curvej_3->y1());
	glVertex2d(curvej_4->x2(),curvej_4->y2());
	glVertex2d(curvej_1->x1(),curvej_1->y2());
    glEnd();

   
    glColor3f(GUI::getPanelColor().r,GUI::getPanelColor().g,GUI::getPanelColor().b);
    glBegin(GL_LINES);
    
    glVertex2d(curvei_3->x1(),curvei_3->y1());
    glVertex2d(curvei_4->x1(),curvei_4->y2());
    
    glVertex2d(curvej_3->x1(),curvej_3->y1());
    glVertex2d(curvej_4->x1(),curvej_4->y2());
    
    glEnd();
    
    glColor3f(GUI::getSelectedTextColor().r,GUI::getSelectedTextColor().g,GUI::getSelectedTextColor().b);

    glBegin(GL_QUADS);

    glVertex2d(curvei_1->x1()+surf->getI1()*curveW-2,curvei_1->y2());
    glVertex2d(curvei_1->x1()+surf->getI1()*curveW+2,curvei_1->y2());
    glVertex2d(curvei_1->x1()+surf->getI1()*curveW+2,curvei_1->y2()-4);
    glVertex2d(curvei_1->x1()+surf->getI1()*curveW-2,curvei_1->y2()-4);

    glVertex2d(curvei_1->x1()+surf->getI2()*curveW-2,curvei_2->y1());
    glVertex2d(curvei_1->x1()+surf->getI2()*curveW+2,curvei_2->y1());
    glVertex2d(curvei_1->x1()+surf->getI2()*curveW+2,curvei_2->y1()+4);
    glVertex2d(curvei_1->x1()+surf->getI2()*curveW-2,curvei_2->y1()+4);

    glVertex2d(curvei_1->x1()+surf->getI3()*curveW-2,curvei_3->y1());
    glVertex2d(curvei_1->x1()+surf->getI3()*curveW+2,curvei_3->y1());
    glVertex2d(curvei_1->x1()+surf->getI3()*curveW+2,curvei_3->y1()+4);
    glVertex2d(curvei_1->x1()+surf->getI3()*curveW-2,curvei_3->y1()+4);

    glVertex2d(curvei_1->x1()+surf->getI4()*curveW-2,curvei_4->y2());
    glVertex2d(curvei_1->x1()+surf->getI4()*curveW+2,curvei_4->y2());
    glVertex2d(curvei_1->x1()+surf->getI4()*curveW+2,curvei_4->y2()-4);
    glVertex2d(curvei_1->x1()+surf->getI4()*curveW-2,curvei_4->y2()-4);

    glVertex2d(curvej_1->x1()+surf->getJ1()*curveW-2,curvej_1->y2());
    glVertex2d(curvej_1->x1()+surf->getJ1()*curveW+2,curvej_1->y2());
    glVertex2d(curvej_1->x1()+surf->getJ1()*curveW+2,curvej_1->y2()-4);
    glVertex2d(curvej_1->x1()+surf->getJ1()*curveW-2,curvej_1->y2()-4);

    glVertex2d(curvej_1->x1()+surf->getJ2()*curveW-2,curvej_2->y1());
    glVertex2d(curvej_1->x1()+surf->getJ2()*curveW+2,curvej_2->y1());
    glVertex2d(curvej_1->x1()+surf->getJ2()*curveW+2,curvej_2->y1()+4);
    glVertex2d(curvej_1->x1()+surf->getJ2()*curveW-2,curvej_2->y1()+4);

    glVertex2d(curvej_1->x1()+surf->getJ3()*curveW-2,curvej_3->y1());
    glVertex2d(curvej_1->x1()+surf->getJ3()*curveW+2,curvej_3->y1());
    glVertex2d(curvej_1->x1()+surf->getJ3()*curveW+2,curvej_3->y1()+4);
    glVertex2d(curvej_1->x1()+surf->getJ3()*curveW-2,curvej_3->y1()+4);

    glVertex2d(curvej_1->x1()+surf->getJ4()*curveW-2,curvej_4->y2());
    glVertex2d(curvej_1->x1()+surf->getJ4()*curveW+2,curvej_4->y2());
    glVertex2d(curvej_1->x1()+surf->getJ4()*curveW+2,curvej_4->y2()-4);
    glVertex2d(curvej_1->x1()+surf->getJ4()*curveW-2,curvej_4->y2()-4);
    
    glEnd();
    
    
    int ri = surf->getResolutionI();
    int rj = surf->getResolutionJ();
    glColor3f(GUI::getTextColor().r,GUI::getTextColor().g,GUI::getTextColor().b);
    glBegin(GL_LINE_STRIP);
    for (int i=0;i< ri;i++) {
	glVertex2d(curvei_1->x1()+curveW*surf->surface[i][(rj-1)/2].x,curvei_1->y2()- 2 - 0.8*curveH*surf->surface[i][(rj-1)/2].z);
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (int j=0;j< rj;j++) {
	glVertex2d(curvej_1->x1()+curveW*surf->surface[(ri-1)/2][j].y,curvej_1->y2() - 2 - 0.8*curveH*surf->surface[(ri-1)/2][j].z);
    }
    glEnd();
    
    
    if(exitButton->isActive()) exitButton->draw();
    
    iLabel->draw();
    jLabel->draw();

    sprintf(str,"Degree %i",surf->getTI());
    ti->setText(str);
    ti->draw();
    sprintf(str,"Degree %i",surf->getTJ());
    tj->setText(str);
    tj->draw();
    
    sprintf(str,"Resolution %i",surf->getResolutionI());
    resi->setText(str);
    resi->draw();
    sprintf(str,"Resolution %i",surf->getResolutionJ());
    resj->setText(str);
    resj->draw();
    
    ti_p->draw();
    ti_m->draw();
    resi_p->draw();
    resi_m->draw();
    tj_p->draw();
    tj_m->draw();
    resj_p->draw();
    resj_m->draw();

    glPopMatrix();
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    glutSwapBuffers();
    
}

