/*
 *  MainWindow.cpp
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

#include "MainWindow.h"
#include "SDIFViewerWindow.h"

using namespace std;

#ifndef WIN32
#define min(a,b) (((a)<(b))?(a):(b))
#endif

/* Constructeur */
MainWindow::MainWindow(const char* _fileName, int w, int h) {
    
	//int strlength = min(42,strlen(_fileName));
    fileName = new char[strlen(_fileName)];
    //char* fileName2 = new char[strlength];
	strcpy(fileName,_fileName);
    //strncpy(fileName2,fileName,strlength-1);
    
	windowTitle = (char *) fileName;
	
	STOP_EXE = false;

    editWin = NULL;
    audioFileName = NULL;
    editorID = -1;
    
    width = w;
    //height = h;
    
    // Initialisation des differents attributs
    sdd = NULL;
    nbMatrices = 0;
    nbStr = 0;
    errFlag = 0;
    font = GLUT_BITMAP_HELVETICA_10;
    space = 15;
    
    string name = "FILE: ";
    name += fileName;
    fullPanel = new GUIComponent(TYPE_PANEL,0,"",NULL);
    fileLabel = new GUIComponent(TYPE_LABEL,0,name,GLUT_BITMAP_HELVETICA_12);
    selectButton = new GUIComponent(TYPE_BUTTON,0,"Select",font);
    partialsButton = new GUIComponent(TYPE_BUTTON,0,"View Partial Set",font);
    tmpLabel1 = new GUIComponent(TYPE_LABEL,0,"",font);
    tmpLabel2 = new GUIComponent(TYPE_LABEL,0,"",font);
    streamLabels = NULL;
    matrixButtons = NULL;

    closeButton = new GUIComponent(TYPE_BUTTON,0,"Close",font);
#if defined(GLUTWMCLOSEFUNC)
    closeButton->setActive(false);
#endif
	partialsButton->setActive(false);
        
    // Lecture du fichier SDIF
    errFlag = readSdifFile();
    if(errFlag == -1){
		errMsg = "ERROR : FILE CAN NOT BE READ.";
		height = 200;
    } else {
		sdd->createMatrixTable();
	
		
		
		nbStr = sdd->getNbStreams();
		height = ((nbMatrices+nbStr)/2+12)*space;
	
		SVStream* svs;
		SVMatrix* svm;
		char tmp[128];
		int i,j;
	
		streamLabels = new GUIComponent*[nbStr];
		matrixButtons = new GUIComponent*[nbMatrices];
		int cpt = 0;
		for( i = 0; i< nbStr; i++){
			svs = sdd->getStream(i);
			int nMat = svs->getNbMat();
			int n = 0;
			for( j = 0; j< nMat; j++){
				svm = svs->getMatrix(j);
				int nf = svm->getNbTimes();
				if(nf>0){
					sprintf(tmp,"        %s %s",svm->getSigText(),svm->getName());
					matrixButtons[cpt] = new GUIComponent(TYPE_LABEL,cpt,tmp,font);
					cpt++;
					n++;
				}
			}
			//if((strcmp(svs->getName(),"1TRC") == 0) || (strcmp(svs->getName(),"1MRK") == 0) || (strcmp(svs->getName(),"1HRM") == 0)){
			//	partialsButton->setActive(true);
			//}
			sprintf(tmp,"     Stream %d - %s",svs->getId(),svs->getName());
			streamLabels[i] = new GUIComponent(TYPE_BUTTON,n,tmp,font);
		}
    }
	
	newSelection(0);	
	// Creation de la fenetre
	createWindow();
    // Initialisation du visualisateur
    initGL();
	setComponents();
}

void MainWindow::setAudioFileName(char* name){
    audioFileName = new char[strlen(name)];
    strcpy(audioFileName,name);
}

MainWindow::~MainWindow(){
    
    glutDestroyWindow(ID);
    delete sdd;

    delete fullPanel;
    delete fileLabel;
    delete selectButton;
	delete partialsButton;	
	if(streamLabels!=NULL) delete[] streamLabels;
    if(matrixButtons!=NULL) delete[] matrixButtons;
    delete tmpLabel1;
    delete tmpLabel2;    
    delete closeButton;
    
    //if(editorID != -1)
    if(editWin != NULL) 
	delete editWin;
	
	// new 1.4
	quit();
	// �a quitte tout quand on ferne une ...
}

