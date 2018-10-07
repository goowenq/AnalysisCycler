#include "AnaBuffer.hh"
#include <iostream>
#include <fstream>
#include "TString.h"
#include "TChain.h"
#include "TLeaf.h"
#include "TObjArray.h"
#include "AnaObject.hh"
#include "AnaOptionParser.hh"
using namespace std;

AnaBuffer* AnaBuffer::m_instance = 0;

AnaBuffer* AnaBuffer::instance()
{
  if(m_instance==0){
    m_instance = new AnaBuffer();
  }
  return m_instance;
}

AnaBuffer::AnaBuffer()
:m_currentChainEntryNum(-1)
{
  AnaOptionParser* parser = AnaOptionParser::instance();
  TString treeName = parser->getTString("anabuffer_input_tree_name");
  m_chain = new TChain(treeName);
  //m_chain = new TChain("mysql_tree");

  TString fileName = parser->getTString("anabuffer_input_file_name");
  m_chain->Add(fileName);
  //m_chain->Add("test/fullsites.root");

  m_array = new TObjArray();
  cout << "Analysis Buffer Constructed. " << endl;
}

bool AnaBuffer::move_to_next_object(){
  if(m_currentObj==0){
    update_buffer();
    return true;
  }
  else{
    update_buffer();
  }

  AnaObject* prevObj = m_currentObj;
  m_currentObj = (AnaObject*)m_array->After(prevObj);
  // TObjArray::After() returns 0 if that's the last
  if(m_currentObj==0) return false;
  else return true;
}

AnaObject* AnaBuffer::current_object(){
  // need a type cast
  return (AnaObject*)m_currentObj;
}

TObjArray* AnaBuffer::get_obj_array(){
  return m_array;
}

void AnaBuffer::update_buffer()
{
  // the first time enter into the chain
  if(m_currentChainEntryNum==-1){
    AnaObject* newObj = new AnaObject();
    build_new_entry(newObj);
    m_array->AddLast(newObj);
    m_currentObj = (AnaObject*)m_array->First();
  }

  AnaOptionParser* parser = AnaOptionParser::instance();
  double forwardTime = parser->getDouble("anabuffer_forward_time_sec");
  double backwardTime = parser->getDouble("anabuffer_backward_time_sec");
  //double forwardTime = 86400;
  //double backwardTime = 86400;
  // remove the old entries off the buffer
  while(! m_array->IsEmpty()){
    AnaObject* theObj = (AnaObject*)m_array->First();
    double dt = m_currentObj->file_start - theObj->file_start;
    if(dt > backwardTime){
      //m_array->Remove(theObj);
      m_array->RemoveFirst();
      // delete the object to avoid mem leakage
      delete theObj;
      // can improve further, only Compress() once
      m_array->Compress();
    }
    else{
      break;
    }
  }

  // read entries and save into buffer
  double dt=0;
  do{
    // do-while make sure update the buffer at least for one time
    AnaObject* newObj = new AnaObject();
    bool status =build_new_entry(newObj);
    if(status==0){
      //cout << "null newObj ..."<< endl;
      delete newObj; // avoid mem leak
      break;
    }
    m_array->AddLast(newObj);
    dt = newObj->file_start - m_currentObj->file_start;
  }
  while(dt < forwardTime);
  
}

