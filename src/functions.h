// Function definitions
void load_board ();
void parse_rank (char * rank, char * pieces);
void split (char * array[], char * buf, char * splitter);
void print_position ();
int is_piece (char p);
//

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

void print_position () {
  printf(" ");
  for (int i = 0; i < wid * 2 - 1; i++) printf("_");
  printf("\n");
  for (int i = 0; i < len; i++) {
    printf("%s", "|");
    for (int j = 0; j < wid; j++) {
      printf("%c|", board[i][j]);
    }
    printf("\n");
  }
  printf("%s\n", "");
}

int is_piece (char p) {
  return (p == 'K' || p == 'Q' || p == 'B' || p == 'N' || p == 'R' || p == 'P' || p == 'k' || p == 'q'
  || p == 'b' || p == 'n' || p == 'r' || p == 'p');
}
