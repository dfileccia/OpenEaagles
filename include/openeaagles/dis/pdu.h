//--------------------------------------------------------------
// IEEE Standard for Distributed Interactive Simulation (DIS)
// Application Protocols - PDU Structures for DIS 
//--------------------------------------------------------------
#ifndef _1278_1a_PDU_H_
#define _1278_1a_PDU_H_

#include "openeaagles/dis/structs.h"
#include "openeaagles/dis/enums.h"

#include <string>

namespace Eaagles {
namespace Network {
namespace Dis {

class EntityStatePDU;
class CollisionPDU;
class FirePDU;
class DetonationPDU;
class IffAtcNavaidsPDU;
class ElectromagneticEmissionPDU;

//-----------------------------------
// EntityStatePDU

class EntityStatePDU {
public:

   void swapBytes(){
      // Swap our stuff first
      header.swapBytes();
      entityID.swapBytes();
      entityType.swapBytes();
      alternativeType.swapBytes();
      entityLinearVelocity.swapBytes();
      entityLocation.swapBytes();
      entityOrientation.swapBytes();
      DRentityLinearAcceleration.swapBytes();
      DRentityAngularVelocity.swapBytes();
      appearance = convertUInt(appearance);
      capabilites = convertUInt(capabilites);

      // then swap the articulation parameters
      for(int i = 0; i < numberOfArticulationParameters; i++) {
         ArticulationParameter* ap = getArticulationParameter(i);
         if (ap != 0) ap->swapBytes();
      }
   };

   PDUHeader            header;
   entityIdentifierDIS  entityID;
   unsigned char        forceID;
   unsigned char        numberOfArticulationParameters;
   EntityType           entityType;
   EntityType           alternativeType;
   vectorDIS            entityLinearVelocity;
   WorldCoordinates     entityLocation;
   EulerAngles          entityOrientation;
   unsigned int         appearance;
   unsigned char        deadReckoningAlgorithm;
   unsigned char        otherParameters[15];
   vectorDIS            DRentityLinearAcceleration;
   AngularVelocityvectorDIS DRentityAngularVelocity;
   EntityMarking        entityMarking;
   unsigned int         capabilites;

   // Returns a pointer to the idx'th ArticulationParameter structure;
   // Note: This ONLY works after the PDU have been created and initialized!
   //   (The "numberOfArticulationParameters' member variable needs to be correct
   //    for this to work)
   // Note: 'idx' is zero based, so an idx == 1 will return the second ArticulationParameter structure
   ArticulationParameter* getArticulationParameter(const int idx)
   {
      ArticulationParameter* ap = 0;
      if (idx < numberOfArticulationParameters) {
         unsigned char *p = ((unsigned char *)this) + sizeof(*this);

         // First Emission system data is just after this structure
         ap = (ArticulationParameter*) p;

         for (int i = 0; i < idx; i++) {
            // step down to the correct emission system data using 'systemDataLength'
            p += sizeof(ArticulationParameter);
            ap = (ArticulationParameter*) p;
         }

      }
      return ap;
   }
   const ArticulationParameter* getArticulationParameter(const int idx) const
   {
      const ArticulationParameter* ap = 0;
      if (idx < numberOfArticulationParameters) {

         // First articulation parameter is just after this structure
         const unsigned char *p = ((unsigned char *)this) + sizeof(*this);
         ap = (const ArticulationParameter*) p;

         for (int i = 0; i < idx; i++) {
            // step down to the correct ArticulationParameter structure 
            p += sizeof(ArticulationParameter);
            ap = (const ArticulationParameter*) p;
         }

      }
      return ap;
   }

