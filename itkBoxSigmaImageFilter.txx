#ifndef __itkBoxSigmaImageFilter_txx
#define __itkBoxSigmaImageFilter_txx

#include "itkImage.h"
#include "itkBoxSigmaImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkOffset.h"
#include "itkProgressAccumulator.h"
#include "itkNumericTraits.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkBoxAccumulatorImageFilter.h"
#include "itkBoxMeanCalculatorImageFilter.h"
#include "itkShapedNeighborhoodIterator.h"

// #include "ioutils.h"

namespace itk {


template<class TInputImage, class TOutputImage>
BoxSigmaImageFilter<TInputImage, TOutputImage>
::BoxSigmaImageFilter()
{
  m_Radius.Fill(1);
}
template<class TInputImage, class TOutputImage>
void
BoxSigmaImageFilter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the input and output
  typename Superclass::InputImagePointer  inputPtr =
    const_cast< TInputImage * >( this->GetInput() );

  if ( !inputPtr )
    {
    return;
    }

  // get a copy of the input requested region (should equal the output
  // requested region)
  typename TInputImage::RegionType inputRequestedRegion;
  inputRequestedRegion = inputPtr->GetRequestedRegion();

  // pad the input requested region by the operator radius
  inputRequestedRegion.PadByRadius( m_Radius );

  // crop the input requested region at the input's largest possible region
  if ( inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()) )
    {
    inputPtr->SetRequestedRegion( inputRequestedRegion );
    return;
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // store what we tried to request (prior to trying to crop)
    inputPtr->SetRequestedRegion( inputRequestedRegion );

    // build an exception
    InvalidRequestedRegionError e(__FILE__, __LINE__);
    OStringStream msg;
    msg << static_cast<const char *>(this->GetNameOfClass())
        << "::GenerateInputRequestedRegion()";
    e.SetLocation(msg.str().c_str());
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
    }
}

#if 0
template<class TInputImage, class TOutputImage>
void
BoxSigmaImageFilter<TInputImage, TOutputImage>
::GenerateData() 
// ::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, int threadId) 
{
  this->AllocateOutputs();

  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();

  progress->SetMiniPipelineFilter(this);
 
  // Accumulate type is too small
  typedef typename itk::NumericTraits<PixelType>::RealType AccPixType;
  typedef typename itk::Image<AccPixType, TInputImage::ImageDimension> AccumImageType;
  
  typedef typename itk::BoxAccumulatorImageFilter<TInputImage, AccumImageType> AccumType;
  typedef typename itk::BoxMeanCalculatorImageFilter<AccumImageType, TOutputImage> MeanType;
  
  typename AccumType::Pointer accumulator = AccumType::New();
  typename MeanType::Pointer boxmean = MeanType::New();
  accumulator->SetInput(this->GetInput());
  boxmean->SetRadius(m_Radius);
  boxmean->SetInput(accumulator->GetOutput());
  boxmean->SetNumberOfThreads( this->GetNumberOfThreads() );
  boxmean->GraftOutput(this->GetOutput());
  progress->RegisterInternalFilter(accumulator, 0.5f);
  progress->RegisterInternalFilter(boxmean, 0.5f);
  boxmean->Update();
  boxmean->GraftOutput(this->GetOutput());
}
#else

// threaded version - need to allocate an internal buffer for the
// accumulation image
template<class TInputImage, class TOutputImage>
void
BoxSigmaImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, int threadId) 
{

  ProgressReporter progress(this, 0, 2*outputRegionForThread.GetNumberOfPixels());
  // Accumulate type is too small
  typedef typename itk::NumericTraits<PixelType>::RealType AccValueType;
  typedef typename itk::Vector<AccValueType, 2> AccPixType;
  typedef typename itk::Image<AccPixType, TInputImage::ImageDimension> AccumImageType;

  typename TInputImage::SizeType internalRadius;
  for( int i=0; i<TInputImage::ImageDimension; i++ )
    {
    internalRadius[i] = m_Radius[i] + 1;
    }
  

  const InputImageType* inputImage = this->GetInput();
  OutputImageType *outputImage = this->GetOutput();
  RegionType accumRegion = outputRegionForThread;
  accumRegion.PadByRadius(internalRadius);
  accumRegion.Crop(inputImage->GetRequestedRegion());

  typename AccumImageType::Pointer accImage = AccumImageType::New();
  accImage->SetRegions(accumRegion);
  accImage->Allocate();
  typename AccumImageType::ConstPointer accImageConst = static_cast<typename AccumImageType::ConstPointer>(accImage);

  BoxSquareAccumulateFunction<TInputImage, AccumImageType >(inputImage, accImage, 
						     accumRegion,
						     accumRegion,
						     progress);
//   writeIm<AccumImageType>(accImage, "/tmp/acc.nii.gz");
  BoxSigmaCalculatorFunction<AccumImageType, TOutputImage>(accImageConst, outputImage,
							  accumRegion,
							  outputRegionForThread,
							  m_Radius,
							  progress);
						     

}
#endif


}// end namespace itk
#endif