// Copyright (C) 2001, 2003, 2004, 2006 EPITA Research and Development
// Laboratory
//
// This file is part of the Olena Library.  This library is free
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

#ifndef OLENA_CORE_2D_ARRAY2D_HH
# define OLENA_CORE_2D_ARRAY2D_HH

# include <cstdlib>
# include <set>
# include <mlc/contract.hh>


namespace oln
{

  template <typename T>
  struct tracked_ptr
  {
    typedef tracked_ptr<T> self_t;
    typedef std::set<self_t*> holders_t;

    T* ptr_;
    holders_t* holders_;

    /// Coercion towards Boolean (for arithmetical tests).
    operator bool() const
    {
      invariant_();
      return ptr_ != 0;
    }

    /// Negation (for arithmetical tests).
    bool operator not() const
    {
      invariant_();
      return not bool(*this);
    }

    /*! \brief Mimics the behavior of op-> for a pointer in the const case.
    **
    ** \invariant Pointer proxy exists.
    */
    const T*const operator->() const
    {
      invariant_();
      precondition(ptr_ != 0);
      return ptr_;
    }

    /*! \brief Mimics the behavior of op-> for a pointer in the mutable case.
    **
    ** \invariant Pointer proxy exists.
    */
    T*const operator->()
    {
      invariant_();
      precondition(ptr_ != 0);
      return ptr_;
    }

    /// Ctor.
    tracked_ptr() :
      ptr_(0),
      holders_(0)
    {
      invariant_();
    }
   
    /// Ctor.
    tracked_ptr(T* ptr) :
      ptr_(ptr)
    {
      if (ptr == 0)
	holders_ = 0;
      else
	{
	  holders_ = new holders_t;
	  holders_->insert(this);
	}
      invariant_();
    }

    /// Cpy ctor.
    tracked_ptr(const self_t& rhs) :
      ptr_(rhs.ptr_),
      holders_(rhs.holders_)
    {
      rhs.invariant_();
      if (ptr_ != 0)
	holders_->insert(this);
      invariant_();
    }

    /// Assignment.
    self_t& operator=(const self_t& rhs)
    {
      invariant_();
      rhs.invariant_();
      if (&rhs == this or rhs.ptr_ == ptr_)
	// no-op
	return *this;
      clean_();
      ptr_ = rhs.ptr_;
      holders_ = rhs.holders_;
      holders_->insert(this);
      return *this;
    }

    /// Assignment.
    self_t& operator=(T* ptr)
    {
      invariant_();
      if (ptr == ptr_)
	// no-op
	return *this;
      clean_();
      ptr_ = ptr;
      if (ptr == 0)
	holders_ = 0;
      else
	{
	  holders_ = new holders_t;
	  holders_->insert(this);
	}
      return *this;
    }

    /// Dtor.
    ~tracked_ptr()
    {
      clean_();
    }

    void invariant_() const
    {
      invariant((ptr_ and holders_) or (not ptr_ and not holders_));
      if (ptr_ == 0)
	return;
      invariant(holders_->size() > 0);
      self_t* this_ = const_cast<self_t*>(this);
      invariant(holders_->find(this_) != holders_->end());
      typename holders_t::const_iterator i;
      for (i = holders_->begin(); i != holders_->end(); ++i)
	invariant((*i)->ptr_ == ptr_);
    }

    void clean_()
    {
      invariant_();
      if (ptr_ == 0)
	// no-op
	return;
      if (holders_->size() == 1)
	{
	  delete ptr_;
	  delete holders_;
	}
      else
	holders_->erase(this);
      ptr_ = 0;
      holders_ = 0;
      invariant_();
    }

    friend std::ostream& operator<<(std::ostream& ostr, const self_t& tp)
    {
      ostr << "tracked_ptr @ " << (&tp)
	   << " { ptr = " << tp.ptr_
	   << " / holders = ";
      if (tp.holders_ == 0)
	ostr << "0";
      else
	{
	  typename holders_t::const_iterator i;
	  for (i = tp.holders_->begin(); i != tp.holders_->end(); ++i)
	    ostr << (*i) << ' ';
	}
      ostr << " }";
      return ostr;
    }
  };


  /// General 2D array class.
  template <typename value_t, typename coord_t = int>
  class array2d
  {
  public:

    /// Ctor.
    array2d(coord_t imin, coord_t jmin,
	    coord_t imax, coord_t jmax) :
      imin_(imin),
      jmin_(jmin),
      imax_(imax),
      jmax_(jmax)
    {
      precondition(imax >= imin and jmax >= jmin);
      ilen_ = imax - imin + 1;
      jlen_ = jmax - jmin + 1;
      allocate_();
    }

    /// Ctor.
    array2d(coord_t ilen, coord_t jlen) :
      imin_(0),
      jmin_(0),
      ilen_(ilen),
      jlen_(jlen)
    {
      precondition(ilen > 0 and jlen > 0);
      imax_ = imin_ + ilen_;
      jmax_ = jmin_ + ilen_;
      allocate_();
    }

    /// Dtor.
    ~array2d()
    {
      deallocate_();
    }

    value_t operator()(coord_t i, coord_t j) const
    {
      precondition(has(i, j));
      return array_[i][j];
    }

    value_t& operator()(coord_t i, coord_t j)
    {
      precondition(has(i, j));
      return array_[i][j];
    }

    bool has(coord_t i, coord_t j) const
    {
      return
	i >= imin_ and i <= imax_ and
	j >= jmin_ and j <= jmax_;
    }

    size_t memsize() const
    {
      return
	// buffer_
	size_t(ilen_) * size_t(jlen_) * sizeof(value_t)
	+
	// array_
	size_t(ilen_) * sizeof(value_t*);
    }

  protected:

    coord_t   imin_, jmin_, imax_, jmax_;
    coord_t   ilen_, jlen_;
    value_t*  buffer_;
    value_t** array_;

  private:

    void allocate_()
    {
      buffer_ = new value_t[size_t(ilen_) * size_t(jlen_)];
      array_ = new value_t*[size_t(ilen_)];
      value_t* buf = buffer_ - jmin_;
      for (coord_t i = 0; i < ilen_; ++i)
	{
	  array_[i] = buf;
	  buf += jlen_;
	}
      array_ -= imin_;
    }

    void deallocate_()
    {
      precondition(buffer_ != 0 and array_ != 0);
      delete[] buffer_;
      buffer_ = 0; // safety
      array_ += imin_;
      delete[] array_;
      array_ = 0;  // safety
    }
  };


} // end of namespace oln


#endif // ! OLENA_CORE_2D_ARRAY2D_HH
