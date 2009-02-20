// Author: Andrew J. P. Maclean
// Purpose read in a dataset where each line consists of point
// with its position (x,y,z) and (possibly) a scalar.
// Then create and save a vtk polydata array suitable for further rendering.

#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkParticleReader.h>
#include <vtkSmartPointer.h>



int main ( int argc, char* argv[] )
{
  if ( argc != 3 )
  {
    std::cout << "Need input file and output file." << std::endl;
    return 1;
  }

  std::string ifn = argv[1];
  std::string ofn = argv[2];
  std::cerr << "Input file: " << ifn << std::endl;
  std::cerr << "Output file: " << ofn << std::endl;

  if ( ifn == ofn )
  {
    std::cerr << "File names cannot be the same." << std::endl;
    return 1;
  }

  vtkSmartPointer<vtkParticleReader> reader =
    vtkSmartPointer<vtkParticleReader>::New();

  reader->SetFileName(ifn.c_str());
  reader->Update();

  vtkSmartPointer<vtkXMLPolyDataWriter> writer =
    vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  writer->SetInput(reader->GetOutput());
  writer->SetFileName(ofn.c_str());
  writer->Write();

  // Set up the pipeline.

  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();

  mapper->SetInput(reader->GetOutput());

  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
  ren->SetBackground(0.2,0.2 ,0.3);

  vtkSmartPointer<vtkRenderWindow> renWin =
    vtkSmartPointer<vtkRenderWindow>::New();
  renWin->AddRenderer(ren);
  renWin->SetSize(800 ,600);

  vtkSmartPointer<vtkRenderWindowInteractor> iRen =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  iRen->SetRenderWindow(renWin);
  ren->AddViewProp(actor);
  ren->Render();
  iRen->Initialize();
  iRen->Start();
}


