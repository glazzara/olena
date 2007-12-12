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

#ifndef MLN_CORE_IMAGE3D_HH
# define MLN_CORE_IMAGE3D_HH

/*! \file mln/core/image3d.hh
 *
 * \brief Definition of the basic mln::image3d class.
 */

# include <mln/core/internal/fixme.hh>
# include <mln/core/internal/image_primary.hh>
# include <mln/core/box3d.hh>

# include <mln/border/thickness.hh>
# include <mln/value/set.hh>
# include <mln/fun/i2v/all_to.hh>

# include <mln/core/line_piter.hh>

// FIXME:

// # include <mln/core/pixter3d.hh>
// # include <mln/core/dpoints_pixter.hh>


namespace mln
{

  // Fwd decl.
  template <typename T> struct image3d;



  namespace internal
  {

    /// \internal
    template <typename T>
    struct data_< image3d<T> >
    {
      data_(const box3d& b, unsigned bdr);
      ~data_();

      T*  buffer_;
      T*** array_;

      box3d b_;  // theoretical box
      unsigned bdr_;
      box3d vb_; // virtual box, i.e., box including the virtual border

      void update_vb_();
      void allocate_();
      void deallocate_();
      void swap_ (data_< image3d<T> >& other_);
      void reallocate_(unsigned new_border);
    };

  } // end of namespace mln::internal



  namespace trait
  {

    template <typename T>
    struct image_< image3d<T> > : default_image_< T, image3d<T> >
    {
      typedef trait::image::category::primary category;

      typedef trait::image::access::random   access;
      typedef trait::image::space::three_d   space;
      typedef trait::image::size::regular    size;
      typedef trait::image::support::aligned support;

      typedef trait::image::border::stored   border;
      typedef trait::image::data::raw        data;
      typedef trait::image::io::read_write   io;
      typedef trait::image::speed::fastest   speed;
    };

  } // end of namespace mln::trait



  /*! \brief Basic 3D image class.
   *
   * The parameter \c T is the type of pixel values.  This image class
   * stores data in memory and has a virtual border with constant
   * thickness around data.
   */
  template <typename T>
  struct image3d : public internal::image_primary_< box3d, image3d<T> >
  {
    // Warning: just to make effective types appear in Doxygen:
    typedef box3d   pset;
    typedef point3d psite;
    typedef point3d point;
    typedef dpoint3d dpoint;
    typedef mln_fwd_piter(box3d) fwd_piter;
    typedef mln_bkd_piter(box3d) bkd_piter;
    typedef line_piter_<point> line_piter;
    // End of warning.


    /// Super type
    typedef internal::image_primary_< box3d, image3d<T> > super_;

    /// Value associated type.
    typedef T         value;

    /// Return type of read-only access.
    typedef const T& rvalue;

    /// Return type of read-write access.
    typedef T&       lvalue;


    /// Skeleton.
    typedef image3d< tag::value_<T> > skeleton;


    /// Value_Set associated type.
    typedef mln::value::set<T> vset;


    /// Constructor without argument.
    image3d();

    /// Constructor with a box and the border thickness (default is
    /// 3).
    image3d(const box3d& b, unsigned bdr = border::thickness);

    /// Constructor with the numbers of indexes and the
    /// border thickness.
    image3d(int nslis, int nrows, int ncols, unsigned bdr = border::thickness);


    /// Initialize an empty image.
    void init_(const box3d& b, unsigned bdr = border::thickness);


    /// Test if \p p is valid.
    bool owns_(const point3d& p) const;

    /// Give the set of values of the image.
    const vset& values() const;

    /// Give the definition domain.
    const box3d& domain() const;

    /// Give the border thickness.
    unsigned border() const;

    /// Give the number of cells (points including border ones).
    std::size_t ncells() const;

    /// Read-only access to the image value located at point \p p.
    const T& operator()(const point3d& p) const;

    /// Read-write access to the image value located at point \p p.
    T& operator()(const point3d& p);

    /// Read-only access to the image value located at offset \p o.
    const T& operator[](unsigned o) const;

    /// Read-write access to the image value located at offset \p o.
    T& operator[](unsigned o);

    /// Read-only access to the image value located at (\p ind).
    const T& at(int sli, int row, int col) const;

    /// Read-write access to the image value located at (\p ind).
    T& at(int sli, int row, int col);


    /// Fast Image method

