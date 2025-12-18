#ifndef _GRAPH_H
#define _GRAPH_H

#include <graphviz/cgraph.h>

//------------------------------------------------------------------------------
typedef Agraph_t *graph;
typedef Agnode_t *node;

// -----------------------------------------------------------------------------
// devolve o grafo lido da entrada padrão (formato dot)

graph read_graph(void);

// -----------------------------------------------------------------------------
// desaloca g

int destroy_graph(graph g);

// -----------------------------------------------------------------------------
// escreve g na saída padrão em formato dot
// devolve g

void write_graph(graph g);

//------------------------------------------------------------------------------
// acrescenta à lista de subgrafos de g cada um de seus componentes fortes
// vide agsubg(), agfstsubg(), agnxtsubg()
//
// não faz nada se g não for grafo direcionado
//
// devolve g

graph decompose_strong_components(graph g);


#endif
