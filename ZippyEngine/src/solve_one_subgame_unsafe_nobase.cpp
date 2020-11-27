#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "betting_abstraction.h"
#include "betting_abstraction_params.h"
#include "betting_tree_builder.h"
#include "betting_tree.h"
#include "betting_trees.h"
#include "board_tree.h"
#include "buckets.h"
#include "cards.h"
#include "canonical_cards.h"
#include "card_abstraction.h"
#include "card_abstraction_params.h"
#include "cfr_config.h"
#include "cfr_params.h"
#include "cfr_utils.h"
#include "cfr_values.h"
#include "eg_cfr.h"
#include "files.h"
#include "game.h"
#include "game_params.h"
#include "hand_tree.h"
#include "hand_value_tree.h"
#include "io.h"
#include "params.h"
#include "reach_probs.h"
#include "split.h"
#include "subgame_utils.h" // CreateSubtrees()
#include "unsafe_eg_cfr.h"
#include "vcfr.h"

using std::shared_ptr;
using std::string;
using std::unique_ptr;
using std::unordered_set;
using std::vector;

class SubgameSolver {
public:
  SubgameSolver(const CardAbstraction &card_abstraction,
                const BettingAbstraction &betting_abstraction,
                const CFRConfig &cfr_config, const Buckets &buckets,
                int solve_st, int pot_size,
                int target_bd, bool quantize,
                int num_subgame_its, int num_threads);
  void Solve(const ReachProbs &reach_probs);
private:  
  void Checkpoint(int it, UnsafeEGCFR* eg_cfr, Node* root);
  const CardAbstraction &card_abstraction_;
  const BettingAbstraction &betting_abstraction_;
  const CFRConfig &cfr_config_;
  const Buckets &buckets_;
  unique_ptr<BettingTrees> base_betting_trees_;
  int target_bd_;
  int solve_st_;
  int pot_size_;
  int num_subgame_its_;
  int num_threads_;
};

void Show(Node *node, const string &action_sequence, int target_p, const Buckets &buckets,
   const CFRValues &values, const CanonicalCards *hands) {
  // Initialize to -1 because we want to increment at the top of the loop below.
  int nt = node->NonterminalID();
  int st = node->Street();
  int pa = node->PlayerActing();
  int num_succs = node->NumSuccs();
  int hcp = -1;
  int dsi = node->DefaultSuccIndex();  
  AbstractCFRStreetValues *street_values = values.StreetValues(st);
  CFRStreetValues<double> *d_street_values;
  CFRStreetValues<int> *i_street_values;
  int *i_values = nullptr;
  double *d_values = nullptr;
  if ((d_street_values =
  dynamic_cast<CFRStreetValues<double> *>(street_values))) {
    d_values = d_street_values->AllValues(pa, nt);
  } else {
    i_street_values = dynamic_cast<CFRStreetValues<int> *>(street_values);
    i_values = i_street_values->AllValues(pa, nt);
  }  
  unordered_set<int> seen_buckets;
  int num_hole_card_pairs = Game::NumHoleCardPairs(2);  
  for (int i = 0; i < num_hole_card_pairs; ++i) {
    const Card *cards = hands->Cards(i);
    ++hcp;
    int offset, b = -1;
    if (buckets.None(st)) {
      offset = hcp * num_succs;
    } else {
      b = buckets.Bucket(st, hcp);
      if (seen_buckets.find(b) != seen_buckets.end()) {
        // fprintf(stderr, "hi %i lo %i b %i\n", hi, lo, b);
        continue;
      }
      seen_buckets.insert(b);
      offset = b * num_succs;
    }
    if (action_sequence == "") {
      printf("Root ");
    } else {
      printf("%s ", action_sequence.c_str());
    }
    OutputTwoCards(cards);
    double sum = 0;
    if (i_values) {
      for (int s = 0; s < num_succs; ++s) {
        int iv = i_values[offset + s];
        if (iv > 0) sum += iv;
      }
    } else {
      for (int s = 0; s < num_succs; ++s) {
        double dv = d_values[offset + s];
        if (dv > 0) sum += dv;
      }
    }
    if (sum == 0) {
      for (int s = 0; s < num_succs; ++s) {
        printf(" %f", s == dsi ? 1.0 : 0);
      }
    } else {
      for (int s = 0; s < num_succs; ++s) {
        if (i_values) {
          int iv = i_values[offset + s];
          printf(" %s:%.2f ", node->ActionName(s).c_str(), iv > 0 ? 100 * iv / sum : 0);
        } else {
          double dv = d_values[offset + s];
          printf(" %s:%.2f", node->ActionName(s).c_str(), dv > 0 ? 100 * dv / sum : 0);
        }
      }
    }
    if (! buckets.None(st)) {
      printf(" (b %u)", b);
    }
    printf(" (pa %u nt %u)\n", node->PlayerActing(), node->NonterminalID());
    fflush(stdout);
  }  
}

SubgameSolver::SubgameSolver(const CardAbstraction &card_abstraction,
                             const BettingAbstraction &betting_abstraction,
                             const CFRConfig &cfr_config, const Buckets &buckets,
                             int solve_st, int pot_size,
                             int target_bd, bool quantize,
                             int num_subgame_its, int num_threads) :
  card_abstraction_(card_abstraction),
  betting_abstraction_(betting_abstraction),
  cfr_config_(cfr_config), buckets_(buckets),
  target_bd_(target_bd) {
  num_subgame_its_ = num_subgame_its;
  num_threads_ = num_threads;
  solve_st_ = solve_st;
  pot_size_ = pot_size;
  
  const Card *board = BoardTree::Board(solve_st_, target_bd_);
  //
  fprintf(stderr, "Resolve st %i gbd %i\n", solve_st, target_bd_);  
  OutputNCards(board, solve_st+2);  
}

