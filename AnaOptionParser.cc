// usage: edit option.cfg 
// AnaOptionParser* parser = AnaOptionParser::instance();
// cout << parser.getDouble("key1") << endl;
// cout << parser.getTString("key2") << endl;
// 

#include "AnaOptionParser.hh"
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include "TObjArray.h"
#include "TString.h"
#include "TObjString.h"
using namespace std;

AnaOptionParser* AnaOptionParser::m_instance = 0;

AnaOptionParser* AnaOptionParser::instance()
{
  if(m_instance==0){
    m_instance = new AnaOptionParser();
  }
  return m_instance;
}

AnaOptionParser::AnaOptionParser()
: m_filepath("options.cfg")
{
  ifstream cfg(m_filepath); 
  TString aLine;
  while(! cfg.eof()){
    aLine.ReadLine(cfg);
    //cout << aLine << endl;
    vector<TString> ss = SplitLine(aLine);
    if(ss.size()>0){
      m_keymap[ss[0]] = ss[1];
      //cout << ss[0] << " " << ss[1] << endl;
    }
  }
  cfg.close();
}

TString AnaOptionParser::getTString(TString name){
  try{
    TString res = m_keymap[name];
    if(res.IsNull()) throw name.Data();
    return res;
  }
  catch(char const *e){
    cerr << "AnaOptionParser doesn't exist option: " << e << endl; 
  }
}

int AnaOptionParser::getInt(TString name){
  try{ 
    TString res = getTString(name);
    if(! res.IsFloat()) throw name.Data();
    return m_keymap[name].Atoi();
  }
  catch(char const* e){
    cerr << "AnaOptionParser non-int variable: " << e << endl; 
  }
}

double AnaOptionParser::getDouble(TString name){
  try{ 
    TString res = getTString(name);
    if(! res.IsFloat()) throw name.Data();
    return m_keymap[name].Atof();
  }
  catch(char const* e){
    cerr << "AnaOptionParser non-float variable: " << e << endl; 
  }
}

vector<TString> AnaOptionParser::SplitLine(const TString line)
{
  vector<TString> words;
  words.clear();
  char delimiters[] = " \t\n;,=";
  TObjArray* Strings = line.Tokenize(delimiters);
  if(Strings->GetEntriesFast()) {
    TIter iString(Strings);
    TObjString* os=0;
    while ((os=(TObjString*)iString())) {
      //cout<<os->GetString().Data()<<endl;
      words.push_back(os->GetString());
    }
  }
  return words;
}
