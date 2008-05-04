// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_IMAGE2D_HH
# define MLN_CORE_IMAGE2D_HH

/// \file mln/core/image2d.hh
/// \brief Definition of the basic mln::image2d class.

# include <mln/core/internal/image_primary.hh>
# include <mln/core/internal/fixme.hh>
# include <mln/core/box2d.hh>
# include <mln/core/init.hh>

# include <mln/border/thickness.hh>
# include <mln/value/set.hh>
# include <mln/fun/i2v/all_to.hh>
# include <mln/core/line_piter.hh>


// FIXME:

// # include <mln/core/pixter2d.hh>
// # include <mln/core/dpoints_pixter.hh>


namespace mln
{

  // Fwd decl.
  template <typename T> struct image2d;


  namespace internal
  {

    /// \internal Data structure for \c mln::image2d<T>.
    template <typename T>
    struct data_< image2d<T> >
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
      void swap_(data_< image2d<T> >& other_);
      void reallocate_(unsigned new_border);
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename T>
    struct image_< image2d<T> > : default_image_< T, image2d<T> >
    {
      typedef trait::image::category::primary category;

      typedef trait::image::access::random   access;
      typedef trait::image::space::two_d     space;
      typedef trait::image::size::regular    size;
      typedef trait::image::support::aligned support;

      typedef trait::image::border::stored   border;
      typedef trait::image::data::raw        data;
      typedef trait::image::io::read_write   io;
      typedef trait::image::speed::fastest   speed;
    };

  } // end of namespace mln::trait



  /*! \brief Basic 2D image class.
   *
   * The parameter \c T is the type of pixel values.  This image class
   * stores data in memory and has a virtual border with constant
   * thickness around data.
   */
  template <typename T>
  struct image2d : public internal::image_primary_< box2d, image2d<T> >
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
    typedef image2d< tag::value_<T> > skeleton;


    /// Value_Set associated type.
    typedef mln::value::set<T> vset;


    /// Constructor without argument.
    image2d();

    /// Constructor with the numbers of rows and columns and the
    /// border thickness.
    image2d(int nrows, int ncols, unsigned bdr = border::thickness);

    /// Constructor with a box and the border thickness (default is
    /// 3).
    image2d(const box2d& b, unsigned bdr = border::thickness);


    /// Initialize an empty image.
    void init_(const box2d& b, unsigned bdr = border::thickness);


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


    /// Resize image border with new_border.
    void resize_(unsigned new_border);
  };



  // Fwd decl.

  template <typename T>
  void init_(tag::border_t, unsigned& bdr, const image2d<T>& model);

  template <typename T, typename J>
  void init_(tag::image_t, mln::image2d<T>& target, const J& model);