    /// Give the offset corresponding to the delta-point \p dp.
    int offset(const dpoint3d& dp) const;

    /// Give the point corresponding to the offset \p o.
    point3d point_at_offset(unsigned o) const;

    /// Give a hook to the value buffer.
    const T* buffer() const;

    /// Give a hook to the value buffer.
    T* buffer();

    /// To use the track pointer inherited.
    using super_::data_;



    /// Resize image border with new_border.
    void resize_(unsigned new_border);

  };

  template <typename T, typename J>
  void init_(tag::image_t, mln::image3d<T>& target, const J& model);



# ifndef MLN_INCLUDE_ONLY

  // init_

  template <typename T>
  inline
  void init_(tag::border_t, unsigned& b, const image3d<T>& model)
  {
    b = model.border();
  }

  template <typename T, typename J>
  inline
  void init_(tag::image_t, image3d<T>& target, const J& model)
  {
    box3d b;
    init_(tag::bbox, b, model);
    unsigned bdr;
    init_(tag::border, bdr, model);
    target.init_(b, bdr);
  }


  // internal::data_< image3d<T> >

  namespace internal
  {

    template <typename T>
    inline
    data_< image3d<T> >::data_(const box3d& b, unsigned bdr)
      : buffer_(0),
	array_ (0),
	b_     (b),
	bdr_   (bdr)
    {
      allocate_();
    }

    template <typename T>
    inline
    data_< image3d<T> >::~data_()
    {
      deallocate_();
    }

    template <typename T>
    inline
    void
    data_< image3d<T> >::update_vb_()
    {
      vb_.pmin() = b_.pmin() - dpoint3d(all_to(bdr_));
      vb_.pmax() = b_.pmax() + dpoint3d(all_to(bdr_));
    }

    template <typename T>
    inline
    void
    data_< image3d<T> >::allocate_()
    {
      update_vb_();
      unsigned
	ns = vb_.len(0),
	nr = vb_.len(1),
	nc = vb_.len(2);
      buffer_ = new T[nr * nc * ns];
      array_ = new T**[ns];
      T* buf = buffer_ - vb_.pmin().col();
      for (unsigned i = 0; i < ns; ++i)
      {
	T** tmp = new T*[nr];
	array_[i] = tmp;
	for (unsigned j = 0; j < nr; ++j)
	{
	  array_[i][j] = buf;
	  buf += nc;
	}
	array_[i] -= vb_.pmin().row();
      }
      array_ -= vb_.pmin().sli();
      mln_postcondition(vb_.len(0) == b_.len(0) + 2 * bdr_);
    }

    template <typename T>
    inline
    void
    data_< image3d<T> >::deallocate_()
    {
      if (buffer_)
      {
	delete[] buffer_;
	buffer_ = 0;
      }
      for (typename point3d::coord i = vb_.pmin().sli(); i <= vb_.pmax().sli(); ++i)
      {
	if (array_[i])
	{
	  array_[i] += vb_.pmin().row();
	  delete[] array_[i];
	  array_[i] = 0;
	}
      }
      if (array_)
      {
	array_ += vb_.pmin().sli();
	delete[] array_;
	array_ = 0;
      }
    }

    template <typename T>
    inline
    void
    data_< image3d<T> >::swap_(data_< image3d<T> >& other_)
    {
      data_< image3d<T> > self_ = *this;
      *this = other_;
      other_ = self_;
    }

    template <typename T>
    inline
    void
    data_< image3d<T> >::reallocate_(unsigned new_border)
    {
      data_< image3d<T> >& tmp = *(new data_< image3d<T> >(this->b_, new_border));
      this->swap_(tmp);
    }


  } // end of namespace mln::internal

  // image3d<T>

  template <typename T>
  inline
  image3d<T>::image3d()
  {
  }

  template <typename T>
  inline
  image3d<T>::image3d(const box3d& b, unsigned bdr)
  {
    init_(b, bdr);
  }

  template <typename T>
  inline
  image3d<T>::image3d(int nslis, int nrows, int ncols, unsigned bdr)
  {
    init_(make::box3d(nslis, nrows, ncols), bdr);
  }

  template <typename T>
  inline
  void
  image3d<T>::init_(const box3d& b, unsigned bdr)
  {
    mln_precondition(! this->has_data());
    this->data_ = new internal::data_< image3d<T> >(b, bdr);
  }

  template <typename T>
  inline
  const typename image3d<T>::vset&
  image3d<T>::values() const
  {
    return vset::the();
  }

