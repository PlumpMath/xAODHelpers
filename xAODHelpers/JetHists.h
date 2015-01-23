#ifndef xAODHelpers_JetHists_H
#define xAODHelpers_JetHists_H

#include <xAODHelpers/HistogramManager.h>
#include "xAODJet/JetContainer.h"

class JetHists : public HistogramManager {

  public:
    // initializer and destructor
    JetHists(std::string name, int detailLevel);
    ~JetHists();

    EL::StatusCode initialize();
    EL::StatusCode execute( const xAOD::JetContainer* jets, float eventWeight );
    using HistogramManager::book; // make other overloaded versions of book() to show up in subclass

  private:
    /* detailLevel >= 0 */
        // basics
        TH1 *h_jetPt; //!
        TH1 *h_jetM; //!
        TH1 *h_jetEta; //!
        TH1 *h_jetPhi; //!

        TH1 *h_rc_jetPt; //!
        TH1 *h_rc_jetM; //!

        // topological (eg: per event)
        TH1 *h_numJets; //!
        TH1 *h_numSubjets; //!
        TH1 *h_numrcJets; //!

        // scattering angle of two highest pt jets in CoM frame
        // number of top tags
        // number of W tags
        // angular separate between two highest pt top-tagged jets
        // relative mass difference between two highest pt top-tagged jets

      /* detailLevel >= 1 */
        // substructure
        TH1 *h_jetTau1; //!
        TH1 *h_jetTau2; //!
        TH1 *h_jetTau3; //!
        TH1 *h_jetDip12; //!
        TH1 *h_jetDip13; //!
        TH1 *h_jetDip23; //!
        TH1 *h_jet_numSubjets; //!
        TH1 *h_num_bTags; //!
        TH1 *h_num_bTags_withTruth; //!

        TH1 *h_mv1_discriminant; //!
};

#endif
