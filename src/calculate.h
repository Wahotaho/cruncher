void go_infinite (struct position p) {
  root = create_node(p);
  int horizon = 2;
  build_tree (horizon);
}

void build_tree (int horizon) {
  root.depth = 0;

  if (horizon  > 1) {
    create_child_nodes(&root);
  }

  //struct node * n = &heads[1];
  //while (1==1) {
//    print_position(&n->pos);
  //  n = n->neighbor;
  //}
}

void create_child_nodes (struct node  * n) {
  int max_moves = n -> pos.num_moves;
  struct position working_position = (n -> pos);
  struct node working_node = create_node(working_position);
  int depth = n->depth + 1;

  working_node.pos = working_position;
  make_move(&working_node.pos, &(n -> pos.moves[0]));

  n -> children[0] = working_node;
  n -> children[0].ancestor = n;
  n -> children[0].depth = depth;

  if (heads[depth].neighbor == NULL) {
    heads[depth] = n -> children[0];
  }

  int i;

  for (i = 1; i < max_moves ; i++) {
    working_node.pos = working_position;
    make_move(&working_node.pos, &(n -> pos.moves[i]));

    n -> children[i - 1].neighbor = &n -> children[i];
    n -> children[i] = working_node;
    n -> children[i].ancestor = n;
    n -> children[i].depth = depth;
  }

  tails[depth] = n -> children[i];

  for (int i = 0; i < max_moves; i++) {
    print_position(&n -> children[i].pos);
    printf("%d\n",  n -> children[i].depth);
  }
}
