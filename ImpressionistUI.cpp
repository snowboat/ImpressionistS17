//
// impressionistUI.h
//
// The user interface part for the program.
//


#include <FL/fl_ask.h>

#include <math.h>

#include "impressionistUI.h"
#include "impressionistDoc.h"
#include <iostream>
#include <string>
using namespace std;

/*
//------------------------------ Widget Examples -------------------------------------------------
Here is some example code for all of the widgets that you may need to add to the
project.  You can copy and paste these into your code and then change them to
make them look how you want.  Descriptions for all of the widgets here can be found
in links on the fltk help session page.

//---------Window/Dialog and Menubar-----------------------------------

//----To install a window--------------------------
Fl_Window* myWindow = new Fl_Window(600, 300, "MyWindow");
myWindow->user_data((void*)(this));	// record self to be used by static callback functions

// install menu bar
myMenubar = new Fl_Menu_Bar(0, 0, 600, 25);
Fl_Menu_Item ImpressionistUI::myMenuItems[] = {
{ "&File",		0, 0, 0, FL_SUBMENU },
{ "&Load...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load },
{ "&Save...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save }.
{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
{ 0 },
{ "&Edit",		0, 0, 0, FL_SUBMENU },
{ "&Copy",FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_copy, (void *)COPY },
{ "&Cut",	FL_ALT + 'x', (Fl_Callback *)ImpressionistUI::cb_cut, (void *)CUT },
{ "&Paste",	FL_ALT + 'v', (Fl_Callback *)ImpressionistUI::cb_paste, (void *)PASTE },
{ 0 },
{ "&Help",		0, 0, 0, FL_SUBMENU },
{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
{ 0 },
{ 0 }
};
myMenubar->menu(myMenuItems);
m_mainWindow->end();

//----The window callback--------------------------
// One of the callbacks
void ImpressionistUI::cb_load(Fl_Menu_* o, void* v)
{
ImpressionistDoc *pDoc=whoami(o)->getDocument();

char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
if (newfile != NULL) {
pDoc->loadImage(newfile);
}
}


//------------Slider---------------------------------------

//----To install a slider--------------------------
Fl_Value_Slider * mySlider = new Fl_Value_Slider(10, 80, 300, 20, "My Value");
mySlider->user_data((void*)(this));	// record self to be used by static callback functions
mySlider->type(FL_HOR_NICE_SLIDER);
mySlider->labelfont(FL_COURIER);
mySlider->labelsize(12);
mySlider->minimum(1);
mySlider->maximum(40);
mySlider->step(1);
mySlider->value(m_nMyValue);
mySlider->align(FL_ALIGN_RIGHT);
mySlider->callback(cb_MyValueSlides);

//----The slider callback--------------------------
void ImpressionistUI::cb_MyValueSlides(Fl_Widget* o, void* v)
{
((ImpressionistUI*)(o->user_data()))->m_nMyValue=int( ((Fl_Slider *)o)->value() ) ;
}


//------------Choice---------------------------------------

//----To install a choice--------------------------
Fl_Choice * myChoice = new Fl_Choice(50,10,150,25,"&myChoiceLabel");
myChoice->user_data((void*)(this));	 // record self to be used by static callback functions
Fl_Menu_Item ImpressionistUI::myChoiceMenu[3+1] = {
{"one",FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)ONE},
{"two",FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)TWO},
{"three",FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)THREE},
{0}
};
myChoice->menu(myChoiceMenu);
myChoice->callback(cb_myChoice);

//-----The choice callback-------------------------
void ImpressionistUI::cb_myChoice(Fl_Widget* o, void* v)
{
ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
ImpressionistDoc* pDoc=pUI->getDocument();

int type=(int)v;

pDoc->setMyType(type);
}


//------------Button---------------------------------------

//---To install a button---------------------------
Fl_Button* myButton = new Fl_Button(330,220,50,20,"&myButtonLabel");
myButton->user_data((void*)(this));   // record self to be used by static callback functions
myButton->callback(cb_myButton);

//---The button callback---------------------------
void ImpressionistUI::cb_myButton(Fl_Widget* o, void* v)
{
ImpressionistUI* pUI=((ImpressionistUI*)(o->user_data()));
ImpressionistDoc* pDoc = pUI->getDocument();
pDoc->startPainting();
}


//---------Light Button------------------------------------

//---To install a light button---------------------
Fl_Light_Button* myLightButton = new Fl_Light_Button(240,10,150,25,"&myLightButtonLabel");
myLightButton->user_data((void*)(this));   // record self to be used by static callback functions
myLightButton->callback(cb_myLightButton);

//---The light button callback---------------------
void ImpressionistUI::cb_myLightButton(Fl_Widget* o, void* v)
{
ImpressionistUI *pUI=((ImpressionistUI*)(o->user_data()));

if (pUI->myBool==TRUE) pUI->myBool=FALSE;
else pUI->myBool=TRUE;
}

//----------Int Input--------------------------------------

//---To install an int input-----------------------
Fl_Int_Input* myInput = new Fl_Int_Input(200, 50, 5, 5, "&My Input");
myInput->user_data((void*)(this));   // record self to be used by static callback functions
myInput->callback(cb_myInput);

//---The int input callback------------------------
void ImpressionistUI::cb_myInput(Fl_Widget* o, void* v)
{
((ImpressionistUI*)(o->user_data()))->m_nMyInputValue=int( ((Fl_Int_Input *)o)->value() );
}

//------------------------------------------------------------------------------------------------
*/

