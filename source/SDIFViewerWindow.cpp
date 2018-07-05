/*
 *  SDIFViewerWindowWindow.cpp
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


#include "SDIFViewerWindow.h"


int SDIFViewerWindow::FreqToMidicents(float frequency){
    
    float diapasonFreq = 440;
    int diapasonMidic = 6900;
    int mc = diapasonMidic + (int) ( (log (fabs(frequency/diapasonFreq))) * (1200 / log(2.0)));
    return mc;
}


/* Constructeur */
SDIFViewerWindow::SDIFViewerWindow(SDIFDescriptor * sdifD, int numMatrix, MainWindow* parent, char* audioFileName, int w, int h) {
    
    
    sdd = sdifD;
    mainWin = parent;
    
    
    //STOP_EXE = false;
    //fileName = new char[strlen(_fileName)];
    //strcpy(fileName,_fileName);
    fileName = sdd->getFileName();
    windowTitle = (char *) fileName;
    
    width = w;
    height = h;
    
    // Initialisation des differents attributs
    nbMatrices = 0;
    
	clickedWidget = -1;
    timeParam = TIME_PARAM_DUR;
    
    mode = MODE_VIEW;
    view = ELEM_VIEW;
	
	settings_on = false;
	edition_on = false;
    
    font = GLUT_BITMAP_HELVETICA_10;
    space = 20;
    
	
    bgColor.r = 0.0;
    bgColor.g = 0.0;
    bgColor.b = 0.0;
    
    minColor.r = 1.0;
    minColor.g = 0.7;
    minColor.b = 0.1;

    maxColor.r = 1.0;
    maxColor.g = 0.0;
    maxColor.b = 0.1;
    
    //maxColor.r = 0.1;
    //maxColor.g = 0.4;
    //maxColor.b = 0.12;
    
    lowTh = 0.05;
    highTh = 0.95;

    posX = 0;
    posY = NULL;
    posZ = 0;
    aX = 0;
    aY = 0;
    
    //rot = 0;

    scaleReference = 50.0;
    shortCutParam = PARAM_NONE;
    timeLines = true;
    editStep = 0; // arbitraire..
    editPrecision = 1;
    step = 1;
    editMode = REF_EDIT;
    
    autoSynth = false;
    dataModified = false;
    
    
    editWidthL = 0;
    editWidthR = 0;
    editWidthF = 0;
    editWidthB = 0;
    showEditZone = true;
	zpt = 1.0;
    
    initViewerAttributes();
    
    panelN = new GUIComponent(TYPE_PANEL,0,"",NULL);
    panelS = new GUIComponent(TYPE_PANEL,0,"",NULL);
    panelW = new GUIComponent(TYPE_PANEL,0,"",NULL);
    panelE = new GUIComponent(TYPE_PANEL,0,"",NULL);
    
    fileLabel = new GUIComponent(TYPE_LABEL,0,"",GLUT_BITMAP_HELVETICA_18);
    
    matrixButton = new GUIComponent(TYPE_BUTTON,0,"File Map",font);
    beginTimeButton = new GUIComponent(TYPE_BUTTON,0,"Begin Time ",font);
    endTimeLabel = new GUIComponent(TYPE_LABEL,-1,"End Time ",font);
    durationButton = new GUIComponent(TYPE_BUTTON,0,"Duration ",font);
    nbFrameLabel = new GUIComponent(TYPE_LABEL,0,"Frames",font);
    timePreview = new GUIComponent(TYPE_MOUSECTRL,1,"",font);
	timePreview->setv1(0);
    timePreview->setv2(1);
    xParamButton = new GUIComponent(TYPE_BUTTON,0,"",font);
    xParamLabel = new GUIComponent(TYPE_LABEL,0,"X-Parameter",font);
    eltPreview = new GUIComponent(TYPE_MOUSECTRL,6,"",font);
    eltPreview->setv1(0);
    eltPreview->setv2(1);
    
	movingModeLabel = new GUIComponent(TYPE_LABEL,0,"Moving Mode",font);
    movingModeButton = new GUIComponent(TYPE_BUTTON,0,"",font);
    pointLabel = new GUIComponent(TYPE_LABEL,0,"POINT",font);
    tLabel = new GUIComponent(TYPE_LABEL,0,"Time = ",font);
    eltLabel = new GUIComponent(TYPE_LABEL,0,"Element ",font);
    settingsLabel = new GUIComponent(TYPE_LABEL,0,"SETTINGS",GLUT_BITMAP_HELVETICA_12);
    settingsButton = new GUIComponent(TYPE_BUTTON,0,"+",font);
    speedLabel = new GUIComponent(TYPE_LABEL,0,"Speed ",font);
    editStepLabel = new GUIComponent(TYPE_LABEL,0,"Edit Precision ",font);
    editStepPButton = new GUIComponent(TYPE_BUTTON,0,"+",font);
    editStepMButton = new GUIComponent(TYPE_BUTTON,0,"-",font);
    scaleVLabel = new GUIComponent(TYPE_LABEL,0,"Values Scale",font);
    scaleTLabel = new GUIComponent(TYPE_LABEL,0,"Time Scale",font);
    scaleELabel = new GUIComponent(TYPE_LABEL,0,"Matrix Elements Scale",font);
    speedWidget = new GUIComponent(TYPE_MOUSECTRL,2,"",font);
	speedWidget->setv1(0);
    speedWidget->setv2(step / 100);
    scaleVWidget = new GUIComponent(TYPE_MOUSECTRL,3,"-         +",font);
	scaleVWidget->setv1(0);
    scaleVWidget->setv2(1);
    scaleTWidget = new GUIComponent(TYPE_MOUSECTRL,4,"-         +",font);
	scaleTWidget->setv1(0);
    scaleTWidget->setv2(1);
    scaleEWidget = new GUIComponent(TYPE_MOUSECTRL,5,"-         +",font);
	scaleEWidget->setv1(0);
    scaleEWidget->setv2(1);
    
	initButton = new GUIComponent(TYPE_BUTTON,0,"Init View",font);
    linesButton = new GUIComponent(TYPE_BUTTON,0,"",font);
    saveButton = new GUIComponent(TYPE_BUTTON,0,"Save",font);
    editLabel = new GUIComponent(TYPE_LABEL,0,"EDITION",GLUT_BITMAP_HELVETICA_12);
    editButton = new GUIComponent(TYPE_BUTTON,0,"+",font);
    editAreaButton = new GUIComponent(TYPE_BUTTON,0,"Area",font);
    editTimeFromButton = new GUIComponent(TYPE_LABEL,0,"From ",font);
    editTimeFromPButton = new GUIComponent(TYPE_BUTTON,0,"+",font);
    editTimeFromMButton = new GUIComponent(TYPE_BUTTON,0,"-",font);
    editTimeToButton = new GUIComponent(TYPE_LABEL,0,"To ",font);
    editTimeToPButton = new GUIComponent(TYPE_BUTTON,0,"+",font);
    editTimeToMButton = new GUIComponent(TYPE_BUTTON,0,"-",font);
    editElemFromButton = new GUIComponent(TYPE_LABEL,0,"Min ",font);
    editElemFromPButton = new GUIComponent(TYPE_BUTTON,0,"+",font);
    editElemFromMButton = new GUIComponent(TYPE_BUTTON,0,"-",font);
    editElemToButton = new GUIComponent(TYPE_LABEL,0,"Max ",font);
    editElemToPButton = new GUIComponent(TYPE_BUTTON,0,"+",font);
    editElemToMButton = new GUIComponent(TYPE_BUTTON,0,"-",font);
    editCurveButton = new GUIComponent(TYPE_BUTTON,0,"EDIT CURVE",font);
    flatButton = new GUIComponent(TYPE_BUTTON,0,"Flat",font);
    colorButton = new GUIComponent(TYPE_BUTTON,0,"Colors",font);
    sliceButton = new GUIComponent(TYPE_BUTTON,0,"2D View",font);
    sonagramButton = new GUIComponent(TYPE_BUTTON,0,"Sonagram",font);
    userViewsButton = new GUIComponent(TYPE_BUTTON,0,"ViewPoints",font);
    audioButton = new GUIComponent(TYPE_BUTTON,0,"Audio",font);
    editModeButton = new GUIComponent(TYPE_BUTTON,0,"",font);
	editModeLabel = new GUIComponent(TYPE_LABEL,0,"Mode",font);

    closeButton = new GUIComponent(TYPE_BUTTON,0,"Close",font);
#if defined(GLUTWMCLOSEFUNC)
    closeButton->setActive(false);
#endif

    
    fieldButtons = NULL;
    
    // Creation de la fenetre
    createWindow();
    // Initialisation du visualisateur
    initViewer();
   
    surf = new SplineSurface();
    surf->calculateSplineSurface();
    se = new SplineEditorWindow(surf);
    cc = new ColorChooserWindow(&bgColor,&minColor,&maxColor,&lowTh,&highTh,this);
    svw = new SliceViewWindow(this);
    sona = new SonagramWindow(this);
    
    //ap = new AudioPlayer(this);
    //aw = new AudioWindow(this);

    if(audioFileName != NULL) setAudioFile(audioFileName);
	
    if (numMatrix == -1) SDIF_DATA = PARTIAL_DATA;
	else SDIF_DATA = CLASSIC_DATA;
	
	selectMatrix(numMatrix);
	//SDIF_DATA = checkData(selectedMatrix);
    
	freqFields = NULL;
    nbFreqFields = 0;
    reset();
    setComponents();
    loadViewPoints();
    vpw = new ViewPointsWindow(this);

    if(element<=1 || tps<=1){
		sonagramButton->setActive(false);
    }
    
    if(SDIF_DATA == PARTIAL_DATA){
	movingModeLabel->setActive(false);
	movingModeButton->setActive(false);
	sliceButton->setActive(false);
	//linesButton->setActive(false);
	editAreaButton->setActive(false);
	editTimeFromButton->setActive(false);
	editTimeFromPButton->setActive(false);
	editTimeFromMButton->setActive(false);
	editTimeToButton->setActive(false);
	editTimeToPButton->setActive(false);
	editTimeToMButton->setActive(false);
	editElemFromButton->setActive(false);
	editElemFromPButton->setActive(false);
	editElemFromMButton->setActive(false);
	editElemToButton->setActive(false);
	editElemToPButton->setActive(false);
	editElemToMButton->setActive(false);
	editCurveButton->setActive(false);
	flatButton->setActive(false);	
	editModeButton->setActive(false);
    }

    
}

