/******************************************************************************
*  Copyright (C), 2017-2019,Advanced Technology Institute of Suzhou. 
*  THIS IS AN UNPUBLISHED WORK CONTAINING CONFIDENTIAL AND PROPRIETARY 
*   
*******************************************************************************
*  File Name: Widget.cpp
*  Author: cg.zhang@atisz.ac.cn
*  Version: 1.0.0
*  Date: 2020-3-19
*  Description: 三维重建
*  History: 
******************************************************************************/

#include "Rebuild3D.h"
#include "vtkGenericOpenGLRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkRenderWindowInteractor.h"
#include "QVTKOpenGLWidget.h"
#include "vtkCamera.h"
#include "vtkProperty.h"
#include "vtkDataSetMapper.h"
#include "vtkDICOMImageReader.h"
#include "vtkImageCast.h"
#include "vtkMarchingCubes.h"
#include "vtkStripper.h"
#include "vtkPolyDataMapper.h"
#include <vtkRenderWindow.h>
#include "vtkAutoInit.h"

using namespace LabelAnalysis;

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)
VTK_MODULE_INIT(vtkRenderingFreeType)

Rebuild3D::Rebuild3D(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	m_renderWindow = vtkGenericOpenGLRenderWindow::New();
	m_renderer = vtkRenderer::New();
	m_renderer->GetActiveCamera()->Azimuth(30);
	m_renderer->GetActiveCamera()->Elevation(30);
	m_renderer->GetActiveCamera()->Dolly(1.0);
	m_renderWindow->AddRenderer(m_renderer);
	ui.qvtkWidget3D->SetRenderWindow(m_renderWindow);
}

Rebuild3D::~Rebuild3D()
{
	if (nullptr != m_renderWindow)
	{
		m_renderWindow->Delete();
		m_renderWindow = nullptr;
	}

	if (nullptr == m_renderer)
	{
		m_renderer->Delete();
		m_renderWindow = nullptr;
	}
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: Rebuild
// Description: 三维重建
// Parameters:vtkPoints * pVtkPoints 点集
// Parameters:QRgb rgb 颜色
// Parameters:double dThickness 厚度
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void Rebuild3D::Rebuild(vtkImageReader2* pReader)
{
	pReader->Update();

	//对读取数据类型匹配
	vtkImageCast *readerImageCast = vtkImageCast::New();
	readerImageCast->SetInputConnection((vtkAlgorithmOutput *)pReader->GetOutputPort());
	readerImageCast->SetOutputScalarTypeToUnsignedShort();

	vtkMarchingCubes *skinExtractor = vtkMarchingCubes::New();//建立一个Marching Cubes 算法的对象取出等值面的三角面片
	skinExtractor->SetInputConnection((vtkAlgorithmOutput *)readerImageCast->GetOutputPort());//获得所读取的CT 数据
	skinExtractor->SetValue(0, 500);//提取出等值线值为500 的皮肤

	vtkStripper *skinNormals = vtkStripper::New();//建立三角带对象
	skinNormals->SetInputConnection(skinExtractor->GetOutputPort());//将生成的三角片连接成三角带

	vtkPolyDataMapper *skinMapper = vtkPolyDataMapper::New();//建立一个数据映射对象
	skinMapper->SetInputConnection(skinNormals->GetOutputPort());//将三角带映射为几何数据
	skinMapper->ScalarVisibilityOff();

	vtkActor *skin = vtkActor::New(); //建立一个代表皮肤的演员
	skin->SetMapper(skinMapper); //获得皮肤几何数据的属性
	//skin->GetProperty()->SetDiffuseColor(1, .19, .15); //设置皮肤颜色的属性
	skin->GetProperty()->SetSpecular(.3); //设置反射率
	skin->GetProperty()->SetSpecularPower(20); //设置反射光强度

	vtkCamera *aCamera = vtkCamera::New(); //定义摄像机
	aCamera->SetViewUp(0, 0, -1); //取得摄像机方向
	aCamera->SetPosition(0, 1, 0); //光源位置
	aCamera->SetFocalPoint(0, 0, 0); //取焦点坐标
	aCamera->ComputeViewPlaneNormal();

	m_renderer->AddActor(skin);
	m_renderer->SetActiveCamera(aCamera);
	aCamera->Dolly(1.5); //大于1向摄像机焦点移动 小于1则向远离焦点的方向移动

	// Generate an interesting view
	m_renderer->ResetCamera();
	m_renderer->ResetCameraClippingRange();

	// 显示
	m_renderWindow->Render();
}