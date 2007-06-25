#include "itkMultiThreader.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkSimpleFilterWatcher.h"

#include "itkBoxMeanImageFilter.h"
#include "itkMeanImageFilter.h"
#include "itkTimeProbe.h"
#include "itkRescaleIntensityImageFilter.h"


int main(int argc, char * argv[])
{

  if( argc !=  7 )
    {
    std::cerr << "usage: " << argv[0] << " input boxMean mean acc rescaledAcc radius" << std::endl;
    std::cerr << "  : " << argc << std::endl;
    exit(1);
    }

  const int dim = 3;
//   itk::MultiThreader::SetGlobalDefaultNumberOfThreads(1);  
  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;
  typedef float AType;
  typedef itk::Image< AType, dim > AccType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  typedef itk::BoxMeanImageFilter< IType, IType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );
  filter->SetRadius( atoi( argv[6] ) );
  itk::SimpleFilterWatcher watcher(filter, "filter");

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->SetUseCompression( true );
  writer->Update();

  typedef itk::MeanImageFilter< IType, IType > OrigFilterType;
  OrigFilterType::Pointer filterOrig = OrigFilterType::New();
  OrigFilterType::InputSizeType radius;
  radius.Fill( atoi( argv[6] ) );
  filterOrig->SetInput( reader->GetOutput() );
  filterOrig->SetRadius(radius);

  writer->SetInput( filterOrig->GetOutput() );
  writer->SetFileName( argv[3] );
  writer->Update();
  
  typedef itk::BoxAccumulatorImageFilter< IType, AccType > AccumulatorType;
  AccumulatorType::Pointer accumulator = AccumulatorType::New();
  accumulator->SetInput( reader->GetOutput() );
  itk::SimpleFilterWatcher watcher2(accumulator, "accumulator");

  typedef itk::ImageFileWriter< AccType > AccWriterType;
  AccWriterType::Pointer accWriter = AccWriterType::New();
  accWriter->SetInput( accumulator->GetOutput() );
  accWriter->SetFileName( argv[4] );
  accWriter->SetUseCompression( true );
  accWriter->Update();

  typedef itk::RescaleIntensityImageFilter< AccType, IType > RescaleType;
  RescaleType::Pointer rescale = RescaleType::New();
  rescale->SetInput( accumulator->GetOutput() );

  writer->SetInput( rescale->GetOutput() );
  writer->SetFileName( argv[5] );
  writer->Update();
  
  return 0;
}