int SDIFViewerWindow::checkData(SVMatrix* m){
    if(m->getMatSignature()=='EASM') return PARTIAL_DATA;
    else return CLASSIC_DATA;
}

SDIFViewerWindow::~SDIFViewerWindow(){
    
    if(data!=NULL) delete[] data;
    
    delete se;
    delete cc;
    delete surf;
    delete svw;
    delete vpw;
    delete sona;

    //delete ap;
    //delete aw;
    
    delete[] refSurface;
    
    /*
    if() delete[] posY;
    if() delete[] scaleV;
    if() delete[] fieldNames;
    */

    if(fieldButtons != NULL) delete[] fieldButtons;
    delete panelN;
    delete panelS;
    delete panelW;
    delete panelE;
    delete fileLabel;
    delete beginTimeButton;
    delete endTimeLabel;
    delete durationButton;
    delete nbFrameLabel;
    delete matrixButton;
    delete timePreview;
    delete xParamButton;
    delete xParamLabel;
    delete eltPreview;
	delete movingModeButton;
    delete movingModeLabel;
    delete pointLabel;
    delete tLabel;
    delete eltLabel;
    delete settingsLabel;
	delete settingsButton;
    delete speedLabel;
    delete scaleELabel;
    delete scaleTLabel;
    delete scaleVLabel;
    delete speedWidget;
    delete scaleEWidget;
    delete scaleTWidget;
    delete scaleVWidget;
    delete editStepLabel;
    delete editStepPButton;
    delete editStepMButton;
    delete initButton;
    delete linesButton;
    delete saveButton;
    delete closeButton;
    delete editLabel;
    delete editButton;
    delete editAreaButton;
    delete editTimeFromButton;
    delete editTimeFromPButton;
    delete editTimeFromMButton;
    delete editTimeToButton;
    delete editTimeToPButton;
    delete editTimeToMButton;
    delete editElemFromButton;
    delete editElemFromPButton;
    delete editElemFromMButton;
    delete editElemToButton;
    delete editElemToPButton;
    delete editElemToMButton;
    delete editCurveButton;
    delete flatButton;
    delete colorButton;
    delete sliceButton;
    delete sonagramButton;
    delete userViewsButton;
    delete audioButton;
    delete editModeButton;
	delete editModeLabel;

    glutDestroyWindow(ID);
    
};

void SDIFViewerWindow::setAudioFile(char *fileName){
   // ap->setAudio(fileName);
}

/****************************************************************************************/

/////////////////////
// INITIALISATIONS //
/////////////////////

void SDIFViewerWindow::setComponents(){

    panelN->set(0,0,width,height-vp[1]-vp[3]);
    panelS->set(0,height-vp[1],width,vp[1]);
    panelW->set(0,height-vp[1]-vp[3],vp[0],vp[3]);
    panelE->set(vp[0]+vp[2],height-vp[1]-vp[3],width-vp[0]-vp[2],vp[3]);
    
    
    matrixButton->set(vp[0],10,60,space);
    saveButton->set(vp[0]+65,10,35,space);
    fileLabel->set(vp[0]+110,12,300,space);
    
	initButton->set(vp[0]+vp[2]-70,10,70,space);
    userViewsButton->set(vp[0]+vp[2]-145,10,70,space);
	
	closeButton->set(width-55,10,35,space);
    
	
	if (dataModified) saveButton->setSelected(true);
	else saveButton->setSelected(false);
	
	timePreview->set(vp[0],height-4*space-5,200,10);
    beginTimeButton->set(vp[0],height-3*space,95,space);
	if(timeParam==TIME_PARAM_BEGIN) beginTimeButton->setSelected(true);
	else beginTimeButton->setSelected(false);
    endTimeLabel->set(120,height-3*space,100,space);
    durationButton->set(vp[0],height-2*space,95,space);
	if(timeParam==TIME_PARAM_DUR) durationButton->setSelected(true);
	else durationButton->setSelected(false);
    nbFrameLabel->set(120,height-2*space,100,space);
    
	xParamLabel->set(vp[2]/2-50,height-5*space+30,70,space);
    xParamButton->set(vp[2]/2+30,height-5*space+30,90,space);
    eltPreview->set(vp[2]/2-50,height-5*space+60,170,10);
	
    // Fields
    int pos = 0;
    for(int i = 0; i < field; i++){
	fieldButtons[i]->set(vp[0]+vp[2]+10,height-(field+1-pos)*space,200,space);
	pos++;
    }
    pointLabel->set(vp[0]+vp[2]-100,height-5*space,100,2*space);
    tLabel->set(vp[0]+vp[2]-100,height-5*space+15,100,2*space);
    eltLabel->set(vp[0]+vp[2]-100,height-5*space+30,100,2*space);
    
    sliceButton->set(vp[0]+vp[2]-100,height-5*space+60,60,space);
    sonagramButton->set(vp[0]+vp[2]-165,height-5*space+60,60,space);
    
	
    // Settings
    pos = 3;
    int left = vp[0]+vp[2]+20;
    int right = width - 30;
    int textW = 100;
    int butH = 8;
    int butW = 15;
    int but1Y = (space - 2*butH)/4; 
    int but2Y = butH + 3*but1Y;
    settingsLabel->set(left,space*pos,textW,space);
    settingsButton->set(left+textW,space*pos+5,20,10);
	pos++;
    if(settings_on){
		speedLabel->set(left,space*pos+6,100,12);
		speedWidget->set(left+textW,space*pos+6,right-left-textW,12);
		pos++;
		movingModeLabel->set(left,space*pos,40,space);
		movingModeButton->set(left+70,space*pos+2,80,space-4);
		pos++; pos++;
		scaleVLabel->set(left,space*pos,textW,space);
		scaleVWidget->set(left+textW,space*pos+4,right-left-80,12);
		pos++;
		scaleTLabel->set(left,space*pos,textW,space);
		scaleTWidget->set(left+textW,space*pos+4,right-left-80,12);
		pos++;
		scaleELabel->set(left,space*pos,textW,space);
		scaleEWidget->set(left+textW,space*pos+4,right-left-80,12);
		pos++; pos++;
		colorButton->set(left+4,space*pos+2,40,space-4);
		linesButton->set(left+50,space*pos+2,60,space-4);
		audioButton->set(left+116,space*pos+2,40,space-4);
		pos++;
	}
	pos++;
    editLabel->set(left,space*pos,textW,space);
    editButton->set(left+textW,space*pos+5,20,10);
	pos++;
	if(edition_on){
		editModeLabel->set(left,space*pos,80,space-4);
		editModeButton->set(left+60,space*pos,80,space-4);
		if(editMode == SPLINE_EDIT){
			pos++;
			editCurveButton->set(left+60,space*pos+2,80,space-4);
		}
		pos++;
		pos++;
		editTimeFromButton->set(left,space*pos,textW,space);
		editTimeFromPButton->set(right,space*pos+but1Y,butW,butH);
		editTimeFromMButton->set(right,space*pos+but2Y,butW,butH);
		pos++;
		editTimeToButton->set(left,space*pos,textW,space);
		editTimeToPButton->set(right,space*pos+but1Y,butW,butH);
		editTimeToMButton->set(right,space*pos+but2Y,butW,butH);
		pos++;
		editElemFromButton->set(left,space*pos,textW,space);
		editElemFromPButton->set(right,space*pos+but1Y,butW,butH);
		editElemFromMButton->set(right,space*pos+but2Y,butW,butH);
		pos++;
		editElemToButton->set(left,space*pos,textW,space);
		editElemToPButton->set(right,space*pos+but1Y,butW,butH);
		editElemToMButton->set(right,space*pos+but2Y,butW,butH);
		pos++;
		editAreaButton->set(left,space*pos+2,80,space-4);
		pos++;
		pos++;
		editStepLabel->set(left,space*pos,textW,space);
		editStepPButton->set(right,space*pos+but1Y,butW,butH);
		editStepMButton->set(right,space*pos+but2Y,butW,butH);
		pos++;
		flatButton->set(left,space*pos+2,40,space-4);
	}
}