// Currently assume that this is a street-initial node.
// Might need to do up to four solves.  Imagine we have an asymmetric base
// betting tree, and an asymmetric solving method.
void SubgameSolver::Solve(const ReachProbs &reach_probs) {  
  int st = solve_st_;  
  int num_players = Game::NumPlayers();
  HandTree hand_tree(st, target_bd_, Game::MaxStreet());
  UnsafeEGCFR eg_cfr(card_abstraction_, card_abstraction_,
		     betting_abstraction_, cfr_config_, cfr_config_,
		     buckets_, num_threads_);
  if (st < Game::MaxStreet()) {
    eg_cfr.SetSplitStreet(st + 1);
  }
  
  int num_asym_players = betting_abstraction_.Asymmetric() ? num_players : 1;
  for (int asym_p = 0; asym_p < num_asym_players; ++asym_p) {
    unique_ptr<BettingTrees> subgame_subtrees(CreateSubtrees(st, Game::FirstToAct(st), pot_size_ / 2, asym_p, betting_abstraction_));
    // Print tree::
    //subgame_subtrees.get()->GetBettingTree()->Display();

    double resolving_secs = 0;
    struct timespec start, finish;
    clock_gettime(CLOCK_MONOTONIC, &start);
    // One solve for unsafe endgame solving, no t_vals
    eg_cfr.SolveSubgame(subgame_subtrees.get(), target_bd_, reach_probs, "",
			 &hand_tree, nullptr, -1, true, num_subgame_its_);
    clock_gettime(CLOCK_MONOTONIC, &finish);
    resolving_secs += (finish.tv_sec - start.tv_sec);
    resolving_secs += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Resolving secs: %f\n", resolving_secs);
    fflush(stdout);        
        
    Node* rootNode = subgame_subtrees.get()->Root();    
    // Print out root strategy:
    CFRValues* values = eg_cfr.Sumprobs().get();
    Show(rootNode, "", 0, buckets_, *values, hand_tree.Hands(st, target_bd_));
    // Save strategy
    Checkpoint(0, &eg_cfr, rootNode);
  }
}

void SubgameSolver::Checkpoint(int it, UnsafeEGCFR* eg_cfr, Node* root) {
  char dir[500];
  sprintf(dir, "%s/%s.%u.%s.%u.%u.%u.%s.%s", Files::NewCFRBase(),
	  Game::GameName().c_str(), Game::NumPlayers(),
	  card_abstraction_.CardAbstractionName().c_str(), Game::NumRanks(),
	  Game::NumSuits(), Game::MaxStreet(), betting_abstraction_.BettingAbstractionName().c_str(),
	  cfr_config_.CFRConfigName().c_str());
  // if (betting_abstraction_.Asymmetric()) {
  //   char buf[100];
  //   sprintf(buf, ".p%u", target_p_);
  //   strcat(dir, buf);
  // }
  Mkdir(dir);
  eg_cfr->Regrets().get()->Write(dir, it, root, "x", -1, false);
  eg_cfr->Sumprobs().get()->Write(dir, it, root, "x", -1, true);
}

static void Usage(const char *prog_name) {
  fprintf(stderr, "USAGE: %s <game params> <card params> <betting params> <CFR params> "
                  "<initial street> <pot size> <bd> <its> [quantize|raw] <num threads>\n",
	  prog_name);
  exit(-1);
}

int main(int argc, char *argv[]) {
  if (argc != 11) Usage(argv[0]);
  Files::Init();
  unique_ptr<Params> game_params = CreateGameParams();
  game_params->ReadFromFile(argv[1]);
  Game::Initialize(*game_params);
  unique_ptr<Params> card_params = CreateCardAbstractionParams();
  card_params->ReadFromFile(argv[2]);
  unique_ptr<CardAbstraction>
    card_abstraction(new CardAbstraction(*card_params));
  unique_ptr<Params> betting_params = CreateBettingAbstractionParams();
  betting_params->ReadFromFile(argv[3]);
  unique_ptr<BettingAbstraction>
    betting_abstraction(new BettingAbstraction(*betting_params));
  unique_ptr<Params> cfr_params = CreateCFRParams();
  cfr_params->ReadFromFile(argv[4]);
  unique_ptr<CFRConfig> cfr_config(new CFRConfig(*cfr_params));
  int st, pot_size, bd, its;
  if (sscanf(argv[5], "%i", &st) != 1)        Usage(argv[0]);
  if (sscanf(argv[6], "%i", &pot_size) != 1)  Usage(argv[0]);
  if (sscanf(argv[7], "%i", &bd) != 1)        Usage(argv[0]);
  if (sscanf(argv[8], "%i", &its) != 1)       Usage(argv[0]);
  string q = argv[9];
  bool quantize;
  if (q == "quantize") quantize = true;
  else if (q == "raw") quantize = false;
  else                 Usage(argv[0]);
  int num_threads;
  if (sscanf(argv[10], "%i", &num_threads) != 1) Usage(argv[0]);

  Buckets buckets(*card_abstraction, false);
  BoardTree::Create();
  BoardTree::CreateLookup();
  HandValueTree::Create();

  SubgameSolver solver(*card_abstraction, *betting_abstraction, *cfr_config,
		       buckets, st, pot_size, bd, quantize, its, num_threads);

  unique_ptr<ReachProbs>  reach_probs(ReachProbs::Load());
  solver.Solve(*reach_probs);
}
