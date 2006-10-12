// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006 EPITA Research and
// Development Laboratory
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

#ifndef OLN_CORE_3D_IMAGE3D_HH
# define OLN_CORE_3D_IMAGE3D_HH

# include <oln/core/image_entry.hh>
# include <oln/core/gen/topo_lbbox.hh>
# include <oln/core/internal/tracked_ptr.hh>
# include <oln/core/3d/array3d.hh>
# include <oln/core/3d/point3d.hh>
# include <oln/core/3d/bbox3d.hh>
// For topo3d.
# include <oln/core/3d/aliases.hh>
// For fwd_piter and bkd_piter virtual types.
// FIXME: Really necessary?
# include <oln/core/iterator_vtypes.hh>


namespace oln
{

  // Forward declaration.
  template <typename T> class image3d;


  /// Virtual types associated to oln::image3d<T>.
  template <typename T>
  struct vtypes< image3d<T> >
  {
    typedef topo3d topo_type;
    typedef grid3d grid_type;

    typedef point3d point_type;

    typedef T value_type;
    typedef T lvalue_type;
    typedef mlc::true_ is_mutable_type;

    typedef image3d<T> real_type;
  };


  /// Super type declaration.
  template <typename T>
  struct set_super_type< image3d<T> >
  {
    typedef image3d<T> self_t;
    typedef image_entry<self_t> ret;
  };


  /// General 3D image class.
  template <typename T>
  class image3d : public image_entry< image3d<T> >
  {
    typedef image3d<T> self_t;
    typedef array3d<T> array_t;

  public:

    /// Ctor using sizes.
    image3d(unsigned nslices, unsigned nrows, unsigned ncols,
	    unsigned border = 2);

    /// Ctor using an existing topology.
    image3d(const topo3d& topo);

    const topo3d& impl_topo() const;

    T impl_op_read(const point3d& p) const;
    T impl_at(int slice, int row, int col) const;

    T& impl_op_readwrite(const point3d& p);
    T& impl_at(int slice, int row, int col);

    T* adr_at(int slice, int row, int col);
    const T* adr_at(int slice, int row, int col) const;

  private:

    topo3d topo_;
    internal::tracked_ptr<array_t> data_;
  };


# ifndef OLN_INCLUDE_ONLY

  template <typename T>
  image3d<T>::image3d(unsigned nslices, unsigned nrows, unsigned ncols,
		      unsigned border)
    : topo_(bbox3d(point3d(0,           0,         0        ),
		   point3d(nslices - 1, nrows - 1, ncols - 1)),
	    border),
      data_(new array_t(0           - border,
			0           - border,
			0           - border,
			nslices - 1 + border,
			nrows   - 1 + border,
			ncols   - 1 + border))
  {
  }

  template <typename T>
  image3d<T>::image3d(const topo3d& topo)
    : topo_(topo),
      data_(new array_t(topo.bbox().pmin().slice(),
			topo.bbox().pmin().row(),
			topo.bbox().pmin().col(),
			topo.bbox().pmax().slice(),
			topo.bbox().pmax().row(),
			topo.bbox().pmax().col()))
  {
  }

  template <typename T>
  const topo3d&
  image3d<T>::impl_topo() const
  {
    return topo_;
  }


  template <typename T>
  T
  image3d<T>::impl_op_read(const point3d& p) const
  {
    precondition(data_ != 0);
    precondition(topo_.has_large(p));
    return data_->operator()(p.slice(), p.row(), p.col());
  }

  template <typename T>
  T
  image3d<T>::impl_at(int slice, int row, int col) const
  {
    precondition(data_->has(slice, row, col));
    return data_->operator()(slice, row, col);
  }


  template <typename T>
  T&
  image3d<T>::impl_op_readwrite(const point3d& p)
  {
    precondition(data_ != 0);
    precondition(topo_.has_large(p));
    return data_->operator()(p.slice(), p.row(), p.col());
  }

  template <typename T>
  T&
  image3d<T>::impl_at(int slice, int row, int col)
  {
    precondition(data_->has(slice, row, col));
    return data_->operator()(slice, row, col);
  }


  template <typename T>
  T* image3d<T>::adr_at(int slice, int row, int col)
  {
    precondition(data_ != 0);
    precondition(data_->has(slice, row, col));
    return &(data_->operator()(slice, row, col));
  }

  template <typename T>
  const T* image3d<T>::adr_at(int slice, int row, int col) const
  {
    precondition(data_ != 0);
    precondition(data_->has(slice, row, col));
    return &(data_->operator()(slice, row, col));
  }

# endif


} // end of namespace oln


#endif // ! OLN_CORE_3D_IMAGE3D_HH
