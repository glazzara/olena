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

#ifndef OLN_CORE_INTERNAL_IMAGE_BASE_HH
# define OLN_CORE_INTERNAL_IMAGE_BASE_HH

# include <oln/core/internal/image_selectors.hh>
# include <oln/core/concept/image_identity.hh>

# include <oln/core/internal/tracked_ptr.hh>
# include <oln/core/internal/utils.hh>
# include <oln/core/gen/box.hh>



namespace oln
{


  // FIXME: Experimental code below.

  namespace pl // placeholder for short
  {
    struct value;
    template <typename t> struct rec;
   }
 
  // End of experimental code.



  // Instant value.
  oln_decl_instant_value(ima);



  /// Fwd decls.

  namespace internal
  {
    template <typename Exact> struct image_base_;

    template <typename Exact> struct primitive_image_;
    template <typename Exact> struct plain_primitive_image_;

    template <typename Exact> struct image_morpher_;
    template <typename Exact> struct single_image_morpher_;
    template <typename Exact> struct image_extension_;
    template <typename Exact> struct multiple_image_morpher_;
  }


  /// Super types.

  template <typename Exact>
  struct super_trait_< internal::image_base_<Exact> >
  {
    typedef top<Exact> ret;
  };

  template <typename Exact>
  struct super_trait_< internal::primitive_image_<Exact> >
  {
    typedef internal::image_base_<Exact> ret;
  };

  template <typename Exact>
  struct super_trait_< internal::plain_primitive_image_<Exact> >
  {
    typedef internal::primitive_image_<Exact> ret;
  };

  template <typename Exact>
  struct super_trait_< internal::image_morpher_<Exact> >
  {
    typedef internal::image_base_<Exact> ret;
  };

  template <typename Exact>
  struct super_trait_< internal::single_image_morpher_<Exact> >
  {
    typedef internal::image_morpher_<Exact> ret;
  };

  template <typename Exact>
  struct super_trait_< internal::image_extension_<Exact> >
  {
    typedef internal::single_image_morpher_<Exact> ret;
  };

  template <typename Exact>
  struct super_trait_< internal::multiple_image_morpher_<Exact> >
  {
    typedef internal::image_morpher_<Exact> ret;
  };


  /// Virtual types.

  template <typename Exact>
  struct vtypes< internal::image_base_<Exact> >
  {
    // Abstract.

    typedef stc::abstract point;
    typedef stc::abstract psite;
    typedef stc::abstract pset;

    typedef stc::abstract value;
    typedef stc::abstract rvalue;

    // FIXME: Uncomment below!
    // typedef stc::abstract ...
    typedef stc::abstract skeleton;
    typedef stc::abstract plain;

    typedef stc::abstract data;

    // Deferred.

    typedef stc_deferred(point) point__;
    typedef stc_deferred(pset)  pset__;

    // Final.

    typedef stc::final< stc::is<Image> >         category;
    typedef stc::final< box_<point__> >          box;
    typedef stc::final< oln_grid(point__) >      grid;
    typedef stc::final< oln_dpoint(point__) >    dpoint;
    typedef stc::final< oln_fwd_piter(pset__) >  fwd_piter;
    typedef stc::final< oln_bkd_piter(pset__) >  bkd_piter;
    typedef fwd_piter                            piter;
  };


  template <typename Exact>
  struct vtypes< internal::primitive_image_<Exact> >
  {
  };

  template <typename Exact>
  struct vtypes< internal::plain_primitive_image_<Exact> >
  {
    typedef stc_deferred(point) point__;

    typedef stc::final< point__ > psite;
  };

  template <typename Exact>
  struct vtypes< internal::image_morpher_<Exact> >
  {
    typedef stc::abstract      delegatee;
    typedef stc::abstract      behavior;
    
    // not delegated:
    typedef stc::not_delegated data;
    typedef stc::not_delegated plain;
    typedef stc::not_delegated skeleton;
  };

  template <typename Exact>
  struct vtypes< internal::single_image_morpher_<Exact> >
  {
  };

  template <typename Exact>
  struct vtypes< internal::image_extension_<Exact> >
  {
    typedef stc::final< behavior::identity > behavior;
  };

  template <typename Exact>
  struct vtypes< internal::multiple_image_morpher_<Exact> >
  {
    typedef stc::abstract n;
  };



  /// Implementation base classes.


  namespace internal
  {


    /// image_base_<Exact>

    template <typename Exact>
    class image_base_ : public top<Exact>
    {
      typedef top<Exact> super;
    public:

      stc_typename(data);

      bool has_data() const;
      
      // pseudo-private:
      tracked_ptr<data>& data__();
     
    protected:
      image_base_();

      tracked_ptr<data> data_;
    };


    /// primitive_image_<Exact>

    template <typename Exact>
    class primitive_image_ : public image_base_<Exact>
    {
    public:
      std::size_t impl_npoints() const;
    protected:
      primitive_image_();
    };


    /// plain_primitive_image_<Exact>

    template <typename Exact>
    class plain_primitive_image_ : public primitive_image_<Exact>
    {
    protected:
      plain_primitive_image_();
    };


    /// image_morpher_<Exact>

    template <typename Exact>
    class image_morpher_ : public image_base_<Exact>
    {
    public:

      stc_typename(delegatee);

    protected:
      image_morpher_();
    };


    /// single_image_morpher_<Exact>

    template <typename Exact>
    class single_image_morpher_ : public image_morpher_<Exact>
    {
      typedef image_morpher_<Exact> super;
    public:
      stc_using(delegatee);

