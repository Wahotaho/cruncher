struct node {
  struct node * ancestor;

  struct position pos;

  int is_pruned;
  double node_value;
  int depth;

  struct node * neighbor;
  struct node * nearest_unpruned_neighbor;

  int num_children;
  struct node * children;
};

struct node create_node (struct position p) {
  struct node n;
  n.pos = p;
  n.ancestor = NULL;
  n.is_pruned = FALSE;
  n.neighbor = NULL;
  n.nearest_unpruned_neighbor = NULL;
  n.children = malloc(sizeof(struct node) * POSSIBLE_MOVES);
  n.num_children = 0;
  return n;
}