void MainWindow::setComponents(){
    
    fullPanel->set(10,10,width-20,height-20);
    fileLabel->set(35,35,0,0);
    selectButton->set(width/2-20,height-40,40,20);
	partialsButton->set(width/2+30,height-40,80,20);
	
    closeButton->set(width-55,10,35,space);
    
    int cpt = 0;
    int col = 0;
    int colW = 250;
    int pos = 5;
    char tmp[128];
    
    for(int i = 0; i< nbStr; i++){
	streamLabels[i]->set(10+col*colW,(int)(space*pos++),0,space);
	int nMat = streamLabels[i]->getID();
	for(int j=0; j < nMat; j++){
		matrixButtons[cpt]->setSelected(false);
	    matrixButtons[cpt]->set(10+col*colW,(int)(space*pos++),200,space);
	    if(cpt==selectedMatrixNum){
			matrixButtons[cpt]->setSelected(true);
			sprintf(tmp,"           %i fields - %i elements (max)",
			sdd->getMatrix(cpt)->getNbCol(),sdd->getMatrix(cpt)->getNbRowMax());
			tmpLabel1->setText(tmp);
			tmpLabel1->set(10+col*colW,(int)(space*pos++),0,space);
			sprintf(tmp,"           %i frames from %.3f to %.3f", 
			sdd->getMatrix(cpt)->getNbTimes(),
			sdd->getMatrix(cpt)->getBeginTime(),sdd->getMatrix(cpt)->getEndTime());
			tmpLabel2->setText(tmp);
			tmpLabel2->set(10+col*colW,(int)(space*pos++),0,space);
			tmpLabel1->setSelected(true);
			tmpLabel2->setSelected(true);
	    }
	    
	    cpt++;
	    if(pos > (height/space)-5){
		col+=1;
		pos = 5;
	    }
	    
	}
    }
}


/* Fermeture du visualisateur */
void MainWindow::close(){
    sdd->closeSdifFile();
    EventManager::SendCloseEvent(ID);

#ifdef GLUTCHECKLOOP
	closeMe(ID);
#else 
    closeWindow(ID);
    exit(0);
#endif
}

void MainWindow::newSelection(int i){
    selectedMatrixNum = i;
	if(selectedMatrixNum<0) selectedMatrixNum = nbMatrices-1;
	// check partiels
	setComponents();
	//glutPostRedisplay();
}

void MainWindow::setEditorID(int i){
    editorID = i;
}

int MainWindow::getEditorID(){
    return editorID;
}

void MainWindow::initGL( void )   {
    glShadeModel(GL_SMOOTH);			// Enable Smooth Shading
    //glClearColor(bgColor.r, bgColor.g, bgColor.b, 0.5f); 	// Black Background
    glClearDepth(1.0f);				// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);			// The Type Of Depth Testing To Do
    glEnable ( GL_COLOR_MATERIAL );
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glMatrixMode(GL_PROJECTION);
    //glFrustum(-1.0, 1.0, -1.0, 1.0, 1.8, 1000.0);
}

void MainWindow::reshape(int w, int h){
    width = w;
    height = h;
    setComponents();
    glViewport     ( 0, 0, w, h );
    glMatrixMode   ( GL_PROJECTION );
    glLoadIdentity ( );                // Reset The Projection Matrix
    if ( h==0 )  // Calculate The Aspect Ratio Of The Window
	gluPerspective ( 80, ( float ) w, 1.0, 5000.0 );
    else
	gluPerspective ( 80, ( float ) w / ( float ) h, 1.0, 5000.0 );
    glMatrixMode   ( GL_MODELVIEW );  // Select The Model View Matrix
    glLoadIdentity ( );    // Reset The Model View Matrix
}



