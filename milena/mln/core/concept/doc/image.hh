/*! \file mln/core/concept/doc/image.hh
 * This file documents the concept of mln::Image.
 */

namespace mln
{

  namespace doc
  {

    /*! Documentation class for mln::Image.
     * \see mln::Image
     */
    template <typename E>
    struct Image : public Object<E>
    {
      // to be provided in concrete image classes:

      typedef void value;
      typedef void rvalue;
      typedef void lvalue;

      bool has_data() const;

      bool owns_(const psite& p) const; // default is like "has(p)"
      const pset& domain() const;

      rvalue operator()(const psite& p) const;
      lvalue operator()(const psite& p);

      template <typename T>
      struct change_value
      {
        typedef ret;
      };


      // provided by internal::image_base_:

      typedef void pset;
      typedef void point;
      typedef void psite;

      typedef void piter;
      typedef void fwd_piter;
      typedef void bkd_piter;

      bool has(const psite& p) const;
      const box_<point>& bbox() const;
    };

  } // end of namespace mln::doc

} // end of namespace mln
