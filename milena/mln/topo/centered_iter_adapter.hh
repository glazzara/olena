// Copyright (C) 2008, 2009, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_TOPO_CENTERED_ITER_ADAPTER_HH
# define MLN_TOPO_CENTERED_ITER_ADAPTER_HH

/// \file
/// \brief Complex relative iterator adapters adding the central
/// (reference) point to the set of iterated faces.

# include <mln/topo/internal/complex_relative_iterator_sequence.hh>
# include <mln/topo/center_only_iter.hh>

# include <mln/topo/face.hh>


namespace mln
{

  namespace topo
  {

    /*----------------------------------------.
    | topo::centered_fwd_iter_adapter<D, I>.  |
    `----------------------------------------*/

    // FIXME: We should deduce D from I.

    /*!
      \internal
      \brief Backward complex relative iterator adapters adding the
      central (reference) point to the set of iterated faces.

      \tparam D The dimension of the complex this iterator belongs to.
      \tparam I The adapated complex relative iterator.
    */
    template <unsigned D, typename I>
    class centered_fwd_iter_adapter
      : public internal::complex_relative_iterator_sequence< center_only_iter<D>,
							     I,
							     centered_fwd_iter_adapter<D, I> >
    {
      typedef centered_fwd_iter_adapter<D, I> self_;
      typedef center_only_iter<D> iter1_;
      typedef I iter2_;
      typedef internal::complex_relative_iterator_sequence< iter1_,
							    iter2_,
							    self_ > super_;

    public:
      /// Construction.
      /// \{
      centered_fwd_iter_adapter();
      template <typename Fref>
      centered_fwd_iter_adapter(const Fref& f_ref);
      /// \}
    };


    /*----------------------------------------.
    | topo::centered_bkd_iter_adapter<D, I>.  |
    `----------------------------------------*/

    // FIXME: We should deduce D from I.

    /*!
      \internal
      \brief Forward complex relative iterator adapters adding the
      central (reference) point to the set of iterated faces.

      \tparam D The dimension of the complex this iterator belongs to.
      \tparam I The adapated complex relative iterator.
    */
    template <unsigned D, typename I>
    class centered_bkd_iter_adapter
      : public internal::complex_relative_iterator_sequence< I,
							     center_only_iter<D>,
							     centered_bkd_iter_adapter<D, I> >
    {
      typedef centered_bkd_iter_adapter<D, I> self_;
      typedef I iter1_;
      typedef center_only_iter<D> iter2_;
      typedef internal::complex_relative_iterator_sequence< iter1_,
							    iter2_,
							    self_ > super_;

    public:
      /// Construction.
      /// \{
      centered_bkd_iter_adapter();
      template <typename Fref>
      centered_bkd_iter_adapter(const Fref& f_ref);
      /// \}
    };



# ifndef MLN_INCLUDE_ONLY

    /*----------------------------------------.
    | topo::centered_fwd_iter_adapter<D, I>.  |
    `----------------------------------------*/

    template <unsigned D, typename I>
    inline
    centered_fwd_iter_adapter<D, I>::centered_fwd_iter_adapter()
    {
    }

    template <unsigned D, typename I>
    template <typename Fref>
    inline
    centered_fwd_iter_adapter<D, I>::centered_fwd_iter_adapter(const Fref& f_ref)
      : super_(f_ref)
    {
    }


    /*----------------------------------------.
    | topo::centered_bkd_iter_adapter<D, I>.  |
    `----------------------------------------*/

    template <unsigned D, typename I>
    inline
    centered_bkd_iter_adapter<D, I>::centered_bkd_iter_adapter()
    {
    }

    template <unsigned D, typename I>
    template <typename Fref>
    inline
    centered_bkd_iter_adapter<D, I>::centered_bkd_iter_adapter(const Fref& f_ref)
      : super_(f_ref)
    {
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_CENTERED_ITER_ADAPTER_HH
