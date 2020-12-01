#ifndef BANGUN_GUI
#define BANGUN_GUI

#include <X11/Xlib.h>
#include <GL/glx.h>

struct fbConfigRangkai;
struct deretan_karcis;

struct berita_gui {
	Display *tampilan;
	Window pegangan;
	int nomerLayar;
	Screen *layar;
	XVisualInfo *pandangan;
	GLXContext glContext;
	Colormap warnaKump;
	struct deretan_karcis *alat_tersedia;
	GLXFBConfig frameConfig;
};

int bangkitkan_gui(struct berita_gui*);
void matikan_gui(struct berita_gui);
void lihat_identitas(Display*);

#endif