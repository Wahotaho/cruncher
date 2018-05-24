void split (char * array[], char * buf, char * splitter) {
  int i = 0;
  char * p = strtok (buf, splitter);

  while (p != NULL) {
      array[i++] = p;
      p = strtok (NULL, splitter);
  }
}

_Bool string_contains (char * str, char s) {

  int len = strlen(str);

  for (int i = 0; i < len; i++) {
      if (str[i] == s) return 1;
  }

    return 0;

}
