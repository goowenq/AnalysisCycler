//#include "globalVar.hh"

#include "AnaAlgorithm.hh"
#include "AnaBuffer.hh"
#include "AnaOptionParser.hh"

#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
  
  AnaAlgorithm* alg = new AnaAlgorithm();
  alg->initialize();

  AnaOptionParser* parser = AnaOptionParser::instance();
  long max_entry = parser->getInt("main_max_entries");

  AnaBuffer* buf = AnaBuffer::instance(); 
  long currentEntry = 0;
  while(buf->move_to_next_object()){
    alg->execute();
    currentEntry++;
    if(max_entry>0 && currentEntry>=max_entry) break;
  }

  alg->finalize();
  return 0;
}