      // Abstract.
      delegatee& image();
      const delegatee& image() const;

    protected:
      single_image_morpher_();
    };


    /// image_extension_<Exact>

    template <typename Exact>
    class image_extension_ : public single_image_morpher_<Exact>
    {
    protected:
      image_extension_();
    };


    /// multiple_image_morpher_<Exact>

    template <typename Exact>
    class multiple_image_morpher_ : public image_morpher_<Exact>
    {
      typedef image_morpher_<Exact> super;
    public:
      stc_using(delegatee);
      stc_typename(n);
      enum { n_ = mlc_value(n) };

      // Abstract.
      delegatee& image(unsigned i = 0);
      const delegatee& image(unsigned i = 0) const;

    protected:
      multiple_image_morpher_();
    };




# ifndef OLN_INCLUDE_ONLY

    /// image_base_<Exact>

    template <typename Exact>
    bool image_base_<Exact>::has_data() const
    {
      return this->data_ != 0;
    }

    template <typename Exact>
    image_base_<Exact>::image_base_()
    {
    }

    template <typename Exact>
    tracked_ptr<typename image_base_<Exact>::data>&
    image_base_<Exact>::data__()
    {
      // there is no test here since it is OK even if data_ is unset
      return this->data_;
    }

    /// primitive_image_<Exact>

    template <typename Exact>
    primitive_image_<Exact>::primitive_image_()
    {
    }

    template <typename Exact>
    std::size_t primitive_image_<Exact>::impl_npoints() const
    {
      precondition(this->has_data());
      return this->pset().npoints();
    }

    /// plain_primitive_image_<Exact>

    template <typename Exact>
    plain_primitive_image_<Exact>::plain_primitive_image_()
    {
    }

    // image_morpher_<Exact>

    template <typename Exact>
    image_morpher_<Exact>::image_morpher_()
    {
    }

    // single_image_morpher_<Exact>

    template <typename Exact>
    typename single_image_morpher_<Exact>::delegatee&
    single_image_morpher_<Exact>::image()
    {
      precondition(this->has_data());
      return exact(this)->impl_image();
    }

    template <typename Exact>
    const typename single_image_morpher_<Exact>::delegatee&
    single_image_morpher_<Exact>::image() const
    {
      precondition(this->has_data());
      return exact(this)->impl_image();
    }

    template <typename Exact>
    single_image_morpher_<Exact>::single_image_morpher_()
    {
    }

    // image_extension_<Exact>

    template <typename Exact>
    image_extension_<Exact>::image_extension_()
    {
    }

    // multiple_image_morpher_<Exact>

    template <typename Exact>
    typename multiple_image_morpher_<Exact>::delegatee&
    multiple_image_morpher_<Exact>::image(unsigned i)
    {
      precondition(i < n_);
      precondition(this->has_data());
      return exact(this)->impl_image(i);
    }

    template <typename Exact>
    const typename multiple_image_morpher_<Exact>::delegatee&
    multiple_image_morpher_<Exact>::image(unsigned i) const
    {
      precondition(i < n_);
      precondition(this->has_data());
      return exact(this)->impl_image(i);
    }

    template <typename Exact>
    multiple_image_morpher_<Exact>::multiple_image_morpher_()
    {
    }


# endif // OLN_INCLUDE_ONLY


  } // end of namespace oln::internal



  // init

  template <typename P, typename I>
  bool init_(box_<P>* this_, const internal::image_base_<I>& data);

  template <typename Target, typename I>
  bool init_(Any<Target>* this_, const internal::single_image_morpher_<I>& data);

  template <typename P, typename I> // for disambiguation purpose
  bool init_(box_<P>* this_, const internal::single_image_morpher_<I>& data);


# ifndef OLN_INCLUDE_ONLY

  template <typename P, typename I>
  bool init_(box_<P>* this_, const internal::image_base_<I>& data)
  {
    *this_ = data.bbox();
    return true;
  }

  template <typename Target, typename I>
  bool init_(Any<Target>* this_, const internal::single_image_morpher_<I>& data)
  {
    return init(*this_, with, data.image());
  }

  template <typename P, typename I>
  bool init_(box_<P>* this_, const internal::single_image_morpher_<I>& data)
  {
    *this_ = data.bbox();
    return true;
  }

# endif // ! OLN_INCLUDE_ONLY


} // end of namespace oln




/// \{
/// FIXME: Bad!

# include <oln/core/internal/f_ch_value.hh>
# include <oln/core/internal/op_image_restricted_to_pset.hh>
# include <oln/core/internal/op_image_such_as_fp2b.hh>

namespace oln
{
  // Image | Point_Set    ( ima restricted_to pset )

  oln_decl_op_restricted_to(Image, Point_Set);


  // Image | Function_p2b  (     ima such_as "f : p -> b"
  //                         is  ima restricted_to (ima.points such_as f) )
  oln_decl_op_such_as(Image, Function_p2b);


  // Specialization.

  template <typename I, typename B, typename P>
  op_<const I, such_as, const fun_p2b_<B (*)(P)> >
  operator | (const Image<I>& ima, B (*f)(P))
  {
    typedef oln_strip_(P) P_;
    mlc::assert_< mlc_is_a(P_, Point) >::check(); // FIXME: Add err msg.
    mlc::assert_equal_< P_, oln_point(I) >::check();
    op_<const I, such_as, const fun_p2b_<B (*)(P)> > tmp(exact(ima), f);
    return tmp;
  }

} // end of namespace oln

/// \}



#endif // ! OLN_CORE_INTERNAL_IMAGE_BASE_HH
