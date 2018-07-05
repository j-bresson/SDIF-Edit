/*
 *  ColorChooserWindow.cpp
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

#include "ColorChooserWindow.h"


ColorChooserWindow::ColorChooserWindow(color *bg, color *min, color *max, float *low, float *high, SDIFViewerWindow* sv){
    
    width = 400;
    height = 200;
    font = GLUT_BITMAP_HELVETICA_10;
    
    parent = sv;
    
    tmpBgColor = *bg;
    tmpMinColor = *min;
    tmpMaxColor = *max;
    
    tmpLow = *low;
    tmpHigh = *high;
    
    bgColor = bg;
    minColor = min;
    maxColor = max;
    
    lowTh = low;
    highTh = high;
    
    selectedColor = -1;
    
    steps = 4;
    nbColors = steps*steps*steps + steps;
    float coef = 1/(float)(steps-1);
    
        
    palette = new color[nbColors];
    int cpt = 0;
    for(int g = 0; g < steps ; g++){
	palette[cpt].r = palette[cpt].g = palette[cpt].b = g*coef;
	cpt++;
    }

    	
    for(int B = 0; B<steps ; B++){

    	   	for(int R = 0; R<steps ; R++){

		    for(int G = 0; G<steps ; G++){

		palette[cpt].r = R*coef;
		palette[cpt].g = G*coef;
		palette[cpt].b = B*coef;
		cpt++;
	    }
	}
    }

    colorButtons = new GUIComponent*[nbColors];
    for(int i = 0; i<nbColors; i++){
	colorButtons[i] = new GUIComponent(TYPE_SPECIAL,0,"",NULL);
    }
    
    selectedBgColor = new GUIComponent(TYPE_SPECIAL,0,"",NULL);
    selectedMinColor = new GUIComponent(TYPE_SPECIAL,0,"",NULL);
    selectedMaxColor = new GUIComponent(TYPE_SPECIAL,0,"",NULL);

    bgLabel = new GUIComponent(TYPE_LABEL,0,"Background",font);
    minLabel = new GUIComponent(TYPE_LABEL,0,"Min value",font);
    maxLabel = new GUIComponent(TYPE_LABEL,0,"Max value",font);

    colorPreview = new GUIComponent(TYPE_SPECIAL,0,"",NULL);

    applyButton = new GUIComponent(TYPE_BUTTON,0,"Apply",font);
    exitButton = new GUIComponent(TYPE_BUTTON,0,"X",GLUT_BITMAP_8_BY_13);

    chooseParamLabel = new GUIComponent(TYPE_LABEL,0,"COLOR PARAM",font);
    paramLabels = NULL;
    paramButtons = NULL;

#if defined(GLUTWMCLOSEFUNC)
    exitButton->setActive(false);
#endif
    
    setComponents();

    windowTitle = "Color Chooser";
    createWindow();
    glClearColor(GUI::getPanelColor().r,GUI::getPanelColor().g,GUI::getPanelColor().b,1.0);
    glutHideWindow();
    
}

void ColorChooserWindow::update(){

    tmpBgColor = *bgColor;
    tmpMinColor = *minColor;
    tmpMaxColor = *maxColor;
    tmpLow = *lowTh;
    tmpHigh = *highTh;
    colorField = parent->colorParam - 1;
    //draw();
}

void ColorChooserWindow::setComponents(){
    
    int ox = 20;
    int oy = 20;
    int col = 0;
    int lin = 0;
    int space = 2;
    int size = 8;
    int linspace = 20;
    
    height = (10+parent->field)*linspace;
    
    for(int i = 0; i< nbColors; i++){
	colorButtons[i]->set(ox+col*(size+space),oy+lin*(size+space),size,size);
	col++;
	if( (col == steps && lin == 0) || col == steps*steps){
	    lin++;
	    col = 0;
	}
    }
    lin++;
    //int pos = oy+lin*(size+space);
    selectedBgColor->set(250,20,40,30);
    bgLabel->set(300,40,0,0);
    selectedMinColor->set(250,60,40,30);
    minLabel->set(300,80,0,0);
    selectedMaxColor->set(250,100,40,30);
    maxLabel->set(300,120,0,0);
    
    
    colorPreview->set(ox,100,200,20);
    
    exitButton->set(width-22,2,15,15);
    applyButton->set(width-80,height-30,60,20);

    col = 160;
    int pos = 0;
    oy = 160; 
    chooseParamLabel->set(20,oy,0,linspace);
    if(paramLabels!=NULL){
	for(int f = 0; f<parent->field; f++){
	    paramLabels[f]->set(col,oy+pos*linspace,0,10);
	    paramButtons[f]->set(col+120,oy+pos*linspace,10,10);
	    pos++;
	}
    }
    
}


ColorChooserWindow::~ColorChooserWindow(){
    
    glutDestroyWindow(ID);
    
    delete[] palette;

    delete[] colorButtons;
    delete selectedBgColor;
    delete selectedMinColor;
    delete selectedMaxColor;
    delete bgLabel;
    delete minLabel;
    delete maxLabel;
    delete colorPreview;
    delete applyButton;
    delete exitButton;
    delete chooseParamLabel;
    if(paramLabels != NULL) delete[] paramLabels;
    if(paramButtons != NULL) delete[] paramButtons;
    
}
void ColorChooserWindow::reshape(int w, int h){
    glutReshapeWindow(width,height);
    /*width = w;
    height = h;
    setComponents();
    */
}