  void dumpData() const {
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Entity State PDU(" << (long)header.PDUType << ")" << std::endl;
    std::cout << "Time Stamp (" << (long)header.timeStamp << ")" << std::endl;
    std::cout << entityID << std::endl;
    std::cout << "ForceID (" << (long)forceID << ")" << std::endl;
    std::cout << "Num Articulation Params (" << (long)numberOfArticulationParameters << ")" << std::endl;
    std::cout << "Entity Type:" << std::endl;
    std::cout << entityType << std::endl;
    std::cout << "Entity Alternate Type:" << std::endl;
    std::cout << alternativeType << std::endl;
    std::cout << "Entity Linear Velocity:" << std::endl;
    std::cout << entityLinearVelocity;
    std::cout << "Entity Location:" << std::endl;
    WorldCoordinates loc;
    memcpy(&loc,&entityLocation,sizeof(WorldCoordinates));
    std::cout << loc;
    std::cout << "Entity Orientation:" << std::endl;
    std::cout << entityOrientation;
    std::cout << "  Appearance (" << std::hex << appearance << ")" << std::endl;
    std::cout << std::dec;
    std::cout << "  DR Algor   (" << (long)deadReckoningAlgorithm << ")" << std::endl;
    std::cout << "  DR Params  ( ";
    std::cout << std::hex;
    for(unsigned int i = 0; i < 15; i++){
      std::cout.width(2);
      std::cout.fill('0');
      std::cout << (long)otherParameters[i] << " ";
    }
    std::cout.width(0);
    std::cout << std::dec << ")" << std::endl;;
    std::cout << "DR Entity Linear Acceleration:" << std::endl;
    std::cout << DRentityLinearAcceleration;
    std::cout << "DR Angular Velocity:" << std::endl;
    std::cout << DRentityAngularVelocity;
    std::cout << "Entity Markings:" << std::endl;
    std::cout << entityMarking;
    std::cout << "Capabilities (" << capabilites << ")" << std::endl;
    for (unsigned int i = 0; i < numberOfArticulationParameters; i++) {
      const ArticulationParameter* ap = getArticulationParameter(i);
      std::cout << "Articulation Parameter(" << i << ") = " << *ap << std::endl;
    }
    std::cout.flush();
  };
};

//-----------------------------------
// CollisionPDU

class CollisionPDU{
public:
  void swapBytes(){
    header.swapBytes();
    issuingEntityID.swapBytes();
    collidingEntityID.swapBytes();
    eventID.swapBytes();
    velocity.swapBytes();
    mass = convertFloat(mass);
    location.swapBytes();
  };

  PDUHeader              header;
  entityIdentifierDIS       issuingEntityID;
  entityIdentifierDIS       collidingEntityID;
  EventIdentifier	 eventID;
  unsigned char		 collisionType;
  unsigned char		 padding;
  vectorDIS                 velocity;
  float			 mass;
  vectorDIS		 location;

  void dumpData() const {
  };
};

/////////////////////////////////////
// Warfare Protocol Family

//-----------------------------------
// Fire PDU

class FirePDU{
public:
  void swapBytes(){
    header.swapBytes();
    firingEntityID.swapBytes();
    targetEntityID.swapBytes();
    munitionID.swapBytes();
    eventID.swapBytes();
    fireMissionIndex = convertUInt(fireMissionIndex);
    location.swapBytes();
    burst.swapBytes();
    velocity.swapBytes();
    range = convertFloat(range);
  };

  PDUHeader              header;
  entityIdentifierDIS       firingEntityID;
  entityIdentifierDIS       targetEntityID;
  entityIdentifierDIS       munitionID;
  EventIdentifier        eventID;
  unsigned int		 fireMissionIndex;
  WorldCoordinates       location;
  BurstDescriptor	 burst;
  vectorDIS		 velocity;
  float			 range;

