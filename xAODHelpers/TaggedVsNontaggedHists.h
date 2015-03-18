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
    TH2F* m_WJ_m;                   //!
    TH2F* m_WJ_pt;                  //!
    TH2F* m_WJ_eta;                 //!
    TH2F* m_WJ_phi;                 //!
    TH1F* m_WJ_DR;                  //!
    TH1F* m_WJ_DEta;                //!
    TH1F* m_WJ_DPhi;                //!
};

#endif
