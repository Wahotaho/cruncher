#define KING_VAL 100000.0
#define QUEEN_VAL 9.0
#define BISHOP_VAL 3.25 //3.25
#define KNIGHT_VAL 3.0
#define ROOK_VAL 5.0
#define PAWN_VAL 1.0

double move_weight = .01;

void evaluate_position (struct position * p) {

  int i; int j;

  for (i = 0; i < LEN; i++) {
    for (j = 0; j < WID; j++) {
      if (p -> board[i][j] == 'K') {
        p -> material_value += KING_VAL;
      } else if (p -> board[i][j] == 'k') {
        p -> material_value -= KING_VAL;
      } else if (p -> board[i][j] == 'Q') {
        p -> material_value += QUEEN_VAL;
      } else if (p -> board[i][j] == 'q') {
        p -> material_value -= QUEEN_VAL;
      } else if (p -> board[i][j] == 'B') {
        p -> material_value += BISHOP_VAL;
      } else if (p -> board[i][j] == 'b') {
        p -> material_value -= BISHOP_VAL;
      } else if (p -> board[i][j] == 'N') {
        p -> material_value += KNIGHT_VAL;
      } else if (p -> board[i][j] == 'n') {
        p -> material_value -= KNIGHT_VAL;
      } else if (p -> board[i][j] == 'R') {
        p -> material_value += ROOK_VAL;
      } else if (p -> board[i][j] == 'r') {
        p -> material_value -= ROOK_VAL;
      } else if (p -> board[i][j] == 'P') {
        p -> material_value += PAWN_VAL;
      } else if (p -> board[i][j] == 'p') {
        p -> material_value -= PAWN_VAL;
      }
    }
  }

  if (p -> side_move == 'w') {
    p -> mobility_value = move_weight*(p ->num_moves - p ->previous_num_moves);
    //printf("%f\n", p -> mobility_value);
    //printf("%s\n", "");
  } else {
    p -> mobility_value = move_weight*(p ->previous_num_moves - p ->num_moves);
    //printf("%f\n", p -> mobility_value);
    //printf("%s\n", "");
  }

  p-> position_value = p -> material_value + p -> mobility_value;
}
