// String move from structure
void print_move (struct move * m, struct position * p) {

  printf("%c", m -> start_file + 97);
  printf("%d", WID - m -> start_rank);
  printf("%c", m -> end_file + 97);
  printf("%d", LEN - m -> end_rank);

  if (m -> is_en_passant) {
    printf("%s", "ep");
  } else if (m -> end_rank == 0 && p -> board[m -> start_rank][m -> start_file] == 'P') {
    printf("%c", toUpper(m->promotion_piece));
  } else if (m -> end_rank == 7 && p -> board[m -> start_rank][m -> start_file] == 'p') {
    printf("%c", toLower(m->promotion_piece));
  }
}
//

// Regular move

struct move move_init (int f1, int r1, int f2, int r2, char p, _Bool ep) {
  struct move mov;
  mov.start_rank = r1;
  mov.start_file = f1;
  mov.end_rank = r2;
  mov.end_file = f2;
  mov.promotion_piece = p;
  mov.is_en_passant = ep;
  mov.is_castling = FALSE;
  return mov;
}

// Move passed in using coordinate system: For example e2e4 or e1g1 or h2h1N or e5f6ep
struct move move_init_from_string (char * move_str) {

  struct move mov;

  char len = strlen (move_str);

  mov.start_file = move_str[0] - 97;
  mov.start_rank = LEN - (move_str[1] - 48);
  mov.end_file = move_str[2] - 97;
  mov.end_rank = LEN - (move_str[3] - 48);

  if (len == 5) mov.promotion_piece = move_str[4];
  else mov.promotion_piece = 'x';

  if (len == 6) mov.is_en_passant = TRUE;
  else mov.is_en_passant = 0;

  return mov;
}

void make_move (struct position * pos, struct move * mov) {

  // figure out 50 move rule

  if (is_piece(pos -> board [mov -> end_rank][mov -> end_file]) || toUpper (pos -> board[mov -> start_rank][mov -> start_file]) == 'P') {
    pos -> half_move_clock = 0;
  } else {
    pos -> half_move_clock += .5;
  }

  if (pos -> half_move_clock == 50) {
    pos -> game_ended = TRUE;
    pos -> position_value = 0;
  }

  if (mov -> promotion_piece == 'x') mov -> promotion_piece = pos -> board [mov -> start_rank][mov -> start_file];

  if (toUpper(pos -> board [mov -> start_rank][mov -> start_file]) == 'K' && abs(mov -> start_file - mov -> end_file) == 2) mov -> is_castling = TRUE;
  else mov -> is_castling = 0;

  pos -> board [mov -> end_rank][mov -> end_file] = mov -> promotion_piece;
  pos -> board [mov -> start_rank][mov -> start_file] = '_';

  if (mov -> is_en_passant) {
    if (pos -> side_move == 'w') pos -> board [mov -> end_rank + 1][mov -> end_file] = '_';
    else pos -> board [mov -> end_rank - 1][mov -> end_file] = '_';
  }

  if (mov -> is_castling) {
    if (mov -> end_rank == 7 && mov -> end_file == 6) { // white king side castling
      pos -> board [7][7] = '_';
      pos -> board [7][5] = 'R';
      pos -> white_can_castle_kingside = FALSE;
      pos -> white_can_castle_queenside = FALSE;
    } else if (mov -> end_rank == 7 && mov -> end_file == 2) { // white queen side castling
      pos -> board [7][0] = '_';
      pos -> board [7][3] = 'R';
      pos -> white_can_castle_kingside = FALSE;
      pos -> white_can_castle_queenside = FALSE;
    } else if (mov -> end_rank == 0 && mov -> end_file == 6) { // black king side castling
      pos -> board [0][7] = '_';
      pos -> board [0][5] = 'r';
      pos -> black_can_castle_kingside = FALSE;
      pos -> black_can_castle_queenside = FALSE;
    } else {                                                                          // black queen side castling
      pos -> board [0][0] = '_';
      pos -> board [0][3] = 'r';
      pos -> black_can_castle_kingside = FALSE;
      pos -> black_can_castle_queenside = FALSE;
    }
  }

  // Check Kings and Rooks to see if castling option exists

  if (pos -> board [7][4] != 'K') {
    pos -> white_can_castle_kingside = FALSE;
    pos -> white_can_castle_queenside = FALSE;
  } else {
    if (pos -> board [7][7] != 'R') {
      pos -> white_can_castle_kingside = FALSE;
    } if (pos -> board [7][0] != 'R') {
      pos -> white_can_castle_queenside = FALSE;
    }
}

  if (pos -> board [0][4] != 'k'){
    pos -> black_can_castle_kingside = FALSE;
    pos -> black_can_castle_queenside = FALSE;
  } else {
    if (pos -> board [0][7] != 'r') {
      pos -> black_can_castle_kingside = FALSE;
    } if (pos -> board [0][0] != 'r') {
      pos -> black_can_castle_queenside = FALSE;
    }
}

  if (pos -> side_move == 'w') {

    if (mov -> start_rank - mov -> end_rank == 2 && pos -> board[mov -> end_rank][mov -> end_file] == 'P') {
      pos -> en_passant_rank = mov -> start_rank -  1;
      pos -> en_passant_file = mov -> end_file;
    } else {
      pos -> en_passant_rank = -1;
      pos -> en_passant_rank = -1;
    }

    // change side to move
    pos -> side_move = 'b';
  } else {

    if (mov -> start_rank - mov -> end_rank == -2 && pos -> board[mov -> end_rank][mov -> end_file] == 'p') {
      pos -> en_passant_rank = mov ->start_rank +  1;
      pos -> en_passant_file = mov ->end_file;
    } else {
      pos -> en_passant_rank = -1;
      pos -> en_passant_rank = -1;
    }

     // change side to move
    pos -> side_move = 'w';
    pos -> full_move_number += 1;
  }

  int temp = pos->num_moves;
  pos->previous_num_moves = temp;
  legal_moves(pos);
  evaluate_position(pos);
}
//

