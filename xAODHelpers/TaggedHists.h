#ifndef xAODHelpers_TaggedHists_H
#define xAODHelpers_TaggedHists_H

#include "xAODAnaHelpers/HistogramManager.h"
#include <xAODJet/JetContainer.h>

class TaggedHists : public HistogramManager
{
  public:

    TaggedHists(std::string name, std::string detailStr);
    ~TaggedHists();

    StatusCode initialize();
    StatusCode execute( const xAOD::JetContainer* jets, float eventWeight );

    using HistogramManager::book; // make other overloaded version of book() to show up in subclass
    using HistogramManager::execute; // overload

  private:
    TH1F* m_jetTags;               //!


};

#endif