//------------------------------------- Help Functions --------------------------------------------

//------------------------------------------------------------
// This returns the UI, given the menu item.  It provides a
// link from the menu items to the UI
//------------------------------------------------------------
ImpressionistUI* ImpressionistUI::whoami(Fl_Menu_* o)
{
	return ((ImpressionistUI*)(o->parent()->user_data()));
}


//--------------------------------- Callback Functions --------------------------------------------

//------------------------------------------------------------------
// Brings up a file chooser and then loads the chosen image
// This is called by the UI when the load image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_load_image(Fl_Menu_* o, void* v)
{
	ImpressionistDoc *pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	if (newfile != NULL) {
		pDoc->loadImage(newfile);
	}
}

void ImpressionistUI::cb_load_dissolve_image(Fl_Menu_ * o, void * v)
{
	ImpressionistDoc *pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	if (newfile != NULL) {
		pDoc->loadDissolveImage(newfile);
	}
}

void ImpressionistUI::cb_load_mural_image(Fl_Menu_ * o, void * v)
{
	ImpressionistDoc *pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	if (newfile != NULL) {
		pDoc->loadMuralImage(newfile);
	}
}

void ImpressionistUI::cb_load_alpha_mapped_image(Fl_Menu_ * o, void * v)
{
	ImpressionistDoc *pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	if (newfile != NULL) {
		pDoc->loadAlphaMappedImage(newfile);
	}
}

void ImpressionistUI::cb_load_another_image(Fl_Menu_ * o, void * v)
{
	ImpressionistDoc *pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	if (newfile != NULL) {
		pDoc->loadAnotherImage(newfile);
	}
}



//------------------------------------------------------------------
// Brings up a file chooser and then saves the painted image
// This is called by the UI when the save image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_save_image(Fl_Menu_* o, void* v)
{
	ImpressionistDoc *pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Save File?", "*.bmp", "save.bmp");
	if (newfile != NULL) {
		pDoc->saveImage(newfile);
	}
}

//-------------------------------------------------------------
// Brings up the paint dialog
// This is called by the UI when the brushes menu item
// is chosen
//-------------------------------------------------------------
void ImpressionistUI::cb_brushes(Fl_Menu_* o, void* v)
{
	whoami(o)->m_brushDialog->show();
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	pDoc->clearCanvas();
}

//------------------------------------------------------------
// Causes the Impressionist program to exit
// Called by the UI when the quit menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_exit(Fl_Menu_* o, void* v)
{
	whoami(o)->m_mainWindow->hide();
	whoami(o)->m_brushDialog->hide();
	whoami(o)->m_colorManipulationDialog->hide();

}



//-----------------------------------------------------------
// Brings up an about dialog box
// Called by the UI when the about menu item is chosen
//-----------------------------------------------------------
void ImpressionistUI::cb_about(Fl_Menu_* o, void* v)
{
	fl_message("Impressionist FLTK version for COMP4411, Spring 2017 - HE, ZHOU");
}

//------- UI should keep track of the current for all the controls for answering the query from Doc ---------
//-------------------------------------------------------------
// Sets the type of brush to use to the one chosen in the brush 
// choice.  
// Called by the UI when a brush is chosen in the brush choice
//-------------------------------------------------------------
void ImpressionistUI::cb_brushChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();

	int type = (int)v;


	pDoc->setBrushType(type);
	if (type == BRUSH_ALPHA_MAPPED && pDoc->m_alphaMappedValues == NULL) {
		char* newfile = fl_file_chooser("Please load an alpha image", "*.bmp", pDoc->getImageName());
		if (newfile != NULL) {
			pDoc->loadAlphaMappedImage(newfile);
		}
	}
	// cout << "brushchoice is " << type << endl;
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas button is pushed
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas_button(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	pDoc->clearCanvas();
}

