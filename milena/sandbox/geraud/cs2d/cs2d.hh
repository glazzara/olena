# include <mln/core/image/image2d.hh>
# include <mln/core/neighb2d.hh>
# include <mln/core/window2d.hh>
# include <mln/convert/to_window.hh>

# include <mln/core/image/image_if.hh>
# include <mln/debug/println.hh>


# include "cs2d_utils.hh"


namespace mln
{


  template <typename C, typename E, typename X>
  struct cs2d
  {
    typedef C cell_value;
    typedef E edge_value;
    typedef X vertex_value;

    cs2d(unsigned nrows, unsigned ncols)
    {
      init(nrows, ncols);
    }

    cs2d(const box2d& b)
    {
      init(b.nrows(), b.ncols());
    }

    void init(unsigned nrows, unsigned ncols)
    {
      mln_precondition(nrows && ncols);

      // Raw data (impl).
      b = make::box2d(2 * nrows - 1, 2 * ncols - 1);
      cimpl = image2d<C>(b, 0);
      eimpl = image2d<E>(b, 0);
      ximpl = image2d<X>(b, 0);

      // Images.
      cell  = cimpl | is_cell;
      edge  = eimpl | is_edge;
      vertex = ximpl | is_vertex;
    }

    unsigned nrows() const { return (b.nrows() + 1) / 2; }
    unsigned ncols() const { return (b.ncols() + 1) / 2; }

    box2d b;
    image2d<C> cimpl;
    image2d<E> eimpl;
    image2d<X> ximpl;

    typedef image_if< image2d<C>, is_cell_t > cell_t;
    cell_t cell;

    typedef image_if< image2d<E>, is_edge_t > edge_t;
    edge_t edge;

    typedef image_if< image2d<X>, is_vertex_t > vertex_t;
    vertex_t vertex;
  };




  template <typename T>
  struct cs2d< T, T, T>
  {
    typedef T cell_value;
    typedef T edge_value;
    typedef T vertex_value;

    cs2d(unsigned nrows, unsigned ncols)
    {
      init(nrows, ncols);
    }

    cs2d(const box2d& b)
    {
      init(b.nrows(), b.ncols());
    }

    void init(unsigned nrows, unsigned ncols)
    {
      mln_precondition(nrows && ncols);

      // Raw data (impl).
      b = make::box2d(2 * nrows - 1, 2 * ncols - 1);
      impl = image2d<T>(b, 0);

      // Images.
      cell   = impl | is_cell;
      edge   = impl | is_edge;
      vertex = impl | is_vertex;
    }

    unsigned nrows() const { return (b.nrows() + 1) / 2; }
    unsigned ncols() const { return (b.ncols() + 1) / 2; }

    box2d b;
    image2d<T> impl;

    typedef image_if< image2d<T>, is_cell_t > cell_t;
    cell_t cell;

    typedef image_if< image2d<T>, is_edge_t > edge_t;
    edge_t edge;

    typedef image_if< image2d<T>, is_vertex_t > vertex_t;
    vertex_t vertex;
  };





  template <typename C, typename T>
  struct cs2d< C, T, T>
  {
    typedef C cell_value;
    typedef T edge_value;
    typedef T vertex_value;

    cs2d(unsigned nrows, unsigned ncols)
    {
      init(nrows, ncols);
    }

    cs2d(const box2d& b)
    {
      init(b.nrows(), b.ncols());
    }

    void init(unsigned nrows, unsigned ncols)
    {
      mln_precondition(nrows && ncols);

      // Raw data (impl).
      b = make::box2d(2 * nrows - 1, 2 * ncols - 1);
      cimpl = image2d<C>(b, 0);
      impl  = image2d<T>(b, 0);

      // Images.
      cell   = cimpl | is_cell;
      edge   =  impl | is_edge;
      vertex =  impl | is_vertex;
    }

    unsigned nrows() const { return (b.nrows() + 1) / 2; }
    unsigned ncols() const { return (b.ncols() + 1) / 2; }

    box2d b;
    image2d<C> cimpl;
    image2d<T>  impl;

    typedef image_if< image2d<C>, is_cell_t > cell_t;
    cell_t cell;

    typedef image_if< image2d<T>, is_edge_t > edge_t;
    edge_t edge;

    typedef image_if< image2d<T>, is_vertex_t > vertex_t;
    vertex_t vertex;
  };


} // mln