  void dumpData() const {
    WorldCoordinates loc;
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Fire PDU(" << (long)header.PDUType << ")" << std::endl;
    std::cout << "Firing   Entity:" << std::endl << firingEntityID;
    std::cout << "Target   Entity:" << std::endl << targetEntityID;
    std::cout << "Munition Entity:" << std::endl << munitionID;
    std::cout << "Event ID:" << std::endl << eventID;
    std::cout << "fireMissionIndex (" << fireMissionIndex << ")" << std::endl;
    memcpy(&loc,&location,sizeof(WorldCoordinates));
    std::cout << "Location:" << std::endl << loc;
    std::cout << "Burst Descriptor:" << std::endl << burst;
    std::cout << "Velocity vectorDISy:" << std::endl << velocity;
    std::cout << "Range:" << std::endl << range << std::endl;
    std::cout.flush();
  };
};

//-----------------------------------
// Detonation PDU

class DetonationPDU{
public:
  void swapBytes(){
    header.swapBytes();
    firingEntityID.swapBytes();
    targetEntityID.swapBytes();
    munitionID.swapBytes();
    eventID.swapBytes();
    velocity.swapBytes();
    location.swapBytes();
    burst.swapBytes();
    locationInEntityCoordinates.swapBytes();
    for(int i=0;i<numberOfArticulationParameters;i++)
      articulationPart[i].swapBytes();
  };

  PDUHeader              header;
  entityIdentifierDIS       firingEntityID;
  entityIdentifierDIS       targetEntityID;
  entityIdentifierDIS       munitionID;
  EventIdentifier        eventID;
  vectorDIS		 velocity;
  WorldCoordinates       location;
  BurstDescriptor        burst;
  vectorDIS                 locationInEntityCoordinates;
  unsigned char		 detonationResult;
  unsigned char		 numberOfArticulationParameters;
  unsigned short	 padding;
  ArticulationParameter *articulationPart;

  void dumpData() const {
    WorldCoordinates loc;
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Detonation PDU(" << (long)header.PDUType << ")" << std::endl;
    std::cout << "Firing   Entity:" << std::endl << firingEntityID;
    std::cout << "Target   Entity:" << std::endl << targetEntityID;
    std::cout << "Munition Entity:" << std::endl << munitionID;
    std::cout << "Event ID:" << std::endl << eventID;
    std::cout << "Velocity vectorDISy:" << std::endl << velocity;
    memcpy(&loc,&location,sizeof(WorldCoordinates));
    std::cout << "Location:" << std::endl << loc;
    std::cout << "Burst Descriptor:" << std::endl << burst;
    std::cout << "Location In Entity Coords:" << std::endl << locationInEntityCoordinates;
    std::cout << "Detonation Result (" << (long)detonationResult << ")" << std::endl;
    std::cout << "Num Of Atriculation Params (" << (long)numberOfArticulationParameters << ")" << std::endl;
    std::cout.flush();
  };
};


class IffAtcNavaidsPDU
{
public:
  void swapBytes(){
    header.swapBytes();
    emittingEntityID.swapBytes();
    eventID.swapBytes();
    location.swapBytes();
    systemID.swapBytes();
    operationalData.swapBytes();
  };

  PDUHeader              header;
  entityIdentifierDIS    emittingEntityID;
  EventIdentifier        eventID;
  vectorDIS              location;
  SystemID               systemID;
  unsigned short         padding;
  FundamentalOpData      operationalData;

  void dumpData() const {
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "IFF/ATC/NAVAIDS PDU(" << (long)header.PDUType << ")" << std::endl;
    std::cout << "Header:" << std::endl << header;
    std::cout << "Emitting Entity:" << std::endl << emittingEntityID;
    std::cout << "Event ID:" << std::endl << eventID;
    std::cout << "Emission Location:" << std::endl << location;
    std::cout << "SystemID:" << std::endl << systemID;
    std::cout << "Operation Data:" << std::endl << operationalData;
    std::cout.flush();
  };
};

//-----------------------------------
// Stop PDU

class StopPDU{
public:
  void dumpData() const {
  };

  void swapBytes(){
  };
};

//-----------------------------------
// Start PDU

class StartPDU{
public:
  void dumpData() const {
  };

