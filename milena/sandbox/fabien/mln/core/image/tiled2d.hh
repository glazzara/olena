// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_IMAGE_TILED2D_HH
# define MLN_CORE_IMAGE_TILED2D_HH

/// \file
/// Definition of the basic mln::tiled2d class.

# include <mln/core/internal/image_primary.hh>
# include <mln/core/internal/fixme.hh>
# include <mln/core/alias/box2d.hh>
# include <mln/core/routine/init.hh>

# include <mln/border/thickness.hh>
# include <mln/value/set.hh>
# include <mln/fun/i2v/all_to.hh>
# include <mln/value/proxy.hh>

# include <mln/core/image/cache.hh>



namespace mln
{

  // Forward declaration.
  template <typename T> struct tiled2d;


  namespace internal
  {

    /// Data structure for \c mln::tiled2d<T>.
    template <typename T>
    struct data< tiled2d<T> >
    {
      data(const box2d& b, unsigned bdr);
      ~data();

      std::fstream* f_;
      std::streampos pos_;
      std::string file_;
      bool loaded_;
      T value_;
      cache<box2d, T>* cache_;

      box2d b_;  // theoretical box
      unsigned bdr_;
      box2d vb_; // virtual box, i.e., box including the virtual border

      void update_vb_();
      void allocate_();
      void deallocate_();
      void swap_(data< tiled2d<T> >& other_);
      void reallocate_(unsigned new_border);
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename T>
    struct image_< tiled2d<T> > : default_image_< T, tiled2d<T> >
    {
      // misc
      typedef trait::image::category::primary category;
      typedef trait::image::speed::slow	      speed;
      typedef trait::image::size::regular     size;

      // value
      typedef trait::image::vw_io::none                    vw_io;
      typedef trait::image::vw_set::none                   vw_set;
      typedef trait::image::value_access::direct           value_access;
      typedef trait::image::value_storage::one_block       value_storage;
      typedef trait::image::value_browsing::site_wise_only value_browsing;
      typedef trait::image::value_alignment::with_grid     value_alignment;
      typedef trait::image::value_io::read_only           value_io;

      // site / domain
      typedef trait::image::pw_io::read_write        pw_io;
      typedef trait::image::localization::basic_grid localization;
      typedef trait::image::dimension::two_d         dimension;

      // extended domain
      typedef trait::image::ext_domain::extendable ext_domain;
      typedef trait::image::ext_value::multiple    ext_value;
      typedef trait::image::ext_io::read_only     ext_io;
    };

  } // end of namespace mln::trait



  /// Basic 2D image class.
  ///
  /// The parameter \c T is the type of pixel values.  This image class
  /// stores data in memory and has a virtual border with constant
  /// thickness around data.
  ///
  /// \ingroup modimageconcrete
  //
  template <typename T>
  class tiled2d : public internal::image_primary< T, mln::box2d, tiled2d<T> >
  {
    typedef internal::image_primary< T, mln::box2d, tiled2d<T> > super_;
  public:

    /// Value associated type.
    typedef T         value;

    /// Return type of read-only access.
    typedef const T& rvalue;

    /// Return type of read-write access.
    typedef mln::value::proxy< tiled2d<T> >       lvalue;


    /// Skeleton.
    typedef tiled2d< tag::value_<T> > skeleton;


    /// Constructor without argument.
    tiled2d();

    /// Constructor with a box and the border thickness (default is
    /// 3).
    tiled2d(const box2d& b, unsigned bdr = border::thickness);


    /// Initialize an empty image.
    void init_(const box2d& b, unsigned bdr = border::thickness);

    /// Initialize an empty image.
    //void init_(const std::string& filename);


    /// Test if \p p is valid.
    bool has(const point2d& p) const;

    /// Give the definition domain.
    const box2d& domain() const;

    /// Give the bounding box domain.
    const box2d& bbox() const;

    /// Read-only access to the image value located at point \p p.
    const T& operator()(const point2d& p) const;

    /// Read-write access to the image value located at point \p p.
    mln::value::proxy< tiled2d<T> > operator()(const point2d& p);

    // Read access to the image value located at point \p p.
    const T& read_(const point2d& p) const;

    // Write access to the image value located at point \p p.
    void write_(const point2d& p, const T& value);


    template <typename P>
    T& alt(const P& p)
    {
      typedef def::coord coord_t;
      mln_precondition(this->has(p));

      return this->data_->array_ [*(coord_t*)(&p.p_hook_())] [*((coord_t*)(&p.p_hook_()) + 1)];
    }


    // Specific methods:
    // -----------------

    /// Read-only access to the image value located at (\p row, \p col).
    const T& at_(unsigned row, unsigned col) const;

    /// Read-write access to the image value located at (\p row, \p col).
    T& at_(unsigned row, unsigned col);

    /// Give the number of rows.
    unsigned nrows() const;

    /// Give the number of columns.
    unsigned ncols() const;



    /// Give the border thickness.
    unsigned border() const;

    /// Give the number of elements (points including border ones).
    unsigned nelements() const;

