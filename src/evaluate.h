#define KING_VAL 100000.0
#define QUEEN_VAL 9.0
#define BISHOP_VAL 3.2
#define KNIGHT_VAL 3.0
#define ROOK_VAL 5.0
#define PAWN_VAL 1.0


void evaluate_position (struct position * p) {

  int i; int j;

  for (i = 0; i < LEN; i++) {
    for (j = 0; j < WID; j++) {
      if (p -> board[i][j] == 'K') {
        p -> position_value += KING_VAL;
      } else if (p -> board[i][j] == 'k') {
        p -> position_value -= KING_VAL;
      } else if (p -> board[i][j] == 'Q') {
        p -> position_value += QUEEN_VAL;
      } else if (p -> board[i][j] == 'q') {
        p -> position_value -= QUEEN_VAL;
      } else if (p -> board[i][j] == 'B') {
        p -> position_value += BISHOP_VAL;
      } else if (p -> board[i][j] == 'b') {
        p -> position_value -= BISHOP_VAL;
      } else if (p -> board[i][j] == 'N') {
        p -> position_value += KNIGHT_VAL;
      } else if (p -> board[i][j] == 'n') {
        p -> position_value -= KNIGHT_VAL;
      } else if (p -> board[i][j] == 'R') {
        p -> position_value += ROOK_VAL;
      } else if (p -> board[i][j] == 'r') {
        p -> position_value -= ROOK_VAL;
      } else if (p -> board[i][j] == 'P') {
        p -> position_value += PAWN_VAL;
      } else if (p -> board[i][j] == 'p') {
        p -> position_value -= PAWN_VAL;
      }
    }
  }
}
