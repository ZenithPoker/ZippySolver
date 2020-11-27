#ifndef _CARDS_H_
#define _CARDS_H_

#include <string>

// These need to be before the include of game.h
// Type defintions for Card, Rank, Suit
typedef int Card;
Card MakeCard(int rank, int suit);

#include "game.h"

#define Rank(card)           (card / Game::NumSuits())
#define Suit(card)           (card % Game::NumSuits())


/*
	Output methods for logging card to command line
	Takes card object and prints suit and rank to command line
*/
void OutputRank(int rank);
void CardName(Card c, std::string *name);
// overloaded methods for outputing cards, passed as 7 arguments or an array with size n
void OutputCards(Card card);
void OutputCards(Card c1, Card c2);
void OutputCards(Card c1, Card c2, Card c3);
void OutputCards(Card c1, Card c2, Card c3, Card c4);
void OutputCards(Card c1, Card c2, Card c3, Card c4, Card c5);
void OutputCards(Card c1, Card c2, Card c3, Card c4, Card c5, Card c6);
void OutputCards(Card c1, Card c2, Card c3, Card c4, Card c5,
		      	 Card c6, Card c7);
void OutputCards(const Card *cards, int n); 
void OutputCards(const Card *cards, int n, bool newline); 

Card ParseCard(const char *str); // parse from string to card object
void ParseTwoCards(const char *str, bool space_separated, Card *cards);  //parse two cards, specify if seperated by space
void ParseThreeCards(const char *str, bool space_separated, Card *cards);
void ParseFiveCards(const char *str, bool space_separated, Card *cards);
bool InCards(Card c, const Card *cards, int num_cards); //set IN operator
int MaxSuit(Card *board, int num_board); // return maximum suit

#endif
