#ifndef file_backend_hh
#define file_backend_hh

#include <unistd.h>
#include <cassert>

template<typename Block>
struct file_backend {
  typedef Block block_type;

  file_backend(int fd, unsigned n_blocks)
    : fd_(fd), size_(n_blocks) {
    loaded_ = new Block*[n_blocks];
    for (Block **p = loaded_; p < loaded_ + n_blocks; ++p)
      *p = 0;
  }

  ~file_backend() {
    for (unsigned n = 0; n < size_; ++n)
      if (loaded_[n]) {
	commit(n);
	delete loaded_[n];
      }
    delete[] loaded_;
  }

  unsigned size() const { return size_; }

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
    lseek(fd_, n * Block::nbytes, SEEK_SET);
    write(fd_, b->bytes, Block::nbytes);
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
    lseek(fd_, n * Block::nbytes, SEEK_SET);
    read(fd_, b->bytes, Block::nbytes);
    loaded_[n] = b;
  }
  
  Block** loaded_;
  int fd_;
  unsigned size_;
};

#endif
