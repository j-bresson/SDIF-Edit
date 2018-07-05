/*
 *  SDIFViewerWindow_Draw.cpp
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

/*******************************************************
 * OpenGL draw operations for the SDIFViewerWindow class
 *******************************************************/



#include "SDIFViewerWindow.h"

void SDIFViewerWindow::drawPartials() {

    glPushMatrix();
    for(int part = beginT; part<=endT; part++){
	float time = data[part].time;
	int nbPts = data[part].dimension-1;
	float xRef = data[part].values[0][xParam];
	float valRef = data[part].values[0][selectedField];
	float cRef = data[part].values[0][colorParam];
	float onset,deltaX,deltaV;
	int pt;
	color c;
	glPushMatrix();
	glScalef(scaleT,scaleV[selectedField-1],scaleE);
	glBegin(GL_LINE_STRIP);
	for(pt = 0; pt<nbPts; pt++){
	    onset = data[part].values[pt+1][1];
	    deltaX = data[part].values[pt+1][xParam];
	    deltaV = data[part].values[pt+1][selectedField];
	    if(part == selectedT && (selectedElem==0 || (pt+1 == selectedElem  && !timeLines))) glColor3f(1.0-bgColor.r,1.0-bgColor.g,1.0-bgColor.b);
	    else {	
		c = calculateColor(cRef+data[part].values[pt+1][colorParam]);
		glColor3f(c.r,c.g,c.b);
	    }
	    glVertex3f(time+onset,valRef+deltaV,xRef+deltaX);
	}
	glEnd();
	glPopMatrix();
	if(timeLines){
	    for(pt = 0; pt<nbPts; pt++){
		onset = data[part].values[pt+1][1];
		deltaX = data[part].values[pt+1][xParam];
		deltaV = data[part].values[pt+1][selectedField];
		c = calculateColor(cRef+data[part].values[pt+1][colorParam]);
		glColor3f(c.r,c.g,c.b);
		glPushMatrix();
		    glTranslatef((time+onset)*scaleT,(valRef+deltaV)*scaleV[selectedField-1],(xRef+deltaX)*scaleE);
		    if( part == selectedT && (pt+1 == selectedElem || selectedElem==0)) glColor3f(1.0-bgColor.r,1.0-bgColor.g,1.0-bgColor.b);
		    glutWireCube(0.3);
		glPopMatrix();
	    }
	} 
    }
    glPopMatrix();

}

