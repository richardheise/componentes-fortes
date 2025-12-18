#include <stdio.h>
#include "graph.h"

//------------------------------------------------------------------------------

int main(void) {

  graph g = read_graph();

  decompose_strong_components(g);

  for (graph sub = agfstsubg(g); sub; sub = agnxtsubg(sub)) {
      write_graph(sub);
  }

  destroy_graph(g);

  return 0;
}
