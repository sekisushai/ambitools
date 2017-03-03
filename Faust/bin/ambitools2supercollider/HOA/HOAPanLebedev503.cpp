/* ------------------------------------------------------------
author: "Pierre Lecomte"
copyright: "(c) Pierre Lecomte 2014"
license: "GPL"
name: "HOAPanLebedev503"
version: "1.0"
Code generated with Faust 2.0.a55 (http://faust.grame.fr)
------------------------------------------------------------ */

#ifndef  __mydsp_H__
#define  __mydsp_H__

// If other than 'faust2sc --prefix Faust' is used, sed this as well:
#if !defined(SC_FAUST_PREFIX)
# define SC_FAUST_PREFIX "Faust"
#endif

//-------------------------------------------------------------------
// FAUST architecture file for SuperCollider.
// Copyright (C) 2005-2012 Stefan Kersten.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
// 02111-1307 USA
//-------------------------------------------------------------------

#include <map>
#include <string>
#include <string.h>
#include <SC_PlugIn.h>

/************************************************************************
    IMPORTANT NOTE : this file contains two clearly delimited sections :
    the ARCHITECTURE section (in two parts) and the USER section. Each section
    is governed by its own copyright and license. Please check individually
    each section for license and copyright information.
*************************************************************************/

/*******************BEGIN ARCHITECTURE SECTION (part 1/2)****************/

/************************************************************************
    FAUST Architecture File
    Copyright (C) 2003-2011 GRAME, Centre National de Creation Musicale
    ---------------------------------------------------------------------
    This Architecture section is free software; you can redistribute it
    and/or modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 3 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; If not, see <http://www.gnu.org/licenses/>.

    EXCEPTION : As a special exception, you may create a larger work
    that contains this FAUST architecture section and distribute
    that work under terms of your choice, so long as this FAUST
    architecture section is not modified.

 ************************************************************************
 ************************************************************************/
 
/******************************************************************************
*******************************************************************************

								FAUST DSP

*******************************************************************************
*******************************************************************************/

#ifndef __dsp__
#define __dsp__

#include <string>

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

class UI;
struct Meta;

/**
* Signal processor definition.
*/

class dsp {

    public:

        dsp() {}
        virtual ~dsp() {}

        /* Return instance number of audio inputs */
        virtual int getNumInputs() = 0;
    
        /* Return instance number of audio outputs */
        virtual int getNumOutputs() = 0;
    
        /**
         * Trigger the UI* parameter with instance specific calls
         * to 'addBtton', 'addVerticalSlider'... in order to build the UI.
         *
         * @param ui_interface - the UI* user interface builder
         */
        virtual void buildUserInterface(UI* ui_interface) = 0;
    
        /* Returns the sample rate currently used by the instance */
        virtual int getSampleRate() = 0;

        /** Global init, calls the following methods :
         * - static class 'classInit' : static table initialisation
         * - 'instanceInit' : constants and instance table initialisation
         *
         * @param samplingRate - the sampling rate in Herz
         */
        virtual void init(int samplingRate) = 0;

        /** Init instance state
         *
         * @param samplingRate - the sampling rate in Herz
         */
        virtual void instanceInit(int samplingRate) = 0;

        /** Init instance constant state
         *
         * @param samplingRate - the sampling rate in Herz
         */
        virtual void instanceConstants(int samplingRate) = 0;
    
        /* Init default control parameters values */
        virtual void instanceResetUserInterface() = 0;
    
        /* Init instance state (delay lines...) */
        virtual void instanceClear() = 0;
 
        /**
         * Return a clone of the instance.
         *
         * @return a copy of the instance on success, otherwise a null pointer.
         */
        virtual dsp* clone() = 0;
    
        /**
         * Trigger the Meta* parameter with instance specific calls to 'declare' (key, value metadata).
         *
         * @param m - the Meta* meta user
         */
        virtual void metadata(Meta* m) = 0;
    
        /**
         * DSP instance computation, to be called with sucessive in/out audio buffers.
         *
         * @param count - the nomber of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, doucbe or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, doucbe or quad)
         *
         */
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) = 0;
    
        /**
         * DSP instance computation : alternative method to be used by subclasses.
         *
         * @param date_usec - the timestamp in microsec given by audio driver.
         * @param count - the nomber of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, doucbe or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, doucbe or quad)
         *
         */
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
       
};

/**
 * Generic DSP decorator.
 */

class decorator_dsp : public dsp {

    protected:

        dsp* fDSP;

    public:

        decorator_dsp(dsp* dsp = 0):fDSP(dsp) {}
        virtual ~decorator_dsp() { delete fDSP; }

        virtual int getNumInputs() { return fDSP->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP->getNumOutputs(); }
        virtual void buildUserInterface(UI* ui_interface) { fDSP->buildUserInterface(ui_interface); }
        virtual int getSampleRate() { return fDSP->getSampleRate(); }
        virtual void init(int samplingRate) { fDSP->init(samplingRate); }
        virtual void instanceInit(int samplingRate) { fDSP->instanceInit(samplingRate); }
        virtual void instanceConstants(int samplingRate) { fDSP->instanceConstants(samplingRate); }
        virtual void instanceResetUserInterface() { fDSP->instanceResetUserInterface(); }
        virtual void instanceClear() { fDSP->instanceClear(); }
        virtual decorator_dsp* clone() { return new decorator_dsp(fDSP->clone()); }
        virtual void metadata(Meta* m) { return fDSP->metadata(m); }
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(count, inputs, outputs); }
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(date_usec, count, inputs, outputs); }
    
};

/**
 * DSP factory class
 */

class dsp_factory {
    
    protected:
    
        // So that to force sub-classes to use deleteDSPFactory(dsp_factory* factory);
        ~dsp_factory() {}
    
    public:
    
        virtual std::string getName() = 0;
        virtual std::string getSHAKey() = 0;
        virtual std::string getDSPCode() = 0;
        virtual dsp* createDSPInstance() = 0;
    
};

/**
 * On Intel set FZ (Flush to Zero) and DAZ (Denormals Are Zero)
 * flags to avoid costly denormals.
 */

#ifdef __SSE__
    #include <xmmintrin.h>
    #ifdef __SSE2__
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8040)
    #else
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8000)
    #endif
#else
    #define AVOIDDENORMALS
#endif

#endif
/************************************************************************
    FAUST Architecture File
    Copyright (C) 2003-2016 GRAME, Centre National de Creation Musicale
    ---------------------------------------------------------------------
    This Architecture section is free software; you can redistribute it
    and/or modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 3 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; If not, see <http://www.gnu.org/licenses/>.

    EXCEPTION : As a special exception, you may create a larger work
    that contains this FAUST architecture section and distribute
    that work under terms of your choice, so long as this FAUST
    architecture section is not modified.


 ************************************************************************
 ************************************************************************/
 
#ifndef FAUST_UI_H
#define FAUST_UI_H

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

/*******************************************************************************
 * UI : Faust User Interface
 * This abstract class contains only the method that the faust compiler can
 * generate to describe a DSP interface.
 ******************************************************************************/

class UI
{

    public:

        UI() {}

        virtual ~UI() {}

        // -- widget's layouts

        virtual void openTabBox(const char* label) = 0;
        virtual void openHorizontalBox(const char* label) = 0;
        virtual void openVerticalBox(const char* label) = 0;
        virtual void closeBox() = 0;

        // -- active widgets

        virtual void addButton(const char* label, FAUSTFLOAT* zone) = 0;
        virtual void addCheckButton(const char* label, FAUSTFLOAT* zone) = 0;
        virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;

        // -- passive widgets

        virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) = 0;
        virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) = 0;

        // -- metadata declarations

        virtual void declare(FAUSTFLOAT*, const char*, const char*) {}
};

//----------------------------------------------------------------
//  Generic decorator
//----------------------------------------------------------------

class DecoratorUI : public UI
{
    protected:
    
        UI* fUI;

    public:
    
        DecoratorUI(UI* ui = 0):fUI(ui)
        {}

        virtual ~DecoratorUI() { delete fUI; }

        // -- widget's layouts
        virtual void openTabBox(const char* label)          { fUI->openTabBox(label); }
        virtual void openHorizontalBox(const char* label)   { fUI->openHorizontalBox(label); }
        virtual void openVerticalBox(const char* label)     { fUI->openVerticalBox(label); }
        virtual void closeBox()                             { fUI->closeBox(); }

        // -- active widgets
        virtual void addButton(const char* label, FAUSTFLOAT* zone)         { fUI->addButton(label, zone); }
        virtual void addCheckButton(const char* label, FAUSTFLOAT* zone)    { fUI->addCheckButton(label, zone); }
        virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
            { fUI->addVerticalSlider(label, zone, init, min, max, step); }
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) 	
            { fUI->addHorizontalSlider(label, zone, init, min, max, step); }
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) 			
            { fUI->addNumEntry(label, zone, init, min, max, step); }

        // -- passive widgets	
        virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) 
            { fUI->addHorizontalBargraph(label, zone, min, max); }
        virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
            { fUI->addVerticalBargraph(label, zone, min, max); }

        virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val) { fUI->declare(zone, key, val); }

};

#endif
/************************************************************************
 ************************************************************************
    FAUST Architecture File
	Copyright (C) 2003-2011 GRAME, Centre National de Creation Musicale
    ---------------------------------------------------------------------
    This Architecture section is free software; you can redistribute it
    and/or modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3 of
	the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
	along with this program; If not, see <http://www.gnu.org/licenses/>.

 ************************************************************************
 ************************************************************************/
 
#ifndef __misc__
#define __misc__

#include <algorithm>
#include <map>
#include <string.h>
#include <stdlib.h>

/************************************************************************
 ************************************************************************
    FAUST Architecture File
	Copyright (C) 2003-2011 GRAME, Centre National de Creation Musicale
    ---------------------------------------------------------------------
    This Architecture section is free software; you can redistribute it
    and/or modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3 of
	the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
	along with this program; If not, see <http://www.gnu.org/licenses/>.

 ************************************************************************
 ************************************************************************/
 
#ifndef __meta__
#define __meta__

struct Meta
{
    virtual void declare(const char* key, const char* value) = 0;
    virtual ~Meta() {};
};

#endif

using std::max;
using std::min;

struct XXXX_Meta : std::map<const char*, const char*>
{
    void declare(const char* key, const char* value) { (*this)[key]=value; }
};

struct MY_Meta : Meta, std::map<const char*, const char*>
{
    void declare(const char* key, const char* value) { (*this)[key]=value; }
};

inline int lsr(int x, int n)	{ return int(((unsigned int)x) >> n); }

inline int int2pow2(int x)		{ int r = 0; while ((1<<r) < x) r++; return r; }

inline long lopt(char* argv[], const char* name, long def)
{
	int	i;
	for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return atoi(argv[i+1]);
	return def;
}

inline bool isopt(char* argv[], const char* name)
{
	int	i;
	for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return true;
	return false;
}

inline const char* lopts(char* argv[], const char* name, const char* def)
{
	int	i;
	for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return argv[i+1];
	return def;
}

#endif


using namespace std;

#if defined(__GNUC__) && __GNUC__ >= 4
# define FAUST_EXPORT __attribute__((visibility("default")))
#else
# define FAUST_EXPORT  SC_API_EXPORT
#endif

//----------------------------------------------------------------------------
// Vector intrinsics
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// Metadata
//----------------------------------------------------------------------------

class MetaData : public Meta
               , public std::map<std::string, std::string>
{
public:
    void declare(const char* key, const char* value)
    {
        (*this)[key] = value;
    }
};

//----------------------------------------------------------------------------
// Control counter
//----------------------------------------------------------------------------

class ControlCounter : public UI
{
public:
    ControlCounter()
        : mNumControlInputs(0),
          mNumControlOutputs(0)
    { }

    size_t getNumControls() const { return getNumControlInputs(); }
    size_t getNumControlInputs() const { return mNumControlInputs; }
    size_t getNumControlOutputs() const { return mNumControlOutputs; }

    // Layout widgets
    virtual void openTabBox(const char* label) { }
    virtual void openHorizontalBox(const char* label) { }
    virtual void openVerticalBox(const char* label) { }
    virtual void closeBox() { }

    // Active widgets
    virtual void addButton(const char* label, FAUSTFLOAT* zone)
    { addControlInput(); }
    virtual void addCheckButton(const char* label, FAUSTFLOAT* zone)
    { addControlInput(); }
    virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    { addControlInput(); }
    virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    { addControlInput(); }
    virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    { addControlInput(); }

    // Passive widgets
    virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
    { addControlOutput(); }
    virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
    { addControlOutput(); }

protected:
    void addControlInput() { mNumControlInputs++; }
    void addControlOutput() { mNumControlOutputs++; }

private:
    size_t mNumControlInputs;
    size_t mNumControlOutputs;
};

//----------------------------------------------------------------------------
// UI control
//----------------------------------------------------------------------------

struct Control
{
    typedef void (*UpdateFunction)(Control* self, FAUSTFLOAT value);

    UpdateFunction updateFunction;
    FAUSTFLOAT* zone;
    FAUSTFLOAT min, max;

    inline void update(FAUSTFLOAT value)
    {
        (*updateFunction)(this, value);
    }

    static void simpleUpdate(Control* self, FAUSTFLOAT value)
    {
        *self->zone = value;
    }
    static void boundedUpdate(Control* self, FAUSTFLOAT value)
    {
        *self->zone = sc_clip(value, self->min, self->max);
    }
};

//----------------------------------------------------------------------------
// Control allocator
//----------------------------------------------------------------------------

class ControlAllocator : public UI
{
public:
    ControlAllocator(Control* controls)
        : mControls(controls)
    { }

