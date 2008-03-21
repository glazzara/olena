// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_TILED_IMAGE2D_HH
# define MLN_CORE_TILED_IMAGE2D_HH

/*! \file mln/core/tiled_image2d.hh
 *
 * \brief Definition of the basic mln::tiled_image2d class.
 */

# include <mln/core/internal/image_primary.hh>
# include <mln/core/internal/fixme.hh>
# include <mln/core/box2d.hh>
# include <mln/core/inplace.hh>
# include <mln/core/init.hh>
# include <mln/core/line_piter.hh>

# include <mln/geom/size2d.hh>


# include <mln/border/thickness.hh>
# include <mln/value/set.hh>
# include <mln/fun/i2v/all_to.hh>
# include <fcntl.h>

# include "support/lru.hh"
# include "backend/mmap.hh"
# include "layout/layout2d.hh"
# include "block.hh"
# include "page.hh"

// FIXME : give the side's side of the square of the block.
# define BLOCK_SIDE 128
# define BLOCK_SIZE (BLOCK_SIDE * BLOCK_SIDE)

namespace mln
{

  // Fwd decl.
  template <typename T> struct tiled_image2d;


  namespace internal
  {

    /// \internal Data structure for \c mln::tiled_image2d<T>.
    template <typename T>
    struct data_< tiled_image2d<T> >
    {
      typedef block<T, BLOCK_SIZE>          block;
      typedef mmap_backend<block>   backend;
      typedef lru_support<backend>  support;
      typedef layout2d<BLOCK_SIDE, BLOCK_SIDE>      layout;

      data_(const box2d& b);
      ~data_();

      void prepare(const point2d& p) {
	unsigned page_n = layout::image_layout::page_at(*this, p);
	support_.prepare(page_n);
      }

      support& support_;

      box2d b_;  // theoretical box
      unsigned bdr_;
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename T>
    struct image_< tiled_image2d<T> > : default_image_< T, tiled_image2d<T> >
    {
      typedef trait::image::category::primary category;

      typedef trait::image::access::random   access;
      typedef trait::image::space::two_d     space;
      typedef trait::image::size::regular    size;
      typedef trait::image::support::aligned support;

      typedef trait::image::border::stored   border;
      typedef trait::image::data::linear     data;
      typedef trait::image::io::read_write   io;
      typedef trait::image::speed::fast      speed;
    };

  } // end of namespace mln::trait



  /*! \brief FIXME.
   *
   * FIXME
   */
  template <typename T>
  struct tiled_image2d : public internal::image_primary_< box2d, tiled_image2d<T> >
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
    typedef tiled_image2d< tag::value_<T> > skeleton;


    /// Value_Set associated type.
    typedef mln::value::set<T> vset;

    /// Block type.
    typedef block<T, BLOCK_SIZE> block;

    /// Support type.
    typedef lru_support<mmap_backend<block> > support;
    /// Layout type
    typedef layout2d<BLOCK_SIDE, BLOCK_SIDE>                       layout;
    /// Page type
    typedef page<T, layout> page;

    /// Constructor without argument.
    tiled_image2d();

    /// Constructor with the numbers of rows and columns
    tiled_image2d(int nrows, int ncols);

    /// Constructor with a box (default is
    /// 3).
    tiled_image2d(const box2d& b);


    /// Initialize an empty image.
    void init_(const box2d& b);


    /// Test if \p p is valid.
    bool owns_(const point2d& p) const;

    /// Give the set of values of the image.
    const vset& values() const;

    /// Give the definition domain.
    const box2d& domain() const;

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

  };



  // Fwd decl.

  template <typename T, typename J>
  void init_(tag::image_t, mln::tiled_image2d<T>& target, const J& model);



# ifndef MLN_INCLUDE_ONLY

  // init_

  template <typename T, typename J>
  inline
  void init_(tag::image_t, tiled_image2d<T>& target, const J& model)
  {
    box2d b;
    init_(tag::bbox, b, model);
    unsigned bdr;
    target.init_(b);
  }


