#ifndef _CARD_ABSTRACTION_PARAMS_H_
#define _CARD_ABSTRACTION_PARAMS_H_

#include <memory>

/*
  Param file definition for a card abstraction
    CardAbstractionName
        Required, name for given card abstraction
    Bucketings
        Required, comma seperated bucketing per street. Must contain
        MaxStreets+1 values
    BucketThresholds
        Optional parameter, bucketing only occurs if total bet by player
        is less than BucketThresholds per street. Must contain
        MaxStreets+1 values
  --Jon
*/

class Params;

std::unique_ptr<Params> CreateCardAbstractionParams(void);

#endif
