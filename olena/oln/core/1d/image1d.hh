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

#ifndef OLN_CORE_1D_IMAGE1D_HH
# define OLN_CORE_1D_IMAGE1D_HH

# include <oln/core/image_entry.hh>
# include <oln/core/gen/topo_lbbox.hh>
# include <oln/core/internal/tracked_ptr.hh>
# include <oln/core/1d/array1d.hh>
# include <oln/core/1d/point1d.hh>
# include <oln/core/1d/bbox1d.hh>
// For topo1d.
# include <oln/core/1d/aliases.hh>
// For fwd_piter and bkd_piter virtual types.
// FIXME: Really necessary?
# include <oln/core/fwd_piter.hh>


namespace oln
{

  // Forward declaration.
  template <typename T> class image1d;


  /// Virtual types associated to oln::image1d<T>.
  template <typename T>
  struct vtypes< image1d<T> >
  {
    typedef topo1d topo_type;
    typedef grid1d grid_type;

    typedef point1d point_type;

    typedef T value_type;
    typedef T lvalue_type;
    typedef mlc::true_ is_mutable_type;

    typedef image1d<T> real_type;
  };


  /// Super type declaration.
  template <typename T>
  struct set_super_type< image1d<T> >
  {
    typedef image1d<T> self_t;
    typedef image_entry<self_t> ret;
  };


  /// General 1D image class.
  template <typename T>
  class image1d : public image_entry< image1d<T> >
  {
    typedef image1d<T> self_t;
    typedef array1d<T> array_t;

  public:

    /// Ctor using sizes.
    image1d(unsigned nindices, unsigned border = 2);

    /// Ctor using an existing topology.
    image1d(const topo1d& topo);

    const topo1d& impl_topo() const;

    T impl_op_read(const point1d& p) const;
    T& impl_op_readwrite(const point1d& p);

    T* adr_at(int index);
    const T* adr_at(int index) const;

  private:

    topo1d topo_;
    internal::tracked_ptr<array_t> data_;
  };



# ifndef OLN_INCLUDE_ONLY

  template <typename T>
  image1d<T>::image1d(unsigned nindices, unsigned border)
    : topo_(bbox1d(point1d(0), point1d(nindices - 1)), border),
      data_(new array_t(0            - border,
			nindices - 1 + border))
  {
  }

  template <typename T>
  image1d<T>::image1d(const topo1d& topo)
    : topo_(topo),
      data_(new array_t(topo.bbox().pmin().index(),
			topo.bbox().pmax().index()))
  {
  }

  template <typename T>
  const topo1d& image1d<T>::impl_topo() const
  {
    return topo_;
  }

  template <typename T>
  T image1d<T>::impl_op_read(const point1d& p) const
  {
    precondition(data_ != 0);
    precondition(topo_.has_large(p));
    return data_->operator()(p.index());
  }

  template <typename T>
  T& image1d<T>::impl_op_readwrite(const point1d& p)
  {
    precondition(data_ != 0);
    precondition(topo_.has_large(p));
    return data_->operator()(p.index());
  }

  template <typename T>
  T* image1d<T>::adr_at(int index)
  {
    precondition(data_ != 0);
    precondition(data_->has(index));
    return &(data_->operator()(index));
  }

  template <typename T>
  const T* image1d<T>::adr_at(int index) const
  {
    precondition(data_ != 0);
    precondition(data_->has(index));
    return &(data_->operator()(index));
  }

# endif


} // end of namespace oln


#endif // ! OLN_CORE_1D_IMAGE1D_HH
