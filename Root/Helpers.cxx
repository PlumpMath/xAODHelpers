#include "xAODHelpers/Helpers.h"
#include "AthContainers/ConstDataVector.h"

// jet reclustering
#include <fastjet/PseudoJet.hh>
#include <fastjet/ClusterSequence.hh>

xAODHelpers :: Helpers :: Helpers ()
{
}

bool xAODHelpers::Helpers::check_bTag_cut(const xAOD::Jet* jet, double bTagCut)
{
  const xAOD::BTagging* btag = jet->btagging();
  if(bool(btag)){
    double mv1 = btag->MV1_discriminant();
    return mv1 >= bTagCut;
  }
  return false;
}

bool xAODHelpers::Helpers::check_truthLabel_ID(const xAOD::Jet* jet, int truthLabel_ID)
{
  if( jet->isAvailable<int>("TruthLabelID") ){
    return jet->getAttribute<int>("TruthLabelID") == truthLabel_ID;
  }
  return false;
}

int xAODHelpers::Helpers::count_container_btags(const xAOD::JetContainer* jets, double bTagCut)
{
  int num_bTags = 0;

  for(auto jet : *jets){
    if(xAODHelpers::Helpers::check_bTag_cut(jet, bTagCut)) num_bTags++;
  }

  return num_bTags;
}

const xAOD::JetContainer xAODHelpers::Helpers::select_container_btags(const xAOD::JetContainer* jets, double bTagCut)
{
  /* Email to PATHelp: "Correctly using SG::VIEW_ELEMENTS?"
     xAOD::JetContainer==DataVector<xAOD::Jet> will not accept a const object.
     If you look in the DataVector header, you’ll discover that you have to
     instead use the ConstDataVector<xAOD::JetContainer> object, which is a
     non-const container that allows you to push_back a const object (note that
     the template argument is the DataVector type and not the object type).
     ConstDataVector<xAOD::JetContainer> can return a pointer to const
     DataVector<xAOD::Jet> if you call its “asDataVector” method.

     Note that you can also use vector<xAOD::Jet*> for simple things like
     looping, but ConstDataVector is the way to go if you need a subset of a
     const container to pass to some method that expects a DataVector input.
  */
  ConstDataVector<xAOD::JetContainer> selectedJets(SG::VIEW_ELEMENTS);

  for(auto jet : *jets){
    if(xAODHelpers::Helpers::check_bTag_cut(jet, bTagCut)) selectedJets.push_back( jet );
  }
  return *selectedJets.asDataVector();
}

void xAODHelpers::Helpers::select_container_btags(const xAOD::JetContainer* jets, double bTagCut, std::string decoratorName)
{
  for(auto jet: *jets){
    if(xAODHelpers::Helpers::check_bTag_cut(jet, bTagCut) ) jet->auxdecor<int>(decoratorName) = 1;
  }

}

int xAODHelpers::Helpers::count_truthLabel_byID(const xAOD::JetContainer* jets, int truthLabel_ID)
{
  int num_truthLabel = 0;
  for(auto jet: *jets){
    if(xAODHelpers::Helpers::check_truthLabel_ID(jet, truthLabel_ID)) num_truthLabel++;
  }
  return num_truthLabel;
}

const xAOD::JetContainer xAODHelpers::Helpers::match_largeR_jet_to_smallR_jets(const xAOD::Jet* largeR_jet, const xAOD::JetContainer* smallR_jets)
{
  /* Email to PATHelp: "Correctly using SG::VIEW_ELEMENTS?"
     xAOD::JetContainer==DataVector<xAOD::Jet> will not accept a const object.
     If you look in the DataVector header, you’ll discover that you have to
     instead use the ConstDataVector<xAOD::JetContainer> object, which is a
     non-const container that allows you to push_back a const object (note that
     the template argument is the DataVector type and not the object type).
     ConstDataVector<xAOD::JetContainer> can return a pointer to const
     DataVector<xAOD::Jet> if you call its “asDataVector” method.

     Note that you can also use vector<xAOD::Jet*> for simple things like
     looping, but ConstDataVector is the way to go if you need a subset of a
     const container to pass to some method that expects a DataVector input.
  */
  ConstDataVector<xAOD::JetContainer> matchedJets(SG::VIEW_ELEMENTS);

  // get properties such as the 4-vector and radius of jet
  TLorentzVector largeR_jet4Vector = largeR_jet->p4();
  double jetSizeParameter = largeR_jet->getSizeParameter();

  for(auto smallR_jet : *smallR_jets){
    if(largeR_jet4Vector.DeltaR( smallR_jet->p4() ) <= jetSizeParameter ) matchedJets.push_back( smallR_jet );
  }
  return *matchedJets.asDataVector();
}

std::vector<TLorentzVector> xAODHelpers::Helpers::jet_reclustering(
  const xAOD::JetContainer* jets,
  double radius,
  double fcut,
  fastjet::JetAlgorithm rc_alg
){

  //1. Need to convert the vector of jets to a vector of pseudojets
  std::vector<fastjet::PseudoJet> input_jets;

  for(auto jet : *jets){
    const TLorentzVector jet_p4 = jet->p4();
    input_jets.push_back(
      fastjet::PseudoJet(
        jet_p4.Px()/1000.,
        jet_p4.Py()/1000.,
        jet_p4.Pz()/1000.,
        jet_p4.E ()/1000.
      )
    );
  }

  //2. Build up the new jet definitions using input configurations
  //    - jet algorithm
  //    - radius
  fastjet::JetDefinition jet_def(rc_alg, radius);

  //3. Run the Cluster Sequence on pseudojets with the right jet definition above
  //    cs = clustersequence
  fastjet::ClusterSequence cs(input_jets, jet_def);

  // 4. Grab the reclustered jets, sorted by pt()
  //    rc_jets == reclustered jets
  std::vector<fastjet::PseudoJet> rc_jets = fastjet::sorted_by_pt(cs.inclusive_jets());

  //5.  Apply trimming on PJ.constituents() using fcut
  //    rc_t_jets == reclustered, trimmed jets
  std::vector<TLorentzVector> rc_t_jets;

  for(auto rc_jet : rc_jets){
    TLorentzVector rc_t_jet = TLorentzVector();
    // loop over subjets
    for(auto rc_jet_subjet : rc_jet.constituents()){
      TLorentzVector subjet = TLorentzVector();
      subjet.SetPtEtaPhiE(
        rc_jet_subjet.pt(),
        rc_jet_subjet.eta(),
        rc_jet_subjet.phi(),
        rc_jet_subjet.e()
      );
      if(subjet.Pt() > fcut*rc_jet.pt()) rc_t_jet += subjet;
    }
    rc_t_jets.push_back(rc_t_jet);
  }

  // notes: rc_t_jets is not sorted by pt due to trimming applied
  struct sort_by_pt
  {
      inline bool operator() (const TLorentzVector lhs, const TLorentzVector rhs)
      {
        return (lhs.Pt() < rhs.Pt());
      }
  };
  std::sort(rc_t_jets.begin(), rc_t_jets.end(), sort_by_pt());

  return rc_t_jets;
}
