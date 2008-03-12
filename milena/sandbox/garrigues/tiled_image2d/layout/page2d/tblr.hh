#ifndef layout_page2d_tblr_hh
#define layout_page2d_tblr_hh

#include "ersatz/point.hh"

namespace layout {

template<unsigned W, unsigned H>
struct page2d_tblr {
  enum { dim = 2, width = W, height = H, nitems = W * H };

  template<typename C>
  static unsigned pixel_at(const point2d_<C>& p)
  {
    return p[0] * height + p[1];
  }
};  

}

#endif
