// Copyright (C) 2001, 2002, 2003, 2004 EPITA Research and
// Laboratory
//
// This file is part of the Milena Library.  This library is free
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
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
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

#ifndef MLN_TOPO_DMAP_HH
# define MLN_TOPO_DMAP_HH

//  include <ntg/bin.hh>
//  include <mln/basics2d.hh>
# include <mln/core/w_window2d.hh>

// \todo FIXME: math.h should be included by ntg/config/system.hh
//  include <math.h>
//  include <utility>

/*! \file milenq/mln/topo/dmap.hh
**
** REF: B.J.H. Verwer, Local distances for distance transformations
** in two and three dimensions, Pattern Recognition Letters 12 (1991) 671-682
*/

namespace oln {

  /// \brief Topological algorithms.
  namespace topo {
    /*! Chamfer mask.
    **
    ** The Chamfer mask is a weighted masks that provides  an approximation
    ** of the real Euclidean distance in the discrete space.
    */
    template<class T>
    struct chamfer
    {
      chamfer(const w_window2d<T>& win,
	      float coef);

      coord
      delta() const;

      const w_window2d<T> win;
      const float coef;
    };

    /*! Produce a chamfer mask 3x3
    **
    ** \todo FIXME: This highly not thread safe !
    **
    ** \verbatim
    ** Example of oln::topo::mk_chamfer_3x3<1,2>(3) w;
    ** w.delta(): 1
    ** w.coef: 3
    ** w.win:
    **  2  1  2
    **  1  .  .
    **  .  .  .
    ** \endverbatim
    */
    template<int d10, int d11> inline
    const chamfer<int>&
    mk_chamfer_3x3(float coef = 1.f);

    /*! Produce a chamfer mask 3x3
    **
    ** \todo FIXME: This highly not thread safe !
    **
    ** \verbatim
    ** Example of oln::topo::mk_chamfer_3x3(1.5, 2.5) w;
    ** w.delta(): 1
    ** w.coef: 1
    ** w.win:
    **  2.5  1.5  2.5
    **  1.5   .    .
    **   .    .    .
    ** \endverbatim
    */
    inline
    const chamfer<float>&
    mk_chamfer_3x3(float d10, float d11);

    /*! Chamfer 5x5
    **
    ** \verbatim
    ** Example of oln::topo::mk_chamfer_5x5<1, 2, 3>(4) w;
    ** w.delta(): 2
    ** w.coef: 4
    ** w.win:
    **  .  3  .  3  .
    **  3  2  1  2  3
    **  .  1  .  .  .
    **  .  .  .  .  .
    **  .  .  .  .  .
    ** \endverbatim
    */
    template<int d10, int d11, int d21> inline
    const chamfer<int>&
    mk_chamfer_5x5(float coef = 1.f);

    /*! Chamfer 5x5 using float
    **
    ** \see mk_chamfer_5x5
    */
    inline
    const chamfer<float>&
    mk_chamfer_5x5(float d10, float d11, float d21);

    /*! Chamfer_1_1
    **
    ** \verbatim
    ** Example of oln::topo::chamfer_1_1() w;
    ** w.delta(): 1
    ** w.coef: 0.9003
    ** w.win:
    **  1  1  1
    **  1  .  .
    **  .  .  .
    ** \endverbatim
    */
    inline const chamfer<int>& chamfer_1_1();

