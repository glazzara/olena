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

    protected:

      util::array<page<D, V>* > pages_;
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
    this->number_pages_ = 1;

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
  }

  template <typename D, typename V>
  inline
  void
  cache<D, V>::set_number_pages(unsigned number_pages)
  {
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

    // If we have not found a valid page, we just add one.
    // FIXME: We should test that we have room for a new page.
    page<D, V>* new_page = new page<D, V>(this->domain_,
					  p,
					  this->pos_,
					  this->ncols_,
					  this->f_);
    this->pages_.append(new_page);
    return new_page->read(p);
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

    if (!found)
    {
      // If we have not found a valid page, we just add one.
      // FIXME: We should test that we have room for a new page.
      page<D, V>* new_page = new page<D, V>(this->domain_,
	  p,
	  this->pos_,
	  this->ncols_,
	  this->f_);
      this->pages_.append(new_page);
      new_page->write(p, value);
    }
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_CACHE_HH
