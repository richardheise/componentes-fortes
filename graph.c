// Richard Fernando Heise Ferreira GRR20191053

// Boas férias, Renatão, Murilinho e Pachecão!
#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

// Estrutura de vértice para decomposição
typedef struct vertex_data_s {
    Agnode_t *node;
    int state;
    int component;
} VertexData;

//------------------------------------------------------------------------------
Agraph_t* read_graph(void) {

    return agread(stdin, NULL);
}
//------------------------------------------------------------------------------
int destroy_graph(Agraph_t* g) {

    agclose(g);
    return 1;
}
//------------------------------------------------------------------------------
void write_graph(Agraph_t* g) {

    agwrite(g, stdout);
}

//------------------------------------------------------------------------------

static int search_vector(Agnode_t* vector[], Agnode_t* elem, int size) {
    for (int i = 0; i < size; i++) {
        if (vector[i] == elem) {
            return 1;
        }
    }

    return 0;
}

//------------------------------------------------------------------------------

static int decompose_recursive(Agraph_t* g, VertexData* vertices, VertexData* r, int c, int size) {

    // Caso base, se já processamos, retornamos
    if (r->state == 2) {
        return 0;
    }

    // Processando
    r->state = 1;

    // DFS
    for (int i = 0; i < size; i++) {
        if ( agedge(g, r->node, vertices[i].node, NULL, FALSE) ) {

            // Se o estado é não processado
            if (vertices[i].state == 0) {
                decompose_recursive(g, vertices, &vertices[i], c, size);
            }
        }
    }

    // r pertence a este componente c
    r->component = c;

    // r foi processado
    r->state = 2;

    return 1;

}

//------------------------------------------------------------------------------

static void reverse_post_order_dfs(Agraph_t* g, Agnode_t* ver, Agnode_t* visited[], int* visited_size, Agnode_t* post_order[], int* post_order_size) {

    // Caso base, se já visitamos não fazemos nada
    if (search_vector(visited, ver, *visited_size)) return;

    // Estamos visitando ver
    visited[*visited_size] = ver;
    *visited_size += 1;

    // DFS
    for (Agnode_t* current_node = agfstnode(g); current_node; current_node = agnxtnode(g, current_node)) {
        if ( agedge(g, ver, current_node, NULL, FALSE) ) {
            reverse_post_order_dfs(g, current_node, visited, visited_size, post_order, post_order_size);
        }
    }

    // Salva a pós-ordem em pos[]
    post_order[*post_order_size] = ver;
    *post_order_size += 1;
}

//------------------------------------------------------------------------------

Agraph_t* decompose_strong_components(Agraph_t* g) {

    // Se o grafo não é direcionado não fazemos nada
    if (!agisdirected(g)) return g;

    // Quantidade de vértices
    int size = agnnodes(g);

    // Vetor de visitados
    int visited_size = 0;
    Agnode_t* visited[size];
    memset(visited, 0, size);

    // vetor de pós-ordem
    int post_order_size = 0;
    Agnode_t* post_order[size];
    memset(post_order, 0, size);

    // Varremos o grafo fazendo a pós-ordem reversa
    for (Agnode_t* current_node = agfstnode(g); current_node; current_node = agnxtnode(g, current_node)) {
        reverse_post_order_dfs(g, current_node, visited, &visited_size, post_order, &post_order_size);
    }

    // Vetor de vértices com estado e componente
    int vpp_size = 0;
    VertexData* vertices = malloc(sizeof(VertexData) * size);

    // Inicialização da estrutura verticePP
    for (int i = 0; i < post_order_size; i++) {
        vertices[vpp_size].component = vertices[vpp_size].state = 0;
        vertices[vpp_size++].node = post_order[i];
    }

    // Não temos componentes fortes ainda
    int n_components = 0;

    // Para cada vértice
    for (int i = 0; i < vpp_size; i++) {

        // fazemos a decomposição em componentes
        if ( decompose_recursive(g, vertices, &vertices[i], n_components, vpp_size) ) {

            // Se um componente novo foi criado, precisamos de outro valor para
            // identificar nosso novo componente
            n_components++;
        }
    }

    // Declaração de subgrafos de g
    Agraph_t** subgraphs = malloc(n_components * sizeof(Agraph_t*));

    // Associação de subgrafos com o grafo g
    for (int i = 0; i < n_components; i++){
        subgraphs[i] = agsubg(g, NULL, TRUE);
    }

    // Construindo os subgrafos de g
    if (n_components == 1) {

        // Caso g seja trivial
        subgraphs[0] = g;
    } else {

        // Caso g não seja trivial
        for (int i = 0; i < vpp_size; i++) {
            for (int j = 0; j < vpp_size; j++) {

                // Se existe aresta entre os vértices e são do mesmo componente
                if ( agedge(g, vertices[i].node, vertices[j].node, NULL, FALSE) && vertices[i].component == vertices[j].component) {

                    // Então o subgrafo é alterado de acordo
                    agedge(subgraphs[vertices[i].component], agnode(g, agnameof(vertices[i].node), FALSE), agnode(g, agnameof(vertices[j].node), FALSE), NULL, TRUE);
                }
            }
        }
    }

    return g;
}