    /*! Chamfer_1_2
    **
    ** \verbatim
    ** Example of oln::topo::chamfer_1_2() w;
    ** w.delta(): 1
    ** w.coef: 1.2732
    ** w.win:
    **  2  1  2
    **  1  .  .
    **  .  .  .
    ** \endverbatim
    */
    inline const chamfer<int>& chamfer_1_2();
    /*! Chamfer_2_3
    **
    ** \verbatim
    ** Example of oln::topo::chamfer_2_3() w;
    ** w.delta(): 1
    ** w.coef: 2.1736
    ** w.win:
    **  3  2  3
    **  2  .  .
    **  .  .  .
    ** \endverbatim
    */
    inline const chamfer<int>& chamfer_2_3();
    /*! Chamfer_5_7
    **
    ** \verbatim
    ** Example of oln::topo::chamfer_5_7() w;
    ** w.delta(): 1
    ** w.coef: 5.2474
    ** w.win:
    **  7  5  7
    **  5  .  .
    **  .  .  .
    ** \endverbatim
    */
    inline const chamfer<int>& chamfer_5_7();
    /*! Chamfer_12_17
    **
    ** \verbatim
    ** Example of oln::topo::chamfer_12_17() w;
    ** w.delta(): 1
    ** w.coef: 12.6684
    ** w.win:
    **  17  12  17
    **  12   .   .
    **  .   .   .
    ** \endverbatim
    */
    inline const chamfer<int>& chamfer_12_17();
    /*! Chessboard
    **
    ** \verbatim
    ** Example of oln::topo::chessboard() w;
    ** w.delta(): 1
    ** w.coef: 0.9003
    ** w.win:
    **  1  1  1
    **  1  .  .
    **  .  .  .
    ** \endverbatim
    */
    inline const chamfer<int>& chessboard();
    /*! Cityblock
    **
    ** \verbatim
    ** Example of oln::topo::cityblock() w;
    ** w.delta(): 1
    ** w.coef: 1.2732
    ** w.win:
    **  2  1  2
    **  1  .  .
    **  .  .  .
    ** \endverbatim
    */
    inline const chamfer<int>& cityblock();
    /*! Chamfer_4_6_9
    **
    ** \verbatim
    ** Example of oln::topo::chamfer_4_6_9() w;
    ** w.delta(): 2
    ** w.coef: 4.1203
    ** w.win:
    **  .  9  .  9  .
    **  9  6  4  6  9
    **  .  4  .  .  .
    **  .  .  .  .  .
    **  .  .  .  .  .
    ** \endverbatim
    */
    inline const chamfer<int>& chamfer_4_6_9();
    /*! Chamfer_5_7_11
    **
    ** \verbatim
    ** Example of oln::topo::chamfer_5_7_11() w;
    ** w.delta(): 2
    ** w.coef: 5.0206
    ** w.win:
    **  .  11  .  11  .
    **  11  7  5  7  11
    **  .  5  .  .  .
    **  .  .  .  .  .
    **  .  .  .  .  .
    ** \endverbatim
    */
    inline const chamfer<int>& chamfer_5_7_11();
    /*! Chamfer_9_13_20
    **
    ** \verbatim
    ** Example of oln::topo::chamfer_9_13_20() w;
    ** w.delta(): 2
    ** w.coef: 9.1409
    ** w.win:
    **  .   20  .  20  .
    **  20  13  9  13  20
    **  .   9   .   .  .
    **  .   .   .  .   .
    **  .   .   .  .   .
    ** \endverbatim
    */
    inline const chamfer<int>& chamfer_9_13_20();
    /*! Chamfer_16_23_36
    **
    ** \verbatim
    ** Example of oln::topo::chamfer_16_23_36() w;
    ** w.delta(): 2
    ** w.coef: 16.3351
    ** w.win:
    **  .   36  .   36  .
    **  36  23  16  23  36
    **  .   16  .   .   .
    **  .   .   .   .   .
    **  .   .   .   .   .
    ** \endverbatim
    */
    inline const chamfer<int>& chamfer_16_23_36();
    /*! Best set 3x3
    **
    ** \verbatim
    ** Example of oln::topo::best_set_3x3() w;
    ** w.delta(): 1
    ** w.coef: 1
    ** w.win:
    **  1.3408  0.9481  1.3408
    **  0.9481  .  .
    **  .  .  .
    ** \endverbatim
    */
    inline const chamfer<float>& best_set_3x3();
    /*! Best set 5x5
    **
    ** \verbatim
    ** Example of oln::topo::best_set_5x5() w;
    ** w.delta(): 2
    ** w.coef: 1
    ** w.win:
    **  .  2.2044  .  2.2044  .
    **  2.2044  1.406  0.9801  1.406  2.2044
    **  .  0.9801  .  .  .
    **  .  .  .  .  .
    **  .  .  .  .  .
    ** \endverbatim
    */
    inline const chamfer<float>& best_set_5x5();

