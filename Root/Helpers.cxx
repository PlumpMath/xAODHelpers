#include "xAODHelpers/Helpers.h"
#include "AthContainers/ConstDataVector.h"

// jet reclustering
#include <fastjet/PseudoJet.hh>
#include <fastjet/ClusterSequence.hh>

// jet trimming
#include <fastjet/tools/Filter.hh>
#include <JetEDM/JetConstituentFiller.h>

// what does this do?
#include <JetRec/JetFromPseudojet.h>

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

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*\
|                                                                              |
|   Author  : Giordon Stark                                                    |
|   Email   : gstark@cern.ch                                                   |
|   Thanks to Ben Nachman for inspiration                                      |
|                                                                              |
|   jet_reclustering():                                                        |
|       Takes a set of small-R jets and reclusters to large-R jets             |
|                                                                              |
|       @jets   : jet container to recluster and trim                          |
|       @radius : radius of large-R jet                                        |
|       @fcut   : trimming cut to apply                                        |
|       @rc_alg : clustering algorithm                                         |
|                                                                              |
|                                                                              |
\*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
std::vector<TLorentzVector> xAODHelpers::Helpers::jet_reclustering(
  const xAOD::JetContainer* jets,
  double radius,
  double fcut,
  fastjet::JetAlgorithm rc_alg
){

  //1. Need to convert the vector of jets to a vector of pseudojets
  //    only need p4() since we're using them as inputs
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
        return (lhs.Pt() > rhs.Pt());
      }
  };
  std::sort(rc_t_jets.begin(), rc_t_jets.end(), sort_by_pt());

  return rc_t_jets;
}

/* http://www.lpthe.jussieu.fr/~salam/fastjet/repo/doxygen-3.0alpha2/classfastjet_1_1Filter.html#usage */
std::vector<TLorentzVector> xAODHelpers::Helpers::jet_trimming(
  const xAOD::JetContainer* jets,
  double radius,
  double fcut,
  fastjet::JetAlgorithm rc_alg
){

  std::vector<TLorentzVector> t_jets;
  for(const auto jet: *jets){
    t_jets.push_back( jet_trimming(jet, radius, fcut, rc_alg) );
  }

  // notes: t_jets is not sorted by pt due to trimming applied
  struct sort_by_pt
  {
      inline bool operator() (const TLorentzVector lhs, const TLorentzVector rhs)
      {
        return (lhs.Pt() > rhs.Pt());
      }
  };
  std::sort(t_jets.begin(), t_jets.end(), sort_by_pt());

  return t_jets;
}

TLorentzVector xAODHelpers::Helpers::jet_trimming(
  const xAOD::Jet* jet,
  double radius,
  double fcut,
  fastjet::JetAlgorithm rc_alg
){

  //1. Create the trimmer
  fastjet::Filter trimmer(fastjet::JetDefinition(rc_alg, radius), fastjet::SelectorPtFractionMin(fcut));

  //2.  Apply the trimmer to the jet, this requires the JetEDM
  //        convert xAOD::Jet to PseudoJet with constituents
  //        apply trimmer on the PseudoJet
  TLorentzVector t_jet = TLorentzVector();
  std::vector<fastjet::PseudoJet> constit_pseudojets = jet::JetConstituentFiller::constituentPseudoJets(*jet);

  //3. Need to use ClusterSequence to recluster jet again once we found constituents
  fastjet::ClusterSequence cs(constit_pseudojets, fastjet::JetDefinition( (fastjet::JetAlgorithm) jet->getAlgorithmType(), jet->getSizeParameter()));

  fastjet::PseudoJet t_pjet = trimmer(fastjet::join(cs.inclusive_jets()));

  t_jet.SetPtEtaPhiE(
    t_pjet.pt(),
    t_pjet.eta(),
    t_pjet.phi(),
    t_pjet.e()
  );

  return t_jet;

}


void xAODHelpers::Helpers::jet_to_pj(std::vector<fastjet::PseudoJet>& out_pj, const xAOD::JetContainer* in_jets){
  for(auto jet : *in_jets){
    const TLorentzVector jet_p4 = jet->p4();
    out_pj.push_back(
      fastjet::PseudoJet(
        jet_p4.Px(),
        jet_p4.Py(),
        jet_p4.Pz(),
        jet_p4.E ()
      )
    );
  }
  return;
}

void xAODHelpers::Helpers::jet_reclustering(xAOD::JetContainer& out_jets, const xAOD::JetContainer* in_jets, double radius, fastjet::JetAlgorithm rc_alg){
  //1. Need to convert the vector of jets to a vector of pseudojets
  //    only need p4() since we're using them as inputs
  std::vector<fastjet::PseudoJet> input_jets;
  xAODHelpers::Helpers::jet_to_pj(input_jets, in_jets);

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

  JetFromPseudojet* pj2j_tool = new JetFromPseudojet("JetFromPseudoJetTool");
  //pj2j_tool->setProperty("Attributes", std::vector<std::string>("...", ..., "...");

  const xAOD::JetInput::Type input_type = (*in_jets)[0]->getInputType();
  xAOD::JetTransform::Type transform_type(xAOD::JetTransform::UnknownTransform);
  switch(rc_alg){
    case fastjet::antikt_algorithm:
      transform_type = xAOD::JetTransform::AntiKtRecluster;
    break;
    case fastjet::kt_algorithm:
      transform_type = xAOD::JetTransform::KtRecluster;
    break;
    case fastjet::cambridge_algorithm:
      transform_type = xAOD::JetTransform::CamKtRecluster;
    break;
    default:
      transform_type = xAOD::JetTransform::UnknownTransform;
    break;
  }

  for(auto rc_jet: rc_jets){
    xAOD::Jet* jet_from_pj = pj2j_tool->add(rc_jet, out_jets, nullptr);
    jet_from_pj->setInputType(input_type);
    jet_from_pj->setAlgorithmType(xAOD::JetAlgorithmType::undefined_jet_algorithm);
    jet_from_pj->setSizeParameter(radius);
    jet_from_pj->auxdecor<int>("TransformType") = transform_type;
  }

  delete pj2j_tool;

  return;
}
