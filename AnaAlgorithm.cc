#include "AnaAlgorithm.hh"
#include <iostream>
#include "AnaBuffer.hh"
#include "AnaObject.hh"
#include "TObjArray.h"
#include "TH1F.h"
#include "TMath.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "AnaOptionParser.hh"
using namespace std;

void AnaAlgorithm::initialize(){
  std::cout << "AnaAlgorithm::initialize()" << std::endl;

  hprev = new TH1F("hprev","hprev",1000,0,450);

  AnaOptionParser* parser = AnaOptionParser::instance();
  TString filepath = parser->getTString("anaalg_output_path");
  m_output = new TFile(filepath, "RECREATE");
  m_tree = new TTree("tree","tree");
  m_tree->Branch("good", &m_good, "good/I");
  m_tree->Branch("site", &m_site, "site/I");
  m_tree->Branch("detectorid", &m_detectorid, "detectorid/I");
  m_tree->Branch("run", &m_run, "run/I");
  m_tree->Branch("file", &m_file, "file/I");
  m_tree->Branch("filestart", &m_filestart, "filestart/L");
  m_tree->Branch("triggercounts", &m_triggercounts, "triggercounts/D");
  m_tree->Branch("rawfiletime", &m_rawfiletime, "rawfiletime/D");
  m_tree->Branch("highcut",&m_highcut,"highcut/D");
  m_tree->Branch("lowcut",&m_lowcut,"lowcut/D");
  m_tree->Branch("flashercounts",&m_flashercounts,"flashercounts/D");
  m_tree->Branch("plikecounts",&m_plikecounts,"plikecounts/D");
  m_tree->Branch("nlikecounts",&m_nlikecounts,"nlikecounts/D");
  m_tree->Branch("ibdcounts",&m_ibdcounts,"ibdcounts/D");
  m_tree->Branch("spncounts",&m_spncounts,"spncounts/D");
  m_tree->Branch("muoncounts",&m_muoncounts,"muoncounts/D"); 
  m_tree->Branch("blocktrigcounts",&m_blocktrigcounts,"blocktrigcounts/D");
  m_tree->Branch("randomcounts",&m_randomcounts,"randomcounts/D");
  
  m_tree->Branch("nhitcountsgt",&m_nhitcountsgt,"nhitcountsgt/D");
  m_tree->Branch("esumcountsgt",&m_esumcountsgt,"esumcountsgt/D");
  m_tree->Branch("nhitcounts",&m_nhitcounts,"nhitcounts/D");
  m_tree->Branch("esumcounts",&m_esumcounts,"esumcounts/D");

  m_tree->Branch("k40energy",&m_k40energy,"k40energy/D");
  m_tree->Branch("k40energyerr",&m_k40energyerr,"k40energyerr/D");
  m_tree->Branch("tl208energy",&m_tl208energy,"tl208energy/D");
  m_tree->Branch("tl208energyerr",&m_tl208energyerr,"tl208energyserr/D");
  m_tree->Branch("spnenergy",&m_spnnergy,"spnenergy/D");
  m_tree->Branch("spnenergysigma",&m_spnenergysigma,"spnenergysigma/D");
 
  m_tree->Branch("muonnhit",&m_muonnhit,"muonnhit/D");  
  m_tree->Branch("muonnhiterr",&m_muonnhiterr,"muonnhiterr/D");
  m_tree->Branch("muonpesum",&m_muonpesum,"muonpesum/D");
  m_tree->Branch("muonpesumerr",&m_muonpesumerr,"muonpesumerr/D");
                   


  m_tree->Branch("RMS",&m_RMS,"RMS/D");
 
  TString path = parser->getTString("anaalg_report_path");
  //m_fs.open(path, std::ofstream::out | std::ofstream::trunc);
  m_fs.open(path);
}