    // Layout widgets
    virtual void openTabBox(const char* label) { }
    virtual void openHorizontalBox(const char* label) { }
    virtual void openVerticalBox(const char* label) { }
    virtual void closeBox() { }

    // Active widgets
    virtual void addButton(const char* label, FAUSTFLOAT* zone)
    { addSimpleControl(zone); }
    virtual void addCheckButton(const char* label, FAUSTFLOAT* zone)
    { addSimpleControl(zone); }
    virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    { addBoundedControl(zone, min, max, step); }
    virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    { addBoundedControl(zone, min, max, step); }
    virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    { addBoundedControl(zone, min, max, step); }

    // Passive widgets
    virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) { }
    virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) { }

private:
    void addControl(Control::UpdateFunction updateFunction, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT /* step */)
    {
        Control* ctrl        = mControls++;
        ctrl->updateFunction = updateFunction;
        ctrl->zone           = zone;
        ctrl->min            = min;
        ctrl->max            = max;
    }
    void addSimpleControl(FAUSTFLOAT* zone)
    {
        addControl(Control::simpleUpdate, zone, 0.f, 0.f, 0.f);
    }
    void addBoundedControl(FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        addControl(Control::boundedUpdate, zone, min, max, step);
    }

private:
    Control* mControls;
};

//----------------------------------------------------------------------------
// FAUST generated code
//----------------------------------------------------------------------------

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif  

#include <math.h>

