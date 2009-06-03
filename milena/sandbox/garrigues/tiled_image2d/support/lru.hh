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

#ifndef lru_support_hh
#define lru_support_hh

namespace mln
{

  template<typename Block>
  struct item {
    item(unsigned n, item<Block>* first)
      : block_number(n), prev(0), next(first)
    {
      if (first)
	first->prev = this;
    }


    unsigned block_number;
    item<Block> *prev;
    item<Block> *next;
  };

#ifdef LRU_DEBUG
#include <iostream>

  template<typename Item>
  struct inspect_ {
    inspect_(const Item *first, const Item *last) : first_(first), last_(last) {}
    const Item * first_;
    const Item * last_;
  };
  template<typename Item>
  std::ostream& operator<<(std::ostream& o, const inspect_<Item>& i) {
    const Item *p = i.first_;
    const Item *prev = 0;
    while (p) {
      assert(p->prev == prev);
      o << p << '(' << p->prev << ',' << p->next << ':' << p->block_number << ')' << ' ';
      prev = p;
      p = p->next;
    }
    o << i.last_;
    return o;
  }
  template<typename Item>
  inspect_<Item> inspect(const Item *first, const Item* last) { return inspect_<Item>(first, last); }
#endif

  /*
    LRU: least recently used gets evicted first
  */
  template<typename Backend>
  struct lru_support : public Object< lru_support<Backend> > {

    typedef typename Backend::block_type block_type;
    typedef item<block_type> node_type;

    lru_support(Backend& b, unsigned buffer_size = 5)
      : backend_(b), maxitems_(buffer_size), counter_(0), first_(0), last_(0)
    {
      assert(buffer_size > 1);
      unsigned sz = b.size();
      map_ = new node_type* [sz];
      for (node_type **p = map_; p < map_ + sz; ++p)
	*p = 0;
    }

    ~lru_support() {
      delete[] map_;
    }

    void hit(node_type* node, unsigned n) {
      assert(node->block_number == n);
      if (first_ != node) {
#ifdef LRU_DEBUG
	std::cerr << "lru: pull " << n << '<' << node << '>' << ' ' << inspect(first_,last_) << std::endl;
#endif
	// move to head of queue

	if (node->next) // is last?
	  // no: patch up next node
	  node->next->prev = node->prev;
	else
	  // yes
	  last_ = node->prev;

	// patch up previous node
	node->prev->next = node->next;

	// insert in front
	first_->prev = node;
	node->next = first_;
	node->prev = 0;
	first_ = node;

#ifdef LRU_DEBUG
	std::cerr << "lru: end pull " << n << ' ' << inspect(first_,last_) << std::endl;
#endif
      }

    }

    void evict_last() {
      node_type *prev = last_->prev;

#ifdef LRU_DEBUG
      std::cerr << "lru: evict " << last_->block_number << ' ' << inspect(first_, last_)  << std::endl;
#endif
      backend_.forget(last_->block_number);

      map_[last_->block_number] = 0;
      delete last_;

      prev->next = 0;
      last_ = prev;
      --counter_;
    }

    node_type* miss(unsigned n) {
#ifdef LRU_DEBUG
      std::cerr << "lru: miss " << n << ' ' << inspect(first_, last_)  << std::endl;
#endif

      if (counter_ >= maxitems_)
	evict_last();

#ifdef LRU_DEBUG
      std::cerr << "lru: load " << n << ' ' << inspect(first_, last_)  << std::endl;
#endif

      backend_.load(n);

      node_type *node = new node_type(n, first_);
      map_[n] = node;
      first_ = node;

      if (!last_)
	// first item ever, becomes new last
	last_ = node;

#ifdef LRU_DEBUG
      std::cerr << "lru: end miss " << n << ' ' << inspect(first_, last_)  << std::endl;
#endif

      ++counter_;
      return node;
    }

    block_type& get_block(unsigned n) {
      prepare(n);
      return backend_.get(n);
    }

    void prepare(unsigned n) {
      if (node_type *node = map_[n])
	hit(node, n);
      else
	miss(n);
    }

    Backend& backend_;
    unsigned maxitems_;
    unsigned counter_;
    node_type **map_;
    node_type *first_;
    node_type *last_;
  };


} // end of namespace mln

#endif
