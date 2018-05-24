void load_board () {
  char * pos_array[LEN];

  char temp[FEN_STRING_LEN];
  strcpy(temp, current_position_string);

  split (pos_array, current_position_string, "/");

  for (int i = 0; i < LEN; i++) {
    parse_rank(current_board[i], pos_array[i]);
  }

  strcpy(current_position_string, temp);
}

void load_board_from_struct (struct position * pos) {
  char * pos_array[LEN];

  char temp[FEN_STRING_LEN];
  strcpy(temp, pos->position_string);

  split (pos_array, pos->position_string, "/");

  for (int i = 0; i < LEN; i++) {
    parse_rank(pos -> board[i], pos_array[i]);
  }

  strcpy(pos->position_string, temp);
}

void parse_rank (char * rank, char * pieces) {
  int j = 0;
  int i = 0;
  int num;

  while (i < WID) {

      if (pieces[j] > 47 && pieces[j] < 58) {

        num = pieces[j] - 48;

        while (num > 0) {
          rank[i] = '_';
          num--;
          i++;
        }

      } else if (is_piece(pieces[j])) {

        rank[i] = pieces[j];
        i++;
      } else {
        printf("%s\n", "Invalid position.");
        exit(1);
        }

        j++;
  }

  rank[WID] = 0;

}
