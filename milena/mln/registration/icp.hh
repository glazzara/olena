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

#ifndef MLN_REGISTRATION_ICP_HH
# define MLN_REGISTRATION_ICP_HH

/*! \file mln/registration/icp.hh
 *
 * \brief Register an image over an another using the ICP algorithm.
 */

# include <mln/fun/x2x/all.hh>
# include <mln/fun/x2v/all.hh>
# include <mln/registration/get_rtransf.hh>
# include <mln/registration/internal/rms.hh>

namespace mln
{

  namespace util
  {

    // FIXME: Remove use of this class
    template <unsigned int n, typename T>
    struct buffer
    {
      buffer()
        : setted(0)
      {
      }

      void store(T e)
      {
        for (unsigned i = 0; i < n-1; i++)
          buf[i+1] = buf[i];
        buf[0] = e;

        setted++;
      }

      T& operator[](unsigned int i)
      {
        assert(i < n && i < setted);
        return buf[i];
      }

      const T * get_array()
      {
        return buf;
      }

    private:
      T buf[n];
      unsigned int setted;
    };

  } // end of namespace mln::util

  namespace registration
  {

    using namespace fun::x2x;

    /*! Register point in \p c using a map of closest points \p map
     *
     * \param[in] c The cloud of points.
     * \param[in] map The map of closest points.
     * \param[in] epsilon ICP stops if sqr_norm(qk - qk-1) /
     * sqr_norm(qk) > epsilon
     * \param[out] qk The rigid transformation obtained.
     *
     * \pre \p ima has to be initialized.
     */
    template <typename P, typename M>
    inline
    composed<rotation<P::dim, float>, translation<P::dim, float> >
    icp(const p_array<P>& c, const M& map,
        const float epsilon = 1e-3);

    /*!
     * \fixme
     */
    template <typename P, typename M, typename T>
    inline
    void
    icp_subset(const p_array<P>& c,
               const M& map,
               T& qk,
               const unsigned c_length,
               const float epsilon = 1e-3);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      // FIXME: Move elsewhere
      template <typename P>
      algebra::vec<P::dim,float>
      center(const p_array<P>& a)
      {
        algebra::vec<P::dim,float> c(literal::zero);
        for (unsigned i = 0; i < a.nsites(); ++i)
          c += convert::to< algebra::vec<P::dim,float> > (a[i]);

        return c / a.nsites();
      }

      // FIXME: Make use of level::apply
      template <typename P, typename F>
      void apply(F& f,
                 const p_array<P>& a,
                 p_array<P>& b)
      {
        for (unsigned i = 0; i < a.nsites(); i++)
          b[i] = f(convert::to< algebra::vec<P::dim,float> >(a[i]));
      }


      template <typename P, typename M, typename T>
      inline
      void
      icp_(const p_array<P>& c_,
           const unsigned c_length,
           const M& map,
           T& qk,
           const float epsilon = 1e-3)
      {
	trace::entering("registration::impl::icp_");

        util::buffer<4,T>     buf_qk;
        util::buffer<3,float> buf_dk;

        float         d_k = 10000;

        //work on a reduced version of c_
        p_array<P> c = c_;
        c.hook_std_vector_().resize(c_length);

        p_array<P>    ck(c);
        algebra::vec<P::dim,float> mu_c = center(c);

        buf_qk.store(qk);

        apply(qk, c, ck);

        unsigned int k = 0;
        do {
          buf_dk.store(d_k);

          //compute qk
          qk = get_rtransf(c, mu_c, ck, map);
          buf_qk.store(qk);

          //Ck+1 = qk(c)
          apply(qk, c, ck);

          // d_k = d(Yk, Pk+1)
          //     = d(closest(qk-1(P)), qk(P))
          d_k = internal::rms(c, map, buf_qk[1], qk);

          k++;
          //FIXME: Add matrix norm
          //} while ((qk - buf_qk[1]).sqr_norm() / qk.sqr_norm() > epsilon);
        } while (d_k > epsilon);

        trace::exiting("registration::impl::icp_");
      }

    } // end of namespace mln::registration::impl


    template <typename P, typename M>
    inline
    composed<rotation<P::dim, float>, translation<P::dim, float> >
    icp(const p_array<P>& c, const M& map,
        const float epsilon = 1e-3)
    {
      composed<rotation<P::dim, float>, translation<P::dim, float> > qk;
      impl::icp_(c, c.nsites(),map, qk, epsilon);
      return qk;
    }

    template <typename P, typename M, typename T>
    inline
    void
    icp_subset(const p_array<P>& c,
               const unsigned c_length,
               const M& map,
               T& qk,
               const float epsilon = 1e-3)
    {
      impl::icp_(c, c_length, map, qk, epsilon);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::registration

} // end of namespace mln



#endif // ! MLN_REGISTRATION_ICP_HH
