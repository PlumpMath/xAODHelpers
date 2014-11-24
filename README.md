# UCHelpers - A RootCore Package

## Installing
The last stable analysis base used is **2.0.18**. To install,
```bash
mkdir myRootCore && cd myRootCore
rcSetup Base,2.0.18
git clone git@github.com:kratsg/UCHelpers.git
rc find_packages
rc compile_pkg UCHelpers
```

## Dependencies
 - dependencies are in [cmt/Makefile.RootCore](cmt/Makefile.RootCore)
 - uses Miles [JetSubstructureTools](https://github.com/mileswu/JetSubstructureTools) which is included in AnalysisBase 2.0.18+ _except for SubjetFinder_.

### Functionality Included
 - `HistogramManager` helps manage histograms in `EventLoop` algorithms by initializing histograms and adding it to the worker.
 - `JetKinematics` is an `EventLoop` algorithm used to automate the creation of histograms for a given jet container. See [makeJetKinematics.cxx](util/makeJetKinematics.cxx) for an example.
 - `dumpSG.py` is a python script (a ROOT macro of sorts) used to inspect information about a ROOT file. It should be self-documented via `dumpSG.py --help`.

### [dumpSG.py](scripts/dumpSG.py)
```
$ dumpSG.py --help
usage: dumpSG.py [-h] [--tree TREE_NAME] [-o OUTPUT_FILENAME]
                 [-t CONTAINER_TYPE_REGEX] [-c CONTAINER_NAME_REGEX]
                 [-f {json,pickle,pretty}] [--prop] [--attr]
                 [--filterProps PROPERTY_NAME_REGEX]
                 [--filterAttrs ATTRIBUTE_NAME_REGEX] [-i]
                 input_filename

Process xAOD File and Dump Information.

positional arguments:
  input_filename        an input root file to read

optional arguments:
  -h, --help            show this help message and exit
  --tree TREE_NAME      Specify the tree that contains the StoreGate
                        structure.
  -o OUTPUT_FILENAME, --output OUTPUT_FILENAME
                        Output file to store dumped information.
  -t CONTAINER_TYPE_REGEX, --type CONTAINER_TYPE_REGEX
                        Regex specification for the xAOD container type. The
                        `xAOD::` is automatically preprended. For example,
                        --type="Jet*" will match `xAOD::JetContainer` while
                        --type="*Jet*" will match `xAOD::TauJetContainer`.
                        This uses Unix filename matching.
  -c CONTAINER_NAME_REGEX, --container CONTAINER_NAME_REGEX
                        Regex specification for the xAOD container name. For
                        example, --container="AntiKt10LCTopo*" will match
                        `AntiKt10LCTopoJets`. This uses Unix filename
                        matching.
  -f {json,pickle,pretty}, --format {json,pickle,pretty}
                        Specify the output format.
  --prop                Enable to print properties of container. By default,
                        it only prints the xAOD::ContainerType and containers
                        for that given type. This is like an increased
                        verbosity option for container properties.
  --attr                Enable to print attributes of container. By default,
                        it only prints the xAOD::ContainerType and containers
                        for that given type. This is like an increased
                        verbosity option for container attributes.
  --filterProps PROPERTY_NAME_REGEX
                        (INACTIVE) Regex specification for xAOD property
                        names. Only used if --prop enabled.
  --filterAttrs ATTRIBUTE_NAME_REGEX
                        (INACTIVE) Regex specification for xAOD attribute
                        names. Only used if --attr enabled.
  -i, --interactive     (INACTIVE) Flip on/off interactive mode allowing you
                        to navigate through the container types and
                        properties.
```

#### Authors
- [Giordon Stark](https://github.com/kratsg)

##### Acknowledgements
This is inspired by Gabriel Facini's [package](https://svnweb.cern.ch/trac/atlasperf/browser/CombPerf/Tracking/TrackingInDenseEnvironments/SimpleAnaxAOD/trunk/HistManager/).