  // internal::data_< tiled_image2d<T> >

  namespace internal
  {
    template <typename T>
    inline
    data_< tiled_image2d<T> >::data_(const box2d& b)
      : b_ (b),
	// FIXME : hard coded path.
	support_(*new support(
			       *new backend( open("/tmp/milena_tiled.image", O_RDWR | O_CREAT | O_TRUNC, 0664),
					     layout::image_layout::size(geom::nrows(b), geom::ncols(b))),
			       1500 // Fixme : size of lru cache.
			      )
		 )
    {
      std::cout << layout::image_layout::size(geom::nrows(b), geom::ncols(b)) << " block dans l'image."  << std::endl;
      std::cout << layout::image_layout::size(geom::nrows(b), geom::ncols(b)) * BLOCK_SIZE * sizeof(T) << " o au total." << std::endl;
      std::cout << layout::image_layout::size(geom::nrows(b), geom::ncols(b)) * BLOCK_SIZE * sizeof(T) / 1024. << " Ko au total." << std::endl;
      std::cout << layout::image_layout::size(geom::nrows(b), geom::ncols(b)) * BLOCK_SIZE * sizeof(T) / (1024 * 1024.) << " Mo au total." << std::endl;

      char a = 0;
      lseek(support_.backend_.fd_, layout::image_layout::size(geom::nrows(b), geom::ncols(b)) * BLOCK_SIZE * sizeof(T), SEEK_SET);
      write(support_.backend_.fd_, &a, 1);
    }

    template <typename T>
    inline
    data_< tiled_image2d<T> >::~data_()
    {
      close(support_.backend_.fd_);
    }

  } // end of namespace mln::internal


  // tiled_image2d<T>

  template <typename T>
  inline
  tiled_image2d<T>::tiled_image2d()
  {
  }

  template <typename T>
  inline
  tiled_image2d<T>::tiled_image2d(int nrows, int ncols)
  {
    init_(make::box2d(nrows, ncols));
  }

  template <typename T>
  inline
  tiled_image2d<T>::tiled_image2d(const box2d& b)
  {
    init_(b);
  }

  template <typename T>
  inline
  void
  tiled_image2d<T>::init_(const box2d& b)
  {
    mln_precondition(! this->has_data());
    this->data_ = new internal::data_< tiled_image2d<T> >(b);
  }

  template <typename T>
  inline
  const typename tiled_image2d<T>::vset&
  tiled_image2d<T>::values() const
  {
    return vset::the();
  }

  template <typename T>
  inline
  const box2d&
  tiled_image2d<T>::domain() const
  {
    mln_precondition(this->has_data());
    return this->data_->b_;
  }

  template <typename T>
  inline
  std::size_t
  tiled_image2d<T>::ncells() const
  {
    mln_precondition(this->has_data());
    return this->data_->b_.npoints();
  }

  template <typename T>
  inline
  bool
  tiled_image2d<T>::owns_(const point2d& p) const
  {
    mln_precondition(this->has_data());
    return this->data_->b_.has(p);
  }

  template <typename T>
  inline
  const T&
  tiled_image2d<T>::operator()(const point2d& p) const
  {
    mln_precondition(this->owns_(p));
    unsigned page_n = layout::image_layout::page_at(*this, p);
    // note: although the page instance goes
    // out of scope, the reference stays valid
    // because the block is ultimately held by support.
    return page(const_cast<support&>(this->data_->support_).get_block(page_n))
      (layout::image_layout::changeref(p));
   }

  template <typename T>
  inline
  T&
  tiled_image2d<T>::operator()(const point2d& p)
  {
    mln_precondition(this->owns_(p));
    unsigned page_n = layout::image_layout::page_at(*this, p);
    // note: although the page instance goes
    // out of scope, the reference stays valid
    // because the block is ultimately held by support.
    return page(this->data_->support_.get_block(page_n))
      (layout::image_layout::changeref(p));
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_TILED_IMAGE2D_HH
