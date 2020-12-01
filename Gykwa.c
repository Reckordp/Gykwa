#include <X11/Xlib.h>
#include <GL/glx.h>
#include <fcntl.h>
#include "Gykwa.h"
#include "BangunGUI.h"

extern inline void kosongkan_area(void*, size_t);
struct berita_gykwa {
	struct berita_gui bGui;
};

typedef void (APIENTRYP PFNGLDRAWARRAYSPROC) (GLenum mode, GLint first, GLsizei count);
typedef void (APIENTRYP PFNGLBEGINPROC) (GLenum);
typedef void (APIENTRYP PFNGLENDPROC) (void);
typedef void (APIENTRYP PFNGLVERTEX3F) (GLdouble, GLdouble, GLdouble);
typedef void (APIENTRYP PFNGLCOLOR3F) (GLdouble, GLdouble, GLdouble);


#define GLGUNAKANFUNGSI(ret,proc,name,param) \
	proc name;
	PFNGLBEGINPROC glBeginFunc;
	PFNGLENDPROC glEndFunc;
	PFNGLVERTEX3F glVertex3fFunc;
	PFNGLCOLOR3F glColor3fFunc;
#include "DaftarGLFungsi.h"
#undef GLGUNAKANFUNGSI
bGykwa gykwa;

void angkutOGLFungsi() {
#define GLGUNAKANFUNGSI(ret,proc,name,param) \
	name = (proc)glXGetProcAddressARB(#name);
#include "DaftarGLFungsi.h"
#undef GLGUNAKANFUNGSI
	glBeginFunc = (PFNGLBEGINPROC)(glXGetProcAddressARB("glBegin"));
	glEndFunc = (PFNGLENDPROC)(glXGetProcAddressARB("glEnd"));
	glVertex3fFunc = (PFNGLVERTEX3F)(glXGetProcAddressARB("glVertex3f"));
	glColor3fFunc = (PFNGLCOLOR3F)(glXGetProcAddressARB("glColor3f"));
}

GLfloat vertices[] = {
	 0.0f, -1.0f, 
	-1.0f,  1.0f,
	 1.0f,  1.0f,
};

int main() {
	char vTahan[256], fTahan[256], tahan[256];
	const GLcharARB *vt, *ft;
	int fd;
	off_t ukuran;


	kosongkan_area(&gykwa, sizeof(bGykwa));
	bangkitkan_gui(&gykwa.bGui);
	lihat_identitas(gykwa.bGui.tampilan);
	angkutOGLFungsi();
	glViewport(0, 0, 640, 480);

	GLhandleARB program, vShader, fShader;
	program = glCreateProgramObjectARB();
	vShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);

	fd = open("vertexSederhana.shader", R_OK);
	ukuran = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	read(fd, vTahan, ukuran & 0xff);
	close(fd);
	vTahan[ukuran] = 0;

	vt = vTahan;
	glShaderSourceARB(vShader, 1, &vt, NULL);
	glCompileShaderARB(vShader);
	glGetObjectParameterivARB(vShader, GL_OBJECT_COMPILE_STATUS_ARB, &fd);

	if (!fd) {
		glGetInfoLogARB(vShader, 256, &fd, tahan);
		sampaikan_angka(fd);
		sampaikan(tahan);
	}

	fShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

	fd = open("fragmentSederhana.shader", R_OK);
	ukuran = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	read(fd, fTahan, ukuran & 0xff);
	close(fd);
	fTahan[ukuran] = 0;

	ft = fTahan;
	glShaderSourceARB(fShader, 1, &ft, NULL);
	glCompileShaderARB(fShader);
	glGetObjectParameterivARB(fShader, GL_OBJECT_COMPILE_STATUS_ARB, &fd);

	if (!fd) {
		glGetInfoLogARB(fShader, 256, &fd, tahan);
		sampaikan_angka(fd);
		sampaikan(tahan);
	}

	glAttachObjectARB(program, vShader);
	glAttachObjectARB(program, fShader);
	glLinkProgramARB(program);
	glValidateProgramARB(program);

	glGetObjectParameterivARB(program, GL_OBJECT_LINK_STATUS_ARB, &fd);
	if (!fd) {
		glGetInfoLogARB(program, 256, &fd, tahan);
		sampaikan_angka(fd);
		sampaikan(tahan);
	}

	GLuint posId;
	posId = glGetAttribLocationARB(program, "posisi");

	GLuint vert;
	glGenBuffersARB(1, &vert);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vert);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vertices), vertices, GL_STATIC_DRAW_ARB);

	XSelectInput(gykwa.bGui.tampilan, gykwa.bGui.pegangan, StructureNotifyMask);
	Atom delWindow = XInternAtom(gykwa.bGui.tampilan, "WM_DELETE_WINDOW", 0);
	XSetWMProtocols(gykwa.bGui.tampilan, gykwa.bGui.pegangan, &delWindow, 1);

	XClearWindow(gykwa.bGui.tampilan, gykwa.bGui.pegangan);
	XMapRaised(gykwa.bGui.tampilan, gykwa.bGui.pegangan);

	glScissor(0, 0, 10, 10);
	glUseProgramObjectARB(program);
	while(1) { 
		XEvent e;
		XNextEvent(gykwa.bGui.tampilan, &e);
		if (e.type == MapNotify) sampaikan("Muncul");
		else if (e.type == Expose) sampaikan("Hai");
		else if (e.type == ClientMessage) break;

		glClearColor(0.5f, 0.6f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vert);
		glVertexAttribPointerARB(posId, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, 0);
		glEnableVertexAttribArrayARB(posId);
		
		glEnable(GL_SCISSOR_TEST);
		glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_SCISSOR_TEST);

		// glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);

		glColor3fFunc(1.0f, 0.0f, 0.0f);
		glBeginFunc(GL_TRIANGLES);
		glVertex3fFunc(-1.0f, -1.0f, 0.0f);
		glVertex3fFunc(0.0f, 1.0f, 0.0f);
		glVertex3fFunc(1.0f, -1.0f, 0.0f);
		glEndFunc();

		glDisableVertexAttribArrayARB(posId);
		glXSwapBuffers(gykwa.bGui.tampilan, gykwa.bGui.pegangan);
	}

	glDeleteBuffersARB(1, &vert);
	glDeleteObjectARB(vShader);
	glDeleteObjectARB(fShader);
	glDeleteObjectARB(program);
	matikan_gui(gykwa.bGui);
	return 0;
}