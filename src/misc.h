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
      if (str[i] == s) return TRUE;
  }

    return FALSE;

}

int abs (int a) {
  if (a < 0) return -a;
  return a;
}

char toUpper (char c) {
  if (c < 123 && c > 96) return c - 32;
  return c;
}

char toLower (char c) {
  if (c < 91 && c > 64) return c + 32;
  return c;
}
