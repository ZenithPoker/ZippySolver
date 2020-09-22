#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <string>

#include "files.h"

using std::string;

/*
  Directories of static files and CFR base for storing
  buckets and CFR checkpoints (regrets & sumprobs) respectively
  --Jon
*/

string Files::old_cfr_base_ = "";
string Files::new_cfr_base_ = "";
string Files::static_base_ = "";

void Files::Init(void) {
  old_cfr_base_ = "../cfr";
  new_cfr_base_ = "../cfr";
  static_base_ = "../static";
}

const char *Files::OldCFRBase(void) {
  if (old_cfr_base_ == "") {
    fprintf(stderr, "You forgot to call Files::Init()\n");
    exit(-1);
  }
  return old_cfr_base_.c_str();
}

const char *Files::NewCFRBase(void) {
  if (new_cfr_base_ == "") {
    fprintf(stderr, "You forgot to call Files::Init()\n");
    exit(-1);
  }
  return new_cfr_base_.c_str();
}

const char *Files::StaticBase(void) {
  if (static_base_ == "") {
    fprintf(stderr, "You forgot to call Files::Init()\n");
    exit(-1);
  }
  return static_base_.c_str();
}
