//
// paintview.h
//
// The header file for painting view of the input images
//

#ifndef PAINTVIEW_H
#define PAINTVIEW_H

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>

class ImpressionistDoc;

class PaintView : public Fl_Gl_Window
{
public:
	PaintView(int x, int y, int w, int h, const char* l);
	void draw();
	int handle(int event);

	void refresh();
	
	void resizeWindow(int width, int height);

	void SaveCurrentContent();

	void RestoreContent();

	int getDrawHeight();
	int getWindowHeight();

	ImpressionistDoc *m_pDoc;

	void autoPaint();
	void startAutoPaint();

	//painterly interface
	void doPainterly();
	void paintLayer(unsigned char* canvas, unsigned char* refImage, int radius);

	int redbytePosition(int x, int y);
	int greenbytePosition(int x, int y);
	int bluebytePosition(int x, int y);

private:
	GLvoid* m_pPaintBitstart;
	int		m_nDrawWidth,
			m_nDrawHeight,
			m_nStartRow, 
			m_nEndRow,
			m_nStartCol, 
			m_nEndCol,
			m_nWindowWidth, 
			m_nWindowHeight;

	std::vector<Point> strokes; //to record the random stroke positions of painterly
	int painterlyRadius;
	unsigned char* painterlyRefimage;
	unsigned char* painterlyCanvas;

	//The associated original view for drawing the cursor
	OriginalView* origView;

};

#endif