//
// TraceUI.h
//
// Handles FLTK integration and other user interface tasks
//
#include <stdio.h>
#include <time.h>
#include <string.h>

#include <FL/fl_ask.h>

#include "TraceUI.h"
#include "../RayTracer.h"
#include "../fileio/bitmap.h"

static bool done;

//------------------------------------- Help Functions --------------------------------------------
TraceUI* TraceUI::whoami(Fl_Menu_* o)	// from menu item back to UI itself
{
	return ( (TraceUI*)(o->parent()->user_data()) );
}

//--------------------------------- Callback Functions --------------------------------------------
void TraceUI::cb_load_scene(Fl_Menu_* o, void* v) 
{
	TraceUI* pUI=whoami(o);
	
	char* newfile = fl_file_chooser("Open Scene?", "*.ray", NULL );

	if (newfile != NULL) {
		char buf[256];

		if (pUI->raytracer->loadScene(newfile)) {
			sprintf(buf, "Ray <%s>", newfile);
			done=true;	// terminate the previous rendering
		} else{
			sprintf(buf, "Ray <Not Loaded>");
		}

		pUI->m_mainWindow->label(buf);
	}
}


void TraceUI::cb_load_texture(Fl_Menu_* o, void* v) {
	TraceUI* pUI = whoami(o);
	char* newfile = fl_file_chooser("Open File?", "*.bmp", NULL);
	if (newfile != NULL) {
		unsigned char*	data;
		int				width, height;

		if ((data = readBMP(newfile, width, height)) == NULL)
		{
			fl_alert("Can't load bitmap file");
			return;
		}
		pUI->raytracer->scene->m_ucBitmap = data;
		pUI->raytracer->scene->m_nTextureHeight = height;
		pUI->raytracer->scene->m_nTextureWidth = width;
	}
}
void TraceUI::cb_load_background(Fl_Menu_* o, void* v)
{
	TraceUI* pUI = whoami(o);

	char* newfile = fl_file_chooser("Open Background?", "*.bmp", NULL);

	if (newfile != NULL) {
		char buf[256];

		if (pUI->raytracer->loadBackground(newfile)) {
			sprintf(buf, "Ray <%s>", newfile);
		}
		else {
			sprintf(buf, "Ray <Not loaded>");
		}

		pUI->m_mainWindow->label(buf);

	}
}

void TraceUI::cb_save_image(Fl_Menu_* o, void* v) 
{
	TraceUI* pUI=whoami(o);
	
	char* savefile = fl_file_chooser("Save Image?", "*.bmp", "save.bmp" );
	if (savefile != NULL) {
		pUI->m_traceGlWindow->saveImage(savefile);
	}
}

void TraceUI::cb_exit(Fl_Menu_* o, void* v)
{
	TraceUI* pUI=whoami(o);

	// terminate the rendering
	done=true;

	pUI->m_traceGlWindow->hide();
	pUI->m_mainWindow->hide();
}

void TraceUI::cb_exit2(Fl_Widget* o, void* v) 
{
	TraceUI* pUI=(TraceUI *)(o->user_data());
	
	// terminate the rendering
	done=true;

	pUI->m_traceGlWindow->hide();
	pUI->m_mainWindow->hide();
}

void TraceUI::cb_about(Fl_Menu_* o, void* v) 
{
	fl_message("RayTracer Project, FLTK version for CS 341 Spring 2002. Latest modifications by Jeff Maurer, jmaurer@cs.washington.edu");
}

void TraceUI::cb_sizeSlides(Fl_Widget* o, void* v)
{
	TraceUI* pUI=(TraceUI*)(o->user_data());
	
	pUI->m_nSize=int( ((Fl_Slider *)o)->value() ) ;
	int	height = (int)(pUI->m_nSize / pUI->raytracer->aspectRatio() + 0.5);
	pUI->m_traceGlWindow->resizeWindow( pUI->m_nSize, height );
}

void TraceUI::cb_depthSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nDepth=int( ((Fl_Slider *)o)->value() ) ;
}
void TraceUI::cb_thresholdSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nthreshold = double(((Fl_Slider *)o)->value());
}