/* Lecture du fichier SDIF */
int MainWindow::readSdifFile() {
    
    // C'est la classe SdifDescriptor qui lit le fichier
    sdd = new SDIFDescriptor(fileName);
    int err = sdd->readSdifFile();
    if(err != -1){

	// La suite est une description affich√©e a partir des infos recueillies par le SdifDescriptor
	cout << "description du fichier: " << fileName << endl;
	int nbreStreams = sdd->getNbStreams();
	cout << "Nb Stream = " << nbreStreams << endl;
	int i,j,nMat;
	SVStream* svs;
	SVMatrix* svm;
	for(i = 0; i< nbreStreams; i++){
	    svs = sdd->getStream(i);
	    cout << "Stream " << svs->getId() << " - " << svs->getName() << endl;
	    cout << "\t" << svs->getNbFrames() << " frames" << endl;
	    cout << "\tbegin: " << svs->getTimeBegin() << " - end: " << svs->getTimeEnd() << endl;
	    nMat = svs->getNbMat();
	    cout << "\t" << nMat << " matrice(s):" << endl;
	    for(j=0; j < nMat; j++){
		svm = svs->getMatrix(j);
		// On garde une information sur le nombre de matrices repr√©sent√©es reellement dans le fichier.
  // Une matrice peut en effet exister en tant que composante d'une Frame SDIF mais √™tre absente dans le fichier.
		if(svm->getNbTimes()!=0) nbMatrices++;
cout << "\t\t" << svm->getSigText() << " - " << svm->getName() << "    " << svm->getNbCol() << " colonnes - " << svm->getNbRowMax() << " lignes -  type: " << svm->getDataType() <<  "   nb frames: " << svm->getNbTimes()  << endl;
cout << "\t\t" << svm->getNbPoints() << " points" << endl;
	    }
	}
    }
    return err;
}

/* Selection d'une matrice */
void MainWindow::selectMatrix(int matNum){
        
    if(editWin != NULL) delete editWin;
    editWin = new SDIFViewerWindow(sdd, matNum, this, audioFileName);
    editorID = editWin->getID();
    glutSetWindow(ID);
    glutHideWindow();
    glutSetWindow(editorID);
}



/* Reponse aux evenements des touches ASCII du clavier */
void MainWindow::keyPressed( unsigned char key, int x, int y ){

    switch ( key ) {
	case 27:        // Escape key
	    Window::quit();
	break;
	
	case 13:	// enter key
	    // Validation du choix de la matrice en debut d'executiuon
	    if(errFlag == -1){
		close();
	    } else {
		selectMatrix(selectedMatrixNum);
	    }
	    break;
	default:
	    break;
    }
    glutPostRedisplay();
}



/* touches speciales du clavier */
void MainWindow::specialPressed( int a_keys, int x, int y ){
	switch ( a_keys ) {
	case GLUT_KEY_UP:
	    newSelection(selectedMatrixNum-1);
	    break;
	case GLUT_KEY_DOWN:
	    newSelection((selectedMatrixNum+1) % nbMatrices);
	    break;

	default:
	    break;
	}
	setComponents();
	glutPostRedisplay();
}


void MainWindow::mousePressed(int mouse, int state, int x, int y){
    if(state==GLUT_DOWN){
	
	if(selectButton->inside(x,y) && selectedMatrixNum >= 0){
	    selectMatrix(selectedMatrixNum);
	} else if(partialsButton->inside(x,y)){
	    selectMatrix(-1);
	} else if(closeButton->inside(x,y)){
	    close();
	} else {
	    for(int i = 0; i< nbMatrices; i++){
		if(matrixButtons[i]->inside(x,y)){
		    newSelection(matrixButtons[i]->getID());
			glutPostRedisplay();
			}
	    }
	}
    }
}

void MainWindow::mouseMoved(int x, int y){
    
}



void MainWindow::drawGUI() {

	//int maxP = (height/space)-5;
    fullPanel->draw();
    fileLabel->draw();
    
    if(closeButton->isActive()) closeButton->draw();
    
    if(errFlag == -1){
		GUIComponent tmpErr = GUIComponent(TYPE_LABEL,0,errMsg,GLUT_BITMAP_9_BY_15);
		tmpErr.set(40,100,0,0);
		tmpErr.draw();
		tmpErr.setText("Press ENTER to exit");
		tmpErr.set(40,140,0,0);
		tmpErr.draw();
    } else {
		selectButton->draw();
		if(partialsButton->isActive()) 
			partialsButton->draw();
		int i,j;
		for(i=0; i<nbStr; i++){
			streamLabels[i]->draw();
		}
	
		for(j = 0; j<nbMatrices; j++){
			if(j==selectedMatrixNum){
			matrixButtons[j]->draw();
			tmpLabel1->draw();
			tmpLabel2->draw();
			} else {
			matrixButtons[j]->draw();
			}
		}
    } 
}



void MainWindow::draw() {
    glutSetWindow(ID);
    glClearColor(0,0,0, 0.5f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    orthoProj();
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    drawGUI();
    glPopMatrix();
    
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    glutSwapBuffers();
	
}





