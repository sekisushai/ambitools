/* ------------------------------------------------------------
author: "Pierre Lecomte"
copyright: "(c) Pierre Lecomte 2014"
license: "GPL"
name: "HOADecLebedev503"
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
	float fRec100[2];
	float fRec101[2];
	float fRec102[2];
	float fRec103[2];
	float fRec104[2];
	float fRec105[2];
	float fRec106[2];
	float fRec107[2];
	float fRec108[2];
	float fRec109[2];
	float fRec110[2];
	float fRec111[2];
	float fRec112[2];
	float fRec113[2];
	float fRec114[2];
	float fRec115[2];
	float fRec116[2];
	float fRec117[2];
	float fRec118[2];
	float fRec119[2];
	float fRec120[2];
	float fRec121[2];
	float fRec122[2];
	float fRec123[2];
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
	FAUSTFLOAT fVbargraph59;
	FAUSTFLOAT fVbargraph60;
	FAUSTFLOAT fVbargraph61;
	FAUSTFLOAT fVbargraph62;
	FAUSTFLOAT fVbargraph63;
	FAUSTFLOAT fVbargraph64;
	FAUSTFLOAT fVbargraph65;
	
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
		m->declare("name", "HOADecLebedev503");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
		m->declare("version", "1.0");
	}

	virtual int getNumInputs() {
		return 16;
		
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
		for (int i100 = 0; (i100 < 2); i100 = (i100 + 1)) {
			fRec100[i100] = 0.0f;
			
		}
		for (int i101 = 0; (i101 < 2); i101 = (i101 + 1)) {
			fRec101[i101] = 0.0f;
			
		}
		for (int i102 = 0; (i102 < 2); i102 = (i102 + 1)) {
			fRec102[i102] = 0.0f;
			
		}
		for (int i103 = 0; (i103 < 2); i103 = (i103 + 1)) {
			fRec103[i103] = 0.0f;
			
		}
		for (int i104 = 0; (i104 < 2); i104 = (i104 + 1)) {
			fRec104[i104] = 0.0f;
			
		}
		for (int i105 = 0; (i105 < 2); i105 = (i105 + 1)) {
			fRec105[i105] = 0.0f;
			
		}
		for (int i106 = 0; (i106 < 2); i106 = (i106 + 1)) {
			fRec106[i106] = 0.0f;
			
		}
		for (int i107 = 0; (i107 < 2); i107 = (i107 + 1)) {
			fRec107[i107] = 0.0f;
			
		}
		for (int i108 = 0; (i108 < 2); i108 = (i108 + 1)) {
			fRec108[i108] = 0.0f;
			
		}
		for (int i109 = 0; (i109 < 2); i109 = (i109 + 1)) {
			fRec109[i109] = 0.0f;
			
		}
		for (int i110 = 0; (i110 < 2); i110 = (i110 + 1)) {
			fRec110[i110] = 0.0f;
			
		}
		for (int i111 = 0; (i111 < 2); i111 = (i111 + 1)) {
			fRec111[i111] = 0.0f;
			
		}
		for (int i112 = 0; (i112 < 2); i112 = (i112 + 1)) {
			fRec112[i112] = 0.0f;
			
		}
		for (int i113 = 0; (i113 < 2); i113 = (i113 + 1)) {
			fRec113[i113] = 0.0f;
			
		}
		for (int i114 = 0; (i114 < 2); i114 = (i114 + 1)) {
			fRec114[i114] = 0.0f;
			
		}
		for (int i115 = 0; (i115 < 2); i115 = (i115 + 1)) {
			fRec115[i115] = 0.0f;
			
		}
		for (int i116 = 0; (i116 < 2); i116 = (i116 + 1)) {
			fRec116[i116] = 0.0f;
			
		}
		for (int i117 = 0; (i117 < 2); i117 = (i117 + 1)) {
			fRec117[i117] = 0.0f;
			
		}
		for (int i118 = 0; (i118 < 2); i118 = (i118 + 1)) {
			fRec118[i118] = 0.0f;
			
		}
		for (int i119 = 0; (i119 < 2); i119 = (i119 + 1)) {
			fRec119[i119] = 0.0f;
			
		}
		for (int i120 = 0; (i120 < 2); i120 = (i120 + 1)) {
			fRec120[i120] = 0.0f;
			
		}
		for (int i121 = 0; (i121 < 2); i121 = (i121 + 1)) {
			fRec121[i121] = 0.0f;
			
		}
		for (int i122 = 0; (i122 < 2); i122 = (i122 + 1)) {
			fRec122[i122] = 0.0f;
			
		}
		for (int i123 = 0; (i123 < 2); i123 = (i123 + 1)) {
			fRec123[i123] = 0.0f;
			
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
		ui_interface->addVerticalBargraph("0x2ae0b70", &fVbargraph0, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("1");
		ui_interface->openVerticalBox("1");
		ui_interface->declare(&fVbargraph13, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2b7ab10", &fVbargraph13, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("2");
		ui_interface->declare(&fVbargraph8, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2b4fc80", &fVbargraph8, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("3");
		ui_interface->declare(&fVbargraph9, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2b59440", &fVbargraph9, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2");
		ui_interface->openVerticalBox("4");
		ui_interface->declare(&fVbargraph12, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2b71730", &fVbargraph12, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("5");
		ui_interface->declare(&fVbargraph22, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2bd5af0", &fVbargraph22, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("6");
		ui_interface->declare(&fVbargraph7, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2b454d0", &fVbargraph7, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("7");
		ui_interface->declare(&fVbargraph6, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2b3a1e0", &fVbargraph6, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("8");
		ui_interface->declare(&fVbargraph5, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2b2ebd0", &fVbargraph5, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3");
		ui_interface->openVerticalBox("9");
		ui_interface->declare(&fVbargraph15, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2b98030", &fVbargraph15, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("10");
		ui_interface->declare(&fVbargraph21, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2bca4f0", &fVbargraph21, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("11");
		ui_interface->declare(&fVbargraph14, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2b896b0", &fVbargraph14, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("12");
		ui_interface->declare(&fVbargraph4, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2b20f50", &fVbargraph4, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("13");
		ui_interface->declare(&fVbargraph3, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2b12800", &fVbargraph3, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("14");
		ui_interface->declare(&fVbargraph2, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2b03e70", &fVbargraph2, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("15");
		ui_interface->declare(&fVbargraph1, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2af5400", &fVbargraph1, -70.0f, 6.0f);
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
		ui_interface->declare(&fVbargraph10, "osc", "/output1");
		ui_interface->declare(&fVbargraph10, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2b5f200", &fVbargraph10, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("2");
		ui_interface->declare(&fVbargraph11, "osc", "/output2");
		ui_interface->declare(&fVbargraph11, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2b663f0", &fVbargraph11, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("3");
		ui_interface->declare(&fVbargraph16, "osc", "/output3");
		ui_interface->declare(&fVbargraph16, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2b9e090", &fVbargraph16, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("4");
		ui_interface->declare(&fVbargraph17, "osc", "/output4");
		ui_interface->declare(&fVbargraph17, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2ba4cf0", &fVbargraph17, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("5");
		ui_interface->declare(&fVbargraph18, "osc", "/output5");
		ui_interface->declare(&fVbargraph18, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2baba30", &fVbargraph18, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("6");
		ui_interface->declare(&fVbargraph19, "osc", "/output6");
		ui_interface->declare(&fVbargraph19, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2bb1b50", &fVbargraph19, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("7");
		ui_interface->declare(&fVbargraph20, "osc", "/output7");
		ui_interface->declare(&fVbargraph20, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2bbb720", &fVbargraph20, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("8");
		ui_interface->declare(&fVbargraph23, "osc", "/output8");
		ui_interface->declare(&fVbargraph23, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2bdeba0", &fVbargraph23, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("9");
		ui_interface->declare(&fVbargraph24, "osc", "/output9");
		ui_interface->declare(&fVbargraph24, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2be7d70", &fVbargraph24, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("10");
		ui_interface->declare(&fVbargraph25, "osc", "/output10");
		ui_interface->declare(&fVbargraph25, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2bf0c30", &fVbargraph25, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("11");
		ui_interface->declare(&fVbargraph26, "osc", "/output11");
		ui_interface->declare(&fVbargraph26, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2bf8ea0", &fVbargraph26, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("12");
		ui_interface->declare(&fVbargraph27, "osc", "/output12");
		ui_interface->declare(&fVbargraph27, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2c00430", &fVbargraph27, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("13");
		ui_interface->declare(&fVbargraph28, "osc", "/output13");
		ui_interface->declare(&fVbargraph28, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2c074c0", &fVbargraph28, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("14");
		ui_interface->declare(&fVbargraph29, "osc", "/output14");
		ui_interface->declare(&fVbargraph29, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2c0e9f0", &fVbargraph29, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("15");
		ui_interface->declare(&fVbargraph30, "osc", "/output15");
		ui_interface->declare(&fVbargraph30, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2c15130", &fVbargraph30, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("16");
		ui_interface->declare(&fVbargraph31, "osc", "/output16");
		ui_interface->declare(&fVbargraph31, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2c1c490", &fVbargraph31, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("17");
		ui_interface->declare(&fVbargraph32, "osc", "/output17");
		ui_interface->declare(&fVbargraph32, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2c238b0", &fVbargraph32, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("18");
		ui_interface->declare(&fVbargraph33, "osc", "/output18");
		ui_interface->declare(&fVbargraph33, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2c2af90", &fVbargraph33, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("19");
		ui_interface->declare(&fVbargraph34, "osc", "/output19");
		ui_interface->declare(&fVbargraph34, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2c37fa0", &fVbargraph34, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("20");
		ui_interface->declare(&fVbargraph35, "osc", "/output20");
		ui_interface->declare(&fVbargraph35, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2c42330", &fVbargraph35, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("21");
		ui_interface->declare(&fVbargraph36, "osc", "/output21");
		ui_interface->declare(&fVbargraph36, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2c4b970", &fVbargraph36, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("22");
		ui_interface->declare(&fVbargraph37, "osc", "/output22");
		ui_interface->declare(&fVbargraph37, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2c552b0", &fVbargraph37, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("23");
		ui_interface->declare(&fVbargraph38, "osc", "/output23");
		ui_interface->declare(&fVbargraph38, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2c5d000", &fVbargraph38, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("24");
		ui_interface->declare(&fVbargraph39, "osc", "/output24");
		ui_interface->declare(&fVbargraph39, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2c64a40", &fVbargraph39, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("25");
		ui_interface->declare(&fVbargraph40, "osc", "/output25");
		ui_interface->declare(&fVbargraph40, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2c6c920", &fVbargraph40, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Outputs 26-50");
		ui_interface->openVerticalBox("26");
		ui_interface->declare(&fVbargraph41, "osc", "/output26");
		ui_interface->declare(&fVbargraph41, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2c74700", &fVbargraph41, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("27");
		ui_interface->declare(&fVbargraph42, "osc", "/output27");
		ui_interface->declare(&fVbargraph42, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2c80640", &fVbargraph42, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("28");
		ui_interface->declare(&fVbargraph43, "osc", "/output28");
		ui_interface->declare(&fVbargraph43, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2c88640", &fVbargraph43, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("29");
		ui_interface->declare(&fVbargraph44, "osc", "/output29");
		ui_interface->declare(&fVbargraph44, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2c923f0", &fVbargraph44, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("30");
		ui_interface->declare(&fVbargraph45, "osc", "/output30");
		ui_interface->declare(&fVbargraph45, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2c9c0c0", &fVbargraph45, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("31");
		ui_interface->declare(&fVbargraph46, "osc", "/output31");
		ui_interface->declare(&fVbargraph46, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2ca8860", &fVbargraph46, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("32");
		ui_interface->declare(&fVbargraph47, "osc", "/output32");
		ui_interface->declare(&fVbargraph47, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2cb37e0", &fVbargraph47, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("33");
		ui_interface->declare(&fVbargraph48, "osc", "/output33");
		ui_interface->declare(&fVbargraph48, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2cbde90", &fVbargraph48, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("34");
		ui_interface->declare(&fVbargraph49, "osc", "/output34");
		ui_interface->declare(&fVbargraph49, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2cc7e80", &fVbargraph49, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("35");
		ui_interface->declare(&fVbargraph50, "osc", "/output35");
		ui_interface->declare(&fVbargraph50, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2cd2510", &fVbargraph50, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("36");
		ui_interface->declare(&fVbargraph51, "osc", "/output36");
		ui_interface->declare(&fVbargraph51, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2cdc4b0", &fVbargraph51, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("37");
		ui_interface->declare(&fVbargraph52, "osc", "/output37");
		ui_interface->declare(&fVbargraph52, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2ce6920", &fVbargraph52, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("38");
		ui_interface->declare(&fVbargraph53, "osc", "/output38");
		ui_interface->declare(&fVbargraph53, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2cf1fa0", &fVbargraph53, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("39");
		ui_interface->declare(&fVbargraph54, "osc", "/output39");
		ui_interface->declare(&fVbargraph54, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2cf95f0", &fVbargraph54, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("40");
		ui_interface->declare(&fVbargraph55, "osc", "/output40");
		ui_interface->declare(&fVbargraph55, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2d00930", &fVbargraph55, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("41");
		ui_interface->declare(&fVbargraph56, "osc", "/output41");
		ui_interface->declare(&fVbargraph56, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2d07f30", &fVbargraph56, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("42");
		ui_interface->declare(&fVbargraph57, "osc", "/output42");
		ui_interface->declare(&fVbargraph57, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2d0f640", &fVbargraph57, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("43");
		ui_interface->declare(&fVbargraph58, "osc", "/output43");
		ui_interface->declare(&fVbargraph58, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2d16a40", &fVbargraph58, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("44");
		ui_interface->declare(&fVbargraph59, "osc", "/output44");
		ui_interface->declare(&fVbargraph59, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2d1e2e0", &fVbargraph59, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("45");
		ui_interface->declare(&fVbargraph60, "osc", "/output45");
		ui_interface->declare(&fVbargraph60, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2d25a80", &fVbargraph60, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("46");
		ui_interface->declare(&fVbargraph61, "osc", "/output46");
		ui_interface->declare(&fVbargraph61, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2d2d300", &fVbargraph61, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("47");
		ui_interface->declare(&fVbargraph62, "osc", "/output47");
		ui_interface->declare(&fVbargraph62, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2d34a80", &fVbargraph62, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("48");
		ui_interface->declare(&fVbargraph63, "osc", "/output48");
		ui_interface->declare(&fVbargraph63, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2d3c2e0", &fVbargraph63, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("49");
		ui_interface->declare(&fVbargraph64, "osc", "/output49");
		ui_interface->declare(&fVbargraph64, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2d43fe0", &fVbargraph64, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("50");
		ui_interface->declare(&fVbargraph65, "osc", "/output50");
		ui_interface->declare(&fVbargraph65, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2d4bdc0", &fVbargraph65, -70.0f, 6.0f);
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
			float fTemp1 = (0.0126984129f * fTemp0);
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
			float fTemp7 = (2.43935947e-34f * fTemp6);
			fRec19[0] = (fRec18[1] + fRec19[1]);
			fRec20[0] = (fRec19[1] + fRec20[1]);
			float fTemp8 = (fRec3[0] * float(input13[i]));
			fRec18[0] = (0.0f - (fSlow8 * (((fSlow10 * fRec19[0]) + (fSlow9 * fRec20[0])) - (fSlow11 * fTemp8))));
			fRec21[0] = (fRec17[1] + fRec21[1]);
			fRec17[0] = (fSlow5 * (fRec18[0] - (fSlow12 * fRec21[0])));
			float fTemp9 = (fRec17[0] + (fSlow13 * fTemp8));
			fRec16[0] = max((fRec16[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp9))))));
			fVbargraph3 = FAUSTFLOAT(fRec16[0]);
			float fTemp10 = (5.03912267e-18f * fTemp9);
			float fTemp11 = (fRec3[0] * float(input12[i]));
			fRec25[0] = (fRec24[1] + fRec25[1]);
			fRec26[0] = (fRec25[1] + fRec26[1]);
			fRec24[0] = (fSlow8 * ((fSlow11 * fTemp11) - ((fSlow10 * fRec25[0]) + (fSlow9 * fRec26[0]))));
			fRec27[0] = (fRec23[1] + fRec27[1]);
			fRec23[0] = (fSlow5 * (fRec24[0] - (fSlow12 * fRec27[0])));
			float fTemp12 = (fRec23[0] + (fSlow13 * fTemp11));
			fRec22[0] = max((fRec22[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp12))))));
			fVbargraph4 = FAUSTFLOAT(fRec22[0]);
			float fTemp13 = (0.0335968435f * fTemp12);
			fRec30[0] = (fRec29[1] + fRec30[1]);
			fRec31[0] = (fRec30[1] + fRec31[1]);
			float fTemp14 = (fRec3[0] * float(input8[i]));
			fRec29[0] = (0.0f - (fSlow14 * (((fSlow16 * fRec30[0]) + (fSlow15 * fRec31[0])) - (fSlow11 * fTemp14))));
			float fTemp15 = (fRec29[0] + (fSlow13 * fTemp14));
			fRec28[0] = max((fRec28[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp15))))));
			fVbargraph5 = FAUSTFLOAT(fRec28[0]);
			float fTemp16 = (9.21991188e-35f * fTemp15);
			fRec34[0] = (fRec33[1] + fRec34[1]);
			fRec35[0] = (fRec34[1] + fRec35[1]);
			float fTemp17 = (fRec3[0] * float(input7[i]));
			fRec33[0] = (0.0f - (fSlow14 * (((fSlow16 * fRec34[0]) + (fSlow15 * fRec35[0])) - (fSlow11 * fTemp17))));
			float fTemp18 = (fRec33[0] + (fSlow13 * fTemp17));
			fRec32[0] = max((fRec32[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp18))))));
			fVbargraph6 = FAUSTFLOAT(fRec32[0]);
			float fTemp19 = (3.01145194e-18f * fTemp18);
			float fTemp20 = (fRec3[0] * float(input6[i]));
			fRec38[0] = (fRec37[1] + fRec38[1]);
			fRec39[0] = (fRec38[1] + fRec39[1]);
			fRec37[0] = (fSlow14 * ((fSlow11 * fTemp20) - ((fSlow16 * fRec38[0]) + (fSlow15 * fRec39[0]))));
			float fTemp21 = (fRec37[0] + (fSlow13 * fTemp20));
			fRec36[0] = max((fRec36[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp21))))));
			fVbargraph7 = FAUSTFLOAT(fRec36[0]);
			float fTemp22 = (0.0283945147f * fTemp21);
			float fTemp23 = (fRec3[0] * float(input2[i]));
			fRec42[0] = (fRec41[1] + fRec42[1]);
			fRec41[0] = (fSlow17 * ((fSlow11 * fTemp23) - (fSlow18 * fRec42[0])));
			float fTemp24 = (fRec41[0] + (fSlow13 * fTemp23));
			fRec40[0] = max((fRec40[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp24))))));
			fVbargraph8 = FAUSTFLOAT(fRec40[0]);
			float fTemp25 = (0.0219942965f * fTemp24);
			float fTemp26 = (fRec3[0] * float(input3[i]));
			fRec45[0] = (fRec44[1] + fRec45[1]);
			fRec44[0] = (fSlow17 * ((fSlow11 * fTemp26) - (fSlow18 * fRec45[0])));
			float fTemp27 = (fRec44[0] + (fSlow13 * fTemp26));
			fRec43[0] = max((fRec43[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp27))))));
			fVbargraph9 = FAUSTFLOAT(fRec43[0]);
			float fTemp28 = (1.34676222e-18f * fTemp27);
			float fTemp29 = (fRec1[0] * (fTemp1 + (fTemp4 + (fTemp7 + (fTemp10 + (fTemp13 + (fTemp16 + (fTemp19 + (fTemp22 + (fTemp25 + fTemp28))))))))));
			fRec0[0] = max((fRec0[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp29))))));
			fVbargraph10 = FAUSTFLOAT(fRec0[0]);
			output0[i] = FAUSTFLOAT(fTemp29);
			float fTemp30 = (0.0265606362f * fTemp3);
			float fTemp31 = (0.0219942965f * fTemp27);
			float fTemp32 = (0.0245903712f * fTemp15);
			float fTemp33 = (0.0141972573f * fTemp21);
			float fTemp34 = (0.0205737799f * fTemp9);
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
			float fTemp40 = (0.0219942965f * fTemp39);
			fRec58[0] = (fRec57[1] + fRec58[1]);
			fRec59[0] = (fRec58[1] + fRec59[1]);
			float fTemp41 = (fRec3[0] * float(input11[i]));
			fRec57[0] = (0.0f - (fSlow8 * (((fSlow10 * fRec58[0]) + (fSlow9 * fRec59[0])) - (fSlow11 * fTemp41))));
			fRec60[0] = (fRec56[1] + fRec60[1]);
			fRec56[0] = (fSlow5 * (fRec57[0] - (fSlow12 * fRec60[0])));
			float fTemp42 = (fRec56[0] + (fSlow13 * fTemp41));
			fRec55[0] = max((fRec55[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp42))))));
			fVbargraph14 = FAUSTFLOAT(fRec55[0]);
			float fTemp43 = (0.0205737799f * fTemp42);
			fRec64[0] = (fRec63[1] + fRec64[1]);
			fRec65[0] = (fRec64[1] + fRec65[1]);
			float fTemp44 = (fRec3[0] * float(input9[i]));
			fRec63[0] = (0.0f - (fSlow8 * (((fSlow10 * fRec64[0]) + (fSlow9 * fRec65[0])) - (fSlow11 * fTemp44))));
			fRec66[0] = (fRec62[1] + fRec66[1]);
			fRec62[0] = (fSlow5 * (fRec63[0] - (fSlow12 * fRec66[0])));
			float fTemp45 = (fRec62[0] + (fSlow13 * fTemp44));
			fRec61[0] = max((fRec61[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp45))))));
			fVbargraph15 = FAUSTFLOAT(fRec61[0]);
			float fTemp46 = (0.0265606362f * fTemp45);
			float fTemp47 = (fRec1[0] * ((fTemp1 + ((3.01145194e-18f * fTemp37) + (fTemp28 + fTemp40))) - (fTemp43 + (fTemp46 + ((4.87910954e-18f * fTemp3) + ((1.25978067e-18f * fTemp9) + (fTemp32 + fTemp33)))))));
			fRec47[0] = max((fRec47[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp47))))));
			fVbargraph16 = FAUSTFLOAT(fRec47[0]);
			output2[i] = FAUSTFLOAT(fTemp47);
			float fTemp48 = (fRec1[0] * ((fTemp1 + ((9.75821908e-18f * fTemp45) + ((2.69352445e-18f * fTemp39) + (fTemp32 + fTemp34)))) - ((2.51956133e-18f * fTemp42) + ((6.02290387e-18f * fTemp37) + (fTemp30 + (fTemp31 + fTemp33))))));
			fRec67[0] = max((fRec67[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp48))))));
			fVbargraph17 = FAUSTFLOAT(fRec67[0]);
			output3[i] = FAUSTFLOAT(fTemp48);
			float fTemp49 = (fRec1[0] * ((fTemp1 + (fTemp43 + (fTemp46 + ((9.03435581e-18f * fTemp37) + ((3.7793419e-18f * fTemp9) + (1.46373282e-17f * fTemp3)))))) - (fTemp40 + (fTemp32 + (fTemp33 + (4.04028677e-18f * fTemp27))))));
			fRec68[0] = max((fRec68[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp49))))));
			fVbargraph18 = FAUSTFLOAT(fRec68[0]);
			output4[i] = FAUSTFLOAT(fTemp49);
			float fTemp50 = (fRec1[0] * ((fTemp1 + (fTemp4 + (fTemp10 + (fTemp16 + (fTemp28 + fTemp22))))) - (fTemp7 + (fTemp13 + (fTemp25 + fTemp19)))));
			fRec69[0] = max((fRec69[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp50))))));
			fVbargraph19 = FAUSTFLOAT(fRec69[0]);
			output5[i] = FAUSTFLOAT(fTemp50);
			float fTemp51 = (0.0225749556f * fTemp0);
			float fTemp52 = (0.0166944042f * fTemp3);
			float fTemp53 = (0.0408927724f * fTemp6);
			float fTemp54 = (0.0387942903f * fTemp9);
			float fTemp55 = (0.0218581073f * fTemp15);
			float fTemp56 = (0.0437162146f * fTemp18);
			float fTemp57 = (0.0126197841f * fTemp21);
			float fTemp58 = (0.0276485607f * fTemp24);
			float fTemp59 = (0.0276485607f * fTemp27);
			float fTemp60 = (fTemp58 + fTemp59);
			float fTemp61 = (0.0105584692f * fTemp12);
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
			float fTemp65 = (5.00792059e-18f * fTemp64);
			fRec80[0] = (fRec79[1] + fRec80[1]);
			fRec81[0] = (fRec80[1] + fRec81[1]);
			float fTemp66 = (fRec3[0] * float(input5[i]));
			fRec79[0] = (0.0f - (fSlow14 * (((fSlow16 * fRec80[0]) + (fSlow15 * fRec81[0])) - (fSlow11 * fTemp66))));
			float fTemp67 = (fRec79[0] + (fSlow13 * fTemp66));
			fRec78[0] = max((fRec78[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp67))))));
			fVbargraph22 = FAUSTFLOAT(fRec78[0]);
			float fTemp68 = (0.0437162146f * fTemp67);
			float fTemp69 = (0.0387942903f * fTemp42);
			float fTemp70 = (2.676846e-18f * fTemp37);
			float fTemp71 = (0.0276485607f * fTemp39);
			float fTemp72 = (2.37546523e-18f * fTemp9);
			float fTemp73 = (2.676846e-18f * fTemp18);
			float fTemp74 = (1.69298611e-18f * fTemp27);
			float fTemp75 = (0.0166944042f * fTemp45);
			float fTemp76 = (3.06671236e-18f * fTemp3);
			float fTemp77 = (fRec1[0] * ((fTemp51 + (fTemp65 + (fTemp68 + (fTemp69 + (fTemp70 + (fTemp71 + (fTemp72 + (fTemp73 + (fTemp57 + (fTemp58 + fTemp74)))))))))) - (fTemp75 + ((fTemp53 + (fTemp55 + fTemp61)) + fTemp76))));
			fRec71[0] = max((fRec71[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp77))))));
			fVbargraph23 = FAUSTFLOAT(fRec71[0]);
			output7[i] = FAUSTFLOAT(fTemp77);
			float fTemp78 = (5.35369201e-18f * fTemp67);
			float fTemp79 = (4.75093046e-18f * fTemp42);
			float fTemp80 = (6.13342471e-18f * fTemp45);
			float fTemp81 = (3.38597223e-18f * fTemp39);
			float fTemp82 = (fTemp58 + fTemp57);
			float fTemp83 = (1.00158412e-17f * fTemp64);
			float fTemp84 = (5.35369201e-18f * fTemp37);
			float fTemp85 = (fRec1[0] * ((fTemp51 + (fTemp78 + (fTemp79 + (fTemp80 + (fTemp81 + (fTemp53 + (fTemp55 + fTemp82))))))) - (fTemp83 + (fTemp84 + (fTemp52 + (fTemp54 + (fTemp61 + (fTemp59 + fTemp56))))))));
			fRec82[0] = max((fRec82[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp85))))));
			fVbargraph24 = FAUSTFLOAT(fRec82[0]);
			output8[i] = FAUSTFLOAT(fTemp85);
			float fTemp86 = (1.50237622e-17f * fTemp64);
			float fTemp87 = (8.03053822e-18f * fTemp37);
			float fTemp88 = (9.20013769e-18f * fTemp3);
			float fTemp89 = (5.07895844e-18f * fTemp27);
			float fTemp90 = (8.03053822e-18f * fTemp18);
			float fTemp91 = (7.1263959e-18f * fTemp9);
			float fTemp92 = (fRec1[0] * ((fTemp51 + (fTemp86 + (fTemp75 + (fTemp87 + (fTemp88 + fTemp82))))) - (fTemp68 + (fTemp69 + (fTemp71 + (fTemp53 + ((fTemp61 + (fTemp55 + (fTemp89 + fTemp90))) + fTemp91)))))));
			fRec83[0] = max((fRec83[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp92))))));
			fVbargraph25 = FAUSTFLOAT(fRec83[0]);
			output9[i] = FAUSTFLOAT(fTemp92);
			float fTemp93 = (0.0437162146f * fTemp37);
			float fTemp94 = (0.0276485607f * fTemp39);
			float fTemp95 = (0.0258628614f * fTemp42);
			float fTemp96 = (0.0252395682f * fTemp21);
			float fTemp97 = (0.0258628614f * fTemp9);
			float fTemp98 = (fRec1[0] * ((fTemp51 + (fTemp93 + (fTemp94 + (fTemp59 + (2.676846e-18f * fTemp15))))) - ((fTemp95 + (fTemp96 + fTemp97)) + (0.0333888084f * (fTemp3 - fTemp45)))));
			fRec84[0] = max((fRec84[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp98))))));
			fVbargraph26 = FAUSTFLOAT(fRec84[0]);
			output10[i] = FAUSTFLOAT(fTemp98);
			float fTemp99 = (0.0258628614f * fTemp42);
			float fTemp100 = (fRec1[0] * ((fTemp51 + ((0.0333888084f * fTemp45) + (fTemp71 + ((0.0258628614f * fTemp9) + (0.0333888084f * fTemp3))))) - (fTemp99 + (fTemp93 + ((fTemp96 + (0.0276485607f * fTemp27)) + (8.03053822e-18f * fTemp15))))));
			fRec85[0] = max((fRec85[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp100))))));
			fVbargraph27 = FAUSTFLOAT(fRec85[0]);
			output11[i] = FAUSTFLOAT(fTemp100);
			float fTemp101 = (fRec1[0] * ((fTemp51 + (fTemp95 + (fTemp93 + ((0.0333888084f * fTemp3) + (fTemp97 + (1.33842306e-17f * fTemp15)))))) - ((fTemp94 + (fTemp59 + fTemp96)) + (0.0333888084f * fTemp45))));
			fRec86[0] = max((fRec86[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp101))))));
			fVbargraph28 = FAUSTFLOAT(fRec86[0]);
			output12[i] = FAUSTFLOAT(fTemp101);
			float fTemp102 = (fRec1[0] * ((fTemp51 + (fTemp99 + (0.0276485607f * fTemp27))) - ((0.0333888084f * fTemp45) + (fTemp93 + (fTemp71 + (((fTemp96 + (1.87379222e-17f * fTemp15)) + (0.0258628614f * fTemp9)) + (0.0333888084f * fTemp3)))))));
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
			float fTemp108 = (0.0210937504f * fTemp107);
			float fTemp109 = (0.0415974371f * fTemp64);
			float fTemp110 = (0.0272319149f * fTemp67);
			float fTemp111 = (0.0131542645f * fTemp42);
			float fTemp112 = (0.0169820823f * fTemp45);
			float fTemp113 = (0.0272319149f * fTemp37);
			float fTemp114 = (0.0210937504f * fTemp39);
			float fTemp115 = (5.23659549e-18f * fTemp21);
			float fTemp116 = ((0.0210937504f * fTemp27) + fTemp115);
			float fTemp117 = (0.0272319149f * fTemp18);
			float fTemp118 = (1.6674738e-18f * fTemp15);
			float fTemp119 = (0.0131542645f * fTemp9);
			float fTemp120 = (2.54710831e-18f * fTemp6);
			float fTemp121 = (0.0214808229f * fTemp12);
			float fTemp122 = (0.0169820823f * fTemp3);
			float fTemp123 = (fRec1[0] * ((fTemp108 + (fTemp109 + (fTemp110 + (fTemp111 + (fTemp112 + (fTemp113 + (fTemp114 + ((((fTemp116 + fTemp117) + fTemp118) + fTemp119) + fTemp120)))))))) - (fTemp121 + fTemp122)));
			fRec92[0] = max((fRec92[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp123))))));
			fVbargraph34 = FAUSTFLOAT(fRec92[0]);
			output18[i] = FAUSTFLOAT(fTemp123);
			float fTemp124 = (0.0272319149f * fTemp67);
			float fTemp125 = (0.0131542645f * fTemp42);
			float fTemp126 = (0.0169820823f * fTemp45);
			float fTemp127 = (0.0210937504f * fTemp39);
			float fTemp128 = (0.0169820823f * fTemp3);
			float fTemp129 = (0.0210937504f * fTemp27);
			float fTemp130 = (0.0272319149f * fTemp18);
			float fTemp131 = (5.00242149e-18f * fTemp15);
			float fTemp132 = (0.0131542645f * fTemp9);
			float fTemp133 = (7.64132493e-18f * fTemp6);
			float fTemp134 = (fRec1[0] * ((fTemp108 + (fTemp124 + (fTemp125 + (fTemp126 + (fTemp127 + (fTemp115 + fTemp128)))))) - (fTemp109 + (fTemp113 + (((fTemp121 + ((fTemp129 + fTemp130) + fTemp131)) + fTemp132) + fTemp133)))));
			fRec93[0] = max((fRec93[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp134))))));
			fVbargraph35 = FAUSTFLOAT(fRec93[0]);
			output19[i] = FAUSTFLOAT(fTemp134);
			float fTemp135 = (8.33736929e-18f * fTemp15);
			float fTemp136 = (1.27355415e-17f * fTemp6);
			float fTemp137 = (0.0169820823f * fTemp3);
			float fTemp138 = (0.0169820823f * fTemp45);
			float fTemp139 = (0.0131542645f * fTemp9);
			float fTemp140 = (fRec1[0] * ((fTemp109 + (fTemp113 + (((fTemp115 + fTemp135) + fTemp136) + fTemp137))) - ((0.0210937504f * (fTemp27 - fTemp107)) + (fTemp110 + (fTemp111 + (fTemp138 + (fTemp114 + ((fTemp117 + fTemp121) + fTemp139))))))));
			fRec94[0] = max((fRec94[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp140))))));
			fVbargraph36 = FAUSTFLOAT(fRec94[0]);
			output20[i] = FAUSTFLOAT(fTemp140);
			float fTemp141 = (fTemp115 + (0.0210937504f * fTemp27));
			float fTemp142 = (0.0272319149f * fTemp18);
			float fTemp143 = (0.0131542645f * fTemp42);
			float fTemp144 = (0.0169820823f * fTemp45);
			float fTemp145 = (1.16723167e-17f * fTemp15);
			float fTemp146 = (1.7829759e-17f * fTemp6);
			float fTemp147 = (fRec1[0] * (((fTemp132 + (fTemp141 + fTemp142)) + (0.0210937504f * (fTemp107 - fTemp39))) - (fTemp109 + (fTemp124 + (fTemp143 + (fTemp144 + (fTemp113 + (fTemp122 + ((fTemp121 + fTemp145) + fTemp146)))))))));
			fRec95[0] = max((fRec95[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp147))))));
			fVbargraph37 = FAUSTFLOAT(fRec95[0]);
			output21[i] = FAUSTFLOAT(fTemp147);
			float fTemp148 = (0.0210937504f * (fTemp0 - fTemp24));
			float fTemp149 = (fRec1[0] * ((fTemp148 + (fTemp111 + (fTemp112 + (fTemp113 + (fTemp114 + (fTemp119 + (fTemp121 + (fTemp116 + fTemp118)))))))) - (fTemp109 + (fTemp110 + (fTemp122 + (fTemp117 + fTemp120))))));
			fRec96[0] = max((fRec96[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp149))))));
			fVbargraph38 = FAUSTFLOAT(fRec96[0]);
			output22[i] = FAUSTFLOAT(fTemp149);
			float fTemp150 = (fRec1[0] * ((fTemp148 + (fTemp109 + (fTemp125 + (fTemp126 + (fTemp127 + (fTemp128 + (fTemp133 + (fTemp121 + (fTemp115 + fTemp130))))))))) - (fTemp124 + (fTemp113 + (fTemp132 + (fTemp129 + fTemp131))))));
			fRec97[0] = max((fRec97[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp150))))));
			fVbargraph39 = FAUSTFLOAT(fRec97[0]);
			output23[i] = FAUSTFLOAT(fTemp150);
			float fTemp151 = (fRec1[0] * ((fTemp110 + (fTemp113 + (fTemp137 + (fTemp121 + (fTemp135 + (fTemp115 + fTemp117)))))) - ((0.0210937504f * ((fTemp24 + fTemp27) - fTemp0)) + (fTemp109 + (fTemp111 + (fTemp138 + (fTemp114 + (fTemp136 + fTemp139))))))));
			fRec98[0] = max((fRec98[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp151))))));
			fVbargraph40 = FAUSTFLOAT(fRec98[0]);
			output24[i] = FAUSTFLOAT(fTemp151);
			float fTemp152 = (fRec1[0] * ((fTemp109 + (fTemp124 + (fTemp146 + (fTemp132 + (fTemp121 + fTemp141))))) - ((0.0210937504f * ((fTemp24 + fTemp39) - fTemp0)) + (fTemp143 + (fTemp144 + (fTemp113 + (fTemp122 + (fTemp142 + fTemp145))))))));
			fRec99[0] = max((fRec99[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp152))))));
			fVbargraph41 = FAUSTFLOAT(fRec99[0]);
			output25[i] = FAUSTFLOAT(fTemp152);
			float fTemp153 = (0.0201733354f * fTemp0);
			float fTemp154 = (0.0105351806f * (fTemp27 + fTemp39));
			float fTemp155 = (0.0213084519f * (fTemp18 + fTemp67));
			float fTemp156 = (0.0169982649f * fTemp64);
			float fTemp157 = (0.0304601658f * fTemp42);
			float fTemp158 = (0.00231317081f * fTemp45);
			float fTemp159 = (0.0071028173f * fTemp37);
			float fTemp160 = (0.0304601658f * fTemp9);
			float fTemp161 = (0.0263335984f * fTemp12);
			float fTemp162 = (0.0316055417f * fTemp24);
			float fTemp163 = (0.0328065082f * fTemp21);
			float fTemp164 = (fTemp162 + fTemp163);
			float fTemp165 = (fTemp161 + fTemp164);
			float fTemp166 = (1.14222021e-18f * fTemp15);
			float fTemp167 = (2.73352938e-18f * fTemp6);
			float fTemp168 = (0.00231317081f * fTemp3);
			float fTemp169 = (fRec1[0] * ((fTemp153 + (fTemp154 + (fTemp155 + (fTemp156 + (fTemp157 + (fTemp158 + (fTemp159 + (fTemp160 + fTemp165)))))))) - ((fTemp166 + fTemp167) + fTemp168)));
			fRec100[0] = max((fRec100[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp169))))));
			fVbargraph42 = FAUSTFLOAT(fRec100[0]);
			output26[i] = FAUSTFLOAT(fTemp169);
			float fTemp170 = (0.00231317081f * fTemp3);
			float fTemp171 = (0.0105351806f * (fTemp27 - fTemp39));
			float fTemp172 = (0.0213084519f * (fTemp18 - fTemp67));
			float fTemp173 = (1.30476637e-18f * fTemp15);
			float fTemp174 = (3.1225305e-18f * fTemp6);
			float fTemp175 = (fRec1[0] * ((fTemp153 + (fTemp157 + (fTemp158 + (fTemp170 + fTemp165)))) - (fTemp171 + (fTemp172 + (fTemp156 + (fTemp159 + ((fTemp160 + fTemp173) + fTemp174)))))));
			fRec101[0] = max((fRec101[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp175))))));
			fVbargraph43 = FAUSTFLOAT(fRec101[0]);
			output27[i] = FAUSTFLOAT(fTemp175);
			float fTemp176 = (0.00231317081f * fTemp3);
			float fTemp177 = (5.20421758e-18f * fTemp6);
			float fTemp178 = (2.17461068e-18f * fTemp15);
			float fTemp179 = (0.0213084519f * fTemp67);
			float fTemp180 = (0.0304601658f * fTemp42);
			float fTemp181 = (0.00231317081f * fTemp45);
			float fTemp182 = (0.0105351806f * fTemp39);
			float fTemp183 = (0.0105351806f * fTemp27);
			float fTemp184 = (0.0213084519f * fTemp18);
			float fTemp185 = (0.0304601658f * fTemp9);
			float fTemp186 = (fRec1[0] * ((fTemp153 + (fTemp156 + (fTemp159 + (fTemp176 + (fTemp177 + (fTemp161 + (fTemp178 + fTemp164))))))) - (fTemp179 + (fTemp180 + (fTemp181 + (fTemp182 + ((fTemp183 + fTemp184) + fTemp185)))))));
			fRec102[0] = max((fRec102[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp186))))));
			fVbargraph44 = FAUSTFLOAT(fRec102[0]);
			output28[i] = FAUSTFLOAT(fTemp186);
			float fTemp187 = (0.0304601658f * fTemp9);
			float fTemp188 = (0.0213084519f * fTemp18);
			float fTemp189 = (0.0105351806f * fTemp27);
			float fTemp190 = (0.0213084519f * fTemp67);
			float fTemp191 = (0.0304601658f * fTemp42);
			float fTemp192 = (0.00231317081f * fTemp45);
			float fTemp193 = (0.0105351806f * fTemp39);
			float fTemp194 = (3.04445499e-18f * fTemp15);
			float fTemp195 = (7.28590444e-18f * fTemp6);
			float fTemp196 = (0.00231317081f * fTemp3);
			float fTemp197 = (fRec1[0] * ((fTemp153 + (fTemp187 + (fTemp161 + (fTemp188 + (fTemp163 + (fTemp162 + fTemp189)))))) - (fTemp156 + (fTemp190 + (fTemp191 + (fTemp192 + (fTemp159 + (fTemp193 + ((fTemp194 + fTemp195) + fTemp196)))))))));
			fRec103[0] = max((fRec103[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp197))))));
			fVbargraph45 = FAUSTFLOAT(fRec103[0]);
			output29[i] = FAUSTFLOAT(fTemp197);
			float fTemp198 = (0.0169982649f * fTemp64);
			float fTemp199 = (0.0071028173f * fTemp67);
			float fTemp200 = (0.0300712213f * fTemp45);
			float fTemp201 = (0.0213084519f * fTemp37);
			float fTemp202 = (0.0105351806f * fTemp39);
			float fTemp203 = (0.0208185371f * fTemp3);
			float fTemp204 = (0.0226643533f * fTemp6);
			float fTemp205 = (0.0284112692f * fTemp15);
			float fTemp206 = (0.0213084519f * fTemp18);
			float fTemp207 = (0.0105351806f * fTemp24);
			float fTemp208 = (0.0316055417f * fTemp27);
			float fTemp209 = (fTemp207 + fTemp208);
			float fTemp210 = (0.00537532335f * fTemp42);
			float fTemp211 = (0.0164032541f * fTemp21);
			float fTemp212 = (0.020481687f * fTemp12);
			float fTemp213 = (fTemp211 + fTemp212);
			float fTemp214 = (0.0161259696f * fTemp9);
			float fTemp215 = (fRec1[0] * ((fTemp153 + (fTemp198 + (fTemp199 + (fTemp200 + (fTemp201 + (fTemp202 + (fTemp203 + (fTemp204 + (fTemp205 + (fTemp206 + fTemp209)))))))))) - (fTemp210 + (fTemp213 + fTemp214))));
			fRec104[0] = max((fRec104[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp215))))));
			fVbargraph46 = FAUSTFLOAT(fRec104[0]);
			output30[i] = FAUSTFLOAT(fTemp215);
			float fTemp216 = (0.0169982649f * fTemp64);
			float fTemp217 = (0.0213084519f * fTemp67);
			float fTemp218 = (0.0213084519f * fTemp37);
			float fTemp219 = (0.0316055417f * fTemp39);
			float fTemp220 = (0.0071028173f * fTemp18);
			float fTemp221 = (0.0105351806f * fTemp27);
			float fTemp222 = (0.0161259696f * fTemp42);
			float fTemp223 = (0.0208185371f * fTemp45);
			float fTemp224 = (0.0284112692f * fTemp15);
			float fTemp225 = (0.00537532335f * fTemp9);
			float fTemp226 = (0.0226643533f * fTemp6);
			float fTemp227 = (0.0300712213f * fTemp3);
			float fTemp228 = (fRec1[0] * ((fTemp153 + (fTemp216 + (fTemp217 + (fTemp218 + (fTemp219 + (fTemp220 + (fTemp207 + fTemp221))))))) - (fTemp222 + (fTemp223 + ((((fTemp212 + (fTemp211 + fTemp224)) + fTemp225) + fTemp226) + fTemp227)))));
			fRec105[0] = max((fRec105[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp228))))));
			fVbargraph47 = FAUSTFLOAT(fRec105[0]);
			output31[i] = FAUSTFLOAT(fTemp228);
			float fTemp229 = (0.0213084519f * fTemp67);
			float fTemp230 = (0.0300712213f * fTemp3);
			float fTemp231 = (0.00537532335f * fTemp9);
			float fTemp232 = (0.0169982649f * fTemp64);
			float fTemp233 = (0.0161259696f * fTemp42);
			float fTemp234 = (0.0208185371f * fTemp45);
			float fTemp235 = (0.0213084519f * fTemp37);
			float fTemp236 = (0.0105351806f * fTemp27);
			float fTemp237 = (0.0071028173f * fTemp18);
			float fTemp238 = (0.0284112692f * fTemp15);
			float fTemp239 = (fRec1[0] * ((fTemp153 + (fTemp229 + (fTemp219 + (fTemp230 + (fTemp207 + fTemp231))))) - (fTemp232 + (fTemp233 + (fTemp234 + (fTemp235 + (fTemp226 + (fTemp212 + (((fTemp211 + fTemp236) + fTemp237) + fTemp238)))))))));
			fRec106[0] = max((fRec106[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp239))))));
			fVbargraph48 = FAUSTFLOAT(fRec106[0]);
			output32[i] = FAUSTFLOAT(fTemp239);
			float fTemp240 = (0.0071028173f * fTemp67);
			float fTemp241 = (0.0300712213f * fTemp45);
			float fTemp242 = (0.0105351806f * fTemp39);
			float fTemp243 = (0.0226643533f * fTemp6);
			float fTemp244 = (0.0284112692f * fTemp15);
			float fTemp245 = (0.00537532335f * fTemp42);
			float fTemp246 = (0.0213084519f * fTemp37);
			float fTemp247 = (fTemp208 + fTemp211);
			float fTemp248 = (0.0208185371f * fTemp3);
			float fTemp249 = (fRec1[0] * ((fTemp153 + (fTemp240 + (fTemp241 + (fTemp242 + (fTemp243 + (fTemp214 + (fTemp207 + fTemp244))))))) - (fTemp198 + (fTemp245 + (fTemp246 + ((fTemp212 + (fTemp206 + fTemp247)) + fTemp248))))));
			fRec107[0] = max((fRec107[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp249))))));
			fVbargraph49 = FAUSTFLOAT(fRec107[0]);
			output33[i] = FAUSTFLOAT(fTemp249);
			float fTemp250 = (0.0169982649f * fTemp64);
			float fTemp251 = (0.00537532335f * fTemp42);
			float fTemp252 = (0.0213084519f * fTemp37);
			float fTemp253 = (0.0226643533f * fTemp6);
			float fTemp254 = (0.0284112692f * fTemp15);
			float fTemp255 = (0.0071028173f * fTemp67);
			float fTemp256 = (0.0300712213f * fTemp45);
			float fTemp257 = (0.0105351806f * fTemp39);
			float fTemp258 = (0.0213084519f * fTemp18);
			float fTemp259 = (0.0208185371f * fTemp3);
			float fTemp260 = (fRec1[0] * ((fTemp153 + (fTemp250 + (fTemp251 + (fTemp252 + (fTemp253 + (fTemp214 + (fTemp207 + fTemp254))))))) - (fTemp255 + (fTemp256 + (fTemp257 + ((fTemp212 + (fTemp247 + fTemp258)) + fTemp259))))));
			fRec108[0] = max((fRec108[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp260))))));
			fVbargraph50 = FAUSTFLOAT(fRec108[0]);
			output34[i] = FAUSTFLOAT(fTemp260);
			float fTemp261 = (0.0169982649f * fTemp64);
			float fTemp262 = (0.0208185371f * fTemp45);
			float fTemp263 = (0.0213084519f * fTemp37);
			float fTemp264 = (0.0300712213f * fTemp3);
			float fTemp265 = (0.00537532335f * fTemp9);
			float fTemp266 = (0.0105351806f * fTemp27);
			float fTemp267 = (0.0071028173f * fTemp18);
			float fTemp268 = (0.0284112692f * fTemp15);
			float fTemp269 = (fRec1[0] * ((fTemp153 + (fTemp261 + (fTemp222 + (fTemp262 + (fTemp263 + (fTemp264 + (fTemp207 + fTemp265))))))) - (fTemp217 + (fTemp219 + (fTemp253 + (fTemp212 + (((fTemp211 + fTemp266) + fTemp267) + fTemp268)))))));
			fRec109[0] = max((fRec109[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp269))))));
			fVbargraph51 = FAUSTFLOAT(fRec109[0]);
			output35[i] = FAUSTFLOAT(fTemp269);
			float fTemp270 = (0.0208185371f * fTemp45);
			float fTemp271 = (0.0071028173f * fTemp18);
			float fTemp272 = (0.0105351806f * fTemp27);
			float fTemp273 = (0.0169982649f * fTemp64);
			float fTemp274 = (0.0213084519f * fTemp37);
			float fTemp275 = (0.0284112692f * fTemp15);
			float fTemp276 = (0.00537532335f * fTemp9);
			float fTemp277 = (0.0226643533f * fTemp6);
			float fTemp278 = (0.0300712213f * fTemp3);
			float fTemp279 = (fRec1[0] * ((fTemp153 + (fTemp233 + (fTemp270 + (fTemp271 + (fTemp207 + fTemp272))))) - (fTemp273 + (fTemp229 + (fTemp274 + (fTemp219 + ((((fTemp212 + (fTemp211 + fTemp275)) + fTemp276) + fTemp277) + fTemp278)))))));
			fRec110[0] = max((fRec110[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp279))))));
			fVbargraph52 = FAUSTFLOAT(fRec110[0]);
			output36[i] = FAUSTFLOAT(fTemp279);
			float fTemp280 = (0.00537532335f * fTemp42);
			float fTemp281 = (0.0208185371f * fTemp3);
			float fTemp282 = (0.0226643533f * fTemp6);
			float fTemp283 = (0.0284112692f * fTemp15);
			float fTemp284 = (0.0213084519f * fTemp18);
			float fTemp285 = (0.0316055417f * fTemp27);
			float fTemp286 = (0.0169982649f * fTemp64);
			float fTemp287 = (0.0071028173f * fTemp67);
			float fTemp288 = (0.0300712213f * fTemp45);
			float fTemp289 = (0.0213084519f * fTemp37);
			float fTemp290 = (0.0105351806f * fTemp39);
			float fTemp291 = (0.0161259696f * fTemp9);
			float fTemp292 = (fRec1[0] * ((fTemp153 + (fTemp280 + (fTemp281 + (fTemp282 + (fTemp283 + (fTemp284 + (fTemp207 + fTemp285))))))) - (fTemp286 + (fTemp287 + (fTemp288 + (fTemp289 + (fTemp290 + (fTemp213 + fTemp291))))))));
			fRec111[0] = max((fRec111[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp292))))));
			fVbargraph53 = FAUSTFLOAT(fRec111[0]);
			output37[i] = FAUSTFLOAT(fTemp292);
			float fTemp293 = (fTemp207 + fTemp211);
			float fTemp294 = (fRec1[0] * ((fTemp153 + (fTemp200 + (fTemp201 + (fTemp202 + (fTemp203 + (fTemp212 + (fTemp208 + fTemp205))))))) - (fTemp198 + (fTemp199 + (fTemp210 + (fTemp204 + (fTemp214 + (fTemp206 + fTemp293))))))));
			fRec112[0] = max((fRec112[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp294))))));
			fVbargraph54 = FAUSTFLOAT(fRec112[0]);
			output38[i] = FAUSTFLOAT(fTemp294);
			float fTemp295 = (fRec1[0] * ((fTemp153 + (fTemp218 + (fTemp219 + (fTemp226 + (fTemp212 + fTemp221))))) - (fTemp216 + (fTemp217 + (fTemp222 + (fTemp223 + (fTemp227 + (fTemp225 + (fTemp224 + (fTemp220 + fTemp293))))))))));
			fRec113[0] = max((fRec113[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp295))))));
			fVbargraph55 = FAUSTFLOAT(fRec113[0]);
			output39[i] = FAUSTFLOAT(fTemp295);
			float fTemp296 = (fRec1[0] * ((fTemp153 + (fTemp232 + (fTemp219 + (fTemp230 + (fTemp226 + (fTemp231 + (fTemp212 + fTemp237))))))) - (fTemp229 + (fTemp233 + (fTemp234 + (fTemp235 + (fTemp238 + (fTemp211 + (fTemp207 + fTemp236)))))))));
			fRec114[0] = max((fRec114[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp296))))));
			fVbargraph56 = FAUSTFLOAT(fRec114[0]);
			output40[i] = FAUSTFLOAT(fTemp296);
			float fTemp297 = (fTemp211 + fTemp209);
			float fTemp298 = (fRec1[0] * ((fTemp153 + (fTemp198 + (fTemp241 + (fTemp242 + (fTemp214 + (fTemp212 + (fTemp206 + fTemp244))))))) - (fTemp240 + (fTemp245 + (fTemp246 + (fTemp248 + (fTemp243 + fTemp297)))))));
			fRec115[0] = max((fRec115[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp298))))));
			fVbargraph57 = FAUSTFLOAT(fRec115[0]);
			output41[i] = FAUSTFLOAT(fTemp298);
			float fTemp299 = (fRec1[0] * ((fTemp153 + (fTemp255 + (fTemp251 + (fTemp252 + (fTemp214 + (fTemp212 + (fTemp254 + fTemp258))))))) - (fTemp250 + (fTemp256 + (fTemp257 + (fTemp259 + (fTemp253 + fTemp297)))))));
			fRec116[0] = max((fRec116[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp299))))));
			fVbargraph58 = FAUSTFLOAT(fRec116[0]);
			output42[i] = FAUSTFLOAT(fTemp299);
			float fTemp300 = (fRec1[0] * ((fTemp153 + (fTemp217 + (fTemp222 + (fTemp262 + (fTemp263 + (fTemp264 + (fTemp253 + (fTemp265 + (fTemp212 + fTemp267))))))))) - (fTemp261 + (fTemp219 + (fTemp268 + (fTemp211 + (fTemp207 + fTemp266)))))));
			fRec117[0] = max((fRec117[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp300))))));
			fVbargraph59 = FAUSTFLOAT(fRec117[0]);
			output43[i] = FAUSTFLOAT(fTemp300);
			float fTemp301 = (fRec1[0] * ((fTemp153 + (fTemp273 + (fTemp229 + (fTemp233 + (fTemp270 + (fTemp277 + (fTemp212 + fTemp272))))))) - (fTemp274 + (fTemp219 + (fTemp278 + (fTemp276 + (fTemp275 + (fTemp271 + fTemp293))))))));
			fRec118[0] = max((fRec118[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp301))))));
			fVbargraph60 = FAUSTFLOAT(fRec118[0]);
			output44[i] = FAUSTFLOAT(fTemp301);
			float fTemp302 = (fRec1[0] * ((fTemp153 + (fTemp286 + (fTemp287 + (fTemp280 + (fTemp281 + (fTemp212 + (fTemp285 + fTemp283))))))) - (fTemp288 + (fTemp289 + (fTemp290 + (fTemp282 + (fTemp291 + (fTemp284 + fTemp293))))))));
			fRec119[0] = max((fRec119[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp302))))));
			fVbargraph61 = FAUSTFLOAT(fRec119[0]);
			output45[i] = FAUSTFLOAT(fTemp302);
			float fTemp303 = (fRec1[0] * ((fTemp153 + (fTemp154 + (fTemp157 + (fTemp158 + (fTemp159 + (fTemp167 + (fTemp163 + fTemp160))))))) - (fTemp155 + (fTemp156 + (fTemp168 + (fTemp161 + (fTemp162 + fTemp166)))))));
			fRec120[0] = max((fRec120[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp303))))));
			fVbargraph62 = FAUSTFLOAT(fRec120[0]);
			output46[i] = FAUSTFLOAT(fTemp303);
			float fTemp304 = (fRec1[0] * ((fTemp153 + (fTemp172 + (fTemp156 + (fTemp157 + (fTemp158 + (fTemp170 + (fTemp163 + fTemp174))))))) - (fTemp171 + (fTemp159 + (fTemp160 + (fTemp161 + (fTemp162 + fTemp173)))))));
			fRec121[0] = max((fRec121[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp304))))));
			fVbargraph63 = FAUSTFLOAT(fRec121[0]);
			output47[i] = FAUSTFLOAT(fTemp304);
			float fTemp305 = (fRec1[0] * ((fTemp153 + (fTemp179 + (fTemp159 + (fTemp176 + (fTemp178 + (fTemp163 + fTemp184)))))) - (fTemp156 + (fTemp180 + (fTemp181 + (fTemp182 + (fTemp177 + (fTemp185 + (fTemp161 + (fTemp162 + fTemp183))))))))));
			fRec122[0] = max((fRec122[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp305))))));
			fVbargraph64 = FAUSTFLOAT(fRec122[0]);
			output48[i] = FAUSTFLOAT(fTemp305);
			float fTemp306 = (fRec1[0] * ((fTemp153 + (fTemp156 + (fTemp190 + (fTemp195 + (fTemp187 + (fTemp163 + fTemp189)))))) - (fTemp191 + (fTemp192 + (fTemp159 + (fTemp193 + (fTemp196 + (fTemp161 + (fTemp194 + (fTemp162 + fTemp188))))))))));
			fRec123[0] = max((fRec123[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp306))))));
			fVbargraph65 = FAUSTFLOAT(fRec123[0]);
			output49[i] = FAUSTFLOAT(fTemp306);
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
			fRec100[1] = fRec100[0];
			fRec101[1] = fRec101[0];
			fRec102[1] = fRec102[0];
			fRec103[1] = fRec103[0];
			fRec104[1] = fRec104[0];
			fRec105[1] = fRec105[0];
			fRec106[1] = fRec106[0];
			fRec107[1] = fRec107[0];
			fRec108[1] = fRec108[0];
			fRec109[1] = fRec109[0];
			fRec110[1] = fRec110[0];
			fRec111[1] = fRec111[0];
			fRec112[1] = fRec112[0];
			fRec113[1] = fRec113[0];
			fRec114[1] = fRec114[0];
			fRec115[1] = fRec115[0];
			fRec116[1] = fRec116[0];
			fRec117[1] = fRec117[0];
			fRec118[1] = fRec118[0];
			fRec119[1] = fRec119[0];
			fRec120[1] = fRec120[0];
			fRec121[1] = fRec121[0];
			fRec122[1] = fRec122[0];
			fRec123[1] = fRec123[0];
			
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
