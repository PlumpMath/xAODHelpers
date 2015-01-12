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

  protected:
    // generically the main name assigned to all histograms
    std::string m_name;
    // a detail level, should change to a string soon
    int m_detailLevel;
    // a container holding all generated histograms
    //  - loop over this to record to EL output
    std::vector< TH1* > m_allHists; //!

  public:
    // initializer and destructor
    HistogramManager(std::string name, int detailLevel);
    virtual ~HistogramManager();

    // this is used by any class extending to pre-define a set of histograms
    //      to book by default
    //  the following should be defined in other classes
    virtual EL::StatusCode initialize(){ return EL::StatusCode::SUCCESS; };
    virtual EL::StatusCode execute(){ return EL::StatusCode::SUCCESS; };
    virtual EL::StatusCode finalize(){ return EL::StatusCode::SUCCESS; };

    // @book - record a histogram and call various functions
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

    // @delimiter is what to separate the m_name and name when concatenating
    std::string m_delimiter = "_"; //!

    // Record all histograms from m_allHists to the worker
    void record(EL::Worker* wk);

  private:
    // Turn on Sumw2 for the histogram
    void Sumw2(TH1* hist);

    // Push the new histogram to m_allHists
    void record(TH1* hist);

    // Set the xlabel
    void SetLabel(TH1* hist, std::string xlabel);
    // Set the xlabel, ylabel
    void SetLabel(TH1* hist, std::string xlabel, std::string ylabel);
    // Set the xlabel, ylabel, and zlabel
    void SetLabel(TH1* hist, std::string xlabel, std::string ylabel, std::string zlabel);

    // string concatenation helper
    std::string concat(std::string left, std::string right);

};

#endif