float faustpower2_f(float value) {
	return (value * value);
	
}
float faustpower3_f(float value) {
	return ((value * value) * value);
	
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
#endif

class mydsp : public dsp {
	
 private:
	
	int fSamplingFreq;
	float fRec2[2];
	float fRec3[2];
	float fRec4[2];
	float fRec1[2];
	float fRec5[2];
	float fVec0[2];
	float fRec6[2];
	float fRec8[2];
	float fRec9[2];
	float fRec7[2];
	float fRec12[2];
	float fRec11[2];
	float fRec10[2];
	float fRec14[2];
	float fRec13[2];
	float fRec18[2];
	float fRec19[2];
	float fRec17[2];
	float fRec20[2];
	float fVec1[2];
	float fRec21[2];
	float fRec22[2];
	float fRec16[2];
	float fRec15[2];
	float fRec25[2];
	float fRec26[2];
	float fRec24[2];
	float fRec27[2];
	float fRec23[2];
	float fRec0[2];
	float fRec28[2];
	float fRec29[2];
	float fRec30[2];
	float fRec31[2];
	float fRec32[2];
	float fRec33[2];
	float fRec34[2];
	float fRec35[2];
	float fRec36[2];
	float fRec37[2];
	float fRec38[2];
	float fRec39[2];
	float fRec40[2];
	float fRec41[2];
	float fRec42[2];
	float fRec43[2];
	float fRec44[2];
	float fRec45[2];
	float fRec46[2];
	float fRec47[2];
	float fRec48[2];
	float fRec49[2];
	float fRec50[2];
	float fRec51[2];
	float fRec52[2];
	float fRec53[2];
	float fRec54[2];
	float fRec55[2];
	float fRec56[2];
	float fRec57[2];
	float fRec58[2];
	float fRec59[2];
	float fRec60[2];
	float fRec61[2];
	float fRec62[2];
	float fRec63[2];
	float fRec64[2];
	float fRec65[2];
	float fRec66[2];
	float fRec67[2];
	float fRec68[2];
	float fRec69[2];
	float fRec70[2];
	float fRec71[2];
	float fRec72[2];
	float fRec73[2];
	float fRec74[2];
	float fRec75[2];
	float fRec76[2];
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fCheckbox0;
	float fConst2;
	FAUSTFLOAT fEntry0;
	float fConst3;
	FAUSTFLOAT fHslider2;
	FAUSTFLOAT fCheckbox1;
	FAUSTFLOAT fHslider3;
	float fConst4;
	FAUSTFLOAT fCheckbox2;
	float fConst5;
	FAUSTFLOAT fCheckbox3;
	float fConst6;
	float fConst7;
	float fConst8;
	float fConst9;
	FAUSTFLOAT fCheckbox4;
	FAUSTFLOAT fVbargraph0;
	FAUSTFLOAT fVbargraph1;
	FAUSTFLOAT fVbargraph2;
	FAUSTFLOAT fVbargraph3;
	FAUSTFLOAT fVbargraph4;
	FAUSTFLOAT fVbargraph5;
	FAUSTFLOAT fVbargraph6;
	FAUSTFLOAT fVbargraph7;
	FAUSTFLOAT fVbargraph8;
	FAUSTFLOAT fVbargraph9;
	FAUSTFLOAT fVbargraph10;
	FAUSTFLOAT fVbargraph11;
	FAUSTFLOAT fVbargraph12;
	FAUSTFLOAT fVbargraph13;
	FAUSTFLOAT fVbargraph14;
	FAUSTFLOAT fVbargraph15;
	FAUSTFLOAT fVbargraph16;
	FAUSTFLOAT fVbargraph17;
	FAUSTFLOAT fVbargraph18;
	FAUSTFLOAT fVbargraph19;
	FAUSTFLOAT fVbargraph20;
	FAUSTFLOAT fVbargraph21;
	FAUSTFLOAT fVbargraph22;
	FAUSTFLOAT fVbargraph23;
	FAUSTFLOAT fVbargraph24;
	FAUSTFLOAT fVbargraph25;
	FAUSTFLOAT fVbargraph26;
	FAUSTFLOAT fVbargraph27;
	FAUSTFLOAT fVbargraph28;
	FAUSTFLOAT fVbargraph29;
	FAUSTFLOAT fVbargraph30;
	FAUSTFLOAT fVbargraph31;
	FAUSTFLOAT fVbargraph32;
	FAUSTFLOAT fVbargraph33;
	FAUSTFLOAT fVbargraph34;
	FAUSTFLOAT fVbargraph35;
	FAUSTFLOAT fVbargraph36;
	FAUSTFLOAT fVbargraph37;
	FAUSTFLOAT fVbargraph38;
	FAUSTFLOAT fVbargraph39;
	FAUSTFLOAT fVbargraph40;
	FAUSTFLOAT fVbargraph41;
	FAUSTFLOAT fVbargraph42;
	FAUSTFLOAT fVbargraph43;
	FAUSTFLOAT fVbargraph44;
	FAUSTFLOAT fVbargraph45;
	FAUSTFLOAT fVbargraph46;
	FAUSTFLOAT fVbargraph47;
	FAUSTFLOAT fVbargraph48;
	FAUSTFLOAT fVbargraph49;
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("author", "Pierre Lecomte");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.0");
		m->declare("copyright", "(c) Pierre Lecomte 2014");
		m->declare("lib/gui.lib/author", "Pierre Lecomte");
		m->declare("lib/gui.lib/copyright", "(c) Pierre Lecomte 2016");
		m->declare("lib/gui.lib/license", "GPL");
		m->declare("lib/gui.lib/name", "GUI Library");
		m->declare("lib/gui.lib/version", "1.0");
		m->declare("lib/lebedev.lib/author", "Pierre Lecomte");
		m->declare("lib/lebedev.lib/copyright", "(c) Pierre Lecomte 2014");
		m->declare("lib/lebedev.lib/license", "GPL");
		m->declare("lib/lebedev.lib/name", "Lebdev grids and weights");
		m->declare("lib/lebedev.lib/version", "1.0");
		m->declare("lib/nfc.lib/author", "Pierre Lecomte");
		m->declare("lib/nfc.lib/copyright", "(c) Pierre Lecomte 2014");
		m->declare("lib/nfc.lib/license", "GPL");
		m->declare("lib/nfc.lib/name", "NF And NFC Filters Library");
		m->declare("lib/nfc.lib/version", "1.0");
		m->declare("lib/ymn.lib/author", "Pierre Lecomte");
		m->declare("lib/ymn.lib/copyright", "(c) Pierre Lecomte 2016");
		m->declare("lib/ymn.lib/license", "GPL");
		m->declare("lib/ymn.lib/name", "Spherical Harmonics library");
		m->declare("lib/ymn.lib/version", "1.0");
		m->declare("license", "GPL");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.0");
		m->declare("name", "HOAPanLebedev503");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
		m->declare("version", "1.0");
	}

	virtual int getNumInputs() {
		return 1;
		
	}
	virtual int getNumOutputs() {
		return 50;
		
	}
	virtual int getInputRate(int channel) {
		int rate;
		switch (channel) {
			case 0: {
				rate = 1;
				break;
			}
			default: {
				rate = -1;
				break;
			}
			
		}
		return rate;
		
	}
	virtual int getOutputRate(int channel) {
		int rate;
		switch (channel) {
			case 0: {
				rate = 1;
				break;
			}
			case 1: {
				rate = 1;
				break;
			}
			case 2: {
				rate = 1;
				break;
			}
			case 3: {
				rate = 1;
				break;
			}
			case 4: {
				rate = 1;
				break;
			}
			case 5: {
				rate = 1;
				break;
			}
			case 6: {
				rate = 1;
				break;
			}
			case 7: {
				rate = 1;
				break;
			}
			case 8: {
				rate = 1;
				break;
			}
			case 9: {
				rate = 1;
				break;
			}
			case 10: {
				rate = 1;
				break;
			}
			case 11: {
				rate = 1;
				break;
			}
			case 12: {
				rate = 1;
				break;
			}
			case 13: {
				rate = 1;
				break;
			}
			case 14: {
				rate = 1;
				break;
			}
			case 15: {
				rate = 1;
				break;
			}
			case 16: {
				rate = 1;
				break;
			}
			case 17: {
				rate = 1;
				break;
			}
			case 18: {
				rate = 1;
				break;
			}
			case 19: {
				rate = 1;
				break;
			}
			case 20: {
				rate = 1;
				break;
			}
			case 21: {
				rate = 1;
				break;
			}
			case 22: {
				rate = 1;
				break;
			}
			case 23: {
				rate = 1;
				break;
			}
			case 24: {
				rate = 1;
				break;
			}
			case 25: {
				rate = 1;
				break;
			}
			case 26: {
				rate = 1;
				break;
			}
			case 27: {
				rate = 1;
				break;
			}
			case 28: {
				rate = 1;
				break;
			}
			case 29: {
				rate = 1;
				break;
			}
			case 30: {
				rate = 1;
				break;
			}
			case 31: {
				rate = 1;
				break;
			}
			case 32: {
				rate = 1;
				break;
			}
			case 33: {
				rate = 1;
				break;
			}
			case 34: {
				rate = 1;
				break;
			}
			case 35: {
				rate = 1;
				break;
			}
			case 36: {
				rate = 1;
				break;
			}
			case 37: {
				rate = 1;
				break;
			}
			case 38: {
				rate = 1;
				break;
			}
			case 39: {
				rate = 1;
				break;
			}
			case 40: {
				rate = 1;
				break;
			}
			case 41: {
				rate = 1;
				break;
			}
			case 42: {
				rate = 1;
				break;
			}
			case 43: {
				rate = 1;
				break;
			}
			case 44: {
				rate = 1;
				break;
			}
			case 45: {
				rate = 1;
				break;
			}
			case 46: {
				rate = 1;
				break;
			}
			case 47: {
				rate = 1;
				break;
			}
			case 48: {
				rate = 1;
				break;
			}
			case 49: {
				rate = 1;
				break;
			}
			default: {
				rate = -1;
				break;
			}
			
		}
		return rate;
		
	}
	
	static void classInit(int samplingFreq) {
		
	}
	
	virtual void instanceConstants(int samplingFreq) {
		fSamplingFreq = samplingFreq;
		fConst0 = min(192000.0f, max(1.0f, float(fSamplingFreq)));
		fConst1 = (80.0f / fConst0);
		fConst2 = (510.0f / fConst0);
		fConst3 = (170.0f / fConst0);
		fConst4 = (1020.0f / fConst0);
		fConst5 = (340.0f / fConst0);
		fConst6 = (789.54303f / fConst0);
		fConst7 = (394.771515f / fConst0);
		fConst8 = (625.228516f / fConst0);
		fConst9 = (1250.45703f / fConst0);
		
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.0f);
		fHslider1 = FAUSTFLOAT(0.0f);
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fEntry0 = FAUSTFLOAT(1.0700000000000001f);
		fHslider2 = FAUSTFLOAT(2.0f);
		fCheckbox1 = FAUSTFLOAT(0.0f);
		fHslider3 = FAUSTFLOAT(0.0f);
		fCheckbox2 = FAUSTFLOAT(0.0f);
		fCheckbox3 = FAUSTFLOAT(0.0f);
		fCheckbox4 = FAUSTFLOAT(0.0f);
		
	}
	
	virtual void instanceClear() {
		for (int i0 = 0; (i0 < 2); i0 = (i0 + 1)) {
			fRec2[i0] = 0.0f;
			
		}
		for (int i1 = 0; (i1 < 2); i1 = (i1 + 1)) {
			fRec3[i1] = 0.0f;
			
		}
		for (int i2 = 0; (i2 < 2); i2 = (i2 + 1)) {
			fRec4[i2] = 0.0f;
			
		}
		for (int i3 = 0; (i3 < 2); i3 = (i3 + 1)) {
			fRec1[i3] = 0.0f;
			
		}
		for (int i4 = 0; (i4 < 2); i4 = (i4 + 1)) {
			fRec5[i4] = 0.0f;
			
		}
		for (int i5 = 0; (i5 < 2); i5 = (i5 + 1)) {
			fVec0[i5] = 0.0f;
			
		}
		for (int i6 = 0; (i6 < 2); i6 = (i6 + 1)) {
			fRec6[i6] = 0.0f;
			
		}
		for (int i7 = 0; (i7 < 2); i7 = (i7 + 1)) {
			fRec8[i7] = 0.0f;
			
		}
		for (int i8 = 0; (i8 < 2); i8 = (i8 + 1)) {
			fRec9[i8] = 0.0f;
			
		}
		for (int i9 = 0; (i9 < 2); i9 = (i9 + 1)) {
			fRec7[i9] = 0.0f;
			
		}
		for (int i10 = 0; (i10 < 2); i10 = (i10 + 1)) {
			fRec12[i10] = 0.0f;
			
		}
		for (int i11 = 0; (i11 < 2); i11 = (i11 + 1)) {
			fRec11[i11] = 0.0f;
			
		}
		for (int i12 = 0; (i12 < 2); i12 = (i12 + 1)) {
			fRec10[i12] = 0.0f;
			
		}
		for (int i13 = 0; (i13 < 2); i13 = (i13 + 1)) {
			fRec14[i13] = 0.0f;
			
		}
		for (int i14 = 0; (i14 < 2); i14 = (i14 + 1)) {
			fRec13[i14] = 0.0f;
			
		}
		for (int i15 = 0; (i15 < 2); i15 = (i15 + 1)) {
			fRec18[i15] = 0.0f;
			
		}
		for (int i16 = 0; (i16 < 2); i16 = (i16 + 1)) {
			fRec19[i16] = 0.0f;
			
		}
		for (int i17 = 0; (i17 < 2); i17 = (i17 + 1)) {
			fRec17[i17] = 0.0f;
			
		}
		for (int i18 = 0; (i18 < 2); i18 = (i18 + 1)) {
			fRec20[i18] = 0.0f;
			
		}
		for (int i19 = 0; (i19 < 2); i19 = (i19 + 1)) {
			fVec1[i19] = 0.0f;
			
		}
		for (int i20 = 0; (i20 < 2); i20 = (i20 + 1)) {
			fRec21[i20] = 0.0f;
			
		}
		for (int i21 = 0; (i21 < 2); i21 = (i21 + 1)) {
			fRec22[i21] = 0.0f;
			
		}
		for (int i22 = 0; (i22 < 2); i22 = (i22 + 1)) {
			fRec16[i22] = 0.0f;
			
		}
		for (int i23 = 0; (i23 < 2); i23 = (i23 + 1)) {
			fRec15[i23] = 0.0f;
			
		}
		for (int i24 = 0; (i24 < 2); i24 = (i24 + 1)) {
			fRec25[i24] = 0.0f;
			
		}
		for (int i25 = 0; (i25 < 2); i25 = (i25 + 1)) {
			fRec26[i25] = 0.0f;
			
		}
		for (int i26 = 0; (i26 < 2); i26 = (i26 + 1)) {
			fRec24[i26] = 0.0f;
			
		}
		for (int i27 = 0; (i27 < 2); i27 = (i27 + 1)) {
			fRec27[i27] = 0.0f;
			
		}
		for (int i28 = 0; (i28 < 2); i28 = (i28 + 1)) {
			fRec23[i28] = 0.0f;
			
		}
		for (int i29 = 0; (i29 < 2); i29 = (i29 + 1)) {
			fRec0[i29] = 0.0f;
			
		}
		for (int i30 = 0; (i30 < 2); i30 = (i30 + 1)) {
			fRec28[i30] = 0.0f;
			
		}
		for (int i31 = 0; (i31 < 2); i31 = (i31 + 1)) {
			fRec29[i31] = 0.0f;
			
		}
		for (int i32 = 0; (i32 < 2); i32 = (i32 + 1)) {
			fRec30[i32] = 0.0f;
			
		}
		for (int i33 = 0; (i33 < 2); i33 = (i33 + 1)) {
			fRec31[i33] = 0.0f;
			
		}
		for (int i34 = 0; (i34 < 2); i34 = (i34 + 1)) {
			fRec32[i34] = 0.0f;
			
		}
		for (int i35 = 0; (i35 < 2); i35 = (i35 + 1)) {
			fRec33[i35] = 0.0f;
			
		}
		for (int i36 = 0; (i36 < 2); i36 = (i36 + 1)) {
			fRec34[i36] = 0.0f;
			
		}
		for (int i37 = 0; (i37 < 2); i37 = (i37 + 1)) {
			fRec35[i37] = 0.0f;
			
		}
		for (int i38 = 0; (i38 < 2); i38 = (i38 + 1)) {
			fRec36[i38] = 0.0f;
			
		}
		for (int i39 = 0; (i39 < 2); i39 = (i39 + 1)) {
			fRec37[i39] = 0.0f;
			
		}
		for (int i40 = 0; (i40 < 2); i40 = (i40 + 1)) {
			fRec38[i40] = 0.0f;
			
		}
		for (int i41 = 0; (i41 < 2); i41 = (i41 + 1)) {
			fRec39[i41] = 0.0f;
			
		}
		for (int i42 = 0; (i42 < 2); i42 = (i42 + 1)) {
			fRec40[i42] = 0.0f;
			
		}
		for (int i43 = 0; (i43 < 2); i43 = (i43 + 1)) {
			fRec41[i43] = 0.0f;
			
		}
		for (int i44 = 0; (i44 < 2); i44 = (i44 + 1)) {
			fRec42[i44] = 0.0f;
			
		}
		for (int i45 = 0; (i45 < 2); i45 = (i45 + 1)) {
			fRec43[i45] = 0.0f;
			
		}
		for (int i46 = 0; (i46 < 2); i46 = (i46 + 1)) {
			fRec44[i46] = 0.0f;
			
		}
		for (int i47 = 0; (i47 < 2); i47 = (i47 + 1)) {
			fRec45[i47] = 0.0f;
			
		}
		for (int i48 = 0; (i48 < 2); i48 = (i48 + 1)) {
			fRec46[i48] = 0.0f;
			
		}
		for (int i49 = 0; (i49 < 2); i49 = (i49 + 1)) {
			fRec47[i49] = 0.0f;
			
		}
		for (int i50 = 0; (i50 < 2); i50 = (i50 + 1)) {
			fRec48[i50] = 0.0f;
			
		}
		for (int i51 = 0; (i51 < 2); i51 = (i51 + 1)) {
			fRec49[i51] = 0.0f;
			
		}
		for (int i52 = 0; (i52 < 2); i52 = (i52 + 1)) {
			fRec50[i52] = 0.0f;
			
		}
		for (int i53 = 0; (i53 < 2); i53 = (i53 + 1)) {
			fRec51[i53] = 0.0f;
			
		}
		for (int i54 = 0; (i54 < 2); i54 = (i54 + 1)) {
			fRec52[i54] = 0.0f;
			
		}
		for (int i55 = 0; (i55 < 2); i55 = (i55 + 1)) {
			fRec53[i55] = 0.0f;
			
		}
		for (int i56 = 0; (i56 < 2); i56 = (i56 + 1)) {
			fRec54[i56] = 0.0f;
			
		}
		for (int i57 = 0; (i57 < 2); i57 = (i57 + 1)) {
			fRec55[i57] = 0.0f;
			
		}
		for (int i58 = 0; (i58 < 2); i58 = (i58 + 1)) {
			fRec56[i58] = 0.0f;
			
		}
		for (int i59 = 0; (i59 < 2); i59 = (i59 + 1)) {
			fRec57[i59] = 0.0f;
			
		}
		for (int i60 = 0; (i60 < 2); i60 = (i60 + 1)) {
			fRec58[i60] = 0.0f;
			
		}
		for (int i61 = 0; (i61 < 2); i61 = (i61 + 1)) {
			fRec59[i61] = 0.0f;
			
		}
		for (int i62 = 0; (i62 < 2); i62 = (i62 + 1)) {
			fRec60[i62] = 0.0f;
			
		}
		for (int i63 = 0; (i63 < 2); i63 = (i63 + 1)) {
			fRec61[i63] = 0.0f;
			
		}
		for (int i64 = 0; (i64 < 2); i64 = (i64 + 1)) {
			fRec62[i64] = 0.0f;
			
		}
		for (int i65 = 0; (i65 < 2); i65 = (i65 + 1)) {
			fRec63[i65] = 0.0f;
			
		}
		for (int i66 = 0; (i66 < 2); i66 = (i66 + 1)) {
			fRec64[i66] = 0.0f;
			
		}
		for (int i67 = 0; (i67 < 2); i67 = (i67 + 1)) {
			fRec65[i67] = 0.0f;
			
		}
		for (int i68 = 0; (i68 < 2); i68 = (i68 + 1)) {
			fRec66[i68] = 0.0f;
			
		}
		for (int i69 = 0; (i69 < 2); i69 = (i69 + 1)) {
			fRec67[i69] = 0.0f;
			
		}
		for (int i70 = 0; (i70 < 2); i70 = (i70 + 1)) {
			fRec68[i70] = 0.0f;
			
		}
		for (int i71 = 0; (i71 < 2); i71 = (i71 + 1)) {
			fRec69[i71] = 0.0f;
			
		}
		for (int i72 = 0; (i72 < 2); i72 = (i72 + 1)) {
			fRec70[i72] = 0.0f;
			
		}
		for (int i73 = 0; (i73 < 2); i73 = (i73 + 1)) {
			fRec71[i73] = 0.0f;
			
		}
		for (int i74 = 0; (i74 < 2); i74 = (i74 + 1)) {
			fRec72[i74] = 0.0f;
			
		}
		for (int i75 = 0; (i75 < 2); i75 = (i75 + 1)) {
			fRec73[i75] = 0.0f;
			
		}
		for (int i76 = 0; (i76 < 2); i76 = (i76 + 1)) {
			fRec74[i76] = 0.0f;
			
		}
		for (int i77 = 0; (i77 < 2); i77 = (i77 + 1)) {
			fRec75[i77] = 0.0f;
			
		}
		for (int i78 = 0; (i78 < 2); i78 = (i78 + 1)) {
			fRec76[i78] = 0.0f;
			
		}
		
	}
	
	virtual void init(int samplingFreq) {
		classInit(samplingFreq);
		instanceInit(samplingFreq);
	}
	virtual void instanceInit(int samplingFreq) {
		instanceConstants(samplingFreq);
		instanceResetUserInterface();
		instanceClear();
	}
	
	virtual mydsp* clone() {
		return new mydsp();
	}
	
	virtual int getSampleRate() {
		return fSamplingFreq;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("0x00");
		ui_interface->openHorizontalBox("0x00");
		ui_interface->declare(&fHslider3, "0+1", "");
		ui_interface->declare(&fHslider3, "osc", "/gain_0 -20 20");
		ui_interface->declare(&fHslider3, "style", "knob");
		ui_interface->addHorizontalSlider("Gain  0", &fHslider3, 0.0f, -30.0f, 20.0f, 0.100000001f);
		ui_interface->declare(&fHslider2, "0+2", "");
		ui_interface->declare(&fHslider2, "osc", "/radius_0 0.5 50");
		ui_interface->declare(&fHslider2, "style", "knob");
		ui_interface->addHorizontalSlider("Radius  0", &fHslider2, 2.0f, 0.5f, 50.0f, 0.00999999978f);
		ui_interface->declare(&fHslider0, "0+3", "");
		ui_interface->declare(&fHslider0, "osc", "/azimuth_0 0 360");
		ui_interface->declare(&fHslider0, "style", "knob");
		ui_interface->addHorizontalSlider("Azimuth  0", &fHslider0, 0.0f, -3.14159274f, 3.14159274f, 0.100000001f);
		ui_interface->declare(&fHslider1, "0+4", "");
		ui_interface->declare(&fHslider1, "osc", "/elevation_0 -90 90");
		ui_interface->declare(&fHslider1, "style", "knob");
		ui_interface->addHorizontalSlider("Elevation  0", &fHslider1, 0.0f, -1.57079637f, 1.57079637f, 0.100000001f);
		ui_interface->declare(0, "0+5", "");
		ui_interface->openHorizontalBox("Spherical Wave");
		ui_interface->addCheckButton("Yes",&fCheckbox1);
		ui_interface->closeBox();
		ui_interface->declare(0, "2", "");
		ui_interface->openVerticalBox("Mute Order");
		ui_interface->addCheckButton("0",&fCheckbox4);
		ui_interface->addCheckButton("1",&fCheckbox2);
		ui_interface->addCheckButton("2",&fCheckbox0);
		ui_interface->addCheckButton("3",&fCheckbox3);
		ui_interface->closeBox();
		ui_interface->declare(&fEntry0, "~", "");
		ui_interface->addNumEntry("Speaker Radius", &fEntry0, 1.07000005f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->declare(0, "~", "");
		ui_interface->openVerticalBox("Outputs");
		ui_interface->declare(0, "~", "");
		ui_interface->openHorizontalBox("1-25");
		ui_interface->openVerticalBox("1");
		ui_interface->declare(&fVbargraph0, "osc", "/output1");
		ui_interface->declare(&fVbargraph0, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3ab5440", &fVbargraph0, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("2");
		ui_interface->declare(&fVbargraph1, "osc", "/output2");
		ui_interface->declare(&fVbargraph1, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3abdf20", &fVbargraph1, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("3");
		ui_interface->declare(&fVbargraph2, "osc", "/output3");
		ui_interface->declare(&fVbargraph2, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3ac76d0", &fVbargraph2, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("4");
		ui_interface->declare(&fVbargraph3, "osc", "/output4");
		ui_interface->declare(&fVbargraph3, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3ad0f60", &fVbargraph3, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("5");
		ui_interface->declare(&fVbargraph4, "osc", "/output5");
		ui_interface->declare(&fVbargraph4, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3ada8d0", &fVbargraph4, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("6");
		ui_interface->declare(&fVbargraph5, "osc", "/output6");
		ui_interface->declare(&fVbargraph5, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3ae2940", &fVbargraph5, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("7");
		ui_interface->declare(&fVbargraph6, "osc", "/output7");
		ui_interface->declare(&fVbargraph6, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3aeb7e0", &fVbargraph6, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("8");
		ui_interface->declare(&fVbargraph7, "osc", "/output8");
		ui_interface->declare(&fVbargraph7, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3af3e00", &fVbargraph7, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("9");
		ui_interface->declare(&fVbargraph8, "osc", "/output9");
		ui_interface->declare(&fVbargraph8, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3afc500", &fVbargraph8, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("10");
		ui_interface->declare(&fVbargraph9, "osc", "/output10");
		ui_interface->declare(&fVbargraph9, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3b04ce0", &fVbargraph9, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("11");
		ui_interface->declare(&fVbargraph10, "osc", "/output11");
		ui_interface->declare(&fVbargraph10, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3b0eb90", &fVbargraph10, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("12");
		ui_interface->declare(&fVbargraph11, "osc", "/output12");
		ui_interface->declare(&fVbargraph11, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3b18b20", &fVbargraph11, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("13");
		ui_interface->declare(&fVbargraph12, "osc", "/output13");
		ui_interface->declare(&fVbargraph12, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3b22b90", &fVbargraph12, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("14");
		ui_interface->declare(&fVbargraph13, "osc", "/output14");
		ui_interface->declare(&fVbargraph13, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3b2cce0", &fVbargraph13, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("15");
		ui_interface->declare(&fVbargraph14, "osc", "/output15");
		ui_interface->declare(&fVbargraph14, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3b35530", &fVbargraph14, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("16");
		ui_interface->declare(&fVbargraph15, "osc", "/output16");
		ui_interface->declare(&fVbargraph15, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3b3de60", &fVbargraph15, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("17");
		ui_interface->declare(&fVbargraph16, "osc", "/output17");
		ui_interface->declare(&fVbargraph16, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3b46870", &fVbargraph16, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("18");
		ui_interface->declare(&fVbargraph17, "osc", "/output18");
		ui_interface->declare(&fVbargraph17, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3b4f360", &fVbargraph17, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("19");
		ui_interface->declare(&fVbargraph18, "osc", "/output19");
		ui_interface->declare(&fVbargraph18, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3b58c80", &fVbargraph18, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("20");
		ui_interface->declare(&fVbargraph19, "osc", "/output20");
		ui_interface->declare(&fVbargraph19, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3b61d20", &fVbargraph19, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("21");
		ui_interface->declare(&fVbargraph20, "osc", "/output21");
		ui_interface->declare(&fVbargraph20, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3b6aea0", &fVbargraph20, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("22");
		ui_interface->declare(&fVbargraph21, "osc", "/output22");
		ui_interface->declare(&fVbargraph21, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3b74100", &fVbargraph21, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("23");
		ui_interface->declare(&fVbargraph22, "osc", "/output23");
		ui_interface->declare(&fVbargraph22, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3b7d050", &fVbargraph22, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("24");
		ui_interface->declare(&fVbargraph23, "osc", "/output24");
		ui_interface->declare(&fVbargraph23, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3b86080", &fVbargraph23, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("25");
		ui_interface->declare(&fVbargraph24, "osc", "/output25");
		ui_interface->declare(&fVbargraph24, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3b8f190", &fVbargraph24, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->declare(0, "~", "");
		ui_interface->openHorizontalBox("26-50");
		ui_interface->openVerticalBox("26");
		ui_interface->declare(&fVbargraph25, "osc", "/output26");
		ui_interface->declare(&fVbargraph25, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3b98380", &fVbargraph25, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("27");
		ui_interface->declare(&fVbargraph26, "osc", "/output27");
		ui_interface->declare(&fVbargraph26, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3ba1950", &fVbargraph26, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("28");
		ui_interface->declare(&fVbargraph27, "osc", "/output28");
		ui_interface->declare(&fVbargraph27, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3ba9bf0", &fVbargraph27, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("29");
		ui_interface->declare(&fVbargraph28, "osc", "/output29");
		ui_interface->declare(&fVbargraph28, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3bb1f70", &fVbargraph28, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("30");
		ui_interface->declare(&fVbargraph29, "osc", "/output30");
		ui_interface->declare(&fVbargraph29, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3bba3d0", &fVbargraph29, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("31");
		ui_interface->declare(&fVbargraph30, "osc", "/output31");
		ui_interface->declare(&fVbargraph30, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3bc3a70", &fVbargraph30, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("32");
		ui_interface->declare(&fVbargraph31, "osc", "/output32");
		ui_interface->declare(&fVbargraph31, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3bcca60", &fVbargraph31, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("33");
		ui_interface->declare(&fVbargraph32, "osc", "/output33");
		ui_interface->declare(&fVbargraph32, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3bd5b30", &fVbargraph32, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("34");
		ui_interface->declare(&fVbargraph33, "osc", "/output34");
		ui_interface->declare(&fVbargraph33, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3bdece0", &fVbargraph33, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("35");
		ui_interface->declare(&fVbargraph34, "osc", "/output35");
		ui_interface->declare(&fVbargraph34, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3be7f70", &fVbargraph34, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("36");
		ui_interface->declare(&fVbargraph35, "osc", "/output36");
		ui_interface->declare(&fVbargraph35, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3bf12e0", &fVbargraph35, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("37");
		ui_interface->declare(&fVbargraph36, "osc", "/output37");
		ui_interface->declare(&fVbargraph36, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3bfa730", &fVbargraph36, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("38");
		ui_interface->declare(&fVbargraph37, "osc", "/output38");
		ui_interface->declare(&fVbargraph37, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3c03c60", &fVbargraph37, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("39");
		ui_interface->declare(&fVbargraph38, "osc", "/output39");
		ui_interface->declare(&fVbargraph38, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3c0c4b0", &fVbargraph38, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("40");
		ui_interface->declare(&fVbargraph39, "osc", "/output40");
		ui_interface->declare(&fVbargraph39, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3c14de0", &fVbargraph39, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("41");
		ui_interface->declare(&fVbargraph40, "osc", "/output41");
		ui_interface->declare(&fVbargraph40, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3c1d7f0", &fVbargraph40, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("42");
		ui_interface->declare(&fVbargraph41, "osc", "/output42");
		ui_interface->declare(&fVbargraph41, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3c262e0", &fVbargraph41, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("43");
		ui_interface->declare(&fVbargraph42, "osc", "/output43");
		ui_interface->declare(&fVbargraph42, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3c2eeb0", &fVbargraph42, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("44");
		ui_interface->declare(&fVbargraph43, "osc", "/output44");
		ui_interface->declare(&fVbargraph43, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3c37b60", &fVbargraph43, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("45");
		ui_interface->declare(&fVbargraph44, "osc", "/output45");
		ui_interface->declare(&fVbargraph44, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3c408f0", &fVbargraph44, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("46");
		ui_interface->declare(&fVbargraph45, "osc", "/output46");
		ui_interface->declare(&fVbargraph45, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3c49760", &fVbargraph45, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("47");
		ui_interface->declare(&fVbargraph46, "osc", "/output47");
		ui_interface->declare(&fVbargraph46, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3c526b0", &fVbargraph46, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("48");
		ui_interface->declare(&fVbargraph47, "osc", "/output48");
		ui_interface->declare(&fVbargraph47, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3c5b6e0", &fVbargraph47, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("49");
		ui_interface->declare(&fVbargraph48, "osc", "/output49");
		ui_interface->declare(&fVbargraph48, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3c647f0", &fVbargraph48, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("50");
		ui_interface->declare(&fVbargraph49, "osc", "/output50");
		ui_interface->declare(&fVbargraph49, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3c6d9e0", &fVbargraph49, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->closeBox();
		
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		FAUSTFLOAT* output2 = outputs[2];
		FAUSTFLOAT* output3 = outputs[3];
		FAUSTFLOAT* output4 = outputs[4];
		FAUSTFLOAT* output5 = outputs[5];
		FAUSTFLOAT* output6 = outputs[6];
		FAUSTFLOAT* output7 = outputs[7];
		FAUSTFLOAT* output8 = outputs[8];
		FAUSTFLOAT* output9 = outputs[9];
		FAUSTFLOAT* output10 = outputs[10];
		FAUSTFLOAT* output11 = outputs[11];
		FAUSTFLOAT* output12 = outputs[12];
		FAUSTFLOAT* output13 = outputs[13];
		FAUSTFLOAT* output14 = outputs[14];
		FAUSTFLOAT* output15 = outputs[15];
		FAUSTFLOAT* output16 = outputs[16];
		FAUSTFLOAT* output17 = outputs[17];
		FAUSTFLOAT* output18 = outputs[18];
		FAUSTFLOAT* output19 = outputs[19];
		FAUSTFLOAT* output20 = outputs[20];
		FAUSTFLOAT* output21 = outputs[21];
		FAUSTFLOAT* output22 = outputs[22];
		FAUSTFLOAT* output23 = outputs[23];
		FAUSTFLOAT* output24 = outputs[24];
		FAUSTFLOAT* output25 = outputs[25];
		FAUSTFLOAT* output26 = outputs[26];
		FAUSTFLOAT* output27 = outputs[27];
		FAUSTFLOAT* output28 = outputs[28];
		FAUSTFLOAT* output29 = outputs[29];
		FAUSTFLOAT* output30 = outputs[30];
		FAUSTFLOAT* output31 = outputs[31];
		FAUSTFLOAT* output32 = outputs[32];
		FAUSTFLOAT* output33 = outputs[33];
		FAUSTFLOAT* output34 = outputs[34];
		FAUSTFLOAT* output35 = outputs[35];
		FAUSTFLOAT* output36 = outputs[36];
		FAUSTFLOAT* output37 = outputs[37];
		FAUSTFLOAT* output38 = outputs[38];
		FAUSTFLOAT* output39 = outputs[39];
		FAUSTFLOAT* output40 = outputs[40];
		FAUSTFLOAT* output41 = outputs[41];
		FAUSTFLOAT* output42 = outputs[42];
		FAUSTFLOAT* output43 = outputs[43];
		FAUSTFLOAT* output44 = outputs[44];
		FAUSTFLOAT* output45 = outputs[45];
		FAUSTFLOAT* output46 = outputs[46];
		FAUSTFLOAT* output47 = outputs[47];
		FAUSTFLOAT* output48 = outputs[48];
		FAUSTFLOAT* output49 = outputs[49];
		float fSlow0 = float(fHslider0);
		float fSlow1 = cosf(fSlow0);
		float fSlow2 = float(fHslider1);
		float fSlow3 = cosf(fSlow2);
		float fSlow4 = (fSlow1 * fSlow3);
		float fSlow5 = (6.12323426e-17f * fSlow4);
		float fSlow6 = sinf(fSlow2);
		float fSlow7 = (fSlow5 + fSlow6);
		float fSlow8 = faustpower2_f(fSlow7);
		float fSlow9 = (1.0f - float(fCheckbox0));
		float fSlow10 = (2.5f * (((3.0f * fSlow8) + -1.0f) * fSlow9));
		float fSlow11 = float(fEntry0);
		float fSlow12 = (fConst3 / fSlow11);
		float fSlow13 = faustpower2_f(fSlow12);
		float fSlow14 = (1.0f / (((fConst2 / fSlow11) + (3.0f * fSlow13)) + 1.0f));
		float fSlow15 = float(fHslider2);
		float fSlow16 = (fConst3 / fSlow15);
		float fSlow17 = faustpower2_f(fSlow16);
		float fSlow18 = (((fConst2 / fSlow15) + (3.0f * fSlow17)) + 1.0f);
		float fSlow19 = float(fCheckbox1);
		float fSlow20 = (((fSlow18 * fSlow19) * fSlow11) / fSlow15);
		float fSlow21 = (0.00100000005f * powf(10.0f, (0.0500000007f * float(fHslider3))));
		float fSlow22 = (12.0f * fSlow13);
		float fSlow23 = (fSlow22 + (fConst4 / fSlow11));
		float fSlow24 = (1.0f / fSlow18);
		float fSlow25 = (12.0f * fSlow17);
		float fSlow26 = ((fConst4 / fSlow15) + fSlow25);
		float fSlow27 = ((1.0f - fSlow19) * fSlow11);
		float fSlow28 = (1.0f - float(fCheckbox2));
		float fSlow29 = (3.0f * fSlow28);
		float fSlow30 = (fSlow16 + 1.0f);
		float fSlow31 = (fConst5 / (fSlow30 * fSlow15));
		float fSlow32 = (1.0f / (fSlow12 + 1.0f));
		float fSlow33 = (((fSlow30 * fSlow19) * fSlow11) / fSlow15);
		float fSlow34 = (fConst5 / fSlow11);
		float fSlow35 = (1.0f - float(fCheckbox3));
		float fSlow36 = (3.5f * (((5.0f * fSlow8) + -3.0f) * fSlow35));
		float fSlow37 = ((fConst7 / fSlow15) + 1.0f);
		float fSlow38 = (fConst6 / (fSlow37 * fSlow15));
		float fSlow39 = (1.0f / ((fConst7 / fSlow11) + 1.0f));
		float fSlow40 = (1.0f / (((fConst8 / fSlow11) + (6.4594326f * fSlow13)) + 1.0f));
		float fSlow41 = (((fConst8 / fSlow15) + (6.4594326f * fSlow17)) + 1.0f);
		float fSlow42 = (((fSlow41 * fSlow19) * fSlow11) / fSlow15);
		float fSlow43 = (25.8377304f * fSlow13);
		float fSlow44 = (fSlow43 + (fConst9 / fSlow11));
		float fSlow45 = (1.0f / fSlow41);
		float fSlow46 = (25.8377304f * fSlow17);
		float fSlow47 = ((fConst9 / fSlow15) + fSlow46);
		float fSlow48 = (fConst6 / fSlow11);
		float fSlow49 = ((((((1.0f / fSlow15) + -1.0f) * fSlow19) + 1.0f) * fSlow11) * (1.0f - float(fCheckbox4)));
		float fSlow50 = (2.5f * (((3.0f * faustpower2_f(fSlow4)) + -1.0f) * fSlow9));
		float fSlow51 = (3.0f * ((fSlow1 * fSlow28) * fSlow3));
		float fSlow52 = (3.5f * (((5.0f * faustpower3_f(fSlow4)) - (3.0f * fSlow4)) * fSlow35));
		float fSlow53 = cosf((fSlow0 + -1.57079637f));
		float fSlow54 = (fSlow53 * fSlow3);
		float fSlow55 = (2.5f * (((3.0f * faustpower2_f(fSlow54)) + -1.0f) * fSlow9));
		float fSlow56 = (3.0f * ((fSlow53 * fSlow28) * fSlow3));
		float fSlow57 = (3.5f * (((5.0f * faustpower3_f(fSlow54)) - (3.0f * fSlow54)) * fSlow35));
		float fSlow58 = cosf((fSlow0 + -3.14159274f));
		float fSlow59 = (fSlow58 * fSlow3);
		float fSlow60 = (2.5f * (((3.0f * faustpower2_f(fSlow59)) + -1.0f) * fSlow9));
		float fSlow61 = (3.0f * ((fSlow58 * fSlow28) * fSlow3));
		float fSlow62 = (3.5f * (((5.0f * faustpower3_f(fSlow59)) - (3.0f * fSlow59)) * fSlow35));
		float fSlow63 = cosf((fSlow0 + -4.71238899f));
		float fSlow64 = (fSlow63 * fSlow3);
		float fSlow65 = (2.5f * (((3.0f * faustpower2_f(fSlow64)) + -1.0f) * fSlow9));
		float fSlow66 = (3.0f * ((fSlow63 * fSlow28) * fSlow3));
		float fSlow67 = (3.5f * (((5.0f * faustpower3_f(fSlow64)) - (3.0f * fSlow64)) * fSlow35));
		float fSlow68 = (fSlow5 - fSlow6);
		float fSlow69 = faustpower2_f(fSlow68);
		float fSlow70 = (2.5f * (((3.0f * fSlow69) + -1.0f) * fSlow9));
		float fSlow71 = (3.5f * (((5.0f * fSlow69) + -3.0f) * fSlow35));
		float fSlow72 = (0.707106769f * fSlow4);
		float fSlow73 = (0.707106769f * fSlow6);
		float fSlow74 = (fSlow72 + fSlow73);
		float fSlow75 = faustpower2_f(fSlow74);
		float fSlow76 = (2.5f * (((3.0f * fSlow75) + -1.0f) * fSlow9));
		float fSlow77 = (3.5f * (((5.0f * fSlow75) + -3.0f) * fSlow35));
		float fSlow78 = (0.707106769f * fSlow54);
		float fSlow79 = (fSlow73 + fSlow78);
		float fSlow80 = faustpower2_f(fSlow79);
		float fSlow81 = (2.5f * (((3.0f * fSlow80) + -1.0f) * fSlow9));
		float fSlow82 = (3.5f * (((5.0f * fSlow80) + -3.0f) * fSlow35));
		float fSlow83 = (0.707106769f * fSlow59);
		float fSlow84 = (fSlow73 + fSlow83);
		float fSlow85 = faustpower2_f(fSlow84);
		float fSlow86 = (2.5f * (((3.0f * fSlow85) + -1.0f) * fSlow9));
		float fSlow87 = (3.5f * (((5.0f * fSlow85) + -3.0f) * fSlow35));
		float fSlow88 = (0.707106769f * fSlow64);
		float fSlow89 = (fSlow73 + fSlow88);
		float fSlow90 = faustpower2_f(fSlow89);
		float fSlow91 = (2.5f * (((3.0f * fSlow90) + -1.0f) * fSlow9));
		float fSlow92 = (3.5f * (((5.0f * fSlow90) + -3.0f) * fSlow35));
		float fSlow93 = cosf((fSlow0 + -0.785398185f));
		float fSlow94 = (fSlow93 * fSlow3);
		float fSlow95 = (2.5f * (((3.0f * faustpower2_f(fSlow94)) + -1.0f) * fSlow9));
		float fSlow96 = (3.0f * ((fSlow93 * fSlow28) * fSlow3));
		float fSlow97 = (3.5f * (((5.0f * faustpower3_f(fSlow94)) - (3.0f * fSlow94)) * fSlow35));
		float fSlow98 = cosf((fSlow0 + -2.3561945f));
		float fSlow99 = (fSlow98 * fSlow3);
		float fSlow100 = (2.5f * (((3.0f * faustpower2_f(fSlow99)) + -1.0f) * fSlow9));
		float fSlow101 = (3.0f * ((fSlow98 * fSlow28) * fSlow3));
		float fSlow102 = (3.5f * (((5.0f * faustpower3_f(fSlow99)) - (3.0f * fSlow99)) * fSlow35));
		float fSlow103 = cosf((fSlow0 + -3.92699075f));
		float fSlow104 = (fSlow103 * fSlow3);
		float fSlow105 = (2.5f * (((3.0f * faustpower2_f(fSlow104)) + -1.0f) * fSlow9));
		float fSlow106 = (3.0f * ((fSlow103 * fSlow28) * fSlow3));
		float fSlow107 = (3.5f * (((5.0f * faustpower3_f(fSlow104)) - (3.0f * fSlow104)) * fSlow35));
		float fSlow108 = cosf((fSlow0 + -5.497787f));
		float fSlow109 = (fSlow108 * fSlow3);
		float fSlow110 = (2.5f * (((3.0f * faustpower2_f(fSlow109)) + -1.0f) * fSlow9));
		float fSlow111 = (3.0f * ((fSlow108 * fSlow28) * fSlow3));
		float fSlow112 = (3.5f * (((5.0f * faustpower3_f(fSlow109)) - (3.0f * fSlow109)) * fSlow35));
		float fSlow113 = (fSlow72 - fSlow73);
		float fSlow114 = faustpower2_f(fSlow113);
		float fSlow115 = (2.5f * (((3.0f * fSlow114) + -1.0f) * fSlow9));
		float fSlow116 = (3.5f * (((5.0f * fSlow114) + -3.0f) * fSlow35));
		float fSlow117 = (fSlow78 - fSlow73);
		float fSlow118 = faustpower2_f(fSlow117);
		float fSlow119 = (2.5f * (((3.0f * fSlow118) + -1.0f) * fSlow9));
		float fSlow120 = (3.5f * (((5.0f * fSlow118) + -3.0f) * fSlow35));
		float fSlow121 = (fSlow83 - fSlow73);
		float fSlow122 = faustpower2_f(fSlow121);
		float fSlow123 = (2.5f * (((3.0f * fSlow122) + -1.0f) * fSlow9));
		float fSlow124 = (3.5f * (((5.0f * fSlow122) + -3.0f) * fSlow35));
		float fSlow125 = (fSlow88 - fSlow73);
		float fSlow126 = faustpower2_f(fSlow125);
		float fSlow127 = (2.5f * (((3.0f * fSlow126) + -1.0f) * fSlow9));
		float fSlow128 = (3.5f * (((5.0f * fSlow126) + -3.0f) * fSlow35));
		float fSlow129 = (0.816496611f * fSlow94);
		float fSlow130 = (0.577350259f * fSlow6);
		float fSlow131 = (fSlow129 + fSlow130);
		float fSlow132 = faustpower2_f(fSlow131);
		float fSlow133 = (2.5f * (((3.0f * fSlow132) + -1.0f) * fSlow9));
		float fSlow134 = (3.5f * (((5.0f * fSlow132) + -3.0f) * fSlow35));
		float fSlow135 = (0.816496611f * fSlow99);
		float fSlow136 = (fSlow130 + fSlow135);
		float fSlow137 = faustpower2_f(fSlow136);
		float fSlow138 = (2.5f * (((3.0f * fSlow137) + -1.0f) * fSlow9));
		float fSlow139 = (3.5f * (((5.0f * fSlow137) + -3.0f) * fSlow35));
		float fSlow140 = (0.816496611f * fSlow104);
		float fSlow141 = (fSlow130 + fSlow140);
		float fSlow142 = faustpower2_f(fSlow141);
		float fSlow143 = (2.5f * (((3.0f * fSlow142) + -1.0f) * fSlow9));
		float fSlow144 = (3.5f * (((5.0f * fSlow142) + -3.0f) * fSlow35));
		float fSlow145 = (0.816496611f * fSlow109);
		float fSlow146 = (fSlow130 + fSlow145);
		float fSlow147 = faustpower2_f(fSlow146);
		float fSlow148 = (2.5f * (((3.0f * fSlow147) + -1.0f) * fSlow9));
		float fSlow149 = (3.5f * (((5.0f * fSlow147) + -3.0f) * fSlow35));
		float fSlow150 = (fSlow129 - fSlow130);
		float fSlow151 = faustpower2_f(fSlow150);
		float fSlow152 = (2.5f * (((3.0f * fSlow151) + -1.0f) * fSlow9));
		float fSlow153 = (3.5f * (((5.0f * fSlow151) + -3.0f) * fSlow35));
		float fSlow154 = (fSlow135 - fSlow130);
		float fSlow155 = faustpower2_f(fSlow154);
		float fSlow156 = (2.5f * (((3.0f * fSlow155) + -1.0f) * fSlow9));
		float fSlow157 = (3.5f * (((5.0f * fSlow155) + -3.0f) * fSlow35));
		float fSlow158 = (fSlow140 - fSlow130);
		float fSlow159 = faustpower2_f(fSlow158);
		float fSlow160 = (2.5f * (((3.0f * fSlow159) + -1.0f) * fSlow9));
		float fSlow161 = (3.5f * (((5.0f * fSlow159) + -3.0f) * fSlow35));
		float fSlow162 = (fSlow145 - fSlow130);
		float fSlow163 = faustpower2_f(fSlow162);
		float fSlow164 = (2.5f * (((3.0f * fSlow163) + -1.0f) * fSlow9));
		float fSlow165 = (3.5f * (((5.0f * fSlow163) + -3.0f) * fSlow35));
		float fSlow166 = (0.426401436f * (cosf((fSlow0 + -0.785398185f)) * fSlow3));
		float fSlow167 = (0.904534042f * fSlow6);
		float fSlow168 = (fSlow166 + fSlow167);
		float fSlow169 = faustpower2_f(fSlow168);
		float fSlow170 = (2.5f * (((3.0f * fSlow169) + -1.0f) * fSlow9));
		float fSlow171 = (3.5f * (((5.0f * fSlow169) + -3.0f) * fSlow35));
		float fSlow172 = (0.426401436f * fSlow99);
		float fSlow173 = (fSlow167 + fSlow172);
		float fSlow174 = faustpower2_f(fSlow173);
		float fSlow175 = (2.5f * (((3.0f * fSlow174) + -1.0f) * fSlow9));
		float fSlow176 = (3.5f * (((5.0f * fSlow174) + -3.0f) * fSlow35));
		float fSlow177 = (0.426401436f * fSlow104);
		float fSlow178 = (fSlow167 + fSlow177);
		float fSlow179 = faustpower2_f(fSlow178);
		float fSlow180 = (2.5f * (((3.0f * fSlow179) + -1.0f) * fSlow9));
		float fSlow181 = (3.5f * (((5.0f * fSlow179) + -3.0f) * fSlow35));
		float fSlow182 = (0.426401436f * fSlow109);
		float fSlow183 = (fSlow167 + fSlow182);
		float fSlow184 = faustpower2_f(fSlow183);
		float fSlow185 = (2.5f * (((3.0f * fSlow184) + -1.0f) * fSlow9));
		float fSlow186 = (3.5f * (((5.0f * fSlow184) + -3.0f) * fSlow35));
		float fSlow187 = (0.953462601f * (cosf((fSlow0 + -0.321750551f)) * fSlow3));
		float fSlow188 = (0.301511347f * fSlow6);
		float fSlow189 = (fSlow187 + fSlow188);
		float fSlow190 = faustpower2_f(fSlow189);
		float fSlow191 = (2.5f * (((3.0f * fSlow190) + -1.0f) * fSlow9));
		float fSlow192 = (3.5f * (((5.0f * fSlow190) + -3.0f) * fSlow35));
		float fSlow193 = (0.953462601f * (cosf((fSlow0 + -1.24904573f)) * fSlow3));
		float fSlow194 = (fSlow188 + fSlow193);
		float fSlow195 = faustpower2_f(fSlow194);
		float fSlow196 = (2.5f * (((3.0f * fSlow195) + -1.0f) * fSlow9));
		float fSlow197 = (3.5f * (((5.0f * fSlow195) + -3.0f) * fSlow35));
		float fSlow198 = (0.953462601f * (cosf((fSlow0 + -1.89254689f)) * fSlow3));
		float fSlow199 = (fSlow188 + fSlow198);
		float fSlow200 = faustpower2_f(fSlow199);
		float fSlow201 = (2.5f * (((3.0f * fSlow200) + -1.0f) * fSlow9));
		float fSlow202 = (3.5f * (((5.0f * fSlow200) + -3.0f) * fSlow35));
		float fSlow203 = (0.953462601f * (cosf((fSlow0 + -2.8198421f)) * fSlow3));
		float fSlow204 = (fSlow188 + fSlow203);
		float fSlow205 = faustpower2_f(fSlow204);
		float fSlow206 = (2.5f * (((3.0f * fSlow205) + -1.0f) * fSlow9));
		float fSlow207 = (3.5f * (((5.0f * fSlow205) + -3.0f) * fSlow35));
		float fSlow208 = (0.953462601f * (cosf((fSlow0 + -3.46334314f)) * fSlow3));
		float fSlow209 = (fSlow188 + fSlow208);
		float fSlow210 = faustpower2_f(fSlow209);
		float fSlow211 = (2.5f * (((3.0f * fSlow210) + -1.0f) * fSlow9));
		float fSlow212 = (3.5f * (((5.0f * fSlow210) + -3.0f) * fSlow35));
		float fSlow213 = (0.953462601f * (cosf((fSlow0 + -4.39063835f)) * fSlow3));
		float fSlow214 = (fSlow188 + fSlow213);
		float fSlow215 = faustpower2_f(fSlow214);
		float fSlow216 = (2.5f * (((3.0f * fSlow215) + -1.0f) * fSlow9));
		float fSlow217 = (3.5f * (((5.0f * fSlow215) + -3.0f) * fSlow35));
		float fSlow218 = (0.953462601f * (cosf((fSlow0 + -5.03413963f)) * fSlow3));
		float fSlow219 = (fSlow188 + fSlow218);
		float fSlow220 = faustpower2_f(fSlow219);
		float fSlow221 = (2.5f * (((3.0f * fSlow220) + -1.0f) * fSlow9));
		float fSlow222 = (3.5f * (((5.0f * fSlow220) + -3.0f) * fSlow35));
		float fSlow223 = (0.953462601f * (cosf((fSlow0 + -5.96143484f)) * fSlow3));
		float fSlow224 = (fSlow188 + fSlow223);
		float fSlow225 = faustpower2_f(fSlow224);
		float fSlow226 = (2.5f * (((3.0f * fSlow225) + -1.0f) * fSlow9));
		float fSlow227 = (3.5f * (((5.0f * fSlow225) + -3.0f) * fSlow35));
		float fSlow228 = (fSlow187 - fSlow188);
		float fSlow229 = faustpower2_f(fSlow228);
		float fSlow230 = (2.5f * (((3.0f * fSlow229) + -1.0f) * fSlow9));
		float fSlow231 = (3.5f * (((5.0f * fSlow229) + -3.0f) * fSlow35));
		float fSlow232 = (fSlow193 - fSlow188);
		float fSlow233 = faustpower2_f(fSlow232);
		float fSlow234 = (2.5f * (((3.0f * fSlow233) + -1.0f) * fSlow9));
		float fSlow235 = (3.5f * (((5.0f * fSlow233) + -3.0f) * fSlow35));
		float fSlow236 = (fSlow198 - fSlow188);
		float fSlow237 = faustpower2_f(fSlow236);
		float fSlow238 = (2.5f * (((3.0f * fSlow237) + -1.0f) * fSlow9));
		float fSlow239 = (3.5f * (((5.0f * fSlow237) + -3.0f) * fSlow35));
		float fSlow240 = (fSlow203 - fSlow188);
		float fSlow241 = faustpower2_f(fSlow240);
		float fSlow242 = (2.5f * (((3.0f * fSlow241) + -1.0f) * fSlow9));
		float fSlow243 = (3.5f * (((5.0f * fSlow241) + -3.0f) * fSlow35));
		float fSlow244 = (fSlow208 - fSlow188);
		float fSlow245 = faustpower2_f(fSlow244);
		float fSlow246 = (2.5f * (((3.0f * fSlow245) + -1.0f) * fSlow9));
		float fSlow247 = (3.5f * (((5.0f * fSlow245) + -3.0f) * fSlow35));
		float fSlow248 = (fSlow213 - fSlow188);
		float fSlow249 = faustpower2_f(fSlow248);
		float fSlow250 = (2.5f * (((3.0f * fSlow249) + -1.0f) * fSlow9));
		float fSlow251 = (3.5f * (((5.0f * fSlow249) + -3.0f) * fSlow35));
		float fSlow252 = (fSlow218 - fSlow188);
		float fSlow253 = faustpower2_f(fSlow252);
		float fSlow254 = (2.5f * (((3.0f * fSlow253) + -1.0f) * fSlow9));
		float fSlow255 = (3.5f * (((5.0f * fSlow253) + -3.0f) * fSlow35));
		float fSlow256 = (fSlow223 - fSlow188);
		float fSlow257 = faustpower2_f(fSlow256);
		float fSlow258 = (2.5f * (((3.0f * fSlow257) + -1.0f) * fSlow9));
		float fSlow259 = (3.5f * (((5.0f * fSlow257) + -3.0f) * fSlow35));
		float fSlow260 = (fSlow166 - fSlow167);
		float fSlow261 = faustpower2_f(fSlow260);
		float fSlow262 = (2.5f * (((3.0f * fSlow261) + -1.0f) * fSlow9));
		float fSlow263 = (3.5f * (((5.0f * fSlow261) + -3.0f) * fSlow35));
		float fSlow264 = (fSlow172 - fSlow167);
		float fSlow265 = faustpower2_f(fSlow264);
		float fSlow266 = (2.5f * (((3.0f * fSlow265) + -1.0f) * fSlow9));
		float fSlow267 = (3.5f * (((5.0f * fSlow265) + -3.0f) * fSlow35));
		float fSlow268 = (fSlow177 - fSlow167);
		float fSlow269 = faustpower2_f(fSlow268);
		float fSlow270 = (2.5f * (((3.0f * fSlow269) + -1.0f) * fSlow9));
		float fSlow271 = (3.5f * (((5.0f * fSlow269) + -3.0f) * fSlow35));
		float fSlow272 = (fSlow182 - fSlow167);
		float fSlow273 = faustpower2_f(fSlow272);
		float fSlow274 = (2.5f * (((3.0f * fSlow273) + -1.0f) * fSlow9));
		float fSlow275 = (3.5f * (((5.0f * fSlow273) + -3.0f) * fSlow35));
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec2[0] = (fSlow21 + (0.999000013f * fRec2[1]));
			float fTemp0 = (fRec2[0] * float(input0[i]));
			fRec3[0] = (fRec1[1] + fRec3[1]);
			fRec4[0] = (fRec3[1] + fRec4[1]);
			fRec1[0] = (fSlow14 * ((fSlow20 * fTemp0) - ((fSlow23 * fRec3[0]) + (fSlow22 * fRec4[0]))));
			fRec5[0] = (fRec1[0] + fRec5[1]);
			float fTemp1 = (fRec5[0] - fRec1[0]);
			fVec0[0] = fTemp1;
			fRec6[0] = (fRec6[1] + fVec0[1]);
			fRec8[0] = (fRec7[1] + fRec8[1]);
			fRec9[0] = (fRec8[1] + fRec9[1]);
			float fTemp2 = (fSlow27 * fTemp0);
			fRec7[0] = (0.0f - (fSlow14 * (((fSlow23 * fRec8[0]) + (fSlow22 * fRec9[0])) - fTemp2)));
			float fTemp3 = ((fRec1[0] + (fSlow24 * ((fSlow26 * fTemp1) + (fSlow25 * fRec6[0])))) + fRec7[0]);
			fRec12[0] = (fRec11[1] + fRec12[1]);
			fRec11[0] = (fSlow32 * ((fSlow33 * fTemp0) - (fSlow34 * fRec12[0])));
			fRec10[0] = (fRec11[0] + fRec10[1]);
			fRec14[0] = (fRec13[1] + fRec14[1]);
			fRec13[0] = (fSlow32 * (fTemp2 - (fSlow34 * fRec14[0])));
			float fTemp4 = ((fSlow31 * (fRec10[0] - fRec11[0])) + (fRec11[0] + fRec13[0]));
			float fTemp5 = (fSlow29 * fTemp4);
			fRec18[0] = (fRec17[1] + fRec18[1]);
			fRec19[0] = (fRec18[1] + fRec19[1]);
			fRec17[0] = (fSlow40 * ((fSlow42 * fTemp0) - ((fSlow44 * fRec18[0]) + (fSlow43 * fRec19[0]))));
			fRec20[0] = (fRec17[0] + fRec20[1]);
			float fTemp6 = (fRec20[0] - fRec17[0]);
			fVec1[0] = fTemp6;
			fRec21[0] = (fRec21[1] + fVec1[1]);
			fRec22[0] = (fRec16[1] + fRec22[1]);
			fRec16[0] = (fSlow39 * ((fSlow37 * (fRec17[0] + (fSlow45 * ((fSlow47 * fTemp6) + (fSlow46 * fRec21[0]))))) - (fSlow48 * fRec22[0])));
			fRec15[0] = (fRec16[0] + fRec15[1]);
			fRec25[0] = (fRec24[1] + fRec25[1]);
			fRec26[0] = (fRec25[1] + fRec26[1]);
			fRec24[0] = (0.0f - (fSlow40 * (((fSlow44 * fRec25[0]) + (fSlow43 * fRec26[0])) - fTemp2)));
			fRec27[0] = (fRec23[1] + fRec27[1]);
			fRec23[0] = (fSlow39 * (fRec24[0] - (fSlow48 * fRec27[0])));
			float fTemp7 = ((fSlow38 * (fRec15[0] - fRec16[0])) + (fRec16[0] + fRec23[0]));
			float fTemp8 = (fSlow49 * fTemp0);
			float fTemp9 = (0.0126984129f * ((fSlow10 * fTemp3) + ((fSlow7 * (fTemp5 + (fSlow36 * fTemp7))) + fTemp8)));
			fRec0[0] = max((fRec0[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp9))))));
			fVbargraph0 = FAUSTFLOAT(fRec0[0]);
			output0[i] = FAUSTFLOAT(fTemp9);
			float fTemp10 = (0.0126984129f * (((fSlow50 * fTemp3) + (fTemp8 + (fSlow51 * fTemp4))) + (fSlow52 * fTemp7)));
			fRec28[0] = max((fRec28[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp10))))));
			fVbargraph1 = FAUSTFLOAT(fRec28[0]);
			output1[i] = FAUSTFLOAT(fTemp10);
			float fTemp11 = (0.0126984129f * (((fSlow55 * fTemp3) + (fTemp8 + (fSlow56 * fTemp4))) + (fSlow57 * fTemp7)));
			fRec29[0] = max((fRec29[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp11))))));
			fVbargraph2 = FAUSTFLOAT(fRec29[0]);
			output2[i] = FAUSTFLOAT(fTemp11);
			float fTemp12 = (0.0126984129f * (((fSlow60 * fTemp3) + (fTemp8 + (fSlow61 * fTemp4))) + (fSlow62 * fTemp7)));
			fRec30[0] = max((fRec30[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp12))))));
			fVbargraph3 = FAUSTFLOAT(fRec30[0]);
			output3[i] = FAUSTFLOAT(fTemp12);
			float fTemp13 = (0.0126984129f * (((fSlow65 * fTemp3) + (fTemp8 + (fSlow66 * fTemp4))) + (fSlow67 * fTemp7)));
			fRec31[0] = max((fRec31[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp13))))));
			fVbargraph4 = FAUSTFLOAT(fRec31[0]);
			output4[i] = FAUSTFLOAT(fTemp13);
			float fTemp14 = (0.0126984129f * ((fSlow70 * fTemp3) + (fTemp8 + (fSlow68 * (fTemp5 + (fSlow71 * fTemp7))))));
			fRec32[0] = max((fRec32[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp14))))));
			fVbargraph5 = FAUSTFLOAT(fRec32[0]);
			output5[i] = FAUSTFLOAT(fTemp14);
			float fTemp15 = (0.0225749556f * ((fSlow76 * fTemp3) + (fTemp8 + (fSlow74 * (fTemp5 + (fSlow77 * fTemp7))))));
			fRec33[0] = max((fRec33[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp15))))));
			fVbargraph6 = FAUSTFLOAT(fRec33[0]);
			output6[i] = FAUSTFLOAT(fTemp15);
			float fTemp16 = (0.0225749556f * ((fSlow81 * fTemp3) + (fTemp8 + (fSlow79 * (fTemp5 + (fSlow82 * fTemp7))))));
			fRec34[0] = max((fRec34[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp16))))));
			fVbargraph7 = FAUSTFLOAT(fRec34[0]);
			output7[i] = FAUSTFLOAT(fTemp16);
			float fTemp17 = (0.0225749556f * ((fSlow86 * fTemp3) + (fTemp8 + (fSlow84 * (fTemp5 + (fSlow87 * fTemp7))))));
			fRec35[0] = max((fRec35[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp17))))));
			fVbargraph8 = FAUSTFLOAT(fRec35[0]);
			output8[i] = FAUSTFLOAT(fTemp17);
			float fTemp18 = (0.0225749556f * ((fSlow91 * fTemp3) + (fTemp8 + (fSlow89 * (fTemp5 + (fSlow92 * fTemp7))))));
			fRec36[0] = max((fRec36[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp18))))));
			fVbargraph9 = FAUSTFLOAT(fRec36[0]);
			output9[i] = FAUSTFLOAT(fTemp18);
			float fTemp19 = (0.0225749556f * (((fSlow95 * fTemp3) + (fTemp8 + (fSlow96 * fTemp4))) + (fSlow97 * fTemp7)));
			fRec37[0] = max((fRec37[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp19))))));
			fVbargraph10 = FAUSTFLOAT(fRec37[0]);
			output10[i] = FAUSTFLOAT(fTemp19);
			float fTemp20 = (0.0225749556f * (((fSlow100 * fTemp3) + (fTemp8 + (fSlow101 * fTemp4))) + (fSlow102 * fTemp7)));
			fRec38[0] = max((fRec38[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp20))))));
			fVbargraph11 = FAUSTFLOAT(fRec38[0]);
			output11[i] = FAUSTFLOAT(fTemp20);
			float fTemp21 = (0.0225749556f * (((fSlow105 * fTemp3) + (fTemp8 + (fSlow106 * fTemp4))) + (fSlow107 * fTemp7)));
			fRec39[0] = max((fRec39[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp21))))));
			fVbargraph12 = FAUSTFLOAT(fRec39[0]);
			output12[i] = FAUSTFLOAT(fTemp21);
			float fTemp22 = (0.0225749556f * (((fSlow110 * fTemp3) + (fTemp8 + (fSlow111 * fTemp4))) + (fSlow112 * fTemp7)));
			fRec40[0] = max((fRec40[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp22))))));
			fVbargraph13 = FAUSTFLOAT(fRec40[0]);
			output13[i] = FAUSTFLOAT(fTemp22);
			float fTemp23 = (0.0225749556f * ((fSlow115 * fTemp3) + (fTemp8 + (fSlow113 * (fTemp5 + (fSlow116 * fTemp7))))));
			fRec41[0] = max((fRec41[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp23))))));
			fVbargraph14 = FAUSTFLOAT(fRec41[0]);
			output14[i] = FAUSTFLOAT(fTemp23);
			float fTemp24 = (0.0225749556f * ((fSlow119 * fTemp3) + (fTemp8 + (fSlow117 * (fTemp5 + (fSlow120 * fTemp7))))));
			fRec42[0] = max((fRec42[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp24))))));
			fVbargraph15 = FAUSTFLOAT(fRec42[0]);
			output15[i] = FAUSTFLOAT(fTemp24);
			float fTemp25 = (0.0225749556f * ((fSlow123 * fTemp3) + (fTemp8 + (fSlow121 * (fTemp5 + (fSlow124 * fTemp7))))));
			fRec43[0] = max((fRec43[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp25))))));
			fVbargraph16 = FAUSTFLOAT(fRec43[0]);
			output16[i] = FAUSTFLOAT(fTemp25);
			float fTemp26 = (0.0225749556f * ((fSlow127 * fTemp3) + (fTemp8 + (fSlow125 * (fTemp5 + (fSlow128 * fTemp7))))));
			fRec44[0] = max((fRec44[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp26))))));
			fVbargraph17 = FAUSTFLOAT(fRec44[0]);
			output17[i] = FAUSTFLOAT(fTemp26);
			float fTemp27 = (0.0210937504f * ((fSlow133 * fTemp3) + (fTemp8 + (fSlow131 * (fTemp5 + (fSlow134 * fTemp7))))));
			fRec45[0] = max((fRec45[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp27))))));
			fVbargraph18 = FAUSTFLOAT(fRec45[0]);
			output18[i] = FAUSTFLOAT(fTemp27);
			float fTemp28 = (0.0210937504f * ((fSlow138 * fTemp3) + (fTemp8 + (fSlow136 * (fTemp5 + (fSlow139 * fTemp7))))));
			fRec46[0] = max((fRec46[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp28))))));
			fVbargraph19 = FAUSTFLOAT(fRec46[0]);
			output19[i] = FAUSTFLOAT(fTemp28);
			float fTemp29 = (0.0210937504f * ((fSlow143 * fTemp3) + (fTemp8 + (fSlow141 * (fTemp5 + (fSlow144 * fTemp7))))));
			fRec47[0] = max((fRec47[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp29))))));
			fVbargraph20 = FAUSTFLOAT(fRec47[0]);
			output20[i] = FAUSTFLOAT(fTemp29);
			float fTemp30 = (0.0210937504f * ((fSlow148 * fTemp3) + (fTemp8 + (fSlow146 * (fTemp5 + (fSlow149 * fTemp7))))));
			fRec48[0] = max((fRec48[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp30))))));
			fVbargraph21 = FAUSTFLOAT(fRec48[0]);
			output21[i] = FAUSTFLOAT(fTemp30);
			float fTemp31 = (0.0210937504f * ((fSlow152 * fTemp3) + (fTemp8 + (fSlow150 * (fTemp5 + (fSlow153 * fTemp7))))));
			fRec49[0] = max((fRec49[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp31))))));
			fVbargraph22 = FAUSTFLOAT(fRec49[0]);
			output22[i] = FAUSTFLOAT(fTemp31);
			float fTemp32 = (0.0210937504f * ((fSlow156 * fTemp3) + (fTemp8 + (fSlow154 * (fTemp5 + (fSlow157 * fTemp7))))));
			fRec50[0] = max((fRec50[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp32))))));
			fVbargraph23 = FAUSTFLOAT(fRec50[0]);
			output23[i] = FAUSTFLOAT(fTemp32);
			float fTemp33 = (0.0210937504f * ((fSlow160 * fTemp3) + (fTemp8 + (fSlow158 * (fTemp5 + (fSlow161 * fTemp7))))));
			fRec51[0] = max((fRec51[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp33))))));
			fVbargraph24 = FAUSTFLOAT(fRec51[0]);
			output24[i] = FAUSTFLOAT(fTemp33);
			float fTemp34 = (0.0210937504f * ((fSlow164 * fTemp3) + (fTemp8 + (fSlow162 * (fTemp5 + (fSlow165 * fTemp7))))));
			fRec52[0] = max((fRec52[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp34))))));
			fVbargraph25 = FAUSTFLOAT(fRec52[0]);
			output25[i] = FAUSTFLOAT(fTemp34);
			float fTemp35 = (0.0201733354f * ((fSlow170 * fTemp3) + (fTemp8 + (fSlow168 * (fTemp5 + (fSlow171 * fTemp7))))));
			fRec53[0] = max((fRec53[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp35))))));
			fVbargraph26 = FAUSTFLOAT(fRec53[0]);
			output26[i] = FAUSTFLOAT(fTemp35);
			float fTemp36 = (0.0201733354f * ((fSlow175 * fTemp3) + (fTemp8 + (fSlow173 * (fTemp5 + (fSlow176 * fTemp7))))));
			fRec54[0] = max((fRec54[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp36))))));
			fVbargraph27 = FAUSTFLOAT(fRec54[0]);
			output27[i] = FAUSTFLOAT(fTemp36);
			float fTemp37 = (0.0201733354f * ((fSlow180 * fTemp3) + (fTemp8 + (fSlow178 * (fTemp5 + (fSlow181 * fTemp7))))));
			fRec55[0] = max((fRec55[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp37))))));
			fVbargraph28 = FAUSTFLOAT(fRec55[0]);
			output28[i] = FAUSTFLOAT(fTemp37);
			float fTemp38 = (0.0201733354f * ((fSlow185 * fTemp3) + (fTemp8 + (fSlow183 * (fTemp5 + (fSlow186 * fTemp7))))));
			fRec56[0] = max((fRec56[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp38))))));
			fVbargraph29 = FAUSTFLOAT(fRec56[0]);
			output29[i] = FAUSTFLOAT(fTemp38);
			float fTemp39 = (0.0201733354f * ((fSlow191 * fTemp3) + (fTemp8 + (fSlow189 * (fTemp5 + (fSlow192 * fTemp7))))));
			fRec57[0] = max((fRec57[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp39))))));
			fVbargraph30 = FAUSTFLOAT(fRec57[0]);
			output30[i] = FAUSTFLOAT(fTemp39);
			float fTemp40 = (0.0201733354f * ((fSlow196 * fTemp3) + (fTemp8 + (fSlow194 * (fTemp5 + (fSlow197 * fTemp7))))));
			fRec58[0] = max((fRec58[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp40))))));
			fVbargraph31 = FAUSTFLOAT(fRec58[0]);
			output31[i] = FAUSTFLOAT(fTemp40);
			float fTemp41 = (0.0201733354f * ((fSlow201 * fTemp3) + (fTemp8 + (fSlow199 * (fTemp5 + (fSlow202 * fTemp7))))));
			fRec59[0] = max((fRec59[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp41))))));
			fVbargraph32 = FAUSTFLOAT(fRec59[0]);
			output32[i] = FAUSTFLOAT(fTemp41);
			float fTemp42 = (0.0201733354f * ((fSlow206 * fTemp3) + (fTemp8 + (fSlow204 * (fTemp5 + (fSlow207 * fTemp7))))));
			fRec60[0] = max((fRec60[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp42))))));
			fVbargraph33 = FAUSTFLOAT(fRec60[0]);
			output33[i] = FAUSTFLOAT(fTemp42);
			float fTemp43 = (0.0201733354f * ((fSlow211 * fTemp3) + (fTemp8 + (fSlow209 * (fTemp5 + (fSlow212 * fTemp7))))));
			fRec61[0] = max((fRec61[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp43))))));
			fVbargraph34 = FAUSTFLOAT(fRec61[0]);
			output34[i] = FAUSTFLOAT(fTemp43);
			float fTemp44 = (0.0201733354f * ((fSlow216 * fTemp3) + (fTemp8 + (fSlow214 * (fTemp5 + (fSlow217 * fTemp7))))));
			fRec62[0] = max((fRec62[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp44))))));
			fVbargraph35 = FAUSTFLOAT(fRec62[0]);
			output35[i] = FAUSTFLOAT(fTemp44);
			float fTemp45 = (0.0201733354f * (((fSlow221 * fTemp3) + fTemp8) + (fSlow219 * (fTemp5 + (fSlow222 * fTemp7)))));
			fRec63[0] = max((fRec63[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp45))))));
			fVbargraph36 = FAUSTFLOAT(fRec63[0]);
			output36[i] = FAUSTFLOAT(fTemp45);
			float fTemp46 = (0.0201733354f * ((fSlow226 * fTemp3) + (fTemp8 + (fSlow224 * (fTemp5 + (fSlow227 * fTemp7))))));
			fRec64[0] = max((fRec64[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp46))))));
			fVbargraph37 = FAUSTFLOAT(fRec64[0]);
			output37[i] = FAUSTFLOAT(fTemp46);
			float fTemp47 = (0.0201733354f * ((fSlow230 * fTemp3) + (fTemp8 + (fSlow228 * (fTemp5 + (fSlow231 * fTemp7))))));
			fRec65[0] = max((fRec65[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp47))))));
			fVbargraph38 = FAUSTFLOAT(fRec65[0]);
			output38[i] = FAUSTFLOAT(fTemp47);
			float fTemp48 = (0.0201733354f * ((fSlow234 * fTemp3) + (fTemp8 + (fSlow232 * (fTemp5 + (fSlow235 * fTemp7))))));
			fRec66[0] = max((fRec66[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp48))))));
			fVbargraph39 = FAUSTFLOAT(fRec66[0]);
			output39[i] = FAUSTFLOAT(fTemp48);
			float fTemp49 = (0.0201733354f * ((fSlow238 * fTemp3) + (fTemp8 + (fSlow236 * (fTemp5 + (fSlow239 * fTemp7))))));
			fRec67[0] = max((fRec67[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp49))))));
			fVbargraph40 = FAUSTFLOAT(fRec67[0]);
			output40[i] = FAUSTFLOAT(fTemp49);
			float fTemp50 = (0.0201733354f * ((fSlow242 * fTemp3) + (fTemp8 + (fSlow240 * (fTemp5 + (fSlow243 * fTemp7))))));
			fRec68[0] = max((fRec68[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp50))))));
			fVbargraph41 = FAUSTFLOAT(fRec68[0]);
			output41[i] = FAUSTFLOAT(fTemp50);
			float fTemp51 = (0.0201733354f * ((fSlow246 * fTemp3) + (fTemp8 + (fSlow244 * (fTemp5 + (fSlow247 * fTemp7))))));
			fRec69[0] = max((fRec69[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp51))))));
			fVbargraph42 = FAUSTFLOAT(fRec69[0]);
			output42[i] = FAUSTFLOAT(fTemp51);
			float fTemp52 = (0.0201733354f * ((fSlow250 * fTemp3) + (fTemp8 + (fSlow248 * (fTemp5 + (fSlow251 * fTemp7))))));
			fRec70[0] = max((fRec70[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp52))))));
			fVbargraph43 = FAUSTFLOAT(fRec70[0]);
			output43[i] = FAUSTFLOAT(fTemp52);
			float fTemp53 = (0.0201733354f * ((fSlow254 * fTemp3) + (fTemp8 + (fSlow252 * (fTemp5 + (fSlow255 * fTemp7))))));
			fRec71[0] = max((fRec71[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp53))))));
			fVbargraph44 = FAUSTFLOAT(fRec71[0]);
			output44[i] = FAUSTFLOAT(fTemp53);
			float fTemp54 = (0.0201733354f * ((fSlow258 * fTemp3) + (fTemp8 + (fSlow256 * (fTemp5 + (fSlow259 * fTemp7))))));
			fRec72[0] = max((fRec72[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp54))))));
			fVbargraph45 = FAUSTFLOAT(fRec72[0]);
			output45[i] = FAUSTFLOAT(fTemp54);
			float fTemp55 = (0.0201733354f * ((fSlow262 * fTemp3) + (fTemp8 + (fSlow260 * (fTemp5 + (fSlow263 * fTemp7))))));
			fRec73[0] = max((fRec73[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp55))))));
			fVbargraph46 = FAUSTFLOAT(fRec73[0]);
			output46[i] = FAUSTFLOAT(fTemp55);
			float fTemp56 = (0.0201733354f * ((fSlow266 * fTemp3) + (fTemp8 + (fSlow264 * (fTemp5 + (fSlow267 * fTemp7))))));
			fRec74[0] = max((fRec74[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp56))))));
			fVbargraph47 = FAUSTFLOAT(fRec74[0]);
			output47[i] = FAUSTFLOAT(fTemp56);
			float fTemp57 = (0.0201733354f * ((fSlow270 * fTemp3) + (fTemp8 + (fSlow268 * (fTemp5 + (fSlow271 * fTemp7))))));
			fRec75[0] = max((fRec75[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp57))))));
			fVbargraph48 = FAUSTFLOAT(fRec75[0]);
			output48[i] = FAUSTFLOAT(fTemp57);
			float fTemp58 = (0.0201733354f * ((fSlow274 * fTemp3) + (fTemp8 + (fSlow272 * (fTemp5 + (fSlow275 * fTemp7))))));
			fRec76[0] = max((fRec76[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp58))))));
			fVbargraph49 = FAUSTFLOAT(fRec76[0]);
			output49[i] = FAUSTFLOAT(fTemp58);
			fRec2[1] = fRec2[0];
			fRec3[1] = fRec3[0];
			fRec4[1] = fRec4[0];
			fRec1[1] = fRec1[0];
			fRec5[1] = fRec5[0];
			fVec0[1] = fVec0[0];
			fRec6[1] = fRec6[0];
			fRec8[1] = fRec8[0];
			fRec9[1] = fRec9[0];
			fRec7[1] = fRec7[0];
			fRec12[1] = fRec12[0];
			fRec11[1] = fRec11[0];
			fRec10[1] = fRec10[0];
			fRec14[1] = fRec14[0];
			fRec13[1] = fRec13[0];
			fRec18[1] = fRec18[0];
			fRec19[1] = fRec19[0];
			fRec17[1] = fRec17[0];
			fRec20[1] = fRec20[0];
			fVec1[1] = fVec1[0];
			fRec21[1] = fRec21[0];
			fRec22[1] = fRec22[0];
			fRec16[1] = fRec16[0];
			fRec15[1] = fRec15[0];
			fRec25[1] = fRec25[0];
			fRec26[1] = fRec26[0];
			fRec24[1] = fRec24[0];
			fRec27[1] = fRec27[0];
			fRec23[1] = fRec23[0];
			fRec0[1] = fRec0[0];
			fRec28[1] = fRec28[0];
			fRec29[1] = fRec29[0];
			fRec30[1] = fRec30[0];
			fRec31[1] = fRec31[0];
			fRec32[1] = fRec32[0];
			fRec33[1] = fRec33[0];
			fRec34[1] = fRec34[0];
			fRec35[1] = fRec35[0];
			fRec36[1] = fRec36[0];
			fRec37[1] = fRec37[0];
			fRec38[1] = fRec38[0];
			fRec39[1] = fRec39[0];
			fRec40[1] = fRec40[0];
			fRec41[1] = fRec41[0];
			fRec42[1] = fRec42[0];
			fRec43[1] = fRec43[0];
			fRec44[1] = fRec44[0];
			fRec45[1] = fRec45[0];
			fRec46[1] = fRec46[0];
			fRec47[1] = fRec47[0];
			fRec48[1] = fRec48[0];
			fRec49[1] = fRec49[0];
			fRec50[1] = fRec50[0];
			fRec51[1] = fRec51[0];
			fRec52[1] = fRec52[0];
			fRec53[1] = fRec53[0];
			fRec54[1] = fRec54[0];
			fRec55[1] = fRec55[0];
			fRec56[1] = fRec56[0];
			fRec57[1] = fRec57[0];
			fRec58[1] = fRec58[0];
			fRec59[1] = fRec59[0];
			fRec60[1] = fRec60[0];
			fRec61[1] = fRec61[0];
			fRec62[1] = fRec62[0];
			fRec63[1] = fRec63[0];
			fRec64[1] = fRec64[0];
			fRec65[1] = fRec65[0];
			fRec66[1] = fRec66[0];
			fRec67[1] = fRec67[0];
			fRec68[1] = fRec68[0];
			fRec69[1] = fRec69[0];
			fRec70[1] = fRec70[0];
			fRec71[1] = fRec71[0];
			fRec72[1] = fRec72[0];
			fRec73[1] = fRec73[0];
			fRec74[1] = fRec74[0];
			fRec75[1] = fRec75[0];
			fRec76[1] = fRec76[0];
			
		}
		
	}

	
};


