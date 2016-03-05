#include "itkImage.h"
#include <vtkCylinderSource.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <iostream>

using namespace std;

int main() {

    typedef itk::Image< unsigned short, 3 > ImageType;

    ImageType::Pointer image = ImageType::New();

    // Create a sphere
    vtkSmartPointer<vtkCylinderSource> cylinderSource =
            vtkSmartPointer<vtkCylinderSource>::New();
    cylinderSource->SetCenter(0.0, 0.0, 0.0);
    cylinderSource->SetRadius(5.0);
    cylinderSource->SetHeight(7.0);
    cylinderSource->SetResolution(100);

    // Create a mapper and actor
    vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(cylinderSource->GetOutputPort());
    vtkSmartPointer<vtkActor> actor =
            vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    //Create a renderer, render window, and interactor
    vtkSmartPointer<vtkRenderer> renderer =
            vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> renderWindow =
            vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);

    // Add the actor to the scene
    renderer->AddActor(actor);
    renderer->SetBackground(.1, .3,.2); // Background color dark green

    // Render and interact
    renderWindow->SetWindowName("Fire Team Demo");
    renderWindow->Render();
    renderWindowInteractor->Start();

    cout << "Hello, World!" << endl;
    return 0;
}
