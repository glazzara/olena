#ifndef MLN_CORE_INTERNAL_COORD_IMPL_HH
# define MLN_CORE_INTERNAL_COORD_IMPL_HH

# include <mln/core/concept/object.hh>


namespace mln
{

  namespace internal
  {

    // coord_impl

    template <unsigned n, typename C, typename E>
    struct coord_impl_;

    template <typename C, typename E>
    struct coord_impl_<1, C, E>
    {
      C ind() const;
    };

    template <typename C, typename E>
    struct coord_impl_<2, C, E>
    {
      C row() const;
      C col() const;
    };

    template <typename C, typename E>
    struct coord_impl_<3, C, E>
    {
      C sli() const;
      C row() const;
      C col() const;
    };


    // mutable_coord_impl

    template <unsigned n, typename C, typename E>
    struct mutable_coord_impl_;

    template <typename C, typename E>
    struct mutable_coord_impl_<1, C, E>
    {
      C  ind() const;
      C& ind();
    };

    template <typename C, typename E>
    struct mutable_coord_impl_<2, C, E>
    {
      C  row() const;
      C& row();
      C  col() const;
      C& col();
    };

    template <typename C, typename E>
    struct mutable_coord_impl_<3, C, E>
    {
      C  sli() const;
      C& sli();
      C  row() const;
      C& row();
      C  col() const;
      C& col();
    };


# ifndef MLN_INCLUDE_ONLY
    
    // coord_impl

    // 1

    template <typename C, typename E>
    C coord_impl_<1, C, E>::ind() const
    {
      return force_exact<E>(*this)[0];
    }

    // 2

    template <typename C, typename E>
    C coord_impl_<2, C, E>::row() const
    {
      return force_exact<E>(*this)[0];
    }

    template <typename C, typename E>
    C coord_impl_<2, C, E>::col() const
    {
      return force_exact<E>(*this)[1];
    }

    // 3

    template <typename C, typename E>
    C coord_impl_<3, C, E>::sli() const
    {
      return force_exact<E>(*this)[0];
    }

    template <typename C, typename E>
    C coord_impl_<3, C, E>::row() const
    {
      return force_exact<E>(*this)[1];
    }

    template <typename C, typename E>
    C coord_impl_<3, C, E>::col() const
    {
      return force_exact<E>(*this)[2];
    }


    // mutable_coord_impl

    // 1

    template <typename C, typename E>
    C  mutable_coord_impl_<1, C, E>::ind() const
    {
      return force_exact<E>(*this)[0];
    }

    template <typename C, typename E>
    C& mutable_coord_impl_<1, C, E>::ind()
    {
      return force_exact<E>(*this)[0];
    }

    // 2

    template <typename C, typename E>
    C  mutable_coord_impl_<2, C, E>::row() const
    {
      return force_exact<E>(*this)[0];
    }

    template <typename C, typename E>
    C& mutable_coord_impl_<2, C, E>::row()
    {
      return force_exact<E>(*this)[0];
    }

    template <typename C, typename E>
    C  mutable_coord_impl_<2, C, E>::col() const
    {
      return force_exact<E>(*this)[1];
    }

    template <typename C, typename E>
    C& mutable_coord_impl_<2, C, E>::col()
    {
      return force_exact<E>(*this)[1];
    }

    // 3

    template <typename C, typename E>
    C  mutable_coord_impl_<3, C, E>::sli() const
    {
      return force_exact<E>(*this)[0];
    }

    template <typename C, typename E>
    C& mutable_coord_impl_<3, C, E>::sli()
    {
      return force_exact<E>(*this)[0];
    }

    template <typename C, typename E>
    C  mutable_coord_impl_<3, C, E>::row() const
    {
      return force_exact<E>(*this)[1];
    }

    template <typename C, typename E>
    C& mutable_coord_impl_<3, C, E>::row()
    {
      return force_exact<E>(*this)[1];
    }

    template <typename C, typename E>
    C  mutable_coord_impl_<3, C, E>::col() const
    {
      return force_exact<E>(*this)[2];
    }

    template <typename C, typename E>
    C& mutable_coord_impl_<3, C, E>::col()
    {
      return force_exact<E>(*this)[2];
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_COORD_IMPL_HH
