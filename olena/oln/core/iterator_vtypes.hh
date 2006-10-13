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

#ifndef OLN_CORE_FWD_PITER_HH
# define OLN_CORE_FWD_PITER_HH

# include <oln/core/abstract/entry.hh>
# include <oln/core/abstract/image.hh>
# include <oln/core/1d/aliases.hh>
# include <oln/core/2d/aliases.hh>
# include <oln/core/3d/aliases.hh>


namespace oln
{

  /*------------------.
  | oln::image1d<T>.  |
  `------------------*/

  template <typename T> class image1d;

  /// fwd_piter vtype of image1d.
  template <typename T>
  struct single_vtype< image1d<T>, typedef_::fwd_piter_type >
  {
    typedef fwd_piter1d ret;
  };

  /// bkd_piter vtype of image1d.
  template <typename T>
  struct single_vtype< image1d<T>, typedef_::bkd_piter_type >
  {
    typedef bkd_piter1d ret;
  };


  /// fwd_qiter vtype of image1d.
  template <typename T>
  struct single_vtype< image1d<T>, typedef_::fwd_qiter_type >
  {
    typedef fwd_qiter1d ret;
  };

  /// bkd_qiter vtype of image1d.
  template <typename T>
  struct single_vtype< image1d<T>, typedef_::bkd_qiter_type >
  {
    typedef bkd_qiter1d ret;
  };


  /*------------------.
  | oln::image2d<T>.  |
  `------------------*/

  template <typename T> class image2d;

  /// fwd_piter vtype of image2d.
  template <typename T>
  struct single_vtype< image2d<T>, typedef_::fwd_piter_type >
  {
    typedef fwd_piter2d ret;
  };

  /// bkd_piter vtype of image2d.
  template <typename T>
  struct single_vtype< image2d<T>, typedef_::bkd_piter_type >
  {
    typedef bkd_piter2d ret;
  };


  /// fwd_qiter vtype of image2d.
  template <typename T>
  struct single_vtype< image2d<T>, typedef_::fwd_qiter_type >
  {
    typedef fwd_qiter2d ret;
  };

  /// bkd_qiter vtype of image2d.
  template <typename T>
  struct single_vtype< image2d<T>, typedef_::bkd_qiter_type >
  {
    typedef bkd_qiter2d ret;
  };


  /*------------------.
  | oln::image3d<T>.  |
  `------------------*/

  template <typename T> class image3d;

  /// fwd_piter vtype of image3d.
  template <typename T>
  struct single_vtype< image3d<T>, typedef_::fwd_piter_type >
  {
    typedef fwd_piter3d ret;
  };

  /// bkd_piter vtype of image3d.
  template <typename T>
  struct single_vtype< image3d<T>, typedef_::bkd_piter_type >
  {
    typedef bkd_piter3d ret;
  };


  /// fwd_qiter vtype of image3d.
  template <typename T>
  struct single_vtype< image3d<T>, typedef_::fwd_qiter_type >
  {
    typedef fwd_qiter3d ret;
  };

  /// bkd_qiter vtype of image3d.
  template <typename T>
  struct single_vtype< image3d<T>, typedef_::bkd_qiter_type >
  {
    typedef bkd_qiter3d ret;
  };


  /*--------------------------------------------.
  | oln::morpher::add_isubset<Image, Isubset>.  |
  `--------------------------------------------*/

  namespace morpher {
    template <typename Image, typename Isubset> struct add_isubset;
  }
  template <typename piter_t, typename isubset_t> class piter_isubset_;


  template <typename Image, typename Isubset>
  struct single_vtype< morpher::add_isubset<Image, Isubset>, typedef_::fwd_piter_type >
  {
    typedef piter_isubset_<oln_type_of(Image, fwd_piter), Isubset> ret;
  };
  
  template <typename Image, typename Isubset>
  struct single_vtype< morpher::add_isubset<Image, Isubset>, typedef_::bkd_piter_type >
  {
    typedef piter_isubset_<oln_type_of(Image, bkd_piter), Isubset> ret;
  };


  template <typename Image, typename Isubset>
  struct single_vtype< morpher::add_isubset<Image, Isubset>, typedef_::fwd_qiter_type >
  {
    typedef piter_isubset_<oln_type_of(Image, fwd_qiter), Isubset> ret;
  };
  
  template <typename Image, typename Isubset>
  struct single_vtype< morpher::add_isubset<Image, Isubset>, typedef_::bkd_qiter_type >
  {
    typedef piter_isubset_<oln_type_of(Image, bkd_qiter), Isubset> ret;
  };


  template <typename Image, typename Isubset>
  struct single_vtype< morpher::add_isubset<Image, Isubset>, typedef_::fwd_niter_type >
  {
    typedef piter_isubset_<oln_type_of(Image, fwd_niter), Isubset> ret;
  };
  
  template <typename Image, typename Isubset>
  struct single_vtype< morpher::add_isubset<Image, Isubset>, typedef_::bkd_niter_type >
  {
    typedef piter_isubset_<oln_type_of(Image, bkd_niter), Isubset> ret;
  };


  /*-------------------------------------------------.
  | oln::morpher::add_neighborhood<Image, Isubset>.  |
  `-------------------------------------------------*/

  namespace morpher {
    template <typename Image, typename Neighb> struct add_neighborhood;
  }
  template <typename point_t> class fwd_niter_neighb_;
  
  template <typename Image, typename Neighb>
  struct single_vtype< morpher::add_neighborhood<Image, Neighb>, typedef_::fwd_niter_type >
  {
  private:
    typedef oln_type_of(Image, point) point_t;
  public:
    typedef fwd_niter_neighb_<point_t> ret;
  };

  template <typename Image, typename Neighb>
  struct single_vtype< morpher::add_neighborhood<Image, Neighb>, typedef_::bkd_niter_type >
  {
  private:
    typedef oln_type_of(Image, point) point_t;
  public:
    typedef fwd_niter_neighb_<point_t> ret;  // FIXME: should be bkd but not yet impled!
  };


} // end of namespace oln


#endif // ! OLN_CORE_FWD_PITER_HH
