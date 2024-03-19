#ifndef DFS_H_INCLUDED
#define DFS_H_INCLUDED

#include <std.h>
#include <graph.h>

void dfs_search(graph_t graph, graph_node_t *result, vertex_t start, vertex_t end);
void dfs_result_print(graph_t graph, graph_node_t *result);

#endif /* DFS_H_INCLUDED */