    /// Read-only access to the image value located at index \p i.
    const T& element(unsigned i) const;

    /// Read-write access to the image value located at index \p i.
    T& element(unsigned i);

    /// Give the delta-index corresponding to the delta-point \p dp.
    int delta_index(const dpoint2d& dp) const;

    /// Give the point corresponding to the index \p i.
    point2d point_at_index(unsigned i) const;


    // Hooks

    /// Give a hook to the offset for accessing data.
    const std::streampos& pos_() const;

    /// Give a hook to the offset for accessing data.
    std::streampos& pos_();

    /// Give a hook to the filename.
    const std::string& file_() const;

    /// Give a hook to the filename.
    std::string& file_();


    /// Resize image border with new_border.
    void resize_(unsigned new_border);
  };



  // Forward declaration

  template <typename T>
  void init_(tag::border_t, unsigned& bdr, const tiled2d<T>& model);

  template <typename T, typename J>
  void init_(tag::image_t, mln::tiled2d<T>& target, const J& model);



# ifndef MLN_INCLUDE_ONLY

  // init_

  template <typename T>
  inline
  void init_(tag::border_t, unsigned& bdr, const tiled2d<T>& model)
  {
    bdr = model.border();
  }

  template <typename T, typename J>
  inline
  void init_(tag::image_t, tiled2d<T>& target, const J& model)
  {
    box2d b;
    init_(tag::bbox, b, model);
    unsigned bdr;
    init_(tag::border, bdr, model);
    target.init_(b, bdr);
  }


  // internal::data< tiled2d<T> >

  namespace internal
  {
    template <typename T>
    inline
    data< tiled2d<T> >::data(const box2d& b, unsigned bdr)
      : b_     (b),
	bdr_   (bdr)
    {
      allocate_();
    }

    template <typename T>
    inline
    data< tiled2d<T> >::~data()
    {
      deallocate_();
    }

    template <typename T>
    inline
    void
    data< tiled2d<T> >::update_vb_()
    {
      vb_.pmin() = b_.pmin() - dpoint2d(all_to(bdr_));
      vb_.pmax() = b_.pmax() + dpoint2d(all_to(bdr_));
    }

    template <typename T>
    inline
    void
    data< tiled2d<T> >::allocate_()
    {
      update_vb_();
      mln_postcondition(vb_.len(0) == b_.len(0) + 2 * bdr_);
      mln_postcondition(vb_.len(1) == b_.len(1) + 2 * bdr_);
    }

    template <typename T>
    inline
    void
    data< tiled2d<T> >::deallocate_()
    {
      if (this->loaded_)
      {
	delete this->cache_;
	this->f_->close();
      }
    }

    template <typename T>
    inline
    void
    data< tiled2d<T> >::swap_(data< tiled2d<T> >& other_)
    {
      data< tiled2d<T> > self_ = *this;
      *this = other_;
      other_ = self_;
    }

    template <typename T>
    inline
    void
    data< tiled2d<T> >::reallocate_(unsigned new_border)
    {
      data< tiled2d<T> >& tmp = *(new data< tiled2d<T> >(this->b_, new_border));
      this->swap_(tmp);
    }


  } // end of namespace mln::internal


  // tiled2d<T>

  template <typename T>
  inline
  tiled2d<T>::tiled2d()
  {
  }

  template <typename T>
  inline
  tiled2d<T>::tiled2d(const box2d& b, unsigned bdr)
  {
    init_(b, bdr);
  }

  template <typename T>
  inline
  void
  tiled2d<T>::init_(const box2d& b, unsigned bdr)
  {
    mln_precondition(! this->is_valid());
    this->data_ = new internal::data< tiled2d<T> >(b, bdr);
    this->data_->loaded_ = false;
  }

  template <typename T>
  inline
  const box2d&
  tiled2d<T>::domain() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_;
  }

