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

#ifndef MLN_GEOM_COMPLEX_GEOMETRY_HH
# define MLN_GEOM_COMPLEX_GEOMETRY_HH

/// \file
/// A functor associating geometry (location) data to the faces
/// of a complex.
///
/// \see mln::topo::complex.

# include <vector>
# include <set>

# include <mln/topo/face.hh>
# include <mln/topo/adj_m_face_iter.hh>

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


    /** A functor returning the sites of the faces of a complex
        where the locations of each 0-face is stored.  Faces of higher
        dimensions are computed.

        \tparam D The dimension of the complex.
        \tparam P The type of the location of a 0-face.

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
      /// Build a complex geometry object.
      complex_geometry();

    public:
      /// Populate the set of locations.
      ///
      /// Append a new location \a p.  Return the index of the newly
      /// created location (which should semantically match the id of
      /// the corresonding 0-face in the complex).
      unsigned add_location(const P& p);

      /// Retrieve the site associated to \a f.
      site operator()(const mln::topo::face<D>& f) const;

    private:
      mln::util::tracked_ptr< internal::complex_geometry_data<P> > data_;
    };


    namespace internal
    {
      /// The data stored in a complex_geometry object.
      ///
      /// \tparam P The type of the location of a 0-face.
      template <typename P>
      struct complex_geometry_data
      {
	std::vector<P> zero_faces_geom;
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
      data_->zero_faces_geom.push_back(p);
      return data_->zero_faces_geom.size();
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
	  mln_assertion(f.face_id() < data_->zero_faces_geom.size());
	  s.push_back(data_->zero_faces_geom[f.face_id()]);
	}
      else
	{
	  /* F is an n-face, with n > 0.
	     Compute the set of 0-faces transitively adjacent to F.  */
	  topo::adj_m_face_fwd_iter<D> g(f, 0);
	  for_all(g)
	    s.push_back(data_->zero_faces_geom[g.subject().face_id()]);
	}
      return s;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of mln::geom

} // end of mln

#endif // ! MLN_GEOM_COMPLEX_GEOMETRY_HH
