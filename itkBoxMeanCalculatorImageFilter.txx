#ifndef __itkBoxMeanCalculatorImageFilter_txx
#define __itkBoxMeanCalculatorImageFilter_txx

#include "itkImage.h"
#include "itkBoxMeanCalculatorImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
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
    typedef typename itk::NumericTraits<OutputPixelType>::RealType AccPixType;
    if (fit == faceList.begin())
      {
      // this is the body region. This is meant to be an optimized
      // version that doesn't use neigborhood regions
      // compute the various offsets
      AccPixType pixelscount = 1;
      AccPixType Weight = (AccPixType)(TInputImage::ImageDimension - 1);
      typedef typename itk::ImageRegionIterator<OutputImageType> OutputIteratorType;
      typedef typename itk::ImageRegionConstIterator<InputImageType> InputIteratorType;
      typename OutputIteratorType::OffsetType LowCorner, HighCorner;
      for (unsigned i = 0; i < TInputImage::ImageDimension; i++)
	{
	LowCorner[i] = m_Radius[i];
	HighCorner[i] = -internalRadius[i];
	pixelscount *= (AccPixType)(2*m_Radius[i] + 1);
	}
      // In higer dimensions there will be more intermediate corners
      typedef std::vector<InputIteratorType> CornerItType;
      CornerItType OtherCorners;
      for (unsigned k = 0; k < TInputImage::ImageDimension; k++)
	{
	typename InputIteratorType::OffsetType offset;
	offset.Fill(0);
	for (unsigned i = 0; i < TInputImage::ImageDimension; i++)
	  {
	  offset[i] =  m_Radius[i];
	  }
	offset[k] = -internalRadius[k];
	typename InputImageType::RegionType tReg=(*fit);
	tReg.SetIndex(tReg.GetIndex() + offset);
	InputIteratorType tempIt(accImage, tReg);
	tempIt.GoToBegin();
	OtherCorners.push_back(tempIt);
	}
      // set up regions of the same size but offset
      typename InputImageType::RegionType lReg, hReg;
      lReg=(*fit);
      hReg=(*fit);
      lReg.SetIndex(lReg.GetIndex() + LowCorner);
      hReg.SetIndex(hReg.GetIndex() + HighCorner);
      OutputIteratorType oIt(outputImage, *fit);
      InputIteratorType lowIt(accImage, lReg);
      InputIteratorType highIt(accImage, hReg);
      lowIt.GoToBegin();
      highIt.GoToBegin();
      
      for (oIt.GoToBegin(); !oIt.IsAtEnd(); ++oIt, ++lowIt, ++highIt) //, ++N1, ++N2, ++N3)
        {
        AccPixType Sum = (AccPixType)highIt.Get() + Weight * (AccPixType)lowIt.Get();

	for (unsigned k = 0; k < TInputImage::ImageDimension; k++)
	  {
	  Sum -= (AccPixType)OtherCorners[k].Get();
	  ++OtherCorners[k];
	  }

        oIt.Set(static_cast<OutputPixelType>(Sum/pixelscount));
        progress.CompletedPixel();
        }
      }
    else
      {
      // use neighborhood iterators for the border regions to take
      // advantage of boundary checks. Incrementing the neighborhood
      // iterators seems to be a major time cost for the body region
      // need to compute pixelscount for each position
      typedef typename itk::ShapedNeighborhoodIterator<InputImageType> NInputIterator;
      typename NInputIterator::ConstIterator sIt;
      NInputIterator N1(internalRadius, accImage, *fit);
      NInputIterator N2(internalRadius, accImage, *fit);
      NInputIterator N3(internalRadius, accImage, *fit);
      N1.OverrideBoundaryCondition(&nbc);
      N2.OverrideBoundaryCondition(&nbc);
      N3.OverrideBoundaryCondition(&nbc);
      
      int Weight = TInputImage::ImageDimension - 1;
      // set the correct corners
      typename NInputIterator::OffsetType offset1, offset2;
      for (unsigned i = 0; i < TInputImage::ImageDimension; i++)
	{
	offset1[i] = m_Radius[i];
	offset2[i] = -internalRadius[i];
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
	N3.ActivateOffset(offset);
	}
    
    
      N1.GoToBegin();
      N2.GoToBegin();
      N3.GoToBegin();
      
      typedef typename itk::ImageRegionIteratorWithIndex<OutputImageType> OutputIteratorType;
      OutputIteratorType oIt(outputImage, *fit);
      for (oIt.GoToBegin(); !oIt.IsAtEnd(); ++oIt, ++N1, ++N2, ++N3)
        {
        AccPixType Sum = 0;
        PixelType v;
        bool inBound;

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
        oIt.Set(static_cast<OutputPixelType>(Sum/(AccPixType)edgepixelscount));
        progress.CompletedPixel();
        }
      
      }
    }

}



}// end namespace itk
#endif
