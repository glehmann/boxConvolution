#ifndef __itkBoxMeanImageFilter_h
#define __itkBoxMeanImageFilter_h

#include "itkBoxImageFilter.h"

namespace itk {

/**
 * \class BoxMeanImageFilter
 * \brief Implements a fast rectangular mean filter using the
 * accumulator approach
 * \author Richard Beare
 */

template<class TInputImage, class TOutputImage=TInputImage>
class ITK_EXPORT BoxMeanImageFilter : 
    public BoxImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef BoxMeanImageFilter Self;
  typedef BoxImageFilter<TInputImage, TOutputImage>  Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  
  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(BoxMeanImageFilter, 
               BoxImageFilter);
  
  /** Image related typedefs. */
  typedef TInputImage InputImageType;
  typedef TOutputImage OutputImageType;
  typedef typename TInputImage::RegionType RegionType ;
  typedef typename TInputImage::SizeType SizeType ;
  typedef typename TInputImage::IndexType IndexType ;
  typedef typename TInputImage::PixelType PixelType ;
  typedef typename TInputImage::OffsetType OffsetType ;
  typedef typename Superclass::OutputImageRegionType OutputImageRegionType;
  typedef typename TOutputImage::PixelType OutputPixelType ;

  /** Image related typedefs. */
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);


#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(SameDimension,
                  (Concept::SameDimension<itkGetStaticConstMacro(InputImageDimension),itkGetStaticConstMacro(OutputImageDimension)>));

  
  /** End concept checking */
#endif

    
protected:
  BoxMeanImageFilter();
  ~BoxMeanImageFilter() {};

#if 1
  /** Multi-thread version GenerateData. */
  void  ThreadedGenerateData (const OutputImageRegionType& 
			      outputRegionForThread,
			      int threadId);
#else
  void GenerateData();
#endif


private:
  BoxMeanImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBoxMeanImageFilter.txx"
#endif

#endif


