#ifndef __itkBoxMeanCalculatorImageFilter_txx
#define __itkBoxMeanCalculatorImageFilter_txx

#include "itkImage.h"
#include "itkBoxMeanCalculatorImageFilter.h"
#include "itkProgressReporter.h"

namespace itk {


template<class TInputImage, class TOutputImage>
BoxMeanCalculatorImageFilter<TInputImage, TOutputImage>
::BoxMeanCalculatorImageFilter()
{
}

template<class TInputImage, class TOutputImage>
void
BoxMeanCalculatorImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, int threadId) 
{

  ProgressReporter progress( this, threadId, outputRegionForThread.GetNumberOfPixels() );

  OutputImageType* outputImage = this->GetOutput();
  const InputImageType* accImage = this->GetInput();
  RegionType inputRegion = accImage->GetRequestedRegion();

//   sleep(threadId);
//   std::cout << "*****************" << std::endl;

  BoxMeanCalculatorFunction<TInputImage, TOutputImage>(accImage, outputImage,
						       inputRegion, outputRegionForThread,
						       this->GetRadius(), progress);

}



}// end namespace itk
#endif
