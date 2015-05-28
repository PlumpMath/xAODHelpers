#ifndef xAODHelpers_WTaggedHists_H
#define xAODHelpers_WTaggedHists_H

#include "xAODAnaHelpers/HistogramManager.h"
#include <xAODJet/JetContainer.h>

class WTaggedHists : public HistogramManager
{
  public:

    WTaggedHists(std::string name, std::string detailStr);
    ~WTaggedHists();

    StatusCode initialize();
    StatusCode execute( const xAOD::JetContainer* jets, float eventWeight );

    using HistogramManager::book; // make other overloaded version of book() to show up in subclass
    using HistogramManager::execute; // overload

  private:
    TH1F* m_jetTags;               //!


};

#endif
