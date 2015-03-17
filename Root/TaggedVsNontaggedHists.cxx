#include <xAODHelpers/TaggedVsNontaggedHists.h>

TaggedVsNontaggedHists :: TaggedVsNontaggedHists (std::string name, std::string detailStr) :
  HistogramManager(name, detailStr)
{
}

TaggedVsNontaggedHists :: ~TaggedVsNontaggedHists () {}

EL::StatusCode TaggedVsNontaggedHists::initialize() {

  m_WJ_DR           = book(m_name, "DR(J,W1)", "#Delta R(J^{1}, W^{1})", 100, 0, 2*TMath::Pi());

  Info("TaggedVsNontaggedHists::initialize()", m_name.c_str());

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TaggedVsNontaggedHists::execute( const xAOD::Jet* leading_wtaggedJet, const xAOD::Jet* leading_nontaggedJet, float eventWeight){

    m_WJ_DR -> Fill( fabs(leading_wtaggedJet->p4().DeltaR(leading_nontaggedJet->p4())), eventWeight );

  return EL::StatusCode::SUCCESS;
}
