#include "NTupleReader.h"
#include "StopleAlias.h"
#include "baselineDef.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <ctime>
#include <iomanip>
#include <cmath>


int main()
{
  char nBase[] = "/uscms_data/d3/lpcsusyhad/benwu/Moriond2019/TestNanoAOD/CMSSW_9_4_11_cand1/src/PhysicsTools/NanoSUSY/test/test94X_NANO.root";

  TChain *ch = new TChain("Events");

    size_t t0 = clock();

    char chname[512];
    for(int i = 1; i <= 1; ++i)
    {
        sprintf(chname, nBase, i);
        ch->Add(chname);
    }
    try
    {
    NTupleReader tr(ch);
    }
    catch (SATException &e)
    {
      e.print();
    }

    NTupleReader tr(ch);
    //StopleAlias alias;
    //tr.setReThrow(false);
    //tr.registerFunction(alias);
    ////std::cout << "NEVTS: " << tr.getNEntries() << std::endl;

    while(tr.getNextEvent())
    {
      //if(tr.getEvtNum() == 1)
      //{
        //tr.printTupleMembers();
        //FILE * fout = fopen("NewUCSBNTupleTypes.txt", "w");
        //tr.printTupleMembers(fout);
        //fclose(fout);
      //}
      //if(tr.getEvtNum() > 10)
        //break;

      std::cout << "MET " << tr.getVar<float>("MET_pt")  <<" met_pt " << tr.getVar<float>("MET_phi") << std::endl;
    }
    ch->Reset();
}