bool AnaBuffer::build_new_entry(AnaObject* obj){
  // initial value is -1
  m_currentChainEntryNum ++;
  //
  //cout << "build current entry: " << m_currentChainEntryNum << endl;
  int byte = m_chain->GetEntry(m_currentChainEntryNum); 
  if(byte > 0){
    m_chain->SetBranchStatus("*",0);
    m_chain->SetBranchStatus("site",1);
    m_chain->SetBranchStatus("detectorid",1);
    m_chain->SetBranchStatus("run",1);
    m_chain->SetBranchStatus("file",1);
    m_chain->SetBranchStatus("status",1);
    m_chain->SetBranchStatus("file_start",1);
    m_chain->SetBranchStatus("rawfiletime",1);
    m_chain->SetBranchStatus("integralruntime",1);
    m_chain->SetBranchStatus("integrallivetimeblocked",1);
    m_chain->SetBranchStatus("integrallivetimebuffer",1);
    m_chain->SetBranchStatus("triggercounts",1);
    m_chain->SetBranchStatus("flashercounts",1);
    m_chain->SetBranchStatus("muoncounts",1);
    m_chain->SetBranchStatus("ibdcounts",1);
    m_chain->SetBranchStatus("spncounts",1);
    m_chain->SetBranchStatus("blocktrigcounts",1);
    m_chain->SetBranchStatus("plikecounts",1);
    m_chain->SetBranchStatus("nlikecounts",1);
    m_chain->SetBranchStatus("dtnegcounts",1);
    m_chain->SetBranchStatus("dtlargegapcounts",1);
    m_chain->SetBranchStatus("nhitcountsgt",1);
    m_chain->SetBranchStatus("esumcountsgt",1);
    m_chain->SetBranchStatus("randomcounts",1);
    m_chain->SetBranchStatus("nhitcounts",1);
    m_chain->SetBranchStatus("esumcounts",1);
    m_chain->SetBranchStatus("admuoncounts",1);
    m_chain->SetBranchStatus("muonnhit",1);
    m_chain->SetBranchStatus("muonnhiterr",1);
    m_chain->SetBranchStatus("muonpesum",1);
    m_chain->SetBranchStatus("muonpesumerr",1);
    m_chain->SetBranchStatus("spnenergy",1);
    m_chain->SetBranchStatus("spnenergysigma",1);
    m_chain->SetBranchStatus("k40energy",1);
    m_chain->SetBranchStatus("k40energyerr",1);
    m_chain->SetBranchStatus("tl208energy",1);
    m_chain->SetBranchStatus("tl208energyerr",1);

    obj->site = m_chain->GetLeaf("site")->GetValue();
    obj->detectorid = m_chain->GetLeaf("detectorid")->GetValue();
    obj->run = m_chain->GetLeaf("run")->GetValue();
    obj->file = m_chain->GetLeaf("file")->GetValue();
    obj->status = m_chain->GetLeaf("status")->GetValue();

    obj->file_start = m_chain->GetLeaf("file_start")->GetValue();
    obj->rawfiletime = m_chain->GetLeaf("rawfiletime")->GetValue();
    obj->integralruntime = m_chain->GetLeaf("integralruntime")->GetValue();
    obj->integrallivetimeblocked = m_chain->GetLeaf("integrallivetimeblocked")->GetValue();
    obj->integrallivetimebuffer = m_chain->GetLeaf("integrallivetimebuffer")->GetValue();

    obj->triggercounts = m_chain->GetLeaf("triggercounts")->GetValue();
    obj->flashercounts = m_chain->GetLeaf("flashercounts")->GetValue();
    obj->muoncounts = m_chain->GetLeaf("muoncounts")->GetValue();
    obj->ibdcounts = m_chain->GetLeaf("ibdcounts")->GetValue();
    obj->spncounts = m_chain->GetLeaf("spncounts")->GetValue();
    obj->blocktrigcounts = m_chain->GetLeaf("blocktrigcounts")->GetValue();
    obj->plikecounts = m_chain->GetLeaf("plikecounts")->GetValue();
    obj->nlikecounts = m_chain->GetLeaf("nlikecounts")->GetValue();
    obj->dtnegcounts = m_chain->GetLeaf("dtnegcounts")->GetValue();
    obj->dtlargegapcounts = m_chain->GetLeaf("dtlargegapcounts")->GetValue();
    obj->nhitcountsgt = m_chain->GetLeaf("nhitcountsgt")->GetValue();
    obj->esumcountsgt = m_chain->GetLeaf("esumcountsgt")->GetValue();
    obj->randomcounts = m_chain->GetLeaf("randomcounts")->GetValue();
    obj->nhitcounts = m_chain->GetLeaf("nhitcounts")->GetValue();
    obj->esumcounts = m_chain->GetLeaf("esumcounts")->GetValue();
    obj->admuoncounts = m_chain->GetLeaf("admuoncounts")->GetValue();

    obj->muonnhit = m_chain->GetLeaf("muonnhit")->GetValue();
    obj->muonnhiterr = m_chain->GetLeaf("muonnhiterr")->GetValue();
    obj->muonpesum = m_chain->GetLeaf("muonpesum")->GetValue();
    obj->muonpesumerr = m_chain->GetLeaf("muonpesumerr")->GetValue();

    obj->spnenergy = m_chain->GetLeaf("spnenergy")->GetValue();
    obj->spnenergysigma = m_chain->GetLeaf("spnenergysigma")->GetValue();
    obj->k40energy = m_chain->GetLeaf("k40energy")->GetValue();
    obj->k40energyerr = m_chain->GetLeaf("k40energyerr")->GetValue();
    obj->tl208energy = m_chain->GetLeaf("tl208energy")->GetValue();
    obj->tl208energyerr = m_chain->GetLeaf("tl208energyerr")->GetValue();

    //cout << "build entry: " << m_currentChainEntryNum << " ; file_start= " << obj->file_start  << endl;
    return true;
  }
  else{
    //cout << "not loadable entry: " << m_currentChainEntryNum << endl;
    m_currentChainEntryNum --;
    //cout << "please check if the last entry: " <<  m_currentChainEntryNum << endl;
    return false;
  }

}
