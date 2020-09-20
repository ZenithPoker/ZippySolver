#include <stdio.h>
#include <stdlib.h>

#include <memory>

#include "card_abstraction_params.h"
#include "params.h"

using std::unique_ptr;

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
unique_ptr<Params> CreateCardAbstractionParams(void) {
  unique_ptr<Params> params(new Params());
  params->AddParam("CardAbstractionName", P_STRING);
  params->AddParam("Bucketings", P_STRING);
  params->AddParam("BucketThresholds", P_STRING);
  return params;
}
