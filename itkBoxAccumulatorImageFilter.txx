#ifndef __itkBoxAccumulatorImageFilter_txx
#define __itkBoxAccumulatorImageFilter_txx

#include "itkBoxAccumulatorImageFilter.h"
#include "itkShapedNeighborhoodIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "itkConnectedComponentAlgorithm.h"
#include "itkConstantBoundaryCondition.h"
#include "itkProgressReporter.h"

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

  // copy the input to the accumulator
  typedef ImageRegionConstIterator<TInputImage> InputIterator;
  typedef ImageRegionIterator<TOutputImage> OutputIterator;

  typedef ShapedNeighborhoodIterator<TOutputImage> NOutputIterator;
  InputIterator inIt( inputImage, inputRegion);
  typename TInputImage::SizeType kernelRadius;
  kernelRadius.Fill(1);

  NOutputIterator noutIt( kernelRadius, outputImage, inputRegion);
  // this iterator is face connected
  setConnectivityPrevious( &noutIt, false );

  ConstantBoundaryCondition<OutputImageType> oBC;
  oBC.SetConstant(0);
  noutIt.OverrideBoundaryCondition(&oBC);
  // This uses several iterators. An alternative and probably better
  // approach would be to copy the input to the output and convolve
  // with the following weights (in 2D)
  //   -(dim - 1)  1
  //       1       1
  // The result of each convolution needs to get written back to the
  // image being convolved so that the accumulation propogates
  // This should be implementable with neighborhood operators.

  // this one points to the diagonal
  NOutputIterator noutIt2( kernelRadius, outputImage, inputRegion);
  typename NOutputIterator::OffsetType offset;
  offset.Fill(-1);
  noutIt2.ActivateOffset(offset);
  noutIt2.OverrideBoundaryCondition(&oBC);

  inIt.GoToBegin();
  noutIt.GoToBegin();
  noutIt2.GoToBegin();
  int Weight = TInputImage::ImageDimension - 1;

  typename NOutputIterator::ConstIterator sIt;
  while ( !noutIt.IsAtEnd() )
    {
    InputPixelType currentValue = inIt.Get();
    OutputPixelType Sum = 0;
    for (sIt = noutIt.Begin(); !sIt.IsAtEnd();++sIt)
      {
      Sum += sIt.Get();
      }
    for (sIt = noutIt2.Begin(); !sIt.IsAtEnd();++sIt)
      {
      Sum -= Weight * sIt.Get();
      }
    noutIt.SetCenterPixel(Sum + inIt.Get());
    ++inIt;
    ++noutIt;
    ++noutIt2;
    progress.CompletedPixel();
    }

}

} // namespace itk

#endif
