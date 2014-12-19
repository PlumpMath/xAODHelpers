#ifndef xAODHelpers_HistogramManager_H
#define xAODHelpers_HistogramManager_H

#include <TH1.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include <EventLoop/Worker.h>
#include <EventLoop/StatusCode.h>
#include <xAODRootAccess/TEvent.h>

class HistogramManager {

  public:
    // initializer and destructor
    HistogramManager();
    virtual ~HistogramManager();

    // this is used by any class extending to pre-define a set of histograms
    //      to book by default
    //  the following should be defined in other classes
    virtual EL::StatusCode initialize(bool sumw2 = true){ return EL::StatusCode::SUCCESS; };
    virtual EL::StatusCode execute(){ return EL::StatusCode::SUCCESS; };
    virtual EL::StatusCode finalize(){ return EL::StatusCode::SUCCESS; };

    // @assign assigns the worker that manages the algorithms, usually just pass in wk()
    void assign(EL::Worker* wk);

    // @book - record a histogram in wk()->addOutput() and call various functions
    //      ** This is an overloaded function. It will build the right histogram
    //          given the correct number of input arguments.**
    //  @book.name - name of histogram, access it in ROOT file like "h_jetPt->Draw()"
    //  @book.title - usually pointless,put a description of the histogram in here
    //  @book.xlabel - label to put on the x-axis
    //  @book.xbins - number of xbins to use
    //  @book.xlow - lower bound on xbins
    //  @book.xhigh - upper bound on xbins
    //  @book.ylabel - label to put on the y-axis
    //  @book.ylow - lower bound on ybins
    //  @book.yhigh - upper bound on ybins
    //  @book.zlabel - label to put on the z-axix
    //  @book.zlow - lower bound on zbins
    //  @book.zhigh - upper bound on zbins
    //  @book.sumw2 - enable sumw2() for this histogram, default=True
    TH1F* book(std::string name, std::string title,
               std::string xlabel, int xbins, double xlow, double xhigh,
               bool sumw2 = true);

    TH2F* book(std::string name, std::string title,
               std::string xlabel, int xbins, double xlow, double xhigh,
               std::string xyabel, int ybins, double ylow, double yhigh,
               bool sumw2 = true);

    TH3F* book(std::string name, std::string title,
               std::string xlabel, int xbins, double xlow, double xhigh,
               std::string ylabel, int ybins, double ylow, double yhigh,
               std::string zlabel, int zbins, double zlow, double zhigh,
               bool sumw2 = true);

    // @namePrefix is what is prefixed to each histogram name
    // @namePostfix is what is postfixed to each histogram name
    // @delimiter is what to separate the prefix and postfix by when (pre|ap)pending
    std::string m_namePrefix = "h", m_namePostfix, m_delimiter = "_"; //!
    std::string m_containerName = ""; //!
    EL::Worker* m_wk; //!

  private:
    // Turn on Sumw2 for the histogram
    void Sumw2(TH1* hist);
    // Record the histogram on the worker
    void record(TH1* hist);
    // Set the xlabel
    void SetLabel(TH1* hist, std::string xlabel);
    // Set the xlabel, ylabel
    void SetLabel(TH1* hist, std::string xlabel, std::string ylabel);
    // Set the xlabel, ylabel, and zlabel
    void SetLabel(TH1* hist, std::string xlabel, std::string ylabel, std::string zlabel);
    // Creates the histogram name using @namePrefix, @namePostfix, and @delimiter above
    std::string GetName(std::string name);

};

#endif
