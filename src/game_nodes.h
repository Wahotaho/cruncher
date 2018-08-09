struct node {
  struct node * ancestor;

  struct position pos;

  int is_pruned;
  int depth;

  struct node * neighbor;

  struct node ** children;
};

struct node * create_node (struct position p) {
  struct node * n = malloc(sizeof(struct node));
  n->pos = p;
  n->ancestor = NULL;
  n->is_pruned = FALSE;
  n->neighbor = NULL;
  n->children = malloc(sizeof(root) * POSSIBLE_MOVES);
  return n;
}

void insertTail (struct node **headRef, struct node *n) {

  if (*headRef != NULL) {
    struct node * lastNode = *headRef;

	   while (lastNode -> neighbor != NULL) {
		     lastNode = lastNode-> neighbor;
	      }

        lastNode -> neighbor = n;
      } else *headRef = n;
  }

  void insertHead (struct node ** headRef, struct node *n) {
    n -> neighbor = *headRef;
  *headRef = n;
  }
