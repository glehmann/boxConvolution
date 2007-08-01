#ifndef __itkBoxMeanImageFilter_txx
#define __itkBoxMeanImageFilter_txx

#include "itkImage.h"
#include "itkBoxMeanImageFilter.h"
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
BoxMeanImageFilter<TInputImage, TOutputImage>
::BoxMeanImageFilter()
{
}


#if 0
template<class TInputImage, class TOutputImage>
void
BoxMeanImageFilter<TInputImage, TOutputImage>
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
  boxmean->SetRadius( this->GetRadius() );
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
BoxMeanImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, int threadId) 
{

  ProgressReporter progress(this, 0, 2*outputRegionForThread.GetNumberOfPixels());
  // Accumulate type is too small
  typedef typename itk::NumericTraits<PixelType>::RealType AccPixType;
  typedef typename itk::Image<AccPixType, TInputImage::ImageDimension> AccumImageType;

  typename TInputImage::SizeType internalRadius;
  for( int i=0; i<TInputImage::ImageDimension; i++ )
    {
    internalRadius[i] = this->GetRadius()[i] + 1;
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

  BoxAccumulateFunction<TInputImage, AccumImageType>(inputImage, accImage, 
						     accumRegion,
						     accumRegion,
						     progress);
  BoxMeanCalculatorFunction<AccumImageType, TOutputImage>(accImageConst, outputImage,
							  accumRegion,
							  outputRegionForThread,
							  this->GetRadius(),
							  progress);
						     

}
#endif


}// end namespace itk
#endif
