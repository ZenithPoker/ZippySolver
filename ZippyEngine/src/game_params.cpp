#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <memory>

#include "game_params.h"
#include "params.h"

using std::unique_ptr;

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
unique_ptr<Params> CreateGameParams(void) {
  unique_ptr<Params> params(new Params());
  params->AddParam("GameName", P_STRING);
  params->AddParam("NumRanks", P_INT);
  params->AddParam("NumSuits", P_INT);
  params->AddParam("StackSize", P_INT);
  params->AddParam("MaxStreet", P_INT);
  params->AddParam("NumHoleCards", P_INT);
  params->AddParam("NumFlopCards", P_INT);
  params->AddParam("Ante", P_INT);
  params->AddParam("SmallBlind", P_INT);
  params->AddParam("FirstToAct", P_STRING);
  params->AddParam("BigBlind", P_INT);
  params->AddParam("NumPlayers", P_INT);
  return params;
}
