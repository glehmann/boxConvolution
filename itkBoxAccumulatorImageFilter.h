#ifndef __itkBoxAccumulatorImageFilter_h
#define __itkBoxAccumulatorImageFilter_h

#include "itkImageToImageFilter.h"

namespace itk {

/**
 * \class BoxAccumulatorImageFilter
 * \brief Accumulates the sum of voxels "prior" and to the "left"
 *
 * This filter provides an accumulated image of a form that can be
 * used to rapidly compute various kinds of box convolution - means,
 * variances etc. This one isn't multi-threaded - it will get used by
 * other filters that will deal with that part. Obviously care needs
 * to be taken selecting the output image type
 *
 * \author Richard Beare
 */

template<class TInputImage, class TOutputImage>
class ITK_EXPORT BoxAccumulatorImageFilter : 
    public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
    /** Standard class typedefs. */
  typedef BoxAccumulatorImageFilter Self;
  typedef ImageToImageFilter<TInputImage,TOutputImage>  Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  
  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(BoxAccumulatorImageFilter,
               ImageToImageFilter);
 
  /** Image related typedefs. */
  typedef TInputImage InputImageType;
  typedef typename TInputImage::RegionType RegionType ;
  typedef typename TInputImage::SizeType SizeType ;
  typedef typename TInputImage::IndexType IndexType ;
  typedef typename TInputImage::PixelType PixelType ;
  typedef typename TInputImage::OffsetType OffsetType ;
  
  typedef TOutputImage OutputImageType;
  typedef typename TOutputImage::PixelType OutputPixelType ;
  typedef typename TInputImage::PixelType InputPixelType ;

  /** Image related typedefs. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  /** n-dimensional Kernel radius. */
  typedef typename TInputImage::SizeType RadiusType ;

  void GenerateData();

protected:
  BoxAccumulatorImageFilter();
  ~BoxAccumulatorImageFilter() {};

  void PrintSelf(std::ostream& os, Indent indent) const;

private:
  BoxAccumulatorImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  RadiusType m_Radius;

};

} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBoxAccumulatorImageFilter.txx"
#endif


#endif
