#ifndef UCHelpers_JetHists_H
#define UCHelpers_JetHists_H

#include <UCHelpers/HistogramManager.h>
#include "xAODJet/JetContainer.h"


class JetHists : public HistogramManager {

  public:
    // initializer and destructor
    JetHists();
    ~JetHists();

    EL::StatusCode initialize(bool sumw2 = true);
    EL::StatusCode execute();
    EL::StatusCode finalize();
    using HistogramManager::book; // make other overloaded versions of book() to show up in subclass

    // basics
    TH1 *h_jetPt; //!
    TH1 *h_jetM; //!
    TH1 *h_jetEta; //!
    TH1 *h_jetPhi; //!

    // topological (eg: per event)
    TH1 *h_numJets; //!
    TH1 *h_numSubjets; //!
    TH1 *h_numBTags; //!

    // scattering angle of two highest pt jets in CoM frame
    // number of top tags
    // number of W tags
    // angular separate between two highest pt top-tagged jets
    // relative mass difference between two highest pt top-tagged jets

    // substructure
    TH1 *h_jetTau1; //!
    TH1 *h_jetTau2; //!
    TH1 *h_jetTau3; //!
    TH1 *h_jetDip12; //!
    TH1 *h_jetDip13; //!
    TH1 *h_jetDip23; //!
    TH1 *h_jet_numSubjets; //!

    TH1 *h_mv1_discriminant; //!
    TH1 *h_num_bTags; //!

  private:
    bool isTrigger();
};

#endif
