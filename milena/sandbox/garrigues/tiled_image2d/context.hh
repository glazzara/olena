#ifndef context_hh
#define context_hh

#include "ersatz/domain.hh"
#include "page.hh"
#include "paged_image.hh"

template<typename T, typename Layout,
	 template<typename Block> class Backend,
	 template<typename Backend> class Support,
	 typename Domain = domain<Layout::dim, unsigned> >
struct context {

  typedef typename Layout::template block_<T> block_type;

  typedef T value_type;
  
  enum { dim = Layout::dim };

  typedef Backend<block_type> backend_type;
  
  typedef Support<backend_type> support_type;

  typedef Layout layout_type;
  typedef typename layout_type::page_layout page_layout;
  typedef typename layout_type::image_layout image_layout;

  typedef page<T, page_layout> page_type;

  typedef Domain domain_type;
  
  typedef paged_image<value_type, domain_type, layout_type, support_type> image_type;
  
};
	 


#endif
