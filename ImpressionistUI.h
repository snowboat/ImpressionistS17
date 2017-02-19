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
	Fl_Button*          m_ClearCanvasButton;

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
	float				getRed();
	float				getGreen();
	float				getBlue();




private:
	ImpressionistDoc*	m_pDoc;		// pointer to document to communicate with the document

	// All attributes here
	int		m_nSize;
	int 	m_lineWidth;
	int 	m_lineAngle;
	double   m_alphaValue;
	double	m_redValue;
	double	m_greenValue;
	double	m_blueValue;

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
	static void cb_swap_image(Fl_Menu_* o, void* v);
	//color manipulation callbacks
	static void cb_manipulate_color(Fl_Menu_* o, void* v);
	static void cb_redSlides(Fl_Widget* o, void* v);
	static void cb_greenSlides(Fl_Widget* o, void* v);
	static void cb_blueSlides(Fl_Widget* o, void* v);
	static void	cb_manipulate_color_button(Fl_Widget* o, void* v);

	//undo option
	static void	cb_undo(Fl_Menu_* o, void* v);



};

#endif
