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
#include <itkVTKImageExport.h>
#include <vtkImageImport.h>
#include "itkVersion.h"

#include "itkImageSeriesReader.h"
#include "itkImageSeriesWriter.h"
#include "itkMinimumMaximumImageFilter.h"

#include "itkGDCMImageIO.h"
#include "itkShiftScaleImageFilter.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkNumericSeriesFileNames.h"
#include "itkIdentityTransform.h"
#include "itkLinearInterpolateImageFunction.h"
#include "gdcmUIDGenerator.h"
#include <vtkImageExport.h>
#include <itksys/SystemTools.hxx>
#include <iostream>

using namespace std;

template <typename ITK_Exporter, typename VTK_Importer>
void ConnectPipelines(ITK_Exporter exporter, VTK_Importer* importer)
{
    importer->SetUpdateInformationCallback(exporter->GetUpdateInformationCallback());
    importer->SetPipelineModifiedCallback(exporter->GetPipelineModifiedCallback());
    importer->SetWholeExtentCallback(exporter->GetWholeExtentCallback());
    importer->SetSpacingCallback(exporter->GetSpacingCallback());
    importer->SetOriginCallback(exporter->GetOriginCallback());
    importer->SetScalarTypeCallback(exporter->GetScalarTypeCallback());
    importer->SetNumberOfComponentsCallback(exporter->GetNumberOfComponentsCallback());
    importer->SetPropagateUpdateExtentCallback(exporter->GetPropagateUpdateExtentCallback());
    importer->SetUpdateDataCallback(exporter->GetUpdateDataCallback());
    importer->SetDataExtentCallback(exporter->GetDataExtentCallback());
    importer->SetBufferPointerCallback(exporter->GetBufferPointerCallback());
    importer->SetCallbackUserData(exporter->GetCallbackUserData());
}
/**
* This function will connect the given vtkImageExport filter to
* the given itk::VTKImageImport filter.
*/
template <typename VTK_Exporter, typename ITK_Importer>
void ConnectPipelines(VTK_Exporter* exporter, ITK_Importer importer)
{
    importer->SetUpdateInformationCallback(exporter->GetUpdateInformationCallback());
    importer->SetPipelineModifiedCallback(exporter->GetPipelineModifiedCallback());
    importer->SetWholeExtentCallback(exporter->GetWholeExtentCallback());
    importer->SetSpacingCallback(exporter->GetSpacingCallback());
    importer->SetOriginCallback(exporter->GetOriginCallback());
    importer->SetScalarTypeCallback(exporter->GetScalarTypeCallback());
    importer->SetNumberOfComponentsCallback(exporter->GetNumberOfComponentsCallback());
    importer->SetPropagateUpdateExtentCallback(exporter->GetPropagateUpdateExtentCallback());
    importer->SetUpdateDataCallback(exporter->GetUpdateDataCallback());
    importer->SetDataExtentCallback(exporter->GetDataExtentCallback());
    importer->SetBufferPointerCallback(exporter->GetBufferPointerCallback());
    importer->SetCallbackUserData(exporter->GetCallbackUserData());
}

int main() {


    typedef unsigned short    PixelType;
    const unsigned int      Dimension = 3;

    typedef itk::Image< PixelType, Dimension >         ImageType;

    typedef itk::VTKImageExport<ImageType> ImageExportType;
    typedef itk::ImageSeriesReader< ImageType >        ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    typedef itk::GDCMImageIO       ImageIOType;
    ImageIOType::Pointer dicomIO = ImageIOType::New();

    reader->SetImageIO( dicomIO );
    typedef itk::GDCMSeriesFileNames NamesGeneratorType;
    NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();

    nameGenerator->SetUseSeriesDetails( true );
    nameGenerator->AddSeriesRestriction("0008|0021" );
    nameGenerator->SetDirectory( "/Users/mac/BIOMED/Subjects/testSubject");

    typedef std::vector< std::string >    SeriesIdContainer;
    const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();
    std::cout << seriesUID.size() << std::endl;
    SeriesIdContainer::const_iterator seriesItr = seriesUID.begin();
    SeriesIdContainer::const_iterator seriesEnd = seriesUID.end();
    while( seriesItr != seriesEnd )
    {
        std::cout << seriesItr->c_str() << std::endl;
        seriesItr++;
    }
    std::string seriesIdentifier;
    seriesIdentifier = seriesUID.begin()->c_str();
    std::cout << seriesIdentifier.c_str() << std::endl;

    typedef std::vector< std::string >   FileNamesContainer;
    FileNamesContainer fileNames;
    fileNames = nameGenerator->GetFileNames( seriesIdentifier );

    reader->SetFileNames( fileNames );
    try
    {
        reader->Update();
    }
    catch (itk::ExceptionObject &ex)
    {
        std::cout << ex << std::endl;
    }

//------------------------------------------------------------------------
// ITK to VTK pipeline connection.
//------------------------------------------------------------------------

// Create the itk::VTKImageExport instance and connect it to the
// itk::CurvatureFlowImageFilter.
    ImageExportType::Pointer exporter = ImageExportType::New();
    exporter->SetInput(reader->GetOutput());
    exporter->Update();

// Create the vtkImageImport and connect it to the
// itk::VTKImageExport instance.
    vtkImageImport* importer = vtkImageImport::New();
    ConnectPipelines(exporter, importer);

//------------------------------------------------------------------------
// VTK pipeline.
//------------------------------------------------------------------------


    //Demo Display DICOM Image
    /*
    string folder = "/Users/mac/BIOMED/Subjects/testSubject";
    // Read all the DICOM files in the specified directory.
    vtkSmartPointer<vtkDICOMImageReader> reader2 =
            vtkSmartPointer<vtkDICOMImageReader>::New();
    reader2->SetDirectoryName(folder.c_str());
    reader2->Update();
*/

    vtkSmartPointer<vtkImageSliceMapper> imageSliceMapper = vtkSmartPointer<vtkImageSliceMapper>::New();
    imageSliceMapper->SetInputConnection(importer->GetOutputPort());

    //imageSliceMapper->SetInputData(importer->GetOutput());
    imageSliceMapper->Update();


    vtkSmartPointer<vtkImageSlice> imageSlice = vtkSmartPointer<vtkImageSlice>::New();
    imageSlice->SetMapper(imageSliceMapper);



    vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper =
            vtkSmartPointer<vtkSmartVolumeMapper>::New();
    volumeMapper->SetBlendModeToComposite();
    volumeMapper->SetInputConnection(importer->GetOutputPort());

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



    vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
    volume->SetMapper(volumeMapper);
    volume->SetProperty(volumeProperty);
    renderer->AddViewProp(volume);



    renderer->ResetCamera();
    volumeMapper->SetRequestedRenderModeToRayCast();
    volumeMapper->SetBlendModeToMinimumIntensity();

    renderWindow->Render();

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();

    // Render and start interaction
    renderWindowInteractor->SetRenderWindow(renderWindow);
    renderWindowInteractor->Initialize();

    renderWindowInteractor->Start();


    return 0;
}
