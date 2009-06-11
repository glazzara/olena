
  // Constructs an image
  viota_t<pv_t> viota(pv.nsites());
  mln_VAR(graph_vertices_ima, viota | pv);

  //Prints each vertex and its associated data.
  mln_piter_(graph_vertices_ima_t) p(graph_vertices_ima.domain());
  for_all(p)
    std::cout << "graph_vertices_ima(" << p << ") = "
	      << graph_vertices_ima(p) << std::endl;

