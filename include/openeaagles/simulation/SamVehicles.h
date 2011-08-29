//------------------------------------------------------------------------------
// Classes: SamVehicle
//------------------------------------------------------------------------------
#if !defined(__SAMVEHICLE_H_5F30476B_4409_4d7e_915E_CA8585DEC6A9__)
#define __SAMVEHICLE_H_5F30476B_4409_4d7e_915E_CA8585DEC6A9__

#include "openeaagles/simulation/GroundVehicle.h"

namespace Eaagles {
   namespace Basic {
      class Distance;
      class Identifier;
      class Time;
   }
namespace Simulation {

//==============================================================================
// Class SamVehicle
// Description: Sam vehichle (TEL, TELAR, etc)
//
// Form name: SamVehicle
// Slots:
//    minLaunchRange    <Basic::Distance>   ! Min launch range (Basic::Distance)
//                                          ! (default: DEFAULT_MIN_LAUNCH_RANGE)
//    maxLaunchRange    <Basic::Distance>   ! Max launch range (Basic::Distance)
//                                          ! (default: DEFAULT_MAX_LAUNCH_RANGE)
//
// GUID: {5F30476B-4409-4d7e-915E-CA8585DEC6A9}
//==============================================================================
class SamVehicle : public GroundVehicle  
{
   DECLARE_SUBCLASS(SamVehicle,GroundVehicle)

public:
   SamVehicle();

   // Access functions
   virtual unsigned int getNumberOfMissiles() const;  // Number of missiles 

   virtual LCreal getMaxLaunchRange() const;          // Max missile launch range (meters)
   virtual LCreal getMinLaunchRange() const;          // Min missile launch range (meters)

   virtual bool isLauncherReady() const;              // Return true if launcher is ready to fire.

   // Set functions
   virtual bool setMaxLaunchRange(const LCreal rng);
   virtual bool setMinLaunchRange(const LCreal rng);

   // Slot functions
   virtual bool setSlotMinLaunchRange(const Basic::Distance* const msg);
   virtual bool setSlotMaxLaunchRange(const Basic::Distance* const msg);

   // Component Interface
   virtual void updateData(const LCreal dt = 0.0f);

private:
   LCreal            maxMslRng;        // Max Launch Range        (meters)
   LCreal            minMslRng;        // Min Launch Range        (meters)
   unsigned int      numMsl;           // Number of missile available
};

} // End Simulation namespace
} // End Eaagles namespace

#endif // !defined(__SAMVEHICLE_H_5F30476B_4409_4d7e_915E_CA8585DEC6A9__)
