#include <search/dfs.h>

static bool check_if_exists(graph_adj_list_item_t *stack, graph_node_t *new_ptr)
{
  graph_node_t *current = stack->head;
  if (stack->size == 0) return false;
  while (current != NULL) {
    if (current->vertex == new_ptr->vertex) return true;
    current = current->next;
  }
  return false;
}

static int is_empty(graph_adj_list_item_t *stack)
{
  return (stack->head->next == NULL);
}

static void pop_node_stack(graph_adj_list_item_t *stack)
{
  if (is_empty(stack)) exit(1);
  else {
    stack->head->next->prev = stack->head->prev;
    stack->head = stack->head->next;
    stack->size--;
  }
}

static vertex_t top_node_stack(graph_adj_list_item_t *stack)
{
  vertex_t tmp = stack->head->vertex;
  pop_node_stack(stack);
  return tmp;
}

static void push_node_stack(graph_adj_list_item_t *stack, vertex_t vertex)
{
  graph_node_t *top = malloc(sizeof(graph_node_t));
  if (top == NULL) exit(1);
  top->vertex = vertex;
  if (stack->size == 0) {
    top->next = NULL;
    top->prev = NULL;
  } else {
    top->next = stack->head;
    if (stack->head->prev == NULL) top->prev = top->next;
    else top->prev = stack->head->prev;
    stack->head->prev = top;
  }
  stack->head = top;
  stack->size++;
}

static void populate_to_be_visited_stack(graph_adj_list_item_t *to_be_visited,
                                         graph_adj_list_item_t *visited,
                                         graph_node_t *node,
                                         graph_node_t *head)
{
  graph_node_t *current = node;
  while (current != head) {
    if (check_if_exists(to_be_visited, current) == false &&
        check_if_exists(visited, current) == false)
      // TODO: Check why the following leaks the memory of the newly
      // malloc'd top node four times.
      push_node_stack(to_be_visited, current->vertex);
    current = current->prev;
  }
}

void dfs_search(graph_t graph, graph_node_t *result, vertex_t start, vertex_t end)
{
  graph_adj_list_t graph_adj_list = {
      .items = malloc(sizeof(graph_adj_list_item_t) * graph.vertices_count),
      .vertices_count = graph.vertices_count,
      .edges_count = graph.edges_count,
  };
  graph_to_graph_adj_list(graph_adj_list, graph);

  // TODO: Check if a whole graph_adj_list_item is necessary or a simple
  // list of vertex is sufficient. The latter is more efficient and might
  // fix the memory leak issue;
  graph_adj_list_item_t to_be_visited = {.head = NULL, .size = 0};
  graph_adj_list_item_t visited = {.head = NULL, .size = 0};
  graph_node_t *current_ptr = graph_adj_list.items[start].head;
  graph_node_t *result_ptr = result;

  while (current_ptr->vertex != end) {
    if (current_ptr->next == NULL || check_if_exists(&visited, current_ptr)) {
      push_node_stack(&visited, current_ptr->vertex);
      current_ptr = current_ptr->vertex == start
              ? graph_adj_list.items[start + 1].head
              : graph_adj_list.items[top_node_stack(&to_be_visited)].head;
      continue;
    }
    current_ptr = graph_adj_list.items[current_ptr->vertex].head;

    result->vertex = current_ptr->vertex;
    result->next = malloc(sizeof(graph_node_t));
    result->next->prev = result;

    populate_to_be_visited_stack(&to_be_visited, &visited, current_ptr->prev, current_ptr);
    push_node_stack(&visited, current_ptr->vertex);
    current_ptr = graph_adj_list.items[top_node_stack(&to_be_visited)].head;

    result = result->next;
  }

  result->vertex = end;
  result->next = malloc(sizeof(graph_node_t));
  result->next = result_ptr;
  result_ptr->prev = result;

  graph_adj_list_item_free(&to_be_visited);
  graph_adj_list_item_free(&visited);
  graph_adj_list_free(&graph_adj_list);
}

void dfs_result_print(graph_t graph, graph_node_t *result) {

  graph_adj_matrix_t graph_adj_matrix = {
      .vertices = malloc(sizeof(vertex_t) * graph.vertices_count *
                         graph.vertices_count),
      .vertices_count = graph.vertices_count,
      .edges_count = graph.edges_count};
  graph_to_graph_adj_matrix(graph_adj_matrix, graph);

  vertex_t start = result->vertex, end = result->prev->vertex;
  // Should start with (1, 1), then (1, 2), (2, 3), (3, 8) {(3, 3), (3, 4)
  // ... (3, 8)} and lastly (8, 9) {(4, 8) ... (8, 8), (8, 9)}. This means
  // everytime the connection "jumps", we connect the dots with straight
  // lines to and from (and vice versa) the starting vertex's connection
  // to itself.

  for (size_t i = 0; i < graph_adj_matrix.vertices_count; i++) {
    for (size_t j = 0; j < graph_adj_matrix.vertices_count; j++)
      printf(graph_adj_matrix.vertices[i * graph_adj_matrix.vertices_count + j]
                 ? "O "
                 : ". ");
    printf("\n");
  }

  graph_node_t *iter = result;
  while (iter != NULL) {
    printf("%d\n", iter->vertex);
    iter = iter->next;
  }
}
