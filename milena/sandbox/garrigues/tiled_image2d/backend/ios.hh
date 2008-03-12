#ifndef ios_backend_hh
#define ios_backend_hh

#include <iostream>
#include <cassert>

template<typename Block>
struct ios_backend {
  typedef Block block_type;

  ios_backend(std::iostream& ios, unsigned n_blocks)
    : ios_(ios), size_(n_blocks) {
    assert(ios_.good());
    loaded_ = new Block*[n_blocks];
    for (Block **p = loaded_; p < loaded_ + n_blocks; ++p)
      *p = 0;
  }

  ~ios_backend() {
    for (unsigned n = 0; n < size_; ++n)
      if (loaded_[n]) {
	commit(n);
	delete loaded_[n];
      }
    delete[] loaded_;
  }

  unsigned size() { return size_; }


  Block& get(unsigned n) {
    assert(n < size_);
    assert(loaded_[n]);
    return *loaded_[n];
  }

  const Block& get(unsigned n) const {
    assert(n < size_);
    assert(loaded_[n]);
    return *loaded_[n];
  }

  void commit() {
    for (unsigned n = 0; n < size_; ++n)
      if(loaded_[n])
	commit(n);
  }
  
  void commit(unsigned n) {
    assert(n < size_);
    Block *b = loaded_[n];
    ios_.seekp(n * Block::nbytes);
    ios_.write(b->bytes, Block::nbytes);
    ios_.flush();
  }

  void forget(unsigned n) {
    assert(n < size_);
    Block *b = loaded_[n];
    commit(n);
    loaded_[n] = 0;
    delete b;
  }
  
  void load(unsigned n) {
    assert(n < size_);
    Block *b = loaded_[n];
    if (!b) b = new Block;
    ios_.seekp(n * Block::nbytes);
    ios_.read(b->bytes, Block::nbytes);
    loaded_[n] = b;
  }
  
  Block** loaded_;
  std::iostream &ios_;
  unsigned size_;
};

#endif
