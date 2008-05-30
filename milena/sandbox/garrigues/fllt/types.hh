// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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


#ifndef MLN_FIXME_FLLT_TYPES_HH
# define MLN_FIXME_FLLT_TYPES_HH

/*! \file types.hh
 *
 * \brief Types for Fast level line transform.
 *
 */

# include <mln/core/p_set.hh>
# include <mln/util/tree.hh>
# include <mln/util/branch_iter_ind.hh>


# define fllt_tree(P, V)  mln::util::tree< mln::fllt::fllt_node_elt<P, V> >
# define fllt_node(P, V)  mln::util::tree_node< mln::fllt::fllt_node_elt<P, V> >
# define fllt_branch(P, V)  mln::util::branch< mln::fllt::fllt_node_elt<P, V> >
# define fllt_branch_iter_ind(P, V)  mln::util::branch_iter_ind< mln::fllt::fllt_node_elt<P, V> >

namespace mln
{
  namespace fllt
  {

    template <typename P, typename V>
    struct fllt_node_elt
    {
      V	value;
      p_set<P> points;
      p_set<P> holes;
      /// Tell if his parent if brighter or not.  Nb : if the parent
      /// if brighter, the node come from the lower level set
      bool brighter;
    };

    //    # define fllt_node(P, V)  typename fllt_tree(P, V)::node_t


    class ran_domains
    {
    public:

      /// Forward Site_Iterator associated type.
      typedef mln_fwd_piter_(box2d) fwd_piter;

      /// Backward Site_Iterator associated type.
      typedef mln_bkd_piter_(box2d) bkd_piter;

      /// Constructor.
      ran_domains(const box2d& b);

      /// Add a point to a domain.
      template <char domain>
      ran_domains& add_to(const point2d& p);

      /// Test if a point belong to a domain.
      template <char domain>
      bool belongs_to(const point2d& p) const;

      /// Move a point from a domain to another domain.
      template <char src, char dest>
      ran_domains& move_to(const point2d& p);

      /// Clear the image.
      void clear();

      /// Give the exact bounding box.
      const box2d& bbox() const;

      /// Give the number of points.
      unsigned npoints() const;

      /// Hook to the image2d containing the points.
      sub_image<image2d<value::int_u8>, box2d> image();

    private:
      image2d<value::int_u8> ima_;
      unsigned npoints_;
      accu::bbox<point2d> bb_;
    };


# ifndef MLN_INCLUDE_ONLY

    inline
    ran_domains::ran_domains(const box2d& b)
      : ima_(b)
    {
      bb_.init();
      npoints_ = 0;

      level::fill(ima_, false);
    }

    template <char domain>
    inline
    ran_domains&
    ran_domains::add_to(const point2d& p)
    {
      bb_.take(p);
      ima_(p) = domain;
      npoints_++;
      return *this;
    }


    template <char src, char dest>
    ran_domains&
    ran_domains::move_to(const point2d& p)
    {
      mln_assertion(ima_(p) == src);
      ima_(p) += dest - src;
      return *this;
    }

    template <char domain>
    inline
    bool
    ran_domains::belongs_to(const point2d& p) const
    {
      return ima_(p) == domain;
    }


    void
    ran_domains::clear()
    {
      if (npoints_ == 0)
	return;

      //       unsigned bb_nrows  = geom::nrows(bb_.to_result());
      //       unsigned ima_nrows = geom::nrows(points_);

      //       if (bb_nrows * 3 < ima_nrows * 2)
      //       {
      // 	unsigned bb_ncols = geom::ncols(bb_.to_result());
      // 	mln_line_piter_(image2d<value::int_u8>) p(bb_.to_result());
      // 	for_all(p)
      // 	{
      // 	  level::memset_(ima_, p, false, bb_ncols);
      // 	}
      //       }
      //       else
      level::fill(ima_, false);

      npoints_ = 0;
      bb_.init();
    }


    inline
    const box2d&
    ran_domains::bbox() const
    {
      mln_precondition(npoints_ != 0);
      return bb_.to_result();
    }

    inline
    unsigned
    ran_domains::npoints() const
    {
      return npoints_;
    }

    inline
    sub_image<image2d<value::int_u8>, box2d>
    ran_domains::image()
    {
      mln_precondition(npoints_ > 0);
      mln_assertion(ima_.has_data());
      return ima_ | bb_.to_result();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::fllt

} // end of namespace mln



#endif // ! MLN_FIXME_FLLT_TYPES_HH
