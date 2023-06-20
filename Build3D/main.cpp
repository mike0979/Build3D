#include "Rebuild3D.h"
#include <QtWidgets/QApplication>
#include "LabelAnalysis.h"
#include <vtkSmartPointer.h>
#include "vtkDICOMImageReader.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Rebuild3D w;
	//vtkSmartPointer<vtkDICOMImageReader> pReader = vtkSmartPointer<vtkDICOMImageReader>::New();
	//pReader->SetDirectoryName("C:\\Users\\Administrator\\Desktop\\SE0");
	//w.Rebuild(pReader);
	w.show();
	return a.exec();
}
