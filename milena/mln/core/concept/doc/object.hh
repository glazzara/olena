/*! \file mln/core/concept/doc/object.hh
 * \brief This file documents the concept of mln::Object.
 */

namespace mln
{

  /*! \namespace mln::doc
   * \brief The namespace mln::doc is only for documentation purpose.
   *
   * Since concepts are not yet part of the C++ Standard, they are not
   * explicitly expressed in code.  Their documentation is handled by
   * their respective ghost class, located in this namespace.
   *
   * \warning The ghost classes located in mln::doc should not be used
   * by the client.
   */
  namespace doc
  {

    /*! \brief Documentation class for mln::Object.
     * \see mln::Object
     */
    template <typename E>
    struct Object
    {
    };

  } // end of namespace mln::doc

} // end of namespace mln
