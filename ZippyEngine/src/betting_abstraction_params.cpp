#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <memory>

#include "betting_abstraction_params.h"
#include "params.h"

using std::unique_ptr;
/*
  Param file definition for a betting abstraction
    BettingAbstractionName
        Required, name for given betting abstraction
    Limit
        Required, bool: is limit game?
    StackSize
        Required, stack size in BB
    MinBet
        Required, minimum bet size in bb (applies to all streets)
    InitialStreet
        Optional, starting street (defaults to 0)
    Asymmetric
        Required, bool: is betting the same for both players
        If false, use Our{PARAM} and Opp{PARAM} 
    NoLimitTreeType
        Required, deprecated: used to switch between CreateNoLimitTree1 and CreateNoLimitTree2.
        Now CreateMPTree is always used.
    MaxBets (Asymmetric option)
        Required, max number of bets per player per street
    BetSizes (Asymmetric option)
        Required, bet size options per street given as decimal fraction of pot.
        Street seperated by '|', bet seperated by ';' bet size seperated by ','
        For example: in a two street game with 50% open and 75% and 150% 3bet preflop, and 
        25%, 50%, 75% postflop with 50% 3bet: '0.5;0.75,1.5|0.25,0.5,0.75;0.5'
        WARNING: Bet sizes must be specified for each bet up to MaxBets[street]!    
    AlwaysAllIn (Asymmetric option)
        Required, bool: add all in option at every node of betting tree
    MinBets (Asymmetric option) // perhaps deprecated (i.e. has no effect)
        Optional, bets per street which must be minbets. Streets seperated by ';'
        and bet number seperated by ','. For exmaple on a 2 street game: '0,1;0,2'
        would imply the first two bets on the first street and the first and third
        bet on the second street MUST be minbets. Bet number must be less than MaxBets 
        WARNING: the setting is not used in any of the example runs.
    MinAllInPot // not used
    NoOpenLimp // probably deprecated, not used in any runs
    NoRegularBetThreshold // not used
    OurOnlyPotThreshold // not used in any runs. would specify pot size
                           threshold after which we only use pot size bets
    GeometricType // not used
    CloseToAllInFrac
        Optional, if bet would make pot larger than CloseToAllInFrac * size of all in pot,
        go all in instead. E.g CloseToAllInFrac = 0.5: any bet that would make pot larger
        than our stack size, go all in instead.
    BetSizeMultipliers // not used
    AllowableBetTos // not used in any runs
    LastAggressorKey // not used
    TODO:not sure about reentrant stuff yet.
  --Jon
*/
unique_ptr<Params> CreateBettingAbstractionParams(void) {
  unique_ptr<Params> params(new Params());
  params->AddParam("BettingAbstractionName", P_STRING);
  params->AddParam("Limit", P_BOOLEAN);
  params->AddParam("StackSize", P_INT);
  params->AddParam("MinBet", P_INT);
  params->AddParam("InitialStreet", P_INT);
  params->AddParam("Asymmetric", P_BOOLEAN);
  params->AddParam("NoLimitTreeType", P_INT);

  params->AddParam("MaxBets", P_STRING);
  params->AddParam("OurMaxBets", P_STRING);
  params->AddParam("OppMaxBets", P_STRING);

  params->AddParam("BetSizes", P_STRING);
  params->AddParam("OurBetSizes", P_STRING);
  params->AddParam("OppBetSizes", P_STRING);  

  // Deprecated
  params->AddParam("AllBetSizeStreets", P_STRING);
  params->AddParam("AllEvenBetSizeStreets", P_STRING);

  params->AddParam("AlwaysAllIn", P_BOOLEAN);
  params->AddParam("OurAlwaysAllIn", P_BOOLEAN);
  params->AddParam("OppAlwaysAllIn", P_BOOLEAN);

  params->AddParam("MinBets", P_STRING);
  params->AddParam("OurMinBets", P_STRING);
  params->AddParam("OppMinBets", P_STRING);

  params->AddParam("MinAllInPot", P_INT);
  params->AddParam("NoOpenLimp", P_BOOLEAN);
  params->AddParam("OurNoOpenLimp", P_BOOLEAN);
  params->AddParam("OppNoOpenLimp", P_BOOLEAN);

  params->AddParam("NoRegularBetThreshold", P_INT);
  params->AddParam("OurNoRegularBetThreshold", P_INT);
  params->AddParam("OppNoRegularBetThreshold", P_INT);

  params->AddParam("OnlyPotThreshold", P_INT);
  params->AddParam("OurOnlyPotThreshold", P_INT);
  params->AddParam("OppOnlyPotThreshold", P_INT);

  params->AddParam("GeometricType", P_INT);
  params->AddParam("OurGeometricType", P_INT);
  params->AddParam("OppGeometricType", P_INT);

  params->AddParam("CloseToAllInFrac", P_DOUBLE);
  params->AddParam("OurBetSizeMultipliers", P_STRING);
  params->AddParam("OppBetSizeMultipliers", P_STRING);

  // not sure about reentrant stuff yet
  params->AddParam("ReentrantStreets", P_STRING);
  params->AddParam("BettingKeyStreets", P_STRING);
  params->AddParam("MinReentrantPot", P_INT);
  params->AddParam("MergeRules", P_STRING);

  params->AddParam("AllowableBetTos", P_STRING);
  params->AddParam("LastAggressorKey", P_BOOLEAN);

  return params;
}