// Find legal moves
void legal_moves (struct position * p) {
  int i,j;

  p -> num_moves = 0;
  if (p -> game_ended == TRUE) return;

  p -> moves = malloc(sizeof(struct move) * POSSIBLE_MOVES);

  if (p -> side_move == 'w') {
    white_kingside_castle (p);
    white_queenside_castle (p);
    for (i = 0; i < LEN; i++) {
        for (j = 0; j < WID; j++) {
          if (p -> board[i][j] == 'K') {
            white_king_moves (p, i, j);
          } else if (p -> board[i][j] == 'Q') {
            white_rook_moves (p, i, j);
            white_bishop_moves (p, i, j);
          } else if (p -> board[i][j] == 'B') {
            white_bishop_moves (p, i, j);
          }  else if (p -> board[i][j] == 'N') {
            white_knight_moves(p,i,j);
          }  else if (p -> board[i][j] == 'R') {
            white_rook_moves (p, i, j);
          }  else if (p -> board[i][j] == 'P') {
            white_pawn_moves (p,i,j);
          }
        }
    }
  } else {
    black_kingside_castle (p);
    black_queenside_castle (p);
    for (i = 0; i < LEN; i++) {
        for (j = 0; j < WID; j++) {
          if (p -> board[i][j] == 'k') {
            black_king_moves (p, i, j);
          } else if (p -> board[i][j] == 'q') {
            black_rook_moves (p, i, j);
            black_bishop_moves (p,i,j);
          } else if (p -> board[i][j] == 'b') {
            black_bishop_moves (p,i,j);
          }  else if (p -> board[i][j] == 'n') {
            black_knight_moves(p,i,j);
          }  else if (p -> board[i][j] == 'r') {
            black_rook_moves (p, i, j);
          }  else if (p -> board[i][j] == 'p') {
            black_pawn_moves (p,i,j);
          }
        }
    }
  }

  if (p -> num_moves == 0) p -> game_ended = TRUE;
}
//

void white_rook_moves (struct position * p, char rank, char file) {
  int working_rank = rank;
  int working_file = file;

  char promotion_piece = p -> board [working_rank][working_file];

    // UP

  while (working_rank > 0) {
    working_rank--;

    if (is_white(p -> board [working_rank][working_file])) break;

      p -> moves[p -> num_moves] = move_init(file, rank, working_file, working_rank, promotion_piece, 0);
      //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
      //printf("%s\n", "");

      p -> num_moves++;

      if (is_black(p -> board [working_rank][working_file])) break;
  }

  working_rank = rank;

  // DOWN

  while (working_rank + 1 < LEN) {
    working_rank++;

    if (is_white(p -> board [working_rank][working_file])) break;

      p -> moves[p -> num_moves] = move_init(file, rank, working_file, working_rank, promotion_piece, 0);
      //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
      //printf("%s\n", "");
      p -> num_moves++;
      if (is_black(p -> board [working_rank][working_file])) break;
  }

  working_rank = rank;

  // LEFT

  while (working_file > 0) {
    working_file--;

    if (is_white(p -> board [working_rank][working_file])) break;

      p -> moves[p -> num_moves] = move_init(file, rank, working_file, working_rank, promotion_piece, 0);
      //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
      //printf("%s\n", "");
      p -> num_moves++;
      if (is_black(p -> board [working_rank][working_file])) break;
  }

  working_file = file;

  // RIGHT

  while (working_file + 1< WID) {
    working_file++;

    if (is_white(p -> board [working_rank][working_file])) break;

      p -> moves[p -> num_moves] = move_init(file, rank, working_file, working_rank, promotion_piece, 0);
      //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
      //printf("%s\n", "");
      p -> num_moves++;
      if (is_black(p -> board [working_rank][working_file])) break;
  }
}

