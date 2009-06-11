namespace mln
{

  struct my_ext : public Function_v2v<my_ext>
  {

    typedef value::rgb8 result;

    value::rgb8 operator()(const point2d& p) const
    {
      if ((p.row() + p.col()) % 20)
	return literal::black;
      return literal::white;
    }

  };

} // end of namespace mln