  void swapBytes(){
  };
};

//-----------------------------------
// Signal PDU

class SignalPDU{
public:
  void swapBytes(){
    header.swapBytes();
    emittingEntityID.swapBytes();
    radioID        = (unsigned short) convertShort((short)radioID);
    encodingScheme = (unsigned short) convertShort((short)encodingScheme);
    TDLType        = (unsigned short) convertShort((short)TDLType);
    sampleRate     = (unsigned int) convertLong((long)sampleRate);
    dataLength     = (unsigned short) convertShort((short)dataLength);
    samples        = (unsigned short) convertShort((short)samples);
  };

  PDUHeader              header;
  entityIdentifierDIS       emittingEntityID;
  unsigned short	 radioID;
  unsigned short	 encodingScheme;
  unsigned short	 TDLType;
  unsigned int		 sampleRate;
  unsigned short	 dataLength;
  unsigned short	 samples;

  char* getData() {
    unsigned char *p = (unsigned char *)this;
    int pcount = sizeof(PDUHeader) + sizeof(emittingEntityID) + 14; 
    return (char *)&p[pcount];
  };

  const char* getData() const {
      const unsigned char *p = (const unsigned char *)this;
      int pcount = sizeof(PDUHeader) + sizeof(emittingEntityID) + 14; 
      return (const char *)&p[pcount];
  };

  void dumpData() const {
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Signal PDU(" << (long)header.PDUType << ")" << std::endl;
    std::cout << "Emitting Entity:" << std::endl << emittingEntityID;
    std::cout << "Radio ID: ( " << radioID << " )" << std::endl;
    std::cout << "encodingScheme: ( " << std::hex << encodingScheme << std::dec << " )" << std::endl;
    std::cout << "TDLType: ( " << TDLType << " )" << std::endl;
    std::cout << "sampleRate: ( " << sampleRate << " )" << std::endl;
    std::cout << "dataLength: ( " << dataLength << " )" << std::endl;
    std::cout << "samples: ( " << samples << " )" << std::endl;
    std::cout << "Data:" << std::endl;

    int count;

    count = 0;

    const char* p = getData();

    std::cout << std::hex;

    for(int i=0; i<dataLength; i++){

      std::cout.width(2);
      std::cout.fill('0');
      std::cout << (long)p[i] << " ";

      if (count >= 16){
        std::cout << std::endl;   
        count = 0;
      }else{
        count++;
      }
    }

    std::cout << std::dec << std::endl;
    
    std::cout.flush();
  };
};


//-----------------------------------
// Data Query PDU

class DataQueryPDU{
public:
  void swapBytes(){
    header.swapBytes();
    emittingEntityID.swapBytes();
    destinationEntityID.swapBytes();
    requestID           = (unsigned int) convertLong(requestID);
    timeInterval        = (unsigned int) convertLong(timeInterval);
    numFixedRecords     = (unsigned int) convertLong(numFixedRecords);
    numVariableRecords  = (unsigned int) convertLong(numVariableRecords);
  };

  PDUHeader              header;
  entityIdentifierDIS    emittingEntityID;
  entityIdentifierDIS    destinationEntityID;
  unsigned int	         requestID;
  unsigned int	         timeInterval;
  unsigned int	         numFixedRecords;
  unsigned int		     numVariableRecords;
 

  char* getData() {
    unsigned char *p = (unsigned char *)this;
    int pcount = sizeof(PDUHeader)
           + sizeof(emittingEntityID)
           + sizeof(destinationEntityID)
           + 16; 
    return (char *)&p[pcount];
  };

  const char* getData() const {
    const unsigned char *p = (const unsigned char *)this;
    int pcount = sizeof(PDUHeader)
         + sizeof(emittingEntityID)
         + sizeof(destinationEntityID)
         + 16; 
    return (const char *)&p[pcount];
};

