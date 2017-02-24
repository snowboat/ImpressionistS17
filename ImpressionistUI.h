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
#include <FL/Fl_Int_Input.H>
#include <vector>

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
	Fl_Button*			m_startAutoPaintButton;

	//For designing new convolution matrix
	Fl_Window*			m_convolutionDialog;
	Fl_Window*			m_askFilterSize;
	Fl_Int_Input*		m_filterSizex;
	Fl_Int_Input*		m_filterSizey;
	Fl_Button*			m_confirmFilterSize;
	Fl_Button*			m_normalizeConvolutionButton;
	Fl_Float_Input**	m_filterInputBoxes;		//1D array of pointers
	std::vector<Fl_Float_Input*>	m_vectorOfInputBoxes;
	std::vector<float>				m_vectorOfFilterValues;



	//for color manipulation
	Fl_Window*		m_colorManipulationDialog;
	Fl_Slider*		m_redSlider;
	Fl_Slider*		m_greenSlider;
	Fl_Slider*		m_blueSlider;
	Fl_Button*		m_applyManipulationButton;

	//Painterly
	Fl_Window*		m_painterlyDialog;
	Fl_Choice*		m_painterlyStyleChoice;
	Fl_Choice*		m_painterlyStrokeChoice;
	Fl_Button*		m_painterlyRunButton;
	Fl_Slider*		m_painterlyThresholdSlider;
	Fl_Slider*		m_painterlyCurvatureSlider;
	Fl_Slider*		m_painterlyBlurSlider;
	Fl_Slider*      m_painterlyGridsizeSlider;
	Fl_Slider*		m_painterlyMinStrokeSlider;
	Fl_Slider*		m_painterlyMaxStrokeSlider;
	Fl_Slider*		m_painterlyAlphaSlider;
	Fl_Slider*		m_painterlyLayersSlider;
	Fl_Slider*		m_painterlyR0levelSlider;


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
	bool				getSizeRandom();
	int					getFilterRows();
	int					getFilterCols();
	void				resetColorManipulation();
	int					getPainterlyThreshold();
	double				getPainterlyBlur();
	double				getPainterlyGridsize();
	int					getPainterlyLayers();
	int					getPainterlyR0level();


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
	bool    m_sizeRandom;

	//the convolution values
	int m_numFilterRows, m_numFilterCols;


	//painterly parameters
	int m_painterlyStyle;
	int m_painterlyStroke;
	int m_painterlyThreshold;
	double m_painterlyCurvature;
	double m_painterlyBlur;
	double m_painterlyGridsize;
	int m_painterlyMinStroke;
	int m_painterlyMaxStroke;
	double m_painterlyAlpha;
	int m_painterlyLayers;
	int m_painterlyR0level;

	// Static class members
	static Fl_Menu_Item		menuitems[];
	static Fl_Menu_Item		brushTypeMenu[NUM_BRUSH_TYPE + 1];
	static Fl_Menu_Item		strokeDirectionMenu[3 + 1];
	static Fl_Menu_Item		painterlyStyleMenu[5 + 1];
	static Fl_Menu_Item		painterlyStrokeMenu[3 + 1];

	static ImpressionistUI*	whoami(Fl_Menu_* o);

	// All callbacks here.  Callbacks are declared 
	// static
	static void	cb_load_image(Fl_Menu_* o, void* v);
	static void	cb_load_dissolve_image(Fl_Menu_* o, void* v);
	static void	cb_load_mural_image(Fl_Menu_* o, void* v);
	static void	cb_load_alpha_mapped_image(Fl_Menu_* o, void* v);
	static void cb_load_edge_image(Fl_Menu_* o, void* v);
	static void cb_load_another_image(Fl_Menu_ * o, void * v);
	static void	cb_save_image(Fl_Menu_* o, void* v);
	static void cb_display_original(Fl_Menu_* o, void* v);
	static void cb_display_edge(Fl_Menu_* o, void* v);
	static void cb_display_another(Fl_Menu_* o, void* v);
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
	static void cb_paintEdgeMap(Fl_Widget* o, void* v);
	static void cb_edgeThresholdChange(Fl_Widget* o, void* v);
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


	//callbacks for the customized convolution
	static void	cb_customize_convolution(Fl_Menu_* o, void* v);
	static void cb_confirmFilterSize(Fl_Widget* o, void* v);
	static void	cb_normalize_convolution(Fl_Widget* o, void* v);
	static void cb_filter_numRows_changes(Fl_Widget* o, void* v);
	static void cb_filter_numCols_changes(Fl_Widget* o, void* v);

	//callbacks for auto painting
	static void cb_paintSpacingChange(Fl_Widget* o, void* v);
	static void cb_sizeRandom(Fl_Widget* o, void* v);
	static void cb_startAutoPaint(Fl_Widget* o, void* v);

	//for painterly
	static void	cb_painterly(Fl_Menu_* o, void* v);
	static void cb_setPainterlyStyle(Fl_Widget* o, void* v);
	static void cb_setPainterlyStroke(Fl_Widget* o, void* v);
	static void cb_painterlyThresholdChanges(Fl_Widget* o, void* v);
	static void cb_painterlyCurvatureChanges(Fl_Widget* o, void* v);
	static void cb_painterlyBlurChanges(Fl_Widget* o, void* v);
	static void cb_painterlyGridsizeChanges(Fl_Widget* o, void* v);
	static void cb_painterlyMinStrokeChanges(Fl_Widget* o, void* v);
	static void cb_painterlyMaxStrokeChanges(Fl_Widget* o, void* v);
	static void cb_painterlyAlphaChanges(Fl_Widget* o, void* v);
	static void cb_painterlyLayersChanges(Fl_Widget* o, void* v);
	static void cb_painterlyR0levelChanges(Fl_Widget* o, void* v);
	static void cb_runPainterly(Fl_Widget* o, void* v);
	
};

#endif
