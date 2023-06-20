/******************************************************************************
*  Copyright (C), 2017-2019,Advanced Technology Institute of Suzhou. 
*  THIS IS AN UNPUBLISHED WORK CONTAINING CONFIDENTIAL AND PROPRIETARY 
*   
*******************************************************************************
*  File Name: Widget.h
*  Author: cg.zhang@atisz.ac.cn
*  Version: 1.0.0
*  Date: 2020-3-19
*  Description: 三维重建
*  History: 
******************************************************************************/

#ifndef REBUILD3D_H
#define REBUILD3D_H

#include <QtWidgets/QWidget>
#include "ui_Rebuild3D.h"
#include "LabelAnalysis.h"

class vtkRenderer;
class vtkImageReader2;

class Rebuild3D : public QWidget
{
	Q_OBJECT

public:
	Rebuild3D(QWidget *parent = Q_NULLPTR);
	~Rebuild3D();

	// 重建
	void Rebuild(vtkImageReader2* pReader);

public:
	Ui::Build3DClass ui;
	vtkGenericOpenGLRenderWindow* m_renderWindow;
	vtkRenderer* m_renderer;
};

#endif
