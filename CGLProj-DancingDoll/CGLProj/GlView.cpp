// GlView.cpp: implementation of the CGlView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GlView.h"

#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\GLUT.H>		// Header File For The GLut32 Library

#include <math.h>

#define PI 3.14159265

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//

CGlView::CGlView(CWnd *pclWnd)
{
	m_pclWnd = pclWnd;
	m_hDC = ::GetDC(pclWnd->m_hWnd);
	BOOL bSuccess;

	RECT rect;
	m_pclWnd->GetClientRect(&rect);
	int iWidth = rect.right - rect.left;
	int iHeight = rect.bottom - rect.top;

	bSuccess = bSetWindowPixelFormat(m_hDC);
	ASSERT(&bSuccess);

	bSuccess = bCreateViewGLContext(m_hDC);
	ASSERT(&bSuccess);

	vInitGl(iWidth, iHeight);

	generateTextureFromImageFile();
	glEnable(GL_LIGHTING);
}

// +
// *****************************************************************************
//   Module      :  CGlView::~CGlView()
//   Access      :  Public
//   Input       :  None
//   Output      :  None
//   Return Val  :  None
//   Exceptions  :  None
//   Desc        :  This function is the one and only destructor. It provides
//									clean up and performs any needed shutdown procedures
//
//   Notes       :  None
// *****************************************************************************
// -
CGlView::~CGlView()
{
	// make the rendering context not current
	if (wglGetCurrentContext() != NULL)
		wglMakeCurrent(NULL, NULL);

	// delete the Context
	if (m_hGLContext != NULL)
	{
		wglDeleteContext(m_hGLContext);
		m_hGLContext = NULL;
	}
	
	
}


// +
// *****************************************************************************
//   Module      :  void CGlView::DrawGLScene()	
//   Access      :  Public
//   Input       :  None
//   Output      :  None
//   Return Val  :  None
//   Exceptions  :  None
//   Desc        :  This function is called contiously to provide realtime OpenGl
//									drawing
//
//   Notes       :  Place all reatime rendering code here
// *****************************************************************************
// -
GLUquadricObj *obj;



void CGlView::vDraw()
{
	//
	obj = gluNewQuadric();


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();
	glTranslatef(-2.0f,-5.0f, -20.0f);
	glRotatef(-60,1,0, 0);
	glRotatef(-30, 0, 0, 1);
	

	// Translate
	glTranslatef(xShift, yShift, zShift);

	//// Rotate
	glRotatef(xAngle, 1, 0, 0);
	glRotatef(yAngle, 0, 1, 0);
	glRotatef(zAngle, 0, 0, 1);
	//axes
	glBegin(GL_LINES);
	//x  RED
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-3.0f, 0.0f, 0.0f);
	glVertex3f(3.0f, 0.0f, 0.0f);
	//y  GREEN 
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -3.0f, 0.0f);
	glVertex3f(0.0f, 3.0f, 0.0f);
	//z  BLUE
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -3.0f);
	glVertex3f(0.0f, 0.0f, 3.0f);
	glEnd();
	
	
	//////MIRROR //////////////////////////////////

	//Settings for drawing semi transparent stuff
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//making a stencil for the mirror
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF);
	glColorMask((byte)GL_FALSE, (byte)GL_FALSE, (byte)GL_FALSE, (byte)GL_FALSE);
	glDisable(GL_DEPTH_TEST); //draw no matter what
	//not really draw. just makeing a hole in the stencil
	DrawMirror();
	
	//restore regular settings
	glColorMask((byte)GL_TRUE, (byte)GL_TRUE, (byte)GL_TRUE, (byte)GL_TRUE);
	glEnable(GL_DEPTH_TEST);

	//setting stencil test (anything outside is clipped)
	glStencilFunc(GL_EQUAL, 1, 0xFFFFFFFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glEnable(GL_STENCIL_TEST);

	//draw reflected scene
	glPushMatrix();
	glTranslated(0.0f,5.0f,2.5f);   //moving into the mirror
	glRotated(-20, 1, 0, 0);    //rotating according to mirror angle
	glTranslated(0.0f,5.0f, -2.5f);   //moving into the mirror
	glScalef(1, -1, 1);

	//here
	DrawFloor();
	DrawTheMan();


	glPopMatrix();
	glDisable(GL_STENCIL_TEST); //no need for the stencil anymore

	DrawMirror();

	
	glEnable(GL_LIGHTING);
	
	DrawFloor();
	DrawCoverMirror();
	DrawTheMan();
	
	glDisable(GL_LIGHTING);
	glFlush();
	// swap the buffer
	SwapBuffers(m_hDC);

	gluDeleteQuadric(obj);
	
}

