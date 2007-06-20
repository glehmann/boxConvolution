#ifndef __itkBoxMeanImageFilter_h
#define __itkBoxMeanImageFilter_h

#include "itkImageToImageFilter.h"

namespace itk {

/**
 * \class BoxMeanImageFilter
 * \brief Implements a fast rectangular mean filter using the
 * accumulator approach
 * \author Richard Beare
 */

template<class TInputImage, class TOutputImage=TInputImage>
class ITK_EXPORT BoxMeanImageFilter : 
    public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef BoxMeanImageFilter Self;
  typedef ImageToImageFilter<TInputImage, TOutputImage>  Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  
  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(BoxMeanImageFilter, 
               BoxMeanImageFilter);
  
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

    
  /** n-dimensional Kernel radius. in voxels */
  typedef typename TInputImage::SizeType RadiusType ;
  void SetRadius(unsigned R)
  {
    m_Radius.Fill(R);
  }
  


  itkSetMacro(Radius, RadiusType)
  itkGetConstReferenceMacro(Radius, RadiusType)

    

protected:
  RadiusType m_Radius;
  BoxMeanImageFilter();
  ~BoxMeanImageFilter() {};

  void GenerateInputRequestedRegion() ;
  
  /** Multi-thread version GenerateData. */
  void  ThreadedGenerateData (const OutputImageRegionType& 
                              outputRegionForThread,
                              int threadId) ;


private:
  BoxMeanImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBoxMeanImageFilter.txx"
#endif

#endif


