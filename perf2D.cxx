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
  reader->Update();

  typedef itk::BoxMeanImageFilter< IType, AccType > BoxConvType;
  BoxConvType::Pointer boxConv = BoxConvType::New();
  boxConv->SetInput( reader->GetOutput() );
  //itk::SimpleFilterWatcher watcher(filter, "filter");
  typedef itk::MeanImageFilter< IType, AccType > DirectConvType;
  DirectConvType::Pointer directConv = DirectConvType::New();
  directConv->SetInput( reader->GetOutput() );
  DirectConvType::InputSizeType radius;

  std::cout << "Size" << "\t"
            << "Direct" << "\t"
            << "Box" << "\t"
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
    
    itk::TimeProbe origtime;
    itk::TimeProbe boxtime;

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

      }
    std::cout << R << "\t"
              << origtime.GetMeanTime() << "\t"
              << boxtime.GetMeanTime() << "\t"
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

