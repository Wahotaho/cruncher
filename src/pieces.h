int is_piece (char p) {
  return (p == 'K' || p == 'Q' || p == 'B' || p == 'N' || p == 'R' || p == 'P' || p == 'k' || p == 'q'
  || p == 'b' || p == 'n' || p == 'r' || p == 'p');
}

int is_white (char p) {
  return (p < 91 && p > 64);
}

int is_black (char p) {
  return (p > 96 && p < 123);
}
