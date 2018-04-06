//
// rayUI.h
//
// The header file for the UI part
//

#ifndef __rayUI_h__
#define __rayUI_h__

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Button.H>

#include <FL/fl_file_chooser.H>		// FLTK file chooser

#include "TraceGLWindow.h"

class TraceUI {
public:
	TraceUI();

	// The FLTK widgets
	Fl_Window*			m_mainWindow;
	Fl_Menu_Bar*		m_menubar;

	Fl_Slider*			m_sizeSlider;
	Fl_Slider*			m_depthSlider;
	Fl_Slider*			m_thresholdSlider;
	Fl_Button*			m_renderButton;
	Fl_Button*			m_stopButton;
	Fl_Slider*			m_constAttenSlider;
	Fl_Slider*			m_linearAttenSlider;
	Fl_Slider*			m_quadraAttenSlider;
	Fl_Slider*			m_samplingSlider;
	Fl_Light_Button*	m_BVHButton;
	TraceGLWindow*		m_traceGlWindow;

	// member functions
	void show();

	void		setRayTracer(RayTracer *tracer);

	int			getSize();
	int			getDepth();
	double		getThreshold();
	double		getConstAttenuation();
	double		getLinearAttenuation();
	double		getQuadraAttenuation();
	void		setConstAttenuation(double value);
	void		setLinearAttenuation(double value);
	void		setQuadraAttenuation(double value);
	bool		getAllowBVH();

	int			m_nSamplingSize;
private:
	RayTracer*	raytracer;

	int			m_nSize;
	int			m_nDepth;
	double		m_nthreshold;
	double		m_nconstAtten;
	double		m_nlinearAtten;
	double		m_nquadraAtten;
	bool		m_nallowBVH;
	

// static class members
	static Fl_Menu_Item menuitems[];

	static TraceUI* whoami(Fl_Menu_* o);

	static void cb_load_scene(Fl_Menu_* o, void* v);
	static void cb_load_background(Fl_Menu_* o, void* v);
	static void cb_save_image(Fl_Menu_* o, void* v);
	static void cb_exit(Fl_Menu_* o, void* v);
	static void cb_about(Fl_Menu_* o, void* v);

	static void cb_exit2(Fl_Widget* o, void* v);

	static void cb_sizeSlides(Fl_Widget* o, void* v);
	static void cb_depthSlides(Fl_Widget* o, void* v);
	static void cb_thresholdSlides(Fl_Widget* o, void* v);
	static void cb_constAttenSlides(Fl_Widget* o, void* v);
	static void cb_linearAttenSlides(Fl_Widget* o, void* v);
	static void cb_quadraAttenSlides(Fl_Widget* o, void* v);
	static void cb_samplingSlides(Fl_Widget* o, void* v);
	static void cb_BVHButton(Fl_Widget* o, void* v);



	static void cb_render(Fl_Widget* o, void* v);
	static void cb_stop(Fl_Widget* o, void* v);


};

#endif