    // maximum absolute error for integer local distances (Table 2)
    /* Maximum absolute error for integer local distances for chamfer_1_1.
    **
    ** \verbatim
    ** oln::topo::mchamfer_1_1() =
    ** w.delta(): 1
    ** w.coef: 0.8536
    ** w.win:
    **  1  1  1
    **  1  .  .
    **  .  .  .
    ** \endverbatim
    */
    inline const chamfer<int>& mchamfer_1_1();
    /* Maximum absolute error for integer local distances for chamfer_1_2.
    **
    ** \verbatim
    ** oln::topo::mchamfer_1_2() =
    ** w.delta(): 1
    ** w.coef: 1.2071
    ** w.win:
    **  2  1  2
    **  1  .  .
    **  .  .  .
    ** \endverbatim
    */
    inline const chamfer<int>& mchamfer_1_2();
    /* Maximum absolute error for integer local distances for chamfer_2_3.
    **
    ** \verbatim
    ** oln::topo::mchamfer_2_3() =
    ** w.delta(): 1
    ** w.coef: 2.118
    ** w.win:
    **  3  2  3
    **  2  .  .
    **  .  .  .
    ** \endverbatim
    */
    inline const chamfer<int>& mchamfer_2_3();
    /* Maximum absolute error for integer local distances for chamfer_5_7.
    **
    ** \verbatim
    ** oln::topo::mchamfer_5_7() =
    ** w.delta(): 1
    ** w.coef: 5.1675
    ** w.win:
    **  7  5  7
    **  5  .  .
    **  .  .  .
    ** \endverbatim
    */
    inline const chamfer<int>& mchamfer_5_7();
    /* Maximum absolute error for integer local distances for chamfer_12_17.
    **
    ** \verbatim
    ** oln::topo::mchamfer_12_17() =
    ** w.delta(): 1
    ** w.coef: 12.5
    ** w.win:
    **  17  12  17
    **  12  .   .
    **  .   .   .
    ** \endverbatim
    */
    inline const chamfer<int>& mchamfer_12_17();
    /* Maximum absolute error for integer local distances for chessboard.
    **
    ** \verbatim
    ** oln::topo::mchessboard() =
    ** w.delta(): 1
    ** w.coef: 0.8536
    ** w.win:
    **  1  1  1
    **  1  .  .
    **  .  .  .
    ** \endverbatim
    */
    inline const chamfer<int>& mchessboard();
    /* Maximum absolute error for integer local distances for cityblock.
    **
    ** \verbatim
    ** oln::topo::mcityblock() =
    ** w.delta(): 1
    ** w.coef: 1.2071
    ** w.win:
    **  2  1  2
    **  1  .  .
    **  .  .  .
    ** \endverbatim
    */
    inline const chamfer<int>& mcityblock();
    /* Maximum absolute error for integer local distances for chamfer_4_6_9.
    **
    ** \verbatim
    ** oln::topo::mchamfer_4_6_9() =
    ** w.delta(): 2
    ** w.coef: 4.1213
    ** w.win:
    **  .  9  .  9  .
    **  9  6  4  6  9
    **  .  4  .  .  .
    **  .  .  .  .  .
    **  .  .  .  .  .
    ** \endverbatim
    */
    inline const chamfer<int>& mchamfer_4_6_9();
    /* Maximum absolute error for integer local distances for chamfer_5_7_11.
    **
    ** \verbatim
    ** oln::topo::mchamfer_5_7_11() =
    ** w.delta(): 2
    ** w.coef: 5.0092
    ** w.win:
    **  .   11  .   11   .
    **  11  7   5   7   11
    **  .   5   .   .   .
    **  .   .   .   .   .
    **  .   .   .   .   .
    ** \endverbatim
    */
    inline const chamfer<int>& mchamfer_5_7_11();
    /* Maximum absolute error for integer local distances for chamfer_9_13_20.
    **
    ** \verbatim
    ** oln::topo::mchamfer_9_13_20() =
    ** w.delta(): 2
    ** w.coef: 9.0819
    ** w.win:
    **  .   20  .  20  .
    **  20  13  9  13  20
    **  .   9   .  .   .
    **  .   .   .  .   .
    **  .   .   .  .   .
    ** \endverbatim
    */
    inline const chamfer<int>& mchamfer_9_13_20();
    /* Maximum absolute error for integer local distances for chamfer_17_24_38.
    **
    ** \verbatim
    ** oln::topo::mchamfer_17_24_38() =
    ** w.delta(): 2
    ** w.coef: 17.2174
    ** w.win:
    **  .   38  .   38  .
    **  38  24  17  24  38
    **  .   17  .   .   .
    **  .   .   .   .   .
    **  .   .   .   .   .
    ** \endverbatim
    */
    inline const chamfer<int>& mchamfer_17_24_38();
    /* Maximum absolute error for integer local distances for best_set_3x3.
    **
    ** \verbatim
    ** oln::topo:mbest_set_3x3() =
    ** w.delta(): 1
    ** w.coef: 1
    ** w.win:
    **  1.35825  0.96043  1.35825
    **  0.96043  .  .
    **  .  .  .
    ** \endverbatim
    */
    inline const chamfer<float>& mbest_set_3x3();
    /* Maximum absolute error for integer local distances for best_set_5x5.
    **
    ** \verbatim
    ** oln::topo:mbest_set_5x5() =
    ** w.delta(): 2
    ** w.coef: 1
    ** w.win:
    **  .  2.20585  .  2.20585  .
    **  2.20585  1.3951  0.986485  1.3951  2.20585
    **  .  0.986485  .  .  .
    **  .  .  .  .  .
    **  .  .  .  .  .
    ** \endverbatim
    */
    inline const chamfer<float>& mbest_set_5x5();