void black_rook_moves (struct position * p, char rank, char file) {
  int working_rank = rank;
  int working_file = file;

  char promotion_piece = p -> board [working_rank][working_file];

    // UP

  while (working_rank > 0) {
    working_rank--;

    if (is_black(p -> board [working_rank][working_file])) break;

      p -> moves[p -> num_moves] = move_init(file, rank, working_file, working_rank, promotion_piece, 0);
      //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
      //printf("%s\n", "");
      p -> num_moves++;
      if (is_white(p -> board [working_rank][working_file])) break;
  }

  working_rank = rank;

  // DOWN

  while (working_rank + 1 < LEN) {
    working_rank++;

    if (is_black(p -> board [working_rank][working_file])) break;

      p -> moves[p -> num_moves] = move_init(file, rank, working_file, working_rank, promotion_piece, 0);
      //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
      //printf("%s\n", "");
      p -> num_moves++;
      if (is_white(p -> board [working_rank][working_file])) break;
  }

  working_rank = rank;

  // LEFT

  while (working_file > 0) {
    working_file--;

    if (is_black(p -> board [working_rank][working_file])) break;

      p -> moves[p -> num_moves] = move_init(file, rank, working_file, working_rank, promotion_piece, 0);
      //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
      //printf("%s\n", "");
      p -> num_moves++;
      if (is_white(p -> board [working_rank][working_file])) break;
  }

  working_file = file;

  // RIGHT

  while (working_file + 1< WID) {
    working_file++;

    if (is_black(p -> board [working_rank][working_file])) break;

      p -> moves[p -> num_moves] = move_init(file, rank, working_file, working_rank, promotion_piece, 0);
      //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
      //printf("%s\n", "");
      p -> num_moves++;
      if (is_white(p -> board [working_rank][working_file])) break;
  }
}

// BISHOP MOVES

void white_bishop_moves (struct position * p, char rank, char file) {
  int working_rank = rank;
  int working_file = file;

  char promotion_piece = p -> board [working_rank][working_file];

    // UP Left

  while (working_rank > 0 && working_file > 0) {
    working_rank--;
    working_file--;

    if (is_white(p -> board [working_rank][working_file])) break;

      p -> moves[p -> num_moves] = move_init(file, rank, working_file, working_rank, promotion_piece, 0);
      //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
      //printf("%s\n", "");
      p -> num_moves++;
      if (is_black(p -> board [working_rank][working_file])) break;
  }

  working_rank = rank;
  working_file = file;

  // UP Right

while (working_rank > 0 && working_file + 1 < WID) {
  working_rank--;
  working_file++;

  if (is_white(p -> board [working_rank][working_file])) break;

    p -> moves[p -> num_moves] = move_init(file, rank, working_file, working_rank, promotion_piece, 0);
    //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
    //printf("%s\n", "");
    p -> num_moves++;
    if (is_black(p -> board [working_rank][working_file])) break;
}

working_rank = rank;
working_file = file;

// Down Right

while (working_rank + 1 < LEN && working_file + 1 < WID) {
working_rank++;
working_file++;

if (is_white(p -> board [working_rank][working_file])) break;

  p -> moves[p -> num_moves] = move_init(file, rank, working_file, working_rank, promotion_piece, 0);
  //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
  //printf("%s\n", "");
  p -> num_moves++;
  if (is_black(p -> board [working_rank][working_file])) break;
}

  working_rank = rank;
  working_file = file;

  // Down Left

  while (working_rank + 1 < LEN && working_file > 0) {
  working_rank++;
  working_file--;

  if (is_white(p -> board [working_rank][working_file])) break;

    p -> moves[p -> num_moves] = move_init(file, rank, working_file, working_rank, promotion_piece, 0);
    //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
    //printf("%s\n", "");
    p -> num_moves++;
    if (is_black(p -> board [working_rank][working_file])) break;
  }
}

