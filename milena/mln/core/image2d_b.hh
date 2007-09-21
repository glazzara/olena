// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_IMAGE2D_B_HH
# define MLN_CORE_IMAGE2D_B_HH

/*! \file mln/core/image2d_b.hh
 *
 * \brief Definition of the basic mln::image2d_b class.
 */

# include <mln/core/internal/image_primary.hh>
# include <mln/core/box2d.hh>

# include <mln/border/thickness.hh>
# include <mln/value/set.hh>
# include <mln/fun/i2v/all.hh>
# include <mln/core/line_piter.hh>
# include <mln/border/get.hh>
# include <mln/debug/println.hh>

// FIXME:

// # include <mln/core/pixter2d_b.hh>
// # include <mln/core/dpoints_pixter.hh>


namespace mln
{

  // Fwd decl.
  template <typename T> struct image2d_b;



  namespace internal
  {

    template <typename T>
    struct data_< image2d_b<T> >
    {
      data_(const box2d& b, unsigned bdr);
      ~data_();
      
      T*  buffer_;
      T** array_;
      
      box2d b_;  // theoretical box
      unsigned bdr_;
      box2d vb_; // virtual box, i.e., box including the virtual border

      void update_vb_();
      void allocate_();
      void deallocate_();
    };

  } // end of namespace mln::internal



  namespace trait
  {

    template <typename T>
    struct is_fast< image2d_b<T> >
    {
      typedef metal::true_ ret;
    };

  } // end of mln::trait



  /*! \brief Basic 2D image class.
   *
   * The parameter \c T is the type of pixel values.  This image class
   * stores data in memory and has a virtual border with constant
   * thickness around data.
   */
  template <typename T>
  struct image2d_b : public internal::image_primary_< box2d, image2d_b<T> >
  {
    // Warning: just to make effective types appear in Doxygen:
    typedef box2d   pset;
    typedef point2d psite;
    typedef point2d point;
    typedef dpoint2d dpoint;
    typedef mln_fwd_piter(box2d) fwd_piter;
    typedef mln_bkd_piter(box2d) bkd_piter;
    typedef line_piter_<point> line_piter;
    // End of warning.


    /// Value associated type.
    typedef T         value;

    /// Return type of read-only access.
    typedef const T& rvalue;

    /// Return type of read-write access.
    typedef T&       lvalue;


    /// Skeleton.
    typedef image2d_b< tag::value<T> > skeleton;


    /// Value_Set associated type.
    typedef mln::value::set<T> vset;


    /// Constructor without argument.
    image2d_b();

    /// Constructor with the numbers of rows and columns and the
    /// border thickness.
    image2d_b(int nrows, int ncols, unsigned bdr = border::thickness);

    /// Constructor with a box and the border thickness (default is
    /// 3).
    image2d_b(const box2d& b, unsigned bdr = border::thickness);


    /// Detach data from an image (free it if nobody else hold it).
    void destroy();


    /// Initialize an empty image.
    void init_with(const box2d& b, unsigned bdr = border::thickness);


    /// Initialize an empty image.
    template <typename I>
    void init_with(const Image<I>& other) // FIXME: Remove this soon obsolete code!
    {
      mln_precondition(this->data_ == 0);
      mln_precondition(exact(other).has_data());
      this->data_ = new internal::data_< image2d_b<T> >(exact(other).bbox(),
							exact(other).border());
    }


    /// Test if \p p is valid.
    bool owns_(const point2d& p) const;

    /// Give the set of values of the image.
    const vset& values() const;

    /// Give the definition domain.
    const box2d& domain() const;

    /// Give the border thickness.
    unsigned border() const;

    /// Give the number of cells (points including border ones).
    std::size_t ncells() const;

    /// Read-only access to the image value located at point \p p.
    const T& operator()(const point2d& p) const;

    /// Read-write access to the image value located at point \p p.
    T& operator()(const point2d& p);

    /// Read-only access to the image value located at offset \p o.
    const T& operator[](unsigned o) const;

    /// Read-write access to the image value located at offset \p o.
    T& operator[](unsigned o);

