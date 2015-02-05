#include "xAODRootAccess/Init.h"
#include "SampleHandler/SampleHandler.h"
#include "SampleHandler/ToolsDiscovery.h"
#include "EventLoop/Job.h"
#include "EventLoop/DirectDriver.h"
#include "SampleHandler/DiskListLocal.h"
#include <TSystem.h>

#include "xAODHelpers/JetKinematics.h"

int main( int argc, char* argv[] ) {

  // Take the submit directory from the input if provided:
  std::string submitDir = "submitDir";
  if( argc > 1 ) submitDir = argv[ 1 ];

  // Set up the job for xAOD access:
  xAOD::Init().ignore();

  // Construct the samples to run on:
  SH::SampleHandler sh;

  const char* inputFilePath = "/share/t3data/kratsg/xAODs";
  SH::DiskListLocal list (inputFilePath);
  /* 2.0.23
  SH::scanDir (sh, list, "*.root*", "*110351*"); // specifying one particular sample
  */
  SH::scanDir (sh, list, "AOD.01604209._000001.pool.root.1");

  // Set the name of the input TTree. It's always "CollectionTree"
  // for xAOD files.
  sh.setMetaString( "nc_tree", "CollectionTree" );

  // Print what we found:
  sh.print();

  // Create an EventLoop job:
  EL::Job job;
  job.sampleHandler( sh );

  // Add our analysis to the job:
  JetKinematics* jk_AntiKt10LC = new JetKinematics();
  /* This is the default container used */
  jk_AntiKt10LC->m_jetContainerName = "AntiKt10LCTopoJets";
  jk_AntiKt10LC->m_jetDisplayName = "AntiKt10";
  jk_AntiKt10LC->m_jetDetailStr = "foo";

  JetKinematics* jk_AntiKt4LC = new JetKinematics();
  /* This is the default container used */
  jk_AntiKt4LC->m_jetContainerName = "AntiKt4LCTopoJets";
  jk_AntiKt4LC->m_jetDisplayName = "AntiKt4";
  jk_AntiKt4LC->m_jetDetailStr = "foo";

  /*
  JetKinematics* jk_AntiKt4Truth = new JetKinematics();
  jk_AntiKt4Truth->m_jetContainerName = "AntiKt4TruthJets";
  jk_AntiKt4Truth->m_jetDisplayName = "Truth4";
  jk_AntiKt4Truth->m_jetDetailStr = "foo";

  // this is an xAOD::JetTrigContainer
  JetKinematics* jk_HLT = new JetKinematics();
  jk_HLT->m_jetContainerName = "HLT_xAOD__JetContainer_TrigHLTJetRec";
  jk_HLT->m_jetDisplayName = "HLT";
  jk_HLT->m_jetDetailStr = "foo";
  */

  // Attach algorithms
  job.algsAdd( jk_AntiKt10LC );
  job.algsAdd( jk_AntiKt4LC );
  //job.algsAdd( jk_AntiKt4Truth );


  // Run the job using the local/direct driver:
  EL::DirectDriver driver;
  driver.submit( job, submitDir );

  return 0;
}

