#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include "xAODJet/JetContainer.h"
#include "xAODTracking/VertexContainer.h"
#include "xAODEventInfo/EventInfo.h"
#include "AthContainers/ConstDataVector.h"

#include <xAODHelpers/WTaggedHists.h>
#include <xAODHelpers/TaggedVsNontaggedHists.h>
#include <xAODHelpers/LeadingJetKinematicHists.h>

#include <xAODHelpers/WTaggedHistsAlgo.h>
#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/HelperClasses.h>

#include <xAODAnaHelpers/tools/ReturnCheck.h>
#include <xAODAnaHelpers/tools/ReturnCheckConfig.h>

// w-boson tag
#include "JetSubStructureUtils/BosonTag.h"

#include "TEnv.h"
#include "TSystem.h"

// this is needed to distribute the algorithm to the workers
ClassImp(WTaggedHistsAlgo)

WTaggedHistsAlgo :: WTaggedHistsAlgo () {
}

WTaggedHistsAlgo :: WTaggedHistsAlgo (std::string name, std::string configName) :
  Algorithm(),
  m_name(name),
  m_configName(configName),
  m_plots0W(nullptr),
  m_plots1W(nullptr),
  m_plots2W(nullptr),
  m_kinematics0W(nullptr),
  m_kinematics1W(nullptr),
  m_kinematics2W(nullptr),
  m_taggedVsNonTagged0W(nullptr),
  m_taggedVsNonTagged1W(nullptr),
  m_taggedVsNonTagged2W(nullptr)
{
}

