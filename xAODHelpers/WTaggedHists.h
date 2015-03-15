#ifndef xAODHelpers_WTaggedHists_H
#define xAODHelpers_WTaggedHists_H

#include "xAODAnaHelpers/HistogramManager.h"
#include <xAODJet/JetContainer.h>

class WTaggedHists : public HistogramManager
{
  public:

    WTaggedHists(std::string name, std::string detailStr);
    ~WTaggedHists();

    EL::StatusCode initialize();
    EL::StatusCode execute( const xAOD::JetContainer* jets, float eventWeight );
    EL::StatusCode execute( const xAOD::Jet* jet, float eventWeight );
    EL::StatusCode execute( const xAOD::JetContainer* wtaggedJets, const xAOD::JetContainer* nontaggedJets, float eventWeight);

    using HistogramManager::book; // make other overloaded version of book() to show up in subclass
    using HistogramManager::execute; // overload

  private:
    //basic
    TH1F* m_jetPt;                  //!
    TH1F* m_jetEta;                 //!
    TH1F* m_jetPhi;                 //!
    TH1F* m_jetM;                   //!
    TH1F* m_jetE;                   //!
    TH1F* m_jetRapidity;            //!
    TH2F* m_jetPositions;           //!

    // substructure
    TH1F* m_jetTau1;                //!
    TH1F* m_jetTau2;                //!
    TH1F* m_jetTau21;               //!

    // DeltaR
    TH1F* m_WJ_DR;                  //!
    TH1F* m_WW_DR;                  //!

};

#endif
