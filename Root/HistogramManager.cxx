#include "UCHelpers/HistogramManager.h"

HistogramManager::HistogramManager(EL::Worker* wk) {
  m_wk = wk;
}

HistogramManager::~HistogramManager() {}

TH1F* HistogramManager::book(std::string name, std::string title,
                             std::string xlabel, int xbins, double xlow, double xhigh,
                             bool sumw2)
{
  TH1F* tmp = new TH1F( GetName(name).c_str(), title.c_str(), xbins, xlow, xhigh);
  SetLabel(tmp, xlabel);
  if(sumw2) this->Sumw2(tmp);
  this->record(tmp);
  return tmp;
};

TH2F* HistogramManager::book(std::string name, std::string title,
                             std::string xlabel, int xbins, double xlow, double xhigh,
                             std::string ylabel, int ybins, double ylow, double yhigh,
                             bool sumw2)
{
  TH2F* tmp = new TH2F( GetName(name).c_str(), title.c_str(), xbins, xlow, xhigh, ybins, ylow, yhigh);
  SetLabel(tmp, xlabel, ylabel);
  if(sumw2) this->Sumw2(tmp);
  this->record(tmp);
  return tmp;
};

TH3F* HistogramManager::book(std::string name, std::string title,
                             std::string xlabel, int xbins, double xlow, double xhigh,
                             std::string ylabel, int ybins, double ylow, double yhigh,
                             std::string zlabel, int zbins, double zlow, double zhigh,
                             bool sumw2)
{
  TH3F* tmp = new TH3F( GetName(name).c_str(), title.c_str(), xbins, xlow, xhigh, ybins, ylow, yhigh, zbins, zlow, zhigh);
  SetLabel(tmp, xlabel, ylabel, zlabel);
  if(sumw2) this->Sumw2(tmp);
  this->record(tmp);
  return tmp;
};

void HistogramManager::Sumw2(TH1* hist) {
  hist->Sumw2();
};

void HistogramManager::record(TH1* hist) {
  m_wk->addOutput(hist);
};

void HistogramManager::SetLabel(TH1* hist, std::string xlabel)
{
  hist->GetXaxis()->SetTitle(xlabel.c_str());
};

void HistogramManager::SetLabel(TH1* hist, std::string xlabel, std::string ylabel)
{
  hist->GetYaxis()->SetTitle(ylabel.c_str());
  this->SetLabel(hist, xlabel);
};

void HistogramManager::SetLabel(TH1* hist, std::string xlabel, std::string ylabel, std::string zlabel)
{
  hist->GetZaxis()->SetTitle(zlabel.c_str());
  this->SetLabel(hist, xlabel, ylabel);
};

std::string HistogramManager::GetName(std::string name)
{
  return this->namePrefix+this->delimiter+name+this->delimiter+this->namePostfix;
};
