#include <stdio.h>
#include <stdlib.h>

#include <string>

#include "cards.h"

using std::string;

void OutputRank(int rank) {
  if (rank < 8) {
    printf("%i", (int)rank + 2);
  } else if (rank == 8) {
    printf("T");
  } else if (rank == 9) {
    printf("J");
  } else if (rank == 10) {
    printf("Q");
  } else if (rank == 11) {
    printf("K");
  } else if (rank == 12) {
    printf("A");
  } else {
    fprintf(stderr, "Illegal rank %i\n", rank);
    exit(-1);
  }
}


void CardName(Card c, string *name) {
  *name = "";
  int rank = Rank(c);
  int suit = Suit(c);

  if (rank < 8) {
    *name += rank + 50;
  } else if (rank == 8) {
    *name += "T";
  } else if (rank == 9) {
    *name += "J";
  } else if (rank == 10) {
    *name += "Q";
  } else if (rank == 11) {
    *name += "K";
  } else if (rank == 12) {
    *name += "A";
  }
  switch (suit) {
  case 0:
    *name += "c"; break;
  case 1:
    *name += "d"; break;
  case 2:
    *name += "h"; break;
  case 3:
    *name += "s"; break;
  default:
    fprintf(stderr, "Illegal suit\n");
    exit(-1);
  }
}

void OutputCards(Card card) {
  int rank = Rank(card);
  int suit = Suit(card);

  OutputRank(rank);
  switch (suit) {
  case 0:
    printf("c"); break;
  case 1:
    printf("d"); break;
  case 2:
    printf("h"); break;
  case 3:
    printf("s"); break;
  default:
    fprintf(stderr, "Illegal suit\n");
    exit(-1);
  }
}

void OutputCards(Card c1, Card c2) {
  OutputCards(c1);
  printf(" ");
  OutputCards(c2);
}

void OutputCards(Card c1, Card c2, Card c3) {
  OutputCards(c1);
  printf(" ");
  OutputCards(c2);
  printf(" ");
  OutputCards(c3);
}


void OutputCards(Card c1, Card c2, Card c3, Card c4) {
  OutputCards(c1);
  printf(" ");
  OutputCards(c2);
  printf(" ");
  OutputCards(c3);
  printf(" ");
  OutputCards(c4);
}


void OutputCards(Card c1, Card c2, Card c3, Card c4, Card c5) {
  OutputCards(c1);
  printf(" ");
  OutputCards(c2);
  printf(" ");
  OutputCards(c3);
  printf(" ");
  OutputCards(c4);
  printf(" ");
  OutputCards(c5);
}

void OutputCards(Card c1, Card c2, Card c3, Card c4, Card c5, Card c6) {
  OutputCards(c1);
  printf(" ");
  OutputCards(c2);
  printf(" ");
  OutputCards(c3);
  printf(" ");
  OutputCards(c4);
  printf(" ");
  OutputCards(c5);
  printf(" ");
  OutputCards(c6);
}

void OutputCards(Card c1, Card c2, Card c3, Card c4, Card c5,
		      Card c6, Card c7) {
  OutputCards(c1);
  printf(" ");
  OutputCards(c2);
  printf(" ");
  OutputCards(c3);
  printf(" ");
  OutputCards(c4);
  printf(" ");
  OutputCards(c5);
  printf(" ");
  OutputCards(c6);
  printf(" ");
  OutputCards(c7);
}

void OutputCards(const Card *cards, int n, bool newline) {
  for (int i = 0; i < n; ++i) {
    if (i > 0) printf(" ");
    OutputCards(cards[i]);
  }
  if (newline) printf("\n");
}

void OutputCards(const Card *cards, int n) {
  OutputCards(cards, n, false);
}

Card MakeCard(int rank, int suit) {
  return rank * Game::NumSuits() + suit;
}

// return card object from valid string. e.g. 'Ad' -> (12, 1) Card
Card ParseCard(const char *str) {
  char c = str[0];
  int rank;
  if (c >= '0' && c <= '9') {
    rank = (c - '0') - 2;
  } else if (c == 'A') {
    rank = 12;
  } else if (c == 'K') {
    rank = 11;
  } else if (c == 'Q') {
    rank = 10;
  } else if (c == 'J') {
    rank = 9;
  } else if (c == 'T') {
    rank = 8;
  } else {
    fprintf(stderr, "Couldn't parse card rank\n");
    fprintf(stderr, "Str %s\n", str);
    exit(-1);
  }
  c = str[1];
  if (c == 'c') {
    return MakeCard(rank, 0);
  } else if (c == 'd') {
    return MakeCard(rank, 1);
  } else if (c == 'h') {
    return MakeCard(rank, 2);
  } else if (c == 's') {
    return MakeCard(rank, 3);
  } else {
    fprintf(stderr, "Couldn't parse card suit\n");
    fprintf(stderr, "Str %s\n", str);
    exit(-1);
  }
}

// parse two cards, specifying if seperated by space. if space_sep=True, Ad Kh -> (12, 1) and (11,2)
// requires a Card array to be passed, must ensure Card array is memory safe.
void ParseTwoCards(const char *str, bool space_separated, Card *cards) {
  cards[0] = ParseCard(str);
  if (space_separated) {
    cards[1] = ParseCard(str + 3);
  } else {
    cards[1] = ParseCard(str + 2);
  }
}

// parse three cards, analogous to two case.
void ParseThreeCards(const char *str, bool space_separated, Card *cards) {
  cards[0] = ParseCard(str);
  if (space_separated) {
    cards[1] = ParseCard(str + 3);
    cards[2] = ParseCard(str + 6);
  } else {
    cards[1] = ParseCard(str + 2);
    cards[2] = ParseCard(str + 4);
  }
}

void ParseFiveCards(const char *str, bool space_separated, Card *cards) {
  cards[0] = ParseCard(str);
  if (space_separated) {
    cards[1] = ParseCard(str + 3);
    cards[2] = ParseCard(str + 6);
    cards[3] = ParseCard(str + 8);
    cards[4] = ParseCard(str + 12);
  } else {
    cards[1] = ParseCard(str + 2);
    cards[2] = ParseCard(str + 4);
    cards[3] = ParseCard(str + 6);
    cards[4] = ParseCard(str + 8);
  }
}

// Set IN operator.
// Returns true if c is in the set of cards
bool InCards(Card c, const Card *cards, int num_cards) {
  for (int i = 0; i < num_cards; ++i) if (c == cards[i]) return true;
  return false;
}

// returns maximum suit given a board (card array)
// suit order is defined in game.cpp
int MaxSuit(Card *board, int num_board) {
  int max_suit = Suit(board[0]);
  for (int i = 1; i < num_board; ++i) {
    int s = Suit(board[i]);
    if (s > max_suit) max_suit = s;
  }
  return max_suit;
}