void black_bishop_moves (struct position * p, char rank, char file) {
  int working_rank = rank;
  int working_file = file;

  char promotion_piece = p -> board [working_rank][working_file];

    // UP Left

  while (working_rank > 0 && working_file > 0) {
    working_rank--;
    working_file--;

    if (is_black(p -> board [working_rank][working_file])) break;

      p -> moves[p -> num_moves] = move_init(file, rank, working_file, working_rank, promotion_piece, 0);
      //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
      //printf("%s\n", "");
      p -> num_moves++;
      if (is_white(p -> board [working_rank][working_file])) break;
  }

  working_rank = rank;
  working_file = file;

  // UP Right

while (working_rank > 0 && working_file + 1 < WID) {
  working_rank--;
  working_file++;

  if (is_black(p -> board [working_rank][working_file])) break;

    p -> moves[p -> num_moves] = move_init(file, rank, working_file, working_rank, promotion_piece, 0);
    //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
    //printf("%s\n", "");
    p -> num_moves++;
    if (is_white(p -> board [working_rank][working_file])) break;
}

working_rank = rank;
working_file = file;

// Down Right

while (working_rank + 1 < LEN && working_file + 1 < WID) {
working_rank++;
working_file++;

if (is_black(p -> board [working_rank][working_file])) break;

  p -> moves[p -> num_moves] = move_init(file, rank, working_file, working_rank, promotion_piece, 0);
  //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
  //printf("%s\n", "");
  p -> num_moves++;
  if (is_white(p -> board [working_rank][working_file])) break;
}

  working_rank = rank;
  working_file = file;

  // Down Left

  while (working_rank + 1 < LEN && working_file > 0) {
  working_rank++;
  working_file--;

  if (is_black(p -> board [working_rank][working_file])) break;

    p -> moves[p -> num_moves] = move_init(file, rank, working_file, working_rank, promotion_piece, 0);
    //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
    //printf("%s\n", "");
    p -> num_moves++;
    if (is_white(p -> board [working_rank][working_file])) break;
  }
}


void white_knight_moves (struct position * p, char rank, char file) {

  char promotion_piece = 'N';

  // UP MOVES
  if (rank - 2 > -1) {
    // LEFT
    if (file - 1 > - 1 && !(is_white(p -> board [rank - 2][file - 1]))) {
      p -> moves[p -> num_moves] = move_init(file, rank, file - 1, rank - 2, promotion_piece, 0);
      p -> num_moves++;
    }

    // RIGHT
    if (file + 1 < WID && !(is_white(p -> board [rank - 2][file + 1]))) {
      p -> moves[p -> num_moves] = move_init(file, rank, file + 1, rank - 2, promotion_piece, 0);
      p -> num_moves++;
    }
  }

  // DOWN MOVES
  if (rank + 2 < LEN) {
    // LEFT
    if (file - 1 > - 1 && !(is_white(p -> board [rank + 2][file - 1]))) {
      p -> moves[p -> num_moves] = move_init(file, rank, file - 1, rank + 2, promotion_piece, 0);
      p -> num_moves++;
    }

    // RIGHT
    if (file + 1 < WID && !(is_white(p -> board [rank + 2][file + 1]))) {
      p -> moves[p -> num_moves] = move_init(file, rank, file + 1, rank + 2, promotion_piece, 0);
      p -> num_moves++;
    }
  }

  // LEFT MOVES
  if (file - 2 > -1) {
    // UP
    if (rank - 1 > - 1 && !(is_white(p -> board [rank - 1][file - 2]))) {
      p -> moves[p -> num_moves] = move_init(file, rank, file - 2, rank - 1, promotion_piece, 0);
      p -> num_moves++;
    }

    // DOWN
    if (rank + 1 < LEN && !(is_white(p -> board [rank + 1][file - 2]))) {
      p -> moves[p -> num_moves] = move_init(file, rank, file - 2, rank + 1, promotion_piece, 0);
      p -> num_moves++;
    }
  }

  // RIGHT MOVES
  if (file + 2 < WID) {
    // UP
    if (rank - 1 > - 1 && !(is_white(p -> board [rank - 1][file + 2]))) {
      p -> moves[p -> num_moves] = move_init(file, rank, file + 2, rank - 1, promotion_piece, 0);
      p -> num_moves++;
    }

    // DOWN
    if (rank + 1 < LEN && !(is_white(p -> board [rank + 1][file + 2]))) {
      p -> moves[p -> num_moves] = move_init(file, rank, file + 2, rank + 1, promotion_piece, 0);
      p -> num_moves++;
    }
  }
}

