void load_board () {
  char * pos_array[len];
  split (pos_array, position, "/");

  for (int i = 0; i < len; i++) {
    parse_rank(board[i], pos_array[i]);
  }

}

void parse_rank (char * rank, char * pieces) {
  int j = 0;
  int i = 0;
  int num;

  while (i < wid) {

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

}

void split (char * array[], char * buf, char * splitter) {
  int i = 0;
  char * p = strtok (buf, splitter);

  while (p != NULL) {
      array[i++] = p;
      p = strtok (NULL, splitter);
  }
}