void TraceUI::cb_constAttenSlides(Fl_Widget* o, void* v) {
	((TraceUI*)(o->user_data()))->m_nconstAtten = double(((Fl_Slider *)o)->value());
}

void TraceUI::cb_linearAttenSlides(Fl_Widget* o, void* v) {
	((TraceUI*)(o->user_data()))->m_nlinearAtten = double(((Fl_Slider *)o)->value());
}

void TraceUI::cb_quadraAttenSlides(Fl_Widget* o, void* v) {
	((TraceUI*)(o->user_data()))->m_nquadraAtten = double(((Fl_Slider *)o)->value());
}
void TraceUI::cb_samplingSlides(Fl_Widget* o, void* v) {
	((TraceUI*)(o->user_data()))->m_nSamplingSize = int(((Fl_Slider *)o)->value());
}
void TraceUI::cb_adaptive(Fl_Widget* o, void* v) {

	((TraceUI*)(o->user_data()))->m_nAdaptive = !((TraceUI*)(o->user_data()))->m_nAdaptive;
}
void TraceUI::cb_texture(Fl_Widget* o, void* v) {

	((TraceUI*)(o->user_data()))->m_nTexture = !((TraceUI*)(o->user_data()))->m_nTexture;
}
void TraceUI::cb_background(Fl_Widget* o, void* v) {

	((TraceUI*)(o->user_data()))->m_nBackground = !((TraceUI*)(o->user_data()))->m_nBackground;
}
void TraceUI::cb_render(Fl_Widget* o, void* v)
{
	char buffer[256];

	TraceUI* pUI=((TraceUI*)(o->user_data()));
	
	if (pUI->raytracer->sceneLoaded()) {
		int width=pUI->getSize();
		int	height = (int)(width / pUI->raytracer->aspectRatio() + 0.5);
		pUI->m_traceGlWindow->resizeWindow( width, height );

		pUI->m_traceGlWindow->show();

		pUI->raytracer->traceSetup(width, height);
		pUI->raytracer->traceThreshold(pUI->getThreshold());
		// Save the window label
		const char *old_label = pUI->m_traceGlWindow->label();

		// start to render here	
		done=false;
		clock_t prev, now;
		prev=clock();
		
		pUI->m_traceGlWindow->refresh();
		Fl::check();
		Fl::flush();

		for (int y=0; y<height; y++) {
			for (int x=0; x<width; x++) {
				if (done) break;
				
				// current time
				now = clock();

				// check event every 1/2 second
				if (((double)(now-prev)/CLOCKS_PER_SEC)>0.5) {
					prev=now;

					if (Fl::ready()) {
						// refresh
						pUI->m_traceGlWindow->refresh();
						// check event
						Fl::check();

						if (Fl::damage()) {
							Fl::flush();
						}
					}
				}

				pUI->raytracer->tracePixel( x, y );
		
			}
			if (done) break;

			// flush when finish a row
			if (Fl::ready()) {
				// refresh
				pUI->m_traceGlWindow->refresh();

				if (Fl::damage()) {
					Fl::flush();
				}
			}
			// update the window label
			sprintf(buffer, "(%d%%) %s", (int)((double)y / (double)height * 100.0), old_label);
			pUI->m_traceGlWindow->label(buffer);
			
		}
		done=true;
		pUI->m_traceGlWindow->refresh();

		// Restore the window label
		pUI->m_traceGlWindow->label(old_label);		
	}
}

void TraceUI::cb_stop(Fl_Widget* o, void* v)
{
	done=true;
}

void TraceUI::show()
{
	m_mainWindow->show();
}

void TraceUI::setRayTracer(RayTracer *tracer)
{
	raytracer = tracer;
	m_traceGlWindow->setRayTracer(tracer);
}

int TraceUI::getSize()
{
	return m_nSize;
}

int TraceUI::getDepth()
{
	return m_nDepth;
}
double TraceUI::getThreshold() {
	return m_nthreshold;
}

double TraceUI::getConstAttenuation() {
	return m_nconstAtten;
}

double TraceUI::getLinearAttenuation() {
	return m_nlinearAtten;
}