void CGlView::DrawTheMan()
{
	
	glPushMatrix();
	
	glRotatef(dancerAngle, 0, 0, 1);

	glPushMatrix();
	
		// left shoe
		glEnable(GL_TEXTURE_2D);
		gluQuadricTexture(obj, true);
		glBindTexture(GL_TEXTURE_2D, idTexture[offsetTexture + 9]);
		glTranslatef(-0.5f, 0.0f, 2.7f);
		glRotatef(90, 1, 0, 0);
		glRotatef(-90, 0, 1, 0);
		glutSolidTeapot(0.5);
		glDisable(GL_TEXTURE_2D);


		// left leg
		glColor3f(244 / 256.0f, 198 / 256.0f, 175 / 256.0f); // skin tone 
		glRotatef(-90, 1, 0, 0);
		gluCylinder(obj, 0.2, 0.4, 2, 20, 20);
	glPopMatrix();

	glColor3f(1.0f, 1.0f, 1.0f); // white color

	// right shoe
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		gluQuadricTexture(obj, true);
		glBindTexture(GL_TEXTURE_2D, idTexture[offsetTexture + 9]);
		glTranslatef(0.5f, 0.0f, 2.7f);
		glRotatef(90, 1, 0, 0);
		glRotatef(-90, 0, 1, 0);
		glutSolidTeapot(0.5);
		glDisable(GL_TEXTURE_2D);

		// right leg
		glColor3f(244 / 256.0f, 198 / 256.0f, 175 / 256.0f); // skin tone 
		glRotatef(-90, 1, 0, 0);
		gluCylinder(obj, 0.2, 0.4, 2, 20, 20);

	glPopMatrix();
	
	glColor3f(1.0f, 1.0f, 1.0f); // white color


	// skirt
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		gluQuadricTexture(obj, true);
		glBindTexture(GL_TEXTURE_2D, idTexture[offsetTexture+3]);
		glTranslatef(0.0f, 0.0f,4.0f);
		glRotatef(skirtAngle, 0, 0, 1);
		gluCylinder(obj, radiusBaseSkirt, 0.75, heightSkirt, 20, 20);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	// body
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture(obj, true);
	glBindTexture(GL_TEXTURE_2D, idTexture[offsetTexture + 6]);
	glTranslatef(0.0f, 0.0f,5.2f);
	gluCylinder(obj, 0.75, 0.75, 2.5, 20, 20);
	glDisable(GL_TEXTURE_2D);

	// upper blob
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture(obj, true);
	glBindTexture(GL_TEXTURE_2D, idTexture[offsetTexture + 12]);
	glTranslatef(0.0f, 0.0f, 2.5f);
//	glColor3f(244 / 256.0f, 198 / 256.0f, 175 / 256.0f);

	gluSphere(obj, 0.75, 20, 20);
	glDisable(GL_TEXTURE_2D);

		//Right
		glPushMatrix();
			glTranslated(0.75f, 0.0f, 0.0f);
			glColor3f(244/256.0f, 198/256.0f, 175/256.0f); // skin tone 
			//right shoulder
			gluSphere(obj, 0.3, 20, 20);
			glRotated(shoulderAngle, 1, 0, 0);
			//right arm
			gluCylinder(obj, 0.2, 0.19, 1.5, 20, 20);
			glTranslated(0.0f, 0.0f, 1.5f);
			//right elbow
			gluSphere(obj, 0.3, 20, 20);
			glRotated(-15, 1, 0, 0);
			//right forearm
			gluCylinder(obj, 0.2, 0.19, 1.5, 20, 20);
			glTranslated(0.0f, 0.0f, 1.5f);
			//right hand
			gluCylinder(obj, 0.2, 0, .5, 20, 20);
		glPopMatrix();

		//Left
		glPushMatrix();
			glTranslated(-0.75f, 0.0f, 0.0f);
			//glColor3f(0.5f, 0.30f, 0.21f);
			//left shoulder
			gluSphere(obj, 0.3, 20, 20);
			glRotated(shoulderAngle, 1, 0, 0);
			//left arm
			gluCylinder(obj, 0.2, 0.19, 1.5, 20, 20);
			glTranslated(0.0f, 0.0f, 1.5f);
			//left elbow
			gluSphere(obj, 0.3, 20, 20);
			glRotated(-15, 1, 0, 0);
			//left forearm
			gluCylinder(obj, 0.2, 0.19, 1.5, 20, 20);
			glTranslated(0.0f, 0.0f, 1.5f);
			//left hand
			gluCylinder(obj, 0.2, 0, .5, 20, 20);
		glPopMatrix();

		glColor3f(1.0f, 1.0f, 1.0f); // white color 
		// neck
		gluCylinder(obj, 0.2, 0.19, 0.5 + 0.75, 20, 20);

		// head
		glEnable(GL_TEXTURE_2D);
		gluQuadricTexture(obj, true);
		glBindTexture(GL_TEXTURE_2D, idTexture[offsetTexture + 0]);
		glTranslated(0.0f, 0.0f, 1.2f);
		gluSphere(obj, 0.5, 20, 20);
		glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}


