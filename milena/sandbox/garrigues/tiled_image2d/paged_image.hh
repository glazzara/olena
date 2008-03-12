#ifndef paged_image_hh
#define paged_image_hh

#include "page.hh"

template<typename T, typename Domain, typename Layout, typename Support>
struct paged_image {
  typedef Domain domain_type;
  typedef Support support_type;
  typedef Layout layout_type;
  
  typedef page<T, typename Layout::page_layout> page_type;

  paged_image(const Domain& d, Support& s)
    : domain_(d), support_(s)
  {}

  
  const domain_type& domain() const { return domain_; }
  
  template<typename Point>
  page_type page_at(const Point& p) {
    unsigned page_n = Layout::image_layout::page_at(*this, p);
    return page_type(support_.get_block(page_n));
  }

  template<typename Point>
  void prepare(const Point& p) {
    unsigned page_n = Layout::image_layout::page_at(*this, p);
    support_.prepare(page_n);
  }
  
  template<typename Point>
  T& operator()(const Point& p) {
    unsigned page_n = Layout::image_layout::page_at(*this, p);
    // note: although the page instance goes
    // out of scope, the reference stays valid
    // because the block is ultimately held by support.
    return page_type(support_.get_block(page_n))(Layout::image_layout::changeref(p));
  }

  template<typename Point>
  const T& operator()(const Point& p) const {
    unsigned page_n = Layout::page_at(*this, p);
    // note: although the page instance goes
    // out of scope, the reference stays valid
    // because the block is ultimately held by support.
    return page_type(const_cast<support_type&>(support_).get_block(page_n))(Layout::image_layout::changeref(p));
  }


  domain_type domain_;
  support_type& support_;
};

#endif
