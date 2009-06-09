// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_UTIL_INTERNAL_GRAPH_NBH_ITER_HH
# define MLN_UTIL_INTERNAL_GRAPH_NBH_ITER_HH

# include <mln/core/concept/proxy.hh>
# include <mln/util/internal/graph_nbh_iter_base.hh>
# include <mln/util/vertex.hh>
# include <mln/util/edge.hh>

/// \file
/// \brief  Implementation for graph vertex iterators centered to a vertex.

namespace mln
{

  namespace internal
  {

    /*-----------------------------`
    | vertex_nbh_vertex_*_iterator |
    \-----------------------------*/

    template <typename G>
    class vertex_nbh_vertex_fwd_iterator
      : public nbh_iterator_base<G,
				 util::vertex<G>,
				 util::vertex<G>,
				 vertex_nbh_vertex_fwd_iterator<G> >
    {
      typedef util::vertex<G> V;
      typedef vertex_nbh_vertex_fwd_iterator<G> self_;
      typedef nbh_iterator_base<G, V, V, self_> super_;

      public:
	/// Construction and assignment.
	/// \{
	vertex_nbh_vertex_fwd_iterator();
	template <typename C>
	vertex_nbh_vertex_fwd_iterator(const C& c);
	/// \}

      protected:
	// Manipulation.
	/// \{
	/// Test if the iterator is valid.
	bool is_valid_() const;
	/// \}

	/// Start an iteration.
	unsigned start_id_() const;

	/// Go to the next value.
	unsigned next_id_() const;

	void update_();

	friend class nbh_iterator_base<G, V, V, self_>;
    };

    template <typename G>
    class vertex_nbh_vertex_bkd_iterator
      : public nbh_iterator_base<G,
				 util::vertex<G>,
				 util::vertex<G>,
				 vertex_nbh_vertex_bkd_iterator<G> >
    {
      typedef util::vertex<G> V;
      typedef vertex_nbh_vertex_bkd_iterator<G> self_;
      typedef nbh_iterator_base<G, V, V, self_> super_;

      public:
	/// Construction and assignment.
	/// \{
	vertex_nbh_vertex_bkd_iterator();
	template <typename C>
	vertex_nbh_vertex_bkd_iterator(const C& c);
	/// \}

      protected:
	/// Manipulation.
	/// \{
	/// Test if the iterator is valid.
	bool is_valid_() const;
	/// \}

	/// Start an iteration.
	unsigned start_id_() const;

	/// Go to the next value.
	unsigned next_id_() const;

	void update_();

	friend class nbh_iterator_base<G, V, V, self_>;
    };


    /*---------------------------`
    | vertex_nbh_edge_*_iterator |
    \---------------------------*/

    template <typename G>
    class vertex_nbh_edge_fwd_iterator
      : public nbh_iterator_base<G,
				 util::vertex<G>,
				 util::edge<G>,
				 vertex_nbh_edge_fwd_iterator<G> >
    {
      typedef util::vertex<G> V;
      typedef util::edge<G> E;
      typedef vertex_nbh_edge_fwd_iterator<G> self_;
      typedef nbh_iterator_base<G, V, E, self_> super_;

      public:
	/// Construction and assignment.
	/// \{
	vertex_nbh_edge_fwd_iterator();
	template <typename C>
	vertex_nbh_edge_fwd_iterator(const C& c);
	/// \}

      protected:
	// Manipulation.
	/// \{
	/// Test if the iterator is valid.
	bool is_valid_() const;
	/// \}

	/// Start an iteration.
	unsigned start_id_() const;

	/// Go to the next value.
	unsigned next_id_() const;

	void update_();

	friend class nbh_iterator_base<G, V, E, self_>;
    };


    template <typename G>
    class vertex_nbh_edge_bkd_iterator
      : public nbh_iterator_base<G,
				 util::vertex<G>,
				 util::edge<G>,
				 vertex_nbh_edge_bkd_iterator<G> >
    {
      typedef util::vertex<G> V;
      typedef util::edge<G> E;
      typedef vertex_nbh_edge_bkd_iterator<G> self_;
      typedef nbh_iterator_base<G, V, E, self_> super_;

      public:
	/// Construction and assignment.
	/// \{
	vertex_nbh_edge_bkd_iterator();
	template <typename C>
	vertex_nbh_edge_bkd_iterator(const C& c);
	/// \}

      protected:
	// Manipulation.
	/// \{
	/// Test if the iterator is valid.
	bool is_valid_() const;
	/// \}

	/// Start an iteration.
	unsigned start_id_() const;

	/// Go to the next value.
	unsigned next_id_() const;

	void update_();

	friend class nbh_iterator_base<G, V, E, self_>;
    };


    /*---------------------------`
    | edge_nbh_edge_*_iterator |
    \---------------------------*/