void CGlView::DrawFloor() {

	/////////////////////////////////////////////////////////
	//// FLOOR //////////////////////////////////////////////
	/////////////////////////////////////////////////////////
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, idTexture[BOX_IMAGE]);
	glTranslatef(-5.0f, -5.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	//glRotatef(-90, 1, 0, 0);

	glBegin(GL_QUADS);
		//bottom floor
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(0, 0, 0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(0, 10,0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(10, 10, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(10, 0, 0);

		//top floor
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(0, 0, 2.5);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(0, 10,2.5);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(10, 10, 2.5);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(10, 0, 2.5);
	glEnd();

	//covers 4 sides of the floor
	glBegin(GL_QUADS);

		//cover back side floor
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(0, 10, 0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(0, 10, 2.5);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(10, 10,2.5);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(10, 10, 0);


		//cover front side floor
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(0, 0, 0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(0, 0, 2.5);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(10, 0, 2.5);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(10, 0, 0);



		//cover right side floor
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(10, 0, 0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(10, 0, 2.5);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(10, 10, 2.5);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(10, 10, 0);


		//cover left side floor
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(0, 0, 0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(0, 0, 2.5);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(0, 10, 2.5);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(0, 10, 0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void CGlView::DrawMirror() {
	glEnable(GL_LIGHTING);
	// front mirror with transparent colors
	glPushMatrix();
	glTranslatef(-5.0f, 5.0f,2.5f);
	glRotated(70, 1, 0, 0);
	glBegin(GL_QUADS);

	glColor4d(0, 0, 1, 0.5);
	glVertex3d(0.5, 0.5, 0.05f);
	glVertex3d(0.5, 9.5, 0.05f);
	glVertex3d(9.5, 9.5, 0.05f);
	glVertex3d(9.5, 0.5, 0.05f);
	glEnd();
	glPopMatrix();


}

void CGlView::DrawCoverMirror() {

	glPushMatrix();// save starting position of drawing
	
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, idTexture[BOX_IMAGE]);
	glTranslatef(-5.0f, -5.0f, 0.0f);
	glTranslatef(0, 10.0f, 2.5f);
	glRotated(70, 1, 0, 0);

	glBegin(GL_QUADS);

		// bottom
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(0, 0, -2);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(0, 10, -2);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(10, 10, -2);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(10, 0, -2);

		//top
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(0, 0, 0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(0, 10, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(10, 10, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(10, 0, 0);

		//front
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(0, 10, -2);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(0, 10, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(10, 10, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(10, 10, -2);

		//back
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(0, 0, -2);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(0, 0, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(10, 0, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(10, 0, -2);

		//left
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(0, 0, -2);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(0, 0, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(0, 10, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(0, 10, -2);

		//right
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(10, 0, -2);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(10, 0, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(10, 10, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(10, 10, -2);

	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

float CGlView::getZShift() {
	return this->zShift;
}

float CGlView::getYShift() {
	return this->yShift;
}
float CGlView::getXShift() {
	return this->xShift;
}

void CGlView::setZShift(float zShift) {
	this->zShift = zShift;
}
void CGlView::setYShift(float yShift) {
	this->yShift = yShift;
}
void CGlView::setXShift(float xShift) {
	this->xShift = xShift;
}

float CGlView::getZAngle() {
	return this->zAngle;
}
float CGlView::getYAngle() {
	return this->yAngle;
}
float CGlView::getXAngle() {
	return this->xAngle;
}

void CGlView::setZAngle(float zAngle) {
	this->zAngle = zAngle;
}
void CGlView::setYAngle(float yAngle) {
	this->yAngle = yAngle;
}
void CGlView::setXAngle(float xAngle) {
	this->xAngle = xAngle;
}

float CGlView::getShoulderAngle() {
	return this->shoulderAngle;
}

void CGlView::setShoulderAngle(float shoulderAngle) {
	this->shoulderAngle = shoulderAngle;
}

float CGlView::getRadiusBaseSkirt() {
	return this->radiusBaseSkirt;
}

float CGlView::getHeightSkirt() {
	return this->heightSkirt;
}

float CGlView::getSkirtAngle() {
	return this->skirtAngle;
}

void CGlView::setRadiusBaseSkirt(float r) {
	this->radiusBaseSkirt = r;
}

void CGlView::setHeightSkirt(float h) {
	this->heightSkirt = h;
}

void CGlView::setSkirtAngle(float a) {
	this->skirtAngle = a;
}

float CGlView::getDancerAngle() {
	return this->dancerAngle;
}

void CGlView::setDancerAngle(float a) {
	this->dancerAngle = a;
}

int CGlView::getOffsetTexture() {
	return this->offsetTexture;
}

void CGlView::setOffsetTexture(int num) {
	if (num > 2)
		this->offsetTexture = 0;
	else
		this->offsetTexture = num;
}


void CGlView::generateTextureFromImageFile()
{
	glGenTextures(SIZE, idTexture);//generates ID for each texture.

	const char* imageFileName[SIZE] = { 
		"res\\face1.bmp","res\\face2.bmp","res\\face3.bmp",
		"res\\skirt1.bmp","res\\skirt2.bmp","res\\skirt3.bmp",
		"res\\body1.bmp","res\\body2.bmp","res\\body3.bmp",
		"res\\shoe1.bmp","res\\shoe2.bmp","res\\shoe3.bmp",
		"res\\up1.bmp","res\\up2.bmp","res\\up3.bmp",

		"res\\send.bmp"};

	for (int i = 0; i < SIZE; i++)

	{
		texture = auxDIBImageLoad(imageFileName[i]);//loads the width, height, colors from the image file (*.bmp) into opengl structure (AUX_RGBImageRec).

		if (texture)
		{
			glBindTexture(GL_TEXTURE_2D, idTexture[i]);//working on idTexture[i]
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//if the target is smaller than the source then minimize it using LINEAR algorithm.
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//if the target is bigger than the source then maximize it using LINEAR algorithm.
			glTexImage2D(GL_TEXTURE_2D, 0, 3, texture->sizeX, texture->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->data);//save texture in memory.
			free(texture);
			texture = NULL;
		}
	}
}



// +
// *****************************************************************************
//   Module      :  BOOL CGlView::SetWindowPixelFormat(HDC hDC)
//   Access      :  Private
//   Input       :  hDC - Handle to the GL windwos device context
//   Output      :  None
//   Return Val  :  TRUE  - success
//									FALSE - failure
//   Exceptions  :  None
//   Desc        :  This function sets up the required pixelformat descriptor
//
//   Notes       :  None
// *****************************************************************************
// -
BOOL CGlView::bSetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;

	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;

	pixelDesc.dwFlags =

		PFD_DRAW_TO_WINDOW |// Format Must Support Window
		PFD_SUPPORT_OPENGL |// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,	// Must Support Double Buffering
		PFD_TYPE_RGBA,		// Request An RGBA Format
		32,					// Select A 16Bit Color Depth
		0, 0, 0, 0, 0, 0,	// Color Bits Ignored (?)
		0,					// No Alpha Buffer
		0,					// Shift Bit Ignored (?)
		0,					// No Accumulation Buffer
		0, 0, 0, 0,			// Accumulation Bits Ignored (?)
		16,					// 16Bit Z-Buffer (Depth Buffer)  
		0,					// No Stencil Buffer
		0,					// No Auxiliary Buffer (?)
		PFD_MAIN_PLANE,		// Main Drawing Layer
		0,					// Reserved (?)
		0, 0, 0;			// Layer Masks Ignored (?)

	int iGLPixelIndex = ChoosePixelFormat(hDC, &pixelDesc);
	ASSERT(&iGLPixelIndex);

	if (SetPixelFormat(hDC, iGLPixelIndex, &pixelDesc))
		return false;

	return TRUE;
}

// +
// *****************************************************************************
//   Module      :  BOOL CGlView::CreateViewGLContext(HDC hDC)
//   Access      :  Private
//   Input       :  hDC - Handle to the GL windwos device context
//   Output      :  None
//   Return Val  :  TRUE  - success
//									FALSE - failure
//   Exceptions  :  None
//   Desc        :  This function sets up the required GLContext
//
//   Notes       :  None
// *****************************************************************************
// -
BOOL CGlView::bCreateViewGLContext(HDC hDC)
{
	m_hGLContext = wglCreateContext(hDC);
	if (m_hGLContext == NULL)
		return FALSE;

	if (wglMakeCurrent(hDC, m_hGLContext) == FALSE)
		return FALSE;

	return TRUE;
}

// +
// *****************************************************************************
//   Module      :  void CGlView::InitGl(int iWidth, int iHeigth)
//   Access      :  Private
//   Input       :  iWidth  - width of the window
//									iHeight - height of the window
//   Output      :  None
//   Return Val  :  None
//   Exceptions  :  None
//   Desc        :  This function initializes all OpenGL rendering specifics
//
//   Notes       :  None
// *****************************************************************************
// -
void CGlView::vInitGl(int iWidth, int iHeigth)
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glViewport(0, 0, iWidth, iHeigth);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, (GLfloat)iWidth / (GLfloat)iHeigth, 0.45f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);
	float *pos = new float[4]{ -4,0,10,1 };
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
}


