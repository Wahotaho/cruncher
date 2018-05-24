struct position position_init (char * str, char * side, char * castling_options, char * en_passant, double half_moves, int full_moves) {
  struct position pos;

  strcpy (pos.position_string, str);
  strcpy (pos.side_move, side);

  pos.white_can_castle_kingside = string_contains (castling_options, 'K');
  pos.white_can_castle_queenside = string_contains (castling_options, 'Q');
  pos.black_can_castle_kingside = string_contains (castling_options, 'k');
  pos.black_can_castle_queenside = string_contains (castling_options, 'q');

  strcpy (pos.en_passant_square, en_passant);

  pos.half_move_clock = half_moves;
  pos.full_move_number = full_moves;

  load_board(&pos);
  return pos;
}

void print_position (struct position pos) {
  printf(" ");
  for (int i = 0; i < WID * 2 - 1; i++) printf("_");
  printf("\n");
  for (int i = 0; i < LEN; i++) {
    printf("%s", "|");
    for (int j = 0; j < WID; j++) {
      printf("%c|", pos.board[i][j]);
    }
    printf("\n");
  }
  printf("%s\n", "");
}