  template <typename T>
  inline
  const box3d&
  image3d<T>::domain() const
  {
    mln_precondition(this->has_data());
    return data_->b_;
  }

  template <typename T>
  inline
  unsigned
  image3d<T>::border() const
  {
    mln_precondition(this->has_data());
    return data_->bdr_;
  }

  template <typename T>
  inline
  std::size_t
  image3d<T>::ncells() const
  {
    mln_precondition(this->has_data());
    return data_->vb_.npoints();
  }

  template <typename T>
  inline
  bool
  image3d<T>::owns_(const point3d& p) const
  {
    mln_precondition(this->has_data());
    return data_->vb_.has(p);
  }

  template <typename T>
  inline
  const T&
  image3d<T>::operator()(const point3d& p) const
  {
    mln_precondition(this->owns_(p));
    return data_->array_[p.sli()][p.row()][p.col()];
  }

  template <typename T>
  inline
  T&
  image3d<T>::operator()(const point3d& p)
  {
    mln_precondition(this->owns_(p));
    return data_->array_[p.sli()][p.row()][p.col()];
  }

  template <typename T>
  inline
  const T&
  image3d<T>::operator[](unsigned o) const
  {
    mln_precondition(o < ncells());
    return *(data_->buffer_ + o);
  }

  template <typename T>
  inline
  T&
  image3d<T>::operator[](unsigned o)
  {
    mln_precondition(o < ncells());
    return *(data_->buffer_ + o);
  }

  template <typename T>
  inline
  const T&
  image3d<T>::at(int sli, int row, int col) const
  {
    mln_precondition(this->owns_(make::point3d(sli, row, col)));
    return data_->array_[sli][row][col];
  }

  template <typename T>
  inline
  T&
  image3d<T>::at(int sli, int row, int col)
  {
    mln_precondition(this->owns_(make::point3d(sli, row, col)));
    return data_->array_[sli][row][col];
  }

  template <typename T>
  inline
  const T*
  image3d<T>::buffer() const
  {
    mln_precondition(this->has_data());
    return data_->buffer_;
  }

  template <typename T>
  inline
  T*
  image3d<T>::buffer()
  {
    mln_precondition(this->has_data());
    return data_->buffer_;
  }

  template <typename T>
  inline
  int
  image3d<T>::offset(const dpoint3d& dp) const
  {
    mln_precondition(this->has_data());
    int o = dp[0];
    return o;
  }

  template <typename T>
  inline
  point3d
  image3d<T>::point_at_offset(unsigned o) const
  {
    mln_precondition(o < ncells());
    point3d p = make::point3d(o / (data_->vb_.len(1) * data_->vb_.len(2)) + data_->vb_.min_sli(),
			      (o % (data_->vb_.len(1) * data_->vb_.len(2))) / data_->vb_.len(2) + data_->vb_.min_row(),
			      o % data_->vb_.len(2) + data_->vb_.min_col());
    mln_postcondition(& this->operator()(p) == this->data_->buffer_ + o);
    return p;
  }

  template <typename T>
  inline
  void
  image3d<T>::resize_(unsigned new_border)
  {
    this->data_->reallocate_(new_border);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



# include <mln/core/trait/pixter.hh>
# include <mln/core/dpoints_pixter.hh>
# include <mln/core/pixter3d.hh>
# include <mln/core/w_window.hh>


namespace mln
{

  namespace trait
  {

    // pixter

    template <typename T>
    struct fwd_pixter< image3d<T> >
    {
      typedef fwd_pixter3d< image3d<T> > ret;
    };

    template <typename T>
    struct fwd_pixter< const image3d<T> >
    {
      typedef fwd_pixter3d< const image3d<T> > ret;
    };

    template <typename T>
    struct bkd_pixter< image3d<T> >
    {
      typedef mln::internal::fixme ret;
    };

    // qixter

    template <typename T, typename W>
    struct fwd_qixter< image3d<T>, W >
    {
      typedef dpoints_fwd_pixter< image3d<T> > ret;
    };

    template <typename T, typename W>
    struct fwd_qixter< const image3d<T>, W >
    {
      typedef dpoints_fwd_pixter< const image3d<T> > ret;
    };

    template <typename T, typename W>
    struct bkd_qixter< image3d<T>, W >
    {
      typedef mln::internal::fixme ret;
    };

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE3D_HH
