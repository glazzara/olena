// Copyright (C) 2006, 2007 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_INTERNAL_DPOINTS_PITER_IMPL_HH
# define OLN_CORE_INTERNAL_DPOINTS_PITER_IMPL_HH

# include <oln/core/concept/iterator_on_points.hh>
# include <oln/core/internal/dpoints_impl.hh>


namespace oln
{

  namespace internal
  {

    /// Classes to factor code for iterators based on vector of dpoints.

    template <typename P>
    class dpoints_piter_impl_
    {
    public:
      // dpoints_piter_impl_(const dpoints_piter_impl_&);

      void impl_invalidate();
      bool impl_is_valid() const;
      
      const P* impl_point_adr() const;

    protected:

      template <typename Pl>
      dpoints_piter_impl_(const Pl& p_ref,
			  const std::vector<typename P::dpoint>& dps);

      void update_p_();

      const P* p_ref_;
      const std::vector<typename P::dpoint>& dps_;
      unsigned n_, i_;
      P p_;
    };


    // fwd

    template <typename P>
    class dpoints_fwd_piter_impl_ : public dpoints_piter_impl_<P>
    {
    public:

      void impl_start();
      void impl_next();

    protected:

      template <typename Pl>
      dpoints_fwd_piter_impl_(const Pl& p_ref,
			      const dpoints_impl_<typename P::dpoint>& data);
    };


    // bkd

    template <typename P>
    class dpoints_bkd_piter_impl_ : public dpoints_piter_impl_<P>
    {
    public:

      void impl_start();
      void impl_next();

    protected:

      template <typename Pl>
      dpoints_bkd_piter_impl_(const Pl& p_ref,
			      const dpoints_impl_<typename P::dpoint>& data);
    };
  


# ifndef OLN_INCLUDE_ONLY


    // point_adr_

    template <typename Pl>
    const Pl* point_adr_(const Point<Pl>& p)
    {
      return exact(&p);
    }

    template <typename Pl>
    const typename Pl::point* point_adr_(const Iterator_on_Points<Pl>& p)
    {
      return p.point_adr();
    }


    // dpoints_piter_impl_<P>

    template <typename P>
    template <typename Pl>
    dpoints_piter_impl_<P>::dpoints_piter_impl_(const Pl& p_ref,
						const std::vector<typename P::dpoint>& dps)
      : p_ref_(point_adr_(p_ref)),
	dps_(dps),
	n_(dps.size()),
	i_(n_)
    {
      mlc::assert_< mlc_is_a(P, Point) >::check(); // FIXME: Add err msg.
      precondition(dps.size() != 0);
    }

    template <typename P>
    void
    dpoints_piter_impl_<P>::impl_invalidate()
    {
      this->i_ = this->n_;
    }

    template <typename P>
    bool
    dpoints_piter_impl_<P>::impl_is_valid() const
    {
      return this->i_ != this->n_;
    }

    template <typename P>
    const P*
    dpoints_piter_impl_<P>::impl_point_adr() const
    {
      return &(this->p_);
    }

    template <typename P>
    void
    dpoints_piter_impl_<P>::update_p_()
    {
      this->p_ = *(this->p_ref_) + this->dps_[i_];
    }


    // dpoints_fwd_piter_impl_<P>

    template <typename P>
    template <typename Pl>
    dpoints_fwd_piter_impl_<P>::dpoints_fwd_piter_impl_(const Pl& p_ref,
							const dpoints_impl_<typename P::dpoint>& data)
      : dpoints_piter_impl_<P>(p_ref, data.dpoints())
    {
    }

    template <typename P>
    void
    dpoints_fwd_piter_impl_<P>::impl_start()
    {
      this->i_ = 0;
      this->update_p_();
    }

    template <typename P>
    void
    dpoints_fwd_piter_impl_<P>::impl_next()
    {
      if (++this->i_ == this->n_)
	return;
      this->update_p_();
    }


    // dpoints_bkd_piter_impl_<P>

    template <typename P>
    template <typename Pl>
    dpoints_bkd_piter_impl_<P>::dpoints_bkd_piter_impl_(const Pl& p_ref,
							const dpoints_impl_<typename P::dpoint>& data)
      : dpoints_piter_impl_<P>(p_ref, data.dpoints())
    {
    }

    template <typename P>
    void
    dpoints_bkd_piter_impl_<P>::impl_start()
    {
      this->i_ = this->n_ - 1;
      this->update_p_();
    }

    template <typename P>
    void
    dpoints_bkd_piter_impl_<P>::impl_next()
    {
      if (this->i_ == 0)
	{
	  this->i_ = this->n_;
	  return;
	}
      --this->i_;
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::internal

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_DPOINTS_PITER_IMPL_HH
