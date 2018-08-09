void go_infinite (struct position p) {
  root = create_node(p);
  int horizon = 4;

  heads[0] = root;
  struct node * n;

  int i = 0;

  for (; i < horizon; i++) {
    n = heads[i];
    while (n != NULL) {
      legal_moves (&n -> pos);
      create_child_nodes (n);
      n = n -> neighbor;
    }
  }

  n = heads[horizon];
  i = 1;
  while (n != NULL) {
    printf("%d\n", i);
    print_position(&n->pos);
    n = n -> neighbor;
    i++;
  }

}

void create_child_nodes (struct node  * n) {
  if (n->pos.num_moves == 0 || n->is_pruned) return;

  n -> children[0] = create_node(n->pos);
  n -> children[0] -> ancestor = n;
  n -> children[0] -> depth = n-> depth + 1;
  make_move(&n -> children[0]->pos, &n -> children[0]->pos.moves[0]);

  if (abs_double(n -> children[0]->pos.position_value) > CHECKMATE_VAL) {
    n -> children[0]->pos.game_ended = TRUE;
    n -> children[0]->is_pruned = TRUE;
  }

  if (heads[n -> children[0] -> depth] == NULL) heads[n -> children[0] -> depth] = n -> children[0];
  else tails[n -> children[0] -> depth] -> neighbor = n -> children[0];

  for (int i = 1; i < n -> pos.num_moves; i++) {
    n -> children[i] = create_node(n->pos);
    n -> children[i] -> ancestor = n;
    n -> children[i] -> depth = n -> children[0] -> depth;
    make_move(&n -> children[i]->pos, &n -> children[i]->pos.moves[i]);
    n -> children[i - 1] -> neighbor = n -> children[i];

    if (abs_double(n -> children[i]->pos.position_value) > CHECKMATE_VAL) {
      n -> children[i]->pos.game_ended = TRUE;
      n -> children[0]->is_pruned = TRUE;
    }
  }

  tails[n -> children[0] -> depth] = n -> children[n -> pos.num_moves - 1];

}
