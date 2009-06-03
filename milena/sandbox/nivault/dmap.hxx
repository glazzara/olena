// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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

#ifndef MLN_TOPO_DMAP_HXX
# define MLN_TOPO_DMAP_HXX

# include <mlc/array/2d.hh>
# include <ntg/float.hh>
# include <oln/arith/ops.hh>
# include <mln/geom/sym.hh>

namespace mln
{

  namespace topo
  {

//     using mlc::lbrk;
//     using mlc::end;
//     using mlc::x;

    template <class T>
    chamfer<T>::chamfer(const w_window2d<T>& win,
			float		   coef) :
      win(win),
      coef(coef)
    {}

    template <class T>
    coord
    chamfer<T>::delta() const
    {
      coord d = win.delta();
      return d;
    }

    // mk_chamfer...
    // FIXME: this is highly not thread safe !
    template<int d10, int d11> inline
    const chamfer<int>&
    mk_chamfer_3x3(float coef)
    {
      static const w_window2d<int> w_win = ( mlc::ints_2d =
					     d11,      d10, d11, mlc::lbrk,
					     d10, mlc::x(),   0, end );
      static const chamfer<int> ch_ = chamfer<int>(w_win, geom::sym(w_win), coef);
      return ch_;
    }

    inline // FIXME: how to define float by parameters?
    const chamfer<float>&
    mk_chamfer_3x3(float d10, float d11)
      // FIXME: add (?)  , float coef = 1.f
    {
      static const w_window2d<float> w_win = ( mlc::floats_2d =
						 d11, d10, d11, mlc::lbrk,
						 d10, mlc::x(), 0.f, end );
      static const chamfer<float> ch_ =
	chamfer<float>(w_win, geom::sym(w_win), 1.f);
      return ch_;
    }

    template<int d10, int d11, int d21> inline
    const chamfer<int>&
    mk_chamfer_5x5(float coef)
    {
      static const w_window2d<int> w_win = ( mlc::ints_2d =
					     0, d21,        0, d21,   0, mlc::lbrk,
					     d21, d11,      d10, d11, d21,
					     0, d10, mlc::x(),   0,   0, end );
      static const chamfer<int> ch_ = chamfer<int>(w_win, geom::sym(w_win), coef);
      return ch_;
    }

    inline
    const chamfer<float>&
    mk_chamfer_5x5(float d10, float d11, float d21)
    {
      const float O = 0.f;
      static const w_window2d<float> w_win = ( mlc::floats_2d =
						 O,   d21,        O, d21,   O, mlc::lbrk,
						 d21, d11,      d10, d11, d21,
						 O,   d10, mlc::x(),   O,   O, end );
      static const chamfer<float> ch_ =
	chamfer<float>(w_win, geom::sym(w_win), 1.f);
      return ch_;
    }

    // REF: B.J.H. Verwer, Local distances for distance transformations
    // in two and three dimensions, Pattern Recognition Letters 12 (1991) 671-682

    // unbiased minimal mean square error for integer local distances (Table 5)
# define oln_topo_chamfer2_(Name, I, J, D, E) \
    inline const chamfer<int>& Name##_##I##_##J()		\
     { 								\
       static const chamfer<int> tmp =                          \
          mk_chamfer_##D##x##D< I, J >(E);   	                \
       return tmp;						\
     }

# define oln_topo_chamfer3_(Name, I, J, K, D, E)		\
    inline const chamfer<int>& Name##_##I##_##J##_##K()		\
    {								\
      static const chamfer<int> tmp =				\
         mk_chamfer_##D##x##D< I, J, K >(E);			\
      return tmp;						\
    }

    oln_topo_chamfer2_(chamfer, 1,   1,  3, 0.9003)
    oln_topo_chamfer2_(chamfer, 1,   2,  3, 1.2732)
    oln_topo_chamfer2_(chamfer,  2,   3,  3, 2.1736)
    oln_topo_chamfer2_(chamfer,  5,   7,  3, 5.2474)
    oln_topo_chamfer2_(chamfer, 12,  17,  3, 12.6684)

    inline const chamfer<int>&
    chessboard()
    {
      return chamfer_1_1();
    }

    inline const chamfer<int>&
    cityblock()
    {
      return chamfer_1_2();
    }

    oln_topo_chamfer3_(chamfer, 4,  6, 9, 5, 4.1203)
    oln_topo_chamfer3_(chamfer, 5,  7, 11, 5, 5.0206)
    oln_topo_chamfer3_(chamfer, 9,  13, 20, 5, 9.1409)
    oln_topo_chamfer3_(chamfer, 16, 23, 36, 5, 16.3351)

    inline const chamfer<float>& best_set_3x3()
    { return mk_chamfer_3x3(0.9481, 1.3408); }
    inline const chamfer<float>& best_set_5x5()
    { return mk_chamfer_5x5(0.9801, 1.4060, 2.2044); }

    // maximum absolute error for integer local distances (Table 2)
    oln_topo_chamfer2_(mchamfer, 1, 1, 3, 0.8536)
    oln_topo_chamfer2_(mchamfer, 1, 2, 3, 1.2071)
    oln_topo_chamfer2_(mchamfer, 2, 3, 3, 2.1180)
    oln_topo_chamfer2_(mchamfer, 5, 7, 3, 5.1675)
    oln_topo_chamfer2_(mchamfer, 12, 17, 3, 12.5000)

    inline const chamfer<int>& mchessboard()    { return mchamfer_1_1(); }
    inline const chamfer<int>& mcityblock()     { return mchamfer_1_2(); }

