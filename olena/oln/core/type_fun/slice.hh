// Copyright (C) 2006 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_TYPE_FUN_SLICE_HH
# define OLN_CORE_TYPE_FUN_SLICE_HH

# include <mlc/flags.hh>

# include <oln/core/1d/point1d.hh>
# include <oln/core/1d/topo1d.hh>

# include <oln/core/2d/point2d.hh>
# include <oln/core/2d/topo2d.hh>

# include <oln/core/3d/point3d.hh>
# include <oln/core/3d/topo3d.hh>


namespace oln
{

  /*-----------------------------------.
  | Point translation during slicing.  |
  `-----------------------------------*/

  // FIXME: To be moved elsewhere (these are function on values, not
  // on types).

  // FIXME: Can't we rewrite this in a more generic way?

  // --------- //
  // Slicing.  //
  // --------- //

  /// Translate an image point to a slice point.
  /// \{

  /// 2D to 1D version.
  point1d slice_point(const point2d& p, unsigned slice_dim)
  {
    switch (slice_dim)
      {
      case 0:
	return point1d(p.col());
	break;
      case 1:
	return point1d(p.row());
	break;
      default:
	abort();
      }
  }

  /// 3D to 2D version.
  point2d slice_point(const point3d& p, unsigned slice_dim)
  {
    switch (slice_dim)
      {
      case 0:
	return point2d(p.row(), p.col());
	break;
      case 1:
	return point2d(p.slice(), p.col());
	break;
      case 2:
	return point2d(p.slice(), p.row());
	break;
      default:
	abort();
      }
  }

  /// \}


  // ----------- //
  // Unslicing.  //
  // ----------- //

  /// Translate a slice point to an image point.
  /// \{

  /// 1D to 2D version.
  point2d unslice_point(const point1d& p, unsigned slice_dim, int slice_coord)
  {
    switch (slice_dim)
      {
      case 0:
	return point2d(slice_coord, p.index());
	break;
      case 1:
	return point2d(p.index(), slice_coord);
	break;
      default:
	abort();
      }
  }

  /// 2D to 3D version.
  point3d unslice_point(const point2d& p, unsigned slice_dim, int slice_coord)
  {
    switch (slice_dim)
      {
      case 0:
	return point3d(slice_coord, p.row(), p.col());
	break;
      case 1:
	return point3d(p.row(), slice_coord, p.col());
	break;
      case 2:
	return point3d(p.row(), p.col(), slice_coord);
	break;
      default:
	abort();
      }
  }

  /// \}


  /// Error messages raised by static assertions/abortions.
  namespace ERROR
  {
    struct TYPE_FUNCTION_slice_piter_NOT_DEFINED_FOR_THIS_PARAMETER;
    struct TYPE_FUNCTION_slice_topo_NOT_DEFINED_FOR_THIS_PARAMETER;
  }


  namespace type_fun
  {

    /*------------------------.
    | type_fun::slice_piter.  |
    `------------------------*/

    /// Type function returning the ``sliced'' piter corresponding to
    /// \a Piter
    /// \{

    /// Default version, undefined.
    template <typename Piter>
    struct slice_piter :
      public mlc::abort_<
        Piter, 
        oln::ERROR::TYPE_FUNCTION_slice_piter_NOT_DEFINED_FOR_THIS_PARAMETER
      >
    {
      typedef mlc::undefined ret;
    };


    // --------- //
    // image2d.  //
    // --------- //

    template <>
    struct slice_piter<fwd_piter2d>
    {
      typedef fwd_piter1d ret;
    };

    template <>
    struct slice_piter<bkd_piter2d>
    {
      typedef bkd_piter1d ret;
    };


    // --------- //
    // image3d.  //
    // --------- //

    template <>
    struct slice_piter<fwd_piter3d>
    {
      typedef fwd_piter2d ret;
    };

    template <>
    struct slice_piter<bkd_piter3d>
    {
      typedef bkd_piter2d ret;
    };

    /// \}


    /// Type function returning the ``sliced'' topo corresponding to
    /// \a Topo
    /// \{

    /// Default version, undefined.
    template <typename Topo>
    struct slice_topo :
      public mlc::abort_<
        Topo, 
        oln::ERROR::TYPE_FUNCTION_slice_topo_NOT_DEFINED_FOR_THIS_PARAMETER
      >
    {
      typedef mlc::undefined ret;
    };


    // --------- //
    // image2d.  //
    // --------- //

    // FIXME: slice_topo<topo2d> is more than a function on type,
    // because of convert().  (Where should me move this?)

    template <>
    struct slice_topo<topo2d>
    {
      typedef topo1d ret;

      /// Convert a topo2d to a topo1d.
      static topo1d convert (const topo2d& topo, unsigned slice_dim)
      {
	return topo1d(slice_point(topo.bbox().pmin(), slice_dim),
		      slice_point(topo.bbox().pmax(), slice_dim),
		      topo.border());	
      }
    };


    // --------- //
    // image3d.  //
    // --------- //

    // FIXME: slice_topo<topo3d> is more than a function on type,
    // because of convert().  (Where should me move this?)

    template <>
    struct slice_topo<topo3d>
    {

      typedef topo2d ret;

      /* FIXME: slice_topo<topo3d>::convert() is almost the same as
	 slice_topo<topo2d>::convert().  */

      /// Convert a topo2d to a topo1d.
      static topo2d convert (const topo3d& topo, unsigned slice_dim)
      {
	return topo2d(slice_point(topo.bbox().pmin(), slice_dim),
		      slice_point(topo.bbox().pmax(), slice_dim),
		      topo.border());	
      }
    };

  } // end of namespace oln::type_fun

} // end of namespace oln


#endif // ! OLN_CORE_TYPE_FUN_SLICE_HH
