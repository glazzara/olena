// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_UTIL_ADJACENCY_MATRIX_HH
# define MLN_UTIL_ADJACENCY_MATRIX_HH

/// \file
///
/// \brief A class of adjacency matrix.
///
/// \todo the underlying data structure is chosen according
/// to the value type quantification however we may like to make it
/// depend on the number of elements.
///
/// \todo Have theo re-read this file.

# include <mln/core/image/image2d.hh>
# include <mln/util/set.hh>
# include <mln/util/ord_pair.hh>
# include <mln/trait/value_.hh>
# include <mln/metal/converts_to.hh>
# include <mln/debug/println.hh>


namespace mln
{

  namespace util
  {

    namespace internal
    {

      // Implementation for low quantification.

      template <typename V, typename Q>
      struct adjacency_matrix_impl_selector
      {
	/// Data structure used to store adjacency information.
	typedef image2d<bool> adj_t;

	/// Constructor.
	adjacency_matrix_impl_selector(const V& nelements);

	/// Make \p e1 and \p e2 adjacent.
	void add(const V& e1, const V& e2);

	/// Remove adjacency between \p e1 and \p e2.
	void remove(const V& e1, const V& e2);

	/// Clear all adjacencies.
	void clear();

	/// Check whether \p e1 and \p e2 are adjacent.
	bool are_adjacent(const V& e1, const V& e2) const;

	/// Print data to std::out.
	std::ostream& print_data_(std::ostream& ostr) const;

      protected:
	adj_t adj_;
      };


      // Implementation for high quantification.

      template <typename V>
      struct adjacency_matrix_impl_selector<V, metal::bool_<false> >
      {
	/// Data structure used to store adjacency information.
	typedef util::set< util::ord_pair<V> > adj_t;

	/// Constructor.
	adjacency_matrix_impl_selector(const V& nelements);

	/// Make \p e1 and \p e2 adjacent.
	void add(const V& e1, const V& e2);

	/// Remove adjacency between \p e1 and \p e2.
	void remove(const V& e1, const V& e2);

	/// Clear all adjacencies.
	void clear();

	/// Check whether \p e1 and \p e2 are adjacent.
	bool are_adjacent(const V& e1, const V& e2) const;

	/// Print data to std::out.
	std::ostream& print_data_(std::ostream& ostr) const;

      protected:
	adj_t adj_;

# ifndef NDEBUG
	unsigned nelements_;
# endif // ! NDEBUG
      };


    } // end of namespace mln::util::internal


    /// A class of adjacency matrix.
    ///
    /// Support low and high quantification value types.
    /// In case of low quantification value type, it uses
    /// an image2d to store adjacency information.
    /// In case of high quantification value type, it uses
    /// a util::set to store the adjacency information.
    ///
    /// \ingroup modutil
    //
    template <typename V = def::coord>
    class adjacency_matrix
      : private mlc_converts_to(V,unsigned)::check_t,
	public internal::adjacency_matrix_impl_selector<V, typename mlc_equal(mln_trait_value_quant(V),trait::value::quant::low)::eval>
    {
      typedef internal::adjacency_matrix_impl_selector<V, typename mlc_equal(mln_trait_value_quant(V),trait::value::quant::low)::eval>
	      impl_t;

      typedef typename impl_t::adj_t adj_t;

    public:
      /// \name Constructors.
      /// \{

      /// Default constructor.
      adjacency_matrix();

      /// Construct an adjacency matrix with \p nelements elements
      /// maximum.
      adjacency_matrix(const V& nelements);

      /// \}

      /// Hook member used to retrieve the underlying data structure.
      const adj_t& hook_data_() const;
    };


    // <<

    template <typename V>
    std::ostream&
    operator<<(std::ostream& ostr, const adjacency_matrix<V>& adj);



# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {

      // Low quantification.

      template <typename V, typename Q>
      adjacency_matrix_impl_selector<V, Q>::adjacency_matrix_impl_selector(const V& nelements)
	: adj_(nelements, nelements)
      {
	clear();
      }