void SDIFViewerWindow::drawSdifData() {

    glPushMatrix();
    	glScalef(scaleT,scaleV[selectedField-1],scaleE);
	int x,y;
	
	if(regularGridSize){
	    // socle
	    float h;
	    h=0; // socle au niveau de la valeur minimale
	    glColor3f(0.5f,0.5f,0.5f);
	    glBegin(GL_LINES);
	    glVertex3f(data[beginT].time,data[beginT].values[0][selectedField],data[beginT].values[0][xParam]);
	    glVertex3f(data[beginT].time,min-h/4.0,data[beginT].values[0][xParam]);
	    glEnd();
	    glBegin(GL_LINES);
	    glVertex3f(data[endT].time,data[endT].values[0][selectedField],data[endT].values[0][xParam]);
	    glVertex3f(data[endT].time,min-h/4.0,data[endT].values[0][xParam]);
	    glEnd();
	    glBegin(GL_LINES);
	    //glVertex3f(data[0].time,data[0].values[element-1][selectedField-1],element-1);
	    //glVertex3f(data[0].time,min-h/4.0,element-1);
	    glVertex3f(data[beginT].time,data[beginT].values[data[beginT].dimension-1][selectedField],data[beginT].values[data[beginT].dimension-1][xParam]);
	    glVertex3f(data[beginT].time,min-h/4.0,data[beginT].values[data[beginT].dimension-1][xParam]);
	    glEnd();
	    glBegin(GL_LINES);
	    glVertex3f(data[endT].time,data[endT].values[data[endT].dimension-1][selectedField],data[endT].values[data[endT].dimension-1][xParam]);
	    glVertex3f(data[endT].time,min-h/4.0,data[endT].values[data[endT].dimension-1][xParam]);
	    //glVertex3f(data[tps-1].time,data[tps-1].values[element-1][selectedField-1],element-1);
	    //glVertex3f(data[tps-1].time,min-h/4.0,element-1);
	    glEnd();
	    glBegin(GL_LINE_LOOP);
	    glVertex3f(data[beginT].time,min-h/4.0,data[beginT].values[0][xParam]);
	    glVertex3f(data[endT].time,min-h/4.0,data[endT].values[0][xParam]);
	    glVertex3f(data[endT].time,min-h/4.0,data[endT].values[data[endT].dimension-1][xParam]);
	    glVertex3f(data[beginT].time,min-h/4.0,data[beginT].values[data[beginT].dimension-1][xParam]);
	    
		//glVertex3f(data[tps-1].time,min-h/4.0,element-1);
	    //glVertex3f(data[0].time,min-h/4.0,element-1);
	    glEnd();
	}

	int element_var;
	int max_elem = 0;
	color c;
	for( x = beginT; x<= endT ; x++ ){
	    if (maxElt<data[x].dimension) element_var = maxElt;
		else element_var = data[x].dimension;
	    if(element_var > max_elem) max_elem = element_var;
	    glBegin(GL_LINE_STRIP);
	    for(y = 0; y < element_var; y++){
		c = calculateColor(data[x].values[y][colorParam]);
		glColor3f(c.r,c.g,c.b);
		glVertex3f(data[x].time,data[x].values[y][selectedField],data[x].values[y][xParam]);
	    }
	    glEnd();
	}
	

	
	if(timeLines){
	    for( y = 0; y< max_elem ; y++ ){
		for(x = beginT; x <= endT; x++){
		    //if(data[x].dimension > y){
		    glBegin(GL_LINE_STRIP);
			while (x <= endT && data[x].dimension > y){
			    c = calculateColor(data[x].values[y][colorParam]);
			    glColor3f(c.r,c.g,c.b);
			    glVertex3f(data[x].time,data[x].values[y][selectedField],data[x].values[y][xParam]);
			    x++;
			}
		    glEnd();
		}
	    }
	}
				
	glPopMatrix();
	
	//glColor3f(1.0f,1.0f,1.0f);
	glColor3f(1.0-bgColor.r,1.0-bgColor.g,1.0-bgColor.b);
	
	// objet selectionne
	if(selectedT != -1 && selectedElem != -1){
	    glPushMatrix();
	    glTranslatef(data[selectedT].time*scaleT,
			    data[selectedT].values[selectedElem][selectedField]*scaleV[selectedField-1],
			    data[selectedT].values[selectedElem][xParam]*scaleE);
	    //glutWireCube(0.2);
	    glutWireSphere(0.2,8,8);
	    glPopMatrix();
	    
	    if(showEditZone){
	    for(int t = selectedT-editWidthB; t<=selectedT+editWidthF; t++){
		for(int e = selectedElem-editWidthL; e<=selectedElem+editWidthR; e++){
		    if(t>=0 && t<tps && e>=0 && e<data[t].dimension){
			glPushMatrix();
			glTranslatef(data[t].time*scaleT,
				    data[t].values[e][selectedField]*scaleV[selectedField-1],
				    data[t].values[e][xParam]*scaleE);
			
			glutWireSphere(0.08,4,4);
			glPopMatrix();
		    }
		}
	    }
    
	}
	}
}

