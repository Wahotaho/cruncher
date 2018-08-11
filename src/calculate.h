void go_infinite (struct position p) {
  root = create_node(p);
  int horizon = 4;

  heads[0] = root;
  struct node * n;

  n = heads[0];
  create_child_nodes (n);

  int i = 1;

  for (; i < horizon; i++) {
    calculate_next_ply (i);
  }

  n = heads[horizon];
  i = 1;
  double ma = n->pos.position_value;
  while (n != NULL) {
    printf("%d\n", i);
    //printf("previous moves  %d\n", n->pos.previous_num_moves);
    //printf("current moves    %d\n", n->pos.num_moves);
    //printf("%f\n", n->pos.position_value);
    print_position(&n->pos);
    if (n->pos.position_value > ma) ma = n->pos.position_value;
    n = n -> neighbor;
    i++;
  }
}

void calculate_next_ply (int i) {
  struct node * n; int j = 0;
  n = heads[i];
  while (n != NULL) {
    while (j < n -> ancestor -> pos.num_moves) {
      n = n -> ancestor -> children[j];
      create_child_nodes (n);
      j++;
    }
    n = n -> neighbor;
    j = 0;
  }
}

double max (struct node * n) {
  double ret = n->children[0]->pos.position_value;

  for (int i = 1; i < n->pos.num_moves; i++) {
    if (ret < n->children[i]->pos.position_value) ret = n->children[0]->pos.position_value;
  }

  return ret;
}

double min (struct node * n) {
  double ret = n->children[0]->pos.position_value;

  for (int i = 1; i < n->pos.num_moves; i++) {
    if (ret > n->children[i]->pos.position_value) ret = n->children[0]->pos.position_value;
  }

  return ret;
}

void create_child_nodes (struct node  * n) {
  if (n->pos.num_moves == 0 || n->is_pruned) return;

  struct position  working_position = n->pos;
  working_position.position_value = 0;
  working_position.material_value = 0;
  working_position.mobility_value = 0;

  n -> children[0] = create_node(working_position);
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
    n -> children[i] = create_node(working_position);
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
