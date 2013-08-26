//==============================================================================
// Class: RprFom::Nib
//==============================================================================

#include "eaagles/hla/rprFom/RprFomNib.h"

#include "eaagles/hla/rprFom/RprFomIO.h"
#include "eaagles/hla/rprFom/RprFom.h"

#include "eaagles/hla/Ambassador.h"
#include "eaagles/dis/NibDisIo.h"
#include "eaagles/simulation/AirVehicle.h"
#include "eaagles/simulation/Missile.h"
#include "eaagles/simulation/Player.h"
#include "eaagles/simulation/Simulation.h"
#include "eaagles/simulation/Signatures.h"
#include "eaagles/simulation/Weapon.h"
#include "eaagles/basic/Pair.h"
#include "eaagles/basic/PairStream.h"
#include "eaagles/basic/String.h"
#include "eaagles/basic/Nav.h"
#include "eaagles/basic/NetHandler.h"
#include "eaagles/basic/Number.h"

namespace Eaagles {
namespace Network {
namespace Hla {
namespace RprFom {

IMPLEMENT_PARTIAL_SUBCLASS(Nib,"RprFomNib")
EMPTY_SLOTTABLE(Nib)
EMPTY_SERIALIZER(Nib)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Nib::Nib(const Simulation::NetworkIO::IoType ioType) : Hla::Nib(ioType) 
{
   SET_SLOTTABLE

   baseEntity = 0;
   fireEvent = 0;
   geodPos[0] = 0;
   geodPos[1] = 0;
   geodPos[2] = 0;
   haveWorldLocationFlg = false;
   haveOrientationFlg = false;
   haveEntityTypeFlg = false;
   haveEntityIdFlg = false;

   disKind = 0;
   disDomain = 0;
   disCountry = 0;
   disCategory = 0;
   disSubcategory = 0;
   disSpecific = 0;
   disExtra = 0;

   siteID = 0;
   appID = 0;
}

Nib::Nib(const Nib& org) : Hla::Nib(org.getIoType())
{ 
    SET_SLOTTABLE
    copyData(org,true);
}

Nib::~Nib()
{
   deleteData();
}

Nib& Nib::operator=(const Nib& org)
{
    deleteData();
    copyData(org,false);
    return *this;
}

Basic::Object* Nib::clone() const
{
    return new Nib(*this);
}

//------------------------------------------------------------------------------
// copyData() --
//------------------------------------------------------------------------------
void Nib::copyData(const Nib& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) {
   baseEntity = 0;
   }

   // Base entity
   if (org.baseEntity != 0)
      setBaseEntity( (BaseEntity*) org.baseEntity->clone() );
   else
      setBaseEntity(0);

   // other stuff
   fireEvent = org.fireEvent;
   geodPos[0] = org.geodPos[0];
   geodPos[1] = org.geodPos[1];
   geodPos[2] = org.geodPos[2];
   haveWorldLocationFlg = org.haveWorldLocationFlg;
   haveOrientationFlg = org.haveOrientationFlg;
   haveEntityTypeFlg = org.haveEntityTypeFlg;
   haveEntityIdFlg = org.haveEntityIdFlg;

   disKind = org.disKind;
   disDomain = org.disDomain;
   disCountry = org.disCountry;
   disCategory = org.disCategory;
   disSubcategory = org.disSubcategory;
   disSpecific = org.disSpecific;
   disExtra = org.disExtra;

