#ifndef __ANA_OPTION_PARSER_HH__
#define __ANA_OPTION_PARSER_HH__

#include "TString.h"
#include <vector>
#include <map>
using namespace std;

class AnaOptionParser{
  public:
    static AnaOptionParser* instance();
    TString getTString(TString);
    int getInt(TString);
    double getDouble(TString);

  protected:
    AnaOptionParser();

  private:
    static AnaOptionParser*  m_instance;
    TString m_filepath; // config file
    map<TString, TString> m_keymap;

    vector<TString> SplitLine(const TString);
};

#endif