void SDIFViewerWindow::drawGUI() {
	char txt[64];
    // Cadre
    panelN->draw();
    panelS->draw();
    panelW->draw();
    panelE->draw();
	
	fileLabel->draw();
	matrixButton->draw();
	saveButton->draw();
	if(closeButton->isActive()) closeButton->draw();
    timePreview->draw();

	// BEGIN TIME
    if(SDIF_DATA == CLASSIC_DATA) sprintf(txt, "Begin Time %.3f",data[beginT].time);
    else sprintf(txt, "First %.3f",data[beginT].time);
    beginTimeButton->setText(txt);
    beginTimeButton->draw();
	// END TIME
    if(SDIF_DATA == CLASSIC_DATA) sprintf(txt, "End Time %.3f",data[endT].time);
    else sprintf(txt, "Last %.3f", data[endT].time);
    endTimeLabel->setText(txt);
    endTimeLabel->draw();
    // DURATION
    sprintf(txt, "Duration %.3f", data[endT].time-data[beginT].time);
    durationButton->setText(txt);
    durationButton->draw();
    // NB FRAMES
    if(SDIF_DATA == CLASSIC_DATA) sprintf(txt,"%i Frames", endT-beginT+1);
    else sprintf(txt,"%i Partials", endT-beginT+1);
    nbFrameLabel->setText(txt);
    nbFrameLabel->draw();
		
	// X PARAMETER
    xParamButton->setText(fieldNames[xParam]);
    xParamButton->draw();
    xParamLabel->draw();
	eltPreview->draw();
	
	initButton->draw();
	
	// SETTINGS
    settingsLabel->draw();
	settingsButton->draw();
	if(settings_on){
		sprintf(txt,"Speed %.4f",step);
		speedLabel->setText(txt);
		speedLabel->draw();
		speedWidget->draw();
		scaleVLabel->draw();
		scaleVWidget->draw();
		scaleTLabel->draw();
		scaleTWidget->draw();
		sprintf(txt,"%s scale",fieldNames[xParam]);
		scaleELabel->setText(txt);
		scaleELabel->draw();
		scaleEWidget->draw();
		// VIEW MODE
		if(SDIF_DATA == CLASSIC_DATA){
			movingModeLabel->draw();
			if(mode == MODE_VIEW) strcpy(txt,"FREE");
			else if(mode == MODE_EDIT) strcpy(txt,"POINT-BASED");
			movingModeButton->setText(txt);
			movingModeButton->draw();
		}
		// LINES
		if(SDIF_DATA == PARTIAL_DATA){
			strcpy(txt,"Points");
		} else {
			if(timeLines) strcpy(txt,"GRID/Lines");
			else strcpy(txt,"Grid/LINES");
		}
		linesButton->setText(txt);
		linesButton->draw();
		colorButton->draw();
		if(audioButton->isActive()) audioButton->draw();
	}
	
	// EDITION
    editLabel->draw();
	editButton->draw();
	
	if (edition_on) {
		float n = log10(editStep);
		sprintf(txt,"Edit step 10e%.0f",n);
		editStepLabel->setText(txt);
		editStepLabel->draw();
		editStepPButton->draw();	
		editStepMButton->draw();	
		editModeLabel->draw();
		if(SDIF_DATA == CLASSIC_DATA){
			if(editMode == SPLINE_EDIT) strcpy(txt,"Spline surface");
			else if(editMode == REF_EDIT) strcpy(txt,"Model surface");
			else strcpy(txt,"Basic");
			editModeButton->setText(txt);
			editModeButton->draw();
			if(showEditZone) strcpy(txt,"Hide area");
			else strcpy(txt,"Show area");
			editAreaButton->setText(txt);
			editAreaButton->draw();
			editTimeFromPButton->draw();
			editTimeFromMButton->draw();
			float f;
			if (selectedT-editWidthB >= 0) f = data[selectedT-editWidthB].time;
			else f = data[0].time;
			sprintf(txt,"from %.4f (%i frames)",f,editWidthB);
			editTimeFromButton->setText(txt);
			editTimeFromButton->draw();
			editTimeToPButton->draw();
			editTimeToMButton->draw();
			if (selectedT+editWidthF < tps) f = data[selectedT+editWidthF].time;
			else f = data[tps-1].time;
			sprintf(txt,"to %.4f (%i frames)",data[selectedT+editWidthF].time,editWidthF);
			editTimeToButton->setText(txt);
			editTimeToButton->draw();
			editElemFromPButton->draw();
			editElemFromMButton->draw();
			sprintf(txt,"min -%i",editWidthL);
			editElemFromButton->setText(txt);
			editElemFromButton->draw();
			editElemToPButton->draw();
			editElemToMButton->draw();
			sprintf(txt,"max +%i",editWidthR);
			editElemToButton->setText(txt);
			editElemToButton->draw();
			flatButton->draw();
			if(editMode == SPLINE_EDIT) editCurveButton->draw();
		}
		
    }
	
	//Fields et valeurs
    if(selectedT!=-1 && selectedElem != -1){
		// infos sur le point
		pointLabel->draw();
		if(SDIF_DATA == PARTIAL_DATA) sprintf(txt, " Partial %d", selectedT);
		else sprintf(txt, " Time %d = %.4f", selectedT, data[selectedT].time);
		tLabel->setText(txt);
		tLabel->draw();
		sprintf(txt, " Point %d", selectedElem);
		eltLabel->setText(txt);
		eltLabel->draw();
		// fields
		for(int i = 0; i< field; i++){
			if(fieldPrintFormat(i+1)==MC_FORMAT) 
				sprintf(txt, "%s = %d mc", fieldNames[i+1], FreqToMidicents(data[selectedT].values[selectedElem][i+1]));
			else	
				sprintf(txt, "%s = %.6f", fieldNames[i+1], data[selectedT].values[selectedElem][i+1]);
			fieldButtons[i]->setText(txt);
			fieldButtons[i]->draw();
		}
    }
	
	// SLICES
    if(SDIF_DATA == CLASSIC_DATA) sliceButton->draw();
    // SONAGRAM
    if(sonagramButton->isActive()) sonagramButton->draw();
    // USER VIEWS
    userViewsButton->draw();
}

/////////////
// ACTIONS //
/////////////
void SDIFViewerWindow::selectAction(int x, int y){
	if(timePreview->inside(x,y)) {
		click = CLICK_WIDGET;
		clickedWidget = timePreview->getID();
	} else if(beginTimeButton->inside(x,y)){
	    setTimeParam(TIME_PARAM_BEGIN);
	} else if(durationButton->inside(x,y)){
	    setTimeParam(TIME_PARAM_DUR);
	} else if(matrixButton->inside(x,y)){
	    backToMatrixSelection();
	} else if(xParamButton->inside(x,y)){
	    changeXParam(UP);
	    shortCutParam = PARAM_XPARAM;
	} else if(eltPreview->inside(x,y)) {
		click = CLICK_WIDGET;
		clickedWidget = eltPreview->getID();
	} else if(settingsButton->inside(x,y)){
	    if (settings_on) { 
			settings_on = false;
			settingsButton->setText("+");
		} else {
			settings_on = true;
			settingsButton->setText("-");
		}
		setComponents();
	}else if(editButton->inside(x,y)){
	    if (edition_on) { 
			edition_on = false;
			editButton->setText("+");
		} else {
			edition_on = true;
			editButton->setText("-");
		}
		setComponents();
	} else if(initButton->inside(x,y)){
	    initView();
	} else if(saveButton->inside(x,y)){
	    save();
	} else if(closeButton->inside(x,y)){
	    close();
	} else if(sliceButton->inside(x,y)){
	    openSliceWindow();
	} else if(sonagramButton->inside(x,y)){
	    openSonagram();
	} else if(userViewsButton->inside(x,y)){
	    openViewPointsWindow();
	} else {
	    for(int i = 0; i < field; i++){
			if(fieldButtons[i]->inside(x,y)){
				selectField(i+1);
			}
		}
		if(settings_on) {
			if (speedWidget->inside(x,y)){
				click = CLICK_WIDGET;
				clickedWidget = speedWidget->getID();
			} else if (scaleTWidget->inside(x,y)){
				click = CLICK_WIDGET;
				clickedWidget = scaleTWidget->getID();
			} else if (scaleEWidget->inside(x,y)){
				click = CLICK_WIDGET;
				clickedWidget = scaleEWidget->getID();
			} else if (scaleVWidget->inside(x,y)){
				click = CLICK_WIDGET;
				clickedWidget = scaleVWidget->getID();
			} else if(linesButton->inside(x,y)){
				setLinesDrawing(!timeLines);
			} else if(colorButton->inside(x,y)){
				openColorChooser();
			} else if(movingModeButton->inside(x,y)){
				changeViewMode();
			} else if(audioButton->inside(x,y)){
				openAudioWindow();
			}
		}
		if(edition_on) {
			if(editAreaButton->inside(x,y)){
				setEditZoneVisible();
			} else if(editModeButton->inside(x,y)){
				changeEditMode();
			} else if(editStepPButton->inside(x,y)){
				changeEditP(1);
			} else if(editStepMButton->inside(x,y)){
				changeEditP(-1);
			} else if(editTimeFromPButton->inside(x,y)){
				changeEditWidthB(1);
				shortCutParam = PARAM_EAREA_FROM;
			} else if(editTimeFromMButton->inside(x,y)){
				changeEditWidthB(-1);
				shortCutParam = PARAM_EAREA_FROM;
			} else if(editTimeToPButton->inside(x,y)){
				changeEditWidthF(1);
				shortCutParam = PARAM_EAREA_TO;
			} else if(editTimeToMButton->inside(x,y)){
				changeEditWidthF(-1);
				shortCutParam = PARAM_EAREA_TO;
			} else if(editElemFromPButton->inside(x,y)){
				changeEditWidthL(1);
				shortCutParam = PARAM_EAREA_MIN;
			} else if(editElemFromMButton->inside(x,y)){
				changeEditWidthL(-1);
				shortCutParam = PARAM_EAREA_MIN;
			} else if(editElemToPButton->inside(x,y)){
				changeEditWidthR(1);
				shortCutParam = PARAM_EAREA_MAX;
			} else if(editElemToMButton->inside(x,y)){
				changeEditWidthR(-1);
				shortCutParam = PARAM_EAREA_MAX;
			} else if((editMode == SPLINE_EDIT) && editCurveButton->inside(x,y)){
				openSplineEditor();
			} else if(flatButton->inside(x,y)){
				flatEditZone();
			}
		}
	}
    glutPostRedisplay();
}

