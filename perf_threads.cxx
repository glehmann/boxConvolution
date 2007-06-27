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
  typedef float AType;
  typedef itk::Image< AType, dim > AccType;

  
  // read the input image
  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  
  typedef itk::BoxMeanImageFilter< IType, IType > BoxConvType;
  BoxConvType::Pointer boxConv = BoxConvType::New();
  boxConv->SetInput( reader->GetOutput() );
  boxConv->SetRadius( 3 );
  
  typedef itk::MeanImageFilter< IType, IType > DirectConvType;
  DirectConvType::Pointer directConv = DirectConvType::New();
  directConv->SetInput( reader->GetOutput() );
  DirectConvType::InputSizeType radius;
  radius.Fill( 3 );
  directConv->SetRadius( radius );

  typedef itk::BoxAccumulatorImageFilter< IType, AccType > BoxAccType;
  BoxAccType::Pointer acc = BoxAccType::New();
  acc->SetInput( reader->GetOutput() );
  
  typedef itk::BoxMeanCalculatorImageFilter< AccType, IType > BoxMeanType;
  BoxMeanType::Pointer mean = BoxMeanType::New();
  mean->SetInput( acc->GetOutput() );
  mean->SetRadius( radius );
  
  
/*  // write 
  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( dilate->GetOutput() );
  writer->SetFileName( argv[2] );*/
  
  reader->Update();
  
  std::cout << "#nb" << "\t" 
            << "Direct" << "\t"
            << "Box" << "\t"
            << "Acc" << "\t"
            << std::endl;

  for( int t=1; t<=10; t++ )
    {
    itk::TimeProbe origtime;
    itk::TimeProbe boxtime;
    itk::TimeProbe acctime;
    itk::TimeProbe meantime;

    boxConv->SetNumberOfThreads( t );
    directConv->SetNumberOfThreads( t );
    acc->SetNumberOfThreads( t );
    mean->SetNumberOfThreads( t );
    
    for( int i=0; i<10; i++ )
      {
      boxConv->Modified();
      boxtime.Start();
      boxConv->Update();
      boxtime.Stop();

      directConv->Modified();
      origtime.Start();
      directConv->Update();
      origtime.Stop();

      acc->Modified();
      acctime.Start();
      acc->Update();
      acctime.Stop();

      mean->Modified();
      meantime.Start();
      mean->Update();
      meantime.Stop();
      }
      
    std::cout << std::setprecision(3) << t << "\t" 
              << origtime.GetMeanTime() << "\t"
              << boxtime.GetMeanTime() << "\t"
              << acctime.GetMeanTime() << "\t"
              << meantime.GetMeanTime() << "\t"
              << std::endl;
    }
  
  
  return 0;
}