   siteID = org.siteID;
   appID = org.appID;
}

//------------------------------------------------------------------------------
// deleteData() --
//------------------------------------------------------------------------------
void Nib::deleteData()
{
    setBaseEntity(0);
    
    BaseClass::deleteData();
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------
void Nib::setBaseEntity(BaseEntity* const p)
{
   if (baseEntity != 0) baseEntity->unref();
   baseEntity = p;
   if (baseEntity != 0) baseEntity->ref();
}

void Nib::setWeaponFireEvent(const unsigned short e)
{
   fireEvent = e;
}


/// Set (DIS) Entity type enumerations
bool Nib::setEntityType(
       const unsigned char k,       // Kind
       const unsigned char d,       // Domain
       const unsigned short cc,     // Country code
       const unsigned char c,       // Category
       const unsigned char sc,      // Subcategory
       const unsigned char sp,      // Specific
       const unsigned char e        // Extra
    )
{
   // Some simple validity checks
   bool ok = (k < Dis::DisIO::NUM_ENTITY_KINDS) || (d < Dis::DisIO::MAX_ENTITY_DOMAINS);

   if (ok) {
      disKind = k;    
      disDomain = d;
      disCountry = cc;
      disCategory = c;
      disSubcategory = sc;
      disSpecific = sp;
      disExtra = e;
   }

   return ok;
}

//------------------------------------------------------------------------------
// setOutputPlayerType() -- sets the kind, country, ... variables
//------------------------------------------------------------------------------
bool Nib::setOutputPlayerType(const Simulation::Player* const p)
{
   bool ok = BaseClass::setOutputPlayerType(p);
   if (ok) {
      //RprFomIO* rprIO = (RprFomIO*)(getNetworkIO());
      if (p->isClassType(typeid(Simulation::AirVehicle))) {
         setClassIndex( RprFomIO::AIRCRAFT_CLASS );
      }
      else if (p->isClassType(typeid(Simulation::Missile))) {
         setClassIndex( RprFomIO::MISSILE_CLASS );
      }
      else {
         // Invalid type
         ok = false;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// isPlayerStateUpdateRequiredForOutput() -- check to see if an update is required
//------------------------------------------------------------------------------
bool Nib::isPlayerStateUpdateRequired(const LCreal curExecTime)
{
    // Check the update required flags first ...
    bool required = 
      isAttributeUpdateRequired(RprFomIO::WORLD_LOCATION_BE_AI) ||
      isAttributeUpdateRequired(RprFomIO::ENTITY_TYPE_BE_AI) ||
      isAttributeUpdateRequired(RprFomIO::ENTITY_IDENTIFIER_BE_AI) ||
      isAttributeUpdateRequired(RprFomIO::IS_FROZEN_BE_AI) ||
      isAttributeUpdateRequired(RprFomIO::DEAD_RECKONING_ALGORITHM_BE_AI) ||
      isAttributeUpdateRequired(RprFomIO::ORIENTATION_BE_AI) ||
      isAttributeUpdateRequired(RprFomIO::VELOCITY_VECTOR_BE_AI) ||
      isAttributeUpdateRequired(RprFomIO::ANGULAR_VELOCITY_VECTOR_BE_AI) ||
      isAttributeUpdateRequired(RprFomIO::ACCELERATION_VECTOR_BE_AI) ||
      isAttributeUpdateRequired(RprFomIO::FORCE_IDENTIFIER_PE_AI);
    
    // Check the base class next ...
    if (!required) {
        required = BaseClass::isPlayerStateUpdateRequired(curExecTime);
    }
    
    return required;
}


//------------------------------------------------------------------------------
// updateTheIPlayer() -- (Input support)
//   Called by our processInputList() to it's time to update the EAAGLES's
//   networked player.
//------------------------------------------------------------------------------
void Nib::updateTheIPlayer()
{
   Simulation::Player* p = getPlayer();

   // ---
   // If we haven't tried to created the IPlayer yet and we have at least position,
   // velocity and orientation, then try to create one
   // ---
   if (p == 0 && isEntityTypeUnchecked() && haveEntityIdFlg && haveEntityTypeFlg && haveWorldLocationFlg && haveOrientationFlg) {
      // Create the player
      p = getNetworkIO()->createIPlayer(this);
   }

   // ---
   // When we have a player, update it's data from our object's attributes
   // ---
   if (p != 0) {
      // This transfers player data from our basic NIB to the player.
      nib2PlayerState();
      // ... transfer additional data in the future ....
   }
}

} // End RprFom namespace
} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace
