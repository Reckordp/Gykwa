#include "Gykwa.h"
#include "BangunGUI.h"

/*	Tradisional
	GLint perlengkapan_GL[] = {
		GLX_RGBA, GLX_DOUBLEBUFFER, 
		GLX_DEPTH_SIZE, 24, 
		GLX_STENCIL_SIZE, 8, 
		GLX_RED_SIZE, 8, 
		GLX_GREEN_SIZE, 8, 
		GLX_BLUE_SIZE, 8, 
		GLX_SAMPLE_BUFFERS, 0, 
		GLX_SAMPLES, 0, 
		None
	};
*/

/* Modern */
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, int*);
glXCreateContextAttribsARBProc glXCreateContextAttribsARB;

GLint perlengkapan_GL[] = {
	GLX_X_RENDERABLE, 1, 
	GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT, 
	GLX_RENDER_TYPE, GLX_RGBA_BIT, 
	GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR, 
	GLX_RED_SIZE, 8, 
	GLX_GREEN_SIZE, 8, 
	GLX_BLUE_SIZE, 8, 
	GLX_ALPHA_SIZE, 8, 
	GLX_DEPTH_SIZE, 24, 
	GLX_STENCIL_SIZE, 8, 
	GLX_DOUBLEBUFFER, 1, 
	None
};

XVisualInfo *mulaiX11(struct berita_gui *b) {
	int banyakFrame, bf = -1, numsamp = -1, samp_ok, ns;
	XVisualInfo *pandangan = NULL;
	GLXFBConfig *frameBC;

	b->tampilan = XOpenDisplay(NULL);
	b->layar = DefaultScreenOfDisplay(b->tampilan);
	b->nomerLayar = DefaultScreen(b->tampilan);
	/*
	return glXChooseVisual(b->tampilan, b->nomerLayar, perlengkapan_GL);
	*/

	frameBC = glXChooseFBConfig(b->tampilan, b->nomerLayar, perlengkapan_GL, &banyakFrame);
	if (!frameBC) {
		XCloseDisplay(b->tampilan);
		b->tampilan = NULL;
		return NULL;
	}

	for (int i = 0; i < banyakFrame; ++i) {
		pandangan = glXGetVisualFromFBConfig(b->tampilan, frameBC[i]);
		if (pandangan) {
			glXGetFBConfigAttrib(b->tampilan, frameBC[i], GLX_SAMPLE_BUFFERS, &samp_ok);
			glXGetFBConfigAttrib(b->tampilan, frameBC[i], GLX_SAMPLES, &ns);
			if (bf < 0 || (samp_ok && numsamp < ns)) {
				bf = i;
				numsamp = ns;
			}
		}
		XFree(pandangan);
	}

	b->frameConfig = frameBC[bf];
	XFree(frameBC);
	pandangan = glXGetVisualFromFBConfig(b->tampilan, b->frameConfig);
	if (!pandangan) {
		XCloseDisplay(b->tampilan);
		return NULL;
	}

	return ((pandangan->screen != b->nomerLayar)?(NULL):(pandangan));
}

GLXContext mulaiWindow(Window *w, XVisualInfo *v, Display *d, int id, Colormap cm, 
	Window rw, GLXFBConfig frameConfig, int mod) {
	XSetWindowAttributes wAtt = {
		.border_pixel = BlackPixel(d, id), 
		.background_pixel = WhitePixel(d, id),
		.override_redirect = 1, 
		.colormap = cm, 
		.event_mask = ExposureMask
	};
	int cAtt[] = {
		GLX_CONTEXT_MAJOR_VERSION_ARB, 4, 
		GLX_CONTEXT_MINOR_VERSION_ARB, 2, 
		GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, 
		None
	};

	GLXContext c = 0;
	glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB(
		(const GLubyte*)"glXCreateContextAttribsARB");
	if (!glXCreateContextAttribsARB) return NULL;

	*w = XCreateWindow(d, rw, 0, 0, 640, 480, 0, v->depth, InputOutput, 
		v->visual, CWBackPixel | CWColormap | CWBorderPixel | CWEventMask, &wAtt);

	if (mod) {
		c = glXCreateContextAttribsARB(d, frameConfig, 0, 1, cAtt);
	} else {
		c = glXCreateNewContext(d, frameConfig, GLX_RGBA_TYPE, 0, 1);
	}
	// c = glXCreateContext(d, v, NULL, 1);
	XSync(d, 0);
	// if (!glXIsDirect(d, c)) sampaikan("Indirect");
	// else sampaikan("Direct");
	glXMakeCurrent(d, *w, c);
	return c;
}

int bangkitkan_gui(struct berita_gui *brt) {
	if (!(brt->pandangan = mulaiX11(brt))) return 1;
	brt->alat_tersedia = nomorkan_alat_tersedia(glXQueryExtensionsString(
		brt->tampilan, brt->nomerLayar));
	Window rw = RootWindow(brt->tampilan, brt->nomerLayar);
	brt->warnaKump = XCreateColormap(brt->tampilan, rw, brt->pandangan->visual, AllocNone);
	brt->glContext = mulaiWindow(&brt->pegangan, brt->pandangan, brt->tampilan, 
		brt->nomerLayar, brt->warnaKump, rw, brt->frameConfig,
		aph_alat_didukung(brt->alat_tersedia, "GLX_ARB_create_context"));
	if (!brt->glContext) {
		XFree(brt->pandangan);
		XCloseDisplay(brt->tampilan);
		return 1;
	}
	return 0;
}

void lihat_identitas(Display *t) {
	GLint major, minor;
	char cadangan[256], *pt;
	int u = 0, jatah;

	write(PAPAN, "\nInformasi OPEN_GL -----------\n", 31);
	glXQueryVersion(t, &major, &minor);
	write(PAPAN, "\tGL_VERSION", 11);

	jatah = 19;
	while(jatah--) cadangan[u++] = 32;
	cadangan[u++] = ':';
	cadangan[u++] = 32;
	cadangan[u++] = (major / 10) % 10 + 48;
	cadangan[u++] = major % 10 + 48;
	cadangan[u++] = '.';
	cadangan[u++] = (minor / 10) % 10 + 48;
	cadangan[u++] = minor % 10 + 48;
	cadangan[u++] = 10;

#define IDENTITAS(NAMA) \
	cadangan[u++] = '\t'; \
	pt = #NAMA; \
	jatah = 30; \
	while(jatah-- && *pt) cadangan[u++] = *pt++; \
	while(jatah--) cadangan[u++] = 32; \
	cadangan[u++] = ':'; \
	cadangan[u++] = 32 ; \
	pt = (char*)glGetString(NAMA); \
	while(*pt) cadangan[u++] = *pt++; \
	cadangan[u++] = 10;

	IDENTITAS(GL_VENDOR);
	IDENTITAS(GL_RENDERER);
	IDENTITAS(GL_VERSION);
	IDENTITAS(GL_SHADING_LANGUAGE_VERSION);
#undef IDENTITAS

	cadangan[u++] = 10;
	write(PAPAN, cadangan, u);
}

void matikan_gui(struct berita_gui b) {
	matikan_penomoran(b.alat_tersedia);
	glXDestroyContext(b.tampilan, b.glContext);
	XFree(b.pandangan);
	XFreeColormap(b.tampilan, b.warnaKump);
	XDestroyWindow(b.tampilan, b.pegangan);
	XCloseDisplay(b.tampilan);
}