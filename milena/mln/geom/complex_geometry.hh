// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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
// License.
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_GEOM_COMPLEX_GEOMETRY_HH
# define MLN_GEOM_COMPLEX_GEOMETRY_HH

/// \file mln/geom/complex_geometry.hh
/// \brief A functor associating geometry (location) data to the faces
/// of a complex.
///
/// \see mln::topo::complex.

# include <vector>
# include <set>

# include <mln/topo/face.hh>

# include <mln/util/multi_site.hh>
# include <mln/util/tracked_ptr.hh>


/* FIXME: Also provide functors where the locations are computed using
   a function (useful for a complex on a regular grid/support.  */

/* FIXME: This class could probably be turned into something more
   generic, usable for other other purpose, e.g. attaching sites to
   graphs.  */

/* FIXME: Also provide another geometry type where everything is
   stored even for n-face with n > 0.  */


namespace mln
{

  namespace geom
  {

    // Forward declaration.
    namespace internal
    {
      template <typename P> struct complex_geometry_data;
    }


    /** \brief A functor returning the sites of the faces of a complex
        where the locations of each 0-face is stored.  Faces of higher
        dimensions are computed.

        \arg \p D The dimension of the complex.
        \arg \p P The type of the location of a 0-face.  

	Locations of 0-face are usually points (hence the \p P above),
	but can possibly be any (default-constructible) values.

        The functor returns a std::vector of locations: 0-faces are
        singletons, 1-faces are (usually) pairs, faces of higher
        dimensions are arrays of locations.

	Note that for consistency reasons w.r.t. the return type of
	operator(), returned sites are always <em>arrays</em> of
	locations attached to 0-faces; hence the returned singletons
	(of locations) for 0-faces.  */
    template <unsigned D, typename P>
    class complex_geometry
    {
    public:
      typedef P location;
      typedef util::multi_site<P> site;

    public:
      /// \brief Build a complex geometry object.
      complex_geometry();

    public:
      /// \brief Populate the set of locations.
      ///
      /// Append a new location \a p.  Return the index of the newly
      /// created location (which should semantically match the id of
      /// the corresonding 0-face in the complex).
      unsigned add_location(const P& p);

      /// \brief Retrieve the site associated to \a f.
      site operator()(const mln::topo::face<D>& f) const;

    private:
      mln::util::tracked_ptr< internal::complex_geometry_data<P> > data_;
    };


    namespace internal
    {
      /// \brief The data stored in a complex_geometry object.
      ///
      /// \arg \p P The type of the location of a 0-face.  
      template <typename P>
      struct complex_geometry_data
      {
	util::multi_site<P> zero_faces;
      };
    }



# ifndef MLN_INCLUDE_ONLY

    template <unsigned D, typename P>
    inline
    complex_geometry<D, P>::complex_geometry()
      : data_(new internal::complex_geometry_data<P>())
    {
    }

    template <unsigned D, typename P>
    inline
    unsigned
    complex_geometry<D, P>::add_location(const P& p)
    {
      mln_precondition(data_);
      // FIXME: These two lines are not thread safe.
      data_->zero_faces.push_back(p);
      return data_->zero_faces.size();
    }

    template <unsigned D, typename P>
    inline
    util::multi_site<P>
    complex_geometry<D, P>::operator()(const mln::topo::face<D>& f) const
    {
      mln_precondition(data_);
      site s;
      s.reserve(1);
      if (f.n() == 0)
	{
	  // F is a 0-face.
	  mln_assertion(f.face_id() < data_->zero_faces.size());
	  s.push_back(data_->zero_faces[f.face_id()]);
	}
      else
	{
	  // F is an n-face, with n > 0.
	  typedef std::vector < topo::face<D> > faces_t;
	  typedef std::set < topo::face<D> > faces_set_t;

	  // The adjacent m-faces.
	  faces_t m_faces = f.lower_dim_adj_faces();
	  // The set of (m-1)-face being built.
	  /* FIXME: This pattern is recurring in Milena---using an
	     std::set (or any fast associative container) to improve
	     the lookup speed of an std::vector; we should create a
	     class for this, a bit like mln::util::set, but with a
	     garantee on the order of insertion.  */
 	  faces_t work_faces;
	  faces_set_t work_faces_set;

	  // Iteratively compute the set of locations.
	  for (unsigned m = f.n() - 1; m > 0; --m)
	    {
	      for (typename faces_t::const_iterator g = m_faces.begin();
		   g != m_faces.end(); ++g)
		{
		  faces_t m_minus_one_faces = g->lower_dim_adj_faces();
		  // Don't insert a face twice.
		  for (typename faces_t::const_iterator h =
			 m_minus_one_faces.begin();
		       h != m_minus_one_faces.end(); ++h)
		    if (work_faces_set.find(*h) == work_faces_set.end())
		      {
			work_faces.push_back(*h);
			work_faces_set.insert(*h);
		      }
		}
	      work_faces.swap(m_faces);
	      work_faces.clear();
	      work_faces_set.clear();
	    }
	  for (typename faces_t::const_iterator g = m_faces.begin();
	       g != m_faces.end(); ++g)
	    {
	      mln_assertion(g->face_id() < data_->zero_faces.size());
	      s.push_back(data_->zero_faces[g->face_id()]);
	    }
	}
      return s;
    }

# endif // ! MLN_INCLUDE_ONLY

    } // end of mln::geom

  } // end of mln

#endif // MLN_GEOM_COMPLEX_GEOMETRY_HH
