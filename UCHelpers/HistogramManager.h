#ifndef UCHelpers_HistogramManager_H
#define UCHelpers_HistogramManager_H

#include <TH1.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include <EventLoop/Worker.h>

class HistogramManager{

  public:
    // initializer and destructor
    HistogramManager(EL::Worker* wk);
    ~HistogramManager();

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

    std::string namePrefix = "h", namePostfix, delimiter = "_"; //!

  private:
    EL::Worker* m_wk; //!

    virtual void Sumw2(TH1* hist);
    virtual void record(TH1* hist);
    virtual void SetLabel(TH1* hist, std::string xlabel);
    virtual void SetLabel(TH1* hist, std::string xlabel, std::string ylabel);
    virtual void SetLabel(TH1* hist, std::string xlabel, std::string ylabel, std::string zlabel);
    virtual std::string GetName(std::string name);

};

#endif
