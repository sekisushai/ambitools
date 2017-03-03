/* ------------------------------------------------------------
author: "Pierre Lecomte"
copyright: "(c) Pierre Lecomte 2014"
license: "GPL"
name: "HOADecLebedev502"
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
	float fRec6[2];
	float fRec7[2];
	float fRec5[2];
	float fRec4[2];
	float fRec10[2];
	float fRec11[2];
	float fRec9[2];
	float fRec8[2];
	float fRec14[2];
	float fRec15[2];
	float fRec13[2];
	float fRec12[2];
	float fRec18[2];
	float fRec17[2];
	float fRec16[2];
	float fRec21[2];
	float fRec20[2];
	float fRec19[2];
	float fRec0[2];
	float fRec22[2];
	float fRec26[2];
	float fRec27[2];
	float fRec25[2];
	float fRec24[2];
	float fRec30[2];
	float fRec29[2];
	float fRec28[2];
	float fRec23[2];
	float fRec31[2];
	float fRec32[2];
	float fRec33[2];
	float fRec34[2];
	float fRec38[2];
	float fRec39[2];
	float fRec37[2];
	float fRec36[2];
	float fRec35[2];
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
	float fRec77[2];
	float fRec78[2];
	float fRec79[2];
	float fRec80[2];
	float fRec81[2];
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
	FAUSTFLOAT fVbargraph1;
	FAUSTFLOAT fVbargraph2;
	FAUSTFLOAT fVbargraph3;
	float fConst5;
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
	FAUSTFLOAT fVbargraph50;
	FAUSTFLOAT fVbargraph51;
	FAUSTFLOAT fVbargraph52;
	FAUSTFLOAT fVbargraph53;
	FAUSTFLOAT fVbargraph54;
	FAUSTFLOAT fVbargraph55;
	FAUSTFLOAT fVbargraph56;
	FAUSTFLOAT fVbargraph57;
	FAUSTFLOAT fVbargraph58;
	
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
		m->declare("name", "HOADecLebedev502");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
		m->declare("version", "1.0");
	}

	virtual int getNumInputs() {
		return 9;
		
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
			fRec6[i3] = 0.0f;
			
		}
		for (int i4 = 0; (i4 < 2); i4 = (i4 + 1)) {
			fRec7[i4] = 0.0f;
			
		}
		for (int i5 = 0; (i5 < 2); i5 = (i5 + 1)) {
			fRec5[i5] = 0.0f;
			
		}
		for (int i6 = 0; (i6 < 2); i6 = (i6 + 1)) {
			fRec4[i6] = 0.0f;
			
		}
		for (int i7 = 0; (i7 < 2); i7 = (i7 + 1)) {
			fRec10[i7] = 0.0f;
			
		}
		for (int i8 = 0; (i8 < 2); i8 = (i8 + 1)) {
			fRec11[i8] = 0.0f;
			
		}
		for (int i9 = 0; (i9 < 2); i9 = (i9 + 1)) {
			fRec9[i9] = 0.0f;
			
		}
		for (int i10 = 0; (i10 < 2); i10 = (i10 + 1)) {
			fRec8[i10] = 0.0f;
			
		}
		for (int i11 = 0; (i11 < 2); i11 = (i11 + 1)) {
			fRec14[i11] = 0.0f;
			
		}
		for (int i12 = 0; (i12 < 2); i12 = (i12 + 1)) {
			fRec15[i12] = 0.0f;
			
		}
		for (int i13 = 0; (i13 < 2); i13 = (i13 + 1)) {
			fRec13[i13] = 0.0f;
			
		}
		for (int i14 = 0; (i14 < 2); i14 = (i14 + 1)) {
			fRec12[i14] = 0.0f;
			
		}
		for (int i15 = 0; (i15 < 2); i15 = (i15 + 1)) {
			fRec18[i15] = 0.0f;
			
		}
		for (int i16 = 0; (i16 < 2); i16 = (i16 + 1)) {
			fRec17[i16] = 0.0f;
			
		}
		for (int i17 = 0; (i17 < 2); i17 = (i17 + 1)) {
			fRec16[i17] = 0.0f;
			
		}
		for (int i18 = 0; (i18 < 2); i18 = (i18 + 1)) {
			fRec21[i18] = 0.0f;
			
		}
		for (int i19 = 0; (i19 < 2); i19 = (i19 + 1)) {
			fRec20[i19] = 0.0f;
			
		}
		for (int i20 = 0; (i20 < 2); i20 = (i20 + 1)) {
			fRec19[i20] = 0.0f;
			
		}
		for (int i21 = 0; (i21 < 2); i21 = (i21 + 1)) {
			fRec0[i21] = 0.0f;
			
		}
		for (int i22 = 0; (i22 < 2); i22 = (i22 + 1)) {
			fRec22[i22] = 0.0f;
			
		}
		for (int i23 = 0; (i23 < 2); i23 = (i23 + 1)) {
			fRec26[i23] = 0.0f;
			
		}
		for (int i24 = 0; (i24 < 2); i24 = (i24 + 1)) {
			fRec27[i24] = 0.0f;
			
		}
		for (int i25 = 0; (i25 < 2); i25 = (i25 + 1)) {
			fRec25[i25] = 0.0f;
			
		}
		for (int i26 = 0; (i26 < 2); i26 = (i26 + 1)) {
			fRec24[i26] = 0.0f;
			
		}
		for (int i27 = 0; (i27 < 2); i27 = (i27 + 1)) {
			fRec30[i27] = 0.0f;
			
		}
		for (int i28 = 0; (i28 < 2); i28 = (i28 + 1)) {
			fRec29[i28] = 0.0f;
			
		}
		for (int i29 = 0; (i29 < 2); i29 = (i29 + 1)) {
			fRec28[i29] = 0.0f;
			
		}
		for (int i30 = 0; (i30 < 2); i30 = (i30 + 1)) {
			fRec23[i30] = 0.0f;
			
		}
		for (int i31 = 0; (i31 < 2); i31 = (i31 + 1)) {
			fRec31[i31] = 0.0f;
			
		}
		for (int i32 = 0; (i32 < 2); i32 = (i32 + 1)) {
			fRec32[i32] = 0.0f;
			
		}
		for (int i33 = 0; (i33 < 2); i33 = (i33 + 1)) {
			fRec33[i33] = 0.0f;
			
		}
		for (int i34 = 0; (i34 < 2); i34 = (i34 + 1)) {
			fRec34[i34] = 0.0f;
			
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
			fRec35[i39] = 0.0f;
			
		}
		for (int i40 = 0; (i40 < 2); i40 = (i40 + 1)) {
			fRec40[i40] = 0.0f;
			
		}
		for (int i41 = 0; (i41 < 2); i41 = (i41 + 1)) {
			fRec41[i41] = 0.0f;
			
		}
		for (int i42 = 0; (i42 < 2); i42 = (i42 + 1)) {
			fRec42[i42] = 0.0f;
			
		}
		for (int i43 = 0; (i43 < 2); i43 = (i43 + 1)) {
			fRec43[i43] = 0.0f;
			
		}
		for (int i44 = 0; (i44 < 2); i44 = (i44 + 1)) {
			fRec44[i44] = 0.0f;
			
		}
		for (int i45 = 0; (i45 < 2); i45 = (i45 + 1)) {
			fRec45[i45] = 0.0f;
			
		}
		for (int i46 = 0; (i46 < 2); i46 = (i46 + 1)) {
			fRec46[i46] = 0.0f;
			
		}
		for (int i47 = 0; (i47 < 2); i47 = (i47 + 1)) {
			fRec47[i47] = 0.0f;
			
		}
		for (int i48 = 0; (i48 < 2); i48 = (i48 + 1)) {
			fRec48[i48] = 0.0f;
			
		}
		for (int i49 = 0; (i49 < 2); i49 = (i49 + 1)) {
			fRec49[i49] = 0.0f;
			
		}
		for (int i50 = 0; (i50 < 2); i50 = (i50 + 1)) {
			fRec50[i50] = 0.0f;
			
		}
		for (int i51 = 0; (i51 < 2); i51 = (i51 + 1)) {
			fRec51[i51] = 0.0f;
			
		}
		for (int i52 = 0; (i52 < 2); i52 = (i52 + 1)) {
			fRec52[i52] = 0.0f;
			
		}
		for (int i53 = 0; (i53 < 2); i53 = (i53 + 1)) {
			fRec53[i53] = 0.0f;
			
		}
		for (int i54 = 0; (i54 < 2); i54 = (i54 + 1)) {
			fRec54[i54] = 0.0f;
			
		}
		for (int i55 = 0; (i55 < 2); i55 = (i55 + 1)) {
			fRec55[i55] = 0.0f;
			
		}
		for (int i56 = 0; (i56 < 2); i56 = (i56 + 1)) {
			fRec56[i56] = 0.0f;
			
		}
		for (int i57 = 0; (i57 < 2); i57 = (i57 + 1)) {
			fRec57[i57] = 0.0f;
			
		}
		for (int i58 = 0; (i58 < 2); i58 = (i58 + 1)) {
			fRec58[i58] = 0.0f;
			
		}
		for (int i59 = 0; (i59 < 2); i59 = (i59 + 1)) {
			fRec59[i59] = 0.0f;
			
		}
		for (int i60 = 0; (i60 < 2); i60 = (i60 + 1)) {
			fRec60[i60] = 0.0f;
			
		}
		for (int i61 = 0; (i61 < 2); i61 = (i61 + 1)) {
			fRec61[i61] = 0.0f;
			
		}
		for (int i62 = 0; (i62 < 2); i62 = (i62 + 1)) {
			fRec62[i62] = 0.0f;
			
		}
		for (int i63 = 0; (i63 < 2); i63 = (i63 + 1)) {
			fRec63[i63] = 0.0f;
			
		}
		for (int i64 = 0; (i64 < 2); i64 = (i64 + 1)) {
			fRec64[i64] = 0.0f;
			
		}
		for (int i65 = 0; (i65 < 2); i65 = (i65 + 1)) {
			fRec65[i65] = 0.0f;
			
		}
		for (int i66 = 0; (i66 < 2); i66 = (i66 + 1)) {
			fRec66[i66] = 0.0f;
			
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
			fRec71[i71] = 0.0f;
			
		}
		for (int i72 = 0; (i72 < 2); i72 = (i72 + 1)) {
			fRec72[i72] = 0.0f;
			
		}
		for (int i73 = 0; (i73 < 2); i73 = (i73 + 1)) {
			fRec73[i73] = 0.0f;
			
		}
		for (int i74 = 0; (i74 < 2); i74 = (i74 + 1)) {
			fRec74[i74] = 0.0f;
			
		}
		for (int i75 = 0; (i75 < 2); i75 = (i75 + 1)) {
			fRec75[i75] = 0.0f;
			
		}
		for (int i76 = 0; (i76 < 2); i76 = (i76 + 1)) {
			fRec76[i76] = 0.0f;
			
		}
		for (int i77 = 0; (i77 < 2); i77 = (i77 + 1)) {
			fRec77[i77] = 0.0f;
			
		}
		for (int i78 = 0; (i78 < 2); i78 = (i78 + 1)) {
			fRec78[i78] = 0.0f;
			
		}
		for (int i79 = 0; (i79 < 2); i79 = (i79 + 1)) {
			fRec79[i79] = 0.0f;
			
		}
		for (int i80 = 0; (i80 < 2); i80 = (i80 + 1)) {
			fRec80[i80] = 0.0f;
			
		}
		for (int i81 = 0; (i81 < 2); i81 = (i81 + 1)) {
			fRec81[i81] = 0.0f;
			
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
		ui_interface->addVerticalBargraph("0x3220040", &fVbargraph0, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("1");
		ui_interface->openVerticalBox("1");
		ui_interface->declare(&fVbargraph9, "unit", "dB");
		ui_interface->addVerticalBargraph("0x327a450", &fVbargraph9, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("2");
		ui_interface->declare(&fVbargraph4, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3250c80", &fVbargraph4, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("3");
		ui_interface->declare(&fVbargraph5, "unit", "dB");
		ui_interface->addVerticalBargraph("0x325a440", &fVbargraph5, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2");
		ui_interface->openVerticalBox("4");
		ui_interface->declare(&fVbargraph8, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3271070", &fVbargraph8, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("5");
		ui_interface->declare(&fVbargraph15, "unit", "dB");
		ui_interface->addVerticalBargraph("0x32a4260", &fVbargraph15, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("6");
		ui_interface->declare(&fVbargraph3, "unit", "dB");
		ui_interface->addVerticalBargraph("0x32464d0", &fVbargraph3, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("7");
		ui_interface->declare(&fVbargraph2, "unit", "dB");
		ui_interface->addVerticalBargraph("0x323b1e0", &fVbargraph2, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("8");
		ui_interface->declare(&fVbargraph1, "unit", "dB");
		ui_interface->addVerticalBargraph("0x322fbd0", &fVbargraph1, -70.0f, 6.0f);
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
		ui_interface->openHorizontalBox("Outputs 1-25");
		ui_interface->openVerticalBox("1");
		ui_interface->declare(&fVbargraph6, "osc", "/output1");
		ui_interface->declare(&fVbargraph6, "unit", "dB");
		ui_interface->addVerticalBargraph("0x325fa80", &fVbargraph6, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("2");
		ui_interface->declare(&fVbargraph7, "osc", "/output2");
		ui_interface->declare(&fVbargraph7, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3265d30", &fVbargraph7, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("3");
		ui_interface->declare(&fVbargraph10, "osc", "/output3");
		ui_interface->declare(&fVbargraph10, "unit", "dB");
		ui_interface->addVerticalBargraph("0x327f7f0", &fVbargraph10, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("4");
		ui_interface->declare(&fVbargraph11, "osc", "/output4");
		ui_interface->declare(&fVbargraph11, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3285570", &fVbargraph11, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("5");
		ui_interface->declare(&fVbargraph12, "osc", "/output5");
		ui_interface->declare(&fVbargraph12, "unit", "dB");
		ui_interface->addVerticalBargraph("0x328b3d0", &fVbargraph12, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("6");
		ui_interface->declare(&fVbargraph13, "osc", "/output6");
		ui_interface->declare(&fVbargraph13, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3290d70", &fVbargraph13, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("7");
		ui_interface->declare(&fVbargraph14, "osc", "/output7");
		ui_interface->declare(&fVbargraph14, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3298ac0", &fVbargraph14, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("8");
		ui_interface->declare(&fVbargraph16, "osc", "/output8");
		ui_interface->declare(&fVbargraph16, "unit", "dB");
		ui_interface->addVerticalBargraph("0x32ab290", &fVbargraph16, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("9");
		ui_interface->declare(&fVbargraph17, "osc", "/output9");
		ui_interface->declare(&fVbargraph17, "unit", "dB");
		ui_interface->addVerticalBargraph("0x32b2600", &fVbargraph17, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("10");
		ui_interface->declare(&fVbargraph18, "osc", "/output10");
		ui_interface->declare(&fVbargraph18, "unit", "dB");
		ui_interface->addVerticalBargraph("0x32b9820", &fVbargraph18, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("11");
		ui_interface->declare(&fVbargraph19, "osc", "/output11");
		ui_interface->declare(&fVbargraph19, "unit", "dB");
		ui_interface->addVerticalBargraph("0x32c05a0", &fVbargraph19, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("12");
		ui_interface->declare(&fVbargraph20, "osc", "/output12");
		ui_interface->declare(&fVbargraph20, "unit", "dB");
		ui_interface->addVerticalBargraph("0x32c6840", &fVbargraph20, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("13");
		ui_interface->declare(&fVbargraph21, "osc", "/output13");
		ui_interface->declare(&fVbargraph21, "unit", "dB");
		ui_interface->addVerticalBargraph("0x32ccbb0", &fVbargraph21, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("14");
		ui_interface->declare(&fVbargraph22, "osc", "/output14");
		ui_interface->declare(&fVbargraph22, "unit", "dB");
		ui_interface->addVerticalBargraph("0x32d33b0", &fVbargraph22, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("15");
		ui_interface->declare(&fVbargraph23, "osc", "/output15");
		ui_interface->declare(&fVbargraph23, "unit", "dB");
		ui_interface->addVerticalBargraph("0x32d9530", &fVbargraph23, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("16");
		ui_interface->declare(&fVbargraph24, "osc", "/output16");
		ui_interface->declare(&fVbargraph24, "unit", "dB");
		ui_interface->addVerticalBargraph("0x32dfd30", &fVbargraph24, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("17");
		ui_interface->declare(&fVbargraph25, "osc", "/output17");
		ui_interface->declare(&fVbargraph25, "unit", "dB");
		ui_interface->addVerticalBargraph("0x32e6430", &fVbargraph25, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("18");
		ui_interface->declare(&fVbargraph26, "osc", "/output18");
		ui_interface->declare(&fVbargraph26, "unit", "dB");
		ui_interface->addVerticalBargraph("0x32ecdf0", &fVbargraph26, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("19");
		ui_interface->declare(&fVbargraph27, "osc", "/output19");
		ui_interface->declare(&fVbargraph27, "unit", "dB");
		ui_interface->addVerticalBargraph("0x32f68a0", &fVbargraph27, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("20");
		ui_interface->declare(&fVbargraph28, "osc", "/output20");
		ui_interface->declare(&fVbargraph28, "unit", "dB");
		ui_interface->addVerticalBargraph("0x32fe7c0", &fVbargraph28, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("21");
		ui_interface->declare(&fVbargraph29, "osc", "/output21");
		ui_interface->declare(&fVbargraph29, "unit", "dB");
		ui_interface->addVerticalBargraph("0x33059e0", &fVbargraph29, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("22");
		ui_interface->declare(&fVbargraph30, "osc", "/output22");
		ui_interface->declare(&fVbargraph30, "unit", "dB");
		ui_interface->addVerticalBargraph("0x330d860", &fVbargraph30, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("23");
		ui_interface->declare(&fVbargraph31, "osc", "/output23");
		ui_interface->declare(&fVbargraph31, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3314890", &fVbargraph31, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("24");
		ui_interface->declare(&fVbargraph32, "osc", "/output24");
		ui_interface->declare(&fVbargraph32, "unit", "dB");
		ui_interface->addVerticalBargraph("0x331b5b0", &fVbargraph32, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("25");
		ui_interface->declare(&fVbargraph33, "osc", "/output25");
		ui_interface->declare(&fVbargraph33, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3322770", &fVbargraph33, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Outputs 26-50");
		ui_interface->openVerticalBox("26");
		ui_interface->declare(&fVbargraph34, "osc", "/output26");
		ui_interface->declare(&fVbargraph34, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3329830", &fVbargraph34, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("27");
		ui_interface->declare(&fVbargraph35, "osc", "/output27");
		ui_interface->declare(&fVbargraph35, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3332000", &fVbargraph35, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("28");
		ui_interface->declare(&fVbargraph36, "osc", "/output28");
		ui_interface->declare(&fVbargraph36, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3338940", &fVbargraph36, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("29");
		ui_interface->declare(&fVbargraph37, "osc", "/output29");
		ui_interface->declare(&fVbargraph37, "unit", "dB");
		ui_interface->addVerticalBargraph("0x333fee0", &fVbargraph37, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("30");
		ui_interface->declare(&fVbargraph38, "osc", "/output30");
		ui_interface->declare(&fVbargraph38, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3347740", &fVbargraph38, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("31");
		ui_interface->declare(&fVbargraph39, "osc", "/output31");
		ui_interface->declare(&fVbargraph39, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3350770", &fVbargraph39, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("32");
		ui_interface->declare(&fVbargraph40, "osc", "/output32");
		ui_interface->declare(&fVbargraph40, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3358920", &fVbargraph40, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("33");
		ui_interface->declare(&fVbargraph41, "osc", "/output33");
		ui_interface->declare(&fVbargraph41, "unit", "dB");
		ui_interface->addVerticalBargraph("0x33605f0", &fVbargraph41, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("34");
		ui_interface->declare(&fVbargraph42, "osc", "/output34");
		ui_interface->declare(&fVbargraph42, "unit", "dB");
		ui_interface->addVerticalBargraph("0x33681c0", &fVbargraph42, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("35");
		ui_interface->declare(&fVbargraph43, "osc", "/output35");
		ui_interface->declare(&fVbargraph43, "unit", "dB");
		ui_interface->addVerticalBargraph("0x336fe70", &fVbargraph43, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("36");
		ui_interface->declare(&fVbargraph44, "osc", "/output36");
		ui_interface->declare(&fVbargraph44, "unit", "dB");
		ui_interface->addVerticalBargraph("0x33779f0", &fVbargraph44, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("37");
		ui_interface->declare(&fVbargraph45, "osc", "/output37");
		ui_interface->declare(&fVbargraph45, "unit", "dB");
		ui_interface->addVerticalBargraph("0x337f480", &fVbargraph45, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("38");
		ui_interface->declare(&fVbargraph46, "osc", "/output38");
		ui_interface->declare(&fVbargraph46, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3387d40", &fVbargraph46, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("39");
		ui_interface->declare(&fVbargraph47, "osc", "/output39");
		ui_interface->declare(&fVbargraph47, "unit", "dB");
		ui_interface->addVerticalBargraph("0x338e670", &fVbargraph47, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("40");
		ui_interface->declare(&fVbargraph48, "osc", "/output40");
		ui_interface->declare(&fVbargraph48, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3394c90", &fVbargraph48, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("41");
		ui_interface->declare(&fVbargraph49, "osc", "/output41");
		ui_interface->declare(&fVbargraph49, "unit", "dB");
		ui_interface->addVerticalBargraph("0x339b570", &fVbargraph49, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("42");
		ui_interface->declare(&fVbargraph50, "osc", "/output42");
		ui_interface->declare(&fVbargraph50, "unit", "dB");
		ui_interface->addVerticalBargraph("0x33a1f60", &fVbargraph50, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("43");
		ui_interface->declare(&fVbargraph51, "osc", "/output43");
		ui_interface->declare(&fVbargraph51, "unit", "dB");
		ui_interface->addVerticalBargraph("0x33a8640", &fVbargraph51, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("44");
		ui_interface->declare(&fVbargraph52, "osc", "/output44");
		ui_interface->declare(&fVbargraph52, "unit", "dB");
		ui_interface->addVerticalBargraph("0x33af1c0", &fVbargraph52, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("45");
		ui_interface->declare(&fVbargraph53, "osc", "/output45");
		ui_interface->declare(&fVbargraph53, "unit", "dB");
		ui_interface->addVerticalBargraph("0x33b5c40", &fVbargraph53, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("46");
		ui_interface->declare(&fVbargraph54, "osc", "/output46");
		ui_interface->declare(&fVbargraph54, "unit", "dB");
		ui_interface->addVerticalBargraph("0x33bc7a0", &fVbargraph54, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("47");
		ui_interface->declare(&fVbargraph55, "osc", "/output47");
		ui_interface->declare(&fVbargraph55, "unit", "dB");
		ui_interface->addVerticalBargraph("0x33c3200", &fVbargraph55, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("48");
		ui_interface->declare(&fVbargraph56, "osc", "/output48");
		ui_interface->declare(&fVbargraph56, "unit", "dB");
		ui_interface->addVerticalBargraph("0x33c9d40", &fVbargraph56, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("49");
		ui_interface->declare(&fVbargraph57, "osc", "/output49");
		ui_interface->declare(&fVbargraph57, "unit", "dB");
		ui_interface->addVerticalBargraph("0x33d0d20", &fVbargraph57, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("50");
		ui_interface->declare(&fVbargraph58, "osc", "/output50");
		ui_interface->declare(&fVbargraph58, "unit", "dB");
		ui_interface->addVerticalBargraph("0x33d7de0", &fVbargraph58, -70.0f, 6.0f);
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
		float fSlow0 = (0.00100000005f * powf(10.0f, (0.0500000007f * float(fVslider0))));
		float fSlow1 = float(fEntry0);
		float fSlow2 = float(fCheckbox0);
		float fSlow3 = (((fSlow1 + -1.0f) * fSlow2) + 1.0f);
		float fSlow4 = (0.00100000005f * powf(10.0f, (0.0500000007f * float(fVslider1))));
		float fSlow5 = (fConst3 / fSlow1);
		float fSlow6 = faustpower2_f(fSlow5);
		float fSlow7 = (1.0f / (((fConst2 / fSlow1) + (3.0f * fSlow6)) + 1.0f));
		float fSlow8 = (12.0f * fSlow6);
		float fSlow9 = (fSlow8 + (fConst4 / fSlow1));
		float fSlow10 = (fSlow2 * fSlow1);
		float fSlow11 = (1.0f - fSlow2);
		float fSlow12 = (1.0f / (fSlow5 + 1.0f));
		float fSlow13 = (fConst5 / fSlow1);
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec1[0] = (fSlow0 + (0.999000013f * fRec1[1]));
			fRec3[0] = (fSlow4 + (0.999000013f * fRec3[1]));
			float fTemp0 = (fSlow3 * (fRec3[0] * float(input0[i])));
			fRec2[0] = max((fRec2[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp0))))));
			fVbargraph0 = FAUSTFLOAT(fRec2[0]);
			float fTemp1 = (0.0126984129f * fTemp0);
			fRec6[0] = (fRec5[1] + fRec6[1]);
			fRec7[0] = (fRec6[1] + fRec7[1]);
			float fTemp2 = (fRec3[0] * float(input8[i]));
			fRec5[0] = (0.0f - (fSlow7 * (((fSlow9 * fRec6[0]) + (fSlow8 * fRec7[0])) - (fSlow10 * fTemp2))));
			float fTemp3 = (fRec5[0] + (fSlow11 * fTemp2));
			fRec4[0] = max((fRec4[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp3))))));
			fVbargraph1 = FAUSTFLOAT(fRec4[0]);
			float fTemp4 = (9.21991188e-35f * fTemp3);
			fRec10[0] = (fRec9[1] + fRec10[1]);
			fRec11[0] = (fRec10[1] + fRec11[1]);
			float fTemp5 = (fRec3[0] * float(input7[i]));
			fRec9[0] = (0.0f - (fSlow7 * (((fSlow9 * fRec10[0]) + (fSlow8 * fRec11[0])) - (fSlow10 * fTemp5))));
			float fTemp6 = (fRec9[0] + (fSlow11 * fTemp5));
			fRec8[0] = max((fRec8[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp6))))));
			fVbargraph2 = FAUSTFLOAT(fRec8[0]);
			float fTemp7 = (3.01145194e-18f * fTemp6);
			float fTemp8 = (fRec3[0] * float(input6[i]));
			fRec14[0] = (fRec13[1] + fRec14[1]);
			fRec15[0] = (fRec14[1] + fRec15[1]);
			fRec13[0] = (fSlow7 * ((fSlow10 * fTemp8) - ((fSlow9 * fRec14[0]) + (fSlow8 * fRec15[0]))));
			float fTemp9 = (fRec13[0] + (fSlow11 * fTemp8));
			fRec12[0] = max((fRec12[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp9))))));
			fVbargraph3 = FAUSTFLOAT(fRec12[0]);
			float fTemp10 = (0.0283945147f * fTemp9);
			float fTemp11 = (fRec3[0] * float(input2[i]));
			fRec18[0] = (fRec17[1] + fRec18[1]);
			fRec17[0] = (fSlow12 * ((fSlow10 * fTemp11) - (fSlow13 * fRec18[0])));
			float fTemp12 = (fRec17[0] + (fSlow11 * fTemp11));
			fRec16[0] = max((fRec16[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp12))))));
			fVbargraph4 = FAUSTFLOAT(fRec16[0]);
			float fTemp13 = (0.0219942965f * fTemp12);
			float fTemp14 = (fRec3[0] * float(input3[i]));
			fRec21[0] = (fRec20[1] + fRec21[1]);
			fRec20[0] = (fSlow12 * ((fSlow10 * fTemp14) - (fSlow13 * fRec21[0])));
			float fTemp15 = (fRec20[0] + (fSlow11 * fTemp14));
			fRec19[0] = max((fRec19[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp15))))));
			fVbargraph5 = FAUSTFLOAT(fRec19[0]);
			float fTemp16 = (1.34676222e-18f * fTemp15);
			float fTemp17 = (fRec1[0] * (fTemp1 + (fTemp4 + (fTemp7 + (fTemp10 + (fTemp13 + fTemp16))))));
			fRec0[0] = max((fRec0[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp17))))));
			fVbargraph6 = FAUSTFLOAT(fRec0[0]);
			output0[i] = FAUSTFLOAT(fTemp17);
			float fTemp18 = (0.0219942965f * fTemp15);
			float fTemp19 = (0.0245903712f * fTemp3);
			float fTemp20 = (0.0141972573f * fTemp9);
			float fTemp21 = (fRec1[0] * ((fTemp1 + (fTemp18 + fTemp19)) - fTemp20));
			fRec22[0] = max((fRec22[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp21))))));
			fVbargraph7 = FAUSTFLOAT(fRec22[0]);
			output1[i] = FAUSTFLOAT(fTemp21);
			fRec26[0] = (fRec25[1] + fRec26[1]);
			fRec27[0] = (fRec26[1] + fRec27[1]);
			float fTemp22 = (fRec3[0] * float(input4[i]));
			fRec25[0] = (0.0f - (fSlow7 * (((fSlow9 * fRec26[0]) + (fSlow8 * fRec27[0])) - (fSlow10 * fTemp22))));
			float fTemp23 = (fRec25[0] + (fSlow11 * fTemp22));
			fRec24[0] = max((fRec24[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp23))))));
			fVbargraph8 = FAUSTFLOAT(fRec24[0]);
			float fTemp24 = (fRec3[0] * float(input1[i]));
			fRec30[0] = (fRec29[1] + fRec30[1]);
			fRec29[0] = (fSlow12 * ((fSlow10 * fTemp24) - (fSlow13 * fRec30[0])));
			float fTemp25 = (fRec29[0] + (fSlow11 * fTemp24));
			fRec28[0] = max((fRec28[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp25))))));
			fVbargraph9 = FAUSTFLOAT(fRec28[0]);
			float fTemp26 = (0.0219942965f * fTemp25);
			float fTemp27 = (fRec1[0] * ((fTemp1 + ((3.01145194e-18f * fTemp23) + (fTemp16 + fTemp26))) - (fTemp19 + fTemp20)));
			fRec23[0] = max((fRec23[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp27))))));
			fVbargraph10 = FAUSTFLOAT(fRec23[0]);
			output2[i] = FAUSTFLOAT(fTemp27);
			float fTemp28 = (fRec1[0] * ((fTemp1 + (fTemp19 + (2.69352445e-18f * fTemp25))) - ((6.02290387e-18f * fTemp23) + (fTemp18 + fTemp20))));
			fRec31[0] = max((fRec31[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp28))))));
			fVbargraph11 = FAUSTFLOAT(fRec31[0]);
			output3[i] = FAUSTFLOAT(fTemp28);
			float fTemp29 = (fRec1[0] * (((9.03435581e-18f * fTemp23) + fTemp1) - (fTemp26 + (fTemp19 + (fTemp20 + (4.04028677e-18f * fTemp15))))));
			fRec32[0] = max((fRec32[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp29))))));
			fVbargraph12 = FAUSTFLOAT(fRec32[0]);
			output4[i] = FAUSTFLOAT(fTemp29);
			float fTemp30 = (fRec1[0] * ((fTemp1 + (fTemp4 + (fTemp16 + fTemp10))) - (fTemp13 + fTemp7)));
			fRec33[0] = max((fRec33[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp30))))));
			fVbargraph13 = FAUSTFLOAT(fRec33[0]);
			output5[i] = FAUSTFLOAT(fTemp30);
			float fTemp31 = (0.0225749556f * fTemp0);
			float fTemp32 = (0.0218581073f * fTemp3);
			float fTemp33 = (0.0437162146f * fTemp6);
			float fTemp34 = (0.0126197841f * fTemp9);
			float fTemp35 = (0.0276485607f * fTemp12);
			float fTemp36 = (0.0276485607f * fTemp15);
			float fTemp37 = (fTemp35 + fTemp36);
			float fTemp38 = (fRec1[0] * (fTemp31 + (fTemp32 + (fTemp33 + (fTemp34 + fTemp37)))));
			fRec34[0] = max((fRec34[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp38))))));
			fVbargraph14 = FAUSTFLOAT(fRec34[0]);
			output6[i] = FAUSTFLOAT(fTemp38);
			fRec38[0] = (fRec37[1] + fRec38[1]);
			fRec39[0] = (fRec38[1] + fRec39[1]);
			float fTemp39 = (fRec3[0] * float(input5[i]));
			fRec37[0] = (0.0f - (fSlow7 * (((fSlow9 * fRec38[0]) + (fSlow8 * fRec39[0])) - (fSlow10 * fTemp39))));
			float fTemp40 = (fRec37[0] + (fSlow11 * fTemp39));
			fRec36[0] = max((fRec36[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp40))))));
			fVbargraph15 = FAUSTFLOAT(fRec36[0]);
			float fTemp41 = (0.0437162146f * fTemp40);
			float fTemp42 = (2.676846e-18f * fTemp23);
			float fTemp43 = (0.0276485607f * fTemp25);
			float fTemp44 = (2.676846e-18f * fTemp6);
			float fTemp45 = (1.69298611e-18f * fTemp15);
			float fTemp46 = (fRec1[0] * ((fTemp31 + (fTemp41 + (fTemp42 + (fTemp43 + (fTemp44 + (fTemp34 + (fTemp35 + fTemp45))))))) - fTemp32));
			fRec35[0] = max((fRec35[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp46))))));
			fVbargraph16 = FAUSTFLOAT(fRec35[0]);
			output7[i] = FAUSTFLOAT(fTemp46);
			float fTemp47 = (5.35369201e-18f * fTemp40);
			float fTemp48 = (3.38597223e-18f * fTemp25);
			float fTemp49 = (fTemp35 + fTemp34);
			float fTemp50 = (5.35369201e-18f * fTemp23);
			float fTemp51 = (fRec1[0] * ((fTemp31 + (fTemp47 + (fTemp48 + (fTemp32 + fTemp49)))) - (fTemp50 + (fTemp36 + fTemp33))));
			fRec40[0] = max((fRec40[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp51))))));
			fVbargraph17 = FAUSTFLOAT(fRec40[0]);
			output8[i] = FAUSTFLOAT(fTemp51);
			float fTemp52 = (8.03053822e-18f * fTemp23);
			float fTemp53 = (5.07895844e-18f * fTemp15);
			float fTemp54 = (8.03053822e-18f * fTemp6);
			float fTemp55 = (fRec1[0] * ((fTemp31 + (fTemp52 + fTemp49)) - (fTemp41 + (fTemp43 + (fTemp32 + (fTemp53 + fTemp54))))));
			fRec41[0] = max((fRec41[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp55))))));
			fVbargraph18 = FAUSTFLOAT(fRec41[0]);
			output9[i] = FAUSTFLOAT(fTemp55);
			float fTemp56 = (0.0437162146f * fTemp23);
			float fTemp57 = (0.0276485607f * fTemp25);
			float fTemp58 = (0.0252395682f * fTemp9);
			float fTemp59 = (fRec1[0] * ((fTemp31 + (fTemp56 + (fTemp57 + (fTemp36 + (2.676846e-18f * fTemp3))))) - fTemp58));
			fRec42[0] = max((fRec42[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp59))))));
			fVbargraph19 = FAUSTFLOAT(fRec42[0]);
			output10[i] = FAUSTFLOAT(fTemp59);
			float fTemp60 = (fRec1[0] * ((fTemp43 + fTemp31) - (fTemp56 + ((fTemp58 + (0.0276485607f * fTemp15)) + (8.03053822e-18f * fTemp3)))));
			fRec43[0] = max((fRec43[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp60))))));
			fVbargraph20 = FAUSTFLOAT(fRec43[0]);
			output11[i] = FAUSTFLOAT(fTemp60);
			float fTemp61 = (fRec1[0] * ((fTemp31 + (fTemp56 + (1.33842306e-17f * fTemp3))) - (fTemp57 + (fTemp36 + fTemp58))));
			fRec44[0] = max((fRec44[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp61))))));
			fVbargraph21 = FAUSTFLOAT(fRec44[0]);
			output12[i] = FAUSTFLOAT(fTemp61);
			float fTemp62 = (fRec1[0] * (((0.0276485607f * fTemp15) + fTemp31) - (fTemp56 + (fTemp43 + (fTemp58 + (1.87379222e-17f * fTemp3))))));
			fRec45[0] = max((fRec45[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp62))))));
			fVbargraph22 = FAUSTFLOAT(fRec45[0]);
			output13[i] = FAUSTFLOAT(fTemp62);
			float fTemp63 = (fRec1[0] * ((fTemp31 + (fTemp32 + (fTemp36 + fTemp34))) - (fTemp35 + fTemp33)));
			fRec46[0] = max((fRec46[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp63))))));
			fVbargraph23 = FAUSTFLOAT(fRec46[0]);
			output14[i] = FAUSTFLOAT(fTemp63);
			float fTemp64 = (fRec1[0] * ((fTemp31 + (fTemp42 + (fTemp43 + (fTemp34 + fTemp45)))) - (fTemp41 + (fTemp32 + (fTemp35 + fTemp44)))));
			fRec47[0] = max((fRec47[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp64))))));
			fVbargraph24 = FAUSTFLOAT(fRec47[0]);
			output15[i] = FAUSTFLOAT(fTemp64);
			float fTemp65 = (fRec1[0] * ((fTemp31 + (fTemp48 + (fTemp32 + (fTemp34 + fTemp33)))) - (fTemp47 + (fTemp50 + fTemp37))));
			fRec48[0] = max((fRec48[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp65))))));
			fVbargraph25 = FAUSTFLOAT(fRec48[0]);
			output16[i] = FAUSTFLOAT(fTemp65);
			float fTemp66 = (fRec1[0] * ((fTemp31 + (fTemp41 + (fTemp52 + (fTemp34 + fTemp54)))) - (fTemp43 + (fTemp32 + (fTemp35 + fTemp53)))));
			fRec49[0] = max((fRec49[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp66))))));
			fVbargraph26 = FAUSTFLOAT(fRec49[0]);
			output17[i] = FAUSTFLOAT(fTemp66);
			float fTemp67 = (0.0272319149f * fTemp40);
			float fTemp68 = (0.0272319149f * fTemp23);
			float fTemp69 = (0.0210937504f * fTemp25);
			float fTemp70 = (5.23659549e-18f * fTemp9);
			float fTemp71 = ((0.0210937504f * fTemp15) + fTemp70);
			float fTemp72 = (0.0272319149f * fTemp6);
			float fTemp73 = (1.6674738e-18f * fTemp3);
			float fTemp74 = (fTemp12 + fTemp0);
			float fTemp75 = (0.0210937504f * fTemp74);
			float fTemp76 = (fRec1[0] * ((fTemp67 + (fTemp68 + (fTemp69 + ((fTemp71 + fTemp72) + fTemp73)))) + fTemp75));
			fRec50[0] = max((fRec50[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp76))))));
			fVbargraph27 = FAUSTFLOAT(fRec50[0]);
			output18[i] = FAUSTFLOAT(fTemp76);
			float fTemp77 = (0.0272319149f * fTemp40);
			float fTemp78 = (0.0210937504f * fTemp25);
			float fTemp79 = (0.0210937504f * fTemp15);
			float fTemp80 = (0.0272319149f * fTemp6);
			float fTemp81 = (5.00242149e-18f * fTemp3);
			float fTemp82 = (fRec1[0] * (((fTemp77 + (fTemp70 + fTemp78)) + fTemp75) - (fTemp68 + ((fTemp79 + fTemp80) + fTemp81))));
			fRec51[0] = max((fRec51[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp82))))));
			fVbargraph28 = FAUSTFLOAT(fRec51[0]);
			output19[i] = FAUSTFLOAT(fTemp82);
			float fTemp83 = (8.33736929e-18f * fTemp3);
			float fTemp84 = (fRec1[0] * ((fTemp68 + (fTemp70 + fTemp83)) - ((fTemp67 + (fTemp69 + fTemp72)) + (0.0210937504f * (fTemp15 - fTemp74)))));
			fRec52[0] = max((fRec52[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp84))))));
			fVbargraph29 = FAUSTFLOAT(fRec52[0]);
			output20[i] = FAUSTFLOAT(fTemp84);
			float fTemp85 = (fTemp70 + (0.0210937504f * fTemp15));
			float fTemp86 = (0.0272319149f * fTemp6);
			float fTemp87 = (1.16723167e-17f * fTemp3);
			float fTemp88 = (fRec1[0] * (((fTemp85 + fTemp86) + (0.0210937504f * (fTemp74 - fTemp25))) - (fTemp77 + (fTemp68 + fTemp87))));
			fRec53[0] = max((fRec53[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp88))))));
			fVbargraph30 = FAUSTFLOAT(fRec53[0]);
			output21[i] = FAUSTFLOAT(fTemp88);
			float fTemp89 = (0.0210937504f * (fTemp0 - fTemp12));
			float fTemp90 = (fRec1[0] * (((fTemp68 + (fTemp69 + (fTemp71 + fTemp73))) + fTemp89) - (fTemp67 + fTemp72)));
			fRec54[0] = max((fRec54[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp90))))));
			fVbargraph31 = FAUSTFLOAT(fRec54[0]);
			output22[i] = FAUSTFLOAT(fTemp90);
			float fTemp91 = (fRec1[0] * (((fTemp78 + (fTemp70 + fTemp80)) + fTemp89) - (fTemp77 + (fTemp68 + (fTemp79 + fTemp81)))));
			fRec55[0] = max((fRec55[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp91))))));
			fVbargraph32 = FAUSTFLOAT(fRec55[0]);
			output23[i] = FAUSTFLOAT(fTemp91);
			float fTemp92 = (fRec1[0] * ((fTemp67 + (fTemp68 + (fTemp83 + (fTemp70 + fTemp72)))) - (fTemp69 + (0.0210937504f * ((fTemp12 + fTemp15) - fTemp0)))));
			fRec56[0] = max((fRec56[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp92))))));
			fVbargraph33 = FAUSTFLOAT(fRec56[0]);
			output24[i] = FAUSTFLOAT(fTemp92);
			float fTemp93 = (fRec1[0] * ((fTemp77 + fTemp85) - ((fTemp68 + (fTemp86 + fTemp87)) + (0.0210937504f * ((fTemp12 + fTemp25) - fTemp0)))));
			fRec57[0] = max((fRec57[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp93))))));
			fVbargraph34 = FAUSTFLOAT(fRec57[0]);
			output25[i] = FAUSTFLOAT(fTemp93);
			float fTemp94 = (0.0201733354f * fTemp0);
			float fTemp95 = (0.0105351806f * (fTemp15 + fTemp25));
			float fTemp96 = (0.0213084519f * (fTemp6 + fTemp40));
			float fTemp97 = (0.0071028173f * fTemp23);
			float fTemp98 = (0.0316055417f * fTemp12);
			float fTemp99 = (0.0328065082f * fTemp9);
			float fTemp100 = (fTemp98 + fTemp99);
			float fTemp101 = (1.14222021e-18f * fTemp3);
			float fTemp102 = (fRec1[0] * ((fTemp94 + (fTemp95 + (fTemp96 + (fTemp97 + fTemp100)))) - fTemp101));
			fRec58[0] = max((fRec58[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp102))))));
			fVbargraph35 = FAUSTFLOAT(fRec58[0]);
			output26[i] = FAUSTFLOAT(fTemp102);
			float fTemp103 = (0.0105351806f * (fTemp15 - fTemp25));
			float fTemp104 = (0.0213084519f * (fTemp6 - fTemp40));
			float fTemp105 = (1.30476637e-18f * fTemp3);
			float fTemp106 = (fRec1[0] * ((fTemp94 + fTemp100) - (fTemp103 + (fTemp104 + (fTemp97 + fTemp105)))));
			fRec59[0] = max((fRec59[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp106))))));
			fVbargraph36 = FAUSTFLOAT(fRec59[0]);
			output27[i] = FAUSTFLOAT(fTemp106);
			float fTemp107 = (2.17461068e-18f * fTemp3);
			float fTemp108 = (0.0213084519f * fTemp40);
			float fTemp109 = (0.0105351806f * fTemp25);
			float fTemp110 = (0.0105351806f * fTemp15);
			float fTemp111 = (0.0213084519f * fTemp6);
			float fTemp112 = (fRec1[0] * ((fTemp94 + (fTemp97 + (fTemp107 + fTemp100))) - (fTemp108 + (fTemp109 + (fTemp110 + fTemp111)))));
			fRec60[0] = max((fRec60[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp112))))));
			fVbargraph37 = FAUSTFLOAT(fRec60[0]);
			output28[i] = FAUSTFLOAT(fTemp112);
			float fTemp113 = (0.0213084519f * fTemp6);
			float fTemp114 = (0.0105351806f * fTemp15);
			float fTemp115 = (0.0213084519f * fTemp40);
			float fTemp116 = (0.0105351806f * fTemp25);
			float fTemp117 = (3.04445499e-18f * fTemp3);
			float fTemp118 = (fRec1[0] * ((fTemp94 + (fTemp113 + (fTemp99 + (fTemp98 + fTemp114)))) - (fTemp115 + (fTemp97 + (fTemp116 + fTemp117)))));
			fRec61[0] = max((fRec61[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp118))))));
			fVbargraph38 = FAUSTFLOAT(fRec61[0]);
			output29[i] = FAUSTFLOAT(fTemp118);
			float fTemp119 = (0.0071028173f * fTemp40);
			float fTemp120 = (0.0213084519f * fTemp23);
			float fTemp121 = (0.0105351806f * fTemp25);
			float fTemp122 = (0.0284112692f * fTemp3);
			float fTemp123 = (0.0213084519f * fTemp6);
			float fTemp124 = (0.0105351806f * fTemp12);
			float fTemp125 = (0.0316055417f * fTemp15);
			float fTemp126 = (fTemp124 + fTemp125);
			float fTemp127 = (0.0164032541f * fTemp9);
			float fTemp128 = (fRec1[0] * ((fTemp94 + (fTemp119 + (fTemp120 + (fTemp121 + (fTemp122 + (fTemp123 + fTemp126)))))) - fTemp127));
			fRec62[0] = max((fRec62[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp128))))));
			fVbargraph39 = FAUSTFLOAT(fRec62[0]);
			output30[i] = FAUSTFLOAT(fTemp128);
			float fTemp129 = (0.0213084519f * fTemp40);
			float fTemp130 = (0.0213084519f * fTemp23);
			float fTemp131 = (0.0316055417f * fTemp25);
			float fTemp132 = (0.0071028173f * fTemp6);
			float fTemp133 = (0.0105351806f * fTemp15);
			float fTemp134 = (0.0284112692f * fTemp3);
			float fTemp135 = (fRec1[0] * ((fTemp94 + (fTemp129 + (fTemp130 + (fTemp131 + (fTemp132 + (fTemp124 + fTemp133)))))) - (fTemp127 + fTemp134)));
			fRec63[0] = max((fRec63[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp135))))));
			fVbargraph40 = FAUSTFLOAT(fRec63[0]);
			output31[i] = FAUSTFLOAT(fTemp135);
			float fTemp136 = (0.0213084519f * fTemp40);
			float fTemp137 = (0.0213084519f * fTemp23);
			float fTemp138 = (0.0105351806f * fTemp15);
			float fTemp139 = (0.0071028173f * fTemp6);
			float fTemp140 = (0.0284112692f * fTemp3);
			float fTemp141 = (fRec1[0] * ((fTemp94 + (fTemp136 + (fTemp124 + fTemp131))) - (fTemp137 + (((fTemp127 + fTemp138) + fTemp139) + fTemp140))));
			fRec64[0] = max((fRec64[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp141))))));
			fVbargraph41 = FAUSTFLOAT(fRec64[0]);
			output32[i] = FAUSTFLOAT(fTemp141);
			float fTemp142 = (0.0071028173f * fTemp40);
			float fTemp143 = (0.0105351806f * fTemp25);
			float fTemp144 = (0.0284112692f * fTemp3);
			float fTemp145 = (0.0213084519f * fTemp23);
			float fTemp146 = (fTemp125 + fTemp127);
			float fTemp147 = (fRec1[0] * ((fTemp94 + (fTemp142 + (fTemp143 + (fTemp124 + fTemp144)))) - (fTemp145 + (fTemp123 + fTemp146))));
			fRec65[0] = max((fRec65[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp147))))));
			fVbargraph42 = FAUSTFLOAT(fRec65[0]);
			output33[i] = FAUSTFLOAT(fTemp147);
			float fTemp148 = (0.0213084519f * fTemp23);
			float fTemp149 = (0.0284112692f * fTemp3);
			float fTemp150 = (0.0071028173f * fTemp40);
			float fTemp151 = (0.0105351806f * fTemp25);
			float fTemp152 = (0.0213084519f * fTemp6);
			float fTemp153 = (fRec1[0] * ((fTemp94 + (fTemp148 + (fTemp124 + fTemp149))) - (fTemp150 + (fTemp151 + (fTemp146 + fTemp152)))));
			fRec66[0] = max((fRec66[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp153))))));
			fVbargraph43 = FAUSTFLOAT(fRec66[0]);
			output34[i] = FAUSTFLOAT(fTemp153);
			float fTemp154 = (0.0213084519f * fTemp23);
			float fTemp155 = (0.0105351806f * fTemp15);
			float fTemp156 = (0.0071028173f * fTemp6);
			float fTemp157 = (0.0284112692f * fTemp3);
			float fTemp158 = (fRec1[0] * ((fTemp94 + (fTemp124 + fTemp154)) - (fTemp129 + (fTemp131 + (((fTemp127 + fTemp155) + fTemp156) + fTemp157)))));
			fRec67[0] = max((fRec67[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp158))))));
			fVbargraph44 = FAUSTFLOAT(fRec67[0]);
			output35[i] = FAUSTFLOAT(fTemp158);
			float fTemp159 = (0.0071028173f * fTemp6);
			float fTemp160 = (0.0105351806f * fTemp15);
			float fTemp161 = (0.0213084519f * fTemp23);
			float fTemp162 = (0.0284112692f * fTemp3);
			float fTemp163 = (fRec1[0] * ((fTemp94 + (fTemp159 + (fTemp124 + fTemp160))) - (fTemp136 + (fTemp161 + (fTemp131 + (fTemp127 + fTemp162))))));
			fRec68[0] = max((fRec68[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp163))))));
			fVbargraph45 = FAUSTFLOAT(fRec68[0]);
			output36[i] = FAUSTFLOAT(fTemp163);
			float fTemp164 = (0.0284112692f * fTemp3);
			float fTemp165 = (0.0213084519f * fTemp6);
			float fTemp166 = (0.0316055417f * fTemp15);
			float fTemp167 = (0.0071028173f * fTemp40);
			float fTemp168 = (0.0213084519f * fTemp23);
			float fTemp169 = (0.0105351806f * fTemp25);
			float fTemp170 = (fRec1[0] * ((fTemp94 + (fTemp164 + (fTemp165 + (fTemp124 + fTemp166)))) - (fTemp167 + (fTemp168 + (fTemp127 + fTemp169)))));
			fRec69[0] = max((fRec69[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp170))))));
			fVbargraph46 = FAUSTFLOAT(fRec69[0]);
			output37[i] = FAUSTFLOAT(fTemp170);
			float fTemp171 = (fTemp124 + fTemp127);
			float fTemp172 = (fRec1[0] * ((fTemp94 + (fTemp120 + (fTemp121 + (fTemp125 + fTemp122)))) - (fTemp119 + (fTemp123 + fTemp171))));
			fRec70[0] = max((fRec70[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp172))))));
			fVbargraph47 = FAUSTFLOAT(fRec70[0]);
			output38[i] = FAUSTFLOAT(fTemp172);
			float fTemp173 = (fRec1[0] * ((fTemp94 + (fTemp130 + (fTemp131 + fTemp133))) - (fTemp129 + (fTemp134 + (fTemp132 + fTemp171)))));
			fRec71[0] = max((fRec71[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp173))))));
			fVbargraph48 = FAUSTFLOAT(fRec71[0]);
			output39[i] = FAUSTFLOAT(fTemp173);
			float fTemp174 = (fRec1[0] * ((fTemp94 + (fTemp131 + fTemp139)) - (fTemp136 + (fTemp137 + (fTemp140 + (fTemp127 + (fTemp124 + fTemp138)))))));
			fRec72[0] = max((fRec72[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp174))))));
			fVbargraph49 = FAUSTFLOAT(fRec72[0]);
			output40[i] = FAUSTFLOAT(fTemp174);
			float fTemp175 = (fTemp127 + fTemp126);
			float fTemp176 = (fRec1[0] * ((fTemp94 + (fTemp143 + (fTemp123 + fTemp144))) - (fTemp142 + (fTemp145 + fTemp175))));
			fRec73[0] = max((fRec73[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp176))))));
			fVbargraph50 = FAUSTFLOAT(fRec73[0]);
			output41[i] = FAUSTFLOAT(fTemp176);
			float fTemp177 = (fRec1[0] * ((fTemp94 + (fTemp150 + (fTemp148 + (fTemp149 + fTemp152)))) - (fTemp151 + fTemp175)));
			fRec74[0] = max((fRec74[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp177))))));
			fVbargraph51 = FAUSTFLOAT(fRec74[0]);
			output42[i] = FAUSTFLOAT(fTemp177);
			float fTemp178 = (fRec1[0] * ((fTemp94 + (fTemp129 + (fTemp154 + fTemp156))) - (fTemp131 + (fTemp157 + (fTemp127 + (fTemp124 + fTemp155))))));
			fRec75[0] = max((fRec75[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp178))))));
			fVbargraph52 = FAUSTFLOAT(fRec75[0]);
			output43[i] = FAUSTFLOAT(fTemp178);
			float fTemp179 = (fRec1[0] * ((fTemp94 + (fTemp136 + fTemp160)) - (fTemp161 + (fTemp131 + (fTemp162 + (fTemp159 + fTemp171))))));
			fRec76[0] = max((fRec76[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp179))))));
			fVbargraph53 = FAUSTFLOAT(fRec76[0]);
			output44[i] = FAUSTFLOAT(fTemp179);
			float fTemp180 = (fRec1[0] * ((fTemp94 + (fTemp167 + (fTemp166 + fTemp164))) - (fTemp168 + (fTemp169 + (fTemp165 + fTemp171)))));
			fRec77[0] = max((fRec77[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp180))))));
			fVbargraph54 = FAUSTFLOAT(fRec77[0]);
			output45[i] = FAUSTFLOAT(fTemp180);
			float fTemp181 = (fRec1[0] * ((fTemp94 + (fTemp95 + (fTemp97 + fTemp99))) - (fTemp96 + (fTemp98 + fTemp101))));
			fRec78[0] = max((fRec78[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp181))))));
			fVbargraph55 = FAUSTFLOAT(fRec78[0]);
			output46[i] = FAUSTFLOAT(fTemp181);
			float fTemp182 = (fRec1[0] * ((fTemp94 + (fTemp99 + fTemp104)) - (fTemp103 + (fTemp97 + (fTemp98 + fTemp105)))));
			fRec79[0] = max((fRec79[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp182))))));
			fVbargraph56 = FAUSTFLOAT(fRec79[0]);
			output47[i] = FAUSTFLOAT(fTemp182);
			float fTemp183 = (fRec1[0] * ((fTemp94 + (fTemp108 + (fTemp97 + (fTemp107 + (fTemp99 + fTemp111))))) - (fTemp109 + (fTemp98 + fTemp110))));
			fRec80[0] = max((fRec80[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp183))))));
			fVbargraph57 = FAUSTFLOAT(fRec80[0]);
			output48[i] = FAUSTFLOAT(fTemp183);
			float fTemp184 = (fRec1[0] * ((fTemp94 + (fTemp115 + (fTemp99 + fTemp114))) - (fTemp97 + (fTemp116 + (fTemp117 + (fTemp98 + fTemp113))))));
			fRec81[0] = max((fRec81[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp184))))));
			fVbargraph58 = FAUSTFLOAT(fRec81[0]);
			output49[i] = FAUSTFLOAT(fTemp184);
			fRec1[1] = fRec1[0];
			fRec3[1] = fRec3[0];
			fRec2[1] = fRec2[0];
			fRec6[1] = fRec6[0];
			fRec7[1] = fRec7[0];
			fRec5[1] = fRec5[0];
			fRec4[1] = fRec4[0];
			fRec10[1] = fRec10[0];
			fRec11[1] = fRec11[0];
			fRec9[1] = fRec9[0];
			fRec8[1] = fRec8[0];
			fRec14[1] = fRec14[0];
			fRec15[1] = fRec15[0];
			fRec13[1] = fRec13[0];
			fRec12[1] = fRec12[0];
			fRec18[1] = fRec18[0];
			fRec17[1] = fRec17[0];
			fRec16[1] = fRec16[0];
			fRec21[1] = fRec21[0];
			fRec20[1] = fRec20[0];
			fRec19[1] = fRec19[0];
			fRec0[1] = fRec0[0];
			fRec22[1] = fRec22[0];
			fRec26[1] = fRec26[0];
			fRec27[1] = fRec27[0];
			fRec25[1] = fRec25[0];
			fRec24[1] = fRec24[0];
			fRec30[1] = fRec30[0];
			fRec29[1] = fRec29[0];
			fRec28[1] = fRec28[0];
			fRec23[1] = fRec23[0];
			fRec31[1] = fRec31[0];
			fRec32[1] = fRec32[0];
			fRec33[1] = fRec33[0];
			fRec34[1] = fRec34[0];
			fRec38[1] = fRec38[0];
			fRec39[1] = fRec39[0];
			fRec37[1] = fRec37[0];
			fRec36[1] = fRec36[0];
			fRec35[1] = fRec35[0];
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
			fRec77[1] = fRec77[0];
			fRec78[1] = fRec78[0];
			fRec79[1] = fRec79[0];
			fRec80[1] = fRec80[0];
			fRec81[1] = fRec81[0];
			
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
