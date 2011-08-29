//------------------------------------------------------------------------------
// Classes:	AsciiText, NumericReadout, TimeReadout, HexReadout,
//		LatitudeReadout, LongitudeReadout, DirectionReadout, Rotary, Rotary2,
//	        Cursor
//------------------------------------------------------------------------------
#ifndef	__Readouts_H_3D8556B0_47BA_49e4_B3A7_3164B46D3439__
#define __Readouts_H_3D8556B0_47BA_49e4_B3A7_3164B46D3439__

#include "Field.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/Integer.h"


namespace Eaagles {
   namespace Basic { class Float; class Integer; }
namespace BasicGL {
   class Reformat;


//------------------------------------------------------------------------------
// Class:	AsciiText
// Base class: Object > Graphic > Field > AsciiText
//
// Form name: text
// Slots:
//    text   <String>     ! Text String (default: " ")
//    text   <List>       ! Text List
//
// Public Member Functions:
//	bool isValidInputPosition(int tc)
//		Returns true if tc is a valid input position.
//
//	bool setTextString(String* stsobj)
//		Returns true if the text string was set to stsobj.
//
//	bool setTextList(List* stlobj)
//		Returns true if the text list was set to stlobj.
//
// Guid: {3D8556B0-47BA-49e4-B3A7-3164B46D3439}
//------------------------------------------------------------------------------
class AsciiText : public Field {
    DECLARE_SUBCLASS(AsciiText,Field)

public:
   AsciiText();
   
   virtual char filterInputEvent(const int event, const int tc);
   virtual bool isValidInputPosition(const int tc);
   virtual bool setTextString(const Basic::String* const stsobj);
   virtual bool setTextList(const Basic::List* const stlobj);
   
   // Component interface
   virtual bool event(const int key, Basic::Object* const obj = 0);
};

//------------------------------------------------------------------------------
// Class:	Cursor
// Base class: Object > Graphic > Field > Cursor
//
// Form name: Cursor
//
// Guid: {951C6495-F576-433d-B6E2-61F6AF16402C}
//------------------------------------------------------------------------------
class Cursor : public Field {
    DECLARE_SUBCLASS(Cursor,Field)

public:
   Cursor();
   virtual void updateData(const LCreal dt = 0.0f);
};


//------------------------------------------------------------------------------
// Class:	NumericReadout
// Base class: Object > Graphic > Field > NumericReadout
// Form name: NumericReadout
//
// Slots:
//   value           <Float>     ! Value to be displayed (default: 0.0)
//   value           <Number>    ! Value to be displayed
//   maxValue        <Float>     ! Maximum value that can be displayed (default: UNDEFINED_VALUE)
//   maxValue        <Number>    ! Maximum value that can be displayed
//   format          <String>    ! Example format text (default: '\0')
//   plusChar        <String>    ! Positive value character (default: '\0')
//   minusChar       <String>    ! Negative value character (default: '\0')
//   dpChar          <String>    ! Decimal Point character (default: '\0')
//   undefinedChar   <String>    ! Undefined value character (default: '-')
//   overflowChar    <String>    ! Overflow character (default: '+')
//   maxValid        <Number>    ! Maximum valid input (default: UNDEFINED_VALUE)
//   minValid        <Number>    ! Minimum valid input (default: UNDEFINED_VALUE)
//   blankIfZero     <Boolean>   ! Display blanks if value is zero
//
// Example formats: 
//
//    ######	// Integer
//    +#####	// Integer w/plus sign
//    00000#	// Integer w/leading zeros
//    +0000#	// Integer w/plus sign & leading zeros
//    ###.## 	// Float w/2 right of decimal point
//    +##.## 	// Float w/plus sign, 2 right of decimal point
//    00#.## 	// Float w/2 right of decimal point & leading zeros
//    +0#.## 	// Float w/plus sign, 2 right of decimal point, & leading zeros
//
// Public Member Functions:
//	int getInt()
//		Returns num as an int.
//
//	double getFloat()
//		Returns num. (native)
//
//	setValue(int v)
//	setValue(double v)
//		Sets num to v as an double (in both cases) then redisplays the value.
//
//	setMaxValue(int v)
//	setMaxValue(double v)
//		Sets maxNum to v as an double (in both cases) then redisplays the value.
//
//	double getInputValue()
//		Gets the input value.
//
//	bool isInputValueValid()
//		Checks the input value and returns true if it is valid.
//
//Comment sections last updated: 2004.10.14 by MJK
//
// Guid: {F4255967-7E11-4300-942F-0B4C9A7E1687}
//------------------------------------------------------------------------------
class NumericReadout : public Field {
   DECLARE_SUBCLASS(NumericReadout,Field)

public:
   NumericReadout(); 