void SDIFViewerWindow::widgetMouseMoved(int dx, int dy){
	if (clickedWidget == timePreview->getID()){
		int var = (int)(abs(dx)*tps/timePreview->width());
		if(dx!=0 && var==0) var = 1;
		if(timeParam == TIME_PARAM_BEGIN){
			changeBeginTime(dx,var);
		} else if(timeParam == TIME_PARAM_DUR){
			changeDuration(dx,var);
		}
	} else if (clickedWidget == eltPreview->getID()){
		int var = (int)(abs(dx)*element/eltPreview->width());
		if(dx!=0 && var==0) var = 1;
		changeElemRange(dx,var);
	} else if (clickedWidget == speedWidget->getID()){
		if (dx > 0) {
			speedUp(); 
		} else speedDown();
	} else if (clickedWidget == scaleVWidget->getID()){
		if (dx > 0) {
			changeScaleV(1);; 
		} else changeScaleV(-1);
	} else if (clickedWidget == scaleEWidget->getID()){
		if (dx > 0) {
			changeScaleE(1);; 
		} else changeScaleE(-1);
	} else if (clickedWidget == scaleTWidget->getID()){
		if (dx > 0) {
			changeScaleT(1);; 
		} else changeScaleT(-1);
	}
}

/* Fermeture du visualisateur */
void SDIFViewerWindow::close(){
    // Fermeture de la fen�tre principale (selection de matrice)
    // qui d�truira le visualisateur.
    mainWin->close();
}

void SDIFViewerWindow::backToMatrixSelection(){
    // Affichage de la fenetre principale de selection de matrice
    // et fermeture de la fenetre du visualisateur.
    glutSetWindow(mainWin->getID());
    glutShowWindow();
    hideWindow();
    se->hideWindow();
    cc->hideWindow();
    svw->hideWindow();
    vpw->hideWindow();
    sona->hideWindow();
    //aw->hideWindow();
    glutSetWindow(mainWin->getID());
    //mainWin->setEditorID(-1);
    //closeMe(ID);
}

/* Initialisation openGL */
void SDIFViewerWindow::initViewer()   {
    glShadeModel(GL_SMOOTH);			// Enable Smooth Shading
    //glClearColor(bgColor.r, bgColor.g, bgColor.b, 0.5f); 	// Black ground
    glClearDepth(1.0f);				// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);			// The Type Of Depth Testing To Do
    glEnable ( GL_COLOR_MATERIAL );
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glMatrixMode(GL_PROJECTION);
    setViewport();
}

