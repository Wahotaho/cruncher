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
