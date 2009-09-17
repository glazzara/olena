// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_CORE_IMAGE_CACHE_HH
# define MLN_CORE_IMAGE_CACHE_HH

/// \file
/// Definition of the basic mln::cache class.

#include <mln/util/array.hh>

#include <mln/core/image/page.hh>


namespace mln
{

  template <typename D, typename V>
  class cache
  {
    public:

      // Constructor
      cache(D domain, std::streampos pos, unsigned ncols, std::fstream* f);
      ~cache();

      void set_number_pages(unsigned number_pages);
      V read(mln_psite(D) p);
      void write(const mln_psite(D)& p, const V& value);
      unsigned insert_new_page(const mln_psite(D)& p);

    protected:

      util::array<page<D, V>* > pages_;
      util::array<unsigned> order_;
      unsigned count_;
      unsigned number_pages_;

      // Image info.
      D domain_;
      std::streampos pos_;
      unsigned ncols_;
      std::fstream* f_;
  };


# ifndef MLN_INCLUDE_ONLY


  template <typename D, typename V>
  inline
  cache<D, V>::cache(D domain, std::streampos pos, unsigned ncols, std::fstream* f)
  {
    this->number_pages_ = 5; // Default value.
    this->count_ = 1;

    this->domain_ = domain;
    this->pos_ = pos;
    this->ncols_ = ncols;
    this->f_ = f;
  }

  template <typename D, typename V>
  inline
  cache<D, V>::~cache()
  {
    for (unsigned i = 0; i < this->pages_.nelements(); ++i)
      delete this->pages_[i];
    this->pages_.clear();
    this->order_.clear();
  }

  template <typename D, typename V>
  inline
  void
  cache<D, V>::set_number_pages(unsigned number_pages)
  {
    if (number_pages == 0)
    {
      // FIXME: We must not allow that.
    }
    if (number_pages < this->number_pages_)
    {
      // FIXME: Prune pages array.
    }
    this->number_pages_ = number_pages;
  }

  template <typename D, typename V>
  inline
  V
  cache<D, V>::read(mln_psite(D) p)
  {
    for (unsigned i = 0; i < this->pages_.nelements(); ++i)
    {
      if (this->pages_[i]->has(p))
	return this->pages_[i]->read(p);
    }

    // In case we have not found a valid page, we insert a new one.
    unsigned new_page_index = insert_new_page(p);
    return this->pages_[new_page_index]->read(p);
  }

  template <typename D, typename V>
  inline
  void
  cache<D, V>::write(const mln_psite(D)& p, const V& value)
  {
    bool found = false;
    for (unsigned i = 0; !found && i < this->pages_.nelements(); ++i)
    {
      if (this->pages_[i]->has(p))
      {
	this->pages_[i]->write(p, value);
	found = true;
      }
    }

    // In case we have not found a valid page, we insert a new one.
    if (!found)
    {
      unsigned new_page_index = insert_new_page(p);
      this->pages_[new_page_index]->write(p, value);
    }
  }

  // Basic LRU cache algorithm implementation.
  template <typename D, typename V>
  inline
  unsigned
  cache<D, V>::insert_new_page(const mln_psite(D)& p)
  {
    page<D, V>* new_page = new page<D, V>(this->domain_,
					  p,
					  this->pos_,
					  this->ncols_,
					  this->f_);

    unsigned new_index;

    // If there is still room for a new page.
    if (this->pages_.nelements() < this->number_pages_)
    {
      this->pages_.append(new_page);
      this->order_.append(this->count_++);
      new_index = this->pages_.nelements() - 1;
    }
    else // We must find which page to discard.
    {
      unsigned min = this->order_[0];
      new_index = 0;
      for (unsigned i = 1; i < this->pages_.nelements(); ++i)
      {
	if (this->order_[i] < min)
	{
	  min = this->order_[i];
	  new_index = i;
	}
      }
      delete this->pages_[new_index];
      this->pages_[new_index] = new_page;
      for (unsigned i = 0; i < this->order_.nelements(); ++i)
	this->order_[i] -= min;
      this->count_ -= min;
      this->order_[new_index] = this->count_++;
    }

    return new_index;
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_CACHE_HH
