// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef mmap_backend_hh
#define mmap_backend_hh

#include <sys/mman.h>
#include <unistd.h>
#include <cassert>
#include <cstdio>


namespace mln
{

  template<typename Block>
  struct mmap_backend : public Object< mmap_backend<Block> >{
    typedef Block block_type;

    mmap_backend(int fd, unsigned n_blocks)
      : fd_(fd), size_(n_blocks), n_loaded_(0) {
      std::cout << "size: " << size_ << "blocks." << std::endl;
      assert(Block::nbytes % getpagesize() == 0);
      if (fd == -1) { perror("open"); exit(1); }
      assert(fd_ != -1);

#ifndef NDEBUG
      // FIXME: should be performed as static check
      Block *bl = 0;
      assert((void*)bl == (void*)bl->bytes);
#endif

      loaded_ = new Block*[n_blocks];
      for (Block **p = loaded_; p < loaded_ + n_blocks; ++p)
	*p = 0;
    }

    ~mmap_backend() {
      for (unsigned n = 0; n < size_; ++n)
	if (loaded_[n])
	  munmap(loaded_[n]->bytes, Block::nbytes);
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
      if (msync(b->bytes, Block::nbytes, MS_SYNC) == -1)
	perror("msync");
    }

    void forget(unsigned n) {
      assert(n < size_);
      Block *b = loaded_[n];
      loaded_[n] = 0;
      n_loaded_--;
      if (munmap(b->bytes, Block::nbytes) == -1)
	perror("munmap");
    }

    void load(unsigned n) {
      assert(n < size_);
      Block *b = loaded_[n];
      if (!b) {
	n_loaded_++;
	b = (Block*) mmap(0, Block::nbytes, PROT_READ|PROT_WRITE, MAP_FILE|MAP_SHARED, fd_, n * Block::nbytes);
	if (b == (void*)-1)
	  perror("mmap");
	assert(b != 0);
	loaded_[n] = b;

      }
    }

    Block** loaded_;
    int fd_;
    unsigned size_;
    int n_loaded_;
  };


} // end of namespace mln

#endif
