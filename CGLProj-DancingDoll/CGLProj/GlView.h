// GlView.h: interface for the CGlView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLVIEW_H__6EDA23AE_B764_11D5_8D52_0004AC3DB799__INCLUDED_)
#define AFX_GLVIEW_H__6EDA23AE_B764_11D5_8D52_0004AC3DB799__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <gl\GLAUX.H> //// Header File For The glaux Library



#define SIZE 16
#define BOX_IMAGE 15



class CGlView  
{
public:
	CGlView(CWnd *pclWnd);
	~CGlView();
	
	
	protected:
	private:
		void vInitGl(int iWidth, int iHeigth);
		BOOL bSetWindowPixelFormat(HDC hDC);
		BOOL bCreateViewGLContext(HDC hDC);

	// members //////////////////////////////////////////////////////////
	public:
	protected:
	private:			
		HGLRC	m_hGLContext;	// required GLContext
		CWnd *m_pclWnd;			// window inwhich opengl will be drawn
		HDC		m_hDC;			// handle to m_pclWnds device context
		unsigned int idTexture[SIZE];
		AUX_RGBImageRec* texture;

		float zShift = 0.0f;
		float yShift = 0.0f;
		float xShift = 0.0f;
		float zAngle = 0.0f;
		float yAngle = 0.0f;
		float xAngle = 0.0f;
		float shoulderAngle = 20.0f;
		float radiusBaseSkirt = 2.0f;
		float heightSkirt = 1.7f;
		float skirtAngle = 0.0f;
		float dancerAngle = 0.0f;
		int offsetTexture = 0;
		

public:

	float getZShift();
	float getYShift();
	float getXShift();

	void setZShift(float z);
	void setYShift(float y);
	void setXShift(float x);

	float getZAngle();
	float getYAngle();
	float getXAngle();

	void setZAngle(float z);
	void setYAngle(float y);
	void setXAngle(float x);

	float getShoulderAngle();
	void setShoulderAngle(float sholderAngle);
	
	float getRadiusBaseSkirt();
	float getHeightSkirt();
	float getSkirtAngle();

	void setRadiusBaseSkirt(float r);
	void setHeightSkirt(float h);
	void setSkirtAngle(float a);

	float getDancerAngle();
	void setDancerAngle(float a);

	int getOffsetTexture();
	void setOffsetTexture(int num);

	void vDraw();
	void DrawTheMan();
	void DrawFloor();
	void DrawMirror();
	void DrawCoverMirror();
	void generateTextureFromImageFile();

	

	//int intOptionB = 1;
//int intOptionC = 1;
//int textureOffset = 0;
//int shoulderAngle = 20;
//int mirrorAngle = 70;
//int sin_index = 0;
//double AccumulatedRotationsTraslations[16];

	//void vDrawHoleInAPit();
//void vDrawChest();
//void vDrawChestLidTop();
//void vDrawChestLid();
//void vDrawTheWomanInRed();
//void vDrawMirror();
//void Draw();
	//void end_MapTexture();
};

#endif

