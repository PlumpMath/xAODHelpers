# xAODHelpers - A RootCore Package

## Installing
The last stable analysis base used is **2.0.23**. To install,
```bash
rcSetup Base,2.1.30
git clone https://github.com/kratsg/xAODHelpers.git
rc checkout_pkg atlasinst/Institutes/UChicago/xAODAnaHelpers/trunk
source xAODAnaHelpers/scripts/ElectronEfficiencyCorrectionPatch_Base.2.1.30.sh
rc checkout_pkg atlasoff/Reconstruction/Jet/JetSubStructureUtils/tags/JetSubStructureUtils-00-02-08
rc find_packages
rc compile
```

### Functionality Included
 - `JetHists` is a plotting class for Jets to predefine a lot of plots to make for an input JetContainer (see `execute()`). This should only be changed to add new plots, but not to add in plots that require cuts on a jet -- that should be done in an EL algorithm such as `JetKinematics`.
 - `JetKinematics` is an `EventLoop` algorithm used to automate the creation of histograms for a given jet container. See [makeJetKinematics.cxx](util/makeJetKinematics.cxx) for an example.
 - `Helpers::jet_reclustering` and `Helpers::jet_trimming` are functions to allow the user to recluster and trim jets appropriately.

![Jet Transverse Momentum for AntiKt10 reclustered jets, AntiKt10LCTopo jets, and AntiKt10 trimmed jets](/data/jet_pts.png?raw=true "Jet Pts")

![Jet Masses for AntiKt10 reclustered jets, AntiKt10LCTopo jets, and AntiKt10 trimmed jets](/data/jet_masses.png?raw=true "Jet Masses")

### Jet Reclustering

Using `xAODAnaHelpers`, we provide an easy way for you to enable jet reclustering in a job. In particular, given a `json` file

```json
[
  {
    "class": "JetReclustering",
    "configs": {
      "m_inputJetName": "AntiKt4LCTopoJets",
      "m_outputJetName": "AntiKt10LCTopoJetsRCAntiKt4LCTopoJets",
      "m_radius": 1.0,
      "m_debug": false,
      "m_outputXAODName": "output"
    }
  }
]
```

you can run the reclustering code and produce a basic output file (by setting `m_outputXAODName`) and running `xAH_run.py input.root  --config config.json`. This is a minimal working example. You can include this in an algorithm chain.

#### Authors
- [Giordon Stark](https://github.com/kratsg)

##### Acknowledgements
- [Miles Wu](https://github.com/mileswu)
