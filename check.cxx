#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkSimpleFilterWatcher.h"

#include "itkBoxMeanImageFilter.h"
#include "itkMeanImageFilter.h"
#include "itkTimeProbe.h"


int main(int argc, char * argv[])
{

  if( argc !=  4)
    {
    std::cerr << "usage: " << argv[0] << " " << std::endl;
    std::cerr << "  : " << argc << std::endl;
    exit(1);
    }

  const int dim = 2;
  
  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;
  typedef unsigned char AType;
  typedef itk::Image< AType, dim > AccType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  typedef itk::BoxMeanImageFilter< IType, AccType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );
  filter->SetRadius(3);
  //itk::SimpleFilterWatcher watcher(filter, "filter");

  typedef itk::ImageFileWriter< AccType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  typedef itk::MeanImageFilter< IType, AccType > OrigFilterType;
  OrigFilterType::Pointer filterOrig = OrigFilterType::New();
  OrigFilterType::InputSizeType radius;
  radius.Fill(3);
  filterOrig->SetInput( reader->GetOutput() );
  filterOrig->SetRadius(radius);

  writer->SetInput( filterOrig->GetOutput() );
  writer->SetFileName( argv[3] );
  writer->Update();
  
  

  return 0;
}