  template <typename T>
  inline
  const box2d&
  tiled2d<T>::bbox() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_;
  }

  template <typename T>
  inline
  bool
  tiled2d<T>::has(const point2d& p) const
  {
    mln_precondition(this->is_valid());
    return this->data_->vb_.has(p);
  }

  template <typename T>
  inline
  const T&
  tiled2d<T>::operator()(const point2d& p) const
  {
    return read_(p);
  }

  template <typename T>
  inline
  mln::value::proxy< tiled2d<T> >
  tiled2d<T>::operator()(const point2d& p)
  {
    mln::value::proxy<tiled2d> prx(*this, p);
    return prx;
  }

  template <typename T>
  inline
  const T&
  tiled2d<T>::read_(const point2d& p) const
  {
    mln::tiled2d<T>* this_ = const_cast<mln::tiled2d<T>* >(this); // Trust me, I have to do this(_).
    if (!this_->data_->loaded_)
    {
      this_->data_->f_ = new std::fstream(this->data_->file_.c_str());
      this_->data_->loaded_ = true;
      this_->data_->cache_ = new cache<box2d, T>(this_->data_->b_,
						 this_->data_->pos_,
						 this_->ncols(),
						 this_->data_->f_);
    }
    /*std::streampos offset = this_->data_->pos_;
    offset += (this_->ncols() * p.row() + p.col()) * sizeof(T);
    this_->data_->f_->seekg(offset);
    this_->data_->f_->get((char*)(&this_->data_->value_), sizeof(T) + 1); // FIXME: I don't know why
									  // I have to add +1.
    return this_->data_->value_;*/
    return this_->data_->cache_->read(p);
  }

  template <typename T>
  inline
  void
  tiled2d<T>::write_(const point2d& p, const T& value)
  {
    mln::tiled2d<T>* this_ = const_cast<mln::tiled2d<T>* >(this); // Trust me, I have to do this(_).
    if (!this_->data_->loaded_)
    {
      this_->data_->f_ = new std::fstream(this->data_->file_.c_str());
      this_->data_->loaded_ = true;
      this_->data_->cache_ = new cache<box2d, T>(this_->data_->b_,
						 this_->data_->pos_,
						 this_->ncols(),
						 this_->data_->f_);
    }
    this_->data_->cache_->write(p, value);
  }


  // Specific methods:

  template <typename T>
  inline
  const T&
  tiled2d<T>::at_(unsigned row, unsigned col) const
  {
    mln_precondition(this->has(point2d(row, col)));
    return this->data_->cache_(point2d(row, col));
  }

  template <typename T>
  inline
  T&
  tiled2d<T>::at_(unsigned row, unsigned col)
  {
    mln_precondition(this->has(point2d(row, col)));
    return this->data_->cache_(point2d(row, col));
  }

  template <typename T>
  inline
  unsigned
  tiled2d<T>::nrows() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_.len(0);
  }

  template <typename T>
  inline
  unsigned
  tiled2d<T>::ncols() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_.len(1);
  }


  // Hooks.

  template <typename T>
  inline
  const std::streampos&
  tiled2d<T>::pos_() const
  {
    mln_precondition(this->is_valid());
    return this->data_->pos_;
  }

  template <typename T>
  inline
  std::streampos&
  tiled2d<T>::pos_()
  {
    mln_precondition(this->is_valid());
    return this->data_->pos_;
  }

  template <typename T>
  inline
  const std::string&
  tiled2d<T>::file_() const
  {
    mln_precondition(this->is_valid());
    return this->data_->file_;
  }

  template <typename T>
  inline
  std::string&
  tiled2d<T>::file_()
  {
    mln_precondition(this->is_valid());
    return this->data_->file_;
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



# include <mln/core/trait/pixter.hh>
# include <mln/core/dpoints_pixter.hh>
# include <mln/core/pixter2d.hh>
// # include <mln/core/w_window.hh>


namespace mln
{

  namespace trait
  {

    // pixter

    template <typename T>
    struct fwd_pixter< tiled2d<T> >
    {
      typedef fwd_pixter2d< tiled2d<T> > ret;
    };

    template <typename T>
    struct fwd_pixter< const tiled2d<T> >
    {
      typedef fwd_pixter2d< const tiled2d<T> > ret;
    };

    template <typename T>
    struct bkd_pixter< tiled2d<T> >
    {
      typedef bkd_pixter2d< tiled2d<T> > ret;
    };

    template <typename T>
    struct bkd_pixter< const tiled2d<T> >
    {
      typedef bkd_pixter2d< const tiled2d<T> > ret;
    };

    // qixter

    template <typename T, typename W>
    struct fwd_qixter< tiled2d<T>, W >
    {
      typedef dpoints_fwd_pixter< tiled2d<T> > ret;
    };

    template <typename T, typename W>
    struct fwd_qixter< const tiled2d<T>, W >
    {
      typedef dpoints_fwd_pixter< const tiled2d<T> > ret;
    };

    template <typename T, typename W>
    struct bkd_qixter< tiled2d<T>, W >
    {
      typedef dpoints_bkd_pixter< tiled2d<T> > ret;
    };

    template <typename T, typename W>
    struct bkd_qixter< const tiled2d<T>, W >
    {
      typedef dpoints_bkd_pixter< const tiled2d<T> > ret;
    };

    // nixter

    template <typename T, typename N>
    struct fwd_nixter< tiled2d<T>, N >
    {
      typedef dpoints_fwd_pixter< tiled2d<T> > ret;
    };

    template <typename T, typename N>
    struct fwd_nixter< const tiled2d<T>, N >
    {
      typedef dpoints_fwd_pixter< const tiled2d<T> > ret;
    };

    template <typename T, typename N>
    struct bkd_nixter< tiled2d<T>, N >
    {
      typedef dpoints_bkd_pixter< tiled2d<T> > ret;
    };

    template <typename T, typename N>
    struct bkd_nixter< const tiled2d<T>, N >
    {
      typedef dpoints_bkd_pixter< const tiled2d<T> > ret;
    };

  } // end of namespace mln::trait

} // end of namespace mln


# include <mln/make/image.hh>


#endif // ! MLN_CORE_IMAGE_TILED2D_HH