   int getInt() const               { return int(num); }
   double getFloat() const          { return num; }

   void setValue(const int v)       { num = double(v); redisplay(); }
   void setValue(const double v)    { num = v; redisplay(); }

   void setMaxValue(const int v)    { maxNum = double(v); redisplay(); }
   void setMaxValue(const double v) { maxNum = v; redisplay(); }

   virtual double getInputValue() const;
   virtual bool isInputValueValid() const;

   virtual char filterInputEvent(const int event, const int tc);

   //event handler macro functions
   virtual bool onUpdateValue(const Basic::Float* const ouvobj);
   virtual bool onUpdateValue(const Basic::Integer* const ouvobj);
   virtual bool onUpdateValue(const Basic::Number* const ouvobj);
   
   // Component interface
   virtual bool event(const int key, Basic::Object* const obj = 0);
   virtual void updateData(const LCreal dt = 0.0);

protected:
   virtual bool setSlotFloatToBeDisplayed(const Basic::Float* const msg);
   virtual bool setSlotNumberToBeDisplayed(const Basic::Number* const msg);
   virtual bool setSlotFloatMaxValue(const Basic::Float* const msg);
   virtual bool setSlotNumberMaxValue(const Basic::Number* const msg);
   virtual bool setSlotExampleFormatText(const Basic::String* const msg);
   virtual bool setSlotPlusChar(const Basic::String* const msg);
   virtual bool setSlotMinusChar(const Basic::String* const msg);
   virtual bool setSlotDecimalPointChar(const Basic::String* const msg);
   virtual bool setSlotUndefinedChar(const Basic::String* const msg);
   virtual bool setSlotOverflowChar(const Basic::String* const msg);
   virtual bool setSlotMaxValid(const Basic::Number* const msg);
   virtual bool setSlotMinValid(const Basic::Number* const msg);
   virtual bool setSlotBlankZero(const Basic::Number* const msg);
   
   virtual void makeText();
   virtual void redisplay();
   virtual void reformat(const char* const example);

   static const size_t CBUF_LENGTH = 32;   // Max length of cbuf
   static const size_t FORMAT_LENGTH = 32; // Max length of format

   char cbuf[CBUF_LENGTH];       // buffer
   char format[FORMAT_LENGTH];   // Current format string

   char plusChar;                // Positive value character
   char minusChar;               // Negative value character
   char dpChar;                  // Decimal point character
   char undefinedChar;           // Undefined value character
   char overflowChar;            // Overflow character  
   bool postSign;                // If true, sign char is at end of string