void SDIFViewerWindow::drawAxes(){
    glColor3f(1.0-bgColor.r,1.0-bgColor.g,1.0-bgColor.b);
    glPushMatrix();
	glLoadIdentity();
	glTranslatef(-1.4,-1,-1.5);
	glRotatef(aX,1,0,0);
	glRotatef(aY,0,1,0);
	if(view == TIME_VIEW) glRotatef(-90,0,1,0);
	glScalef(0.0004,0.0004,0.0004);
	string strX = "- ";
	if(xParam>0) { 
	    strX += fieldNames[xParam];
	} else {
	    strX += "elements";
	}
	strX += " ->";
	const char* c;
	if(element>1){
	    glPushMatrix();
	    for (c=strX.c_str(); *c != '\0'; c++) {
		glutStrokeCharacter(GLUT_STROKE_ROMAN , *c);
	    }
	    glPopMatrix();
	}
	if(tps>1){
	    glPushMatrix();
	    glRotatef(90,0,1,0);
	    string strZ = "- time ->";
	    for (c=strZ.c_str(); *c != '\0'; c++) {
		glutStrokeCharacter(GLUT_STROKE_ROMAN , *c);
	    }
	    glPopMatrix();
	}
    glPopMatrix();
}

void SDIFViewerWindow::drawPts(){
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    
    glRotatef(aX,1,0,0);
    glRotatef(aY,0,1,0);
    if(view == ELEM_VIEW) glRotatef(90,0,1,0);
    
    glTranslatef(-posX,-posY[selectedField-1],-posZ);    
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    if(data!=NULL){
		glScalef(scaleT,scaleV[selectedField-1],scaleE);
		
		if(SDIF_DATA==PARTIAL_DATA){
			for(int part = beginT; part<=endT; part++){
			float time = data[part].time;
			int nbPts = data[part].dimension-1;
			float xRef = data[part].values[0][xParam];
			float valRef = data[part].values[0][selectedField];
			float onset,deltaX,deltaV;
			int pt;
			glPushName(part);
			for(pt = 0; pt<nbPts; pt++){
				onset = data[part].values[pt+1][1];
				deltaX = data[part].values[pt+1][xParam];
				deltaV = data[part].values[pt+1][selectedField];
				glPushName(pt+1);
				glBegin(GL_POINTS);
					glVertex3f(time+onset,valRef+deltaV,xRef+deltaX);
				glEnd();
				glPopName();
			}
			glPopName();
			}
	    
		} else {
	
			int x,y,element_var;
			for( x = beginT; x<= endT ; x++ ){
				if(maxElt<data[x].dimension) element_var = maxElt; 
				else element_var = data[x].dimension;
				glPushName(x);
				for(y = 0; y < element_var; y++){
					glPushName(y);
					glBegin(GL_POINTS);
						glVertex3f(data[x].time,data[x].values[y][selectedField],data[x].values[y][xParam]);
					glEnd();
				glPopName();
				}
				glPopName();
			}
		}
    }
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    //glutSwapBuffers();
}

void SDIFViewerWindow::draw() {
    glutSetWindow(ID);
    glClearColor(bgColor.r, bgColor.g, bgColor.b, 0.5f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(data!=NULL){
	glViewport(vp[0],vp[1],vp[2],vp[3]);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glRotatef(aX,1,0,0);
	glRotatef(aY,0,1,0);
	if(view == ELEM_VIEW) glRotatef(90,0,1,0);
	glTranslatef(-posX,-posY[selectedField-1],-posZ);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	

	
	if(selectedMatrix->getMatSignature()=='EASM') drawPartials();
	else drawSdifData();
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	// REPERE
	glMatrixMode(GL_MODELVIEW);
	drawAxes();
    }

    glViewport(0,0,width,height);
    // TEXTE :
    // switch to projection mode
    glMatrixMode(GL_PROJECTION);
    // save previous matrix which contains the
    //settings for the perspective projection
    glPushMatrix();
    // set orthographic projection
    orthoProj();
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    drawGUI();
    glPopMatrix();
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    glutSwapBuffers();

    if(svw->isVisible()) svw->draw();
    if(sona->isVisible()) sona->draw();
}
