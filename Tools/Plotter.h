#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TProfile.h"

#include <vector>
#include <utility>
#include <string>

#include "NTupleReader.h"

class Plotter
{
private:
    class Cut
    {
    public:
        std::string name;
        char type;
        double val, val2;
        bool inverted;

        Cut(std::string s, char t, double v, double v2 = 0);
        bool passCut(const NTupleReader& tr) const;
    };
    
    class Cuttable
    {
    public:
        Cuttable() {}
        Cuttable(std::string c);
        bool passCuts(const NTupleReader& tr) const;
        void setCuts(std::string c);
        std::string getCuts() {return cuts_;}
        
    private:
        std::string cuts_;
        std::vector<Cut> cutVec_;
        void parseCutString();
    };


public:

    class HistSummary : public Cuttable
    {
    public:
        std::vector<std::pair<std::string, TH1*>> histVec;
        std::string name, label;
        int nBins;
        double low, high;
        bool isLog;
        std::string xAxisLabel, yAxisLabel;
        
        HistSummary() {}
        HistSummary(std::string l, std::string n, std::string cuts, int nb, double ll, double ul, bool log, std::string xal = "", std::string yal = "");
        TH1* hist() {return histVec.back().second;}
    };

    class FileSummary : public Cuttable
    {
    public:
        std::string name, treePath, label;
        double xsec, lumi, kfactor, nEvts;

        FileSummary() {}
        FileSummary(std::string lab, std::string nam, std::string tree, std::string cuts, double xs, double l, double k, double n);
    };

    Plotter(std::vector<HistSummary> h, std::vector<std::vector<FileSummary>> t);
    void plot();

private:
    std::vector<HistSummary> hists_;
    std::vector<std::vector<FileSummary>> trees_;
        
    void createHistsFromTuple();
};