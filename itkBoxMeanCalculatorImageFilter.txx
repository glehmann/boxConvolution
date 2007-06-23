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
  m_Radius.Fill(1);
}

template<class TInputImage, class TOutputImage>
void
BoxMeanCalculatorImageFilter<TInputImage, TOutputImage>
::GenerateData()
//::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, int threadId) 
{

  ProgressReporter progress(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels());

  OutputImageType* outputImage = this->GetOutput();
  const InputImageType* accImage = this->GetInput();
  RegionType inputRegion = outputImage->GetRequestedRegion();
  this->AllocateOutputs();

  BoxMeanCalculatorFunction<TInputImage, TOutputImage>(accImage, outputImage,
						       inputRegion, inputRegion,
						       m_Radius, progress);

}



}// end namespace itk
#endif