    /*! Distance map
    **
    ** \param T Type of the distance.
    ** \param T2 Type of the chamfer distance.
    **
    ** \note Do not forget to call \a compute.
    **
    ** \code
    ** #include <oln/basics2d.hh>
    ** #include <oln/topo/dmap.hh>
    ** #include <oln/convert/stretch.hh>
    **
    ** int main()
    ** {
    **   oln::image2d<ntg::bin> in = oln::load(IMG_IN "face_se.pbm");
    **
    **   oln::topo::dmap<ntg::int_u<16>, int> m(in.size(), oln::topo::chessboard());
    **   m.compute(in);
    **   save(oln::convert::stretch_balance<ntg::int_u8>(m.imap()),
    **        IMG_OUT "oln_topo_dmap.pgm");
    ** }
    ** \endcode
    ** \image html face_se_pbm.png
    ** \image latex face_se_pbm.png
    ** =>
    ** \image html oln_topo_dmap.png
    ** \image latex oln_topo_dmap.png
    */
    template<class T, class T2>
    class dmap
    {
    public:
      typedef image2d<ntg::bin>::point_type point_type;

      /*! Constructor.
      **
      ** \param size Size of the image on which the dmap will be compute.
      ** \param ch Chamfer distance used.
      */
      dmap(const image2d_size& size, const chamfer<T2>& ch);

      /// Compute the distance map.
      template <class V>
      void
      compute(const image2d<V>& input, float infty = 0.f);

      /// Compute the distance map.
      template <class V>
      void
      compute(const image2d<V>&	input,
	      image2d<point2d>& nearest_point_map,
	      float             infty = 0.f);

      /// Return the distance map of type T.
      const image2d<T>&
      imap() const;

      /// Return the distance map divided by the Chamfer coefficient.
      image2d<float>
      to_image() const;

      /// Distance of a point p.
      const T
      operator[](const point_type& p) const;

      /// Distance of a point2d(row, col).
      const T
      operator()(coord row, coord col) const;

    private:
      image2d<T>	imap_;
      chamfer<T2>	ch_;
      float		inFty_;
      T		       	infTy_;
    };
    /// Distance map using the Euclidean distance.
    template <class I>
    image2d<float> exact_dmap(const abstract::image<I>& input);

  } // end of namespace topo

} // end of namespace oln

# include <oln/topo/dmap.hxx>

#endif // OLENA_TOPO_DMAP_HH
