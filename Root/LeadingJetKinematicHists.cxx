#include <xAODHelpers/LeadingJetKinematicHists.h>

LeadingJetKinematicHists :: LeadingJetKinematicHists (std::string name, std::string detailStr) :
  HistogramManager(name, detailStr)
{
}

LeadingJetKinematicHists :: ~LeadingJetKinematicHists () {}

EL::StatusCode LeadingJetKinematicHists::initialize() {

  m_jetPt           = book(m_name, "wjetPt",  "W-Jet p_{T} [GeV]", 120, 0, 600);
  m_jetEta          = book(m_name, "wjetEta", "W-Jet #eta",         80, -4, 4);
  m_jetPhi          = book(m_name, "wjetPhi", "W-Jet #phi",120, -TMath::Pi(), TMath::Pi() );
  m_jetM            = book(m_name, "wjetMass", "W-Jet Mass [GeV]",120, 0, 400);
  m_jetE            = book(m_name, "wjetEnergy", "W-Jet Energy [GeV]",120, 0, 4000);
  m_jetRapidity     = book(m_name, "wjetRapidity", "W-Jet Rapidity",120, -10, 10);

  m_jetPositions    = book(m_name, "wjetPositions", "W-Jet #eta", 80, -4, 4, "W-Jet #phi", 120, -TMath::Pi(), TMath::Pi() );

  m_jetTau1         = book(m_name, "wjetTau1", "W-Jet #tau_{1}", 100, 0, 1);
  m_jetTau2         = book(m_name, "wjetTau2", "W-Jet #tau_{2}", 100, 0, 1);
  m_jetTau21        = book(m_name, "wjetTau21", "W-Jet #tau_{21}", 100, 0, 1);

  Info("LeadingJetKinematicHists::initialize()", m_name.c_str());

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode LeadingJetKinematicHists::execute( const xAOD::JetContainer* jets, float eventWeight ) {
  return this->execute( (*jets)[0], eventWeight);
}

EL::StatusCode LeadingJetKinematicHists::execute( const xAOD::Jet* jet, float eventWeight ) {

  //basic
  m_jetPt ->        Fill( jet->pt()/1e3,    eventWeight );
  m_jetEta->        Fill( jet->eta(),       eventWeight );
  m_jetPhi->        Fill( jet->phi(),       eventWeight );
  m_jetM->          Fill( jet->m()/1e3,     eventWeight );
  m_jetE->          Fill( jet->e()/1e3,     eventWeight );
  m_jetRapidity->   Fill( jet->rapidity(),  eventWeight );

  m_jetPositions->  Fill( jet->eta(), jet->phi(), eventWeight);

  static SG::AuxElement::ConstAccessor<float> Tau1 ("Tau1");
  static SG::AuxElement::ConstAccessor<float> Tau2 ("Tau2");
  if( Tau1.isAvailable(*jet) && Tau2.isAvailable(*jet) ){
    m_jetTau1-> Fill( Tau1(*jet), eventWeight );
    m_jetTau2-> Fill( Tau2(*jet), eventWeight );
    m_jetTau21->Fill( Tau2(*jet)/Tau1(*jet), eventWeight );
  }

  return EL::StatusCode::SUCCESS;
}
