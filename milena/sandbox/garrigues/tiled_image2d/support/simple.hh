#ifndef simple_support_hh
#define simple_support_hh

/*
Simple support: only one page in memory at a time.
 */
template<typename Backend>
struct simple_support {

  typedef typename Backend::block_type block_type;

  simple_support(Backend& b)
    : backend_(b), current_(0)
  {
    backend_.load(0);
  }

  void prepare(unsigned n) {
    // no-op here
  }

  block_type& get_block(unsigned n) {
    if (n != current_) {
      backend_.forget(current_);
      backend_.load(n);
      current_ = n;
    }
    return backend_.get(n);
  }

  Backend& backend_;
  unsigned current_;
};

#endif