void black_knight_moves (struct position * p, char rank, char file) {

  char promotion_piece = 'n';

  // UP MOVES
  if (rank - 2 > -1) {
    // LEFT
    if (file - 1 > - 1 && !(is_black(p -> board [rank - 2][file - 1]))) {
      p -> moves[p -> num_moves] = move_init(file, rank, file - 1, rank - 2, promotion_piece, 0);
      p -> num_moves++;
    }

    // RIGHT
    if (file + 1 < WID && !(is_black(p -> board [rank - 2][file + 1]))) {
      p -> moves[p -> num_moves] = move_init(file, rank, file + 1, rank - 2, promotion_piece, 0);
      p -> num_moves++;
    }
  }

  // DOWN MOVES
  if (rank + 2 < LEN) {
    // LEFT
    if (file - 1 > - 1 && !(is_black(p -> board [rank + 2][file - 1]))) {
      p -> moves[p -> num_moves] = move_init(file, rank, file - 1, rank + 2, promotion_piece, 0);
      p -> num_moves++;
    }

    // RIGHT
    if (file + 1 < WID && !(is_black(p -> board [rank + 2][file + 1]))) {
      p -> moves[p -> num_moves] = move_init(file, rank, file + 1, rank + 2, promotion_piece, 0);
      p -> num_moves++;
    }
  }

  // LEFT MOVES
  if (file - 2 > -1) {
    // UP
    if (rank - 1 > - 1 && !(is_black(p -> board [rank - 1][file - 2]))) {
      p -> moves[p -> num_moves] = move_init(file, rank, file - 2, rank - 1, promotion_piece, 0);
      p -> num_moves++;
    }

    // DOWN
    if (rank + 1 < LEN && !(is_black(p -> board [rank + 1][file - 2]))) {
      p -> moves[p -> num_moves] = move_init(file, rank, file - 2, rank + 1, promotion_piece, 0);
      p -> num_moves++;
    }
  }

  // RIGHT MOVES
  if (file + 2 < WID) {
    // UP
    if (rank - 1 > - 1 && !(is_black(p -> board [rank - 1][file + 2]))) {
      p -> moves[p -> num_moves] = move_init(file, rank, file + 2, rank - 1, promotion_piece, 0);
      p -> num_moves++;
    }

    // DOWN
    if (rank + 1 < LEN && !(is_black(p -> board [rank + 1][file + 2]))) {
      p -> moves[p -> num_moves] = move_init(file, rank, file + 2, rank + 1, promotion_piece, 0);
      p -> num_moves++;
    }
  }
}

