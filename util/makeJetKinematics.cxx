#include "xAODRootAccess/Init.h"
#include "SampleHandler/SampleHandler.h"
#include "SampleHandler/ToolsDiscovery.h"
#include "EventLoop/Job.h"
#include "EventLoop/DirectDriver.h"
#include "SampleHandler/DiskListLocal.h"
#include <TSystem.h>

#include "xAODHelpers/JetReclustering.h"
#include <xAODJetReclustering/JetReclusteringAlgo.h>

int main( int argc, char* argv[] ) {

  // Take the submit directory from the input if provided:
  std::string submitDir = "submitDir";
  if( argc > 1 ) submitDir = argv[ 1 ];

  // Set up the job for xAOD access:
  xAOD::Init().ignore();

  // Construct the samples to run on:
  SH::SampleHandler sh;

  const char* inputFilePath = "/share/t3data3/kratsg/xAODs";
  SH::DiskListLocal list (inputFilePath);
  /* 2.0.23
  SH::scanDir (sh, list, "*.root*", "*110351*"); // specifying one particular sample
  */
  SH::scanDir (sh, list, "*", "mc15*JETM8*");

  // Set the name of the input TTree. It's always "CollectionTree"
  // for xAOD files.
  sh.setMetaString( "nc_tree", "CollectionTree" );

  // Print what we found:
  sh.print();

  // Create an EventLoop job:
  EL::Job job;
  job.sampleHandler( sh );

  // initialize and set it up
  JetReclusteringAlgo* jetReclusterer = new JetReclusteringAlgo();
  jetReclusterer->m_inputJetContainer = "AntiKt4LCTopoJets";
  jetReclusterer->m_outputJetContainer = "AntiKt10LCTopoJetsRCAntiKt4LCTopoJets";
  jetReclusterer->m_name = "R10"; // unique name for the tool
  jetReclusterer->m_ptMin_input = 25.0; // GeV
  jetReclusterer->m_ptMin_rc = 50.0; // GeV
  jetReclusterer->m_ptFrac = 0.05; // GeV

  // Add our analysis to the job:
  JetReclustering* jc = new JetReclustering();

  // Attach algorithms
  //job.algsAdd(jetReclusterer);
  job.algsAdd( jc );


  // Run the job using the local/direct driver:
  EL::DirectDriver driver;
  driver.submit( job, submitDir );

  return 0;
}