double TraceUI::getQuadraAttenuation() {
	return m_nquadraAtten;
}
//following three functions are only called once when loading 
void TraceUI::setConstAttenuation(double value) {
	m_nconstAtten = value;
	m_constAttenSlider->value(value);
}

void TraceUI::setLinearAttenuation(double value) {
	m_nlinearAtten = value;
	m_linearAttenSlider->value(value);
}

void TraceUI::setQuadraAttenuation(double value) {
	m_nquadraAtten = value;
	m_quadraAttenSlider->value(value);
}

// menu definition
Fl_Menu_Item TraceUI::menuitems[] = {
	{ "&File",		0, 0, 0, FL_SUBMENU },
		{ "&Load Scene...",	FL_ALT + 'l', (Fl_Callback *)TraceUI::cb_load_scene },
		{ "&Load Background...", FL_ALT+'b', (Fl_Callback *)TraceUI::cb_load_background },
		{ "&Save Image...",	FL_ALT + 's', (Fl_Callback *)TraceUI::cb_save_image },
		{ "&Load Texture",	FL_ALT + 't', (Fl_Callback *)TraceUI::cb_load_texture },
		{ "&Exit",			FL_ALT + 'e', (Fl_Callback *)TraceUI::cb_exit },
		{ 0 },

	{ "&Help",		0, 0, 0, FL_SUBMENU },
		{ "&About",	FL_ALT + 'a', (Fl_Callback *)TraceUI::cb_about },
		{ 0 },

	{ 0 }
};

