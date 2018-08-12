void go_infinite (struct position p) {
  root = create_node(p);
  int horizon = 5;

  heads[0] = root;
  struct node * n;

  n = heads[0];
  create_child_nodes (n);

  for (int k = 0; k < n ->pos.num_moves; k++) {
    printf("%f ", n -> children[k]->pos.position_value);
    print_move(&n ->pos.moves[k], &n ->pos);
   printf("%s\n", "");
  }

  int i = 1;

  for (; i < horizon; i++) {
    calculate_next_ply (i);
  }

  i = horizon - 1;
  while (i != -1) {
    n = heads[i];
    calculate_depth_minimax_val(n);
    i--;
  }

  //
  n=heads[0];

  double ma = 0.0;

  if (n->pos.side_move == 'w') ma = max (n);
  else ma = min (n);

  n=heads[1];

  i = 0;
  while (n != NULL) {
    if (n->pos.position_value == ma) break;
    n = n -> neighbor;
    i++;
  }

  printf("%f\n", n->pos.position_value);
  printf("ma  %f\n", ma);
  print_position(&n->pos);

  n = heads[0];

 for (int k = 0; k < n ->pos.num_moves; k++) {
    printf("%f ", n -> children[k]->pos.position_value);
    print_move(&n ->pos.moves[k], &n ->pos);
    printf("%s\n", "");
  }
}

void calculate_depth_minimax_val (struct node * n) {

  if (n -> pos.side_move == 'w') {
      while (n != NULL) {
        n -> pos.position_value = max (n);
        n = n -> neighbor;
      }
  } else {
    while (n != NULL) {
      n -> pos.position_value = min (n);
      n = n -> neighbor;
    }
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
  if (n->pos.num_moves == 0 || n->is_pruned || n->pos.game_ended) return n->pos.position_value;
  double ret = n->children[0]->pos.position_value;


  for (int i = 1; i < n->pos.num_moves; i++) {
    if (ret < n->children[i]->pos.position_value) ret = n->children[i]->pos.position_value;
  }
  return ret;
}

double min (struct node * n) {
  if (n->pos.num_moves == 0 || n->is_pruned || n->pos.game_ended) return n->pos.position_value;

  double ret = n->children[0]->pos.position_value;
  for (int i = 1; i < n->pos.num_moves; i++) {
    if (ret > n->children[i]->pos.position_value) ret = n->children[i]->pos.position_value;
  }
  return ret;
}

void create_child_nodes (struct node  * n) {
  if (n->pos.num_moves == 0 || n->is_pruned || n->pos.game_ended) return;

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
      n -> children[i]->pos.num_moves = 0;
    }
  }

  tails[n -> children[0] -> depth] = n -> children[n -> pos.num_moves -  1];
}
