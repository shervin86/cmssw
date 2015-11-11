//--------------------------------------------------------------------------------------------------
// $Id $
//
// SCEnergyCorrectorSemiParm
//
// Helper Class for applying regression-based energy corrections with optimized BDT implementation
//
// Authors: J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef EGAMMATOOLS_SCEnergyCorrectorSemiParm_H
#define EGAMMATOOLS_SCEnergyCorrectorSemiParm_H
    
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "CondFormats/EgammaObjects/interface/GBRForestD.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h" 
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"

class SCEnergyCorrectorSemiParm {
  public:
    SCEnergyCorrectorSemiParm();
    ~SCEnergyCorrectorSemiParm(); 
        
    void setTokens(edm::ConsumesCollector &cc, const edm::ParameterSet& iConfig);
    
	std::pair<double,double> GetCorrections(const reco::SuperCluster &sc) const;
    void modifyObject(reco::SuperCluster &sc);

    void setEventSetup(const edm::EventSetup &es);
    void setEvent(const edm::Event &e);
    
  protected:    
    GBRForestD *foresteb_;
    GBRForestD *forestee_;
    GBRForestD *forestsigmaeb_;
    GBRForestD *forestsigmaee_;
    
    edm::ESHandle<CaloTopology> calotopo_;
    edm::ESHandle<CaloGeometry> calogeom_;
    const CaloTopologyRecord* topo_record_;
    const CaloGeometryRecord* geom_record_;    
    
    edm::EDGetTokenT<EcalRecHitCollection> tokenEBRecHits_;
    edm::EDGetTokenT<EcalRecHitCollection> tokenEERecHits_;
    edm::EDGetTokenT<reco::VertexCollection> tokenVertices_;    
    edm::Handle<reco::VertexCollection> vertices_;
    edm::Handle<EcalRecHitCollection>  rechitsEB_;
    edm::Handle<EcalRecHitCollection>  rechitsEE_;
    
    
    };


#endif