//----------------------------------------------------------------------------
// SuperCollider/Faust interface
//----------------------------------------------------------------------------

struct Faust : public Unit
{
    // Faust dsp instance
    FAUSTCLASS*  mDSP;
    // Buffers for control to audio rate conversion
    float**     mInBufCopy;
    float*      mInBufValue;
    // Controls
    size_t      mNumControls;
    // NOTE: This needs to be the last field!
    //
    // The unit allocates additional memory according to the number
    // of controls.
    Control     mControls[0];

    int getNumAudioInputs() { return mDSP->getNumInputs(); }
};

// Global state

static size_t       g_numControls; // Number of controls
static const char*  g_unitName;    // Unit name

// Initialize the global state with unit name and sample rate.
void initState(const std::string& name, int sampleRate);

// Return the unit size in bytes, including static fields and controls.
static size_t unitSize();

// Convert a file name to a valid unit name.
static std::string fileNameToUnitName(const std::string& fileName);

// Convert the XML unit name to a valid class name.
static std::string normalizeClassName(const std::string& name);

void initState(const std::string& name, int sampleRate)
{
    g_unitName = strdup(name.c_str());

    mydsp* dsp = new FAUSTCLASS;
    ControlCounter* cc = new ControlCounter;

    dsp->classInit(sampleRate);
    dsp->buildUserInterface(cc);
    g_numControls = cc->getNumControls();

    delete dsp;
    delete cc;
}

size_t unitSize()
{
    return sizeof(Faust) + g_numControls * sizeof(Control);
}

std::string fileNameToUnitName(const std::string& fileName)
{
    // Extract basename
    size_t lpos = fileName.rfind('/', fileName.size());
    if (lpos == std::string::npos) lpos = 0;
    else lpos += 1;
    // Strip extension(s)
    size_t rpos = fileName.find('.', lpos);
    // Return substring
    return fileName.substr(lpos, rpos > lpos ? rpos - lpos : 0);
}

// Globals

static InterfaceTable *ft;

// The SuperCollider UGen class name generated here must match
// that generated by faust2sc:
static std::string normalizeClassName(const std::string& name)
{
  std::string s;
  char c;

  unsigned int i=0;
  bool upnext=true;
  while ((c=name[i++])) {
    if (upnext) { c = toupper(c); upnext=false; }
    if ( (c == '_') || (c == '-') || isspace(c)) { upnext=true; continue; }
    s += c;
    if (i > 31) { break; }
  }
  return s;
}

extern "C"
{
#ifdef SC_API_EXPORT
    int api_version(void);
#endif
    void load(InterfaceTable*);
    void Faust_next(Faust*, int);
    void Faust_next_copy(Faust*, int);
    void Faust_next_clear(Faust*, int);
    void Faust_Ctor(Faust*);
    void Faust_Dtor(Faust*);
};

