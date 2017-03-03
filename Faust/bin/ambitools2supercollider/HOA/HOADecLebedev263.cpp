/* ------------------------------------------------------------
author: "Pierre Lecomte"
copyright: "(c) Pierre Lecomte 2014"
license: "GPL"
name: "HOADecLebedev263"
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

#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
#endif

class mydsp : public dsp {
	
 private:
	
	int fSamplingFreq;
	float fRec1[2];
	float fRec3[2];
	float fRec2[2];
	float fRec7[2];
	float fRec8[2];
	float fRec6[2];
	float fRec9[2];
	float fRec5[2];
	float fRec4[2];
	float fRec13[2];
	float fRec14[2];
	float fRec12[2];
	float fRec15[2];
	float fRec11[2];
	float fRec10[2];
	float fRec19[2];
	float fRec20[2];
	float fRec18[2];
	float fRec21[2];
	float fRec17[2];
	float fRec16[2];
	float fRec25[2];
	float fRec26[2];
	float fRec24[2];
	float fRec27[2];
	float fRec23[2];
	float fRec22[2];
	float fRec30[2];
	float fRec31[2];
	float fRec29[2];
	float fRec28[2];
	float fRec34[2];
	float fRec35[2];
	float fRec33[2];
	float fRec32[2];
	float fRec38[2];
	float fRec39[2];
	float fRec37[2];
	float fRec36[2];
	float fRec42[2];
	float fRec41[2];
	float fRec40[2];
	float fRec45[2];
	float fRec44[2];
	float fRec43[2];
	float fRec0[2];
	float fRec46[2];
	float fRec50[2];
	float fRec51[2];
	float fRec49[2];
	float fRec48[2];
	float fRec54[2];
	float fRec53[2];
	float fRec52[2];
	float fRec58[2];
	float fRec59[2];
	float fRec57[2];
	float fRec60[2];
	float fRec56[2];
	float fRec55[2];
	float fRec64[2];
	float fRec65[2];
	float fRec63[2];
	float fRec66[2];
	float fRec62[2];
	float fRec61[2];
	float fRec47[2];
	float fRec67[2];
	float fRec68[2];
	float fRec69[2];
	float fRec70[2];
	float fRec75[2];
	float fRec76[2];
	float fRec74[2];
	float fRec77[2];
	float fRec73[2];
	float fRec72[2];
	float fRec80[2];
	float fRec81[2];
	float fRec79[2];
	float fRec78[2];
	float fRec71[2];
	float fRec82[2];
	float fRec83[2];
	float fRec84[2];
	float fRec85[2];
	float fRec86[2];
	float fRec87[2];
	float fRec88[2];
	float fRec89[2];
	float fRec90[2];
	float fRec91[2];
	float fRec92[2];
	float fRec93[2];
	float fRec94[2];
	float fRec95[2];
	float fRec96[2];
	float fRec97[2];
	float fRec98[2];
	float fRec99[2];
	float fConst0;
	float fConst1;
	FAUSTFLOAT fVslider0;
	FAUSTFLOAT fEntry0;
	FAUSTFLOAT fCheckbox0;
	FAUSTFLOAT fVslider1;
	FAUSTFLOAT fVbargraph0;
	float fConst2;
	float fConst3;
	float fConst4;
	float fConst5;
	float fConst6;
	FAUSTFLOAT fVbargraph1;
	FAUSTFLOAT fVbargraph2;
	FAUSTFLOAT fVbargraph3;
	FAUSTFLOAT fVbargraph4;
	float fConst7;
	float fConst8;
	FAUSTFLOAT fVbargraph5;
	FAUSTFLOAT fVbargraph6;
	FAUSTFLOAT fVbargraph7;
	float fConst9;
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
		m->declare("name", "HOADecLebedev263");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
		m->declare("version", "1.0");
	}

	virtual int getNumInputs() {
		return 16;
		
	}
	virtual int getNumOutputs() {
		return 26;
		
	}
	virtual int getInputRate(int channel) {
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
		fConst2 = (394.771515f / fConst0);
		fConst3 = (625.228516f / fConst0);
		fConst4 = (170.0f / fConst0);
		fConst5 = (1250.45703f / fConst0);
		fConst6 = (789.54303f / fConst0);
		fConst7 = (510.0f / fConst0);
		fConst8 = (1020.0f / fConst0);
		fConst9 = (340.0f / fConst0);
		
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(0.0f);
		fEntry0 = FAUSTFLOAT(1.0700000000000001f);
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fVslider1 = FAUSTFLOAT(0.0f);
		
	}
	
	virtual void instanceClear() {
		for (int i0 = 0; (i0 < 2); i0 = (i0 + 1)) {
			fRec1[i0] = 0.0f;
			
		}
		for (int i1 = 0; (i1 < 2); i1 = (i1 + 1)) {
			fRec3[i1] = 0.0f;
			
		}
		for (int i2 = 0; (i2 < 2); i2 = (i2 + 1)) {
			fRec2[i2] = 0.0f;
			
		}
		for (int i3 = 0; (i3 < 2); i3 = (i3 + 1)) {
			fRec7[i3] = 0.0f;
			
		}
		for (int i4 = 0; (i4 < 2); i4 = (i4 + 1)) {
			fRec8[i4] = 0.0f;
			
		}
		for (int i5 = 0; (i5 < 2); i5 = (i5 + 1)) {
			fRec6[i5] = 0.0f;
			
		}
		for (int i6 = 0; (i6 < 2); i6 = (i6 + 1)) {
			fRec9[i6] = 0.0f;
			
		}
		for (int i7 = 0; (i7 < 2); i7 = (i7 + 1)) {
			fRec5[i7] = 0.0f;
			
		}
		for (int i8 = 0; (i8 < 2); i8 = (i8 + 1)) {
			fRec4[i8] = 0.0f;
			
		}
		for (int i9 = 0; (i9 < 2); i9 = (i9 + 1)) {
			fRec13[i9] = 0.0f;
			
		}
		for (int i10 = 0; (i10 < 2); i10 = (i10 + 1)) {
			fRec14[i10] = 0.0f;
			
		}
		for (int i11 = 0; (i11 < 2); i11 = (i11 + 1)) {
			fRec12[i11] = 0.0f;
			
		}
		for (int i12 = 0; (i12 < 2); i12 = (i12 + 1)) {
			fRec15[i12] = 0.0f;
			
		}
		for (int i13 = 0; (i13 < 2); i13 = (i13 + 1)) {
			fRec11[i13] = 0.0f;
			
		}
		for (int i14 = 0; (i14 < 2); i14 = (i14 + 1)) {
			fRec10[i14] = 0.0f;
			
		}
		for (int i15 = 0; (i15 < 2); i15 = (i15 + 1)) {
			fRec19[i15] = 0.0f;
			
		}
		for (int i16 = 0; (i16 < 2); i16 = (i16 + 1)) {
			fRec20[i16] = 0.0f;
			
		}
		for (int i17 = 0; (i17 < 2); i17 = (i17 + 1)) {
			fRec18[i17] = 0.0f;
			
		}
		for (int i18 = 0; (i18 < 2); i18 = (i18 + 1)) {
			fRec21[i18] = 0.0f;
			
		}
		for (int i19 = 0; (i19 < 2); i19 = (i19 + 1)) {
			fRec17[i19] = 0.0f;
			
		}
		for (int i20 = 0; (i20 < 2); i20 = (i20 + 1)) {
			fRec16[i20] = 0.0f;
			
		}
		for (int i21 = 0; (i21 < 2); i21 = (i21 + 1)) {
			fRec25[i21] = 0.0f;
			
		}
		for (int i22 = 0; (i22 < 2); i22 = (i22 + 1)) {
			fRec26[i22] = 0.0f;
			
		}
		for (int i23 = 0; (i23 < 2); i23 = (i23 + 1)) {
			fRec24[i23] = 0.0f;
			
		}
		for (int i24 = 0; (i24 < 2); i24 = (i24 + 1)) {
			fRec27[i24] = 0.0f;
			
		}
		for (int i25 = 0; (i25 < 2); i25 = (i25 + 1)) {
			fRec23[i25] = 0.0f;
			
		}
		for (int i26 = 0; (i26 < 2); i26 = (i26 + 1)) {
			fRec22[i26] = 0.0f;
			
		}
		for (int i27 = 0; (i27 < 2); i27 = (i27 + 1)) {
			fRec30[i27] = 0.0f;
			
		}
		for (int i28 = 0; (i28 < 2); i28 = (i28 + 1)) {
			fRec31[i28] = 0.0f;
			
		}
		for (int i29 = 0; (i29 < 2); i29 = (i29 + 1)) {
			fRec29[i29] = 0.0f;
			
		}
		for (int i30 = 0; (i30 < 2); i30 = (i30 + 1)) {
			fRec28[i30] = 0.0f;
			
		}
		for (int i31 = 0; (i31 < 2); i31 = (i31 + 1)) {
			fRec34[i31] = 0.0f;
			
		}
		for (int i32 = 0; (i32 < 2); i32 = (i32 + 1)) {
			fRec35[i32] = 0.0f;
			
		}
		for (int i33 = 0; (i33 < 2); i33 = (i33 + 1)) {
			fRec33[i33] = 0.0f;
			
		}
		for (int i34 = 0; (i34 < 2); i34 = (i34 + 1)) {
			fRec32[i34] = 0.0f;
			
		}
		for (int i35 = 0; (i35 < 2); i35 = (i35 + 1)) {
			fRec38[i35] = 0.0f;
			
		}
		for (int i36 = 0; (i36 < 2); i36 = (i36 + 1)) {
			fRec39[i36] = 0.0f;
			
		}
		for (int i37 = 0; (i37 < 2); i37 = (i37 + 1)) {
			fRec37[i37] = 0.0f;
			
		}
		for (int i38 = 0; (i38 < 2); i38 = (i38 + 1)) {
			fRec36[i38] = 0.0f;
			
		}
		for (int i39 = 0; (i39 < 2); i39 = (i39 + 1)) {
			fRec42[i39] = 0.0f;
			
		}
		for (int i40 = 0; (i40 < 2); i40 = (i40 + 1)) {
			fRec41[i40] = 0.0f;
			
		}
		for (int i41 = 0; (i41 < 2); i41 = (i41 + 1)) {
			fRec40[i41] = 0.0f;
			
		}
		for (int i42 = 0; (i42 < 2); i42 = (i42 + 1)) {
			fRec45[i42] = 0.0f;
			
		}
		for (int i43 = 0; (i43 < 2); i43 = (i43 + 1)) {
			fRec44[i43] = 0.0f;
			
		}
		for (int i44 = 0; (i44 < 2); i44 = (i44 + 1)) {
			fRec43[i44] = 0.0f;
			
		}
		for (int i45 = 0; (i45 < 2); i45 = (i45 + 1)) {
			fRec0[i45] = 0.0f;
			
		}
		for (int i46 = 0; (i46 < 2); i46 = (i46 + 1)) {
			fRec46[i46] = 0.0f;
			
		}
		for (int i47 = 0; (i47 < 2); i47 = (i47 + 1)) {
			fRec50[i47] = 0.0f;
			
		}
		for (int i48 = 0; (i48 < 2); i48 = (i48 + 1)) {
			fRec51[i48] = 0.0f;
			
		}
		for (int i49 = 0; (i49 < 2); i49 = (i49 + 1)) {
			fRec49[i49] = 0.0f;
			
		}
		for (int i50 = 0; (i50 < 2); i50 = (i50 + 1)) {
			fRec48[i50] = 0.0f;
			
		}
		for (int i51 = 0; (i51 < 2); i51 = (i51 + 1)) {
			fRec54[i51] = 0.0f;
			
		}
		for (int i52 = 0; (i52 < 2); i52 = (i52 + 1)) {
			fRec53[i52] = 0.0f;
			
		}
		for (int i53 = 0; (i53 < 2); i53 = (i53 + 1)) {
			fRec52[i53] = 0.0f;
			
		}
		for (int i54 = 0; (i54 < 2); i54 = (i54 + 1)) {
			fRec58[i54] = 0.0f;
			
		}
		for (int i55 = 0; (i55 < 2); i55 = (i55 + 1)) {
			fRec59[i55] = 0.0f;
			
		}
		for (int i56 = 0; (i56 < 2); i56 = (i56 + 1)) {
			fRec57[i56] = 0.0f;
			
		}
		for (int i57 = 0; (i57 < 2); i57 = (i57 + 1)) {
			fRec60[i57] = 0.0f;
			
		}
		for (int i58 = 0; (i58 < 2); i58 = (i58 + 1)) {
			fRec56[i58] = 0.0f;
			
		}
		for (int i59 = 0; (i59 < 2); i59 = (i59 + 1)) {
			fRec55[i59] = 0.0f;
			
		}
		for (int i60 = 0; (i60 < 2); i60 = (i60 + 1)) {
			fRec64[i60] = 0.0f;
			
		}
		for (int i61 = 0; (i61 < 2); i61 = (i61 + 1)) {
			fRec65[i61] = 0.0f;
			
		}
		for (int i62 = 0; (i62 < 2); i62 = (i62 + 1)) {
			fRec63[i62] = 0.0f;
			
		}
		for (int i63 = 0; (i63 < 2); i63 = (i63 + 1)) {
			fRec66[i63] = 0.0f;
			
		}
		for (int i64 = 0; (i64 < 2); i64 = (i64 + 1)) {
			fRec62[i64] = 0.0f;
			
		}
		for (int i65 = 0; (i65 < 2); i65 = (i65 + 1)) {
			fRec61[i65] = 0.0f;
			
		}
		for (int i66 = 0; (i66 < 2); i66 = (i66 + 1)) {
			fRec47[i66] = 0.0f;
			
		}
		for (int i67 = 0; (i67 < 2); i67 = (i67 + 1)) {
			fRec67[i67] = 0.0f;
			
		}
		for (int i68 = 0; (i68 < 2); i68 = (i68 + 1)) {
			fRec68[i68] = 0.0f;
			
		}
		for (int i69 = 0; (i69 < 2); i69 = (i69 + 1)) {
			fRec69[i69] = 0.0f;
			
		}
		for (int i70 = 0; (i70 < 2); i70 = (i70 + 1)) {
			fRec70[i70] = 0.0f;
			
		}
		for (int i71 = 0; (i71 < 2); i71 = (i71 + 1)) {
			fRec75[i71] = 0.0f;
			
		}
		for (int i72 = 0; (i72 < 2); i72 = (i72 + 1)) {
			fRec76[i72] = 0.0f;
			
		}
		for (int i73 = 0; (i73 < 2); i73 = (i73 + 1)) {
			fRec74[i73] = 0.0f;
			
		}
		for (int i74 = 0; (i74 < 2); i74 = (i74 + 1)) {
			fRec77[i74] = 0.0f;
			
		}
		for (int i75 = 0; (i75 < 2); i75 = (i75 + 1)) {
			fRec73[i75] = 0.0f;
			
		}
		for (int i76 = 0; (i76 < 2); i76 = (i76 + 1)) {
			fRec72[i76] = 0.0f;
			
		}
		for (int i77 = 0; (i77 < 2); i77 = (i77 + 1)) {
			fRec80[i77] = 0.0f;
			
		}
		for (int i78 = 0; (i78 < 2); i78 = (i78 + 1)) {
			fRec81[i78] = 0.0f;
			
		}
		for (int i79 = 0; (i79 < 2); i79 = (i79 + 1)) {
			fRec79[i79] = 0.0f;
			
		}
		for (int i80 = 0; (i80 < 2); i80 = (i80 + 1)) {
			fRec78[i80] = 0.0f;
			
		}
		for (int i81 = 0; (i81 < 2); i81 = (i81 + 1)) {
			fRec71[i81] = 0.0f;
			
		}
		for (int i82 = 0; (i82 < 2); i82 = (i82 + 1)) {
			fRec82[i82] = 0.0f;
			
		}
		for (int i83 = 0; (i83 < 2); i83 = (i83 + 1)) {
			fRec83[i83] = 0.0f;
			
		}
		for (int i84 = 0; (i84 < 2); i84 = (i84 + 1)) {
			fRec84[i84] = 0.0f;
			
		}
		for (int i85 = 0; (i85 < 2); i85 = (i85 + 1)) {
			fRec85[i85] = 0.0f;
			
		}
		for (int i86 = 0; (i86 < 2); i86 = (i86 + 1)) {
			fRec86[i86] = 0.0f;
			
		}
		for (int i87 = 0; (i87 < 2); i87 = (i87 + 1)) {
			fRec87[i87] = 0.0f;
			
		}
		for (int i88 = 0; (i88 < 2); i88 = (i88 + 1)) {
			fRec88[i88] = 0.0f;
			
		}
		for (int i89 = 0; (i89 < 2); i89 = (i89 + 1)) {
			fRec89[i89] = 0.0f;
			
		}
		for (int i90 = 0; (i90 < 2); i90 = (i90 + 1)) {
			fRec90[i90] = 0.0f;
			
		}
		for (int i91 = 0; (i91 < 2); i91 = (i91 + 1)) {
			fRec91[i91] = 0.0f;
			
		}
		for (int i92 = 0; (i92 < 2); i92 = (i92 + 1)) {
			fRec92[i92] = 0.0f;
			
		}
		for (int i93 = 0; (i93 < 2); i93 = (i93 + 1)) {
			fRec93[i93] = 0.0f;
			
		}
		for (int i94 = 0; (i94 < 2); i94 = (i94 + 1)) {
			fRec94[i94] = 0.0f;
			
		}
		for (int i95 = 0; (i95 < 2); i95 = (i95 + 1)) {
			fRec95[i95] = 0.0f;
			
		}
		for (int i96 = 0; (i96 < 2); i96 = (i96 + 1)) {
			fRec96[i96] = 0.0f;
			
		}
		for (int i97 = 0; (i97 < 2); i97 = (i97 + 1)) {
			fRec97[i97] = 0.0f;
			
		}
		for (int i98 = 0; (i98 < 2); i98 = (i98 + 1)) {
			fRec98[i98] = 0.0f;
			
		}
		for (int i99 = 0; (i99 < 2); i99 = (i99 + 1)) {
			fRec99[i99] = 0.0f;
			
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
		ui_interface->openHorizontalBox("Inputs");
		ui_interface->openHorizontalBox("B-Format");
		ui_interface->openHorizontalBox("0");
		ui_interface->openVerticalBox("0");
		ui_interface->declare(&fVbargraph0, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3527a60", &fVbargraph0, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("1");
		ui_interface->openVerticalBox("1");
		ui_interface->declare(&fVbargraph13, "unit", "dB");
		ui_interface->addVerticalBargraph("0x35c1a00", &fVbargraph13, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("2");
		ui_interface->declare(&fVbargraph8, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3596b70", &fVbargraph8, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("3");
		ui_interface->declare(&fVbargraph9, "unit", "dB");
		ui_interface->addVerticalBargraph("0x35a0330", &fVbargraph9, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2");
		ui_interface->openVerticalBox("4");
		ui_interface->declare(&fVbargraph12, "unit", "dB");
		ui_interface->addVerticalBargraph("0x35b8620", &fVbargraph12, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("5");
		ui_interface->declare(&fVbargraph22, "unit", "dB");
		ui_interface->addVerticalBargraph("0x361c9e0", &fVbargraph22, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("6");
		ui_interface->declare(&fVbargraph7, "unit", "dB");
		ui_interface->addVerticalBargraph("0x358c3c0", &fVbargraph7, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("7");
		ui_interface->declare(&fVbargraph6, "unit", "dB");
		ui_interface->addVerticalBargraph("0x35810d0", &fVbargraph6, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("8");
		ui_interface->declare(&fVbargraph5, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3575ac0", &fVbargraph5, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3");
		ui_interface->openVerticalBox("9");
		ui_interface->declare(&fVbargraph15, "unit", "dB");
		ui_interface->addVerticalBargraph("0x35def20", &fVbargraph15, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("10");
		ui_interface->declare(&fVbargraph21, "unit", "dB");
		ui_interface->addVerticalBargraph("0x36113e0", &fVbargraph21, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("11");
		ui_interface->declare(&fVbargraph14, "unit", "dB");
		ui_interface->addVerticalBargraph("0x35d05a0", &fVbargraph14, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("12");
		ui_interface->declare(&fVbargraph4, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3567e40", &fVbargraph4, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("13");
		ui_interface->declare(&fVbargraph3, "unit", "dB");
		ui_interface->addVerticalBargraph("0x35596f0", &fVbargraph3, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("14");
		ui_interface->declare(&fVbargraph2, "unit", "dB");
		ui_interface->addVerticalBargraph("0x354ad60", &fVbargraph2, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("15");
		ui_interface->declare(&fVbargraph1, "unit", "dB");
		ui_interface->addVerticalBargraph("0x353c2f0", &fVbargraph1, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->declare(&fVslider1, "1", "");
		ui_interface->declare(&fVslider1, "osc", "/levelin -10 60");
		ui_interface->declare(&fVslider1, "unit", "dB");
		ui_interface->addVerticalSlider("Inputs Gain", &fVslider1, 0.0f, -10.0f, 60.0f, 0.100000001f);
		ui_interface->declare(&fVslider0, "2", "");
		ui_interface->declare(&fVslider0, "osc", "/levelout -10 60");
		ui_interface->declare(&fVslider0, "unit", "dB");
		ui_interface->addVerticalSlider("Outputs Gain", &fVslider0, 0.0f, -10.0f, 60.0f, 0.100000001f);
		ui_interface->declare(0, "3", "");
		ui_interface->openVerticalBox("NFC");
		ui_interface->addCheckButton("Yes",&fCheckbox0);
		ui_interface->closeBox();
		ui_interface->declare(&fEntry0, "4", "");
		ui_interface->addNumEntry("Speakers Radius", &fEntry0, 1.07000005f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Outputs 1-outs/2");
		ui_interface->openVerticalBox("1");
		ui_interface->declare(&fVbargraph10, "osc", "/output1");
		ui_interface->declare(&fVbargraph10, "unit", "dB");
		ui_interface->addVerticalBargraph("0x35a60f0", &fVbargraph10, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("2");
		ui_interface->declare(&fVbargraph11, "osc", "/output2");
		ui_interface->declare(&fVbargraph11, "unit", "dB");
		ui_interface->addVerticalBargraph("0x35ad2e0", &fVbargraph11, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("3");
		ui_interface->declare(&fVbargraph16, "osc", "/output3");
		ui_interface->declare(&fVbargraph16, "unit", "dB");
		ui_interface->addVerticalBargraph("0x35e4f80", &fVbargraph16, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("4");
		ui_interface->declare(&fVbargraph17, "osc", "/output4");
		ui_interface->declare(&fVbargraph17, "unit", "dB");
		ui_interface->addVerticalBargraph("0x35ebbe0", &fVbargraph17, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("5");
		ui_interface->declare(&fVbargraph18, "osc", "/output5");
		ui_interface->declare(&fVbargraph18, "unit", "dB");
		ui_interface->addVerticalBargraph("0x35f2920", &fVbargraph18, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("6");
		ui_interface->declare(&fVbargraph19, "osc", "/output6");
		ui_interface->declare(&fVbargraph19, "unit", "dB");
		ui_interface->addVerticalBargraph("0x35f8a40", &fVbargraph19, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("7");
		ui_interface->declare(&fVbargraph20, "osc", "/output7");
		ui_interface->declare(&fVbargraph20, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3602610", &fVbargraph20, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("8");
		ui_interface->declare(&fVbargraph23, "osc", "/output8");
		ui_interface->declare(&fVbargraph23, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3625a90", &fVbargraph23, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("9");
		ui_interface->declare(&fVbargraph24, "osc", "/output9");
		ui_interface->declare(&fVbargraph24, "unit", "dB");
		ui_interface->addVerticalBargraph("0x362ec60", &fVbargraph24, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("10");
		ui_interface->declare(&fVbargraph25, "osc", "/output10");
		ui_interface->declare(&fVbargraph25, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3637b20", &fVbargraph25, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("11");
		ui_interface->declare(&fVbargraph26, "osc", "/output11");
		ui_interface->declare(&fVbargraph26, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3640140", &fVbargraph26, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("12");
		ui_interface->declare(&fVbargraph27, "osc", "/output12");
		ui_interface->declare(&fVbargraph27, "unit", "dB");
		ui_interface->addVerticalBargraph("0x36476d0", &fVbargraph27, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("13");
		ui_interface->declare(&fVbargraph28, "osc", "/output13");
		ui_interface->declare(&fVbargraph28, "unit", "dB");
		ui_interface->addVerticalBargraph("0x364e590", &fVbargraph28, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Outputs 14-26");
		ui_interface->openVerticalBox("14");
		ui_interface->declare(&fVbargraph29, "osc", "/output14");
		ui_interface->declare(&fVbargraph29, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3655ac0", &fVbargraph29, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("15");
		ui_interface->declare(&fVbargraph30, "osc", "/output15");
		ui_interface->declare(&fVbargraph30, "unit", "dB");
		ui_interface->addVerticalBargraph("0x365b860", &fVbargraph30, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("16");
		ui_interface->declare(&fVbargraph31, "osc", "/output16");
		ui_interface->declare(&fVbargraph31, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3662140", &fVbargraph31, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("17");
		ui_interface->declare(&fVbargraph32, "osc", "/output17");
		ui_interface->declare(&fVbargraph32, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3668ae0", &fVbargraph32, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("18");
		ui_interface->declare(&fVbargraph33, "osc", "/output18");
		ui_interface->declare(&fVbargraph33, "unit", "dB");
		ui_interface->addVerticalBargraph("0x366f740", &fVbargraph33, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("19");
		ui_interface->declare(&fVbargraph34, "osc", "/output19");
		ui_interface->declare(&fVbargraph34, "unit", "dB");
		ui_interface->addVerticalBargraph("0x367bcd0", &fVbargraph34, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("20");
		ui_interface->declare(&fVbargraph35, "osc", "/output20");
		ui_interface->declare(&fVbargraph35, "unit", "dB");
		ui_interface->addVerticalBargraph("0x36857b0", &fVbargraph35, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("21");
		ui_interface->declare(&fVbargraph36, "osc", "/output21");
		ui_interface->declare(&fVbargraph36, "unit", "dB");
		ui_interface->addVerticalBargraph("0x368e370", &fVbargraph36, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("22");
		ui_interface->declare(&fVbargraph37, "osc", "/output22");
		ui_interface->declare(&fVbargraph37, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3697270", &fVbargraph37, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("23");
		ui_interface->declare(&fVbargraph38, "osc", "/output23");
		ui_interface->declare(&fVbargraph38, "unit", "dB");
		ui_interface->addVerticalBargraph("0x369e540", &fVbargraph38, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("24");
		ui_interface->declare(&fVbargraph39, "osc", "/output24");
		ui_interface->declare(&fVbargraph39, "unit", "dB");
		ui_interface->addVerticalBargraph("0x36a5500", &fVbargraph39, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("25");
		ui_interface->declare(&fVbargraph40, "osc", "/output25");
		ui_interface->declare(&fVbargraph40, "unit", "dB");
		ui_interface->addVerticalBargraph("0x36ac960", &fVbargraph40, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("26");
		ui_interface->declare(&fVbargraph41, "osc", "/output26");
		ui_interface->declare(&fVbargraph41, "unit", "dB");
		ui_interface->addVerticalBargraph("0x36b3cc0", &fVbargraph41, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->closeBox();
		
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* input2 = inputs[2];
		FAUSTFLOAT* input3 = inputs[3];
		FAUSTFLOAT* input4 = inputs[4];
		FAUSTFLOAT* input5 = inputs[5];
		FAUSTFLOAT* input6 = inputs[6];
		FAUSTFLOAT* input7 = inputs[7];
		FAUSTFLOAT* input8 = inputs[8];
		FAUSTFLOAT* input9 = inputs[9];
		FAUSTFLOAT* input10 = inputs[10];
		FAUSTFLOAT* input11 = inputs[11];
		FAUSTFLOAT* input12 = inputs[12];
		FAUSTFLOAT* input13 = inputs[13];
		FAUSTFLOAT* input14 = inputs[14];
		FAUSTFLOAT* input15 = inputs[15];
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
		float fSlow0 = (0.00100000005f * powf(10.0f, (0.0500000007f * float(fVslider0))));
		float fSlow1 = float(fEntry0);
		float fSlow2 = float(fCheckbox0);
		float fSlow3 = (((fSlow1 + -1.0f) * fSlow2) + 1.0f);
		float fSlow4 = (0.00100000005f * powf(10.0f, (0.0500000007f * float(fVslider1))));
		float fSlow5 = (1.0f / ((fConst2 / fSlow1) + 1.0f));
		float fSlow6 = (fConst4 / fSlow1);
		float fSlow7 = faustpower2_f(fSlow6);
		float fSlow8 = (1.0f / (((fConst3 / fSlow1) + (6.4594326f * fSlow7)) + 1.0f));
		float fSlow9 = (25.8377304f * fSlow7);
		float fSlow10 = (fSlow9 + (fConst5 / fSlow1));
		float fSlow11 = (fSlow2 * fSlow1);
		float fSlow12 = (fConst6 / fSlow1);
		float fSlow13 = (1.0f - fSlow2);
		float fSlow14 = (1.0f / (((fConst7 / fSlow1) + (3.0f * fSlow7)) + 1.0f));
		float fSlow15 = (12.0f * fSlow7);
		float fSlow16 = (fSlow15 + (fConst8 / fSlow1));
		float fSlow17 = (1.0f / (fSlow6 + 1.0f));
		float fSlow18 = (fConst9 / fSlow1);
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec1[0] = (fSlow0 + (0.999000013f * fRec1[1]));
			fRec3[0] = (fSlow4 + (0.999000013f * fRec3[1]));
			float fTemp0 = (fSlow3 * (fRec3[0] * float(input0[i])));
			fRec2[0] = max((fRec2[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp0))))));
			fVbargraph0 = FAUSTFLOAT(fRec2[0]);
			float fTemp1 = (0.0476190485f * fTemp0);
			fRec7[0] = (fRec6[1] + fRec7[1]);
			fRec8[0] = (fRec7[1] + fRec8[1]);
			float fTemp2 = (fRec3[0] * float(input15[i]));
			fRec6[0] = (0.0f - (fSlow8 * (((fSlow10 * fRec7[0]) + (fSlow9 * fRec8[0])) - (fSlow11 * fTemp2))));
			fRec9[0] = (fRec5[1] + fRec9[1]);
			fRec5[0] = (fSlow5 * (fRec6[0] - (fSlow12 * fRec9[0])));
			float fTemp3 = (fRec5[0] + (fSlow13 * fTemp2));
			fRec4[0] = max((fRec4[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp3))))));
			fVbargraph1 = FAUSTFLOAT(fRec4[0]);
			float fTemp4 = (0.0f * fTemp3);
			fRec13[0] = (fRec12[1] + fRec13[1]);
			fRec14[0] = (fRec13[1] + fRec14[1]);
			float fTemp5 = (fRec3[0] * float(input14[i]));
			fRec12[0] = (0.0f - (fSlow8 * (((fSlow10 * fRec13[0]) + (fSlow9 * fRec14[0])) - (fSlow11 * fTemp5))));
			fRec15[0] = (fRec11[1] + fRec15[1]);
			fRec11[0] = (fSlow5 * (fRec12[0] - (fSlow12 * fRec15[0])));
			float fTemp6 = (fRec11[0] + (fSlow13 * fTemp5));
			fRec10[0] = max((fRec10[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp6))))));
			fVbargraph2 = FAUSTFLOAT(fRec10[0]);
			float fTemp7 = (9.14759786e-34f * fTemp6);
			fRec19[0] = (fRec18[1] + fRec19[1]);
			fRec20[0] = (fRec19[1] + fRec20[1]);
			float fTemp8 = (fRec3[0] * float(input13[i]));
			fRec18[0] = (0.0f - (fSlow8 * (((fSlow10 * fRec19[0]) + (fSlow9 * fRec20[0])) - (fSlow11 * fTemp8))));
			fRec21[0] = (fRec17[1] + fRec21[1]);
			fRec17[0] = (fSlow5 * (fRec18[0] - (fSlow12 * fRec21[0])));
			float fTemp9 = (fRec17[0] + (fSlow13 * fTemp8));
			fRec16[0] = max((fRec16[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp9))))));
			fVbargraph3 = FAUSTFLOAT(fRec16[0]);
			float fTemp10 = (1.88967111e-17f * fTemp9);
			float fTemp11 = (fRec3[0] * float(input12[i]));
			fRec25[0] = (fRec24[1] + fRec25[1]);
			fRec26[0] = (fRec25[1] + fRec26[1]);
			fRec24[0] = (fSlow8 * ((fSlow11 * fTemp11) - ((fSlow10 * fRec25[0]) + (fSlow9 * fRec26[0]))));
			fRec27[0] = (fRec23[1] + fRec27[1]);
			fRec23[0] = (fSlow5 * (fRec24[0] - (fSlow12 * fRec27[0])));
			float fTemp12 = (fRec23[0] + (fSlow13 * fTemp11));
			fRec22[0] = max((fRec22[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp12))))));
			fVbargraph4 = FAUSTFLOAT(fRec22[0]);
			float fTemp13 = (0.125988156f * fTemp12);
			fRec30[0] = (fRec29[1] + fRec30[1]);
			fRec31[0] = (fRec30[1] + fRec31[1]);
			float fTemp14 = (fRec3[0] * float(input8[i]));
			fRec29[0] = (0.0f - (fSlow14 * (((fSlow16 * fRec30[0]) + (fSlow15 * fRec31[0])) - (fSlow11 * fTemp14))));
			float fTemp15 = (fRec29[0] + (fSlow13 * fTemp14));
			fRec28[0] = max((fRec28[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp15))))));
			fVbargraph5 = FAUSTFLOAT(fRec28[0]);
			float fTemp16 = (3.45746707e-34f * fTemp15);
			fRec34[0] = (fRec33[1] + fRec34[1]);
			fRec35[0] = (fRec34[1] + fRec35[1]);
			float fTemp17 = (fRec3[0] * float(input7[i]));
			fRec33[0] = (0.0f - (fSlow14 * (((fSlow16 * fRec34[0]) + (fSlow15 * fRec35[0])) - (fSlow11 * fTemp17))));
			float fTemp18 = (fRec33[0] + (fSlow13 * fTemp17));
			fRec32[0] = max((fRec32[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp18))))));
			fVbargraph6 = FAUSTFLOAT(fRec32[0]);
			float fTemp19 = (1.12929443e-17f * fTemp18);
			float fTemp20 = (fRec3[0] * float(input6[i]));
			fRec38[0] = (fRec37[1] + fRec38[1]);
			fRec39[0] = (fRec38[1] + fRec39[1]);
			fRec37[0] = (fSlow14 * ((fSlow11 * fTemp20) - ((fSlow16 * fRec38[0]) + (fSlow15 * fRec39[0]))));
			float fTemp21 = (fRec37[0] + (fSlow13 * fTemp20));
			fRec36[0] = max((fRec36[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp21))))));
			fVbargraph7 = FAUSTFLOAT(fRec36[0]);
			float fTemp22 = (0.106479429f * fTemp21);
			float fTemp23 = (fRec3[0] * float(input2[i]));
			fRec42[0] = (fRec41[1] + fRec42[1]);
			fRec41[0] = (fSlow17 * ((fSlow11 * fTemp23) - (fSlow18 * fRec42[0])));
			float fTemp24 = (fRec41[0] + (fSlow13 * fTemp23));
			fRec40[0] = max((fRec40[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp24))))));
			fVbargraph8 = FAUSTFLOAT(fRec40[0]);
			float fTemp25 = (0.0824786127f * fTemp24);
			float fTemp26 = (fRec3[0] * float(input3[i]));
			fRec45[0] = (fRec44[1] + fRec45[1]);
			fRec44[0] = (fSlow17 * ((fSlow11 * fTemp26) - (fSlow18 * fRec45[0])));
			float fTemp27 = (fRec44[0] + (fSlow13 * fTemp26));
			fRec43[0] = max((fRec43[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp27))))));
			fVbargraph9 = FAUSTFLOAT(fRec43[0]);
			float fTemp28 = (5.05035826e-18f * fTemp27);
			float fTemp29 = (fRec1[0] * (fTemp1 + (fTemp4 + (fTemp7 + (fTemp10 + (fTemp13 + (fTemp16 + (fTemp19 + (fTemp22 + (fTemp25 + fTemp28))))))))));
			fRec0[0] = max((fRec0[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp29))))));
			fVbargraph10 = FAUSTFLOAT(fRec0[0]);
			output0[i] = FAUSTFLOAT(fTemp29);
			float fTemp30 = (0.0996023864f * fTemp3);
			float fTemp31 = (0.0824786127f * fTemp27);
			float fTemp32 = (0.0922138914f * fTemp15);
			float fTemp33 = (0.0532397144f * fTemp21);
			float fTemp34 = (0.0771516785f * fTemp9);
			float fTemp35 = (fRec1[0] * ((fTemp1 + (fTemp30 + (fTemp31 + fTemp32))) - (fTemp33 + fTemp34)));
			fRec46[0] = max((fRec46[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp35))))));
			fVbargraph11 = FAUSTFLOAT(fRec46[0]);
			output1[i] = FAUSTFLOAT(fTemp35);
			fRec50[0] = (fRec49[1] + fRec50[1]);
			fRec51[0] = (fRec50[1] + fRec51[1]);
			float fTemp36 = (fRec3[0] * float(input4[i]));
			fRec49[0] = (0.0f - (fSlow14 * (((fSlow16 * fRec50[0]) + (fSlow15 * fRec51[0])) - (fSlow11 * fTemp36))));
			float fTemp37 = (fRec49[0] + (fSlow13 * fTemp36));
			fRec48[0] = max((fRec48[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp37))))));
			fVbargraph12 = FAUSTFLOAT(fRec48[0]);
			float fTemp38 = (fRec3[0] * float(input1[i]));
			fRec54[0] = (fRec53[1] + fRec54[1]);
			fRec53[0] = (fSlow17 * ((fSlow11 * fTemp38) - (fSlow18 * fRec54[0])));
			float fTemp39 = (fRec53[0] + (fSlow13 * fTemp38));
			fRec52[0] = max((fRec52[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp39))))));
			fVbargraph13 = FAUSTFLOAT(fRec52[0]);
			float fTemp40 = (0.0824786127f * fTemp39);
			fRec58[0] = (fRec57[1] + fRec58[1]);
			fRec59[0] = (fRec58[1] + fRec59[1]);
			float fTemp41 = (fRec3[0] * float(input11[i]));
			fRec57[0] = (0.0f - (fSlow8 * (((fSlow10 * fRec58[0]) + (fSlow9 * fRec59[0])) - (fSlow11 * fTemp41))));
			fRec60[0] = (fRec56[1] + fRec60[1]);
			fRec56[0] = (fSlow5 * (fRec57[0] - (fSlow12 * fRec60[0])));
			float fTemp42 = (fRec56[0] + (fSlow13 * fTemp41));
			fRec55[0] = max((fRec55[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp42))))));
			fVbargraph14 = FAUSTFLOAT(fRec55[0]);
			float fTemp43 = (0.0771516785f * fTemp42);
			fRec64[0] = (fRec63[1] + fRec64[1]);
			fRec65[0] = (fRec64[1] + fRec65[1]);
			float fTemp44 = (fRec3[0] * float(input9[i]));
			fRec63[0] = (0.0f - (fSlow8 * (((fSlow10 * fRec64[0]) + (fSlow9 * fRec65[0])) - (fSlow11 * fTemp44))));
			fRec66[0] = (fRec62[1] + fRec66[1]);
			fRec62[0] = (fSlow5 * (fRec63[0] - (fSlow12 * fRec66[0])));
			float fTemp45 = (fRec62[0] + (fSlow13 * fTemp44));
			fRec61[0] = max((fRec61[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp45))))));
			fVbargraph15 = FAUSTFLOAT(fRec61[0]);
			float fTemp46 = (0.0996023864f * fTemp45);
			float fTemp47 = (fRec1[0] * ((fTemp1 + ((1.12929443e-17f * fTemp37) + (fTemp28 + fTemp40))) - (fTemp43 + (fTemp46 + ((1.82966611e-17f * fTemp3) + ((4.72417778e-18f * fTemp9) + (fTemp32 + fTemp33)))))));
			fRec47[0] = max((fRec47[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp47))))));
			fVbargraph16 = FAUSTFLOAT(fRec47[0]);
			output2[i] = FAUSTFLOAT(fTemp47);
			float fTemp48 = (fRec1[0] * ((fTemp1 + ((3.65933222e-17f * fTemp45) + ((1.01007165e-17f * fTemp39) + (fTemp32 + fTemp34)))) - ((9.44835557e-18f * fTemp42) + ((2.25858887e-17f * fTemp37) + (fTemp30 + (fTemp31 + fTemp33))))));
			fRec67[0] = max((fRec67[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp48))))));
			fVbargraph17 = FAUSTFLOAT(fRec67[0]);
			output3[i] = FAUSTFLOAT(fTemp48);
			float fTemp49 = (fRec1[0] * ((fTemp1 + (fTemp43 + (fTemp46 + ((3.3878833e-17f * fTemp37) + ((1.41725321e-17f * fTemp9) + (5.48899849e-17f * fTemp3)))))) - (fTemp40 + (fTemp32 + (fTemp33 + (1.51510752e-17f * fTemp27))))));
			fRec68[0] = max((fRec68[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp49))))));
			fVbargraph18 = FAUSTFLOAT(fRec68[0]);
			output4[i] = FAUSTFLOAT(fTemp49);
			float fTemp50 = (fRec1[0] * ((fTemp1 + (fTemp4 + (fTemp10 + (fTemp16 + (fTemp28 + fTemp22))))) - (fTemp7 + (fTemp13 + (fTemp25 + fTemp19)))));
			fRec69[0] = max((fRec69[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp50))))));
			fVbargraph19 = FAUSTFLOAT(fRec69[0]);
			output5[i] = FAUSTFLOAT(fTemp50);
			float fTemp51 = (0.0380952395f * fTemp0);
			float fTemp52 = (0.0281718094f * fTemp3);
			float fTemp53 = (0.0690065548f * fTemp6);
			float fTemp54 = (0.0654653683f * fTemp9);
			float fTemp55 = (0.0368855558f * fTemp15);
			float fTemp56 = (0.0737711117f * fTemp18);
			float fTemp57 = (0.0212958846f * fTemp21);
			float fTemp58 = (0.0466569476f * fTemp24);
			float fTemp59 = (0.0466569476f * fTemp27);
			float fTemp60 = (fTemp58 + fTemp59);
			float fTemp61 = (0.0178174153f * fTemp12);
			float fTemp62 = (fRec1[0] * ((fTemp51 + (fTemp52 + (fTemp53 + (fTemp54 + (fTemp55 + (fTemp56 + (fTemp57 + fTemp60))))))) - fTemp61));
			fRec70[0] = max((fRec70[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp62))))));
			fVbargraph20 = FAUSTFLOAT(fRec70[0]);
			output6[i] = FAUSTFLOAT(fTemp62);
			fRec75[0] = (fRec74[1] + fRec75[1]);
			fRec76[0] = (fRec75[1] + fRec76[1]);
			float fTemp63 = (fRec3[0] * float(input10[i]));
			fRec74[0] = (0.0f - (fSlow8 * (((fSlow10 * fRec75[0]) + (fSlow9 * fRec76[0])) - (fSlow11 * fTemp63))));
			fRec77[0] = (fRec73[1] + fRec77[1]);
			fRec73[0] = (fSlow5 * (fRec74[0] - (fSlow12 * fRec77[0])));
			float fTemp64 = (fRec73[0] + (fSlow13 * fTemp63));
			fRec72[0] = max((fRec72[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp64))))));
			fVbargraph21 = FAUSTFLOAT(fRec72[0]);
			float fTemp65 = (8.45086591e-18f * fTemp64);
			fRec80[0] = (fRec79[1] + fRec80[1]);
			fRec81[0] = (fRec80[1] + fRec81[1]);
			float fTemp66 = (fRec3[0] * float(input5[i]));
			fRec79[0] = (0.0f - (fSlow14 * (((fSlow16 * fRec80[0]) + (fSlow15 * fRec81[0])) - (fSlow11 * fTemp66))));
			float fTemp67 = (fRec79[0] + (fSlow13 * fTemp66));
			fRec78[0] = max((fRec78[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp67))))));
			fVbargraph22 = FAUSTFLOAT(fRec78[0]);
			float fTemp68 = (0.0737711117f * fTemp67);
			float fTemp69 = (0.0654653683f * fTemp42);
			float fTemp70 = (4.5171779e-18f * fTemp37);
			float fTemp71 = (0.0466569476f * fTemp39);
			float fTemp72 = (4.00859748e-18f * fTemp9);
			float fTemp73 = (4.5171779e-18f * fTemp18);
			float fTemp74 = (2.85691412e-18f * fTemp27);
			float fTemp75 = (0.0281718094f * fTemp45);
			float fTemp76 = (5.17507725e-18f * fTemp3);
			float fTemp77 = (fRec1[0] * ((fTemp51 + (fTemp65 + (fTemp68 + (fTemp69 + (fTemp70 + (fTemp71 + (fTemp72 + (fTemp73 + (fTemp57 + (fTemp58 + fTemp74)))))))))) - (fTemp75 + ((fTemp53 + (fTemp55 + fTemp61)) + fTemp76))));
			fRec71[0] = max((fRec71[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp77))))));
			fVbargraph23 = FAUSTFLOAT(fRec71[0]);
			output7[i] = FAUSTFLOAT(fTemp77);
			float fTemp78 = (9.03435581e-18f * fTemp67);
			float fTemp79 = (8.01719497e-18f * fTemp42);
			float fTemp80 = (1.03501545e-17f * fTemp45);
			float fTemp81 = (5.71382825e-18f * fTemp39);
			float fTemp82 = (fTemp58 + fTemp57);
			float fTemp83 = (1.69017318e-17f * fTemp64);
			float fTemp84 = (9.03435581e-18f * fTemp37);
			float fTemp85 = (fRec1[0] * ((fTemp51 + (fTemp78 + (fTemp79 + (fTemp80 + (fTemp81 + (fTemp53 + (fTemp55 + fTemp82))))))) - (fTemp83 + (fTemp84 + (fTemp52 + (fTemp54 + (fTemp61 + (fTemp59 + fTemp56))))))));
			fRec82[0] = max((fRec82[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp85))))));
			fVbargraph24 = FAUSTFLOAT(fRec82[0]);
			output8[i] = FAUSTFLOAT(fTemp85);
			float fTemp86 = (2.53525977e-17f * fTemp64);
			float fTemp87 = (1.35515337e-17f * fTemp37);
			float fTemp88 = (1.55252321e-17f * fTemp3);
			float fTemp89 = (8.57074258e-18f * fTemp27);
			float fTemp90 = (1.35515337e-17f * fTemp18);
			float fTemp91 = (1.20257925e-17f * fTemp9);
			float fTemp92 = (fRec1[0] * ((fTemp51 + (fTemp86 + (fTemp75 + (fTemp87 + (fTemp88 + fTemp82))))) - (fTemp68 + (fTemp69 + (fTemp71 + (fTemp53 + ((fTemp61 + (fTemp55 + (fTemp89 + fTemp90))) + fTemp91)))))));
			fRec83[0] = max((fRec83[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp92))))));
			fVbargraph25 = FAUSTFLOAT(fRec83[0]);
			output9[i] = FAUSTFLOAT(fTemp92);
			float fTemp93 = (0.0737711117f * fTemp37);
			float fTemp94 = (0.0466569476f * fTemp39);
			float fTemp95 = (0.0436435789f * fTemp42);
			float fTemp96 = (0.0425917692f * fTemp21);
			float fTemp97 = (0.0436435789f * fTemp9);
			float fTemp98 = (fRec1[0] * ((fTemp51 + ((0.0563436188f * fTemp45) + (fTemp93 + (fTemp94 + (fTemp59 + (4.5171779e-18f * fTemp15)))))) - (fTemp95 + ((fTemp96 + fTemp97) + (0.0563436188f * fTemp3)))));
			fRec84[0] = max((fRec84[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp98))))));
			fVbargraph26 = FAUSTFLOAT(fRec84[0]);
			output10[i] = FAUSTFLOAT(fTemp98);
			float fTemp99 = (0.0436435789f * fTemp42);
			float fTemp100 = (fRec1[0] * ((fTemp51 + ((0.0563436188f * fTemp45) + (fTemp71 + ((0.0436435789f * fTemp9) + (0.0563436188f * fTemp3))))) - (fTemp99 + (fTemp93 + ((fTemp96 + (0.0466569476f * fTemp27)) + (1.35515337e-17f * fTemp15))))));
			fRec85[0] = max((fRec85[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp100))))));
			fVbargraph27 = FAUSTFLOAT(fRec85[0]);
			output11[i] = FAUSTFLOAT(fTemp100);
			float fTemp101 = (fRec1[0] * ((fTemp51 + (fTemp95 + (fTemp93 + ((0.0563436188f * fTemp3) + (fTemp97 + (2.25858887e-17f * fTemp15)))))) - ((fTemp94 + (fTemp59 + fTemp96)) + (0.0563436188f * fTemp45))));
			fRec86[0] = max((fRec86[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp101))))));
			fVbargraph28 = FAUSTFLOAT(fRec86[0]);
			output12[i] = FAUSTFLOAT(fTemp101);
			float fTemp102 = (fRec1[0] * ((fTemp51 + (fTemp99 + (0.0466569476f * fTemp27))) - ((0.0563436188f * fTemp45) + (fTemp93 + (fTemp71 + (((fTemp96 + (3.16202428e-17f * fTemp15)) + (0.0436435789f * fTemp9)) + (0.0563436188f * fTemp3)))))));
			fRec87[0] = max((fRec87[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp102))))));
			fVbargraph29 = FAUSTFLOAT(fRec87[0]);
			output13[i] = FAUSTFLOAT(fTemp102);
			float fTemp103 = (fRec1[0] * ((fTemp51 + (fTemp52 + (fTemp54 + (fTemp61 + (fTemp55 + (fTemp59 + fTemp57)))))) - (fTemp53 + (fTemp58 + fTemp56))));
			fRec88[0] = max((fRec88[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp103))))));
			fVbargraph30 = FAUSTFLOAT(fRec88[0]);
			output14[i] = FAUSTFLOAT(fTemp103);
			float fTemp104 = (fRec1[0] * ((fTemp51 + (fTemp69 + (fTemp70 + (fTemp71 + (fTemp53 + (fTemp72 + (fTemp61 + (fTemp57 + fTemp74)))))))) - (fTemp65 + (fTemp68 + (fTemp75 + (fTemp76 + (fTemp55 + (fTemp58 + fTemp73))))))));
			fRec89[0] = max((fRec89[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp104))))));
			fVbargraph31 = FAUSTFLOAT(fRec89[0]);
			output15[i] = FAUSTFLOAT(fTemp104);
			float fTemp105 = (fRec1[0] * ((fTemp51 + (fTemp83 + (fTemp79 + (fTemp80 + (fTemp81 + (fTemp61 + (fTemp55 + (fTemp57 + fTemp56)))))))) - (fTemp78 + (fTemp84 + (fTemp52 + (fTemp53 + (fTemp54 + fTemp60)))))));
			fRec90[0] = max((fRec90[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp105))))));
			fVbargraph32 = FAUSTFLOAT(fRec90[0]);
			output16[i] = FAUSTFLOAT(fTemp105);
			float fTemp106 = (fRec1[0] * ((fTemp51 + (fTemp68 + (fTemp75 + (fTemp87 + (fTemp88 + (fTemp53 + (fTemp61 + (fTemp57 + fTemp90)))))))) - (fTemp86 + (fTemp69 + (fTemp71 + (fTemp91 + (fTemp55 + (fTemp58 + fTemp89))))))));
			fRec91[0] = max((fRec91[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp106))))));
			fVbargraph33 = FAUSTFLOAT(fRec91[0]);
			output17[i] = FAUSTFLOAT(fTemp106);
			float fTemp107 = (fTemp24 + fTemp0);
			float fTemp108 = (0.032142859f * fTemp107);
			float fTemp109 = (0.0633865669f * fTemp64);
			float fTemp110 = (0.0414962508f * fTemp67);
			float fTemp111 = (0.0200445931f * fTemp42);
			float fTemp112 = (0.025877459f * fTemp45);
			float fTemp113 = (0.0414962508f * fTemp37);
			float fTemp114 = (0.032142859f * fTemp39);
			float fTemp115 = (7.97957397e-18f * fTemp21);
			float fTemp116 = ((0.032142859f * fTemp27) + fTemp115);
			float fTemp117 = (0.0414962508f * fTemp18);
			float fTemp118 = (2.54091252e-18f * fTemp15);
			float fTemp119 = (0.0200445931f * fTemp9);
			float fTemp120 = (3.88130804e-18f * fTemp6);
			float fTemp121 = (0.0327326842f * fTemp12);
			float fTemp122 = (0.025877459f * fTemp3);
			float fTemp123 = (fRec1[0] * ((fTemp108 + (fTemp109 + (fTemp110 + (fTemp111 + (fTemp112 + (fTemp113 + (fTemp114 + ((((fTemp116 + fTemp117) + fTemp118) + fTemp119) + fTemp120)))))))) - (fTemp121 + fTemp122)));
			fRec92[0] = max((fRec92[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp123))))));
			fVbargraph34 = FAUSTFLOAT(fRec92[0]);
			output18[i] = FAUSTFLOAT(fTemp123);
			float fTemp124 = (0.0414962508f * fTemp67);
			float fTemp125 = (0.0200445931f * fTemp42);
			float fTemp126 = (0.025877459f * fTemp45);
			float fTemp127 = (0.032142859f * fTemp39);
			float fTemp128 = (0.025877459f * fTemp3);
			float fTemp129 = (0.032142859f * fTemp27);
			float fTemp130 = (0.0414962508f * fTemp18);
			float fTemp131 = (7.62273735e-18f * fTemp15);
			float fTemp132 = (0.0200445931f * fTemp9);
			float fTemp133 = (1.16439237e-17f * fTemp6);
			float fTemp134 = (fRec1[0] * ((fTemp108 + (fTemp124 + (fTemp125 + (fTemp126 + (fTemp127 + (fTemp115 + fTemp128)))))) - (fTemp109 + (fTemp113 + (((fTemp121 + ((fTemp129 + fTemp130) + fTemp131)) + fTemp132) + fTemp133)))));
			fRec93[0] = max((fRec93[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp134))))));
			fVbargraph35 = FAUSTFLOAT(fRec93[0]);
			output19[i] = FAUSTFLOAT(fTemp134);
			float fTemp135 = (1.27045628e-17f * fTemp15);
			float fTemp136 = (1.94065389e-17f * fTemp6);
			float fTemp137 = (0.025877459f * fTemp3);
			float fTemp138 = (0.025877459f * fTemp45);
			float fTemp139 = (0.0200445931f * fTemp9);
			float fTemp140 = (fRec1[0] * ((fTemp109 + (fTemp113 + (((fTemp115 + fTemp135) + fTemp136) + fTemp137))) - ((0.032142859f * (fTemp27 - fTemp107)) + (fTemp110 + (fTemp111 + (fTemp138 + (fTemp114 + ((fTemp117 + fTemp121) + fTemp139))))))));
			fRec94[0] = max((fRec94[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp140))))));
			fVbargraph36 = FAUSTFLOAT(fRec94[0]);
			output20[i] = FAUSTFLOAT(fTemp140);
			float fTemp141 = (fTemp115 + fTemp129);
			float fTemp142 = (0.0414962508f * fTemp18);
			float fTemp143 = (0.0200445931f * fTemp42);
			float fTemp144 = (0.025877459f * fTemp45);
			float fTemp145 = (1.77863883e-17f * fTemp15);
			float fTemp146 = (2.71691558e-17f * fTemp6);
			float fTemp147 = (0.025877459f * fTemp3);
			float fTemp148 = (fRec1[0] * (((fTemp132 + (fTemp141 + fTemp142)) + (0.032142859f * (fTemp107 - fTemp39))) - (fTemp109 + (fTemp124 + (fTemp143 + (fTemp144 + (fTemp113 + (((fTemp121 + fTemp145) + fTemp146) + fTemp147))))))));
			fRec95[0] = max((fRec95[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp148))))));
			fVbargraph37 = FAUSTFLOAT(fRec95[0]);
			output21[i] = FAUSTFLOAT(fTemp148);
			float fTemp149 = (0.032142859f * (fTemp0 - fTemp24));
			float fTemp150 = (fRec1[0] * ((fTemp149 + (fTemp111 + (fTemp112 + (fTemp113 + (fTemp114 + (fTemp119 + (fTemp121 + (fTemp116 + fTemp118)))))))) - (fTemp109 + (fTemp110 + (fTemp122 + (fTemp117 + fTemp120))))));
			fRec96[0] = max((fRec96[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp150))))));
			fVbargraph38 = FAUSTFLOAT(fRec96[0]);
			output22[i] = FAUSTFLOAT(fTemp150);
			float fTemp151 = (fRec1[0] * ((fTemp149 + (fTemp109 + (fTemp125 + (fTemp126 + (fTemp127 + (fTemp128 + (fTemp133 + (fTemp121 + (fTemp115 + fTemp130))))))))) - (fTemp124 + (fTemp113 + (fTemp132 + (fTemp129 + fTemp131))))));
			fRec97[0] = max((fRec97[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp151))))));
			fVbargraph39 = FAUSTFLOAT(fRec97[0]);
			output23[i] = FAUSTFLOAT(fTemp151);
			float fTemp152 = (fRec1[0] * ((fTemp110 + (fTemp113 + (fTemp137 + (fTemp121 + (fTemp135 + (fTemp115 + fTemp117)))))) - ((0.032142859f * ((fTemp24 + fTemp27) - fTemp0)) + (fTemp109 + (fTemp111 + (fTemp138 + (fTemp114 + (fTemp136 + fTemp139))))))));
			fRec98[0] = max((fRec98[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp152))))));
			fVbargraph40 = FAUSTFLOAT(fRec98[0]);
			output24[i] = FAUSTFLOAT(fTemp152);
			float fTemp153 = (fRec1[0] * ((fTemp109 + (fTemp124 + (fTemp146 + (fTemp132 + (fTemp121 + fTemp141))))) - ((0.032142859f * ((fTemp24 + fTemp39) - fTemp0)) + (fTemp143 + (fTemp144 + (fTemp113 + (fTemp147 + (fTemp142 + fTemp145))))))));
			fRec99[0] = max((fRec99[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp153))))));
			fVbargraph41 = FAUSTFLOAT(fRec99[0]);
			output25[i] = FAUSTFLOAT(fTemp153);
			fRec1[1] = fRec1[0];
			fRec3[1] = fRec3[0];
			fRec2[1] = fRec2[0];
			fRec7[1] = fRec7[0];
			fRec8[1] = fRec8[0];
			fRec6[1] = fRec6[0];
			fRec9[1] = fRec9[0];
			fRec5[1] = fRec5[0];
			fRec4[1] = fRec4[0];
			fRec13[1] = fRec13[0];
			fRec14[1] = fRec14[0];
			fRec12[1] = fRec12[0];
			fRec15[1] = fRec15[0];
			fRec11[1] = fRec11[0];
			fRec10[1] = fRec10[0];
			fRec19[1] = fRec19[0];
			fRec20[1] = fRec20[0];
			fRec18[1] = fRec18[0];
			fRec21[1] = fRec21[0];
			fRec17[1] = fRec17[0];
			fRec16[1] = fRec16[0];
			fRec25[1] = fRec25[0];
			fRec26[1] = fRec26[0];
			fRec24[1] = fRec24[0];
			fRec27[1] = fRec27[0];
			fRec23[1] = fRec23[0];
			fRec22[1] = fRec22[0];
			fRec30[1] = fRec30[0];
			fRec31[1] = fRec31[0];
			fRec29[1] = fRec29[0];
			fRec28[1] = fRec28[0];
			fRec34[1] = fRec34[0];
			fRec35[1] = fRec35[0];
			fRec33[1] = fRec33[0];
			fRec32[1] = fRec32[0];
			fRec38[1] = fRec38[0];
			fRec39[1] = fRec39[0];
			fRec37[1] = fRec37[0];
			fRec36[1] = fRec36[0];
			fRec42[1] = fRec42[0];
			fRec41[1] = fRec41[0];
			fRec40[1] = fRec40[0];
			fRec45[1] = fRec45[0];
			fRec44[1] = fRec44[0];
			fRec43[1] = fRec43[0];
			fRec0[1] = fRec0[0];
			fRec46[1] = fRec46[0];
			fRec50[1] = fRec50[0];
			fRec51[1] = fRec51[0];
			fRec49[1] = fRec49[0];
			fRec48[1] = fRec48[0];
			fRec54[1] = fRec54[0];
			fRec53[1] = fRec53[0];
			fRec52[1] = fRec52[0];
			fRec58[1] = fRec58[0];
			fRec59[1] = fRec59[0];
			fRec57[1] = fRec57[0];
			fRec60[1] = fRec60[0];
			fRec56[1] = fRec56[0];
			fRec55[1] = fRec55[0];
			fRec64[1] = fRec64[0];
			fRec65[1] = fRec65[0];
			fRec63[1] = fRec63[0];
			fRec66[1] = fRec66[0];
			fRec62[1] = fRec62[0];
			fRec61[1] = fRec61[0];
			fRec47[1] = fRec47[0];
			fRec67[1] = fRec67[0];
			fRec68[1] = fRec68[0];
			fRec69[1] = fRec69[0];
			fRec70[1] = fRec70[0];
			fRec75[1] = fRec75[0];
			fRec76[1] = fRec76[0];
			fRec74[1] = fRec74[0];
			fRec77[1] = fRec77[0];
			fRec73[1] = fRec73[0];
			fRec72[1] = fRec72[0];
			fRec80[1] = fRec80[0];
			fRec81[1] = fRec81[0];
			fRec79[1] = fRec79[0];
			fRec78[1] = fRec78[0];
			fRec71[1] = fRec71[0];
			fRec82[1] = fRec82[0];
			fRec83[1] = fRec83[0];
			fRec84[1] = fRec84[0];
			fRec85[1] = fRec85[0];
			fRec86[1] = fRec86[0];
			fRec87[1] = fRec87[0];
			fRec88[1] = fRec88[0];
			fRec89[1] = fRec89[0];
			fRec90[1] = fRec90[0];
			fRec91[1] = fRec91[0];
			fRec92[1] = fRec92[0];
			fRec93[1] = fRec93[0];
			fRec94[1] = fRec94[0];
			fRec95[1] = fRec95[0];
			fRec96[1] = fRec96[0];
			fRec97[1] = fRec97[0];
			fRec98[1] = fRec98[0];
			fRec99[1] = fRec99[0];
			
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
