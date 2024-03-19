#ifndef GRAPH_H_INCLUDE
#define GRAPH_H_INCLUDE

#include <std.h>

typedef int32_t vertex_t;
typedef vertex_t edge_t[2];

typedef struct graph_t {
  vertex_t *vertices;
  size_t vertices_count;
  edge_t *edges;
  size_t edges_count;
} graph_t;

typedef struct graph_node_t {
  vertex_t vertex;
  struct graph_node_t *next;
  struct graph_node_t *prev;
} graph_node_t ;

typedef struct graph_node_weighted_t {
  vertex_t vertex;
  int32_t weight;
  struct graph_node_weighted_t *next;
  struct graph_node_weighted_t *prev;
} graph_node_weighted_t ;

typedef struct graph_adj_list_item_t {
  graph_node_t *head;
  size_t size;
} graph_adj_list_item_t;

// Implemented as an array of doubly circular linked list with a header.
typedef struct graph_adj_list_t {
  graph_adj_list_item_t *items;
  size_t vertices_count;
  size_t edges_count;
} graph_adj_list_t;

// Implemented as a vector of size row times column.
typedef struct graph_adj_matrix_t {
  vertex_t *vertices;
  size_t vertices_count; // used as rows and columns
  size_t edges_count; 
} graph_adj_matrix_t;

void graph_to_graph_adj_matrix(graph_adj_matrix_t graph_adj_matrix, graph_t graph);
void graph_adj_matrix_to_graph(graph_adj_matrix_t graph_adj_matrix, graph_t graph);
void graph_to_graph_adj_list(graph_adj_list_t graph_adj_list, graph_t graph);
void graph_adj_list_to_graph(graph_adj_list_t graph_adj_list, graph_t graph);

graph_node_t* graph_adj_list_item_find(vertex_t vertex, graph_adj_list_item_t graph_adj_list_item);
/* void graph_adj_list_delete(vertex_t vertex, graph_adj_list_t graph_adj_list); */

void graph_free(graph_t graph);
void graph_node_free(graph_node_t *graph);
void graph_adj_list_item_free(graph_adj_list_item_t *graph_adj_list_item);
void graph_adj_list_free(graph_adj_list_t *graph_adj_list);

void graph_adj_list_print(graph_adj_list_t graph_adj_list);
void graph_adj_matrix_print(graph_adj_matrix_t graph_adj_matrix);

#endif /* GRAPH_H_INCLUDE */
