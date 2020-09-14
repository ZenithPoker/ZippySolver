#ifndef _CFR_UTILS_H_
#define _CFR_UTILS_H_

#include <memory>
#include <string>

template<typename T> class CFRStreetValues;
class CanonicalCards;
class CardAbstraction;
class CFRConfig;
class Node;

/*Calculates playing strategy as variable probs by using regret matching -Brian*/
template <typename T> void RMProbs(const T *vals, int num_succs, int dsi, double *probs);
template <typename T> void ComputeOurValsBucketed(const T *all_cs_vals, int num_hole_card_pairs,
						  int num_succs, int dsi,
						  std::shared_ptr<double []> *succ_vals,
						  int *street_buckets,
						  std::shared_ptr<double []> vals);
/*Calculates ev for each hand
  ev = summation of (ev of taking an action * how often we take that action) for all actions -Brian*/
template <typename T> void ComputeOurVals(const T *all_cs_vals, int num_hole_card_pairs,
					  int num_succs, int dsi,
					  std::shared_ptr<double []> *succ_vals, int lbd,
					  std::shared_ptr<double []> vals);
template <typename T> void SetCurrentAbstractedStrategy(const T *all_regrets, int num_buckets,
							int num_succs, int dsi,
							double *all_cs_probs);
/*Calculates ev of each hand at showdown -Brian*/
std::shared_ptr<double []> Showdown(Node *node, const CanonicalCards *hands, double *opp_probs,
				    double sum_opp_probs, double *total_card_probs);
/* Calculates ev of each hand when someone folds -Brian*/
std::shared_ptr<double []> Fold(Node *node, int p, const CanonicalCards *hands, double *opp_probs,
				double sum_opp_probs, double *total_card_probs);
/* updates opp reach sum and card reach sum (see Fold function for information on card reach) after they take an action -Brian*/
void CommonBetResponseCalcs(int st, const CanonicalCards *hands, double *opp_probs,
			    double *sum_opp_probs, double *total_card_probs);
template <typename T>
/*For all possible hands-
  Calculates opponents strategy with reget matching
  Calls function UpdateSumprobsAndSuccOppProbs(in cfr_utils.cpp, the function is not declared here)
   to update villains strategysum at this node and update 
  their reach probability for each successor node based on their current strategy for this node -Brian
*/
void ProcessOppProbs(Node *node, const CanonicalCards *hands, int *street_buckets,
		     double *opp_probs, std::shared_ptr<double []> *succ_opp_probs,
		     double *current_probs, int it, int soft_warmup, int hard_warmup,
		     double sumprob_scaling, CFRStreetValues<T> *sumprobs);
template <typename T1, typename T2>
void ProcessOppProbs(Node *node, int lbd, const CanonicalCards *hands, bool bucketed,
		     int *street_buckets, double *opp_probs,
		     std::shared_ptr<double []> *succ_opp_probs,
		     const CFRStreetValues<T1> &cs_vals, int dsi, int it, int soft_warmup,
		     int hard_warmup, double sumprob_scaling,
		     CFRStreetValues<T2> *sumprobs);
/*
    I think every iteration is saved in the cfr folder. This function is for deleting the old files,
     as we're just interested in the last iteration. -Brian
 */
void DeleteOldFiles(const CardAbstraction &ca, const std::string &betting_abstraction_name,
		    const CFRConfig &cc, int it);

#endif