    oln_topo_chamfer3_(mchamfer, 4,  6,  9, 5, 4.1213)
    oln_topo_chamfer3_(mchamfer, 5,  7, 11, 5, 5.0092)
    oln_topo_chamfer3_(mchamfer, 9, 13, 20, 5, 9.0819)
    oln_topo_chamfer3_(mchamfer, 17, 24, 38, 5, 17.2174)

    inline const chamfer<float>& mbest_set_3x3() {
      const float coef = 1.0412;
      return mk_chamfer_3x3(1/coef, sqrt(2.f)/coef);
    }
    inline const chamfer<float>& mbest_set_5x5() {
      const float coef = 1.0137;
      return mk_chamfer_5x5(1/coef, sqrt(2.f)/coef, sqrt(5.f)/coef);
    }

# undef oln_topo_chamfer2_
# undef oln_topo_chamfer3_

    template <class T, class T2>
    dmap<T, T2>::dmap(const image2d_size&  size,
		      const chamfer<T2>& ch) :
      imap_(size),
      ch_(ch)
    {
      // FIXME: if T is float then precondition(ch.coef == 1.f)
    }

    template <class T, class T2>
    template <class V>
    void
    dmap<T, T2>::compute(const image2d<V>&	input,
			 float			infty)
    {
      image2d<point_type> dummy(input.size());
      compute(input, dummy, infty);
    }

    template <class T, class T2>
    template <class V>
    void dmap<T, T2>::compute(const image2d<V>&     input,
			      image2d<point_type>&  nearest_point_map,
			      float		    infty)
    {
      precondition(input.size() == imap_.size());
      if (infty == 0.f)
	{
	  inFty_ = ntg_max_val(T);
	  infTy_ = ntg_max_val(T);
	}
      else
	{
	  inFty_ = infty;
	  infTy_ = T(infty); // FIXME: use ceil if T is integer!
	}

      // init
      {
	typename image2d<V>::iter_type p(input);
	for (p = begin; p != end; ++p)
	  if (input[p] != ntg_zero_val(V))
	    {
	      imap_[p] = T(0);
	      nearest_point_map[p] = p;
	    }
	  else
	    imap_[p] = infTy_;
	imap_.border_adapt_copy(ch_.delta()); // FIXME: this is geodesic only!
      }

      // win
      {
	typename image2d<V>::win_iter_type p(input);
	for (p = begin; p != end; ++p)
	  {
	    if (imap_[p] == T(0))
	      continue;
	    T min = imap_[p];
	    for (unsigned i = 0; i < ch_.win.card(); ++i)
	      {
		point_type q = p + ch_.win.dp(i);
		if (imap_[q] == infTy_) // FIXME: || imap_[q] >= min
		  continue;
		if (imap_[q] + ch_.win.w(i) < min)
		  {
		    nearest_point_map[p] = nearest_point_map[q];
		    min = imap_[q] + ch_.win.w(i);
		  }
	      }
	    imap_[p] = min;
	  }
      }

      const w_window2d<T2>& bkd = geom::sym(ch_.win);

      // bkd
      {
	typename image2d<V>::bkd_iter_type p(input);
	for_all(p)
	  {
	    if (imap_[p] == T(0))
	      continue;
	    T min = imap_[p];
	    for (unsigned i = 0; i < ch_.bkd.card(); ++i)
	      {
		point_type q = p + ch_.bkd.dp(i);
		if (imap_[q] == infTy_) // FIXME: || imap_[q] >= min
		  continue;
		if (imap_[q] + ch_.bkd.w(i) < min)
		  {
		    nearest_point_map[p] = nearest_point_map[q];
		    min = imap_[q] + ch_.bkd.w(i);
		  }
	      }
	    imap_[p] = min;
	  }
      }
    }

    template <class T, class T2>
    const image2d<T>&
    dmap<T, T2>::imap() const
    {
      return imap_;
    }

    template <class T, class T2>
    image2d<float>
    dmap<T, T2>::to_image() const
    {
      // FIXME: if T is float, call invariant(ch_.coef == 1.f);
      // and then return imap();
      // otherwise: 	return arith::div(imap_, ch_.coef);
      image2d<float> output(imap_.size());
      typename image2d<float>::iter_type p(imap_);
      for_all(p)
	output[p] = (imap_[p] == infTy_ ?
		     inFty_ :
		     imap_[p] / ch_.coef);
      return output;
    }

    template <class T, class T2>
    const T
    dmap<T, T2>::operator[](const point_type& p) const
    {
      return imap_[p] / ch_.coef;
    }

    template <class T, class T2>
    const T
    dmap<T, T2>::operator()(coord row, coord col) const
    {
      return imap_(row, col) / ch_.coef;
    }

    inline float
    euclidian_dist2(const point2d& p1, const point2d& p2)
    {
      float dr = p1.row() - p2.row();
      float dc = p1.col() - p2.col();
      return dr * dr + dc * dc;
    }

    // FIXME: why abstract::image!

    template <class I>
    image2d<float>
    exact_dmap(const abstract::image<I>& input)
    {
      image2d<float> output(input.size());
      image2d<float>::fwd_iter_type p(input);
      for_all(p)
	{
	  if (input[p] == true)
	    {
	      output[p] = 0.f;
	      continue;
	    }
	  image2d<float>::fwd_iter_type q(input);
	  bool found = false;
	  float d = 0.f;
	  for_all(q)
	    {
	      if (input[q] == false || q == p)
		continue;
	      if (! found)
		{
		  d = euclidian_dist2(p, q);
		  found = true;
		  continue;
		}
	      d = std::min(euclidian_dist2(p, q), d);
	    }
	  output[p] = sqrt(d);
	}
      return output;
    }

  } // end of namespace topo

} // end of namespace mln

#endif // ! MLN_TOPO_DMAP_HXX