/* redimensionnement de la fenetre */
void SDIFViewerWindow::reshape(int w, int h){
    width = w;
    height = h;
    setViewport();
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


void SDIFViewerWindow::setViewport(){

    vp[0]=(int) (20);
    vp[1]=(int) (5*space);
    vp[2]=(int) (0.9*width-120);
    vp[3]=(int) (height-vp[1]-2*space);
    
}

void SDIFViewerWindow::initViewerAttributes(){
    selectedStream = NULL;
    selectedMatrix = NULL;
    selectedField = -1;
    colorParam = -1;
    selectedT = 0;
    selectedElem = 0;
    selectedMatrixNum=-1;
    xParam = 0;
    tps = 0;
    element = 0;
    field = 0;
    data = NULL;
    regularGridSize = true;
    max = min = 0;
    xmax = xmin = 0;
    scaleE = scaleT = 1;
    scaleV = NULL;
    beginT = endT = 0;
	maxElt = 0;
}

void SDIFViewerWindow::updateRefSurface(){
    int t, e; // positions reelles dans data
    int i, j;
	float minZone = FLT_MAX;
    refSurface = new float*[editWidthF + editWidthB +1];
    for(i = 0; i<editWidthF + editWidthB +1; i++){
	t = selectedT-editWidthB+i;
	refSurface[i] = new float[editWidthL + editWidthR +1];
	for(j = 0; j<editWidthL + editWidthR +1; j++){
	    e = selectedElem-editWidthL+j;
	    if(t>=0 && t<tps && e>=0 && e<data[t].dimension){
		if((t != selectedT || e != selectedElem) && data[t].values[e][selectedField] < minZone) minZone = data[t].values[e][selectedField];
	    }
	}
    }
   
    for(i = 0; i<editWidthF + editWidthB +1; i++){
	t = selectedT-editWidthB+i;
	for(j = 0; j<editWidthL + editWidthR +1; j++){
	    e = selectedElem-editWidthL+j;
	    if(t == selectedT && e == selectedElem){
		refSurface[i][j] = 1;
	    } else if(t>=0 && t<tps && e>=0 && e<data[t].dimension){
		refSurface[i][j] = (data[t].values[e][selectedField]-minZone)/
		    (data[selectedT].values[selectedElem][selectedField]-minZone);
	    } else {
		refSurface[i][j] = 0;
	    }
	}
    }
}


void SDIFViewerWindow::initView(){
    if(SDIF_DATA == PARTIAL_DATA) view = TIME_VIEW;
    else if(element == 1) view = TIME_VIEW;
    else view = ELEM_VIEW;
    
    float minT,maxT;
    minT = minTime();
    maxT = maxTime();
    
    if(view == TIME_VIEW){
	posX = (minT+(maxT-minT)/2)*scaleT;
	posZ = (xmax)*scaleE+scaleReference;
    } else if(view == ELEM_VIEW){
	posZ = (xmin+(xmax-xmin)/2)*scaleE;
	posX = minT*scaleT - scaleReference;
    }
    
    posY[selectedField-1] = (min+(max-min)/2)*scaleV[selectedField-1];
    if((beginT != endT ) && (element > 1) && SDIF_DATA==CLASSIC_DATA) aX = 10;
    else aX = 0;
    aY = 0;
    glutPostRedisplay();

}




/* Selection d'une matrice */
void SDIFViewerWindow::selectMatrix(int matNum){
	selectedMatrix = sdd->getMatrix(matNum);
    selectedMatrixNum = matNum;
    // Remplissage du tableau de donn�es avec la nouvelle matrice
    int err = setData();
    if(err == -1){
	cout << "Error reading matrix data";
	return;
    }
}

void SDIFViewerWindow::reset(){
    
    if(SDIF_DATA == PARTIAL_DATA){
	selectedField = 3;
	setColorParam(2);
	xParam = 2;
    } else {
	selectedField = 1;
	setColorParam(1);
	xParam = 0;	
    }

    // Reglage echelle du temps
    float minT,maxT;
    minT = minTime();
    maxT = maxTime();
    if (minT==maxT) scaleT = 1;
    else scaleT = 2*scaleReference / (maxT-minT);

    // reglage echelle des elements des matrices
    processXData();
    
    // reglages des echelles des valeurs
    
    float Min,Max;
    for(int f = 1; f<=field ; f++){
	Min = getMinValue(f);
	Max = getMaxValue(f);
	if (Min==Max) scaleV[f-1] = 1;
	else scaleV[f-1] = scaleReference / ( Max-Min ) ;
    }
    
    editPrecision = 1;
    processGridData();
    posY = new float[field];
    initYInitValues();
    initView();
    if(element>1) mode = MODE_VIEW;
    else mode = MODE_EDIT;
    fieldNames = new char*[field+1];
    fieldNames[0]="Matrix Elements";
	int i;
    for(i = 1; i <= field; i++) fieldNames[i] = selectedMatrix->getColName(i-1);
    fieldButtons = new GUIComponent*[field];
    for(i = 0; i <field; i++) fieldButtons[i] = new GUIComponent(TYPE_BUTTON,i,"",font);
	fieldButtons[selectedField-1]->setSelected(true);
    string str;
    str = selectedMatrix->getSigText();
    str += " - ";
    str += selectedMatrix->getName();
    fileLabel->setText(str);
    setComponents();
    // update audio window with new parameters
   // aw->reset();
    cc->reset();
    
    updateRefSurface();
    
    glutPostRedisplay();
}


int SDIFViewerWindow::setData(){
    int i,j;
    if(data!=NULL) delete[] data;
    tps = selectedMatrix->getNbTimes();
    beginT = 0;
    endT = tps-1;
    element = selectedMatrix->getNbRowMax();
	maxElt = selectedMatrix->getNbRowMax();
    int element_var;
    regularGridSize = true;
    field = selectedMatrix->getNbCol();
    scaleV = new float[field];
    for(i=0 ; i< field; i++) scaleV[i]=1;
    data = new matrixValues[tps];
    for(i = 0; i< tps; i++){
	data[i].time = selectedMatrix->getTime(i);
	element_var = selectedMatrix->getNbElem(i);
	if(element_var != element) regularGridSize = false;
	data[i].dimension = element_var;
	data[i].values = new float*[element_var];
	for(j=0;j<element_var;j++){
	    data[i].values[j]=new float[field+1];
	    data[i].values[j][0] = j;
	}
    }
    int err = sdd->getMatrixData(data,selectedMatrix);
    if(err == -1){
	return err;
    }
    //....///
    return 0;
}


void SDIFViewerWindow::processGridData(){
    max = getMaxValue(selectedField);
    min = getMinValue(selectedField);
    calculateEditStep();
}

void SDIFViewerWindow::initYInitValues(){
    for(int f = 0; f<field; f++){
	float Min = getMinValue(f+1);
	float Max = getMaxValue(f+1);
	posY[f] = (Min+(Max-Min)/2)*scaleV[f];
    }
}

void SDIFViewerWindow::processXData(){
    if(element==1) {
	scaleE = 1;
	//view = TIME_VIEW;
    } else {
	xmin = getMinValue(xParam);
	xmax = getMaxValue(xParam);
	scaleE = 2*scaleReference / (xmax-xmin);
	if(SDIF_DATA == PARTIAL_DATA) scaleE /= 10;
	//view = ELEM_VIEW;
	posZ = (xmin+(xmax-xmin)/2)*scaleE;
    }
}

float SDIFViewerWindow::getMinValue(int Field){
    float minValue = FLT_MAX;
    float val;
    for(int t = 0;  t < tps; t++){
	for(int e=0; e < data[t].dimension;e++){
	    val = data[t].values[e][Field];
	    if(SDIF_DATA == PARTIAL_DATA && e>0) val += data[t].values[0][Field];
	    if(val<minValue) minValue = val;
	}
    }
    return minValue;
}

float SDIFViewerWindow::getMaxValue(int Field){
    float maxValue = -FLT_MAX;
    float val;
    for(int t = 0;  t < tps; t++){
	for(int e=0; e < data[t].dimension;e++){
	    val = data[t].values[e][Field];
	    if(SDIF_DATA == PARTIAL_DATA && e>0) val += data[t].values[0][Field];
	    if(val>maxValue) maxValue = val;
	}
    }
    return maxValue;
}

void SDIFViewerWindow::calculateEditStep(){
    float s = log10(scaleV[selectedField-1]);
    editStep = pow(10,-(floor(s)+editPrecision));
}

/* Ecriture des donnees dans le fichier */
void SDIFViewerWindow::save(){
    if(selectedMatrix!=NULL && data!=NULL){
	int err = sdd->writeMatrixData(data,selectedMatrix);
	if(err == -1) cout << "Error : file could not be saved";
	saveViewPoints();
    }
	saveButton->setSelected(false);
	dataModified = false;
}
void SDIFViewerWindow::loadViewPoints(){

    for(int n= 0; n<sdd->getNbViewPoints(); n++){
	viewpoint vp = *sdd->getViewPoint(n);
	if(vp.matrixNum == selectedMatrixNum) viewpoints.push_back(vp);
    }
    cout << viewpoints.size() << " viewpoints loaded" << endl;
}

void SDIFViewerWindow::saveViewPoints(){
    
    sdd->removeAllVP(selectedMatrixNum);
    for(int i=0; i<viewpoints.size();i++){
	sdd->addVP(viewpoints[i]);
    }
    sdd->writeViewPointsFile();
}

///////////////////////
// UI EVENTS PROCESS //
///////////////////////
/* Reponse aux evenements des touches ASCII du clavier */
void SDIFViewerWindow::keyPressed( unsigned char key, int x, int y ){
    switch ( key ) {
	case 27:        // Escape key
	    Window::quit();
	    break;
	/* 
	case 13:	// enter key
	    if(ap->haveAttachedSound()) ap->play();
	    break; */
	case '+':
	    editUp();
	    break;
	case '-':
	    editDown();
	    break;
	case 'p':
	case 'P':
	    if(SDIF_DATA==PARTIAL_DATA){
		if(selectedElem == 0) selectedElem = 1;
		else selectedElem = 0;
	    }
	    break;
	case 'f':
	case 'F':
	    updateFrequencyFields(selectedField);
	    break;
	case ' ':
	    sound();
	    break;
	default:
	    break;
    }
    glutPostRedisplay();
}


/* touches speciales du clavier */
void SDIFViewerWindow::specialPressed( int a_keys, int x, int y ){
	int m = glutGetModifiers();
        switch ( a_keys ) {
	case GLUT_KEY_UP:
	    if(m == GLUT_ACTIVE_CTRL) selectedPtFwd();
	    else if(mode == MODE_EDIT || m == GLUT_ACTIVE_SHIFT) moveToSelectedPt();
	    else cameraFwd();
	    break;
	case GLUT_KEY_DOWN:
	    if(m == GLUT_ACTIVE_CTRL) selectedPtBwd();
	    else if(mode == MODE_EDIT || m == GLUT_ACTIVE_SHIFT) moveAwayFromSelectedPt();
	    else cameraBwd();
	    break;

	case GLUT_KEY_PAGE_UP:
	    cameraUp();
	    break;
	
	case GLUT_KEY_PAGE_DOWN:
	    cameraDown();
	    break;

	case GLUT_KEY_LEFT:
	    if(m == GLUT_ACTIVE_CTRL) selectedPtL();
	    else if(mode == MODE_EDIT || m == GLUT_ACTIVE_SHIFT) turnLAroundSelectedPt();
	    else cameraLeft();
	    break;
	
	case GLUT_KEY_RIGHT:
	    if(m == GLUT_ACTIVE_CTRL) selectedPtR();
	    else if(mode == MODE_EDIT || m == GLUT_ACTIVE_SHIFT) turnRAroundSelectedPt();
	    else cameraRight();
	    break;

	case GLUT_KEY_HOME:
	    if(selectedField-1 <= 0) selectField(field);
	    else selectField(selectedField - 1);
	    break;
	case GLUT_KEY_END:
	    selectField((selectedField)%(field) + 1);
	    break;
	
	case GLUT_KEY_F3:
	    switch(shortCutParam){
		case PARAM_SPEED:
		    speedDown();
		    break;
		case PARAM_SCALEV:
		    changeScaleV(-1);
		    break;
		case PARAM_SCALET:
		    changeScaleT(-1);
		    break;
		case PARAM_SCALEE:
		    changeScaleE(-1);
		    break;
		case PARAM_EDITP:
		    changeEditP(-1);
		    break;
		case PARAM_EAREA_FROM:
		    changeEditWidthB(-1);
		    break;
		case PARAM_EAREA_TO:
		    changeEditWidthF(-1);
		    break;
		case PARAM_EAREA_MIN:
		    changeEditWidthL(-1);
		    break;
		case PARAM_EAREA_MAX:
		    changeEditWidthR(-1);
		    break;
		case PARAM_XPARAM:
		    changeXParam(DOWN);
		    break;
		default :
		    break;
	    }
	    glutPostRedisplay();
	    break;
	    
	case GLUT_KEY_F4:
	    switch(shortCutParam){
		case PARAM_SPEED:
		    speedUp();
		    break;
		case PARAM_SCALEV:
		    changeScaleV(1);
		    break;
		case PARAM_SCALET:
		    changeScaleT(1);
		    break;
		case PARAM_SCALEE:
		   changeScaleE(1);
		    break;
		case PARAM_EDITP:
		    changeEditP(1);
		    break;
		case PARAM_EAREA_FROM:
		    changeEditWidthB(1);
		    break;
		case PARAM_EAREA_TO:
		    changeEditWidthF(1);
		    break;
		case PARAM_EAREA_MIN:
		    changeEditWidthL(1);
		    break;
		case PARAM_EAREA_MAX:
		    changeEditWidthR(1);
		    break;
		case PARAM_XPARAM:
		    changeXParam(UP);
		    break;
		default :
		    break;
	    }
	    break;

	default:
	    break;
	}
	glutPostRedisplay();
}

void SDIFViewerWindow::mousePressed(int mouse, int state, int x, int y){
    if(state==GLUT_DOWN){
	mouseX = x;
	mouseY = y;
	if(x>=vp[0] && y>=height-(vp[3]+vp[1]) && x<=vp[0]+vp[2] && y<=height-vp[1]){
	    if (glutGetModifiers() == GLUT_ACTIVE_ALT) click = EDIT_GRID;
		else click = CLICK_GRID;
	    selectPt(x,y);
	} else {
	    click = CLICK_NONE;
	    selectShortCutParam(x,y);
	    selectAction(x,y);
	}
    }
}

void SDIFViewerWindow::mouseMoved(int x, int y){
    if(click == CLICK_GRID){
		aY += x-mouseX;
		aX += y-mouseY;
    } else if(click == CLICK_WIDGET){
		int dx = x - mouseX;
		int dy = y - mouseY;
		widgetMouseMoved(dx, dy);
	} else if (click == EDIT_GRID) { // && glutGetModifiers() == GLUT_ACTIVE_ALT) {
		//int dx = x - mouseX;
		int dy = y - mouseY;
		mouseEdit(dy);
	}
	mouseX=x;
    mouseY=y;
    glutPostRedisplay();
}


void SDIFViewerWindow::selectShortCutParam(int x, int y){
    if(xParamButton->inside(x,y) || xParamLabel->inside(x,y)){
	shortCutParam = PARAM_XPARAM;
    } else if(editTimeFromPButton->inside(x,y) || editTimeFromMButton->inside(x,y) || editTimeFromButton->inside(x,y)){
	shortCutParam = PARAM_EAREA_FROM;
    } else if(editTimeToPButton->inside(x,y) || editTimeToMButton->inside(x,y) || editTimeToButton->inside(x,y)){
	shortCutParam = PARAM_EAREA_TO;
    } else if(editElemFromPButton->inside(x,y)  || editElemFromMButton->inside(x,y) || editElemFromButton->inside(x,y)){
	shortCutParam = PARAM_EAREA_MIN;
    } else if(editElemToPButton->inside(x,y)  || editElemToMButton->inside(x,y) || editElemToButton->inside(x,y)){
	shortCutParam = PARAM_EAREA_MAX;
    } 
}


/****************************************************************************************/



void SDIFViewerWindow::updateFrequencyFields(int newfield){
    int i,j;
    int fieldPos = -1;
    for(i = 0; i<nbFreqFields; i++){
	if(freqFields[i] == newfield){
	    fieldPos = i;
	    break;
	}
    }
    if(fieldPos==-1){
	// field not found -> add it
	int *tmp = new int[nbFreqFields+1];
	for(j = 0; j< nbFreqFields; j++) tmp[j] = freqFields[j];
	tmp[nbFreqFields] = newfield;
	delete[] freqFields;
	freqFields = tmp;
	nbFreqFields ++;
    } else {
	// field found -> remove it
	if(nbFreqFields == 1){
	    delete[] freqFields;
	    freqFields = NULL;
	} else {
	    int *tmp = new int[nbFreqFields-1];
	    for(j = 0; j< fieldPos; j++) tmp[j] = freqFields[j];
	    for(j = i; j< nbFreqFields-1; j++) tmp[j] = freqFields[j+1];
	    delete[] freqFields;
	    freqFields = tmp;
	}
	nbFreqFields --;	
    }
}


int SDIFViewerWindow::fieldPrintFormat(int fieldNum){
    for(int i = 0; i<nbFreqFields; i++){
	if(freqFields[i] == fieldNum) return MC_FORMAT;
    }
    return FLT_FORMAT;
}


void SDIFViewerWindow::selectPt(int x, int y){
    
    double matProj[16];
    GLint hits;
    GLint viewport[4] ;
        glViewport(vp[0],vp[1],vp[2],vp[3]);
	    GLsizei sizeSelectBuf = 1024;
	    GLuint *selectBuf = (GLuint*) malloc(1024*sizeof(GLuint));
	        glGetDoublev(GL_PROJECTION_MATRIX, matProj);
		// Copie de la matrice de projection
		// Recuperation du viewport
		glGetIntegerv(GL_VIEWPORT,viewport) ;
		// Determination du buffer de selection :
		glSelectBuffer(sizeSelectBuf, selectBuf) ;
		// Mode select : active le traitement de la pile de noms
		(void) glRenderMode(GL_SELECT) ;
		// Initialisation de la pile de noms
		glInitNames() ;
		//glPushName(0);
		// Creation de la matrice de projection pour la selection
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		// Generation d'une mini matrice de projection autour du curseur
		//gluPickMatrix((GLdouble) x, (GLdouble)(viewport[3]- y), 10 ,10 ,viewport) ;
		gluPickMatrix((GLdouble) x, (GLdouble)(height - y), 10 ,10 ,viewport) ;
		// Multiplication par la matrice de projection courante:
		glMultMatrixd(matProj);
		// Copie de la nouvelle matrice dans la matrice de projection
		//glGetDoublev(GL_PROJECTION_MATRIX, matrix);
		// Dessin de la scËne en mode selection (avec notre matrice)
		glMatrixMode(GL_MODELVIEW);
		drawPts();
		// Suppression de la matrice de selection
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		// Retablissement de la matrice de projection d'origine
		//memcpy(matrix,matProj,16*sizeof(double));
		// Rendu de la scene avec retour sur les objets atteints
		glMatrixMode(GL_MODELVIEW);
		hits = glRenderMode(GL_RENDER);
		// hits recupere le nombre d'objets rencontrÈs, et
		// les informations sur ceux-ci sont Ècrites dans le buffer de selection
		// Traitement du resultat de selection
		processHits(hits, selectBuf) ;

		glViewport(0,0,width,height);
}

/**
* Traitement du buffer de selection pour dÈterminer l'objet sÈlectionnÈ
 * Parametres :
 *    - hits : le nombre d'objets
 *    - buffer : le buffer de selection
 */
void SDIFViewerWindow::processHits(GLint hits, GLuint buffer[]){
    int  hit;
    unsigned int j;
    GLuint nbNames, *ptr ;
    float z1,zMin;
    //float z2;
    int x,y;
    //cout << "Nombre de points rencontres : " << hits << endl ;
    ptr = (GLuint *) buffer ;
    if (hits > 0 ){
	// Parcours du select buffer pour tous les objets rencontrÈs
	for (hit = 0 ; hit < hits ; hit++){
	    // premiere valeur : nombre de noms
	    nbNames = *ptr;
	    ptr++ ;
	    // Valeur suivante: z1 = profondeur min de l'objet
	    z1 = (float) *ptr/0x7fffffff;
	    // Valeur suivante : z2 profondeur max de l'objet
		ptr++ ;
	    //z2 = (float) *ptr/0x7fffffff;
	    // Initialisation du tableau pour stocker les noms
	    int* names = new int[nbNames];
	    // Enfin l'ensemble des noms pour cet objet
	    for (j = 0 ; j < nbNames; j++){
		ptr++ ;
		names[j]= *ptr;
	    }
	    // On effectue une comparaison des profondeurs pour garder la minimale
	    if (hit == 0 || z1 < zMin){
		zMin = z1;
		x = names[0];
		y = names[1];
	    }
	    			ptr++;
	}
	selectedT = x;
	selectedElem = y;
	
	if(click == EDIT_GRID) zpt = zMin;
	
	updateRefSurface();
	//cout << "Pt selectionne : " << x << " , " << y << endl;
    }
    glutPostRedisplay();
}


void SDIFViewerWindow::selectedPtL(){
    if(selectedElem>0){
	selectedElem --;
	if(mode==MODE_EDIT)
	    posZ -=(data[selectedT].values[selectedElem+1][xParam]-data[selectedT].values[selectedElem][xParam]) * scaleE;
	updateRefSurface();
    }
    if(autoSynth) sound();
}

void SDIFViewerWindow::selectedPtR(){
    if(selectedElem<data[selectedT].dimension-1 && selectedElem<maxElt-1){
	selectedElem ++;
	if(mode==MODE_EDIT)
	    posZ += (data[selectedT].values[selectedElem][xParam]-data[selectedT].values[selectedElem-1][xParam]) * scaleE;
	updateRefSurface();
    }
    if(autoSynth) sound();
}

void SDIFViewerWindow::selectedPtFwd(){
    int tmpT = selectedT+1;
    while ((tmpT <= endT) && (selectedElem > data[tmpT].dimension-1)){
	tmpT++;
    }
    if(tmpT <= endT){
	if(mode==MODE_EDIT)
	    posX += (data[tmpT].time-data[selectedT].time) * scaleT;
	selectedT = tmpT;
	updateRefSurface();
    }
    if(autoSynth) sound();
}

void SDIFViewerWindow::selectedPtBwd(){
    int tmpT = selectedT-1;
    while ((tmpT >= beginT) && (selectedElem > data[tmpT].dimension-1)){
	tmpT--;
    }
    if(tmpT>=beginT){
	if(mode==MODE_EDIT)
	    posX -= (data[selectedT].time-data[tmpT].time) * scaleT;
	selectedT = tmpT;
	updateRefSurface();
    }
    if(autoSynth) sound();
}	

void SDIFViewerWindow::cameraFwd(){
    posX += step;
}

void SDIFViewerWindow::moveToSelectedPt(){
    float mvX,mvY,mvZ;
    
    if(SDIF_DATA==PARTIAL_DATA){
	mvX = (data[selectedT].time+data[selectedT].values[selectedElem][1])*scaleT - posX;
	mvZ = (data[selectedT].values[selectedElem][xParam]+data[selectedT].values[0][xParam])*scaleE - posZ;
	mvY = (data[selectedT].values[selectedElem][selectedField]+data[selectedT].values[0][selectedField])*scaleV[selectedField-1] - posY[selectedField-1];
    } else {
	mvX = data[selectedT].time*scaleT - posX;
	mvZ = data[selectedT].values[selectedElem][xParam]*scaleE - posZ;
	mvY = data[selectedT].values[selectedElem][selectedField]*scaleV[selectedField-1] - posY[selectedField-1];
    }
    posX += 0.1 * mvX;
    posY[selectedField-1] += 0.1 * mvY;
    posZ += 0.1 * mvZ;
    
}

void SDIFViewerWindow::cameraBwd(){
    posX -= step;

}

void SDIFViewerWindow::moveAwayFromSelectedPt(){
    float mvX,mvY,mvZ;

    if(SDIF_DATA==PARTIAL_DATA){
	mvX = (data[selectedT].time+data[selectedT].values[selectedElem][1])*scaleT - posX;
	mvZ = (data[selectedT].values[selectedElem][xParam]+data[selectedT].values[0][xParam])*scaleE - posZ;
	mvY = (data[selectedT].values[selectedElem][selectedField]+data[selectedT].values[0][selectedField])*scaleV[selectedField-1] - posY[selectedField-1];
    } else {
	mvX = data[selectedT].time*scaleT - posX;
	mvZ = data[selectedT].values[selectedElem][xParam]*scaleE - posZ;
	mvY = data[selectedT].values[selectedElem][selectedField]*scaleV[selectedField-1] - posY[selectedField-1];
    }
    
    posX -= 0.1 * mvX;
    posZ -= 0.1 * mvZ;
    posY[selectedField-1] -= 0.1 * mvY;
}

void SDIFViewerWindow::cameraLeft(){
	posZ-= step;
}

void SDIFViewerWindow::cameraRight(){
 	posZ += step;
}

void SDIFViewerWindow::turnLAroundSelectedPt(){
    float e_x,e_z,cosG,sinG;
    float rot = 2*M_PI/100;
    cosG = cos(rot);
    sinG = sin(rot);
    if(SDIF_DATA == PARTIAL_DATA){
	e_x = cosG*(posX-(data[selectedT].time+data[selectedT].values[selectedElem][1])*scaleT)
	    + sinG*(posZ-(data[selectedT].values[selectedElem][xParam]+data[selectedT].values[0][xParam])*scaleE);
	e_z = -sinG*(posX-(data[selectedT].time+data[selectedT].values[selectedElem][1])*scaleT)
	    + cosG*(posZ-(data[selectedT].values[selectedElem][xParam]+data[selectedT].values[0][xParam])*scaleE);
	posX = e_x + (data[selectedT].time+data[selectedT].values[selectedElem][1])*scaleT;
	posZ = e_z + (data[selectedT].values[selectedElem][xParam]+data[selectedT].values[0][xParam])*scaleE;
    } else {
	e_x = cosG*(posX-data[selectedT].time*scaleT)
	    + sinG*(posZ-data[selectedT].values[selectedElem][xParam]*scaleE);
	e_z = -sinG*(posX-data[selectedT].time*scaleT)
	    + cosG*(posZ-data[selectedT].values[selectedElem][xParam]*scaleE);
	posX = e_x + data[selectedT].time*scaleT;
	posZ = e_z + data[selectedT].values[selectedElem][xParam]*scaleE;
    }
    aY -= 360*rot/(2*M_PI);    
}

void SDIFViewerWindow::turnRAroundSelectedPt(){
    float e_x,e_z,cosG,sinG;
    float rot = - 2*M_PI/100;
    cosG = cos(rot);
    sinG = sin(rot);
    if(SDIF_DATA == PARTIAL_DATA){
	e_x = cosG*(posX-(data[selectedT].time+data[selectedT].values[selectedElem][1])*scaleT)
	+ sinG*(posZ-(data[selectedT].values[selectedElem][xParam]+data[selectedT].values[0][xParam])*scaleE);
	e_z = -sinG*(posX-(data[selectedT].time+data[selectedT].values[selectedElem][1])*scaleT)
	    + cosG*(posZ-(data[selectedT].values[selectedElem][xParam]+data[selectedT].values[0][xParam])*scaleE);
	posX = e_x + (data[selectedT].time+data[selectedT].values[selectedElem][1])*scaleT;
	posZ = e_z + (data[selectedT].values[selectedElem][xParam]+data[selectedT].values[0][xParam])*scaleE;
	
    } else {
	e_x = cosG*(posX-data[selectedT].time*scaleT)
	    + sinG*(posZ-data[selectedT].values[selectedElem][xParam]*scaleE);
	e_z = -sinG*(posX-data[selectedT].time*scaleT)
	    + cosG*(posZ-data[selectedT].values[selectedElem][xParam]*scaleE);
   	posX = e_x + data[selectedT].time*scaleT;
	posZ = e_z + data[selectedT].values[selectedElem][xParam]*scaleE;
    }
    aY -= 360*rot/(2*M_PI);
}

void SDIFViewerWindow::cameraUp(){
    posY[selectedField-1]+=step;
    
}

void SDIFViewerWindow::cameraDown(){
    posY[selectedField-1]-=step;
}

void SDIFViewerWindow::changeEditMode(){
    editMode = (editMode+1)%3;
	setComponents();
}

void SDIFViewerWindow::editUp(){
    float c;
    int i,j;
    float ipt,jpt;
    i = 0;
    for(int t = selectedT-editWidthB; t<=selectedT+editWidthF; t++){
	j = 0;
	for(int e = selectedElem-editWidthL; e<=selectedElem+editWidthR; e++){
	    if(t>=0 && t<tps && e>=0 && e<data[t].dimension){
		if(editMode == SPLINE_EDIT){
		    if(t == selectedT) ipt = 0.5;
		    else if(t < selectedT) ipt = i/(float) (2*editWidthB);
		    else ipt = 0.5 + (i-editWidthB)/(float)(2*editWidthF);
		
		    if(e == selectedElem) jpt = 0.5;
		    else if(e <= selectedElem) jpt = j/(float) (2*editWidthL);
		    else jpt = 0.5 + (j-editWidthL)/(float)(2*editWidthR);

		
		    c = surf->findSplineAlt(ipt,jpt);
		} else if (editMode == REF_EDIT){
		  
		      c = refSurface[i][j];

		} else if(editMode == FLAT_EDIT){
		    c = 1;
		}
		
		data[t].values[e][selectedField] += editStep*c;
	    }
	    j++;
	}
	i++;
    }
    dataModified = true;
	saveButton->setSelected(true);

}



void SDIFViewerWindow::editDown(){
    float c;
    //int i,j;
    float ipt,jpt;
    //i = 0;
    for(int t = selectedT-editWidthB, i=0 ; t<=selectedT+editWidthF; t++, i++){
	//j = 0;
	for(int e = selectedElem-editWidthL, j=0; e<=selectedElem+editWidthR; e++, j++){
	    if(t>=0 && t<tps && e>=0 && e<data[t].dimension){
		if(editMode == SPLINE_EDIT){
		    if(t == selectedT) ipt = 0.5;
		    else if(t < selectedT) ipt = i/(float) (2*editWidthB);
		    else ipt = 0.5 + (i-editWidthB)/(float)(2*editWidthF);

		    if(e == selectedElem) jpt = 0.5;
		    else if(e <= selectedElem) jpt = j/(float) (2*editWidthL);
		    else jpt = 0.5 + (j-editWidthL)/(float)(2*editWidthR);

		    c = surf->findSplineAlt(ipt,jpt);
		    
		} else if(editMode == REF_EDIT){
		    c = refSurface[i][j];
		} else if(editMode == FLAT_EDIT){
		    c = 1;
		}
		data[t].values[e][selectedField] -= editStep*c;
	    }
	    //j++;
	}
	//i++;
    }
    dataModified = true;
	saveButton->setSelected(true);
}

void SDIFViewerWindow::mouseEdit(int dy){
	
	float editfactor = (zpt*dy*editStep/-2);
	//cout << editfactor << endl; 
	
    float c;
    //int i,j;
    float ipt,jpt;
    //i = 0;
    for(int t = selectedT-editWidthB, i=0 ; t<=selectedT+editWidthF; t++, i++){
	//j = 0;
	for(int e = selectedElem-editWidthL, j=0; e<=selectedElem+editWidthR; e++, j++){
	    if(t>=0 && t<tps && e>=0 && e<data[t].dimension){
		if(editMode == SPLINE_EDIT){
		    if(t == selectedT) ipt = 0.5;
		    else if(t < selectedT) ipt = i/(float) (2*editWidthB);
		    else ipt = 0.5 + (i-editWidthB)/(float)(2*editWidthF);

		    if(e == selectedElem) jpt = 0.5;
		    else if(e <= selectedElem) jpt = j/(float) (2*editWidthL);
		    else jpt = 0.5 + (j-editWidthL)/(float)(2*editWidthR);

		    c = surf->findSplineAlt(ipt,jpt);
		    
		} else if(editMode == REF_EDIT){
		    c = refSurface[i][j];
		} else if(editMode == FLAT_EDIT){
		    c = 1;
		}
		data[t].values[e][selectedField] += editfactor*c;
	    }
	    //j++;
	}
	//i++;
    }
    dataModified = true;
	saveButton->setSelected(true);
}




void SDIFViewerWindow::flatEditZone(){
    float value = data[selectedT].values[selectedElem][selectedField];
    for(int t = selectedT-editWidthB; t<=selectedT+editWidthF; t++){
	for(int e = selectedElem-editWidthL; e<=selectedElem+editWidthR; e++){
	    if(t>=0 && t<tps && e>=0 && e<data[t].dimension){
		data[t].values[e][selectedField] = value;
	    }
	}
    }
}

void SDIFViewerWindow::setEditZoneVisible(){
    
    showEditZone = !showEditZone;

}

void SDIFViewerWindow::changeEditWidthL(int v){
    if(selectedElem-(editWidthL+v) >= 0 && editWidthL+v >=0){
	editWidthL += v;
	updateRefSurface();
    }
}
void SDIFViewerWindow::changeEditWidthR(int v){
    if(selectedElem+(editWidthR+v) < element && editWidthR+v >= 0){
	editWidthR += v;
	updateRefSurface();
    }
}
void SDIFViewerWindow::changeEditWidthB(int v){
    if(selectedT-(editWidthB+v) >= 0 && editWidthB+v >=0){
	editWidthB += v;
	updateRefSurface();
    }
}
void SDIFViewerWindow::changeEditWidthF(int v){
    if(selectedT+(editWidthF+v) < tps && editWidthF+v >=0){
	editWidthF += v;
	updateRefSurface();
    }
}

void SDIFViewerWindow::selectField(int f){
    selectedField = f;
    //colorParam = f;
    setColorParam(f);
    processGridData();
    updateRefSurface();
	for(int i = 0; i < field; i++)
		fieldButtons[i]->setSelected(false);
	fieldButtons[f-1]->setSelected(true);
}

void SDIFViewerWindow::setTimeParam(int param){
    timeParam = param;
	setComponents();
}

void SDIFViewerWindow::changeXParam(int up_or_down){
    if(up_or_down == UP){
	xParam = (xParam+1)%(field+1);
    } else if(up_or_down == DOWN) {
	if(xParam==0) xParam = field;
	else xParam--;	
    }
    processXData();
}

void SDIFViewerWindow::changeViewMode(){

    if(mode==MODE_VIEW){
	mode = MODE_EDIT;
    } else if(mode==MODE_EDIT){
	mode = MODE_VIEW;
    }
}   

void SDIFViewerWindow::speedDown(){
    if(step <= 1 && step >= 0.0001) step/=2;
    else step -=1;
	speedWidget->setv2(step / 100);
}

void SDIFViewerWindow::speedUp(){
    if(step <= 0.5) step*=2;
    else if (step <= 100) step +=1;
	speedWidget->setv2(step / 100);
}

void SDIFViewerWindow::changeEditP(int v){
    editPrecision -= v;
    calculateEditStep();
}

void SDIFViewerWindow::changeScaleV(int v){
    if(v>0) scaleV[selectedField-1]*=1.5;
    else if(v<0) scaleV[selectedField-1]/=1.5;
    calculateEditStep();
	//scaleVWidget->setv2(scaleV[selectedField-1]);
}
void SDIFViewerWindow::changeScaleT(int v){
    if(v<0){
	if(posX>=(data[beginT].time*scaleT) && posX<=(data[endT].time*scaleT)) posX/= 1.5;
	else if (posX<(data[beginT].time*scaleT)) posX += data[beginT].time*scaleT/1.5 - data[beginT].time*scaleT;
	else posX += data[endT].time*scaleT/1.5 - data[endT].time*scaleT;
	scaleT/=1.5;
    } else if(v>0) {
	if(posX>(data[beginT].time*scaleT) && posX<(data[endT].time*scaleT)) posX*= 1.5;
	else if (posX<(data[beginT].time*scaleT)) posX += data[beginT].time*scaleT*1.5 - data[beginT].time*scaleT;
	else posX += data[endT].time*scaleT*1.5 - data[endT].time*scaleT;
	scaleT*=1.5;
    }
	//scaleTWidget->setv2(scaleT);
}
void SDIFViewerWindow::changeScaleE(int v){
    if(v<0){
	if(posZ>=xmin*scaleE && posZ<=xmax*scaleE) posZ/= 1.5;
	else if(posZ>xmax*scaleE) posZ += xmax*scaleE/1.5 - xmax*scaleE;
	else if(posZ<xmin*scaleE) posZ += xmin*scaleE/1.5 - xmin*scaleE;
	scaleE/=1.5;
	
    } else if(v>0) {
	if(posZ>=xmin*scaleE && posZ<=xmax*scaleE) posZ*= 1.5;
	else if(posZ>xmax*scaleE) posZ += xmax*scaleE*1.5 - xmax*scaleE;
	else if(posZ<xmin*scaleE) posZ += xmin*scaleE*1.5 - xmin*scaleE;
	scaleE*=1.5;
    }
	//scaleEWidget->setv2(scaleE);
}

void SDIFViewerWindow::setLinesDrawing(bool ln){
    timeLines = ln;
}

void SDIFViewerWindow::changeBeginTime(int d, int var){
    if(d<0){
	if(beginT-var<0) var = beginT;
	beginT -= var;
	endT -= var;
	posX -= (data[endT+var].time-data[endT].time) * scaleT;
	if(selectedT>endT){
	    selectedElem = 0;
	    selectedT = endT;
	    updateRefSurface();
	}

    } else if(d>0){
	if(endT+var>tps-1) var = tps-1-endT;
	beginT += var;
	endT += var;
	posX += (data[beginT].time-data[beginT-var].time) * scaleT;
	if(selectedT < beginT){
	    selectedElem = 0;
	    selectedT = beginT;
	    updateRefSurface();
	}
	
    }
	timePreview->setv1((data[beginT].time-selectedMatrix->getBeginTime())
						/(selectedMatrix->getEndTime()-selectedMatrix->getBeginTime()));
	timePreview->setv2((data[endT].time-selectedMatrix->getBeginTime())
						       /(selectedMatrix->getEndTime()-selectedMatrix->getBeginTime()));

}

void SDIFViewerWindow::changeDuration(int d, int var){

    if(d<0){
	if(endT-var<beginT) var = endT-beginT;
	endT -= var;
	if(selectedT>endT){
	    selectedT = endT;
	    selectedElem=0;
	    updateRefSurface();
	}
	
    } else if (d>0){
	if(endT+var>tps-1) var = tps-1-endT;
	endT += var;
    }
	timePreview->setv2((data[endT].time-selectedMatrix->getBeginTime())
						       /(selectedMatrix->getEndTime()-selectedMatrix->getBeginTime()));

}

void SDIFViewerWindow::changeElemRange(int d, int var){
	if(d<0){
		if(maxElt-var < 1) maxElt=1;
		else maxElt -= var;
		if(selectedElem > maxElt-1){
			selectedElem = maxElt - 1;
			//updateRefSurface();
		}   
	} else if (d>0){
		if(maxElt+var>element) maxElt = element;
		else maxElt += var;
	}
	eltPreview->setv2((maxElt / ((float) element)));
}

void SDIFViewerWindow::openSplineEditor(){
    
    se->showWindow();
   
}

void SDIFViewerWindow::openSliceWindow(){
    svw->showWindow();
}

void SDIFViewerWindow::openViewPointsWindow(){
    vpw->showWindow();
}

void SDIFViewerWindow::openAudioWindow(){
    //aw->showWindow();
}

void SDIFViewerWindow::openSonagram(){
    sona->showWindow();
}

void SDIFViewerWindow::openColorChooser(){
    cc->update();
    cc->showWindow();
}

/****************************************************************************************/

///////////////////////
// DISPLAY FUNCTIONS //
///////////////////////

float SDIFViewerWindow::maxTime(){
    float maxT;
    maxT = -FLT_MAX;
    float t;
    for(int i = beginT; i<=endT; i++){
	t = data[i].time;
	if(SDIF_DATA == PARTIAL_DATA) t += data[i].values[data[i].dimension-1][1];
	if(t>maxT) maxT = t;
    }
    return maxT;
}
float SDIFViewerWindow::minTime(){
    float minT;
    minT = FLT_MAX;
    float t;
    for(int i = beginT; i<=endT; i++){
	t = data[i].time;
	if(t<minT) minT = t;
    }
    return minT;
}

color SDIFViewerWindow::calculateColor(float value){
    color c;
    float v;
    if(cmax==cmin) v = 0.5;
    else v = (value-cmin)/(cmax-cmin);
    if(v<=lowTh){
	c.r=minColor.r ; c.g=minColor.g ; c.b=minColor.b;
    } else if(v>=highTh) {
	c.r=maxColor.r ; c.g=maxColor.g ; c.b=maxColor.b;
    } else {
	v = (v-lowTh)/(highTh-lowTh);
	c.r=(1-v)*minColor.r+v*maxColor.r ; c.g=(1-v)*minColor.g+v*maxColor.g ; c.b=(1-v)*minColor.b+v*maxColor.b;
    }    
    return c;
}






void SDIFViewerWindow::setMinColor(color c){
    minColor = c;
    glutPostRedisplay();
}

void SDIFViewerWindow::setMaxColor(color c){
    maxColor = c;
    glutPostRedisplay();
}

void SDIFViewerWindow::setBgColor(color c){
    bgColor = c;
    glutPostRedisplay();
}

color SDIFViewerWindow::getBgColor(){
    return bgColor;
}

color SDIFViewerWindow::getMinColor(){
    return minColor;
}

color SDIFViewerWindow::getMaxColor(){
    return maxColor;
}

void SDIFViewerWindow::sound(){
    //if(ap->synthesiserOn()){
	//if((SDIF_DATA==PARTIAL_DATA) && (selectedElem == 0)) ap->soundPartial();
	//else ap->sound();
    //}
}

void SDIFViewerWindow::setColorParam(int cp){
    if(cp>0 && cp<=field) colorParam = cp;
    cmax = getMaxValue(colorParam);
    cmin = getMinValue(colorParam);
    cc->update();
    cc->draw();
    glutSetWindow(ID);
}