// callback function of stroke direction change
void ImpressionistUI::cb_setStrokeDirection(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI = ((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();

	int type = (int)v;

	pDoc->setStrokeDirection(type);
}


//-----------------------------------------------------------
// Updates the brush size to use from the value of the size
// slider
// Called by the UI when the size slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_sizeSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nSize = int(((Fl_Slider *)o)->value());
}

// change line width when slider changes
void ImpressionistUI::cb_lineWidthChanges(Fl_Widget* o, void* v) {
	((ImpressionistUI*)(o->user_data()))->m_lineWidth = int(((Fl_Slider *)o)->value());
}

// change line angle when slider slides
void ImpressionistUI::cb_lineAngleChanges(Fl_Widget* o, void* v) {
	((ImpressionistUI*)(o->user_data()))->m_lineAngle = int(((Fl_Slider *)o)->value());
}

// change the ALPHA value when the slider changes.
void ImpressionistUI::cb_alphaSlides(Fl_Widget * o, void * v)
{
	((ImpressionistUI*)(o->user_data()))->m_alphaValue = float(((Fl_Slider *)o)->value());
}

// change the paint spacing when the slider changes.
void ImpressionistUI::cb_paintSpacingChange(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_paintSpacing = float(((Fl_Slider *)o)->value());
}

// change the edge threshold when the slider changes.
void ImpressionistUI::cb_edgeThresholdChange(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_edgeThreshold = float(((Fl_Slider *)o)->value());
}

// paint the edge map on original view
void ImpressionistUI::cb_paintEdgeMap(Fl_Widget* o, void* v)
{
	cout << "paint edge" << endl;
}

//swap the painting&original image
void ImpressionistUI::cb_swap_image(Fl_Menu_ * o, void * v)
{
	ImpressionistDoc* m_pDoc = whoami(o)->getDocument();

	//swap the bitmaps
	if (whoami(o)->getDocument()->m_ucBitmap && whoami(o)->getDocument()->m_ucPainting) {
		unsigned char* temp = whoami(o)->getDocument()->m_ucBitmap;

		//update backup bitmap to the new ucBitmap
		whoami(o)->getDocument()->m_ucBitmap = whoami(o)->getDocument()->m_ucPainting;
		memcpy(whoami(o)->getDocument()->backupBitmap, whoami(o)->getDocument()->m_ucBitmap, whoami(o)->getDocument()->m_nPaintHeight*whoami(o)->getDocument()->m_nPaintWidth * 3);
		whoami(o)->getDocument()->m_ucPainting = temp;

		//update the undo bitmap to be null(nothing to undo)
		delete[] m_pDoc->m_undoImage;
		m_pDoc->m_undoImage = NULL;

		//redraw the two view
		whoami(o)->m_origView->refresh();
		whoami(o)->m_paintView->refresh();

	}
}

void ImpressionistUI::cb_manipulate_color(Fl_Menu_ * o, void * v)
{
	whoami(o)->m_colorManipulationDialog->show();
}

void ImpressionistUI::cb_redSlides(Fl_Widget * o, void * v)
{
	((ImpressionistUI*)(o->user_data()))->m_redValue = float(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_greenSlides(Fl_Widget * o, void * v)
{
	((ImpressionistUI*)(o->user_data()))->m_greenValue = float(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_blueSlides(Fl_Widget * o, void * v)
{
	((ImpressionistUI*)(o->user_data()))->m_blueValue = float(((Fl_Slider *)o)->value());

}

void ImpressionistUI::cb_manipulate_color_button(Fl_Widget * o, void * v)
{
	((ImpressionistUI*)(o->user_data()))->getDocument()->applyManipulation();
}

void ImpressionistUI::cb_undo(Fl_Menu_ * o, void * v)
{
	whoami(o)->getDocument()->undo();
}

void ImpressionistUI::cb_edgeClipping(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_edgeClipping = bool(((Fl_Light_Button *)o)->value());
}

void ImpressionistUI::cb_anotherGradient(Fl_Widget* o, void* v)
{
	ImpressionistUI *pUI = ((ImpressionistUI*)(o->user_data()));

	if (pUI->m_anotherGradient == true)
		pUI->m_anotherGradient = false;
	else {
		if (pUI->getDocument()->m_ucAnotherBitmap) {
			pUI->m_anotherGradient = true;
		}
		else {
			fl_alert("Please load another image!");
			pUI->m_AnotherGradientButton->value(false);
		}
	}
}

void ImpressionistUI::cb_customize_convolution(Fl_Menu_ * o, void * v)
{
	whoami(o)->m_askFilterSize->show();
}

void ImpressionistUI::cb_confirmFilterSize(Fl_Widget * o, void * v)
{
	ImpressionistUI *pUI = ((ImpressionistUI*)(o->user_data()));

	//create the convolution dialog
	pUI->m_convolutionDialog = new Fl_Window(400, 325, "Make your OWN convolution");
	pUI->m_normalizeConvolutionButton = new Fl_Button(10, 250, 100, 30, "Normalize");
	pUI->m_normalizeConvolutionButton->callback(cb_normalize_convolution);
	
	pUI->m_filterParameters = new double*[pUI->m_numFilterRows];
	for (int row = 0; row < pUI->m_numFilterRows; row++) {
		pUI->m_filterParameters[row] = new double[pUI->m_numFilterCols];
	}

	pUI->m_convolutionDialog->end();
	pUI->m_convolutionDialog->show();
}



void ImpressionistUI::cb_normalize_convolution(Fl_Widget * o, void * v)
{
	//double sum = conv00 + conv01 + conv02 + conv10 + conv11 + conv12 + conv20 + conv21 + conv22;
	int sum = 0;
	if (sum == 0)
		fl_alert("Invalid convolution. sum is 0");
	else {

	}
	fl_message("Impressionist FLTK version for CS341, Spring 2002");
	
}
void ImpressionistUI::cb_filter_numRows_changes(Fl_Widget * o, void * v)
{
	
	std::string tempstr(((Fl_Float_Input *)o)->value());
	((ImpressionistUI*)(o->user_data()))->m_numFilterRows = std::stof(tempstr);

}
void ImpressionistUI::cb_filter_numCols_changes(Fl_Widget * o, void * v)
{
	std::string tempstr(((Fl_Float_Input *)o)->value());
	((ImpressionistUI*)(o->user_data()))->m_numFilterCols = std::stof(tempstr);

}
//---------------------------------- per instance functions --------------------------------------

//------------------------------------------------
// Return the ImpressionistDoc used
//------------------------------------------------
ImpressionistDoc* ImpressionistUI::getDocument()
{
	return m_pDoc;
}

//------------------------------------------------
// Draw the main window
//------------------------------------------------
void ImpressionistUI::show() {
	m_mainWindow->show();
	m_paintView->show();
	m_origView->show();
}

//------------------------------------------------
// Change the paint and original window sizes to 
// w by h
//------------------------------------------------
void ImpressionistUI::resize_windows(int w, int h) {
	m_paintView->size(w, h);
	m_origView->size(w, h);
	std::cout << "resized to " << w << "and " << h << std::endl;
}

//------------------------------------------------ 
// Set the ImpressionistDoc used by the UI to 
// communicate with the brushes 
//------------------------------------------------
void ImpressionistUI::setDocument(ImpressionistDoc* doc)
{
	m_pDoc = doc;

	m_origView->m_pDoc = doc;
	m_paintView->m_pDoc = doc;
}

//------------------------------------------------
// Return the brush size
//------------------------------------------------
int ImpressionistUI::getSize()
{
	return m_nSize;
}

//-------------------------------------------------
// Set the brush size
//-------------------------------------------------
void ImpressionistUI::setSize(int size)
{
	m_nSize = size;

	if (size <= 40)
		m_BrushSizeSlider->value(m_nSize);
	else
		m_BrushSizeSlider->value(40);
}

//------------------------------------------------
// Return the line width
//------------------------------------------------
int ImpressionistUI::getLineWidth()
{
	return m_lineWidth;
}

//-------------------------------------------------
// Set the line width
//-------------------------------------------------
void ImpressionistUI::setLineWidth(int lineWidth) 
{
	m_lineWidth = lineWidth;

	if (lineWidth <= 40 && lineWidth > 0)
		m_LineWidthSlider->value(m_lineWidth);
}

//------------------------------------------------
// Return the line angle
//------------------------------------------------
int ImpressionistUI::getLineAngle()
{
	return m_lineAngle;
}

//-------------------------------------------------
// Set the line angle
//-------------------------------------------------
void ImpressionistUI::setLineAngle(int lineAngle)
{
	m_lineAngle = lineAngle;

	if (lineAngle <= 359 && lineAngle >= 0)
		m_LineAngleSlider->value(m_lineAngle);
}

//------------------------------------------------
// Return the alpha
//------------------------------------------------
double ImpressionistUI::getAlpha()
{
	return m_alphaValue;
}

//-------------------------------------------------
// Set the alpha
//-------------------------------------------------
void ImpressionistUI::setAlpha(double alpha)
{
	m_alphaValue = alpha;

	if (alpha <= 1.0 && alpha>= 0.0)
		m_AlphaValueSlider->value(m_alphaValue);
}

int ImpressionistUI::getPaintSpacing()
{
	return m_paintSpacing;
}

int ImpressionistUI::getEdgeThreshold()
{
	return m_edgeThreshold;
}

float ImpressionistUI::getRed()
{
	return m_redValue;
}

float ImpressionistUI::getGreen()
{
	return m_greenValue;
}

float ImpressionistUI::getBlue()
{
	return m_blueValue;
}

// get the edge clipping
bool ImpressionistUI::getEdgeClipping()
{
	return m_edgeClipping;
}


// get the another gradient
bool ImpressionistUI::getAnotherGradient() {
	return m_anotherGradient;
}


// Main menu definition
Fl_Menu_Item ImpressionistUI::menuitems[] = {
	{ "&File",		0, 0, 0, FL_SUBMENU },
	{ "&Load Image...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load_image },
	{ "&Save Image...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save_image, 0, FL_MENU_DIVIDER },
	
	{ "&Load Mural Image...",	FL_ALT + 'm', (Fl_Callback *)ImpressionistUI::cb_load_mural_image },
	{ "&Load Alpha Mapped Image...",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_load_alpha_mapped_image },
	{ "&Load Edge Image...",	FL_ALT + 'e', (Fl_Callback *)ImpressionistUI::cb_load_image },
	{ "&Load Another Image...",	FL_ALT + 'g', (Fl_Callback *)ImpressionistUI::cb_load_another_image, 0, FL_MENU_DIVIDER },

	{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
	{ 0 },

	{ "&Edit",		0, 0, 0, FL_SUBMENU },
	{ "&Undo...",	FL_ALT + 'u', (Fl_Callback *)ImpressionistUI::cb_undo },
	{ "&Clear Canvas", FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_clear_canvas, 0, FL_MENU_DIVIDER },

	{ "&Brushes...",	FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_brushes },
	{ "&Manipulate Color...",	FL_ALT + 'm', (Fl_Callback *)ImpressionistUI::cb_manipulate_color },
	{ "&Dissolve Image...",	FL_ALT + 'd', (Fl_Callback *)ImpressionistUI::cb_load_dissolve_image },
	{ "&Customize Convolution...",	FL_ALT + 'v', (Fl_Callback *)ImpressionistUI::cb_customize_convolution },
	{ "&Swap Image...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_swap_image, 0, FL_MENU_DIVIDER },

	{ "&Colors...",	FL_ALT + 'k', (Fl_Callback *)ImpressionistUI::cb_brushes },
	{ "&Paintly", FL_ALT + 'p', (Fl_Callback *)ImpressionistUI::cb_clear_canvas, 0, FL_MENU_DIVIDER },
	{ 0 },

	{ "&Display",		0, 0, 0, FL_SUBMENU },
	{ "&Original Image...",	FL_ALT + 'o', (Fl_Callback *)ImpressionistUI::cb_load_image },
	{ "&Mural Image...",	FL_ALT + 'm', (Fl_Callback *)ImpressionistUI::cb_save_image },
	{ "&Alpha Mapped Image...",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_save_image },
	{ "&Edge Image...",	FL_ALT + 'e', (Fl_Callback *)ImpressionistUI::cb_save_image },
	{ "&Another Image...",	FL_ALT + 'g', (Fl_Callback *)ImpressionistUI::cb_save_image },
	{ 0 },

	{ "&Options",		0, 0, 0, FL_SUBMENU },
	{ "&Faster...",	FL_ALT + 'f', (Fl_Callback *)ImpressionistUI::cb_load_image },
	{ "&Safer...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save_image },
	{ 0 },

	{ "&Help",		0, 0, 0, FL_SUBMENU },
	{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
	{ 0 },

	{ 0 }
};

// Brush choice menu definition
Fl_Menu_Item ImpressionistUI::brushTypeMenu[NUM_BRUSH_TYPE + 1] = {
	{ "Points",			FL_ALT + 'p', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_POINTS },
	{ "Lines",				FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_LINES },
	{ "Circles",			FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_CIRCLES },
	{ "Scattered Points",	FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_POINTS },
	{ "Scattered Lines",	FL_ALT + 'm', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_LINES },
	{ "Scattered Circles",	FL_ALT + 'd', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_CIRCLES },
	{ "Saturation",	FL_ALT + 't', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SATURATION_BRUSH },
	{ "Black and White",	FL_ALT + 'w', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_BLACKANDWHITE_BRUSH },
	{ "Blur",	FL_ALT + 'w', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_BLURRING },
	{ "Sharpen",	FL_ALT + 'w', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SHARPENING },
	{ "Alpha Mapped",	FL_ALT + 'w', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_ALPHA_MAPPED },
	{ 0 }
};

Fl_Menu_Item ImpressionistUI::strokeDirectionMenu[3 + 1] = {
	{ "Slider/Right Mouse",FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_setStrokeDirection, (void *)1 },
	{ "Gradient",FL_ALT + 'g', (Fl_Callback *)ImpressionistUI::cb_setStrokeDirection, (void *)2 },
	{ "Brush Direction",FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_setStrokeDirection, (void *)3 },
	{ 0 }
};



//----------------------------------------------------
// Constructor.  Creates all of the widgets.
// Add new widgets here
//----------------------------------------------------
ImpressionistUI::ImpressionistUI() {
	// Create the main window
	m_mainWindow = new Fl_Window(600, 300, "Impressionist");
	m_mainWindow->user_data((void*)(this));	// record self to be used by static callback functions
											// install menu bar
	m_menubar = new Fl_Menu_Bar(0, 0, 600, 25);
	m_menubar->menu(menuitems);

	// Create a group that will hold two sub windows inside the main
	// window
	Fl_Group* group = new Fl_Group(0, 25, 600, 275);

	// install paint view window
	m_paintView = new PaintView(300, 25, 300, 275, "This is the paint view");//0jon
	m_paintView->box(FL_DOWN_FRAME);

	// install original view window
	m_origView = new OriginalView(0, 25, 300, 275, "This is the orig view");//300jon
	m_origView->box(FL_DOWN_FRAME);
	m_origView->deactivate();//may delete this line for displaying the red cursor on origView

	group->end();
	Fl_Group::current()->resizable(group);
	m_mainWindow->end();

	// init values
	m_nSize = 10;
	m_lineWidth = 1;
	m_lineAngle = 0;
	m_alphaValue = 1.00;
	m_paintSpacing = 4;
	m_edgeThreshold = 200;
	m_redValue = 1.00;
	m_greenValue = 1.00;
	m_blueValue = 1.00;
	m_edgeClipping = true;
	m_anotherGradient = false;
	m_numFilterRows = 0;
	m_numFilterCols = 0;

	// brush dialog definition
	m_brushDialog = new Fl_Window(400, 325, "Brush Dialog");

	// Add a brush type choice to the dialog
	m_BrushTypeChoice = new Fl_Choice(50, 10, 150, 25, "&Brush");
	m_BrushTypeChoice->user_data((void*)(this));	// record self to be used by static callback functions
	m_BrushTypeChoice->menu(brushTypeMenu);
	m_BrushTypeChoice->callback(cb_brushChoice);

	//clear Canvas
	m_ClearCanvasButton = new Fl_Button(240, 10, 150, 25, "&Clear Canvas");
	m_ClearCanvasButton->user_data((void*)(this));
	m_ClearCanvasButton->callback(cb_clear_canvas_button);


	//Choice for Stroke Direction
	m_strokeDirectionChoice = new Fl_Choice(114, 45, 150, 25, "&Stroke Direction");
	m_strokeDirectionChoice->user_data((void*)(this));	 // record self to be used by static callback functions
	m_strokeDirectionChoice->menu(strokeDirectionMenu);
	m_strokeDirectionChoice->callback(cb_setStrokeDirection);
	m_strokeDirectionChoice->deactivate();



	// Add brush size slider to the dialog 
	m_BrushSizeSlider = new Fl_Value_Slider(10, 80, 300, 20, "Size");
	m_BrushSizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
	m_BrushSizeSlider->type(FL_HOR_NICE_SLIDER);
	m_BrushSizeSlider->labelfont(FL_COURIER);
	m_BrushSizeSlider->labelsize(12);
	m_BrushSizeSlider->minimum(1);
	m_BrushSizeSlider->maximum(40);
	m_BrushSizeSlider->step(1);
	m_BrushSizeSlider->value(m_nSize);
	m_BrushSizeSlider->align(FL_ALIGN_RIGHT);
	m_BrushSizeSlider->callback(cb_sizeSlides);

	// Line width slider
	m_LineWidthSlider = new Fl_Value_Slider(10, 110, 300, 20, "Line Width");
	m_LineWidthSlider->user_data((void*)(this));	// record self to be used by static callback functions
	m_LineWidthSlider->type(FL_HOR_NICE_SLIDER);
	m_LineWidthSlider->labelfont(FL_COURIER);
	m_LineWidthSlider->labelsize(12);
	m_LineWidthSlider->minimum(1);
	m_LineWidthSlider->maximum(40);
	m_LineWidthSlider->step(1);
	m_LineWidthSlider->value(m_lineWidth);
	m_LineWidthSlider->align(FL_ALIGN_RIGHT);
	m_LineWidthSlider->callback(cb_lineWidthChanges);
	m_LineWidthSlider->deactivate();

	// Line Angle Slider
	m_LineAngleSlider = new Fl_Value_Slider(10, 140, 300, 20, "Line Angle");
	m_LineAngleSlider->user_data((void*)(this));	// record self to be used by static callback functions
	m_LineAngleSlider->type(FL_HOR_NICE_SLIDER);
	m_LineAngleSlider->labelfont(FL_COURIER);
	m_LineAngleSlider->labelsize(12);
	m_LineAngleSlider->minimum(0);
	m_LineAngleSlider->maximum(359);
	m_LineAngleSlider->step(1);
	m_LineAngleSlider->value(m_lineAngle);
	m_LineAngleSlider->align(FL_ALIGN_RIGHT);
	m_LineAngleSlider->callback(cb_lineAngleChanges);
	m_LineAngleSlider->deactivate();

	// Alpha Slider
	m_AlphaValueSlider = new Fl_Value_Slider(10, 170, 300, 20, "Alpha");
	m_AlphaValueSlider->user_data((void*)(this));	// record self to be used by static callback functions
	m_AlphaValueSlider->type(FL_HOR_NICE_SLIDER);
	m_AlphaValueSlider->labelfont(FL_COURIER);
	m_AlphaValueSlider->labelsize(12);
	m_AlphaValueSlider->minimum(0);
	m_AlphaValueSlider->maximum(1);
	m_AlphaValueSlider->step(0.01);
	m_AlphaValueSlider->value(m_alphaValue);
	m_AlphaValueSlider->align(FL_ALIGN_RIGHT);
	m_AlphaValueSlider->callback(cb_alphaSlides);

	// edge clipping button
	m_EdgeClippingButton = new Fl_Light_Button(10, 200, 150, 25, "Edge Clipping");
	m_EdgeClippingButton->user_data((void*)(this));   // record self to be used by static callback functions
	m_EdgeClippingButton->value(m_edgeClipping);
	m_EdgeClippingButton->callback(cb_edgeClipping); 
	m_EdgeClippingButton->deactivate();

	// another gradient button
	m_AnotherGradientButton = new Fl_Light_Button(240, 200, 150, 25, "Another Gradient");
	m_AnotherGradientButton->user_data((void*)(this));   // record self to be used by static callback functions
	m_AnotherGradientButton->value(m_anotherGradient);
	m_AnotherGradientButton->callback(cb_anotherGradient);
	m_AnotherGradientButton->deactivate();

	// Add paint space slider
	m_PaintSpacingSlider = new Fl_Value_Slider(10, 230, 150, 20, "Spacing");
	m_PaintSpacingSlider->user_data((void*)(this));	// record self to be used by static callback functions
	m_PaintSpacingSlider->type(FL_HOR_NICE_SLIDER);
	m_PaintSpacingSlider->labelfont(FL_COURIER);
	m_PaintSpacingSlider->labelsize(12);
	m_PaintSpacingSlider->minimum(1);
	m_PaintSpacingSlider->maximum(16);
	m_PaintSpacingSlider->step(1);
	m_PaintSpacingSlider->value(m_paintSpacing);
	m_PaintSpacingSlider->align(FL_ALIGN_RIGHT);
	m_PaintSpacingSlider->callback(cb_paintSpacingChange);

	// Add edge threshold slider
	m_EdgeThresholdSlider = new Fl_Value_Slider(10, 260, 200, 20, "Edge Threshold");
	m_EdgeThresholdSlider->user_data((void*)(this));	// record self to be used by static callback functions
	m_EdgeThresholdSlider->type(FL_HOR_NICE_SLIDER);
	m_EdgeThresholdSlider->labelfont(FL_COURIER);
	m_EdgeThresholdSlider->labelsize(12);
	m_EdgeThresholdSlider->minimum(0);
	m_EdgeThresholdSlider->maximum(500);
	m_EdgeThresholdSlider->step(1);
	m_EdgeThresholdSlider->value(m_edgeThreshold);
	m_EdgeThresholdSlider->align(FL_ALIGN_RIGHT);
	m_EdgeThresholdSlider->callback(cb_edgeThresholdChange);

	// add button for painting edge map
	m_EdgeButton = new Fl_Button(300, 260, 50, 25, "&Do it");
	m_EdgeButton->user_data((void*)(this));
	m_EdgeButton->callback(cb_paintEdgeMap);

	m_brushDialog->end();

	m_colorManipulationDialog = new Fl_Window(400, 325, "Color Manipulation Dialog");
	
	// Red Channel Slider
	m_redSlider = new Fl_Value_Slider(10, 110, 300, 20, "Red");
	m_redSlider->user_data((void*)(this));	// record self to be used by static callback functions
	m_redSlider->type(FL_HOR_NICE_SLIDER);
	m_redSlider->labelfont(FL_COURIER);
	m_redSlider->labelsize(12);
	m_redSlider->minimum(0);
	m_redSlider->maximum(1);
	m_redSlider->step(0.01);
	m_redSlider->value(m_redValue);
	m_redSlider->align(FL_ALIGN_RIGHT);
	m_redSlider->callback(cb_redSlides);

	// Green Channel Slider
	m_greenSlider = new Fl_Value_Slider(10, 140, 300, 20, "Green");
	m_greenSlider->user_data((void*)(this));	// record self to be used by static callback functions
	m_greenSlider->type(FL_HOR_NICE_SLIDER);
	m_greenSlider->labelfont(FL_COURIER);
	m_greenSlider->labelsize(12);
	m_greenSlider->minimum(0);
	m_greenSlider->maximum(1);
	m_greenSlider->step(0.01);
	m_greenSlider->value(m_greenValue);
	m_greenSlider->align(FL_ALIGN_RIGHT);
	m_greenSlider->callback(cb_greenSlides);

	//Blue slider
	m_blueSlider = new Fl_Value_Slider(10, 170, 300, 20, "Blue");
	m_blueSlider->user_data((void*)(this));	// record self to be used by static callback functions
	m_blueSlider->type(FL_HOR_NICE_SLIDER);
	m_blueSlider->labelfont(FL_COURIER);
	m_blueSlider->labelsize(12);
	m_blueSlider->minimum(0);
	m_blueSlider->maximum(1);
	m_blueSlider->step(0.01);
	m_blueSlider->value(m_blueValue);
	m_blueSlider->align(FL_ALIGN_RIGHT);
	m_blueSlider->callback(cb_blueSlides);

	m_applyManipulationButton = new Fl_Button(10, 200, 150, 25, "&Apply manipulation");
	m_applyManipulationButton->user_data((void*)(this));
	m_applyManipulationButton->callback(cb_manipulate_color_button);

	m_colorManipulationDialog->end();


	//construct the customized convolution dialog one-by-one

	




	//the window which asks for filter size
	m_askFilterSize = new Fl_Window(300, 100, "Input the size");

	m_filterSizex = new Fl_Float_Input(40, 10, 60, 30, "Rows");
	m_filterSizex->user_data((void*)(this));
	m_filterSizex->callback(cb_filter_numRows_changes);

	m_filterSizey = new Fl_Float_Input(150, 10, 60, 30, "Cols");
	m_filterSizey->user_data((void*)(this));
	m_filterSizey->callback(cb_filter_numCols_changes);

	m_confirmFilterSize = new Fl_Button(40, 50, 60, 30, "Confirm");
	m_confirmFilterSize->user_data((void*)(this));
	m_confirmFilterSize->callback(cb_confirmFilterSize);

	m_askFilterSize->end();

}
