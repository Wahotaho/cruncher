void go_infinite (struct position p) {
  root = create_node(p);
  int horizon = 2;

  heads[0] = root;
  struct node * n;

  int i = 0;

  for (; i < horizon; i++) {
    n = heads[i];
    while (n != NULL) {
      create_child_nodes (n);
      n = n -> neighbor;
    }
  }

  n = heads[2];
  i = 1;
  while (n != NULL) {
    printf("%d\n", i);
    print_position(&n->pos);
    n = n -> neighbor;
    i++;
  }
}

void create_child_nodes (struct node  * n) {
  if (n->pos.num_moves == 0 && !(n->is_pruned)) return;

  n -> children[0] = create_node(n->pos);
  n -> children[0] -> ancestor = n;
  n -> children[0] -> depth = n-> depth + 1;
  make_move(&n -> children[0]->pos, &n -> children[0]->pos.moves[0]);

  if (heads[n -> children[0] -> depth] == NULL) {
    insertHead(&heads[n -> children[0] -> depth],n -> children[0]);
  }

  //printf("%d\n", n->depth);
  //print_position(&n-> pos);
  printf("%d\n", n -> children[0]->depth);
  print_position(&n -> children[0]->pos);
  printf("Num moves: %d\n", n -> pos.num_moves);

  for (int i = 1; i < n -> pos.num_moves; i++) {
    n -> children[i] = create_node(n->pos);
    n -> children[i] -> ancestor = n;
    n -> children[i] -> depth = n -> children[0] -> depth;
    make_move(&n -> children[i]->pos, &n -> children[i]->pos.moves[i]);
    insertTail (&heads[n -> children[i] -> depth], n -> children[i]);
    printf("%d\n", n -> children[i]->depth);
    print_position(&n -> children[i]->pos);
  }

}