   static Reformat* reformatter; // Generates format statements by example

private:
   double   num;            // Value as double
   double   maxNum;         // Maximum value
   double   maxValid;       // Maximum valid input value
   double   minValid;       // Minimum valid input value
   bool     blankZero;      // Display blank instead of zero value
};


//------------------------------------------------------------------------------
// Class:	HexReadout
// Base class: Object > Graphic > Field > NumericReadout > HexReadout
//
// Form name: HexReadout
//
// Example formats: 
//
//    ######	// Hex number
//    00000#	// Hex number w/leading zeros
//
// Guid: {1B78328A-D4A5-4ad3-A194-FE69B5A368B6}
//------------------------------------------------------------------------------
class HexReadout : public NumericReadout {
   DECLARE_SUBCLASS(HexReadout,NumericReadout)

public:
   HexReadout(); 
   virtual void makeText();
   virtual char filterInputEvent(const int event, const int tc);
   virtual double getInputValue() const;

protected:
    virtual void reformat(const char* const example);
};


//------------------------------------------------------------------------------
// Class:	OctalReadout
// Base class: Object > Graphic > Field > NumericReadout > OctalReadout
//
// Form name: OctalReadout
//
// Example formats: 
//
//    ######	// Octal number
//    00000#	// Octal number w/leading zeros
//
// Guid: {495FBE81-E6E4-4ff9-BF35-07D3111750DF}
//------------------------------------------------------------------------------
class OctalReadout : public NumericReadout {
   DECLARE_SUBCLASS(OctalReadout,NumericReadout)

public:
   OctalReadout(); 
   virtual void makeText();
   virtual char filterInputEvent(const int event, const int tc);
   virtual double getInputValue() const;

protected:
    virtual void reformat(const char* const example);
};


//------------------------------------------------------------------------------
// Class:	TimeReadout
// Base class:	Object > Graphic > Field > NumericReadout > TimeReadout
//
// Form name: TimeReadout
//
// Example formats: 
//
//    HH:MM:SS 		// Hours, minutes and seconds
//    0H:MM:SS.S	// Hours (w/leading zero), minutes and seconds decimal
//   +HH:MM		// Hours (w/sign) and minutes
//    HHMM.M		// Hours (w/sign) and minutes decimal
//   +0HMM:SS.S		// Hours (w/signed and leading zero), minutes
//			//   and seconds decimal
//    MMSS.S		// Minutes and seconds decimal
//    SSSS		// Seconds
//
// Notes: The seconds and third numbers always have leading zeros.  The first
// number requires a zero, '0', to have leading zeros.
//
// Guid: {28202353-B187-4814-969D-F51A7E944EA0}
//------------------------------------------------------------------------------
class TimeReadout : public NumericReadout {
   DECLARE_SUBCLASS(TimeReadout,NumericReadout)

public:
   enum TimeMode { invalid, hhmmss, hhmm, hh, mmss, mm, ss };
   TimeReadout(); 
   virtual char filterInputEvent(const int event, const int tc);
   virtual double getInputValue() const;

protected:
   virtual void makeText();
   virtual void reformat(const char* const example);
   TimeMode tmode;
};


//------------------------------------------------------------------------------
// Class:	DirectionReadout
// Base class: Object > Graphic > Field > NumericReadout > DirectionReadout
//
// Form name: DirectionReadout
//
// Guid: {731A5994-384A-4ef5-91EF-93194D838B83}
//------------------------------------------------------------------------------
class DirectionReadout : public NumericReadout {
   DECLARE_SUBCLASS(DirectionReadout,NumericReadout)

public:
   enum DirMode { invalid, ddmmss, ddmm, dd };
   DirectionReadout();
   virtual char filterInputEvent(const int event, const int tc);
   virtual double getInputValue() const;

protected:
   virtual void makeText();
   virtual void reformat(const char* const example);
   DirMode tmode;
};


//------------------------------------------------------------------------------
// Class:	LatitudeReadout
// Base class: Object > Graphic > Field > NumericReadout > DirectionReadout > LatitudeReadout
//
// Form name: LatitudeReadout
//
// Guid: {872DA29B-A6CA-4e4b-A4D1-E071B7F79A89}
//------------------------------------------------------------------------------
class LatitudeReadout : public DirectionReadout {
    DECLARE_SUBCLASS(LatitudeReadout,DirectionReadout)
public:
    LatitudeReadout();
    virtual char filterInputEvent(const int event, const int tc);
};


//------------------------------------------------------------------------------
// Class:	LongitudeReadout
// Base class:	Object > Graphic > Field > NumericReadout > DirectionReadout > LongitudeReadout
//
// Form name: LongitudeReadout
//
// Guid: {4958F477-3AAB-48fa-AF34-4788EFB95CD0}
//------------------------------------------------------------------------------
class LongitudeReadout : public DirectionReadout {
    DECLARE_SUBCLASS(LongitudeReadout,DirectionReadout)
public:
    LongitudeReadout();
    virtual char filterInputEvent(const int event, const int tc);
};


//------------------------------------------------------------------------------
// Class:	Rotary
// Base class: Object > Graphic > Field > Rotary
//
// Description: Rotary text
//
// Form name: Rotary
//
//
// Guid: {6976864B-D419-4b46-B73D-8DB1FE4ECED1}
//------------------------------------------------------------------------------
class Rotary : public Field {
    DECLARE_SUBCLASS(Rotary,Field)

public:
   Rotary();
   virtual bool event(const int key, Object* const obj = 0);
};


//------------------------------------------------------------------------------
// Class:	Rotary2
// Base class: Object > Graphic > Field > Rotary > Rotary2
//
// Description: Binary rotary
//
// Form name: Rotary2
//
// Guid: {612CC5EF-B400-489b-8978-23A583004BB6}
//------------------------------------------------------------------------------
class Rotary2 : public Rotary {
    DECLARE_SUBCLASS(Rotary2,Rotary)

public:
   Rotary2();
   virtual bool event(const int key, Basic::Object* const obj = 0);
   //macro function for event handler
   virtual bool onSelect(const Basic::Number* const osobj);
};

} // End BasicGL namespace
} // End Eaagles namespace

#endif	/* __Readouts_H_3D8556B0_47BA_49e4_B3A7_3164B46D3439__ */