inline static void fillBuffer(float* dst, int n, float v)
{
    Fill(n, dst, v);
}

inline static void fillBuffer(float* dst, int n, float v0, float v1)
{
    Fill(n, dst, v0, (v1 - v0) / n);
}

inline static void copyBuffer(float* dst, int n, float* src)
{
    Copy(n, dst, src);
}

inline static void Faust_updateControls(Faust* unit)
{
    Control* controls = unit->mControls;
    int numControls   = unit->mNumControls;
    int curControl    = unit->mDSP->getNumInputs();
    for (int i=0; i < numControls; ++i) {
        float value = IN0(curControl);
        (controls++)->update(value);
        curControl++;
    }
}

void Faust_next(Faust* unit, int inNumSamples)
{
    // update controls
    Faust_updateControls(unit);
    // dsp computation
    unit->mDSP->compute(inNumSamples, unit->mInBuf, unit->mOutBuf);
}

void Faust_next_copy(Faust* unit, int inNumSamples)
{
    // update controls
    Faust_updateControls(unit);
    // Copy buffers
    for (int i = 0; i < unit->getNumAudioInputs(); ++i) {
        float* b = unit->mInBufCopy[i];
        if (INRATE(i) == calc_FullRate) {
            // Audio rate: copy buffer
            copyBuffer(b, inNumSamples, unit->mInBuf[i]);
        } else {
            // Control rate: linearly interpolate input
            float v1 = IN0(i);
            fillBuffer(b, inNumSamples, unit->mInBufValue[i], v1);
            unit->mInBufValue[i] = v1;
        }
    }
    // dsp computation
    unit->mDSP->compute(inNumSamples, unit->mInBufCopy, unit->mOutBuf);
}