    /// Read-only access to the image value located at (\p row, \p col).
    const T& at(int row, int col) const;

    /// Read-write access to the image value located at (\p row, \p col).
    T& at(int row, int col);


    /// Fast Image method

    /// Give the offset corresponding to the delta-point \p dp.
    int offset(const dpoint2d& dp) const;

    /// Give the point corresponding to the offset \p o.
    point2d point_at_offset(unsigned o) const;

    /// Give a hook to the value buffer.
    const T* buffer() const;

    /// Give a hook to the value buffer.
    T* buffer();
  };


  namespace impl
  {

    template <typename T, typename I>
    void init_with_(image2d_b<T>& target, const I& model)
    {
      box2d b = model.domain();
      unsigned bdr = border::get(model);
      target = image2d_b<T>(b, bdr);
    }
    
  } // end of namespace mln::impl



# ifndef MLN_INCLUDE_ONLY


  // internal::data_< image2d_b<T> >

  namespace internal
  {

    template <typename T>
    data_< image2d_b<T> >::data_(const box2d& b, unsigned bdr)
      : buffer_(0),
	array_ (0),
	b_     (b),
	bdr_   (bdr)
    {
      allocate_();
    }

    template <typename T>
    data_< image2d_b<T> >::~data_()
    {
      deallocate_();
    }

    template <typename T>
    void
    data_< image2d_b<T> >::update_vb_()
    {
      vb_.pmin() = b_.pmin() - dpoint2d(all(bdr_));
      vb_.pmax() = b_.pmax() + dpoint2d(all(bdr_));
    }

    template <typename T>
    void
    data_< image2d_b<T> >::allocate_()
    {
      update_vb_();
      unsigned
	nr = vb_.len(0),
	nc = vb_.len(1);
      buffer_ = new T[nr * nc];
      array_ = new T*[nr];
      T* buf = buffer_ - vb_.pmin().col();
      for (unsigned i = 0; i < nr; ++i)
	{
	  array_[i] = buf;
	  buf += nc;
	}
      array_ -= vb_.pmin().row();
      mln_postcondition(vb_.len(0) == b_.len(0) + 2 * bdr_);
      mln_postcondition(vb_.len(1) == b_.len(1) + 2 * bdr_);
    }

    template <typename T>
    void
    data_< image2d_b<T> >::deallocate_()
    {
      if (buffer_)
	{
	  delete[] buffer_;
	  buffer_ = 0;
	}
      if (array_)
	{
	  array_ += vb_.pmin().row();
	  delete[] array_;
	  array_ = 0;
	}
    }

  } // end of namespace mln::internal


  // image2d_b<T>

  template <typename T>
  image2d_b<T>::image2d_b()
  {
  }

  template <typename T>
  image2d_b<T>::image2d_b(int nrows, int ncols, unsigned bdr)
  {
    init_with(make::box2d(nrows, ncols), bdr);
  }

  template <typename T>
  image2d_b<T>::image2d_b(const box2d& b, unsigned bdr)
  {
    init_with(b, bdr);
  }

  template <typename T>
  void
  image2d_b<T>::init_with(const box2d& b, unsigned bdr)
  {
    mln_precondition(! this->has_data());
    this->data_ = new internal::data_< image2d_b<T> >(b, bdr);
  }

  template <typename T>
  const typename image2d_b<T>::vset&
  image2d_b<T>::values() const
  {
    return vset::the();
  }

  template <typename T>
  const box2d&
  image2d_b<T>::domain() const
  {
    mln_precondition(this->has_data());
    return this->data_->b_;
  }

  template <typename T>
  unsigned
  image2d_b<T>::border() const
  {
    mln_precondition(this->has_data());
    return this->data_->bdr_;
  }

  template <typename T>
  std::size_t
  image2d_b<T>::ncells() const
  {
    mln_precondition(this->has_data());
    return this->data_->vb_.npoints();
  }

  template <typename T>
  bool
  image2d_b<T>::owns_(const point2d& p) const
  {
    mln_precondition(this->has_data());
    return this->data_->vb_.has(p);
  }