    template <typename G>
    class edge_nbh_edge_fwd_iterator
      : public nbh_iterator_base<G,
				 util::edge<G>,
				 util::edge<G>,
				 edge_nbh_edge_fwd_iterator<G> >
    {
      typedef util::edge<G> E;
      typedef edge_nbh_edge_fwd_iterator<G> self_;
      typedef nbh_iterator_base<G, E, E, self_> super_;

      public:
	/// Construction and assignment.
	/// \{
	edge_nbh_edge_fwd_iterator();
	template <typename C>
	edge_nbh_edge_fwd_iterator(const C& c);
	/// \}

      protected:
	// Manipulation.
	/// \{
	/// Test if the iterator is valid.
	bool is_valid_() const;
	/// \}

	/// Start an iteration.
	unsigned start_id_() const;

	/// Go to the next value.
	unsigned next_id_() const;

	void update_();

	friend class nbh_iterator_base<G, E, E, self_>;
    };


    template <typename G>
    class edge_nbh_edge_bkd_iterator
      : public nbh_iterator_base<G,
				 util::edge<G>,
				 util::edge<G>,
				 edge_nbh_edge_bkd_iterator<G> >
    {
      typedef util::edge<G> E;
      typedef edge_nbh_edge_bkd_iterator<G> self_;
      typedef nbh_iterator_base<G, E, E, self_> super_;

      public:
	/// Construction and assignment.
	/// \{
	edge_nbh_edge_bkd_iterator();
	template <typename C>
	edge_nbh_edge_bkd_iterator(const C& c);
	/// \}

      protected:
	// Manipulation.
	/// \{
	/// Test if the iterator is valid.
	bool is_valid_() const;
	/// \}

	/// Start an iteration.
	unsigned start_id_() const;

	/// Go to the next value.
	unsigned next_id_() const;

	void update_();

	friend class nbh_iterator_base<G, E, E, self_>;
    };

# ifndef MLN_INCLUDE_ONLY

    /*-------------------------------`
    | vertex_nbh_vertex_fwd_iterator |
    \-------------------------------*/

    template <typename G>
    inline
    vertex_nbh_vertex_fwd_iterator<G>::vertex_nbh_vertex_fwd_iterator()
    {
    }

    template <typename G>
    template <typename C>
    inline
    vertex_nbh_vertex_fwd_iterator<G>::vertex_nbh_vertex_fwd_iterator(const C& c)
    : super_(c)
    {
    }

    template <typename G>
    inline
    bool
    vertex_nbh_vertex_fwd_iterator<G>::is_valid_() const
    {
      return this->c_->is_valid() && this->i_ < this->c_->nmax_nbh_vertices();
    }

    template <typename G>
    inline
    unsigned
    vertex_nbh_vertex_fwd_iterator<G>::start_id_() const
    {
      return 0;
    }

    template <typename G>
    inline
    unsigned
    vertex_nbh_vertex_fwd_iterator<G>::next_id_() const
    {
      return this->i_ + 1;
    }

    template <typename G>
    inline
    void
    vertex_nbh_vertex_fwd_iterator<G>::update_()
    {
      this->elt_.update_id(this->c_->ith_nbh_vertex(this->i_));
    }

    /*-------------------------------`
    | vertex_nbh_vertex_bkd_iterator |
    \-------------------------------*/

    template <typename G>
    inline
    vertex_nbh_vertex_bkd_iterator<G>::vertex_nbh_vertex_bkd_iterator()
    {
    }

    template <typename G>
    template <typename C>
    inline
    vertex_nbh_vertex_bkd_iterator<G>::vertex_nbh_vertex_bkd_iterator(const C& c)
      : super_(c)
    {
    }

    template <typename G>
    inline
    bool
    vertex_nbh_vertex_bkd_iterator<G>::is_valid_() const
    {
      return this->c_->is_valid() && this->i_ < this->c_->nmax_nbh_vertices();
    }

    template <typename G>
    inline
    unsigned
    vertex_nbh_vertex_bkd_iterator<G>::start_id_() const
    {
      return this->c_->nmax_nbh_vertices() - 1;
    }

    template <typename G>
    inline
    unsigned
    vertex_nbh_vertex_bkd_iterator<G>::next_id_() const
    {
      return this->i_ - 1;
    }

    template <typename G>
    inline
    void
    vertex_nbh_vertex_bkd_iterator<G>::update_()
    {
      this->elt_.update_id(this->c_->ith_nbh_vertex(this->i_));
    }


    /*-----------------------------`
    | vertex_nbh_edge_fwd_iterator |
    \-----------------------------*/

    template <typename G>
    inline
    vertex_nbh_edge_fwd_iterator<G>::vertex_nbh_edge_fwd_iterator()
    {
    }

    template <typename G>
    template <typename C>
    inline
    vertex_nbh_edge_fwd_iterator<G>::vertex_nbh_edge_fwd_iterator(const C& c)
      : super_(c)
    {
    }

    template <typename G>
    inline
    bool
    vertex_nbh_edge_fwd_iterator<G>::is_valid_() const
    {
      return this->c_->is_valid() && this->i_ < this->c_->nmax_nbh_edges();
    }