      template <typename V, typename Q>
      void
      adjacency_matrix_impl_selector<V, Q>::add(const V& e1, const V& e2)
      {
	mln_precondition(adj_.is_valid());
	mln_precondition(e1 < adj_.nrows());
	mln_precondition(e2 < adj_.nrows());

	if (e1 > e2)
	  opt::at(adj_, e2, e1) = true;
	else
	  opt::at(adj_, e1, e2) = true;
      }

      template <typename V, typename Q>
      void
      adjacency_matrix_impl_selector<V, Q>::remove(const V& e1, const V& e2)
      {
	mln_precondition(adj_.is_valid());
	mln_precondition(e1 < adj_.nrows());
	mln_precondition(e2 < adj_.nrows());

	if (e1 > e2)
	  opt::at(adj_, e2, e1) = false;
	else
	  opt::at(adj_, e1, e2) = false;
      }

      template <typename V, typename Q>
      void
      adjacency_matrix_impl_selector<V, Q>::clear()
      {
	mln_precondition(adj_.is_valid());
	data::fill(adj_, false);
      }

      template <typename V, typename Q>
      bool
      adjacency_matrix_impl_selector<V, Q>::are_adjacent(const V& e1,
							 const V& e2) const
      {
	mln_precondition(adj_.is_valid());
	mln_precondition(e1 < adj_.nrows());
	mln_precondition(e2 < adj_.nrows());

	if (e1 > e2)
	  return opt::at(adj_, e2, e1);
	return opt::at(adj_, e1, e2);
      }

      template <typename V, typename Q>
      std::ostream&
      adjacency_matrix_impl_selector<V, Q>::print_data_(std::ostream& ostr) const
      {
	mln_precondition(adj_.is_valid());
	debug::println(adj_);
	return ostr;
      }




      // High quantification.

      template <typename V>
      adjacency_matrix_impl_selector<V, metal::bool_<false> >
	::adjacency_matrix_impl_selector(const V& nelements)
      {
	(void) nelements;
# ifndef NDEBUG
	nelements_ = nelements;
# endif // ! NDEBUG
      }

      template <typename V>
      void
      adjacency_matrix_impl_selector<V, metal::bool_<false> >
	::add(const V& e1, const V& e2)
      {
	mln_precondition(int(e1) < int(nelements_));
	mln_precondition(int(e2) < int(nelements_));
	adj_.insert(make::ord_pair(e1, e2));
      }

      template <typename V>
      void
      adjacency_matrix_impl_selector<V, metal::bool_<false> >
	::remove(const V& e1, const V& e2)
      {
	mln_precondition(int(e1) < int(nelements_));
	mln_precondition(int(e2) < int(nelements_));
	mln_precondition(adj_.has(make::ord_pair(e1, e2)));
	adj_.remove(make::ord_pair(e1, e2));
      }

      template <typename V>
      void
      adjacency_matrix_impl_selector<V, metal::bool_<false> >::clear()
      {
	adj_.clear();
      }

      template <typename V>
      bool
      adjacency_matrix_impl_selector<V, metal::bool_<false> >
	::are_adjacent(const V& e1, const V& e2) const
      {
	mln_precondition(int(e1) < int(nelements_));
	mln_precondition(int(e2) < int(nelements_));
	return adj_.has(make::ord_pair(e1, e2));
      }

      template <typename V>
      std::ostream&
      adjacency_matrix_impl_selector<V, metal::bool_<false> >::print_data_(std::ostream& ostr) const
      {
	return ostr << adj_;
      }

    } // end of namespace mln::internal


    template <typename V>
    adjacency_matrix<V>::adjacency_matrix()
      : impl_t()
    {
    }


    template <typename V>
    adjacency_matrix<V>::adjacency_matrix(const V& nelements)
      : impl_t(nelements)
    {
    }


    template <typename V>
    std::ostream&
    operator<<(std::ostream& ostr, const adjacency_matrix<V>& adj)
    {
      return adj.print_data_(ostr);
    }


# endif // ! MLN_UTIL_ADJACENCY_MATRIX_HH

  } // end of namespace mln::util

} // end of namespace mln


#endif // ! MLN_UTIL_ADJACENCY_MATRIX_HH
