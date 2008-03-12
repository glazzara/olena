#ifndef layout_image2d_tblr_hh
#define layout_image2d_tblr_hh

#include "ersatz/point.hh"

namespace layout {

template<unsigned W, unsigned H>
struct image2d_tblr {
  enum { dim = 2, page_width = W, page_height = H };

  template<typename Image>
  static unsigned size(const Image& im) {
    const typename Image::domain_type& d = im.domain();
    return (d.len(0) / page_width ) * (d.len(1) / page_height);
  }

  template<typename Image, typename C>
  static unsigned page_at(const Image& im, const point2d_<C>& p)
  {
    return (p[0] / page_width) * (im.domain().len(1) / page_height) + p[1] / page_height;
  };

  template<typename C>
  static point2d_<C> changeref(const point2d_<C>& p)
  {
    return point2d_<C(p[0] % page_width, p[1] % page_height);
  }
};

}

#endif
