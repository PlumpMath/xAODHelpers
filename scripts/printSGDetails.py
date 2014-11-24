#!/usr/bin/env python

# @file:    printSGDetails.py
# @purpose: read a ROOT file containing xAOD Objects and dump the contents
# @author:  Giordon Stark <gstark@cern.ch>
# @date:    November 2014
#
# @example:
# @code
# printSGDetails.py aod.pool.root
# printSGDetails.py aod.pool.root --interactive
# printSGDetails.py aod.pool.root --help
# @endcode
#

# First check if ROOTCOREDIR exists, implies ROOTCORE is set up
import os, sys
try:
  os.environ['ROOTCOREDIR']
except KeyError:
  print "It appears RootCore is not set up. Please set up RootCore and then try running me again."
  print "\tHint: try running `rcSetup`"
  sys.exit(1)


if __name__ == "__main__":
  import argparse

  parser = argparse.ArgumentParser(description='Process xAOD File and Dump Information.')
  # positional argument, require the first argument to be the input filename
  parser.add_argument('input_filename',
                      type=str,
                      help='an input root file to read')
  # these are options allowing for various additional configurations in filtering container and types to dump
  parser.add_argument('-o',
                      '--output',
                      type=str,
                      required=False,
                      dest='output_filename',
                      help='output file to store dump',
                      default=None)
  parser.add_argument('-t',
                      '--type',
                      type=str,
                      required=False,
                      dest='container_type_regex',
                      help='Regex specification for the xAOD container type',
                      default='*')
  parser.add_argument('-c',
                      '--container',
                      type=str,
                      required=False,
                      dest='container_name_regex',
                      help='Regex specification for the xAOD container name',
                      default='*')

  # additional verbosity arguments (flags)
  parser.add_argument('--prop',
                      dest='list_properties',
                      action='store_true',
                      help='Enable to print properties of container. By default, it only prints the xAOD::ContainerType and containers for that given type. This is like an increased verbosity option for container properties.')
  parser.add_argument('--attr',
                      dest='list_attributes',
                      action='store_true',
                      help='Enable to print attributes of container. By default, it only prints the xAOD::ContainerType and containers for that given type. This is like an increased verbosity option for container attributes.')

  parser.add_argument('--filterProps'  ,
                      type=str,
                      required=False,
                      dest='property_name_regex',
                      help='Regex specification for xAOD property names. Only used if --prop enabled.',
                      default='*')
  parser.add_argument('--filterAttrs',
                      type=str,
                      required=False,
                      dest='attribute_name_regex',
                      help='Regex specification for xAOD attribute names. Only used if --attr enabled.',
                      default='*')

  parser.add_argument('-i',
                      '--interactive',
                      dest='interactive',
                      action='store_true',
                      help='Flip on/off interactive mode allowing you to navigate through the container types and properties')

  # parse the arguments, throw errors if missing any
  args = parser.parse_args()

  # defaultdict is useful for parsing the file
  from collections import defaultdict
  # use regular expressions to filter everything down
  import re

  # Set up ROOT and RootCore (do we need RootCore??)
  import ROOT
  ROOT.gROOT.Macro('$ROOTCOREDIR/scripts/load_packages.C')

  # Initialize the xAOD infrastructure
  ROOT.xAOD.Init()

  # Set up the input files (PDSF)
  fileName = '/share/t3data/kratsg/xAODs/mc14_13TeV.110351.PowhegPythia_P2012_ttbar_allhad.merge.AOD.e3232_s1982_s2008_r5787_r5853_tid01604209_00/AOD.01604209._000001.pool.root.1'
  treeName = 'CollectionTree'
  
  '''
  # redirect stdout, stderr here
  sys_stdout = sys.stdout
  sys_stderr = sys.stderr
  nullity = open(os.devnull, 'w')
  sys.stdout = nullity
  sys.stderr = nullity
  '''

  f = ROOT.TFile.Open(fileName)

  # Make the "transient tree" ? I guess we don't need to
  t = f.Get(treeName)

  # Print some information
  print('Number of input events: %s' % t.GetEntries())

  xAODObjects = {}  # list of properties and methods given Container Name
  xAODContainers = defaultdict(list)  # list of objects given type

  xAOD_Container_Name = re.compile('([^\.]+)$')
  xAOD_AuxContainer_Name = re.compile('(.*)Aux\.$')
  xAOD_Container_Prop = re.compile('(.*)Aux\.([^:]+)$')
  xAOD_Container_Attr = re.compile('(.*)AuxDyn\.([^:]+)$')

  xAOD_Type_Name = re.compile('^(?:vector<)?(.+?)(?: ?> ?)?$')

  elements = t.GetListOfLeaves()
  for i in range(elements.GetEntries()):
    el = elements.At(i)

    elName = el.GetName()
    elType = xAOD_Type_Name.match(el.GetTypeName()).groups()[0].replace('Aux','').split('_')[0]

    m_cont_name = xAOD_Container_Name.match(elName)
    m_aux_name = xAOD_AuxContainer_Name.match(elName)
    m_cont_prop = xAOD_Container_Prop.match(elName)
    m_cont_attr = xAOD_Container_Attr.match(elName)

    if m_aux_name:
      container, = m_aux_name.groups()
      xAODObjects[container] = {'prop': [], 'attr': [], 'type': elType}
      xAODContainers[elType].append(container)

    elif m_cont_prop:
      container, property = m_cont_prop.groups()
      xAODObjects[container]['prop'].append({'name': property, 'type': elType})

    elif m_cont_attr:
      container, attribute = m_cont_attr.groups()
      xAODObjects[container]['attr'].append({'name': attribute, 'type': elType})

  # dump to file
  with open('xAODContainers_By_Type.dump', 'w+') as f:
    for ContainerType, ContainerNames in sorted(xAODContainers.items()):
      f.write('%s\n\t%s\n' % (ContainerType, '\n\t'.join(sorted(ContainerNames))))

  with open('xAODContainers_Detail.dump', 'w+') as f:
    for ContainerName, Elements in sorted(xAODObjects.items()):
      f.write('%s* %s\n' % (Elements['type'], ContainerName))
      for prop in sorted(Elements['prop'], key=lambda k: k['name']):
        f.write('\t%s &->%s()\n' % (prop['type'], prop['name']))
      for attr in sorted(Elements['attr'], key=lambda k: k['name']):
        f.write('\t&->getAttribute<%s>("%s")\n' % (attr['type'], attr['name']))
