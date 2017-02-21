//
// impressionistUI.h
//
// The header file for the UI part
//

#ifndef ImpressionistUI_h
#define ImpressionistUI_h

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_file_chooser.H>		// FLTK file chooser
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Float_Input.H>

#include "Impressionist.h"
#include "OriginalView.h"
#include "PaintView.h"

#include "ImpBrush.h"

class ImpressionistUI {
public:
	ImpressionistUI();

	// The FLTK widgets
	Fl_Window*			m_mainWindow;
	Fl_Menu_Bar*		m_menubar;

	PaintView*			m_paintView;
	OriginalView*		m_origView;

	// for brush dialog
	Fl_Window*			m_brushDialog;
	Fl_Choice*			m_BrushTypeChoice;
	Fl_Choice*			m_strokeDirectionChoice;
	Fl_Slider*			m_BrushSizeSlider;
	Fl_Slider*			m_LineWidthSlider;
	Fl_Slider*			m_LineAngleSlider;
	Fl_Slider*			m_AlphaValueSlider;
	Fl_Slider*          m_PaintSpacingSlider;
	Fl_Slider*          m_EdgeThresholdSlider;
	Fl_Button*          m_ClearCanvasButton;
	Fl_Button*          m_PaintButton;
	Fl_Button*          m_EdgeButton;
	Fl_Light_Button*    m_EdgeClippingButton;
	Fl_Light_Button*    m_AnotherGradientButton;
	Fl_Light_Button*    m_RandomSizeButton;

	//For designing new convolution matrix
	Fl_Window*			m_convolutionDialog;
	Fl_Float_Input*	    m_convolution00;
	Fl_Float_Input*	    m_convolution01;
	Fl_Float_Input*	    m_convolution02;
	Fl_Float_Input*	    m_convolution10;
	Fl_Float_Input*	    m_convolution11;
	Fl_Float_Input*	    m_convolution12;
	Fl_Float_Input*	    m_convolution20;
	Fl_Float_Input*	    m_convolution21;
	Fl_Float_Input*	    m_convolution22;
	Fl_Button*		m_normalizeConvolutionButton;



	//for color manipulation
	Fl_Window*		m_colorManipulationDialog;
	Fl_Slider*		m_redSlider;
	Fl_Slider*		m_greenSlider;
	Fl_Slider*		m_blueSlider;
	Fl_Button*		m_applyManipulationButton;


	// Member functions
	void				setDocument(ImpressionistDoc* doc);
	ImpressionistDoc*	getDocument();

	void				show();
	void				resize_windows(int w, int h);

	// Interface to get attribute
	int					getSize();
	void				setSize(int size);
	int                 getLineWidth();
	void                setLineWidth(int lineWidth);
	int                 getLineAngle();
	void                setLineAngle(int lineAngle);
	double				getAlpha();
	void				setAlpha(double alpha);
	int                 getPaintSpacing();
	int					getEdgeThreshold();
	float				getRed();
	float				getGreen();
	float				getBlue();
	bool				getEdgeClipping();
	bool				getAnotherGradient();


private:
	ImpressionistDoc*	m_pDoc;		// pointer to document to communicate with the document

	// All attributes here
	int		m_nSize;
	int 	m_lineWidth;
	int 	m_lineAngle;
	double  m_alphaValue;

	int		m_paintSpacing;
	int     m_edgeThreshold;

	double	m_redValue;
	double	m_greenValue;
	double	m_blueValue;

	bool	m_edgeClipping;
	bool	m_anotherGradient;

	//the nine convolution values
	double conv00, conv01, conv02, conv10, conv11, conv12, conv20, conv21, conv22;

	// Static class members
	static Fl_Menu_Item		menuitems[];
	static Fl_Menu_Item		brushTypeMenu[NUM_BRUSH_TYPE + 1];
	static Fl_Menu_Item		strokeDirectionMenu[3 + 1];

	static ImpressionistUI*	whoami(Fl_Menu_* o);

	// All callbacks here.  Callbacks are declared 
	// static
	static void	cb_load_image(Fl_Menu_* o, void* v);
	static void	cb_load_dissolve_image(Fl_Menu_* o, void* v);
	static void	cb_load_mural_image(Fl_Menu_* o, void* v);
	static void	cb_load_alpha_mapped_image(Fl_Menu_* o, void* v);
	static void cb_load_another_image(Fl_Menu_ * o, void * v);
	static void	cb_save_image(Fl_Menu_* o, void* v);
	static void	cb_brushes(Fl_Menu_* o, void* v);
	static void	cb_clear_canvas(Fl_Menu_* o, void* v);
	static void	cb_exit(Fl_Menu_* o, void* v);
	static void	cb_about(Fl_Menu_* o, void* v);
	static void	cb_brushChoice(Fl_Widget* o, void* v);
	static void	cb_clear_canvas_button(Fl_Widget* o, void* v);
	static void	cb_setStrokeDirection(Fl_Widget* o, void* v);
	static void	cb_sizeSlides(Fl_Widget* o, void* v);
	static void cb_lineAngleChanges(Fl_Widget* o, void* v);
	static void cb_lineWidthChanges(Fl_Widget* o, void* v);
	static void cb_alphaSlides(Fl_Widget* o, void* v);
	static void cb_paintSpacingChange(Fl_Widget* o, void* v);
	static void cb_edgeThresholdChange(Fl_Widget* o, void* v);
	static void cb_paintEdgeMap(Fl_Widget* o, void* v);
	static void cb_swap_image(Fl_Menu_* o, void* v);
	//color manipulation callbacks
	static void cb_manipulate_color(Fl_Menu_* o, void* v);
	static void cb_redSlides(Fl_Widget* o, void* v);
	static void cb_greenSlides(Fl_Widget* o, void* v);
	static void cb_blueSlides(Fl_Widget* o, void* v);
	static void	cb_manipulate_color_button(Fl_Widget* o, void* v);
	static void cb_edgeClipping(Fl_Widget* o, void* v);
	static void cb_anotherGradient(Fl_Widget* o, void* v);

	//undo option
	static void	cb_undo(Fl_Menu_* o, void* v);

	static void	cb_customize_convolution(Fl_Menu_* o, void* v);

	//callbacks for the customized convolution
	static void cb_conv00changes(Fl_Widget* o, void* v);
	static void cb_conv01changes(Fl_Widget* o, void* v);
	static void cb_conv02changes(Fl_Widget* o, void* v);
	static void cb_conv10changes(Fl_Widget* o, void* v);
	static void cb_conv11changes(Fl_Widget* o, void* v);
	static void cb_conv12changes(Fl_Widget* o, void* v);
	static void cb_conv20changes(Fl_Widget* o, void* v);
	static void cb_conv21changes(Fl_Widget* o, void* v);
	static void cb_conv22changes(Fl_Widget* o, void* v);
	static void	cb_normalize_convolution(Fl_Widget* o, void* v);



};

#endif
