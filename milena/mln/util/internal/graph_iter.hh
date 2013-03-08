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

#ifndef MLN_UTIL_INTERNAL_GRAPH_ITER_HH
# define MLN_UTIL_INTERNAL_GRAPH_ITER_HH

/// \file
///
/// Implementation for graph iterators.

# include <mln/util/internal/graph_iter_base.hh>
# include <mln/util/vertex.hh>
# include <mln/util/edge.hh>



namespace mln
{

  namespace internal
  {

    /*!
      \internal
      \brief Forward vertex iterator.
    */
    template<typename G>
    class vertex_fwd_iterator
      : public graph_iter_base<G, util::vertex<G>, vertex_fwd_iterator<G> >
    {
      typedef graph_iter_base<G, util::vertex<G>, vertex_fwd_iterator<G> > super_;

      public:
	/// Constructors.
	/// \{
	vertex_fwd_iterator();
	vertex_fwd_iterator(const G& g);
	/// \}

      protected:
	/// Returns the id of the first element.
	/// Called in start();
	util::vertex_id_t start_id_() const;

	/// Returns the next element id.
	/// Called in next();
	util::vertex_id_t next_id_() const;

	friend class graph_iter_base<G, util::vertex<G>, vertex_fwd_iterator<G> >;
    };


    /*!
      \internal
      \brief Backward vertex iterator.
    */
    template<typename G>
    class vertex_bkd_iterator
      : public graph_iter_base<G, util::vertex<G>, vertex_bkd_iterator<G> >
    {
      typedef graph_iter_base<G, util::vertex<G>, vertex_bkd_iterator<G> > super_;

      public:
	/// Constructors.
	/// \{
	vertex_bkd_iterator();
	vertex_bkd_iterator(const G& g);
	/// \}

      protected:
	/// Returns the id of the first element.
	/// Called in start();
	util::vertex_id_t start_id_() const;

	/// Returns the next element id.
	/// Called in next();
	util::vertex_id_t next_id_() const;

	friend class graph_iter_base<G, util::vertex<G>, vertex_bkd_iterator<G> >;
    };


    /*!
      \internal
      \brief Forward edge iterator.
    */
    template <typename G>
    class edge_fwd_iterator
      : public graph_iter_base<G, util::edge<G>, edge_fwd_iterator<G> >
    {
      typedef graph_iter_base<G, util::edge<G>, edge_fwd_iterator<G> > super_;

      public:
	/// Constructors.
	/// \{
	edge_fwd_iterator();
	edge_fwd_iterator(const G& g);
	/// \}

      protected:
	/// Returns the id of the first element.
	/// Called in start();
	util::edge_id_t start_id_() const;

	/// Returns the next element id.
	/// Called in next();
	util::edge_id_t next_id_() const;

	friend class graph_iter_base<G, util::edge<G>, edge_fwd_iterator<G> >;
    };


    /*!
      \internal
      \brief Backward edge iterator.
    */
    template <typename G>
    class edge_bkd_iterator
      : public graph_iter_base<G, util::edge<G>, edge_bkd_iterator<G> >
    {
      typedef graph_iter_base<G, util::edge<G>, edge_bkd_iterator<G> > super_;

      public:
	/// Constructors.
	/// \{
	edge_bkd_iterator();
	edge_bkd_iterator(const G& g);
	/// \}

      protected:
	/// Returns the id of the first element.
	/// Called in start();
	util::edge_id_t start_id_() const;

	/// Returns the next element id.
	/// Called in next();
	util::edge_id_t next_id_() const;

	friend class graph_iter_base<G, util::edge<G>, edge_bkd_iterator<G> >;
    };




# ifndef MLN_INCLUDE_ONLY


    /*--------------------`
    | vertex_fwd_iterator |
    \--------------------*/

    template <typename G>
    inline
    vertex_fwd_iterator<G>::vertex_fwd_iterator()
    {
    }

    template <typename G>
    inline
    vertex_fwd_iterator<G>::vertex_fwd_iterator(const G& g)
      : super_(g)
    {
    }

    template <typename G>
    inline
    util::vertex_id_t
    vertex_fwd_iterator<G>::start_id_() const
    {
      return 0;
    }

    template <typename G>
    inline
    util::vertex_id_t
    vertex_fwd_iterator<G>::next_id_() const
    {
      return this->p_.id().value() + 1;
    }



    /*--------------------`
    | vertex_bkd_iterator |
    \--------------------*/

    template <typename G>
    inline
    vertex_bkd_iterator<G>::vertex_bkd_iterator()
    {
    }

    template <typename G>
    inline
    vertex_bkd_iterator<G>::vertex_bkd_iterator(const G& g)
      : super_(g)
    {
    }

    template <typename G>
    inline
    util::vertex_id_t
    vertex_bkd_iterator<G>::start_id_() const
    {
      return this->p_.graph().v_nmax() - 1;
    }

    template <typename G>
    inline
    util::vertex_id_t
    vertex_bkd_iterator<G>::next_id_() const
    {
      return this->p_.id().value() - 1;
    }



    /*------------------`
    | edge_fwd_iterator |
    \------------------*/

    template <typename G>
    inline
    edge_fwd_iterator<G>::edge_fwd_iterator()
    {
    }

    template <typename G>
    inline
    edge_fwd_iterator<G>::edge_fwd_iterator(const G& g)
      : super_(g)
    {
    }

    template <typename G>
    inline
    util::edge_id_t
    edge_fwd_iterator<G>::start_id_() const
    {
      return 0;
    }

    template <typename G>
    inline
    util::edge_id_t
    edge_fwd_iterator<G>::next_id_() const
    {
      return this->p_.id().value() + 1;
    }



    /*------------------`
    | edge_bkd_iterator |
    \------------------*/

    template <typename G>
    inline
    edge_bkd_iterator<G>::edge_bkd_iterator()
    {
    }

    template <typename G>
    inline
    edge_bkd_iterator<G>::edge_bkd_iterator(const G& g)
      : super_(g)
    {
    }

    template <typename G>
    inline
    util::edge_id_t
    edge_bkd_iterator<G>::start_id_() const
    {
      return this->p_.graph().e_nmax() - 1;
    }

    template <typename G>
    inline
    util::edge_id_t
    edge_bkd_iterator<G>::next_id_() const
    {
      return this->p_.id().value() - 1;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_UTIL_INTERNAL_GRAPH_ITER_HH
