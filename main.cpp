#include "itkImage.h"
#include <vtkCylinderSource.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageViewer2.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageMapper.h>
#include <vtkImageSliceMapper.h>
#include <vtkImageSlice.h>
#include <vtkVolumeProperty.h>
#include <vtkSmartVolumeMapper.h>

#include <iostream>

using namespace std;

int main() {

    typedef itk::Image< unsigned short, 3 > ImageType;

    ImageType::Pointer image = ImageType::New();


    //Demo Display DICOM Image
    string folder = "/Users/mac/BIOMED/Subjects/testSubject";
    // Read all the DICOM files in the specified directory.
    vtkSmartPointer<vtkDICOMImageReader> reader =
            vtkSmartPointer<vtkDICOMImageReader>::New();
    reader->SetDirectoryName(folder.c_str());
    reader->Update();


    vtkSmartPointer<vtkImageSliceMapper> imageSliceMapper = vtkSmartPointer<vtkImageSliceMapper>::New();
    imageSliceMapper->SetInputConnection(reader->GetOutputPort());
    //imageSliceMapper->SetInputData(reader->GetOutput());

    vtkSmartPointer<vtkImageSlice> imageSlice = vtkSmartPointer<vtkImageSlice>::New();
    imageSlice->SetMapper(imageSliceMapper);


    vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper =
            vtkSmartPointer<vtkSmartVolumeMapper>::New();
    volumeMapper->SetBlendModeToComposite(); // composite first
    volumeMapper->SetInputConnection(reader->GetOutputPort());

    vtkSmartPointer<vtkVolumeProperty> volumeProperty =
            vtkSmartPointer<vtkVolumeProperty>::New();
    volumeProperty->ShadeOff();
    volumeProperty->SetInterpolationType(VTK_LINEAR_INTERPOLATION);



    // Setup renderers
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddViewProp(imageSlice);
    renderer->ResetCamera();
    renderer->SetBackground(0.1,0.4,0.2);

    // Setup render window
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->SetSize(500, 500);
    renderWindow->AddRenderer(renderer);

    // Setup render window interactor
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();






    vtkSmartPointer<vtkVolume> volume =
            vtkSmartPointer<vtkVolume>::New();
    volume->SetMapper(volumeMapper);
    volume->SetProperty(volumeProperty);
    renderer->AddViewProp(volume);
    renderer->ResetCamera();
    volumeMapper->SetRequestedRenderModeToRayCast();
    renderWindow->Render();

    // Render and start interaction
    renderWindowInteractor->SetRenderWindow(renderWindow);
    renderWindowInteractor->Initialize();

    renderWindowInteractor->Start();


    return 0;
}