TraceUI::TraceUI() {
	// init.
	m_nDepth = 0;
	m_nSize = 150;
	m_nthreshold = 0;
	m_nconstAtten = 0.5;
	m_nlinearAtten = 0.2;
	m_nquadraAtten = 0.0;
	m_nSamplingSize = 0;
	m_nAdaptive = FALSE;
	m_nTexture = FALSE;
	m_nBackground = FALSE;
	m_mainWindow = new Fl_Window(100, 40, 400, 400, "Ray <Not Loaded>");
		m_mainWindow->user_data((void*)(this));	// record self to be used by static callback functions
		// install menu bar
		m_menubar = new Fl_Menu_Bar(0, 0, 350, 25);
		m_menubar->menu(menuitems);

		// install slider depth
		m_depthSlider = new Fl_Value_Slider(10, 30, 180, 20, "Depth");
		m_depthSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_depthSlider->type(FL_HOR_NICE_SLIDER);
        m_depthSlider->labelfont(FL_COURIER);
        m_depthSlider->labelsize(12);
		m_depthSlider->minimum(0);
		m_depthSlider->maximum(10);
		m_depthSlider->step(1);
		m_depthSlider->value(m_nDepth);
		m_depthSlider->align(FL_ALIGN_RIGHT);
		m_depthSlider->callback(cb_depthSlides);

		// install slider size
		m_sizeSlider = new Fl_Value_Slider(10, 55, 180, 20, "Size");
		m_sizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_sizeSlider->type(FL_HOR_NICE_SLIDER);
        m_sizeSlider->labelfont(FL_COURIER);
        m_sizeSlider->labelsize(12);
		m_sizeSlider->minimum(64);
		m_sizeSlider->maximum(512);
		m_sizeSlider->step(1);
		m_sizeSlider->value(m_nSize);
		m_sizeSlider->align(FL_ALIGN_RIGHT);
		m_sizeSlider->callback(cb_sizeSlides);

		// install slider threshold
		m_thresholdSlider = new Fl_Value_Slider(10, 80, 200, 20, "Threshold");
		m_thresholdSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_thresholdSlider->type(FL_HOR_NICE_SLIDER);
		m_thresholdSlider->labelfont(FL_COURIER);
		m_thresholdSlider->labelsize(12);
		m_thresholdSlider->minimum(0.00);
		m_thresholdSlider->maximum(1.00);
		m_thresholdSlider->step(0.01);
		m_thresholdSlider->value(m_nthreshold);
		m_thresholdSlider->align(FL_ALIGN_RIGHT);
		m_thresholdSlider->callback(cb_thresholdSlides);

		//install const attenuation coeff slider
		m_constAttenSlider = new Fl_Value_Slider(10, 105, 200, 20, "Const Atten Coeff");
		m_constAttenSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_constAttenSlider->type(FL_HOR_NICE_SLIDER);
		m_constAttenSlider->labelfont(FL_COURIER);
		m_constAttenSlider->labelsize(12);
		m_constAttenSlider->minimum(0.00);
		m_constAttenSlider->maximum(1.00);
		m_constAttenSlider->step(0.01);
		m_constAttenSlider->value(m_nconstAtten);
		m_constAttenSlider->align(FL_ALIGN_RIGHT);
		m_constAttenSlider->callback(cb_constAttenSlides);
		//linear attenuation
		m_linearAttenSlider = new Fl_Value_Slider(10, 130, 200, 20, "Linear Atten Coeff");
		m_linearAttenSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_linearAttenSlider->type(FL_HOR_NICE_SLIDER);
		m_linearAttenSlider->labelfont(FL_COURIER);
		m_linearAttenSlider->labelsize(12);
		m_linearAttenSlider->minimum(0.00);
		m_linearAttenSlider->maximum(1.00);
		m_linearAttenSlider->step(0.01);
		m_linearAttenSlider->value(m_nlinearAtten);
		m_linearAttenSlider->align(FL_ALIGN_RIGHT);
		m_linearAttenSlider->callback(cb_linearAttenSlides);
		//quadratic attenuation
		m_quadraAttenSlider = new Fl_Value_Slider(10, 155, 200, 20, "Quadra Atten Coeff");
		m_quadraAttenSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_quadraAttenSlider->type(FL_HOR_NICE_SLIDER);
		m_quadraAttenSlider->labelfont(FL_COURIER);
		m_quadraAttenSlider->labelsize(12);
		m_quadraAttenSlider->minimum(0.00);
		m_quadraAttenSlider->maximum(1.00);
		m_quadraAttenSlider->step(0.01);
		m_quadraAttenSlider->value(m_nquadraAtten);
		m_quadraAttenSlider->align(FL_ALIGN_RIGHT);
		m_quadraAttenSlider->callback(cb_quadraAttenSlides);

		//quadratic attenuation
		m_samplingSlider = new Fl_Value_Slider(10, 180, 200, 20, "Sampling Size");
		m_samplingSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_samplingSlider->type(FL_HOR_NICE_SLIDER);
		m_samplingSlider->labelfont(FL_COURIER);
		m_samplingSlider->labelsize(12);
		m_samplingSlider->minimum(0);
		m_samplingSlider->maximum(5);
		m_samplingSlider->step(1);
		m_samplingSlider->value(m_nSamplingSize);
		m_samplingSlider->align(FL_ALIGN_RIGHT);
		m_samplingSlider->callback(cb_samplingSlides);

		m_adapativeSampling = new Fl_Light_Button(280, 180, 80, 20, "Adaptive");
		m_adapativeSampling->user_data((void*)(this));
		m_adapativeSampling->callback(cb_adaptive);
	
		m_textureMapping = new Fl_Light_Button(10, 205, 80, 20, "texture");
		m_textureMapping->user_data((void*)(this));
		m_textureMapping->callback(cb_texture);

		m_background = new Fl_Light_Button(100, 205, 80, 20, "background");
		m_background->user_data((void*)(this));
		m_background->callback(cb_background);

		m_renderButton = new Fl_Button(240, 27, 70, 25, "&Render");
		m_renderButton->user_data((void*)(this));
		m_renderButton->callback(cb_render);

		m_stopButton = new Fl_Button(240, 55, 70, 25, "&Stop");
		m_stopButton->user_data((void*)(this));
		m_stopButton->callback(cb_stop);

		m_mainWindow->callback(cb_exit2);
		m_mainWindow->when(FL_HIDE);
    m_mainWindow->end();

	// image view
	m_traceGlWindow = new TraceGLWindow(100, 150, m_nSize, m_nSize, "Rendered Image");
	m_traceGlWindow->end();
	m_traceGlWindow->resizable(m_traceGlWindow);
}