void ColorChooserWindow::reset(){

    if(paramLabels!=NULL) delete[] paramLabels;
    if(paramButtons!=NULL) delete[] paramButtons;
    paramLabels = new GUIComponent*[parent->field];
    paramButtons = new GUIComponent*[parent->field];
    for(int f = 0; f < parent->field; f++){
	paramLabels[f] = new GUIComponent(TYPE_LABEL,0,parent->fieldNames[f+1],font);
	paramButtons[f] = new GUIComponent(TYPE_BUTTON,f,"",font);
    }
    setComponents();
    reshape(width,height);
    
}


void ColorChooserWindow::mousePressed(int mouse, int state, int x,int y){
    
    if(state == GLUT_DOWN){	
	mouseX = x;
	mouseY = y;
	click = CLICK_NONE;
	selectAction(x,y);
	
    } else if(state == GLUT_UP){
	click = CLICK_NONE;
	if(selectedColor != -1){
	    if(selectedBgColor->inside(x,y)){
		tmpBgColor = palette[selectedColor];
	    } else if(selectedMinColor->inside(x,y)){
		tmpMinColor = palette[selectedColor];
	    } else if(selectedMaxColor->inside(x,y)){
		tmpMaxColor = palette[selectedColor];
	    }
	    selectedColor = -1;
	}
    }
    glutPostRedisplay();
}


void ColorChooserWindow::selectAction(int x, int y){
    
    if(colorPreview->inside(x,y)) {
	float position = (x-colorPreview->x1())/(float)(colorPreview->width());
	if(x<=colorPreview->x1()+2 || position < tmpLow){
	    click = CLICK_LOW_TH;
	} else if(x>=colorPreview->x2()-2 || position > tmpHigh){
	    click = CLICK_HIGH_TH;
	}
    } else if(applyButton->inside(x,y)){
	*bgColor = tmpBgColor;
	*minColor = tmpMinColor;
	*maxColor = tmpMaxColor;
	*highTh = tmpHigh;
	*lowTh = tmpLow;
	parent->setColorParam(colorField+1); 
	glutSetWindow(parent->getID());
	glutPostRedisplay();
	glutSetWindow(ID);
    } else if(exitButton->inside(x,y)){
	close();
    } else {
	for(int f = 0; f < parent->field; f++){
	    if(paramButtons[f]->inside(x,y)){
		colorField = f;
		return;
	    }
	}
	for(int i=0; i<nbColors; i++){
	    if(colorButtons[i]->inside(x,y)){
		selectedColor = i;
		return;
	    }
	}
    }

}

void ColorChooserWindow::mouseMoved(int x,int y){
    
    float dx = (x - mouseX)/(float)(colorPreview->width());
    if(click == CLICK_LOW_TH){
	if(tmpLow+dx>=0 && tmpLow+dx<tmpHigh) tmpLow += dx;
    } else if(click == CLICK_HIGH_TH){
	if(tmpHigh+dx<=1 && tmpHigh+dx>tmpLow) tmpHigh += dx;
    } 
    
    mouseX = x;
    mouseY = y;
    glutPostRedisplay();
}

void ColorChooserWindow::close(){
    hideWindow();
}

