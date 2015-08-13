#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include "xAODJet/JetContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODJet/JetAuxContainer.h"

#include "xAODTracking/VertexContainer.h"
#include "xAODEventInfo/EventInfo.h"
#include "AthContainers/ConstDataVector.h"

#include <xAODHelpers/TaggedHists.h>
#include <xAODHelpers/TaggedVsNontaggedHists.h>
#include <xAODHelpers/LeadingJetKinematicHists.h>

#include <xAODHelpers/TaggedHistsAlgo.h>
#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/HelperClasses.h>

#include <xAODAnaHelpers/tools/ReturnCheck.h>
#include <xAODAnaHelpers/tools/ReturnCheckConfig.h>

// h tagger
#include "JetSubStructureUtils/BoostedXbbTag.h"

#include "TEnv.h"
#include "TSystem.h"

// this is needed to distribute the algorithm to the workers
ClassImp(TaggedHistsAlgo)

TaggedHistsAlgo :: TaggedHistsAlgo () {
}

TaggedHistsAlgo :: TaggedHistsAlgo (std::string name, std::string configName) :
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

EL::StatusCode TaggedHistsAlgo :: setupJob (EL::Job& job)
{
  job.useXAOD();
  xAOD::Init("TaggedHistsAlgo").ignore();

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TaggedHistsAlgo :: histInitialize ()
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
  m_plots0W = new TaggedHists(m_name+"0W", m_detailStr);
  m_plots1W = new TaggedHists(m_name+"1W", m_detailStr);
  m_plots2W = new TaggedHists(m_name+"2W", m_detailStr);

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

EL::StatusCode TaggedHistsAlgo :: configure ()
{
  m_configName = gSystem->ExpandPathName( m_configName.c_str() );
  RETURN_CHECK_CONFIG( "TaggedHistsAlgo::configure()", m_configName);

  // the file exists, use TEnv to read it off
  TEnv* config = new TEnv(m_configName.c_str());
  m_inContainerName         = config->GetValue("InputContainer",  "");
  m_inMuonContainerName     = config->GetValue("InputMuonContainer",  "");
  m_detailStr               = config->GetValue("DetailStr",       "");
  m_decorationName          = config->GetValue("DecorationName", "wtag");

  // in case anything was missing or blank...
  if( m_inContainerName.empty() || m_detailStr.empty() || m_inMuonContainerName.empty() ){
    Error("configure()", "One or more required configuration values are empty");
    return EL::StatusCode::FAILURE;
  }

  Info("configure()", "Loaded in configuration values");

  // everything seems preliminarily ok, let's print config and say we were successful
  config->Print();
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TaggedHistsAlgo :: fileExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode TaggedHistsAlgo :: changeInput (bool /*firstFile*/) { return EL::StatusCode::SUCCESS; }

EL::StatusCode TaggedHistsAlgo :: initialize ()
{
  Info("initialize()", "TaggedHistsAlgo");
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TaggedHistsAlgo :: execute ()
{

  // load up the Xbb tagger
  static std::string wk_pt("medium");
  static std::string recs_file("$ROOTCOREBIN/data/JetSubStructureUtils/config_13TeV_20150812_Htagging.dat");
  static std::string boson_type("Higgs");
  static std::string alg_name("AK10LCTRIMF5R20");
  static int num_bTags(2);
  static bool debug(true);
  static bool verbose(true);
  static JetSubStructureUtils::BoostedXbbTag HTagger(wk_pt, recs_file, boson_type, alg_name, num_bTags, debug, verbose);

  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("TaggedHistsAlgo::execute()", HelperFunctions::retrieve(eventInfo, "EventInfo", m_event, m_store, false), "");

  float eventWeight(1);
  if( eventInfo->isAvailable< float >( "eventWeight" ) ) {
    eventWeight = eventInfo->auxdecor< float >( "eventWeight" );
  }

  // this will be the collection processed - no matter what!!
  const xAOD::JetContainer* inJets(nullptr);
  RETURN_CHECK("TaggedHistsAlgo::execute()", HelperFunctions::retrieve(inJets, m_inContainerName, m_event, m_store, false), "Could not retrieve the jets");

  if(!inJets->size() > 0) return EL::StatusCode::SUCCESS;

  const xAOD::MuonContainer* inMuons(nullptr);
  RETURN_CHECK("TaggedHistsAlgo::execute()", HelperFunctions::retrieve(inMuons, m_inMuonContainerName, m_event, m_store, false), "Could not retrieve the muons");


  // original containers are sorted by pt
  ConstDataVector<xAOD::JetContainer> nontaggedJets(SG::VIEW_ELEMENTS);
  ConstDataVector<xAOD::JetContainer> taggedJets(SG::VIEW_ELEMENTS);

  // loop over, split into two containers
  for(const auto jet: *inJets){
    // skip anythin less than 2.5 GeV
    if(jet->pt()/1e3 < 2.5) continue;

    if(HTagger.result(*jet, "AK10LCTRIMF5R20", inMuons)) {
      taggedJets.push_back(jet);
    } else {
      nontaggedJets.push_back(jet);
    }
  }

  // convert back to actual const containers
  const xAOD::JetContainer* nontagged_jets  = nontaggedJets.asDataVector();
  const xAOD::JetContainer* tagged_jets    = taggedJets.asDataVector();

  if(nontagged_jets->size() == 0 || tagged_jets == 0) return EL::StatusCode::SUCCESS;

  switch( static_cast<int>(tagged_jets->size()) ){
      case 0:
        m_plots0W->execute( inJets, eventWeight );
        m_kinematics0W->execute( (*nontagged_jets)[0], eventWeight );
      break;
      case 1:
        m_plots1W->execute( inJets, eventWeight );
        m_kinematics1W->execute( (*nontagged_jets)[0], eventWeight );
        m_w_kinematics1W->execute( (*tagged_jets)[0], eventWeight );
        m_taggedVsNonTagged1W->execute( (*tagged_jets)[0], (*nontagged_jets)[0], eventWeight );
      break;
      case 2:
        m_plots2W->execute( inJets, eventWeight );
        m_kinematics2W->execute( (*nontagged_jets)[0], eventWeight );
        m_w_kinematics2W->execute( (*tagged_jets)[0], eventWeight );
        m_taggedVsNonTagged2W->execute( (*tagged_jets)[0], (*nontagged_jets)[0], eventWeight );
      break;
      default:
        Info("execute()", "More than 2 W-tags???");
      break;
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TaggedHistsAlgo :: postExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode TaggedHistsAlgo :: finalize () { return EL::StatusCode::SUCCESS; }

EL::StatusCode TaggedHistsAlgo :: histFinalize () {
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
