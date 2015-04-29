#ifndef xAODHelpers_Helpers_H
#define xAODHelpers_Helpers_H

// we make use of this a lot
#include <TLorentzVector.h>

// include AntiKt4Truth Jets for btagging
#include "xAODJet/JetContainer.h"
// aux container for deep copies
#include "xAODJet/JetAuxContainer.h"
// deal with btagging
#include "xAODBTagging/BTagging.h"

// jet reclustering
#include <fastjet/JetDefinition.hh>

namespace xAODHelpers {

  class Helpers {

  public:
    Helpers();

    struct sort_by_pt;

    // booleans to help check for certain properties
    bool check_bTag_cut(const xAOD::Jet* jet, double bTagCut);
    bool check_truthLabel_ID(const xAOD::Jet* jet, int truthLabel_ID);

    // helper functions to quickly count
    int count_container_btags (const xAOD::JetContainer* jets, double bTagCut);
    int count_truthLabel_byID (const xAOD::JetContainer* jets, int truthLabel_ID);

    // helper functions to quickly select
    const xAOD::JetContainer select_container_btags(const xAOD::JetContainer* jets, double bTagCut);
    void select_container_btags(const xAOD::JetContainer* jets, double bTagCut, std::string decoratorName);

    // given a largeR jet and a container of smallR jets, return a std::pair of dR matching jets
    const xAOD::JetContainer match_largeR_jet_to_smallR_jets(const xAOD::Jet* largeR_jet, const xAOD::JetContainer* smallR_jets);

    // jet reclustering and trimming
    std::vector<TLorentzVector> jet_reclustering(const xAOD::JetContainer* jets, double radius = 1.0, double fcut = 0.05, fastjet::JetAlgorithm rc_alg = fastjet::antikt_algorithm);
    std::vector<TLorentzVector> jet_trimming(const xAOD::JetContainer* jets, double radius = 0.3, double fcut = 0.05, fastjet::JetAlgorithm rc_alg = fastjet::kt_algorithm);
    TLorentzVector jet_trimming(const xAOD::Jet* jet, double radius = 0.3, double fcut = 0.05, fastjet::JetAlgorithm rc_alg = fastjet::kt_algorithm);

    void jet_to_pj(std::vector<fastjet::PseudoJet>& out_pj, const xAOD::JetContainer* in_jets);
    void jet_reclustering(xAOD::JetContainer& out_jets, const xAOD::JetContainer* in_jets, double radius = 1.0, fastjet::JetAlgorithm rc_alg = fastjet::antikt_algorithm);


    template<typename T>
    T sort_container_pt(T* inCont){
      T sortedCont(SG::VIEW_ELEMENTS);
      for(auto el : *inCont) sortedCont.push_back( el );

      std::sort(sortedCont.begin(), sortedCont.end(), xAODHelpers::Helpers::sort_by_pt());
      return sortedCont;
    }

    template<typename T>
    const T sort_container_pt(const T* inCont){
      ConstDataVector<T> sortedCont(SG::VIEW_ELEMENTS);

      for(auto el : *inCont) sortedCont.push_back( el );
      std::sort(sortedCont.begin(), sortedCont.end(), xAODHelpers::Helpers::sort_by_pt());
      return *sortedCont.asDataVector();
    }

  private:

  };
}
#endif
