// CleanedJets.h
//
// Caleb J. Smith
// October 22, 2018

// Input vector of TLorentzVector: objects (photons, leptons, etc)
// Input vector of TLorentzVector: jets (before they are clean)
// Return vector of TLorentzVector: cleaned jets (dR cleaning)


#ifndef CLEANEDJETS_H
#define CLEANEDJETS_H

#include "SusyAnaTools/Tools/NTupleReader.h"
#include "SusyAnaTools/Tools/customize.h"
#include "Math/VectorUtil.h"

#include <vector>
#include <iostream>
#include <string>

#endif

class CleanedJets {

private:
    // AK4 jet variables
    std::vector<std::string> AK4JetVariables_;
    // AK8 jet variables
    std::vector<std::string> AK8JetVariables_;
    
    NTupleReader* tr_;
    void setReader(NTupleReader& tr) { tr_ = &tr; }

    void generateCleanedJets() 
    {
        //std::cout << "Running CleanedJets.h" << std::endl;
        // clean all variables in jet  collection
        // cleanJetCollection(std::string jetCollectionLVec, std::vector<TLorentzVector> jetCollectionVariables, std::string prefix)
        cleanJetCollection("jetsLVec",      AK4JetVariables_, "");
        cleanJetCollection("jetsLVec",      AK4JetVariables_, "prodJetsNoLep_");
        cleanJetCollection("puppiJetsLVec", AK8JetVariables_, "");
        cleanJetCollection("puppiJetsLVec", AK8JetVariables_, "prodJetsNoLep_");
    }

    template <class type> void cleanVector(std::string vectorName, std::vector<bool> keepJet)
    {
        //std::cout << "In cleanVector(): vectorName = " << vectorName << ", type = " << typeid(type).name() << std::endl;
        const auto& vec = tr_->getVec<type>(vectorName); 
        std::vector<type>* cleanedVec = new std::vector<type>();
        if (keepJet.size() != vec.size())
        {
            std::cout << "ERROR in cleanVector(): vector sizes do not match for " << vectorName << std::endl;
            std::cout << "In cleanVector(): keepJet.size() = " << keepJet.size() << " and vec.size() = " << vec.size() << std::endl;
        }
        for (int i = 0; i < vec.size(); ++i)
        {
            if (keepJet[i]) cleanedVec->push_back(vec[i]);
        }
        tr_->registerDerivedVec(vectorName+"_NoPhoton", cleanedVec);
    }

    // clean all variables in jet  collection
    void cleanJetCollection(const std::string& jetCollectionLVec, const std::vector<std::string>& jetCollectionVariables, const std::string& prefix)
    {
        const auto& gammaLVec = tr_->getVec<TLorentzVector>("gammaLVecPassLooseID");      // selected reco photon
        const auto& jetsLVec  = tr_->getVec<TLorentzVector>(prefix + jetCollectionLVec);  // jet lorentz vector
        std::vector<float>* dRvec = new std::vector<float>();
        const float dRMax = 0.15; // dR between photon and jet
        // loop over photons
        // determine which jets to keep
        std::vector<bool> keepJet(jetsLVec.size(), true);
        for (int i = 0; i < gammaLVec.size(); ++i)
        {
            //jetObjectdRMatch(const TLorentzVector& object, const std::vector<TLorentzVector>& jetsLVec, const float jetObjectdRMax)
            int match = AnaFunctions::jetObjectdRMatch(gammaLVec[i], jetsLVec, dRMax, dRvec);
            if (match >= 0) keepJet[match] = false;
        }
        
        // clean all variables in jet collection
        for (const auto& jetVariable : jetCollectionVariables)
        {
            // TLorentzVector
            if (jetVariable.compare("jetsLVec") == 0 || jetVariable.compare("puppiJetsLVec") == 0)
            {
                cleanVector<TLorentzVector>(prefix + jetVariable, keepJet);
            }
            // std::vector<TLorentzVector> 
            else if (jetVariable.compare("puppiAK8SubjetLVec") == 0)
            {
                cleanVector<std::vector<TLorentzVector>>(prefix + jetVariable, keepJet);
            }
            // int
            else if (jetVariable.compare("qgMult") == 0)
            {
                cleanVector<int>(prefix + jetVariable, keepJet);
            }
            // float
            else
            {
                cleanVector<float>(prefix + jetVariable, keepJet);
            }
        }
        // dR between jets and photon
        tr_->registerDerivedVec("dRjetsAndPhoton", dRvec);
    }

public:

    CleanedJets()
    {
        // AK4 jet variables
        AK4JetVariables_ = {
                             "jetsLVec",              
                             "recoJetsCSVv2",
                             "qgLikelihood",
                             "qgPtD",                               
                             "qgAxis1",                             
                             "qgAxis2",                             
                             "qgMult",                              
                             "recoJetschargedHadronEnergyFraction", 
                             "recoJetschargedEmEnergyFraction",     
                             "recoJetsneutralEmEnergyFraction",     
                             "recoJetsmuonEnergyFraction",          
                             "recoJetsHFHadronEnergyFraction",     
                             "recoJetsHFEMEnergyFraction",          
                             "recoJetsneutralEnergyFraction",       
                             "PhotonEnergyFraction",                
                             "ElectronEnergyFraction",             
                             "ChargedHadronMultiplicity",          
                             "NeutralHadronMultiplicity",          
                             "PhotonMultiplicity",                
                             "ElectronMultiplicity",               
                             "MuonMultiplicity",                    
                             "DeepCSVb",                            
                             "DeepCSVc",                            
                             "DeepCSVl",                            
                             "DeepCSVbb",                           
                             "DeepCSVcc"
                           };
        
        // AK8 jet variables
        AK8JetVariables_ = {
                             "puppiJetsLVec",              
                             "puppiAK8SubjetLVec",              
                             "puppisoftDropMass",              
                             "puppitau1",
                             "puppitau2",
                             "puppitau3",
                             "deepAK8btop",              
                             "deepAK8bW"
                           };
    }

    ~CleanedJets(){}

    void operator()(NTupleReader& tr)
    {
        // first set NTupleReader
        setReader(tr);
        generateCleanedJets();
    }
};