  template <typename T>
  const T&
  image2d_b<T>::operator()(const point2d& p) const
  {
    mln_precondition(this->owns_(p));
    return this->data_->array_[p.row()][p.col()];
  }

  template <typename T>
  T&
  image2d_b<T>::operator()(const point2d& p)
  {
    mln_precondition(this->owns_(p));
    return this->data_->array_[p.row()][p.col()];
  }

  template <typename T>
  const T&
  image2d_b<T>::operator[](unsigned o) const
  {
    mln_precondition(o < ncells());
    return *(this->data_->buffer_ + o);
  }

  template <typename T>
  T&
  image2d_b<T>::operator[](unsigned o)
  {
    mln_precondition(o < ncells());
    return *(this->data_->buffer_ + o);
  }

  template <typename T>
  const T&
  image2d_b<T>::at(int row, int col) const
  {
    mln_precondition(this->owns_(make::point2d(row, col)));
    return this->data_->array_[row][col];
  }

  template <typename T>
  T&
  image2d_b<T>::at(int row, int col)
  {
    mln_precondition(this->owns_(make::point2d(row, col)));
    return this->data_->array_[row][col];
  }

  template <typename T>
  const T*
  image2d_b<T>::buffer() const
  {
    mln_precondition(this->has_data());
    return this->data_->buffer_;
  }

  template <typename T>
  T*
  image2d_b<T>::buffer()
  {
    mln_precondition(this->has_data());
    return this->data_->buffer_;
  }

  template <typename T>
  int
  image2d_b<T>::offset(const dpoint2d& dp) const
  {
    mln_precondition(this->has_data());
    int o = dp[0] * this->data_->vb_.len(1) + dp[1];
    return o;
  }

  template <typename T>
  point2d
  image2d_b<T>::point_at_offset(unsigned o) const
  {
    mln_precondition(o < ncells());
    point2d p = make::point2d(o / this->data_->vb_.len(1) + this->data_->vb_.min_row(),
			      o % this->data_->vb_.len(1) + this->data_->vb_.min_col());
    mln_postcondition(& this->operator()(p) == this->data_->buffer_ + o);
    return p;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



# include <mln/core/trait/pixter.hh>
# include <mln/core/dpoints_pixter.hh>
# include <mln/core/pixter2d_b.hh>
# include <mln/core/w_window.hh>


namespace mln
{

  namespace trait
  {

    // pixter

    template <typename T>
    struct fwd_pixter< image2d_b<T> >
    {
      typedef fwd_pixter2d_b< image2d_b<T> > ret;
    };

    template <typename T>
    struct fwd_pixter< const image2d_b<T> >
    {
      typedef fwd_pixter2d_b< const image2d_b<T> > ret;
    };

    template <typename T>
    struct bkd_pixter< image2d_b<T> >
    {
      typedef bkd_pixter2d_b< image2d_b<T> > ret;
    };

    template <typename T>
    struct bkd_pixter< const image2d_b<T> >
    {
      typedef bkd_pixter2d_b< const image2d_b<T> > ret;
    };

    // qixter

    template <typename T, typename W>
    struct fwd_qixter< image2d_b<T>, W >
    {
      typedef dpoints_fwd_pixter< image2d_b<T> > ret;
    };

    template <typename T, typename W>
    struct fwd_qixter< const image2d_b<T>, W >
    {
      typedef dpoints_fwd_pixter< const image2d_b<T> > ret;
    };

    template <typename T, typename W>
    struct bkd_qixter< image2d_b<T>, W >
    {
      typedef internal::fixme ret;
    };

    // nixter

    template <typename T, typename N>
    struct fwd_nixter< image2d_b<T>, N >
    {
      typedef dpoints_fwd_pixter< image2d_b<T> > ret;
    };

    template <typename T, typename N>
    struct fwd_nixter< const image2d_b<T>, N >
    {
      typedef dpoints_fwd_pixter< const image2d_b<T> > ret;
    };

    template <typename T, typename N>
    struct bkd_nixter< image2d_b<T>, N >
    {
      typedef internal::fixme ret;
    };

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE2D_B_HH