void white_pawn_moves (struct position * p, char rank, char file) {
  // One move forward
  if (p -> board [rank - 1][file + 0] == '_') {
    if (rank - 1 != 0) {
      p -> moves[p -> num_moves] = move_init(file, rank, file , rank - 1, 'P', 0);
      p -> num_moves++;
    } else { // PROMOTION
      p -> moves[p -> num_moves] = move_init(file, rank, file , rank - 1, 'Q', 0);
      p -> num_moves++;
      p -> moves[p -> num_moves] = move_init(file, rank, file , rank - 1, 'B', 0);
      p -> num_moves++;
      p -> moves[p -> num_moves] = move_init(file, rank, file , rank - 1, 'N', 0);
      p -> num_moves++;
      p -> moves[p -> num_moves] = move_init(file, rank, file , rank - 1, 'R', 0);
      p -> num_moves++;
    }

    // Two moves forward
    if (rank == WID - 2) {
      if (p -> board [rank - 2][file + 0] == '_') {
        p -> moves[p -> num_moves] = move_init(file, rank, file , rank - 2, 'P', 0);
        p -> num_moves++;
      }
    }
  }

    // Capture LEFT
    if (file > 0) {
      if (is_black(p -> board [rank - 1][file - 1])) {
        if (rank - 1 != 0) {
          p -> moves[p -> num_moves] = move_init(file, rank, file - 1, rank - 1, 'P', 0);
          p -> num_moves++;
        } else {
          p -> moves[p -> num_moves] = move_init(file, rank, file - 1, rank - 1, 'Q', 0);
          p -> num_moves++;
          p -> moves[p -> num_moves] = move_init(file, rank, file - 1, rank - 1, 'B', 0);
          p -> num_moves++;
          p -> moves[p -> num_moves] = move_init(file, rank, file - 1, rank - 1, 'N', 0);
          p -> num_moves++;
          p -> moves[p -> num_moves] = move_init(file, rank, file - 1, rank - 1, 'R', 0);
          p -> num_moves++;
        }
      }
    }

    // Capture RIGHT
    if (file < WID - 1) {
      if (is_black(p -> board [rank - 1][file + 1])) {
        if (rank - 1 != 0) {
          p -> moves[p -> num_moves] = move_init(file, rank, file + 1, rank - 1, 'P', 0);
          p -> num_moves++;
        } else {
          p -> moves[p -> num_moves] = move_init(file, rank, file + 1, rank - 1, 'Q', 0);
          p -> num_moves++;
          p -> moves[p -> num_moves] = move_init(file, rank, file + 1, rank - 1, 'B', 0);
          p -> num_moves++;
          p -> moves[p -> num_moves] = move_init(file, rank, file + 1, rank - 1, 'N', 0);
          p -> num_moves++;
          p -> moves[p -> num_moves] = move_init(file, rank, file + 1, rank - 1, 'R', 0);
          p -> num_moves++;
        }
      }
    }

    //EN PASSANT left
    if (rank - 1 == p -> en_passant_rank && file - 1 == p -> en_passant_file) {
      p -> moves[p -> num_moves] = move_init(file, rank, file - 1, rank - 1, 'P', 1);
      p -> num_moves++;
    }

    //EN PASSANT right
    if (rank - 1 == p -> en_passant_rank && file + 1 == p -> en_passant_file) {
      p -> moves[p -> num_moves] = move_init(file, rank, file + 1, rank - 1, 'P', 1);
      p -> num_moves++;
    }
  }

void black_pawn_moves (struct position * p, char rank, char file) {

  // One move forward
  if (p -> board [rank + 1][file + 0] == '_') {
    if (rank < LEN - 2) {
      p -> moves[p -> num_moves] = move_init(file, rank, file , rank + 1, 'p', 0);
      p -> num_moves++;
    } else { // PROMOTION
      p -> moves[p -> num_moves] = move_init(file, rank, file , rank + 1, 'q', 0);
      p -> num_moves++;
      p -> moves[p -> num_moves] = move_init(file, rank, file , rank + 1, 'b', 0);
      p -> num_moves++;
      p -> moves[p -> num_moves] = move_init(file, rank, file , rank + 1, 'n', 0);
      p -> num_moves++;
      p -> moves[p -> num_moves] = move_init(file, rank, file , rank + 1, 'r', 0);
      p -> num_moves++;
    }

    // Two moves forward
    if (rank == 1) {
      if (p -> board [rank + 2][file + 0] == '_') {
        p -> moves[p -> num_moves] = move_init(file, rank, file , rank + 2, 'p', 0);
        p -> num_moves++;
      }
    }
  }
    // Capture LEFT
    if (file > 0) {
      if (is_white(p -> board [rank + 1][file - 1])) {
        if (rank + 2 != WID) {
          p -> moves[p -> num_moves] = move_init(file, rank, file - 1, rank + 1, 'p', 0);
          p -> num_moves++;
        } else {
          p -> moves[p -> num_moves] = move_init(file, rank, file - 1, rank + 1, 'q', 0);
          p -> num_moves++;
          p -> moves[p -> num_moves] = move_init(file, rank, file - 1, rank + 1, 'b', 0);
          p -> num_moves++;
          p -> moves[p -> num_moves] = move_init(file, rank, file - 1, rank + 1, 'n', 0);
          p -> num_moves++;
          p -> moves[p -> num_moves] = move_init(file, rank, file - 1, rank + 1, 'r', 0);
          p -> num_moves++;
        }
      }
    }

    // Capture RIGHT
    if (file < WID - 1) {
      if (is_white(p -> board [rank + 1][file + 1])) {
        if (rank != LEN - 2) {
          p -> moves[p -> num_moves] = move_init(file, rank, file + 1, rank + 1, 'p', 0);
          p -> num_moves++;
        } else {
          p -> moves[p -> num_moves] = move_init(file, rank, file + 1, rank + 1, 'q', 0);
          p -> num_moves++;
          p -> moves[p -> num_moves] = move_init(file, rank, file + 1, rank + 1, 'b', 0);
          p -> num_moves++;
          p -> moves[p -> num_moves] = move_init(file, rank, file + 1, rank + 1, 'n', 0);
          p -> num_moves++;
          p -> moves[p -> num_moves] = move_init(file, rank, file + 1, rank + 1, 'r', 0);
          p -> num_moves++;
        }
      }
    }

    //EN PASSANT left
    if (rank + 1 == p -> en_passant_rank && file - 1 == p -> en_passant_file) {
      p -> moves[p -> num_moves] = move_init(file, rank, file - 1, rank + 1, 'P', 1);
      p -> num_moves++;
    }

    //EN PASSANT right
    if (rank + 1 == p -> en_passant_rank && file + 1 == p -> en_passant_file) {
      p -> moves[p -> num_moves] = move_init(file, rank, file + 1, rank + 1, 'P', 1);
      p -> num_moves++;
    }
  }


