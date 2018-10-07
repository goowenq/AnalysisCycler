#ifndef __ANA_ALGORITHM_HH__
#define __ANA_ALGORITHM_HH__

class TH1F;
class TTree;
class TFile;

#include <fstream>

class AnaAlgorithm{
  public:
    void initialize();    
    void execute();
    void finalize();

  private:
    TH1F* hprev;
    TFile* m_output;

    TTree* m_tree;
    int m_good;
    int m_site;
    int m_detectorid;
    int m_run;
    int m_file;
    long m_filestart;
    double m_triggercounts;
    double m_rawfiletime;
    double m_RMS;
    double m_highcut;
    double m_lowcut;
    double m_flashercounts;  
    double m_plikecounts;
    double m_nlikecounts;
    double m_ibdcounts;
    double m_spncounts;
    double m_muoncounts;
    double m_blocktrigcounts;
    double m_randomcounts;
    double m_nhitcountsgt;
    double m_esumcountsgt;
    double m_nhitcounts;
    double m_esumcounts; 
    double m_k40energy;
    double m_k40energyerr;
    double m_tl208energy;
    double m_tl208energyerr;
    double m_spnnergy;
    double m_spnenergysigma;
   
    double m_muonnhit; 
    double m_muonnhiterr;
    double m_muonpesum;   
    double m_muonpesumerr;

  
    std::ofstream m_fs;
};
#endif