    template <typename G>
    inline
    unsigned
    vertex_nbh_edge_fwd_iterator<G>::start_id_() const
    {
      return 0;
    }

    template <typename G>
    inline
    unsigned
    vertex_nbh_edge_fwd_iterator<G>::next_id_() const
    {
      return this->i_ + 1;
    }

    template <typename G>
    inline
    void
    vertex_nbh_edge_fwd_iterator<G>::update_()
    {
      this->elt_.update_id(this->c_->ith_nbh_edge(this->i_));
    }

    /*-----------------------------`
    | vertex_nbh_edge_bkd_iterator |
    \-----------------------------*/

    template <typename G>
    inline
    vertex_nbh_edge_bkd_iterator<G>::vertex_nbh_edge_bkd_iterator()
    {
    }

    template <typename G>
    template <typename C>
    inline
    vertex_nbh_edge_bkd_iterator<G>::vertex_nbh_edge_bkd_iterator(const C& c)
      : super_(c)
    {
    }

    template <typename G>
    inline
    bool
    vertex_nbh_edge_bkd_iterator<G>::is_valid_() const
    {
      return this->c_->is_valid() && this->i_ < this->c_->nmax_nbh_edges();
    }

    template <typename G>
    inline
    unsigned
    vertex_nbh_edge_bkd_iterator<G>::start_id_() const
    {
      return this->c_->nmax_nbh_edges() - 1;
    }

    template <typename G>
    inline
    unsigned
    vertex_nbh_edge_bkd_iterator<G>::next_id_() const
    {
      return this->i_ - 1;
    }

    template <typename G>
    inline
    void
    vertex_nbh_edge_bkd_iterator<G>::update_()
    {
      this->elt_.update_id(this->c_->ith_nbh_edge(this->i_));
    }



    /*-----------------------------`
    | edge_nbh_edge_fwd_iterator |
    \-----------------------------*/

    template <typename G>
    inline
    edge_nbh_edge_fwd_iterator<G>::edge_nbh_edge_fwd_iterator()
    {
    }

    template <typename G>
    template <typename C>
    inline
    edge_nbh_edge_fwd_iterator<G>::edge_nbh_edge_fwd_iterator(const C& c)
      : super_(c)
    {
    }

    template <typename G>
    inline
    bool
    edge_nbh_edge_fwd_iterator<G>::is_valid_() const
    {
      return this->c_->is_valid() && this->i_ < this->c_->nmax_nbh_edges();
    }

    template <typename G>
    inline
    unsigned
    edge_nbh_edge_fwd_iterator<G>::start_id_() const
    {
      return 0;
    }

    template <typename G>
    inline
    unsigned
    edge_nbh_edge_fwd_iterator<G>::next_id_() const
    {
      return this->i_ + 1;
    }

    template <typename G>
    inline
    void
    edge_nbh_edge_fwd_iterator<G>::update_()
    {
      // We shall encounter the current edge in its neighborhood
      // since it is computed thanks to the edge neighboors of its
      // two vertices.
      // We do not want the current edge to be part of its neighbors.
      util::edge_id_t e_id = this->c_->ith_nbh_edge(this->i_);
      while (e_id == this->c_->id())
      {
	this->i_ = next_id_();
	e_id = this->c_->ith_nbh_edge(this->i_);
      }

      this->elt_.update_id(e_id);
    }

    /*-----------------------------`
    | edge_nbh_edge_bkd_iterator |
    \-----------------------------*/

    template <typename G>
    inline
    edge_nbh_edge_bkd_iterator<G>::edge_nbh_edge_bkd_iterator()
    {
    }

    template <typename G>
    template <typename C>
    inline
    edge_nbh_edge_bkd_iterator<G>::edge_nbh_edge_bkd_iterator(const C& c)
      : super_(c)
    {
    }

    template <typename G>
    inline
    bool
    edge_nbh_edge_bkd_iterator<G>::is_valid_() const
    {
      return this->c_->is_valid() && this->i_ < this->c_->nmax_nbh_edges();
    }

    template <typename G>
    inline
    unsigned
    edge_nbh_edge_bkd_iterator<G>::start_id_() const
    {
      return this->c_->nmax_nbh_edges() - 1;
    }

    template <typename G>
    inline
    unsigned
    edge_nbh_edge_bkd_iterator<G>::next_id_() const
    {
      return this->i_ - 1;
    }

    template <typename G>
    inline
    void
    edge_nbh_edge_bkd_iterator<G>::update_()
    {
      // We shall encounter vertices which are part of the
      // current edge.
      // We do not want them to be part of the edge neighbors.
      unsigned e_id = this->c_->ith_nbh_edge(this->i_);
      while (e_id == this->c_->id())
      {
	this->i_ = next_id_();
	e_id = this->c_->ith_nbh_edge(this->i_);
      }

      this->elt_.update_id(e_id);
    }

# endif // !MLN_INCLUDE_ONLY

  } // End of namespace mln::internal

} // End of namespace mln


#endif // ! MLN_UTIL_INTERNAL_GRAPH_NBH_ITER_HH
