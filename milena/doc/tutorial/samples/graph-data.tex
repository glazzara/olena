template <typename S>
struct viota_t : public mln::Function_p2v< viota_t<S> >
{
  typedef unsigned result;

  viota_t(unsigned size)
  {
    v_.resize(size);
    for (unsigned i = 0; i < size; ++i)
      v_[i] = 10 + i;
  }

  unsigned
  operator()(const mln_psite(S)& p) const
  {
    return v_[p.v().id()];
  }

  protected:
    std::vector<result> v_;
};

void my_fun()
{
  ...
  // Constructs an image 
  mln_VAR(graph_vertices_ima, viota_t<pv_t>() | pv);

  //Prints each vertex and its associated data.
  mln_piter(graph_vertices_ima_t) p(graph_vertices_ima);
  for_all(p)
    std::Cout << "ima(" << p << ") = "
	      << ima(p) << std::endl;
  ...
}

