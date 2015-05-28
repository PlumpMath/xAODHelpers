#ifndef xAODHelpers_LeadingJetKinematicHists_H
#define xAODHelpers_LeadingJetKinematicHists_H

#include "xAODAnaHelpers/HistogramManager.h"
#include <xAODJet/JetContainer.h>

class LeadingJetKinematicHists : public HistogramManager
{
  public:

    LeadingJetKinematicHists(std::string name, std::string detailStr, std::string prepend);
    ~LeadingJetKinematicHists();

    StatusCode initialize();
    StatusCode execute( const xAOD::JetContainer* jets, float eventWeight );
    StatusCode execute( const xAOD::Jet* jet, float eventWeight );

    using HistogramManager::book; // make other overloaded version of book() to show up in subclass
    using HistogramManager::execute; // overload

    std::string m_prepend;          //!

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

};

#endif
