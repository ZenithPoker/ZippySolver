#ifndef _GAME_PARAMS_H_
#define _GAME_PARAMS_H_

#include <memory>

/*
  Param file definition for a game
  Streets:
    preflop = 0
    flop = 1
    turn = 2
    river = 3
  NumFlopCards on flop, turn and river always a single card
  Game has a betting round on each street up to MaxStreet followed
  by showdown
  FirstToAct specifies the first player to act on each street e.g.
    MaxStreet 3
    FirstToAct 2,0,0,0
    NumPlayers 3
  Implies that preflop button acts first, then sb first on further
  streets.
  Betting definitions are giving by betting_abstraction_params

  All params required other  than NumFlopCards if MaxStreet = 0
  Constraints:
    0 < NumHoleCards <= 2 (currently)
    NumRanks > 0
    NumSuits > 0
    FirstToAct contains MaxStreet+1 values (first to act per street)

  --Jon
*/

class Params;

std::unique_ptr<Params> CreateGameParams(void);

#endif
