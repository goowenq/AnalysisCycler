#ifndef __DATA_MODEL_HH__
#define __DATA_MODEL_HH__
#include "TObject.h"

class AnaBuffer;

class AnaObject : public TObject{
  public:
    int site;
    int detectorid;
    int run;
    int file;
    int status;

    float file_start;
    float rawfiletime; 
    float integralruntime;
    float integrallivetimeblocked;
    float integrallivetimebuffer;

    float triggercounts;
    float flashercounts;
    float muoncounts;
    float ibdcounts;
    float spncounts;
    float blocktrigcounts;
    float plikecounts;
    float nlikecounts;
    float dtnegcounts;
    float dtlargegapcounts;
    float nhitcountsgt;
    float esumcountsgt;
    float randomcounts;
    float nhitcounts;
    float esumcounts;
    float admuoncounts;

    float muonnhit;
    float muonnhiterr; 
    float muonpesum;
    float muonpesumerr;

    float spnenergy;
    float spnenergysigma;
    float k40energy;
    float k40energyerr;
    float tl208energy;
    float tl208energyerr;

  private:
    // only AnaBuffer can delete AnaObject
    ~AnaObject(){}

  friend class AnaBuffer;
  ClassDef(AnaObject, 1);
};


#endif