# ifndef MLN_INCLUDE_ONLY

  // init_

  template <typename T>
  inline
  void init_(tag::border_t, unsigned& bdr, const image2d<T>& model)
  {
    bdr = model.border();
  }

  template <typename T, typename J>
  inline
  void init_(tag::image_t, image2d<T>& target, const J& model)
  {
    box2d b;
    init_(tag::bbox, b, model);
    unsigned bdr;
    init_(tag::border, bdr, model);
    target.init_(b, bdr);
  }


  // internal::data_< image2d<T> >

  namespace internal
  {
    template <typename T>
    inline
    data_< image2d<T> >::data_(const box2d& b, unsigned bdr)
      : buffer_(0),
	array_ (0),
	b_     (b),
	bdr_   (bdr)
    {
      allocate_();
    }

    template <typename T>
    inline
    data_< image2d<T> >::~data_()
    {
      deallocate_();
    }

    template <typename T>
    inline
    void
    data_< image2d<T> >::update_vb_()
    {
      vb_.pmin() = b_.pmin() - dpoint2d(all_to(bdr_));
      vb_.pmax() = b_.pmax() + dpoint2d(all_to(bdr_));
    }

    template <typename T>
    inline
    void
    data_< image2d<T> >::allocate_()
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
    inline
    void
    data_< image2d<T> >::deallocate_()
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

    template <typename T>
    inline
    void
    data_< image2d<T> >::swap_(data_< image2d<T> >& other_)
    {
      data_< image2d<T> > self_ = *this;
      *this = other_;
      other_ = self_;
    }

    template <typename T>
    inline
    void
    data_< image2d<T> >::reallocate_(unsigned new_border)
    {
      data_< image2d<T> >& tmp = *(new data_< image2d<T> >(this->b_, new_border));
      this->swap_(tmp);
    }


  } // end of namespace mln::internal


  // image2d<T>

  template <typename T>
  inline
  image2d<T>::image2d()
  {
  }

  template <typename T>
  inline
  image2d<T>::image2d(int nrows, int ncols, unsigned bdr)
  {
    init_(make::box2d(nrows, ncols), bdr);
  }

  template <typename T>
  inline
  image2d<T>::image2d(const box2d& b, unsigned bdr)
  {
    init_(b, bdr);
  }

  template <typename T>
  inline
  void
  image2d<T>::init_(const box2d& b, unsigned bdr)
  {
    mln_precondition(! this->has_data());
    this->data_ = new internal::data_< image2d<T> >(b, bdr);
  }

  template <typename T>
  inline
  const typename image2d<T>::vset&
  image2d<T>::values() const
  {
    return vset::the();
  }

  template <typename T>
  inline
  const box2d&
  image2d<T>::domain() const
  {
    mln_precondition(this->has_data());
    return this->data_->b_;
  }

  template <typename T>
  inline
  unsigned
  image2d<T>::border() const
  {
    mln_precondition(this->has_data());
    return this->data_->bdr_;
  }

  template <typename T>
  inline
  std::size_t
  image2d<T>::ncells() const
  {
    mln_precondition(this->has_data());
    return this->data_->vb_.npoints();
  }

  template <typename T>
  inline
  bool
  image2d<T>::owns_(const point2d& p) const
  {
    mln_precondition(this->has_data());
    return this->data_->vb_.has(p);
  }

  template <typename T>
  inline
  const T&
  image2d<T>::operator()(const point2d& p) const
  {
    mln_precondition(this->owns_(p));
    return this->data_->array_[p.row()][p.col()];
  }

  template <typename T>
  inline
  T&
  image2d<T>::operator()(const point2d& p)
  {
    mln_precondition(this->owns_(p));
    return this->data_->array_[p.row()][p.col()];
  }

  template <typename T>
  inline
  const T&
  image2d<T>::operator[](unsigned o) const
  {
    mln_precondition(o < ncells());
    return *(this->data_->buffer_ + o);
  }

  template <typename T>
  inline
  T&
  image2d<T>::operator[](unsigned o)
  {
    mln_precondition(o < ncells());
    return *(this->data_->buffer_ + o);
  }

  template <typename T>
  inline
  const T&
  image2d<T>::at(int row, int col) const
  {
    mln_precondition(this->owns_(make::point2d(row, col)));
    return this->data_->array_[row][col];
  }

  template <typename T>
  inline
  T&
  image2d<T>::at(int row, int col)
  {
    mln_precondition(this->owns_(make::point2d(row, col)));
    return this->data_->array_[row][col];
  }

  template <typename T>
  inline
  const T*
  image2d<T>::buffer() const
  {
    mln_precondition(this->has_data());
    return this->data_->buffer_;
  }

  template <typename T>
  inline
  T*
  image2d<T>::buffer()
  {
    mln_precondition(this->has_data());
    return this->data_->buffer_;
  }

  template <typename T>
  inline
  int
  image2d<T>::offset(const dpoint2d& dp) const
  {
    mln_precondition(this->has_data());
    int o = dp[0] * this->data_->vb_.len(1) + dp[1];
    return o;
  }

  template <typename T>
  inline
  point2d
  image2d<T>::point_at_offset(unsigned o) const
  {
    mln_precondition(o < ncells());
    point2d p = make::point2d(o / this->data_->vb_.len(1) + this->data_->vb_.min_row(),
			      o % this->data_->vb_.len(1) + this->data_->vb_.min_col());
    mln_postcondition(& this->operator()(p) == this->data_->buffer_ + o);
    return p;
  }

  template <typename T>
  inline
  void
  image2d<T>::resize_(unsigned new_border)
  {
    this->data_->reallocate_(new_border);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



# include <mln/core/trait/pixter.hh>
# include <mln/core/dpoints_pixter.hh>
# include <mln/core/pixter2d.hh>
# include <mln/core/w_window.hh>


namespace mln
{

  namespace trait
  {

    // pixter

    template <typename T>
    struct fwd_pixter< image2d<T> >
    {
      typedef fwd_pixter2d< image2d<T> > ret;
    };

    template <typename T>
    struct fwd_pixter< const image2d<T> >
    {
      typedef fwd_pixter2d< const image2d<T> > ret;
    };

    template <typename T>
    struct bkd_pixter< image2d<T> >
    {
      typedef bkd_pixter2d< image2d<T> > ret;
    };

    template <typename T>
    struct bkd_pixter< const image2d<T> >
    {
      typedef bkd_pixter2d< const image2d<T> > ret;
    };

    // qixter

    template <typename T, typename W>
    struct fwd_qixter< image2d<T>, W >
    {
      typedef dpoints_fwd_pixter< image2d<T> > ret;
    };

    template <typename T, typename W>
    struct fwd_qixter< const image2d<T>, W >
    {
      typedef dpoints_fwd_pixter< const image2d<T> > ret;
    };

    template <typename T, typename W>
    struct bkd_qixter< image2d<T>, W >
    {
      typedef dpoints_bkd_pixter< image2d<T> > ret;
    };

    template <typename T, typename W>
    struct bkd_qixter< const image2d<T>, W >
    {
      typedef dpoints_bkd_pixter< const image2d<T> > ret;
    };

    // nixter

    template <typename T, typename N>
    struct fwd_nixter< image2d<T>, N >
    {
      typedef dpoints_fwd_pixter< image2d<T> > ret;
    };

    template <typename T, typename N>
    struct fwd_nixter< const image2d<T>, N >
    {
      typedef dpoints_fwd_pixter< const image2d<T> > ret;
    };

    template <typename T, typename N>
    struct bkd_nixter< image2d<T>, N >
    {
      typedef dpoints_bkd_pixter< image2d<T> > ret;
    };

    template <typename T, typename N>
    struct bkd_nixter< const image2d<T>, N >
    {
      typedef dpoints_bkd_pixter< const image2d<T> > ret;
    };

  } // end of namespace mln::trait

} // end of namespace mln

# include <mln/make/image2d.hh>

#endif // ! MLN_CORE_IMAGE2D_HH
