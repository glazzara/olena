/*! \file mln/core/concept/doc/window.hh
 * \brief This file documents the concept of mln::Window.
 */

namespace mln
{

  namespace doc
  {

    /*! \brief Documentation class for mln::Window.
     * \see mln::Window
     */
    template <typename E>
    struct Window : public Object<E>
    {
      /*! \brief Piter type associated to this window to browse its
       * points.
       */
      typedef void qiter;

      /*! \brief Piter type associated to this window to browse its
       * points in a forward way.
       */
      typedef void fwd_qiter;

      /*! \brief Piter type associated to this window to browse its
       * points in a backward way.
       */
      typedef void bkd_qiter;

      /*! \brief Test if the window is empty.
       *
       * A window of null size is empty.
       */
      bool is_empty() const;

      /*! \brief Test if the window is centered.
       *
       * A window is centered is the origin belongs to the window.
       */
      bool is_centered() const;

      /*! \brief Test if the window is symmetric.
       */
      bool is_symmetric() const;
    };

  } // end of namespace mln::doc

} // end of namespace mln