  void dumpData() const {
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Data Query PDU(" << (long)header.PDUType << ")" << std::endl;
    std::cout << "Emitting Entity:" << std::endl << emittingEntityID;
    std::cout << "Destination Entity:" << std::endl << destinationEntityID;
    std::cout << "Request ID: " << std::endl << requestID;
    std::cout << "Time Interval: " << std::endl << timeInterval;
    std::cout << "Number of fixed records: " << std::endl << numFixedRecords;
    std::cout << "Number of variable records: " << std::endl << numVariableRecords;
    };
};


//-----------------------------------
// Data PDU
class DataPDU{
public:
  void swapBytes(){
    header.swapBytes();
    emittingEntityID.swapBytes();
    destinationEntityID.swapBytes();
    requestID           = (unsigned int) convertLong(requestID);
    numFixedRecords     = (unsigned int) convertLong(numFixedRecords);
    numVariableRecords  = (unsigned int) convertLong(numVariableRecords);
  };

  PDUHeader              header;
  entityIdentifierDIS    emittingEntityID;
  entityIdentifierDIS    destinationEntityID;
  unsigned int	         requestID;
  unsigned int	         padding;
  unsigned int	         numFixedRecords;
  unsigned int		     numVariableRecords;
 
  char* getData() {
    unsigned char *p = (unsigned char *)this;
    int pcount = sizeof(PDUHeader)
           + sizeof(emittingEntityID)
           + sizeof(destinationEntityID)
           + 16; 
    return (char *)&p[pcount];
  };
  const char* getData() const {
    const unsigned char *p = (const unsigned char *)this;
    int pcount = sizeof(PDUHeader)
           + sizeof(emittingEntityID)
           + sizeof(destinationEntityID)
           + 16; 
    return (const char *)&p[pcount];
  };
  
  void dumpData() const {
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Data Query PDU(" << (long)header.PDUType << ")" << std::endl;
    std::cout << "Emitting Entity:" << std::endl << emittingEntityID;
    std::cout << "Destination Entity:" << std::endl << destinationEntityID;
    std::cout << "Request ID: " << std::endl << requestID;
    std::cout << "Padding: " << std::endl << padding;
    std::cout << "Number of fixed records: " << std::endl << numFixedRecords;
    std::cout << "Number of variable records: " << std::endl << numVariableRecords;
    };
};


//-----------------------------------
// Comment PDU
class CommentPDU{
public:
  void swapBytes(){
    header.swapBytes();
    emittingEntityID.swapBytes();
    destinationEntityID.swapBytes();
    numFixedRecords     = (unsigned int) convertLong(numFixedRecords);
    numVariableRecords  = (unsigned int) convertLong(numVariableRecords);
  };

  PDUHeader              header;
  entityIdentifierDIS    emittingEntityID;
  entityIdentifierDIS    destinationEntityID;
  unsigned int	         numFixedRecords;
  unsigned int		     numVariableRecords;
 

  char* getData() {
    unsigned char *p = (unsigned char *)this;
    int pcount = sizeof(PDUHeader)
           + sizeof(emittingEntityID)
           + sizeof(destinationEntityID)
           + 8; 
    return (char *)&p[pcount];
  };
  const char* getData() const {
    const unsigned char *p = (const unsigned char *)this;
    int pcount = sizeof(PDUHeader)
           + sizeof(emittingEntityID)
           + sizeof(destinationEntityID)
           + 8; 
    return (const char *)&p[pcount];
  };
  
  void dumpData() const {
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Data Query PDU(" << (long)header.PDUType << ")" << std::endl;
    std::cout << "Emitting Entity:" << std::endl << emittingEntityID;
    std::cout << "Destination Entity:" << std::endl << destinationEntityID;
    std::cout << "Number of fixed records: " << std::endl << numFixedRecords;
    std::cout << "Number of variable records: " << std::endl << numVariableRecords;
    };
};


//-----------------------------------
// Electromagnetic Emission PDU
//-----------------------------------
class ElectromagneticEmissionPDU {
public:

   // (IST-CF-03-01, May 5, 2003)
   enum { STATE_UPDATE = 0, CHANGED_DATA_UPDATE = 1 };

   PDUHeader            header;                 // PDU Header
   entityIdentifierDIS  emittingEntityID;       // Entity that owns these systems
   EventIdentifier      eventID;                // Event ID
   unsigned char        stateUpdateIndicator;   // State
   unsigned char        numberOfSystems;        // Number of systems (EmissionSystem) that follow
   unsigned short       padding;