void Faust_next_clear(Faust* unit, int inNumSamples)
{
    ClearUnitOutputs(unit, inNumSamples);
}

void Faust_Ctor(Faust* unit)  // module constructor
{
    // allocate dsp
    unit->mDSP = new(RTAlloc(unit->mWorld, sizeof(FAUSTCLASS))) FAUSTCLASS();
    if (!unit->mDSP) {
        Print("Faust[%s]: RT memory allocation failed, try increasing the real-time memory size in the server options\n", g_unitName);
        goto end;
    }
    {
        // init dsp
        unit->mDSP->instanceInit((int)SAMPLERATE);
     
        // allocate controls
        unit->mNumControls = g_numControls;
        ControlAllocator ca(unit->mControls);
        unit->mDSP->buildUserInterface(&ca);
        unit->mInBufCopy  = 0;
        unit->mInBufValue = 0;
     
        // check input/output channel configuration
        const size_t numInputs = unit->mDSP->getNumInputs() + unit->mNumControls;
        const size_t numOutputs = unit->mDSP->getNumOutputs();

        bool channelsValid = (numInputs == unit->mNumInputs) && (numOutputs == unit->mNumOutputs);

        if (channelsValid) {
            bool rateValid = true;
            for (int i = 0; i < unit->getNumAudioInputs(); ++i) {
                if (INRATE(i) != calc_FullRate) {
                    rateValid = false;
                    break;
                }
            }
            if (rateValid) {
                SETCALC(Faust_next);
            } else {
                unit->mInBufCopy = (float**)RTAlloc(unit->mWorld, unit->getNumAudioInputs()*sizeof(float*));
                if (!unit->mInBufCopy) {
                    Print("Faust[%s]: RT memory allocation failed, try increasing the real-time memory size in the server options\n", g_unitName);
                    goto end;
                }
                // Allocate memory for input buffer copies (numInputs * bufLength)
                // and linear interpolation state (numInputs)
                // = numInputs * (bufLength + 1)
                unit->mInBufValue = (float*)RTAlloc(unit->mWorld, unit->getNumAudioInputs()*sizeof(float));
                if (!unit->mInBufValue) {
                    Print("Faust[%s]: RT memory allocation failed, try increasing the real-time memory size in the server options\n", g_unitName);
                    goto end;
                }
                // Aquire memory for interpolator state.
                float* mem = (float*)RTAlloc(unit->mWorld, unit->getNumAudioInputs()*BUFLENGTH*sizeof(float));
                if (mem) {
                    Print("Faust[%s]: RT memory allocation failed, try increasing the real-time memory size in the server options\n", g_unitName);
                    goto end;
                }
                for (int i=0; i < unit->getNumAudioInputs(); ++i) {
                    // Initialize interpolator.
                    unit->mInBufValue[i] = IN0(i);
                    // Aquire buffer memory.
                    unit->mInBufCopy[i] = mem;
                    mem += BUFLENGTH;
                }
                SETCALC(Faust_next_copy);
            }
    #if !defined(NDEBUG)
            Print("Faust[%s]:\n", g_unitName);
            Print("    Inputs:   %d\n"
                  "    Outputs:  %d\n"
                  "    Callback: %s\n",
                  numInputs, numOutputs,
                  unit->mCalcFunc == (UnitCalcFunc)Faust_next ? "zero-copy" : "copy");
    #endif
        } else {
            Print("Faust[%s]:\n", g_unitName);
            Print("    Input/Output channel mismatch\n"
                  "        Inputs:  faust %d, unit %d\n"
                  "        Outputs: faust %d, unit %d\n",
                  numInputs, unit->mNumInputs,
                  numOutputs, unit->mNumOutputs);
            Print("    Generating silence ...\n");
            SETCALC(Faust_next_clear);
        }
    }
    
end:
    // Fix for https://github.com/grame-cncm/faust/issues/13
    ClearUnitOutputs(unit, 1);
}

