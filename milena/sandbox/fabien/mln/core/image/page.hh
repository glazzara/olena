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

#ifndef MLN_CORE_IMAGE_PAGE_HH
# define MLN_CORE_IMAGE_PAGE_HH

/// \file
/// Definition of the basic mln::page class.

# include <fstream>

# include <mln/util/array.hh>


namespace mln
{

  template <typename D, typename V>
  class page
  {
    public:

      // Constructor & destructor.
      page(D domain, mln_psite(D) p, std::streampos pos, unsigned ncols, std::fstream* f);
      ~page();

      bool has(mln_psite(D) p);
      V read(mln_psite(D) p);
      void write(const mln_psite(D)& p, const V& value);
      void flush();

      // Debug.
      void print_buffer();

    protected:

      D domain_;
      D range_;
      V* buffer_;
      std::fstream* f_;
      std::streampos offset_;
      unsigned buffer_size_;
  };


# ifndef MLN_INCLUDE_ONLY


  template <typename D, typename V>
  inline
  page<D, V>::page(D domain, mln_psite(D) p, std::streampos pos, unsigned ncols, std::fstream* f)
  {
    // Defining layout for a line.
    this->range_.pmin() = p;
    this->range_.pmin().col() = domain.pmin().col();
    this->range_.pmax() = p;
    this->range_.pmax().col() = domain.pmax().col();

    this->buffer_ = new V[ncols];

    this->domain_ = domain;
    this->f_ = f;
    this->buffer_size_ = ncols;

    this->offset_ = pos;
    this->offset_ += (ncols * p.row()) * sizeof(V);
    f->seekg(this->offset_);
    f->get((char*)(buffer_), buffer_size_ * sizeof(V));
  }

  template <typename D, typename V>
  inline
  page<D, V>::~page()
  {
    this->flush();
    delete this->buffer_;
  }

  template <typename D, typename V>
  inline
  bool
  page<D, V>::has(mln_psite(D) p)
  {
    return p.col() >= this->range_.pmin().col() &&
	   p.col() <= this->range_.pmax().col() &&
	   p.row() >= this->range_.pmin().row() &&
	   p.row() <= this->range_.pmax().row();
  }

  template <typename D, typename V>
  inline
  V
  page<D, V>::read(mln_psite(D) p)
  {
    return buffer_[p.col() - this->range_.pmin().col()];
  }

  template <typename D, typename V>
  inline
  void
  page<D, V>::write(const mln_psite(D)& p, const V& value)
  {
    buffer_[p.col() - this->range_.pmin().col()] = value;
  }

  template <typename D, typename V>
  inline
  void
  page<D, V>::flush()
  {
    this->f_->seekg(this->offset_);
    this->f_->write((char*)(buffer_), buffer_size_ * sizeof(V));
  }


  // Debug.

  template <typename D, typename V>
  inline
  void
  page<D, V>::print_buffer()
  {
    for (unsigned i = 0; i < this->buffer_size_; ++i)
      std::cout << "buffer[" << i << "] = " << this->buffer_[i] << std::endl;
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_PAGE_HH