void ColorChooserWindow::draw(){
    
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
    for(int i = 0; i<nbColors; i++){
	glColor3f(palette[i].r,palette[i].g,palette[i].b);
	glBegin(GL_QUADS);
	glVertex2d(colorButtons[i]->x1(),colorButtons[i]->y1());
	glVertex2d(colorButtons[i]->x2(),colorButtons[i]->y1());
	glVertex2d(colorButtons[i]->x2(),colorButtons[i]->y2());
	glVertex2d(colorButtons[i]->x1(),colorButtons[i]->y2());
	glEnd();
    }

    glColor3f(tmpBgColor.r,tmpBgColor.g,tmpBgColor.b);
    glBegin(GL_QUADS);
    glVertex2d(selectedBgColor->x1(),selectedBgColor->y1());
    glVertex2d(selectedBgColor->x2(),selectedBgColor->y1());
    glVertex2d(selectedBgColor->x2(),selectedBgColor->y2());
    glVertex2d(selectedBgColor->x1(),selectedBgColor->y2());
    glEnd();

    glColor3f(tmpMinColor.r,tmpMinColor.g,tmpMinColor.b);
    glBegin(GL_QUADS);
    glVertex2d(selectedMinColor->x1(),selectedMinColor->y1());
    glVertex2d(selectedMinColor->x2(),selectedMinColor->y1());
    glVertex2d(selectedMinColor->x2(),selectedMinColor->y2());
    glVertex2d(selectedMinColor->x1(),selectedMinColor->y2());
    glEnd();

    glColor3f(tmpMaxColor.r,tmpMaxColor.g,tmpMaxColor.b);
    glBegin(GL_QUADS);
    glVertex2d(selectedMaxColor->x1(),selectedMaxColor->y1());
    glVertex2d(selectedMaxColor->x2(),selectedMaxColor->y1());
    glVertex2d(selectedMaxColor->x2(),selectedMaxColor->y2());
    glVertex2d(selectedMaxColor->x1(),selectedMaxColor->y2());
    glEnd();

    
    int posL = (int) (tmpLow * colorPreview->width());
    int posH = (int) (tmpHigh * colorPreview->width());
    
    glBegin(GL_QUADS);
    glColor3f(tmpMinColor.r,tmpMinColor.g,tmpMinColor.b);
    glVertex2d(colorPreview->x1(),colorPreview->y1());
    glVertex2d(colorPreview->x1(),colorPreview->y2());
    glVertex2d(colorPreview->x1()+posL+1,colorPreview->y2());
    glVertex2d(colorPreview->x1()+posL+1,colorPreview->y1());
    
    glVertex2d(colorPreview->x1()+posL+2,colorPreview->y1());
    glVertex2d(colorPreview->x1()+posL+2,colorPreview->y2());
    
    glColor3f(tmpMaxColor.r,tmpMaxColor.g,tmpMaxColor.b);
    glVertex2d(colorPreview->x1()+posH-1,colorPreview->y2());
    glVertex2d(colorPreview->x1()+posH-1,colorPreview->y1());
    
    glVertex2d(colorPreview->x1()+posH,colorPreview->y1());
    glVertex2d(colorPreview->x1()+posH,colorPreview->y2());
    glVertex2d(colorPreview->x2(),colorPreview->y2());
    glVertex2d(colorPreview->x2(),colorPreview->y1());
    
    glEnd();
    
    
    if(exitButton->isActive()) exitButton->draw();
    
    applyButton->draw();
    bgLabel->draw();
    minLabel->draw();
    maxLabel->draw();

    if(selectedColor!=-1){
	glColor4f(palette[selectedColor].r,palette[selectedColor].g,palette[selectedColor].b,0.1);
	glBegin(GL_QUADS);
	glVertex2d(mouseX-4,mouseY-4);
	glVertex2d(mouseX+4,mouseY-4);
	glVertex2d(mouseX+4,mouseY+4);
	glVertex2d(mouseX-4,mouseY+4);
	glEnd();
	
    }

    chooseParamLabel->draw();
    if(paramLabels!=NULL){
	for(int f = 0; f<parent->field; f++){
	    paramLabels[f]->draw();
	    if(colorField == f) paramButtons[f]->setText("x");
	    else paramButtons[f]->setText("");
	    paramButtons[f]->draw();
	}
    }
    
    
    glPopMatrix();
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
       
    glutSwapBuffers();
    
}