EL::StatusCode WTaggedHistsAlgo :: setupJob (EL::Job& job)
{
  job.useXAOD();
  xAOD::Init("WTaggedHistsAlgo").ignore();

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode WTaggedHistsAlgo :: histInitialize ()
{

  Info("histInitialize()", "%s", m_name.c_str() );
  // needed here and not in initalize since this is called first
  Info("histInitialize()", "Attempting to configure using: %s", m_configName.c_str());
  if ( this->configure() == EL::StatusCode::FAILURE ) {
    Error("histInitialize()", "%s failed to properly configure. Exiting.", m_name.c_str() );
    return EL::StatusCode::FAILURE;
  } else {
    Info("histInitialize()", "Succesfully configured! \n");
  }


  // declare class and add histograms to output
  m_plots0W = new WTaggedHists(m_name+"0W", m_detailStr);
  m_plots1W = new WTaggedHists(m_name+"1W", m_detailStr);
  m_plots2W = new WTaggedHists(m_name+"2W", m_detailStr);

  m_kinematics0W = new LeadingJetKinematicHists(m_name+"0W", m_detailStr, "leading_nonwjet");
  m_kinematics1W = new LeadingJetKinematicHists(m_name+"1W", m_detailStr, "leading_nonwjet");
  m_kinematics2W = new LeadingJetKinematicHists(m_name+"2W", m_detailStr, "leading_nonwjet");

  m_w_kinematics1W = new LeadingJetKinematicHists(m_name+"1W", m_detailStr, "leading_wjet");
  m_w_kinematics2W = new LeadingJetKinematicHists(m_name+"2W", m_detailStr, "leading_wjet");

  m_taggedVsNonTagged0W = new TaggedVsNontaggedHists(m_name+"0W", m_detailStr);
  m_taggedVsNonTagged1W = new TaggedVsNontaggedHists(m_name+"1W", m_detailStr);
  m_taggedVsNonTagged2W = new TaggedVsNontaggedHists(m_name+"2W", m_detailStr);

  m_plotsHolder.push_back(m_plots0W);
  m_plotsHolder.push_back(m_plots1W);
  m_plotsHolder.push_back(m_plots2W);
  m_plotsHolder.push_back(m_kinematics0W);
  m_plotsHolder.push_back(m_kinematics1W);
  m_plotsHolder.push_back(m_kinematics2W);
  m_plotsHolder.push_back(m_w_kinematics1W);
  m_plotsHolder.push_back(m_w_kinematics2W);
  m_plotsHolder.push_back(m_taggedVsNonTagged0W);
  m_plotsHolder.push_back(m_taggedVsNonTagged1W);
  m_plotsHolder.push_back(m_taggedVsNonTagged2W);

  for(auto plotGroup: m_plotsHolder){
    plotGroup -> initialize();
    plotGroup -> record( wk() );
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode WTaggedHistsAlgo :: configure ()
{
  m_configName = gSystem->ExpandPathName( m_configName.c_str() );
  RETURN_CHECK_CONFIG( "WTaggedHistsAlgo::configure()", m_configName);

  // the file exists, use TEnv to read it off
  TEnv* config = new TEnv(m_configName.c_str());
  m_inContainerName         = config->GetValue("InputContainer",  "");
  m_detailStr               = config->GetValue("DetailStr",       "");
  m_decorationName          = config->GetValue("DecorationName", "wtag");

  // in case anything was missing or blank...
  if( m_inContainerName.empty() || m_detailStr.empty() ){
    Error("configure()", "One or more required configuration values are empty");
    return EL::StatusCode::FAILURE;
  }
  Info("configure()", "Loaded in configuration values");

  // everything seems preliminarily ok, let's print config and say we were successful
  config->Print();
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode WTaggedHistsAlgo :: fileExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode WTaggedHistsAlgo :: changeInput (bool /*firstFile*/) { return EL::StatusCode::SUCCESS; }

EL::StatusCode WTaggedHistsAlgo :: initialize ()
{
  Info("initialize()", "WTaggedHistsAlgo");
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode WTaggedHistsAlgo :: execute ()
{

  // load up the w-tagger
  static JetSubStructureUtils::BosonTag bosonTagger("medium", "smooth", "$ROOTCOREBIN/data/JetSubStructureUtils/config_13TeV_20150528_Ztagging.dat", false, true, true);

  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("WTaggedHistsAlgo::execute()", HelperFunctions::retrieve(eventInfo, "EventInfo", m_event, m_store, false), "");

  float eventWeight(1);
  if( eventInfo->isAvailable< float >( "eventWeight" ) ) {
    eventWeight = eventInfo->auxdecor< float >( "eventWeight" );
  }

  // this will be the collection processed - no matter what!!
  const xAOD::JetContainer* inJets(nullptr);
  RETURN_CHECK("WTaggedHistsAlgo::execute()", HelperFunctions::retrieve(inJets, m_inContainerName, m_event, m_store, false), "Could not retrieve the jet");

  if(!inJets->size() > 0) return EL::StatusCode::SUCCESS;

  // original containers are sorted by pt
  ConstDataVector<xAOD::JetContainer> nontaggedJets(SG::VIEW_ELEMENTS);
  ConstDataVector<xAOD::JetContainer> wtaggedJets(SG::VIEW_ELEMENTS);

  // loop over, split into two containers
  for(const auto jet: *inJets){
    // skip anythin less than 2.5 GeV
    if(jet->pt()/1e3 < 2.5) continue;

    if(bosonTagger.result(*jet)) {
      wtaggedJets.push_back(jet);
    } else {
      nontaggedJets.push_back(jet);
    }
  }

  // convert back to actual const containers
  const xAOD::JetContainer* nontagged_jets  = nontaggedJets.asDataVector();
  const xAOD::JetContainer* wtagged_jets    = wtaggedJets.asDataVector();

  if(nontagged_jets->size() == 0 || wtagged_jets == 0) return EL::StatusCode::SUCCESS;

  switch( static_cast<int>(wtagged_jets->size()) ){
      case 0:
        m_plots0W->execute( inJets, eventWeight );
        m_kinematics0W->execute( (*nontagged_jets)[0], eventWeight );
      break;
      case 1:
        m_plots1W->execute( inJets, eventWeight );
        m_kinematics1W->execute( (*nontagged_jets)[0], eventWeight );
        m_w_kinematics1W->execute( (*wtagged_jets)[0], eventWeight );
        m_taggedVsNonTagged1W->execute( (*wtagged_jets)[0], (*nontagged_jets)[0], eventWeight );
      break;
      case 2:
        m_plots2W->execute( inJets, eventWeight );
        m_kinematics2W->execute( (*nontagged_jets)[0], eventWeight );
        m_w_kinematics2W->execute( (*wtagged_jets)[0], eventWeight );
        m_taggedVsNonTagged2W->execute( (*wtagged_jets)[0], (*nontagged_jets)[0], eventWeight );
      break;
      default:
        Info("execute()", "More than 2 W-tags???");
      break;
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode WTaggedHistsAlgo :: postExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode WTaggedHistsAlgo :: finalize () { return EL::StatusCode::SUCCESS; }

EL::StatusCode WTaggedHistsAlgo :: histFinalize () {
  // clean up memory
  for(auto plotGroup: m_plotsHolder){
    if(plotGroup){
      delete plotGroup;
      plotGroup = 0;
    }
  }
  m_plotsHolder.clear();

  return EL::StatusCode::SUCCESS;
}