   // Returns a pointer to the idx'th EmissionSystem structure;
   // Note: This ONLY works after the PDU and emissions systems have been created and initialized!
   //   (The systemDataLength's in the EmissionSystem structures, as well as our
   //    'numberOfSystems' member variable, need to be correct for this to work)
   // Note: 'idx' is zero based, so a idx == 1 will return the second EmissionSystem structure
   EmissionSystem* getEmissionSystem(const int idx)
   {
      EmissionSystem* es = 0;
      if (idx < numberOfSystems) {
         unsigned char *p = ((unsigned char *)this) + sizeof(*this);

         // First Emission system data is just after this structure
         es = (EmissionSystem*) p;

         for (int i = 0; i < idx; i++) {
            // step down to the correct emission system data using 'systemDataLength'
            p += (es->systemDataLength * 4);
            es = (EmissionSystem*) p;
         }

      }
      return es;
   }
   const EmissionSystem* getEmissionSystem(const int idx) const
   {
      const EmissionSystem* es = 0;
      if (idx < numberOfSystems) {
         unsigned char *p = ((unsigned char *)this) + sizeof(*this);

         // First Emission system data is just after this structure
         es = (const EmissionSystem*) p;

         for (int i = 0; i < idx; i++) {
            // step down to the correct emission system data using 'systemDataLength'
            p += (es->systemDataLength * 4);
            es = (const EmissionSystem*) p;
         }

      }
      return es;
   }

   // Swap bytes 'to' or 'from' the network.
   void swapBytes()
   {
      // Swap our stuff first
      header.swapBytes();
      emittingEntityID.swapBytes();
      eventID.swapBytes();

      // then swap emission data
      for(int i = 0; i < numberOfSystems; i++) {
         EmissionSystem* em = getEmissionSystem(i);
         if (em != 0) em->swapBytes();
      }
   }

   // Dump the PDU's data to the standard output device, std::cout.
   void dumpData()  const {
      std::cout << "------------------------------------------------" << std::endl;
      std::cout << "Electromagnetic PDU(" << (long)header.PDUType << ")" << std::endl;
      std::cout << "Header:" << std::endl << header;
      std::cout << "Emitting Entity:" << std::endl << emittingEntityID;
      std::cout << "Event ID:" << std::endl << eventID;
      std::cout << "State Update Indicator: " << (long)stateUpdateIndicator << std::endl;
      std::cout << "Num Of Systems:         " << (long)numberOfSystems << std::endl;

      for(int i = 0; i < numberOfSystems; i++){
         std::cout << "*****************************************" << std::endl;
         std::cout << ">>>System (" << i << ")<<<" << std::endl;
         const EmissionSystem* es = getEmissionSystem(i);
         if (es != 0) std::cout << es;
         std::cout << "*****************************************" << std::endl;
      }
      std::cout.flush();
   }
};


//-----------------------------------
// Transmitter PDU
//-----------------------------------
class TransmitterPDU {
public:

   PDUHeader            header;                 // PDU Header
   entityIdentifierDIS  emittingEntityID;       // Entity that owns these systems
   unsigned short       radioID;                // Radio ID (unique to entity)
   RadioEntityType      radioEntity;            // Radio type
   unsigned char        txState;                // Transmit state
   unsigned char        inputSource;            // Operator position: pilot, co-pilot, etc
   unsigned short       padding;
                                                
   double               antLoc_X_coord;         // Antenna location on entity
   double               antLoc_Y_coord;
   double               antLoc_Z_coord; 
   float                antLoc_x_coord;
   float                antLoc_y_coord;
   float                antLoc_z_coord;

