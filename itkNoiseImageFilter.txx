/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNoiseImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2006/01/11 19:43:32 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkNoiseImageFilter_txx
#define _itkNoiseImageFilter_txx
#include "itkNoiseImageFilter.h"

#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkOffset.h"
#include "itkProgressReporter.h"

namespace itk
{

template <class TInputImage, class TOutputImage>
NoiseImageFilter<TInputImage, TOutputImage>
::NoiseImageFilter()
{
}


template< class TInputImage, class TOutputImage>
void
NoiseImageFilter< TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       int threadId)
{
  unsigned int i;
  ZeroFluxNeumannBoundaryCondition<InputImageType> nbc;

  ConstNeighborhoodIterator<InputImageType> bit;
  ImageRegionIterator<OutputImageType> it;
  
  // Allocate output
  typename OutputImageType::Pointer output = this->GetOutput();
  typename  InputImageType::ConstPointer input  = this->GetInput();
  
  // Find the data-set boundary "faces"
  typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType faceList;
  NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType> bC;
  faceList = bC(input, outputRegionForThread, this->GetRadius());

  typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType::iterator fit;

  // support progress methods/callbacks
  ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
  
  InputRealType value;
  InputRealType sum;
  InputRealType sumOfSquares;
  InputRealType var;
  InputRealType num;

  // Process each of the boundary faces.  These are N-d regions which border
  // the edge of the buffer.
  for (fit=faceList.begin(); fit != faceList.end(); ++fit)
    { 
    bit = ConstNeighborhoodIterator<InputImageType>(this->GetRadius(),
                                                    input, *fit);
    unsigned int neighborhoodSize = bit.Size();
    num = static_cast<InputRealType>( bit.Size() );
      
    it = ImageRegionIterator<OutputImageType>(output, *fit);
    bit.OverrideBoundaryCondition(&nbc);
    bit.GoToBegin();

    while ( ! bit.IsAtEnd() )
      {
      sum = NumericTraits<InputRealType>::Zero;
      sumOfSquares = NumericTraits<InputRealType>::Zero;
      for (i = 0; i < neighborhoodSize; ++i)
        {
        value = static_cast<InputRealType>( bit.GetPixel(i) );
        sum += value;
        sumOfSquares += (value*value);
        }
      
      // calculate the standard deviation value
      var = (sumOfSquares - (sum*sum/num)) / (num - 1.0);
      it.Set( static_cast<OutputPixelType>(sqrt(var)) );
      
      ++bit;
      ++it;
      progress.CompletedPixel();
      }
    }
}

} // end namespace itk

#endif