void white_king_moves (struct position * p, char rank, char file) {

  // UP
  if (rank > 0) {
    //Straight
    if (!is_white(p -> board [rank - 1][file + 0])) {
      p -> moves[p -> num_moves] = move_init(file, rank, file, rank - 1, 'K', 0);
      //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
      //printf("%s\n", "");
      p -> num_moves++;
    }

    //Left
    if (file > 0) {
      if (!is_white(p -> board [rank - 1][file - 1])) {
        p -> moves[p -> num_moves] = move_init(file, rank, file - 1, rank - 1, 'K', 0);
        //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
        //printf("%s\n", "");
        p -> num_moves++;
      }
    }

    if (file + 1 < WID) {
      if (!is_white(p -> board [rank - 1][file + 1])) {
        p -> moves[p -> num_moves] = move_init(file, rank, file + 1, rank - 1, 'K', 0);
        //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
        //printf("%s\n", "");
        p -> num_moves++;
      }
    }
  }

  // DOWN
  if (rank < WID - 1) {
    //Straight
    if (!is_white(p -> board [rank + 1][file + 0])) {
      p -> moves[p -> num_moves] = move_init(file, rank, file, rank + 1, 'K', 0);
      //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
      //printf("%s\n", "");
      p -> num_moves++;
    }

    //Left
    if (file > 0) {
      if (!is_white(p -> board [rank + 1][file - 1])) {
        p -> moves[p -> num_moves] = move_init(file, rank, file - 1, rank + 1, 'K', 0);
        //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
        //printf("%s\n", "");
        p -> num_moves++;
      }
    }

    if (file + 1 < WID) {
      if (!is_white(p -> board [rank + 1][file + 1])) {
        p -> moves[p -> num_moves] = move_init(file, rank, file + 1, rank + 1, 'K', 0);
        //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
        //printf("%s\n", "");
        p -> num_moves++;
      }
    }
  }

  // LEFT
  if (file > 0) {
    if (!is_white(p -> board [rank + 0][file - 1])) {
      p -> moves[p -> num_moves] = move_init(file, rank, file - 1, rank, 'K', 0);
      //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
      //printf("%s\n", "");
      p -> num_moves++;
    }
  }

  // RIGHT
  if (file < WID - 1) {
    if (!is_white(p -> board [rank + 0][file + 1])) {
      p -> moves[p -> num_moves] = move_init(file, rank, file + 1, rank, 'K', 0);
      //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
      //printf("%s\n", "");
      p -> num_moves++;
    }
  }
}

