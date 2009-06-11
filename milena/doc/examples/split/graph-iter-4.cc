    // Iterator on vertices.
    mln_vertex_iter_(util::graph) v(g);

    // Iterator on vertices adjacent to v.
    mln_vertex_nbh_vertex_iter_(util::graph) nv(v);

    // Prints the graph
    // List all adjacent edges for each edge.
    for_all(v)
    {
      std::cout << v << " : ";
      for_all(nv)
	std::cout << nv << " ";
      std::cout << std::endl;
    }