   unsigned short       antennaPatternType;     // Pattern type 
   unsigned short       antennaPatternLength;   // Pattern length
   LCuint64             frequency;              // Center frequency (hz)
   float                transmitFrequencyBandwidth; //Bandwidth (hz)
   float                power;                  // Power decibel-milliwatts
   ModulationType       modulationType;         // Modulation type
   unsigned short       cryptoSystem;           // Crypto system
   unsigned short       cryptoKeyID;            // Crypto key id
   unsigned char        lengthOfModulationParameters;
   unsigned char        padding1;
   unsigned char        padding2;
   unsigned char        padding3;

   void swapBytes(){
      header.swapBytes();
      emittingEntityID.swapBytes();
      radioID = convertUShort(radioID);
      radioEntity.swapBytes();
      antLoc_X_coord = convertDouble(antLoc_X_coord);
      antLoc_Y_coord = convertDouble(antLoc_Y_coord);
      antLoc_Z_coord = convertDouble(antLoc_Z_coord);
      antLoc_x_coord = convertFloat(antLoc_x_coord);
      antLoc_y_coord = convertFloat(antLoc_y_coord);
      antLoc_z_coord = convertFloat(antLoc_z_coord);
      antennaPatternType = convertUShort(antennaPatternType);
      antennaPatternLength = convertUShort(antennaPatternLength);
      frequency = convertULongLong(frequency);
      transmitFrequencyBandwidth = convertFloat(transmitFrequencyBandwidth);
      power = convertFloat(power);
      modulationType.swapBytes();
      cryptoSystem = convertUShort(cryptoSystem);
      cryptoKeyID = convertUShort(cryptoKeyID);
      //modulationParam5 = convertUInt(modulationParam5);
   };

   char* getModulationData()
   {
      unsigned char* p = (unsigned char*)this;
      size_t offset = sizeof(TransmitterPDU);
      return (char*) &p[offset];
   };
  
   const char* getModulationData() const
   {
      const unsigned char* p = (const unsigned char*)this;
      size_t offset = sizeof(TransmitterPDU);
      return (const char*) &p[offset];
   };

   void dumpData() const {
      std::cout << "--------------------data---------------------" << std::endl;
      std::cout << "Emitting Entity ID:" << std::endl;
      std::cout << "\tEntity: " << emittingEntityID.ID << std::endl;
      std::cout << "\tAppID:  " << emittingEntityID.simulationID.applicationIdentification << std::endl;
      std::cout << "\tSiteID: " << emittingEntityID.simulationID.siteIdentification << std::endl;
      std::cout << "Radio ID: " << radioID << std::endl;
      std::cout << "RadioEntity:" << radioEntity << std::endl;
      std::cout << "Transmission State: " << (int)txState << std::endl;
      std::cout << "Input Source: " << (int)inputSource << std::endl;
      std::cout << "antLoc_X_coord: " << (long)antLoc_X_coord << std::endl;
      std::cout << "antLoc_Y_coord: " << (long)antLoc_Y_coord << std::endl;
      std::cout << "antLoc_Z_coord: " << (long)antLoc_Z_coord << std::endl;
      std::cout << "antLoc_x_coord: " << (long)antLoc_x_coord << std::endl;
      std::cout << "antLoc_y_coord: " << (long)antLoc_y_coord << std::endl;
      std::cout << "antLoc_z_coord: " << (long)antLoc_z_coord << std::endl;
      std::cout << "Antenna Pattern Type: " << antennaPatternType << std::endl;
      std::cout << "Antenna Pattern Length: " << antennaPatternLength << std::endl;
      std::cout << "Frequency: " << frequency << std::endl;
      std::cout << "Transmit Frequency Bandwidth: " << transmitFrequencyBandwidth << std::endl;
      std::cout << "Power: " << power << std::endl;
      std::cout << "Modulation Type: " << modulationType << std::endl;
      std::cout << "Crypto System: " << cryptoSystem << std::endl;
      std::cout << "Crypto Key ID: " << cryptoKeyID << std::endl;
      std::cout << "Length of Modulation Parameters: " << lengthOfModulationParameters << std::endl;
      std::cout << "--------------------end data-----------------" << std::endl;
   };

};

} // End Dis namespace
} // End Network namespace
} // End Eaagles namespace

#endif
