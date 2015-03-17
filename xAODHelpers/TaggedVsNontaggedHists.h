#ifndef xAODHelpers_TaggedVsNontaggedHists_H
#define xAODHelpers_TaggedVsNontaggedHists_H

#include "xAODAnaHelpers/HistogramManager.h"
#include <xAODJet/JetContainer.h>

class TaggedVsNontaggedHists : public HistogramManager
{
  public:

    TaggedVsNontaggedHists(std::string name, std::string detailStr);
    ~TaggedVsNontaggedHists();

    EL::StatusCode initialize();
    EL::StatusCode execute( const xAOD::Jet* leading_taggedJet, const xAOD::Jet* leading_nontaggedJet, float eventWeight);

    using HistogramManager::book; // make other overloaded version of book() to show up in subclass
    using HistogramManager::execute; // overload

  private:
    TH1F* m_WJ_M;                   //!
    TH1F* m_WJ_pT;                  //!
    TH1F* m_WJ_DR;                  //!
};

#endif
