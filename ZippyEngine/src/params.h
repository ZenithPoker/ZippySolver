#ifndef _PARAMS_
#define _PARAMS_

#include <string>
#include <vector>

/*
  Param file type utility types & functions.
  Param files are list of key-value pairs with the following format:
    {key}[whitespace]{value}[newline]
  Comments in param files begin with #
    [#]{comment}

  Params must be added using AddParam to populate the vectors
    param_names_ ({key} type string}
    param_values_ ({value} type ParamValue)
    param_types_ ({value_type} ParamType)

  After param type definitions are given paramaters can be loaded using
    ReadFromFile
  which parses {key}{value} pairs with type given by param_types_ storing
  the result in the corresponding param_values_ struct

  After parsing, values can be retreived using
    GetStringValue
    GetIntValue
    GetDoubleValue
    GetBooleanValue

  If a param in param_names_ was not found during ReadFromFile then
    IsSet
  returns false
  -- Jon
 */

enum ParamType {
  P_STRING,
  P_INT,
  P_DOUBLE,
  P_BOOLEAN
};

struct ParamValue {
  bool set;
  std::string s;
  int i;
  double d;
};

class Params {
public:
  Params(void);
  ~Params(void);
  void AddParam(const std::string &name, ParamType ptype);
  void ReadFromFile(const char *filename);
  bool IsSet(const char *name) const;
  std::string GetStringValue(const char *name) const;
  int GetIntValue(const char *name) const;
  double GetDoubleValue(const char *name) const;
  bool GetBooleanValue(const char *name) const;
private:
  int GetParamIndex(const char *name) const;

  std::vector<std::string> param_names_;
  std::vector<ParamType> param_types_;
  std::vector<ParamValue> param_values_;
};

#endif