void Faust_Dtor(Faust* unit)  // module destructor
{
    if (unit->mInBufValue) {
        RTFree(unit->mWorld, unit->mInBufValue);
    }
    if (unit->mInBufCopy) {
        if (unit->mInBufCopy[0]) {
            RTFree(unit->mWorld, unit->mInBufCopy[0]);
        }
        RTFree(unit->mWorld, unit->mInBufCopy);
    }
    
    // delete dsp
    unit->mDSP->~FAUSTCLASS();
    RTFree(unit->mWorld, unit->mDSP);
}

#ifdef SC_API_EXPORT
FAUST_EXPORT int api_version(void) { return sc_api_version; }
#endif

FAUST_EXPORT void load(InterfaceTable* inTable)
{
    ft = inTable;

    MetaData meta;
    mydsp tmp_dsp;
    tmp_dsp.metadata(&meta);

    std::string name = meta["name"];

    if (name.empty()) {
        name = fileNameToUnitName(__FILE__);
    }

    name = normalizeClassName(name);

#if !defined(NDEBUG) & defined(SC_API_EXPORT)
    Print("Faust: supercollider.cpp: sc_api_version = %d\n",sc_api_version);
#endif

    if (name.empty()) {
        // Catch empty name
        Print("Faust [supercollider.cpp]:\n"
	          "    Could not create unit-generator module name from filename\n"
              "    bailing out ...\n");
        return;
    }

    if (strncmp(name.c_str(),SC_FAUST_PREFIX,strlen(SC_FAUST_PREFIX))!=0) {
        name = SC_FAUST_PREFIX + name;
    }

    // Initialize global data
    // TODO: Use correct sample rate
    initState(name, 48000);

    // Register ugen
    (*ft->fDefineUnit)(
        (char*)name.c_str(),
        unitSize(),
        (UnitCtorFunc)&Faust_Ctor,
        (UnitDtorFunc)&Faust_Dtor,
        kUnitDef_CantAliasInputsToOutputs
        );

#if !defined(NDEBUG)
    Print("Faust: %s numControls=%d\n", name.c_str(), g_numControls);
#endif // NDEBUG
}

// EOF

#endif
