#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkSimpleFilterWatcher.h"

#include "itkBoxMeanImageFilter.h"
#include "itkMeanImageFilter.h"
#include "itkBoxSigmaImageFilter.h"
#include "itkNoiseImageFilter.h"
#include "itkTimeProbe.h"
#include <iomanip>


int main(int argc, char * argv[])
{

  if( argc !=  4)
    {
    std::cerr << "usage: " << argv[0] << " " << std::endl;
    std::cerr << "  : " << argc << std::endl;
    exit(1);
    }

  const int dim = 2;

  itk::MultiThreader::SetGlobalDefaultNumberOfThreads(1);  
  
  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;
  typedef unsigned char AType;
  typedef itk::Image< AType, dim > AccType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  reader->Update();

  typedef itk::BoxMeanImageFilter< IType, AccType > BoxConvType;
  BoxConvType::Pointer boxConv = BoxConvType::New();
  boxConv->SetInput( reader->GetOutput() );
  //itk::SimpleFilterWatcher watcher(filter, "filter");
  typedef itk::MeanImageFilter< IType, AccType > DirectConvType;
  DirectConvType::Pointer directConv = DirectConvType::New();
  directConv->SetInput( reader->GetOutput() );
  DirectConvType::InputSizeType radius;

  typedef itk::BoxSigmaImageFilter< IType, AccType > BoxSigmaType;
  BoxSigmaType::Pointer boxSigma = BoxSigmaType::New();
  boxSigma->SetInput( reader->GetOutput() );
  //itk::SimpleFilterWatcher watcher(filter, "filter");
  typedef itk::NoiseImageFilter< IType, AccType > DirectSigmaType;
  DirectSigmaType::Pointer directSigma = DirectSigmaType::New();
  directSigma->SetInput( reader->GetOutput() );

  std::cout << "Radius" << "\t"
            << "DirectM" << "\t"
            << "BoxM" << "\t"
            << "DirectS" << "\t"
            << "BoxS" << "\t"
            << std::endl;

  std::vector<int> Sizes;

  for (unsigned R = 1; R < 11; R++)
    {
    Sizes.push_back(R);
    }

  Sizes.push_back(15);
  Sizes.push_back(20);
  Sizes.push_back(25);

  for (unsigned k = 0; k<Sizes.size();k++)
    {
    unsigned R = Sizes[k];

    boxConv->SetRadius(R);
    radius.Fill(R);
    directConv->SetRadius(radius);
    boxSigma->SetRadius(R);
    directSigma->SetRadius(radius);
    
    itk::TimeProbe origtime;
    itk::TimeProbe boxtime;

    itk::TimeProbe sorigtime;
    itk::TimeProbe sboxtime;

    for( int i=0; i<15; i++ )
      {
      boxtime.Start();
      boxConv->Update();
      boxtime.Stop();
      boxConv->Modified();

      origtime.Start();
      directConv->Update();
      origtime.Stop();
      directConv->Modified();

      sboxtime.Start();
      boxSigma->Update();
      sboxtime.Stop();
      boxSigma->Modified();

      sorigtime.Start();
      directSigma->Update();
      sorigtime.Stop();
      directSigma->Modified();

      }
    std::cout << std::setprecision(3) << R << "\t"
              << origtime.GetMeanTime() << "\t"
              << boxtime.GetMeanTime() << "\t"
              << sorigtime.GetMeanTime() << "\t"
              << sboxtime.GetMeanTime() << "\t"
              << std::endl;

    }

  typedef itk::ImageFileWriter< AccType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( boxConv->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();


  writer->SetInput( directConv->GetOutput() );
  writer->SetFileName( argv[3] );
  writer->Update();
  
  

  return 0;
}

