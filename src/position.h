struct position position_init (char * str, char side, char * castling_options, char * en_passant, double half_moves, int full_moves) {
  struct position pos;

  strcpy (pos.position_string, str);
  load_board(&pos);
  pos.side_move = side;

  pos.white_can_castle_kingside = string_contains (castling_options, 'K');
  pos.white_can_castle_queenside = string_contains (castling_options, 'Q');
  pos.black_can_castle_kingside = string_contains (castling_options, 'k');
  pos.black_can_castle_queenside = string_contains (castling_options, 'q');

  // find Kings
  pos.white_king_rank = -1;
  pos.white_king_file = -1;
  pos.black_king_rank = -1;
  pos.black_king_file = -1;

  for (int i = 0; i < LEN; i++) {
    for (int j = 0; j < WID; j++) {
      if (pos.board[i][j] == 'K') {
        pos.white_king_rank = i;
        pos.white_king_file = j;
      } else if (pos.board[i][j] == 'k') {
        pos.black_king_rank = i;
        pos.black_king_file = j;
      }
    }
  }

  if (!(pos.board[7][4] == 'K') && !(pos.board[7][7] == 'R')) {
    pos.white_can_castle_kingside = FALSE;
  } if (!(pos.board[7][4] == 'K') && !(pos.board[7][0] == 'R')) {
    pos.white_can_castle_queenside = FALSE;
  } if (!(pos.board[0][4] == 'k') && !(pos.board[0][7] == 'r')) {
    pos.black_can_castle_kingside = FALSE;
  } if (!(pos.board[0][4] == 'k') && !(pos.board[0][0] == 'r')) {
    pos.black_can_castle_queenside = FALSE;
  }

  if (strlen(en_passant) == 1) {
    pos.en_passant_rank = -1;
    pos.en_passant_file = -1;
  } else {
    pos.en_passant_file = en_passant[0] - 97;
    pos.en_passant_rank = LEN - (en_passant[1] - 48);
  }

  pos.half_move_clock = half_moves;
  pos.full_move_number = full_moves;

  // figure out initial position value
  legal_moves (&pos);

  struct position other_color = pos;

  if (other_color.side_move == 'w')  other_color.side_move = 'b';
  else other_color.side_move = 'w';

  legal_moves(&other_color);

  pos.previous_num_moves = other_color.previous_num_moves;

  evaluate_position(&pos);

  // check if king is still there
  if (abs_double(pos.position_value) > CHECKMATE_VAL) {
    pos.game_ended = TRUE;
    pos.num_moves = 0;
  }

  pos.previous_num_moves = 0;

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

  // RIGHT MOVES
  if (file + 2 < WID) {
    // UP
    if (rank - 1 > - 1) {
      if (pos -> board [rank - 1][file + 2] == 'n') return TRUE;
    }

    // DOWN
    if (rank - 1 > - 1) {
      if (pos -> board [rank + 1][file + 2] == 'n') return TRUE;
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
  // UP

while (working_rank > 0) {
  working_rank--;
  if (pos -> board [working_rank][working_file] == 'q' || pos -> board [working_rank][working_file] == 'r') return TRUE;
  else if (is_white(pos -> board [working_rank][working_file]) || is_black(pos -> board [working_rank][working_file])) break;
}

working_rank = rank;

// DOWN

while (working_rank + 1 < LEN) {
  working_rank++;
  if (pos -> board [working_rank][working_file] == 'q' || pos -> board [working_rank][working_file] == 'r') return TRUE;
  else if (is_white(pos -> board [working_rank][working_file]) || is_black(pos -> board [working_rank][working_file])) break;
}

working_rank = rank;

// LEFT

while (working_file > 0) {
  working_file--;
  if (pos -> board [working_rank][working_file] == 'q' || pos -> board [working_rank][working_file] == 'r') return TRUE;
  else if (is_white(pos -> board [working_rank][working_file]) || is_black(pos -> board [working_rank][working_file])) break;
}

working_file = file;

// RIGHT

while (working_file + 1< WID) {
  working_file++;
  if (pos -> board [working_rank][working_file] == 'q' ||
   pos -> board [working_rank][working_file] == 'r') return TRUE;
  else if (is_white(pos -> board [working_rank][working_file]) || is_black(pos -> board [working_rank][working_file])) break;
}

  return FALSE;
}

int black_king_checked (struct position * pos, char rank, char file) {

  // Check from pawn

  if (rank < LEN - 2) {
    // Left
    if (file > 0) {
      if (pos -> board [rank + 1][file - 1] == 'P') return TRUE;
    }

    // Right
    if (file < WID - 1) {
      if (pos -> board [rank + 1][file + 1] == 'P') return TRUE;
    }

  }

  // Check from knight //////////////////////////////////////////////////////////////////////

  // UP MOVES
  if (rank - 2 > -1) {
    // LEFT
    if (file - 1 > - 1) {
      if (pos -> board [rank - 2][file - 1] == 'N') return TRUE;
    }

    // RIGHT
    if (file + 1 < WID) {
      if (pos -> board [rank - 2][file + 1] == 'N') return TRUE;
    }
  }

  // DOWN MOVES
  if (rank + 2 < LEN) {
    // LEFT
    if (file - 1 > - 1) {
      if (pos -> board [rank + 2][file - 1] == 'N') return TRUE;
    }

    // RIGHT
    if (file + 1 < WID) {
      if (pos -> board [rank + 2][file + 1] == 'N') return TRUE;
    }
  }

  // LEFT MOVES
  if (file - 2 > -1) {
    // UP
    if (rank - 1 > - 1) {
      if (pos -> board [rank - 1][file - 2] == 'N') return TRUE;
    }

    // DOWN
    if (rank + 1 < LEN) {
      if (pos -> board [rank + 1][file - 2] == 'N') return TRUE;
    }
  }

  // RIGHT MOVES
  if (file + 2 < WID) {
    // UP
    if (rank - 1 > - 1) {
      if (pos -> board [rank - 1][file + 2] == 'N') return TRUE;
    }

    // DOWN
    if (rank - 1 > - 1) {
      if (pos -> board [rank + 1][file + 2] == 'N') return TRUE;
    }
  }

  // Check from King ////////////////////////////////////////////////////////////////////////
  // UP
  if (rank > 0) {
    //Straight
    if (pos -> board [rank - 1][file + 0] == 'K') return TRUE;
    //Left
    if (file > 0) if (pos -> board [rank - 1][file - 1]  == 'K') return TRUE;
    // Right
    if (file + 1 < WID) if (pos -> board [rank - 1][file + 1]  == 'K') return TRUE;
  }

  // DOWN
  if (rank < WID - 1) {
    //Straight
    if (pos -> board [rank + 1][file + 0]  == 'K') return TRUE;

    //Left
    if (file > 0) {
      if (pos -> board [rank + 1][file - 1]  == 'K') return TRUE;
    }

    if (file + 1 < WID) {
      if (pos -> board [rank + 1][file + 1] == 'K') return TRUE;
    }
  }

  // LEFT
  if (file > 0) {
    if (pos -> board [rank + 0][file - 1] == 'K') return TRUE;
  }

  // RIGHT
  if (file < WID - 1) {
    if (pos -> board [rank + 0][file + 1] == 'K') return TRUE;
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////
  int working_rank = rank;
  int working_file = file;

  // Check from bishop or queen diagonal ///////////////////////////////////////////////////////////////////////////////////
  // UP Right

  while (working_rank > 0 && working_file + 1 < WID) {
    working_rank--;
    working_file++;
      if (pos -> board [working_rank][working_file] == 'Q' || pos -> board [working_rank][working_file] == 'B') return TRUE;
      else if (is_white(pos -> board [working_rank][working_file]) || is_black(pos -> board [working_rank][working_file])) break;
    }

    working_rank = rank;
    working_file = file;

    // Down Right

    while (working_rank + 1 < LEN && working_file + 1 < WID) {
      working_rank++;
      working_file++;
      if (pos -> board [working_rank][working_file] == 'Q' || pos -> board [working_rank][working_file] == 'B') return TRUE;
      else if (is_white(pos -> board [working_rank][working_file]) || is_black(pos -> board [working_rank][working_file])) break;
    }

    working_rank = rank;
    working_file = file;

    // Down Left

    while (working_rank + 1 < LEN && working_file > 0) {
      working_rank++;
      working_file--;
      if (pos -> board [working_rank][working_file] == 'Q' || pos -> board [working_rank][working_file] == 'B') return TRUE;
      else if (is_white(pos -> board [working_rank][working_file]) || is_black(pos -> board [working_rank][working_file])) break;
    }

    working_rank = rank;
    working_file = file;

    // UP Left

  while (working_rank > 0 && working_file > 0) {
    working_rank--;
    working_file--;
      if (pos -> board [working_rank][working_file] == 'Q' || pos -> board [working_rank][working_file] == 'B') return TRUE;
      else if (is_white(pos -> board [working_rank][working_file]) || is_black(pos -> board [working_rank][working_file])) break;
  }

  working_rank = rank;
  working_file = file;

  // Check from queen or rook straight ///////////////////////////////////////////////////////////////////////////////////////////
  // UP

while (working_rank > 0) {
  working_rank--;
  if (pos -> board [working_rank][working_file] == 'Q' || pos -> board [working_rank][working_file] == 'R') return TRUE;
  else if (is_white(pos -> board [working_rank][working_file]) || is_black(pos -> board [working_rank][working_file])) break;
}

working_rank = rank;

// DOWN

while (working_rank + 1 < LEN) {
  working_rank++;
  if (pos -> board [working_rank][working_file] == 'Q' || pos -> board [working_rank][working_file] == 'R') return TRUE;
  else if (is_white(pos -> board [working_rank][working_file]) || is_black(pos -> board [working_rank][working_file])) break;
}

working_rank = rank;

// LEFT

while (working_file > 0) {
  working_file--;
  if (pos -> board [working_rank][working_file] == 'Q' || pos -> board [working_rank][working_file] == 'R') return TRUE;
  else if (is_white(pos -> board [working_rank][working_file]) || is_black(pos -> board [working_rank][working_file])) break;
}

working_file = file;

// RIGHT

while (working_file + 1< WID) {
  working_file++;
  if (pos -> board [working_rank][working_file] == 'Q' || pos -> board [working_rank][working_file] == 'R') return TRUE;
  else if (is_white(pos -> board [working_rank][working_file]) || is_black(pos -> board [working_rank][working_file])) break;
}

  return FALSE;
}
