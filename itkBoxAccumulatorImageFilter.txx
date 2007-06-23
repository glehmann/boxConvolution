#ifndef __itkBoxAccumulatorImageFilter_txx
#define __itkBoxAccumulatorImageFilter_txx

#include "itkBoxAccumulatorImageFilter.h"
#include "itkProgressReporter.h"
#include "itkBoxUtilities.h"

namespace itk {

template<class TInputImage, class TOutputImage>
BoxAccumulatorImageFilter<TInputImage, TOutputImage>
::BoxAccumulatorImageFilter()
{
  m_Radius.Fill(1);
}

template<class TInputImage, class TOutputImage>
void
BoxAccumulatorImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Radius: " << m_Radius << std::endl;
}


////////////////////////////////////////////
template<class TInputImage, class TOutputImage>
void
BoxAccumulatorImageFilter<TInputImage, TOutputImage>
::GenerateData() 
{
  OutputImageType* outputImage = this->GetOutput();
  const InputImageType* inputImage = this->GetInput();
  RegionType inputRegion = outputImage->GetRequestedRegion();

  this->AllocateOutputs();

  ProgressReporter progress(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels());

  BoxAccumulateFunction<TInputImage, TOutputImage>(inputImage, outputImage, inputRegion, inputRegion, progress);
}

} // namespace itk

#endif
