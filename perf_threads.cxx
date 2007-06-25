#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkBoxMeanImageFilter.h"
#include "itkMeanImageFilter.h"
#include "itkTimeProbe.h"
#include <vector>
#include "itkFlatStructuringElement.h"
#include <iomanip>

int main(int, char * argv[])
{
  const int dim = 2;
  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;
  typedef unsigned char AType;
  typedef itk::Image< AType, dim > AccType;

  
  // read the input image
  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  
  typedef itk::BoxMeanImageFilter< IType, AccType > BoxConvType;
  BoxConvType::Pointer boxConv = BoxConvType::New();
  boxConv->SetInput( reader->GetOutput() );
  boxConv->SetRadius( 5 );
  
  typedef itk::MeanImageFilter< IType, AccType > DirectConvType;
  DirectConvType::Pointer directConv = DirectConvType::New();
  directConv->SetInput( reader->GetOutput() );
  DirectConvType::InputSizeType radius;
  radius.Fill( 5 );
  directConv->SetRadius( radius );

  
/*  // write 
  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( dilate->GetOutput() );
  writer->SetFileName( argv[2] );*/
  
  reader->Update();
  
  std::cout << "#nb" << "\t" 
            << "Direct" << "\t"
            << "Box" << "\t"
            << std::endl;

  for( int t=1; t<=10; t++ )
    {
    itk::TimeProbe origtime;
    itk::TimeProbe boxtime;

    boxConv->SetNumberOfThreads( t );
    directConv->SetNumberOfThreads( t );
    
    for( int i=0; i<50; i++ )
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
      
    std::cout << std::setprecision(3) << t << "\t" 
              << origtime.GetMeanTime() << "\t"
              << boxtime.GetMeanTime() << "\t"
              << std::endl;
    }
  
  
  return 0;
}