void black_king_moves (struct position * p, char rank, char file) {

  // UP
  if (rank > 0) {
    //Straight
    if (!is_black(p -> board [rank - 1][file + 0])) {
      p -> moves[p -> num_moves] = move_init(file, rank, file, rank - 1, 'k', 0);
      //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
      //printf("%s\n", "");
      p -> num_moves++;
    }

    //Left
    if (file > 0) {
      if (!is_black(p -> board [rank - 1][file - 1])) {
        p -> moves[p -> num_moves] = move_init(file, rank, file - 1, rank - 1, 'k', 0);
        //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
        //printf("%s\n", "");
        p -> num_moves++;
      }
    }

    if (file + 1 < WID) {
      if (!is_black(p -> board [rank - 1][file + 1])) {
        p -> moves[p -> num_moves] = move_init(file, rank, file + 1, rank - 1, 'k', 0);
        //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
        //printf("%s\n", "");
        p -> num_moves++;
      }
    }
  }

  // DOWN
  if (rank < WID - 1) {
    //Straight
    if (!is_black(p -> board [rank + 1][file + 0])) {
      p -> moves[p -> num_moves] = move_init(file, rank, file, rank + 1, 'k', 0);
      //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
      //printf("%s\n", "");
      p -> num_moves++;
    }

    //Left
    if (file > 0) {
      if (!is_black(p -> board [rank + 1][file - 1])) {
        p -> moves[p -> num_moves] = move_init(file, rank, file - 1, rank + 1, 'k', 0);
        //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
        //printf("%s\n", "");
        p -> num_moves++;
      }
    }

    if (file + 1 < WID) {
      if (!is_black(p -> board [rank + 1][file + 1])) {
        p -> moves[p -> num_moves] = move_init(file, rank, file + 1, rank + 1, 'k', 0);
        //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
        //printf("%s\n", "");
        p -> num_moves++;
      }
    }
  }

  // LEFT
  if (file > 0) {
    if (!is_black(p -> board [rank + 0][file - 1])) {
      p -> moves[p -> num_moves] = move_init(file, rank, file - 1, rank, 'k', 0);
      //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
      //printf("%s\n", "");
      p -> num_moves++;
    }
  }

  // RIGHT
  if (file < WID - 1) {
    if (!is_black(p -> board [rank + 0][file + 1])) {
      p -> moves[p -> num_moves] = move_init(file, rank, file + 1, rank, 'k', 0);
      //print_move(&p -> moves[p -> num_moves] , p); printf("%c", p -> moves[p -> num_moves].promotion_piece);
      //printf("%s\n", "");
      p -> num_moves++;
    }
  }
}

void white_kingside_castle (struct position * p) {
  if (!(p -> white_can_castle_kingside)) return;
  if (!(p -> board[7][5] == '_') || !(p -> board[7][6] == '_')) return;

  if (white_king_checked(p, 7, 4)) return;

  p->board[7][4] = '_';
  p->board[7][5] = 'K';

  if (white_king_checked(p, 7, 5)) {
    p->board[7][4] = 'K';
    p->board[7][5] = '_';
    return;
  }

  p->board[7][4] = 'K';
  p->board[7][5] = '_';

  p -> moves[p -> num_moves] = move_init(4, 7, 6, 7, 'K', FALSE);
  p -> moves[p -> num_moves].is_castling = TRUE;
  p -> num_moves++;
}

void black_kingside_castle (struct position * p) {
  if (!(p -> black_can_castle_kingside)) return;
  if (!(p -> board[0][5] == '_') || !(p -> board[0][6] == '_')) return;

  if (black_king_checked(p, 0, 4)) return;

  p->board[0][4] = '_';
  p->board[0][5] = 'k';

  if (black_king_checked(p, 0, 5)) {
    p->board[0][4] = 'k';
    p->board[0][5] = '_';
    return;
  }

    p->board[0][4] = 'k';
    p->board[0][5] = '_';

  p -> moves[p -> num_moves] = move_init(4, 0, 6, 0, 'k', FALSE);
  p -> moves[p -> num_moves].is_castling = TRUE;
  p -> num_moves++;
}

void white_queenside_castle (struct position * p) {
  if (!(p -> white_can_castle_queenside)) return;
  if (!(p -> board[7][3] == '_') || !(p -> board[7][2] == '_') || !(p -> board[7][1] == '_')) return;

  if (white_king_checked(p, 7, 4)) return;

  p->board[7][4] = '_';
  p->board[7][3] = 'K';

  if (white_king_checked(p, 7, 3)) {
    p->board[7][4] = 'K';
    p->board[7][3] = '_';
    return;
  }

  p->board[7][4] = 'K';
  p->board[7][3] = '_';

  p -> moves[p -> num_moves] = move_init(4, 7, 2, 7, 'K', FALSE);
  p -> moves[p -> num_moves].is_castling = TRUE;
  p -> num_moves++;
}

void black_queenside_castle (struct position * p) {
  if (!(p -> black_can_castle_queenside)) return;
  if (!(p -> board[0][3] == '_') || !(p -> board[0][2] == '_') || !(p -> board[0][1] == '_')) return;

  if (black_king_checked(p, 0, 4)) return;

  p->board[0][4] = '_';
  p->board[0][3] = 'k';

  if (black_king_checked(p, 0, 3)) {
    p->board[0][4] = 'k';
    p->board[0][3] = '_';
    return;
  }

  p->board[0][4] = 'k';
  p->board[0][3] = '_';

  p -> moves[p -> num_moves] = move_init(4, 0, 2, 0, 'k', FALSE);
  p -> moves[p -> num_moves].is_castling = TRUE;
  p -> num_moves++;
}
