#include <graph.h>

void graph_to_graph_adj_matrix(graph_adj_matrix_t graph_adj_matrix, graph_t graph)
{
  for (size_t i = 0; i < graph.edges_count; i++)
    graph_adj_matrix.vertices[graph.edges[i][0]*graph_adj_matrix.vertices_count+graph.edges[i][1]] = 1;
  for (size_t i = 0; i < graph_adj_matrix.vertices_count; i++)
    graph_adj_matrix.vertices[i*graph_adj_matrix.vertices_count+i] = 1;
}

void graph_adj_matrix_to_graph(graph_adj_matrix_t graph_adj_matrix, graph_t graph)
{
  for (size_t i = 0; i < graph_adj_matrix.vertices_count * graph_adj_matrix.vertices_count; i++) {
    if (graph_adj_matrix.vertices[i] == 1) {
      graph.edges[graph.edges_count][0] = i / graph_adj_matrix.vertices_count;
      graph.edges[graph.edges_count][1] = i % graph_adj_matrix.vertices_count;
      graph.edges_count++;
    }
  }
}

static void graph_recurse_to_get_edges_for_g2l(graph_node_t *graph, vertex_t *vertex,
    vertex_t *last_vertex_addr, graph_node_t *head_addr)
{
  graph->next = malloc(sizeof(graph_node_t));
  graph->next->vertex = *vertex;
  graph->next->prev = graph;
  if (vertex + 1 == last_vertex_addr) {
    head_addr->prev = graph->next;
    graph->next->next = head_addr;
  } else graph_recurse_to_get_edges_for_g2l(graph->next, vertex + 1,
      last_vertex_addr, head_addr);
}

void graph_to_graph_adj_list(graph_adj_list_t graph_adj_list, graph_t graph)
{
  vertex_t edge_vertices[graph.edges_count];
  int32_t edge_vertices_count = 0;
  for (size_t i = 0; i < graph.vertices_count; i++) {
    edge_vertices_count = 0;
    graph_adj_list.items[i].head = malloc(sizeof(graph_node_t));
    graph_adj_list.items[i].head->vertex = graph.vertices[i];
    graph_adj_list.items[i].head->prev = NULL;
    for (size_t j = 0; j < graph.edges_count; j++) {
      if (graph.edges[j][0] == graph.vertices[i]) {
        edge_vertices[edge_vertices_count] = graph.edges[j][1];
        edge_vertices_count++;
      }
    }
    // TODO: Decide, to self loop or not to self loop? Self loop might
    // create problems, but represents the actual graph as each node does
    // "connects to itself".
    /* edge_vertices[edge_vertices_count] = i; */
    /* edge_vertices_count++; */
    graph_adj_list.items[i].size = edge_vertices_count;
    if (edge_vertices_count > 0)
      graph_recurse_to_get_edges_for_g2l(graph_adj_list.items[i].head, edge_vertices,
          &edge_vertices[edge_vertices_count], graph_adj_list.items[i].head);
    else graph_adj_list.items[i].head->next = NULL;
  }
}

static void graph_recurse_to_get_edges_for_l2g(graph_node_t *src_graph, graph_t
    *tgt_graph, vertex_t current_vertex, size_t count, size_t total_edges)
{
  if (count == total_edges) return;
  tgt_graph->edges[count][0] = current_vertex;
  tgt_graph->edges[count][1] = src_graph->vertex;
  graph_recurse_to_get_edges_for_l2g(src_graph->next, tgt_graph, current_vertex, count + 1, total_edges);
}

void graph_adj_list_to_graph(graph_adj_list_t graph_adj_list, graph_t graph)
{
  graph.vertices_count = graph_adj_list.vertices_count;
  graph.edges_count = graph_adj_list.edges_count;
  for (size_t i = 0; i < graph.vertices_count; i++) {
    graph.vertices[i] = graph_adj_list.items[i].head->vertex;
    size_t count = 0;
    if (graph_adj_list.items[i].size > 0)
      graph_recurse_to_get_edges_for_l2g(graph_adj_list.items[i].head->next,
          &graph, graph.vertices[i], count, graph.edges_count);
  }
}

graph_node_t* graph_adj_list_item_find(vertex_t vertex, graph_adj_list_item_t graph_adj_list_item)
{
  graph_node_t* addr;
  addr = graph_adj_list_item.head;
  while ((addr->next != NULL) && (addr->next->vertex != vertex))
    addr = addr->next;
  return addr;
}

void graph_free(graph_t graph)
{
  free(graph.vertices);
  free(graph.edges);
}

void graph_node_free(graph_node_t *graph)
{
  // Free every node except for head and the last node.
  for (graph_node_t *j = graph->next; j != NULL && j != graph; j = j->next) {
    if (j->prev != graph) free(j->prev);
  }
  // Free head and last node.
  free(graph->prev);
  free(graph);
}

void graph_adj_list_item_free(graph_adj_list_item_t *graph_adj_list_item)
{
  /* graph_node_free(graph_adj_list_item->head); */
  // Free every node except for head and the last node.
  for (graph_node_t *j = graph_adj_list_item->head->next; j != NULL && j != graph_adj_list_item->head; j = j->next) {
    if (j->prev != graph_adj_list_item->head) free(j->prev);
  }
  // Free head and last node.
  free(graph_adj_list_item->head->prev);
  free(graph_adj_list_item->head);
}

void graph_adj_list_free(graph_adj_list_t *graph_adj_list)
{
  for (size_t i = 0; i < graph_adj_list->vertices_count; i++) {
    // Free every node except for head and the last node.
    /* graph_adj_list_item_free(&graph_adj_list->items[i]); */
    for (graph_node_t *j = graph_adj_list->items[i].head->next; j != NULL && j != graph_adj_list->items[i].head; j = j->next) {
      if (j->prev != graph_adj_list->items[i].head) free(j->prev);
    }
    // Free head and last node.
    free(graph_adj_list->items[i].head->prev);
    free(graph_adj_list->items[i].head);
  }
  // Free items array.
  free(graph_adj_list->items);
}

void graph_adj_list_print(graph_adj_list_t graph_adj_list)
{
  for (size_t i = 0; i < graph_adj_list.vertices_count; i++) {
    printf("[%d] ", graph_adj_list.items[i].head->vertex);
    for (graph_node_t *j = graph_adj_list.items[i].head->next; j != NULL && j != graph_adj_list.items[i].head; j = j->next) {
      printf("%d ", j->vertex);
    }
    printf("\n");
  }
}

void graph_adj_matrix_print(graph_adj_matrix_t graph_adj_matrix)
{
  // Iterate rows, then columns inside it, when calculating indices,
  // graph_adj_matrix.vertices_count are columns.
  for (size_t i = 0; i < graph_adj_matrix.vertices_count; i++) {
    for (size_t j = 0; j < graph_adj_matrix.vertices_count; j++)
      printf(graph_adj_matrix.vertices[i*graph_adj_matrix.vertices_count+j] ? "O " : ". ");
    printf("\n");
  }
}
