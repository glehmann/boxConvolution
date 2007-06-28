#include "itkMultiThreader.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkSimpleFilterWatcher.h"

#include "itkBoxSigmaImageFilter.h"
#include "itkNoiseImageFilter.h"
#include "itkTimeProbe.h"
#include "itkRescaleIntensityImageFilter.h"


int main(int argc, char * argv[])
{

  if( argc !=  5 )
    {
    std::cerr << "usage: " << argv[0] << " input boxSigma sigma radius" << std::endl;
    std::cerr << "  : " << argc << std::endl;
    exit(1);
    }

  const int dim = 2;
//   itk::MultiThreader::SetGlobalDefaultNumberOfThreads(1);  
  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;
  typedef float AType;
  typedef itk::Image< AType, dim > AccType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  typedef itk::BoxSigmaImageFilter< IType, IType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );
  filter->SetRadius( atoi( argv[4] ) );
  itk::SimpleFilterWatcher watcher(filter, "filter");

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  typedef itk::NoiseImageFilter< IType, IType > OrigFilterType;
  OrigFilterType::Pointer filterOrig = OrigFilterType::New();
  OrigFilterType::InputSizeType radius;
  radius.Fill( atoi( argv[4] ) );
  filterOrig->SetInput( reader->GetOutput() );
  filterOrig->SetRadius(radius);

  writer->SetInput( filterOrig->GetOutput() );
  writer->SetFileName( argv[3] );
  writer->Update();
  
  return 0;
}

