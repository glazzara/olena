// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_REGISTRATION_MULTISCALE_HH
# define MLN_REGISTRATION_MULTISCALE_HH

# include <mln/registration/icp.hh>

namespace mln
{

  namespace registration
  {

    // FIXME: Make it works in 3d *AND* 2d
    template <typename P, typename M>
    inline
    quat7<P::dim>
    multiscale(p_array<P>& cloud,
               const M& map,
               const float q,
               const unsigned nb_it,
               const p_array<P>& x)
    {
      trace::entering("registration::registration");

      mln_precondition(P::dim == 3 || P::dim == 2);
      mln_precondition(cloud.nsites() != 0);

      // Shuffle cloud
      shuffle(cloud);

      //init rigid transform qk
      quat7<P::dim> qk;

      //run registration
      for (int e = nb_it-1; e >= 0; e--)
        {
          unsigned l = cloud.nsites() / std::pow(q, e);
          l = (l<1) ? 1 : l;
          impl::registration_(cloud, map, qk, l, 1e-3);
        }

      trace::exiting("registration::registration");

      return qk;
    }

  } // end of namespace mln::registration

} // end of namespace mln


#endif // ! MLN_REGISTRATION_MULTISCALE_HH