void AnaAlgorithm::execute(){
  AnaBuffer* buf = AnaBuffer::instance(); 
  AnaObject* currentObj = buf->current_object();
  TObjArray* bufArray = buf->get_obj_array();
  AnaOptionParser* parser = AnaOptionParser::instance();

  //std::cout << "-------------------------------------" << std::endl;
  //std::cout << "current entry: site= " << currentObj->site;
  //std::cout << " run= " << currentObj->run;
  //std::cout << " file= " << currentObj->file;
  //std::cout << " detectorid= " << currentObj->detectorid;
  //std::cout << " file_start= " << long(currentObj->file_start);
  //std::cout << " rawfiletime= " << currentObj->rawfiletime;
  //std::cout << " triggercounts= " << currentObj->triggercounts << std::endl;

  int status = currentObj->status;
  if(status==0){
    cout << "skip bad file" << endl;
    return; // skip, already tagged as bad
  }

  m_site = currentObj->site;
  m_detectorid = currentObj->detectorid;
  m_run = currentObj->run;
  m_file = currentObj->file;
  m_good = 1; // initial status is good
  m_plikecounts = currentObj->plikecounts;   
  m_nlikecounts = currentObj->nlikecounts;
  m_ibdcounts = currentObj->ibdcounts;
  m_spncounts = currentObj->spncounts;
  m_muoncounts = currentObj->muoncounts;
  m_blocktrigcounts = currentObj->blocktrigcounts;
  m_randomcounts = currentObj->randomcounts;
  m_nhitcountsgt = currentObj->nhitcountsgt;
  m_esumcountsgt = currentObj->esumcountsgt;
  m_nhitcounts = currentObj->nhitcounts;
  m_esumcounts = currentObj->esumcounts;
  m_k40energy = currentObj->k40energy;
  m_k40energyerr = currentObj->k40energyerr;
  m_tl208energy = currentObj->tl208energy;
  m_tl208energyerr = currentObj->tl208energyerr;
  m_spnnergy = currentObj->spnenergy;
  m_spnenergysigma = currentObj->spnenergysigma;
  m_muonnhit = currentObj->muonnhit;
  m_muonnhiterr = currentObj->muonnhiterr;
  m_muonpesum = currentObj->muonpesum;
  m_muonpesumerr = currentObj->muonpesumerr;
  m_flashercounts = currentObj->flashercounts;                 
  m_filestart = currentObj->file_start;
  m_triggercounts = currentObj->triggercounts;
  m_rawfiletime = currentObj->rawfiletime;
  m_flashercounts = currentObj->flashercounts;
  double triggerrate = m_triggercounts/m_rawfiletime;
  if(m_detectorid>4){
     m_good = 1;
     m_tree->Fill();
     return; // currently only process AD
  }
  else if(m_detectorid==-10000){
    TString word;
    word.Form("%d;%d;not in DB yet;\n", m_run, m_file);
    m_fs << word;
    cout << word;
    return;
  }

//  m_filestart = currentObj->file_start;
//  m_triggercounts = currentObj->triggercounts;
//  m_rawfiletime = currentObj->rawfiletime;
//  double triggerrate = m_triggercounts/m_rawfiletime;
  // lowest level check
  double r1 = currentObj->integrallivetimebuffer / currentObj->integralruntime;
  double r2 = currentObj->integrallivetimeblocked / currentObj->integralruntime;
  if( r1<0.998 || r1>1 || r2<0.998 || r2>1){
    TString word;
    word.Form("%d;%d;%d;site=%d and det=%d: livetime inconsistent;\n", m_run, m_file, m_detectorid, m_site, m_detectorid, triggerrate);
    m_fs << word;
    cout << word;
    m_good = 0;
  }
  if(m_rawfiletime<20){
    TString word;
    word.Form("%d;%d;%d;site=%d and det=%d: too short run(<20s);\n", m_run, m_file, m_detectorid, m_site, m_detectorid, triggerrate);
    m_fs << word;
    cout << word;
    m_good = 0;
  }
  if(currentObj->blocktrigcounts/currentObj->triggercounts>0.005){
    TString word;
    word.Form("%d;%d;%d;site=%d and det=%d: block trig too high(>0.5%);\n", m_run, m_file, m_detectorid, m_site, m_detectorid, triggerrate);
    m_fs << word;
    cout << word;
    m_good = 0;
  }
  if(currentObj->dtnegcounts>0){
    TString word;
    word.Form("%d;%d;%d;site=%d and det=%d: inverse timing problem;\n", m_run, m_file, m_detectorid, m_site, m_detectorid, triggerrate);
    m_fs << word;
    cout << word;
    m_good = 0;
  }
  if(currentObj->dtlargegapcounts>0){
    TString word;
    word.Form("%d;%d;%d;site=%d and det=%d: large gap in timging;\n", m_run, m_file, m_detectorid, m_site, m_detectorid, triggerrate);
    m_fs << word;
    cout << word;
    m_good = 0;
  }
  

  TString title1;
  title1.Form("anaalg_site%d_det%d_low", m_site, m_detectorid);
  TString title2;
  title2.Form("anaalg_site%d_det%d_high", m_site, m_detectorid);
  double preliminary_low_cut = parser->getDouble(title1);
  double preliminary_high_cut = parser->getDouble(title2);
  if(triggerrate > preliminary_high_cut){
    TString word;
    word.Form("%d;%d;%d;site=%d and det=%d too high trigger rate: %.1f;\n", m_run, m_file, m_detectorid, m_site, m_detectorid, triggerrate);
    m_fs << word;
    cout << word;
    m_good = 0;
  }
  else if(triggerrate < preliminary_low_cut){
    TString word;
    word.Form("%d;%d;%d;site=%d and det=%d too low trigger rate: %.1f;\n", m_run, m_file, m_detectorid, m_site, m_detectorid, triggerrate);
    m_fs << word;
    cout << word;
    m_good = 0;
  }
  else{
    hprev->Reset();
    // retrieve buffer element
    long maxLength = bufArray->GetEntries();
    for(long jj=0; jj<maxLength; jj++){
      AnaObject* theObj = (AnaObject*)bufArray->At(jj);
      //double dt = currentObj->file_start - theObj->file_start;
      //if(dt > 86400) continue;
      //if(dt < -86400) break;
      int status = theObj->status;
      if(status==0) continue; // skip bad files
      if(theObj->site == m_site && theObj->detectorid == m_detectorid){
        hprev->Fill(theObj->triggercounts/theObj->rawfiletime);
      }
    }

    double meanrate = hprev->GetMean();
    m_highcut = meanrate*1.02; // allow 2% fluctuation
    m_lowcut = meanrate*0.98;

    if(triggerrate>m_highcut || triggerrate<m_lowcut){
      TString word;
      word.Form("%d;%d;%d;site=%d and det=%d: 2 percent trigger rate(%.1f Hz) deviation to normal value(%.1f Hz) within 1 day;\n", m_run, m_file, m_detectorid, m_site, m_detectorid, triggerrate, meanrate);
      m_fs << word;
      cout << word;
      m_good = 0;

      //std::cout << "-------------------------------------" << std::endl;
      //std::cout << "find a bad entry: site= " << currentObj->site;
      //std::cout << " run= " << currentObj->run;
      //std::cout << " file= " << currentObj->file;
      //std::cout << " detectorid= " << currentObj->detectorid;
      //std::cout << " file_start= " << long(currentObj->file_start);
      //std::cout << " rawfiletime= " << currentObj->rawfiletime;
      //std::cout << " triggercounts= " << currentObj->triggercounts << std::endl;

    }
  }
  //
  m_tree->Fill();
    
}

void AnaAlgorithm::finalize(){
  m_tree->Write();
  m_output->Close();
  m_fs.close();
  std::cout << "AnaAlgorithm::finalize()" << std::endl;
}
