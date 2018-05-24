void split (char * array[], char * buf, char * splitter) {
  int i = 0;
  char * p = strtok (buf, splitter);

  while (p != NULL) {
      array[i++] = p;
      p = strtok (NULL, splitter);
  }
}
