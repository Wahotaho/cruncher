struct position position_init (char * str, char side, char * castling_options, char * en_passant, double half_moves, int full_moves) {
  struct position pos;

  strcpy (pos.position_string, str);
  pos.side_move = side;

  pos.white_can_castle_kingside = string_contains (castling_options, 'K');
  pos.white_can_castle_queenside = string_contains (castling_options, 'Q');
  pos.black_can_castle_kingside = string_contains (castling_options, 'k');
  pos.black_can_castle_queenside = string_contains (castling_options, 'q');

  if (strlen(en_passant) == 1) {
    pos.en_passant_rank = -1;
    pos.en_passant_file = -1;
  } else {
    pos.en_passant_file = en_passant[0] - 97;
    pos.en_passant_rank = LEN - (en_passant[1] - 48);
  }

  pos.half_move_clock = half_moves;
  pos.full_move_number = full_moves;
  load_board(&pos);
  evaluate_position(&pos);
  legal_moves (&pos);

  return pos;
}

void print_position (struct position * pos) {
  printf(" ");
  for (int i = 0; i < WID * 2 - 1; i++) printf("_");
  printf("\n");
  for (int i = 0; i < LEN; i++) {
    printf("%s", "|");
    for (int j = 0; j < WID; j++) {
      printf("%c|", pos -> board[i][j]);
    }
    printf("\n");
  }
  printf("%s\n", "");
}

int white_king_checked (struct position * pos, char rank, char file) {

  // Check from pawn

  if (rank > 1) {
    // Left
    if (file > 0) {
      if (pos -> board [rank - 1][file - 1] == 'p') return TRUE;
    }

    // Right
    if (file < WID - 1) {
      if (pos -> board [rank - 1][file + 1] == 'p') return TRUE;
    }

  }

  // Check from knight //////////////////////////////////////////////////////////////////////

  // UP MOVES
  if (rank - 2 > -1) {
    // LEFT
    if (file - 1 > - 1) {
      if (pos -> board [rank - 2][file - 1] == 'n') return TRUE;
    }

    // RIGHT
    if (file + 1 < WID) {
      if (pos -> board [rank - 2][file + 1] == 'n') return TRUE;
    }
  }

  // DOWN MOVES
  if (rank + 2 < LEN) {
    // LEFT
    if (file - 1 > - 1) {
      if (pos -> board [rank + 2][file - 1] == 'n') return TRUE;
    }

    // RIGHT
    if (file + 1 < WID) {
      if (pos -> board [rank + 2][file + 1] == 'n') return TRUE;
    }
  }

  // LEFT MOVES
  if (file - 2 > -1) {
    // UP
    if (rank - 1 > - 1) {
      if (pos -> board [rank - 1][file - 2] == 'n') return TRUE;
    }

    // DOWN
    if (rank + 1 < LEN) {
      if (pos -> board [rank + 1][file - 2] == 'n') return TRUE;
    }
  }

  // Check from King ////////////////////////////////////////////////////////////////////////
  // UP
  if (rank > 0) {
    //Straight
    if (pos -> board [rank - 1][file + 0] == 'k') return TRUE;
    //Left
    if (file > 0) if (pos -> board [rank - 1][file - 1]  == 'k') return TRUE;
    // Right
    if (file + 1 < WID) if (pos -> board [rank - 1][file + 1]  == 'k') return TRUE;
  }

  // DOWN
  if (rank < WID - 1) {
    //Straight
    if (pos -> board [rank + 1][file + 0]  == 'k') return TRUE;

    //Left
    if (file > 0) {
      if (pos -> board [rank + 1][file - 1]  == 'k') return TRUE;
    }

    if (file + 1 < WID) {
      if (pos -> board [rank + 1][file + 1] == 'k') return TRUE;
    }
  }

  // LEFT
  if (file > 0) {
    if (pos -> board [rank + 0][file - 1] == 'k') return TRUE;
  }

  // RIGHT
  if (file < WID - 1) {
    if (pos -> board [rank + 0][file + 1] == 'k') return TRUE;
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////
  int working_rank = rank;
  int working_file = file;

  // Check from bishop or queen diagonal ///////////////////////////////////////////////////////////////////////////////////
  // UP Right

  while (working_rank > 0 && working_file + 1 < WID) {
    working_rank--;
    working_file++;
      if (pos -> board [working_rank][working_file] == 'q' || pos -> board [working_rank][working_file] == 'b') return TRUE;
      else if (is_white(pos -> board [working_rank][working_file]) || is_black(pos -> board [working_rank][working_file])) break;
    }

    working_rank = rank;
    working_file = file;

    // Down Right

    while (working_rank + 1 < LEN && working_file + 1 < WID) {
      working_rank++;
      working_file++;
      if (pos -> board [working_rank][working_file] == 'q' || pos -> board [working_rank][working_file] == 'b') return TRUE;
      else if (is_white(pos -> board [working_rank][working_file]) || is_black(pos -> board [working_rank][working_file])) break;
    }

    working_rank = rank;
    working_file = file;

    // Down Left

    while (working_rank + 1 < LEN && working_file > 0) {
      working_rank++;
      working_file--;
      if (pos -> board [working_rank][working_file] == 'q' || pos -> board [working_rank][working_file] == 'b') return TRUE;
      else if (is_white(pos -> board [working_rank][working_file]) || is_black(pos -> board [working_rank][working_file])) break;
    }

    working_rank = rank;
    working_file = file;

    // UP Left

  while (working_rank > 0 && working_file > 0) {
    working_rank--;
    working_file--;
      if (pos -> board [working_rank][working_file] == 'q' || pos -> board [working_rank][working_file] == 'b') return TRUE;
      else if (is_white(pos -> board [working_rank][working_file]) || is_black(pos -> board [working_rank][working_file])) break;
  }

  working_rank = rank;
  working_file = file;

  // Check from queen or rook straight ///////////////////////////////////////////////////////////////////////////////////////////

  return FALSE;
}
