#include <xAODHelpers/TaggedVsNontaggedHists.h>

TaggedVsNontaggedHists :: TaggedVsNontaggedHists (std::string name, std::string detailStr) :
  HistogramManager(name, detailStr)
{
}

TaggedVsNontaggedHists :: ~TaggedVsNontaggedHists () {}

StatusCode TaggedVsNontaggedHists::initialize() {

  m_WJ_DR           = book(m_name, "DR(J,W1)", "| #Delta R(J^{1}, W^{1}) |", 100, 0, 2*TMath::Pi());
  m_WJ_DEta         = book(m_name, "DEta(J,W1)", "| #Delta #eta (J^{1}, W^{1}) |", 100, 0, 10);
  m_WJ_DPhi         = book(m_name, "DPhi(J,W1)", "| #Delta #phi (J^{1}, W^{1}) |", 100, 0, 2*TMath::Pi());

  m_WJ_m            = book(m_name, "Mass between Leading Jets", "W-Jet M [GeV]", 120, 0, 400, "Non-W Jet M [GeV]", 120, 0, 400);
  m_WJ_pt           = book(m_name, "Pt between Leading Jets", "W-Jet p_{T} [GeV]", 120, 0, 600, "Non-W Jet p_{T} [GeV]", 120, 0, 600);
  m_WJ_eta          = book(m_name, "Eta between Leading Jets", "W-Jet #eta", 100, -4.9, 4.9, "Non-W Jet #eta", 100, -4.9, 4.9);
  m_WJ_phi          = book(m_name, "Phi between Leading Jets", "W-Jet #phi", 100, -TMath::Pi(), TMath::Pi(), "Non-W Jet #phi", 100, -TMath::Pi(), TMath::Pi());

  Info("TaggedVsNontaggedHists::initialize()", m_name.c_str());

  return StatusCode::SUCCESS;
}

StatusCode TaggedVsNontaggedHists::execute( const xAOD::Jet* leading_wtaggedJet, const xAOD::Jet* leading_nontaggedJet, float eventWeight){

    TLorentzVector wjet = leading_wtaggedJet->p4();
    TLorentzVector nonwjet = leading_nontaggedJet->p4();

    m_WJ_DR     -> Fill( fabs(wjet.DeltaR(nonwjet)), eventWeight );
    m_WJ_DEta   -> Fill( fabs(wjet.Eta() - nonwjet.Eta()), eventWeight );
    m_WJ_DPhi   -> Fill( fabs(wjet.DeltaPhi( nonwjet )), eventWeight );

    m_WJ_m -> Fill( leading_wtaggedJet->m()/1000., leading_nontaggedJet->m()/1000., eventWeight);
    m_WJ_pt -> Fill( leading_wtaggedJet->pt()/1000., leading_nontaggedJet->pt()/1000., eventWeight);
    m_WJ_eta -> Fill( leading_wtaggedJet->eta(), leading_nontaggedJet->eta(), eventWeight);
    m_WJ_phi -> Fill( leading_wtaggedJet->phi(), leading_nontaggedJet->phi(), eventWeight);

  return StatusCode::SUCCESS;
}
