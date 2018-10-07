#ifndef __ANA_BUFFER_HH__
#define __ANA_BUFFER_HH__

class AnaObject;
class TChain;
class TObjArray;

class AnaBuffer{
  public:
    static AnaBuffer* instance();
    bool move_to_next_object();
    AnaObject* current_object();
    TObjArray* get_obj_array(); 

  protected:
    AnaBuffer();

  private:
    void update_buffer();
    bool build_new_entry(AnaObject* obj);

    static AnaBuffer*  m_instance;
    AnaObject* m_currentObj;
    TObjArray* m_array;

    TChain* m_chain;
    long m_currentChainEntryNum;

};

#endif
