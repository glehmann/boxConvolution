#ifndef __itkBoxMeanCalculatorImageFilter_txx
#define __itkBoxMeanCalculatorImageFilter_txx

#include "itkImage.h"
#include "itkBoxMeanCalculatorImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkOffset.h"
#include "itkProgressAccumulator.h"
#include "itkNumericTraits.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkBoxAccumulatorImageFilter.h"
#include "itkShapedNeighborhoodIterator.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"

namespace itk {


template<class TInputImage, class TOutputImage>
BoxMeanCalculatorImageFilter<TInputImage, TOutputImage>
::BoxMeanCalculatorImageFilter()
{
  m_Radius.Fill(1);
}
#if 0
template<class TInputImage, class TOutputImage>
void
BoxMeanCalculatorImageFilter<TInputImage, TOutputImage>
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

#endif
template<class TInputImage, class TOutputImage>
void
BoxMeanCalculatorImageFilter<TInputImage, TOutputImage>
::GenerateData()
//::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, int threadId) 
{
  ZeroFluxNeumannBoundaryCondition<TInputImage> nbc;

  ProgressReporter progress(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels());

  OutputImageType* outputImage = this->GetOutput();
  const InputImageType* accImage = this->GetInput();
  RegionType inputRegion = outputImage->GetRequestedRegion();
  this->AllocateOutputs();

  // use the face generator for speed
  typedef typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType> FaceCalculatorType;
  typedef typename FaceCalculatorType::FaceListType FaceListType;
  typedef typename FaceCalculatorType::FaceListType::iterator FaceListTypeIt;
  FaceCalculatorType faceCalculator;

  FaceListType faceList;
  FaceListTypeIt fit;

  // this process is actually slightly asymmetric because we need to
  // subtract rectangles that are next to our kernel, not overlapping it
  SizeType kernelSize, internalRadius;
  for( int i=0; i<ImageDimension; i++ )
    {
    kernelSize[i] = m_Radius[i] * 2 + 1;
    internalRadius[i] = m_Radius[i] + 1;
    }

  faceList = faceCalculator(accImage, inputRegion, internalRadius);
  // start with the body region
  for (fit = faceList.begin(); fit != faceList.end(); ++fit)
    {


    typedef typename itk::ShapedNeighborhoodIterator<InputImageType> NInputIterator;
    NInputIterator N1(internalRadius, accImage, *fit);
    NInputIterator N2(internalRadius, accImage, *fit);
    NInputIterator N3(internalRadius, accImage, *fit);
    N1.OverrideBoundaryCondition(&nbc);
    N2.OverrideBoundaryCondition(&nbc);
    N3.OverrideBoundaryCondition(&nbc);

    int Weight = TInputImage::ImageDimension - 1;
    // set the correct corners
    typename NInputIterator::OffsetType offset1, offset2;
    int pixelscount = 1;
    for (unsigned i = 0; i < TInputImage::ImageDimension; i++)
      {
      offset1[i] = m_Radius[i];
      offset2[i] = -internalRadius[i];
      pixelscount *= 2*m_Radius[i] + 1;
      }

    N1.ActivateOffset(offset1);
    N2.ActivateOffset(offset2);
    for (unsigned k = 0; k < TInputImage::ImageDimension; k++)
      {
      typename NInputIterator::OffsetType offset;
      offset.Fill(0);
      for (unsigned i = 0; i < TInputImage::ImageDimension; i++)
        {
        offset[i] =  m_Radius[i];
        }
      offset[k] = -internalRadius[k];
//    std::cout << "offset = " << offset << std::endl;
      N3.ActivateOffset(offset);
      }
    
    typedef typename itk::ImageRegionIteratorWithIndex<OutputImageType> OutputIteratorType;
    OutputIteratorType oIt(outputImage, *fit);
    
    N1.GoToBegin();
    N2.GoToBegin();
    N3.GoToBegin();
    
    typedef typename itk::NumericTraits<OutputPixelType>::RealType AccPixType;
    typename NInputIterator::ConstIterator sIt;
    if (fit == faceList.begin())
      {
      for (oIt.GoToBegin(); !oIt.IsAtEnd(); ++oIt, ++N1, ++N2, ++N3)
        {
        AccPixType Sum = 0;
        // these first two don't need loops
        for (sIt = N1.Begin(); !sIt.IsAtEnd();++sIt)
          {
          Sum += (AccPixType)sIt.Get();
          }
        for (sIt = N2.Begin(); !sIt.IsAtEnd();++sIt)
          {
          Sum += (AccPixType)Weight * (AccPixType)sIt.Get();
          }
        for (sIt = N3.Begin(); !sIt.IsAtEnd();++sIt)
          {
          Sum -= (AccPixType)sIt.Get();
          }
        oIt.Set(static_cast<OutputPixelType>((AccPixType)Sum/(AccPixType)pixelscount));
        progress.CompletedPixel();
        }
      }
    else
      {
      // need to compute pixelscount for each position
      for (oIt.GoToBegin(); !oIt.IsAtEnd(); ++oIt, ++N1, ++N2, ++N3)
        {
        AccPixType Sum = 0;
        PixelType v;
        bool inBound;
#if 0
        // these first two don't need loops
	for (sIt = N1.Begin(); !sIt.IsAtEnd();++sIt)
	  {
	  Sum += (AccPixType)sIt.Get();
	  std::cout << "A " << sIt.Get() << std::endl;
	  }
	for (sIt = N2.Begin(); !sIt.IsAtEnd();++sIt)
	  {
	  Sum += (AccPixType)Weight * (AccPixType)sIt.Get();
	  std::cout << "B " << sIt.Get() << std::endl;
	  }
	for (sIt = N3.Begin(); !sIt.IsAtEnd();++sIt)
          {
          Sum -= (AccPixType)sIt.Get();
	  std::cout << "C " << sIt.Get() << std::endl;
          }
#else

	// check the low corner is inbound
	bool LowCornerOut = false;
        for( typename NInputIterator::IndexListType::const_iterator idxIt = N2.GetActiveIndexList().begin();
          idxIt != N2.GetActiveIndexList().end();
          idxIt++ )
          {
          v = N2.GetPixel( *idxIt, inBound );
	  if (!inBound) 
	    { 
	    LowCornerOut = !inBound;
	    break;
	    }
          }

        for( typename NInputIterator::IndexListType::const_iterator idxIt = N1.GetActiveIndexList().begin();
          idxIt != N1.GetActiveIndexList().end();
          idxIt++ )
          {
          v = N1.GetPixel( *idxIt, inBound );
	  Sum += (AccPixType) v;
          }
	if (LowCornerOut) 
	  {
	  int CornerCount = 0;
	  
	  for( typename NInputIterator::IndexListType::const_iterator idxIt = N3.GetActiveIndexList().begin();
	       idxIt != N3.GetActiveIndexList().end();
	       idxIt++ )
	    {
	    v = N3.GetPixel( *idxIt, inBound );
	    if (inBound)
	      {
	      Sum -= (AccPixType) v;
	      }
	    else
	      {
	      ++CornerCount;
	      }
	    }
	  for( typename NInputIterator::IndexListType::const_iterator idxIt = N2.GetActiveIndexList().begin();
	       idxIt != N2.GetActiveIndexList().end();
	       idxIt++ )
	    {
	    v = N2.GetPixel( *idxIt, inBound );
	    Sum += (AccPixType)(Weight - CornerCount) * (AccPixType) v;
	    }
	  }
	else
	  {
	  for( typename NInputIterator::IndexListType::const_iterator idxIt = N3.GetActiveIndexList().begin();
	       idxIt != N3.GetActiveIndexList().end();
	       idxIt++ )
	    {
	    v = N3.GetPixel( *idxIt, inBound );
	    Sum -= (AccPixType) v;
	    }

	  for( typename NInputIterator::IndexListType::const_iterator idxIt = N2.GetActiveIndexList().begin();
	       idxIt != N2.GetActiveIndexList().end();
	       idxIt++ )
	    {
	    v = N2.GetPixel( *idxIt, inBound );
	    Sum += (AccPixType)Weight * (AccPixType) v;
	    }
	  }
#endif
        // count the number of pixels in the neighborhood which are still inside the image region
        // To do that, the region of the kernel at the current position is computed and cropped
        // by the image region.
        RegionType currentKernelRegion;
        currentKernelRegion.SetSize( kernelSize );
        // compute the region's index
        IndexType kernelRegionIdx = oIt.GetIndex();
        for( int i=0; i<ImageDimension; i++ )
          {
          kernelRegionIdx[i] -= m_Radius[i];
          }
        currentKernelRegion.SetIndex( kernelRegionIdx );
        currentKernelRegion.Crop( inputRegion );
        long edgepixelscount = currentKernelRegion.GetNumberOfPixels();
        //std::cout << oIt.GetIndex() << kernelRegionIdx << currentKernelRegion << edgepixelscount;
        oIt.Set(static_cast<OutputPixelType>((AccPixType)Sum/(AccPixType)edgepixelscount));
        progress.CompletedPixel();
        }
      
      }
    }

}



}// end namespace itk
#endif
