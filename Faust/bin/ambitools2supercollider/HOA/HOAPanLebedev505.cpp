/* ------------------------------------------------------------
author: "Pierre Lecomte"
copyright: "(c) Pierre Lecomte 2014"
license: "GPL"
name: "HOAPanLebedev505"
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
	float fRec3[2];
	float fRec4[2];
	float fRec5[2];
	float fRec2[2];
	float fRec6[2];
	float fVec0[2];
	float fRec7[2];
	float fRec8[2];
	float fRec9[2];
	float fRec1[2];
	float fRec10[2];
	float fVec1[2];
	float fRec11[2];
	float fRec13[2];
	float fRec14[2];
	float fRec16[2];
	float fRec17[2];
	float fRec15[2];
	float fRec12[2];
	float fRec19[2];
	float fRec20[2];
	float fRec18[2];
	float fRec21[2];
	float fVec2[2];
	float fRec22[2];
	float fRec24[2];
	float fRec25[2];
	float fRec23[2];
	float fRec28[2];
	float fRec27[2];
	float fRec26[2];
	float fRec30[2];
	float fRec29[2];
	float fRec34[2];
	float fRec35[2];
	float fRec33[2];
	float fRec36[2];
	float fVec3[2];
	float fRec37[2];
	float fRec38[2];
	float fRec32[2];
	float fRec31[2];
	float fRec41[2];
	float fRec42[2];
	float fRec40[2];
	float fRec43[2];
	float fRec39[2];
	float fRec48[2];
	float fRec49[2];
	float fRec47[2];
	float fRec50[2];
	float fVec4[2];
	float fRec51[2];
	float fRec52[2];
	float fRec53[2];
	float fRec46[2];
	float fRec54[2];
	float fVec5[2];
	float fRec55[2];
	float fRec56[2];
	float fRec45[2];
	float fRec44[2];
	float fRec59[2];
	float fRec60[2];
	float fRec62[2];
	float fRec63[2];
	float fRec61[2];
	float fRec58[2];
	float fRec64[2];
	float fRec57[2];
	float fRec0[2];
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
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fCheckbox0;
	float fConst2;
	FAUSTFLOAT fEntry0;
	float fConst3;
	FAUSTFLOAT fHslider2;
	float fConst4;
	FAUSTFLOAT fCheckbox1;
	FAUSTFLOAT fHslider3;
	float fConst5;
	float fConst6;
	FAUSTFLOAT fCheckbox2;
	float fConst7;
	float fConst8;
	FAUSTFLOAT fCheckbox3;
	float fConst9;
	FAUSTFLOAT fCheckbox4;
	float fConst10;
	float fConst11;
	float fConst12;
	float fConst13;
	FAUSTFLOAT fCheckbox5;
	float fConst14;
	float fConst15;
	float fConst16;
	float fConst17;
	float fConst18;
	float fConst19;
	FAUSTFLOAT fCheckbox6;
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
		m->declare("name", "HOAPanLebedev505");
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
		fConst2 = (984.711609f / fConst0);
		fConst3 = (170.0f / fConst0);
		fConst4 = (715.288391f / fConst0);
		fConst5 = (1430.57678f / fConst0);
		fConst6 = (1969.42322f / fConst0);
		fConst7 = (510.0f / fConst0);
		fConst8 = (1020.0f / fConst0);
		fConst9 = (340.0f / fConst0);
		fConst10 = (789.54303f / fConst0);
		fConst11 = (394.771515f / fConst0);
		fConst12 = (625.228516f / fConst0);
		fConst13 = (1250.45703f / fConst0);
		fConst14 = (1239.89111f / fConst0);
		fConst15 = (619.945557f / fConst0);
		fConst16 = (1139.66516f / fConst0);
		fConst17 = (790.389282f / fConst0);
		fConst18 = (1580.77856f / fConst0);
		fConst19 = (2279.33032f / fConst0);
		
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
		fCheckbox5 = FAUSTFLOAT(0.0f);
		fCheckbox6 = FAUSTFLOAT(0.0f);
		
	}
	
	virtual void instanceClear() {
		for (int i0 = 0; (i0 < 2); i0 = (i0 + 1)) {
			fRec3[i0] = 0.0f;
			
		}
		for (int i1 = 0; (i1 < 2); i1 = (i1 + 1)) {
			fRec4[i1] = 0.0f;
			
		}
		for (int i2 = 0; (i2 < 2); i2 = (i2 + 1)) {
			fRec5[i2] = 0.0f;
			
		}
		for (int i3 = 0; (i3 < 2); i3 = (i3 + 1)) {
			fRec2[i3] = 0.0f;
			
		}
		for (int i4 = 0; (i4 < 2); i4 = (i4 + 1)) {
			fRec6[i4] = 0.0f;
			
		}
		for (int i5 = 0; (i5 < 2); i5 = (i5 + 1)) {
			fVec0[i5] = 0.0f;
			
		}
		for (int i6 = 0; (i6 < 2); i6 = (i6 + 1)) {
			fRec7[i6] = 0.0f;
			
		}
		for (int i7 = 0; (i7 < 2); i7 = (i7 + 1)) {
			fRec8[i7] = 0.0f;
			
		}
		for (int i8 = 0; (i8 < 2); i8 = (i8 + 1)) {
			fRec9[i8] = 0.0f;
			
		}
		for (int i9 = 0; (i9 < 2); i9 = (i9 + 1)) {
			fRec1[i9] = 0.0f;
			
		}
		for (int i10 = 0; (i10 < 2); i10 = (i10 + 1)) {
			fRec10[i10] = 0.0f;
			
		}
		for (int i11 = 0; (i11 < 2); i11 = (i11 + 1)) {
			fVec1[i11] = 0.0f;
			
		}
		for (int i12 = 0; (i12 < 2); i12 = (i12 + 1)) {
			fRec11[i12] = 0.0f;
			
		}
		for (int i13 = 0; (i13 < 2); i13 = (i13 + 1)) {
			fRec13[i13] = 0.0f;
			
		}
		for (int i14 = 0; (i14 < 2); i14 = (i14 + 1)) {
			fRec14[i14] = 0.0f;
			
		}
		for (int i15 = 0; (i15 < 2); i15 = (i15 + 1)) {
			fRec16[i15] = 0.0f;
			
		}
		for (int i16 = 0; (i16 < 2); i16 = (i16 + 1)) {
			fRec17[i16] = 0.0f;
			
		}
		for (int i17 = 0; (i17 < 2); i17 = (i17 + 1)) {
			fRec15[i17] = 0.0f;
			
		}
		for (int i18 = 0; (i18 < 2); i18 = (i18 + 1)) {
			fRec12[i18] = 0.0f;
			
		}
		for (int i19 = 0; (i19 < 2); i19 = (i19 + 1)) {
			fRec19[i19] = 0.0f;
			
		}
		for (int i20 = 0; (i20 < 2); i20 = (i20 + 1)) {
			fRec20[i20] = 0.0f;
			
		}
		for (int i21 = 0; (i21 < 2); i21 = (i21 + 1)) {
			fRec18[i21] = 0.0f;
			
		}
		for (int i22 = 0; (i22 < 2); i22 = (i22 + 1)) {
			fRec21[i22] = 0.0f;
			
		}
		for (int i23 = 0; (i23 < 2); i23 = (i23 + 1)) {
			fVec2[i23] = 0.0f;
			
		}
		for (int i24 = 0; (i24 < 2); i24 = (i24 + 1)) {
			fRec22[i24] = 0.0f;
			
		}
		for (int i25 = 0; (i25 < 2); i25 = (i25 + 1)) {
			fRec24[i25] = 0.0f;
			
		}
		for (int i26 = 0; (i26 < 2); i26 = (i26 + 1)) {
			fRec25[i26] = 0.0f;
			
		}
		for (int i27 = 0; (i27 < 2); i27 = (i27 + 1)) {
			fRec23[i27] = 0.0f;
			
		}
		for (int i28 = 0; (i28 < 2); i28 = (i28 + 1)) {
			fRec28[i28] = 0.0f;
			
		}
		for (int i29 = 0; (i29 < 2); i29 = (i29 + 1)) {
			fRec27[i29] = 0.0f;
			
		}
		for (int i30 = 0; (i30 < 2); i30 = (i30 + 1)) {
			fRec26[i30] = 0.0f;
			
		}
		for (int i31 = 0; (i31 < 2); i31 = (i31 + 1)) {
			fRec30[i31] = 0.0f;
			
		}
		for (int i32 = 0; (i32 < 2); i32 = (i32 + 1)) {
			fRec29[i32] = 0.0f;
			
		}
		for (int i33 = 0; (i33 < 2); i33 = (i33 + 1)) {
			fRec34[i33] = 0.0f;
			
		}
		for (int i34 = 0; (i34 < 2); i34 = (i34 + 1)) {
			fRec35[i34] = 0.0f;
			
		}
		for (int i35 = 0; (i35 < 2); i35 = (i35 + 1)) {
			fRec33[i35] = 0.0f;
			
		}
		for (int i36 = 0; (i36 < 2); i36 = (i36 + 1)) {
			fRec36[i36] = 0.0f;
			
		}
		for (int i37 = 0; (i37 < 2); i37 = (i37 + 1)) {
			fVec3[i37] = 0.0f;
			
		}
		for (int i38 = 0; (i38 < 2); i38 = (i38 + 1)) {
			fRec37[i38] = 0.0f;
			
		}
		for (int i39 = 0; (i39 < 2); i39 = (i39 + 1)) {
			fRec38[i39] = 0.0f;
			
		}
		for (int i40 = 0; (i40 < 2); i40 = (i40 + 1)) {
			fRec32[i40] = 0.0f;
			
		}
		for (int i41 = 0; (i41 < 2); i41 = (i41 + 1)) {
			fRec31[i41] = 0.0f;
			
		}
		for (int i42 = 0; (i42 < 2); i42 = (i42 + 1)) {
			fRec41[i42] = 0.0f;
			
		}
		for (int i43 = 0; (i43 < 2); i43 = (i43 + 1)) {
			fRec42[i43] = 0.0f;
			
		}
		for (int i44 = 0; (i44 < 2); i44 = (i44 + 1)) {
			fRec40[i44] = 0.0f;
			
		}
		for (int i45 = 0; (i45 < 2); i45 = (i45 + 1)) {
			fRec43[i45] = 0.0f;
			
		}
		for (int i46 = 0; (i46 < 2); i46 = (i46 + 1)) {
			fRec39[i46] = 0.0f;
			
		}
		for (int i47 = 0; (i47 < 2); i47 = (i47 + 1)) {
			fRec48[i47] = 0.0f;
			
		}
		for (int i48 = 0; (i48 < 2); i48 = (i48 + 1)) {
			fRec49[i48] = 0.0f;
			
		}
		for (int i49 = 0; (i49 < 2); i49 = (i49 + 1)) {
			fRec47[i49] = 0.0f;
			
		}
		for (int i50 = 0; (i50 < 2); i50 = (i50 + 1)) {
			fRec50[i50] = 0.0f;
			
		}
		for (int i51 = 0; (i51 < 2); i51 = (i51 + 1)) {
			fVec4[i51] = 0.0f;
			
		}
		for (int i52 = 0; (i52 < 2); i52 = (i52 + 1)) {
			fRec51[i52] = 0.0f;
			
		}
		for (int i53 = 0; (i53 < 2); i53 = (i53 + 1)) {
			fRec52[i53] = 0.0f;
			
		}
		for (int i54 = 0; (i54 < 2); i54 = (i54 + 1)) {
			fRec53[i54] = 0.0f;
			
		}
		for (int i55 = 0; (i55 < 2); i55 = (i55 + 1)) {
			fRec46[i55] = 0.0f;
			
		}
		for (int i56 = 0; (i56 < 2); i56 = (i56 + 1)) {
			fRec54[i56] = 0.0f;
			
		}
		for (int i57 = 0; (i57 < 2); i57 = (i57 + 1)) {
			fVec5[i57] = 0.0f;
			
		}
		for (int i58 = 0; (i58 < 2); i58 = (i58 + 1)) {
			fRec55[i58] = 0.0f;
			
		}
		for (int i59 = 0; (i59 < 2); i59 = (i59 + 1)) {
			fRec56[i59] = 0.0f;
			
		}
		for (int i60 = 0; (i60 < 2); i60 = (i60 + 1)) {
			fRec45[i60] = 0.0f;
			
		}
		for (int i61 = 0; (i61 < 2); i61 = (i61 + 1)) {
			fRec44[i61] = 0.0f;
			
		}
		for (int i62 = 0; (i62 < 2); i62 = (i62 + 1)) {
			fRec59[i62] = 0.0f;
			
		}
		for (int i63 = 0; (i63 < 2); i63 = (i63 + 1)) {
			fRec60[i63] = 0.0f;
			
		}
		for (int i64 = 0; (i64 < 2); i64 = (i64 + 1)) {
			fRec62[i64] = 0.0f;
			
		}
		for (int i65 = 0; (i65 < 2); i65 = (i65 + 1)) {
			fRec63[i65] = 0.0f;
			
		}
		for (int i66 = 0; (i66 < 2); i66 = (i66 + 1)) {
			fRec61[i66] = 0.0f;
			
		}
		for (int i67 = 0; (i67 < 2); i67 = (i67 + 1)) {
			fRec58[i67] = 0.0f;
			
		}
		for (int i68 = 0; (i68 < 2); i68 = (i68 + 1)) {
			fRec64[i68] = 0.0f;
			
		}
		for (int i69 = 0; (i69 < 2); i69 = (i69 + 1)) {
			fRec57[i69] = 0.0f;
			
		}
		for (int i70 = 0; (i70 < 2); i70 = (i70 + 1)) {
			fRec0[i70] = 0.0f;
			
		}
		for (int i71 = 0; (i71 < 2); i71 = (i71 + 1)) {
			fRec65[i71] = 0.0f;
			
		}
		for (int i72 = 0; (i72 < 2); i72 = (i72 + 1)) {
			fRec66[i72] = 0.0f;
			
		}
		for (int i73 = 0; (i73 < 2); i73 = (i73 + 1)) {
			fRec67[i73] = 0.0f;
			
		}
		for (int i74 = 0; (i74 < 2); i74 = (i74 + 1)) {
			fRec68[i74] = 0.0f;
			
		}
		for (int i75 = 0; (i75 < 2); i75 = (i75 + 1)) {
			fRec69[i75] = 0.0f;
			
		}
		for (int i76 = 0; (i76 < 2); i76 = (i76 + 1)) {
			fRec70[i76] = 0.0f;
			
		}
		for (int i77 = 0; (i77 < 2); i77 = (i77 + 1)) {
			fRec71[i77] = 0.0f;
			
		}
		for (int i78 = 0; (i78 < 2); i78 = (i78 + 1)) {
			fRec72[i78] = 0.0f;
			
		}
		for (int i79 = 0; (i79 < 2); i79 = (i79 + 1)) {
			fRec73[i79] = 0.0f;
			
		}
		for (int i80 = 0; (i80 < 2); i80 = (i80 + 1)) {
			fRec74[i80] = 0.0f;
			
		}
		for (int i81 = 0; (i81 < 2); i81 = (i81 + 1)) {
			fRec75[i81] = 0.0f;
			
		}
		for (int i82 = 0; (i82 < 2); i82 = (i82 + 1)) {
			fRec76[i82] = 0.0f;
			
		}
		for (int i83 = 0; (i83 < 2); i83 = (i83 + 1)) {
			fRec77[i83] = 0.0f;
			
		}
		for (int i84 = 0; (i84 < 2); i84 = (i84 + 1)) {
			fRec78[i84] = 0.0f;
			
		}
		for (int i85 = 0; (i85 < 2); i85 = (i85 + 1)) {
			fRec79[i85] = 0.0f;
			
		}
		for (int i86 = 0; (i86 < 2); i86 = (i86 + 1)) {
			fRec80[i86] = 0.0f;
			
		}
		for (int i87 = 0; (i87 < 2); i87 = (i87 + 1)) {
			fRec81[i87] = 0.0f;
			
		}
		for (int i88 = 0; (i88 < 2); i88 = (i88 + 1)) {
			fRec82[i88] = 0.0f;
			
		}
		for (int i89 = 0; (i89 < 2); i89 = (i89 + 1)) {
			fRec83[i89] = 0.0f;
			
		}
		for (int i90 = 0; (i90 < 2); i90 = (i90 + 1)) {
			fRec84[i90] = 0.0f;
			
		}
		for (int i91 = 0; (i91 < 2); i91 = (i91 + 1)) {
			fRec85[i91] = 0.0f;
			
		}
		for (int i92 = 0; (i92 < 2); i92 = (i92 + 1)) {
			fRec86[i92] = 0.0f;
			
		}
		for (int i93 = 0; (i93 < 2); i93 = (i93 + 1)) {
			fRec87[i93] = 0.0f;
			
		}
		for (int i94 = 0; (i94 < 2); i94 = (i94 + 1)) {
			fRec88[i94] = 0.0f;
			
		}
		for (int i95 = 0; (i95 < 2); i95 = (i95 + 1)) {
			fRec89[i95] = 0.0f;
			
		}
		for (int i96 = 0; (i96 < 2); i96 = (i96 + 1)) {
			fRec90[i96] = 0.0f;
			
		}
		for (int i97 = 0; (i97 < 2); i97 = (i97 + 1)) {
			fRec91[i97] = 0.0f;
			
		}
		for (int i98 = 0; (i98 < 2); i98 = (i98 + 1)) {
			fRec92[i98] = 0.0f;
			
		}
		for (int i99 = 0; (i99 < 2); i99 = (i99 + 1)) {
			fRec93[i99] = 0.0f;
			
		}
		for (int i100 = 0; (i100 < 2); i100 = (i100 + 1)) {
			fRec94[i100] = 0.0f;
			
		}
		for (int i101 = 0; (i101 < 2); i101 = (i101 + 1)) {
			fRec95[i101] = 0.0f;
			
		}
		for (int i102 = 0; (i102 < 2); i102 = (i102 + 1)) {
			fRec96[i102] = 0.0f;
			
		}
		for (int i103 = 0; (i103 < 2); i103 = (i103 + 1)) {
			fRec97[i103] = 0.0f;
			
		}
		for (int i104 = 0; (i104 < 2); i104 = (i104 + 1)) {
			fRec98[i104] = 0.0f;
			
		}
		for (int i105 = 0; (i105 < 2); i105 = (i105 + 1)) {
			fRec99[i105] = 0.0f;
			
		}
		for (int i106 = 0; (i106 < 2); i106 = (i106 + 1)) {
			fRec100[i106] = 0.0f;
			
		}
		for (int i107 = 0; (i107 < 2); i107 = (i107 + 1)) {
			fRec101[i107] = 0.0f;
			
		}
		for (int i108 = 0; (i108 < 2); i108 = (i108 + 1)) {
			fRec102[i108] = 0.0f;
			
		}
		for (int i109 = 0; (i109 < 2); i109 = (i109 + 1)) {
			fRec103[i109] = 0.0f;
			
		}
		for (int i110 = 0; (i110 < 2); i110 = (i110 + 1)) {
			fRec104[i110] = 0.0f;
			
		}
		for (int i111 = 0; (i111 < 2); i111 = (i111 + 1)) {
			fRec105[i111] = 0.0f;
			
		}
		for (int i112 = 0; (i112 < 2); i112 = (i112 + 1)) {
			fRec106[i112] = 0.0f;
			
		}
		for (int i113 = 0; (i113 < 2); i113 = (i113 + 1)) {
			fRec107[i113] = 0.0f;
			
		}
		for (int i114 = 0; (i114 < 2); i114 = (i114 + 1)) {
			fRec108[i114] = 0.0f;
			
		}
		for (int i115 = 0; (i115 < 2); i115 = (i115 + 1)) {
			fRec109[i115] = 0.0f;
			
		}
		for (int i116 = 0; (i116 < 2); i116 = (i116 + 1)) {
			fRec110[i116] = 0.0f;
			
		}
		for (int i117 = 0; (i117 < 2); i117 = (i117 + 1)) {
			fRec111[i117] = 0.0f;
			
		}
		for (int i118 = 0; (i118 < 2); i118 = (i118 + 1)) {
			fRec112[i118] = 0.0f;
			
		}
		for (int i119 = 0; (i119 < 2); i119 = (i119 + 1)) {
			fRec113[i119] = 0.0f;
			
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
		ui_interface->addCheckButton("0",&fCheckbox6);
		ui_interface->addCheckButton("1",&fCheckbox3);
		ui_interface->addCheckButton("2",&fCheckbox2);
		ui_interface->addCheckButton("3",&fCheckbox4);
		ui_interface->addCheckButton("4",&fCheckbox0);
		ui_interface->addCheckButton("5",&fCheckbox5);
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
		ui_interface->addVerticalBargraph("0x381aea0", &fVbargraph0, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("2");
		ui_interface->declare(&fVbargraph1, "osc", "/output2");
		ui_interface->declare(&fVbargraph1, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3826400", &fVbargraph1, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("3");
		ui_interface->declare(&fVbargraph2, "osc", "/output3");
		ui_interface->declare(&fVbargraph2, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3832630", &fVbargraph2, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("4");
		ui_interface->declare(&fVbargraph3, "osc", "/output4");
		ui_interface->declare(&fVbargraph3, "unit", "dB");
		ui_interface->addVerticalBargraph("0x383e940", &fVbargraph3, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("5");
		ui_interface->declare(&fVbargraph4, "osc", "/output5");
		ui_interface->declare(&fVbargraph4, "unit", "dB");
		ui_interface->addVerticalBargraph("0x384ad30", &fVbargraph4, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("6");
		ui_interface->declare(&fVbargraph5, "osc", "/output6");
		ui_interface->declare(&fVbargraph5, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3855200", &fVbargraph5, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("7");
		ui_interface->declare(&fVbargraph6, "osc", "/output7");
		ui_interface->declare(&fVbargraph6, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3860500", &fVbargraph6, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("8");
		ui_interface->declare(&fVbargraph7, "osc", "/output8");
		ui_interface->declare(&fVbargraph7, "unit", "dB");
		ui_interface->addVerticalBargraph("0x386af80", &fVbargraph7, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("9");
		ui_interface->declare(&fVbargraph8, "osc", "/output9");
		ui_interface->declare(&fVbargraph8, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3875ae0", &fVbargraph8, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("10");
		ui_interface->declare(&fVbargraph9, "osc", "/output10");
		ui_interface->declare(&fVbargraph9, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3880720", &fVbargraph9, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("11");
		ui_interface->declare(&fVbargraph10, "osc", "/output11");
		ui_interface->declare(&fVbargraph10, "unit", "dB");
		ui_interface->addVerticalBargraph("0x388d050", &fVbargraph10, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("12");
		ui_interface->declare(&fVbargraph11, "osc", "/output12");
		ui_interface->declare(&fVbargraph11, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3899a60", &fVbargraph11, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("13");
		ui_interface->declare(&fVbargraph12, "osc", "/output13");
		ui_interface->declare(&fVbargraph12, "unit", "dB");
		ui_interface->addVerticalBargraph("0x38a6550", &fVbargraph12, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("14");
		ui_interface->declare(&fVbargraph13, "osc", "/output14");
		ui_interface->declare(&fVbargraph13, "unit", "dB");
		ui_interface->addVerticalBargraph("0x38b3120", &fVbargraph13, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("15");
		ui_interface->declare(&fVbargraph14, "osc", "/output15");
		ui_interface->declare(&fVbargraph14, "unit", "dB");
		ui_interface->addVerticalBargraph("0x38bddd0", &fVbargraph14, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("16");
		ui_interface->declare(&fVbargraph15, "osc", "/output16");
		ui_interface->declare(&fVbargraph15, "unit", "dB");
		ui_interface->addVerticalBargraph("0x38c8b60", &fVbargraph15, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("17");
		ui_interface->declare(&fVbargraph16, "osc", "/output17");
		ui_interface->declare(&fVbargraph16, "unit", "dB");
		ui_interface->addVerticalBargraph("0x38d39d0", &fVbargraph16, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("18");
		ui_interface->declare(&fVbargraph17, "osc", "/output18");
		ui_interface->declare(&fVbargraph17, "unit", "dB");
		ui_interface->addVerticalBargraph("0x38de920", &fVbargraph17, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("19");
		ui_interface->declare(&fVbargraph18, "osc", "/output19");
		ui_interface->declare(&fVbargraph18, "unit", "dB");
		ui_interface->addVerticalBargraph("0x38ea6a0", &fVbargraph18, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("20");
		ui_interface->declare(&fVbargraph19, "osc", "/output20");
		ui_interface->declare(&fVbargraph19, "unit", "dB");
		ui_interface->addVerticalBargraph("0x38f5ba0", &fVbargraph19, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("21");
		ui_interface->declare(&fVbargraph20, "osc", "/output21");
		ui_interface->declare(&fVbargraph20, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3901180", &fVbargraph20, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("22");
		ui_interface->declare(&fVbargraph21, "osc", "/output22");
		ui_interface->declare(&fVbargraph21, "unit", "dB");
		ui_interface->addVerticalBargraph("0x390c840", &fVbargraph21, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("23");
		ui_interface->declare(&fVbargraph22, "osc", "/output23");
		ui_interface->declare(&fVbargraph22, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3917bf0", &fVbargraph22, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("24");
		ui_interface->declare(&fVbargraph23, "osc", "/output24");
		ui_interface->declare(&fVbargraph23, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3923080", &fVbargraph23, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("25");
		ui_interface->declare(&fVbargraph24, "osc", "/output25");
		ui_interface->declare(&fVbargraph24, "unit", "dB");
		ui_interface->addVerticalBargraph("0x392e5f0", &fVbargraph24, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->declare(0, "~", "");
		ui_interface->openHorizontalBox("26-50");
		ui_interface->openVerticalBox("26");
		ui_interface->declare(&fVbargraph25, "osc", "/output26");
		ui_interface->declare(&fVbargraph25, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3939c40", &fVbargraph25, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("27");
		ui_interface->declare(&fVbargraph26, "osc", "/output27");
		ui_interface->declare(&fVbargraph26, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3945670", &fVbargraph26, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("28");
		ui_interface->declare(&fVbargraph27, "osc", "/output28");
		ui_interface->declare(&fVbargraph27, "unit", "dB");
		ui_interface->addVerticalBargraph("0x394fd70", &fVbargraph27, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("29");
		ui_interface->declare(&fVbargraph28, "osc", "/output29");
		ui_interface->declare(&fVbargraph28, "unit", "dB");
		ui_interface->addVerticalBargraph("0x395a550", &fVbargraph28, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("30");
		ui_interface->declare(&fVbargraph29, "osc", "/output30");
		ui_interface->declare(&fVbargraph29, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3964e10", &fVbargraph29, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("31");
		ui_interface->declare(&fVbargraph30, "osc", "/output31");
		ui_interface->declare(&fVbargraph30, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3970910", &fVbargraph30, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("32");
		ui_interface->declare(&fVbargraph31, "osc", "/output32");
		ui_interface->declare(&fVbargraph31, "unit", "dB");
		ui_interface->addVerticalBargraph("0x397bd60", &fVbargraph31, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("33");
		ui_interface->declare(&fVbargraph32, "osc", "/output33");
		ui_interface->declare(&fVbargraph32, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3987290", &fVbargraph32, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("34");
		ui_interface->declare(&fVbargraph33, "osc", "/output34");
		ui_interface->declare(&fVbargraph33, "unit", "dB");
		ui_interface->addVerticalBargraph("0x39928a0", &fVbargraph33, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("35");
		ui_interface->declare(&fVbargraph34, "osc", "/output35");
		ui_interface->declare(&fVbargraph34, "unit", "dB");
		ui_interface->addVerticalBargraph("0x399df90", &fVbargraph34, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("36");
		ui_interface->declare(&fVbargraph35, "osc", "/output36");
		ui_interface->declare(&fVbargraph35, "unit", "dB");
		ui_interface->addVerticalBargraph("0x39a9760", &fVbargraph35, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("37");
		ui_interface->declare(&fVbargraph36, "osc", "/output37");
		ui_interface->declare(&fVbargraph36, "unit", "dB");
		ui_interface->addVerticalBargraph("0x39b5010", &fVbargraph36, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("38");
		ui_interface->declare(&fVbargraph37, "osc", "/output38");
		ui_interface->declare(&fVbargraph37, "unit", "dB");
		ui_interface->addVerticalBargraph("0x39c09a0", &fVbargraph37, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("39");
		ui_interface->declare(&fVbargraph38, "osc", "/output39");
		ui_interface->declare(&fVbargraph38, "unit", "dB");
		ui_interface->addVerticalBargraph("0x39cb650", &fVbargraph38, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("40");
		ui_interface->declare(&fVbargraph39, "osc", "/output40");
		ui_interface->declare(&fVbargraph39, "unit", "dB");
		ui_interface->addVerticalBargraph("0x39d63e0", &fVbargraph39, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("41");
		ui_interface->declare(&fVbargraph40, "osc", "/output41");
		ui_interface->declare(&fVbargraph40, "unit", "dB");
		ui_interface->addVerticalBargraph("0x39e1250", &fVbargraph40, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("42");
		ui_interface->declare(&fVbargraph41, "osc", "/output42");
		ui_interface->declare(&fVbargraph41, "unit", "dB");
		ui_interface->addVerticalBargraph("0x39ec1a0", &fVbargraph41, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("43");
		ui_interface->declare(&fVbargraph42, "osc", "/output43");
		ui_interface->declare(&fVbargraph42, "unit", "dB");
		ui_interface->addVerticalBargraph("0x39f71d0", &fVbargraph42, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("44");
		ui_interface->declare(&fVbargraph43, "osc", "/output44");
		ui_interface->declare(&fVbargraph43, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3a022e0", &fVbargraph43, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("45");
		ui_interface->declare(&fVbargraph44, "osc", "/output45");
		ui_interface->declare(&fVbargraph44, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3a0d4d0", &fVbargraph44, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("46");
		ui_interface->declare(&fVbargraph45, "osc", "/output46");
		ui_interface->declare(&fVbargraph45, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3a187a0", &fVbargraph45, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("47");
		ui_interface->declare(&fVbargraph46, "osc", "/output47");
		ui_interface->declare(&fVbargraph46, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3a23b50", &fVbargraph46, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("48");
		ui_interface->declare(&fVbargraph47, "osc", "/output48");
		ui_interface->declare(&fVbargraph47, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3a2efe0", &fVbargraph47, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("49");
		ui_interface->declare(&fVbargraph48, "osc", "/output49");
		ui_interface->declare(&fVbargraph48, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3a3a550", &fVbargraph48, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("50");
		ui_interface->declare(&fVbargraph49, "osc", "/output50");
		ui_interface->declare(&fVbargraph49, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3a45ba0", &fVbargraph49, -70.0f, 6.0f);
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
		float fSlow10 = (1.125f * (((fSlow8 * ((35.0f * fSlow8) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow11 = float(fEntry0);
		float fSlow12 = (fConst3 / fSlow11);
		float fSlow13 = faustpower2_f(fSlow12);
		float fSlow14 = (1.0f / (((fConst2 / fSlow11) + (9.140131f * fSlow13)) + 1.0f));
		float fSlow15 = float(fHslider2);
		float fSlow16 = (fConst3 / fSlow15);
		float fSlow17 = faustpower2_f(fSlow16);
		float fSlow18 = (((fConst2 / fSlow15) + (9.140131f * fSlow17)) + 1.0f);
		float fSlow19 = (1.0f / (((fConst4 / fSlow11) + (11.4878006f * fSlow13)) + 1.0f));
		float fSlow20 = (((fConst4 / fSlow15) + (11.4878006f * fSlow17)) + 1.0f);
		float fSlow21 = float(fCheckbox1);
		float fSlow22 = (((fSlow20 * fSlow21) * fSlow11) / fSlow15);
		float fSlow23 = (0.00100000005f * powf(10.0f, (0.0500000007f * float(fHslider3))));
		float fSlow24 = (45.9512024f * fSlow13);
		float fSlow25 = (fSlow24 + (fConst5 / fSlow11));
		float fSlow26 = (1.0f / fSlow20);
		float fSlow27 = (45.9512024f * fSlow17);
		float fSlow28 = ((fConst5 / fSlow15) + fSlow27);
		float fSlow29 = (36.560524f * fSlow13);
		float fSlow30 = (fSlow29 + (fConst6 / fSlow11));
		float fSlow31 = (1.0f / fSlow18);
		float fSlow32 = (36.560524f * fSlow17);
		float fSlow33 = ((fConst6 / fSlow15) + fSlow32);
		float fSlow34 = ((1.0f - fSlow21) * fSlow11);
		float fSlow35 = (1.0f - float(fCheckbox2));
		float fSlow36 = (2.5f * (((3.0f * fSlow8) + -1.0f) * fSlow35));
		float fSlow37 = (1.0f / (((fConst7 / fSlow11) + (3.0f * fSlow13)) + 1.0f));
		float fSlow38 = (((fConst7 / fSlow15) + (3.0f * fSlow17)) + 1.0f);
		float fSlow39 = (((fSlow38 * fSlow21) * fSlow11) / fSlow15);
		float fSlow40 = (12.0f * fSlow13);
		float fSlow41 = (fSlow40 + (fConst8 / fSlow11));
		float fSlow42 = (1.0f / fSlow38);
		float fSlow43 = (12.0f * fSlow17);
		float fSlow44 = ((fConst8 / fSlow15) + fSlow43);
		float fSlow45 = (1.0f - float(fCheckbox3));
		float fSlow46 = (3.0f * fSlow45);
		float fSlow47 = (fSlow16 + 1.0f);
		float fSlow48 = (fConst9 / (fSlow47 * fSlow15));
		float fSlow49 = (1.0f / (fSlow12 + 1.0f));
		float fSlow50 = (((fSlow47 * fSlow21) * fSlow11) / fSlow15);
		float fSlow51 = (fConst9 / fSlow11);
		float fSlow52 = (1.0f - float(fCheckbox4));
		float fSlow53 = (3.5f * (((5.0f * fSlow8) + -3.0f) * fSlow52));
		float fSlow54 = ((fConst11 / fSlow15) + 1.0f);
		float fSlow55 = (fConst10 / (fSlow54 * fSlow15));
		float fSlow56 = (1.0f / ((fConst11 / fSlow11) + 1.0f));
		float fSlow57 = (1.0f / (((fConst12 / fSlow11) + (6.4594326f * fSlow13)) + 1.0f));
		float fSlow58 = (((fConst12 / fSlow15) + (6.4594326f * fSlow17)) + 1.0f);
		float fSlow59 = (((fSlow58 * fSlow21) * fSlow11) / fSlow15);
		float fSlow60 = (25.8377304f * fSlow13);
		float fSlow61 = (fSlow60 + (fConst13 / fSlow11));
		float fSlow62 = (1.0f / fSlow58);
		float fSlow63 = (25.8377304f * fSlow17);
		float fSlow64 = ((fConst13 / fSlow15) + fSlow63);
		float fSlow65 = (fConst10 / fSlow11);
		float fSlow66 = (1.0f - float(fCheckbox5));
		float fSlow67 = (1.375f * (((fSlow8 * ((63.0f * fSlow8) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow68 = ((fConst15 / fSlow15) + 1.0f);
		float fSlow69 = (fConst14 / (fSlow68 * fSlow15));
		float fSlow70 = (1.0f / ((fConst15 / fSlow11) + 1.0f));
		float fSlow71 = (1.0f / (((fConst16 / fSlow11) + (14.272481f * fSlow13)) + 1.0f));
		float fSlow72 = (((fConst16 / fSlow15) + (14.272481f * fSlow17)) + 1.0f);
		float fSlow73 = (1.0f / (((fConst17 / fSlow11) + (18.1563148f * fSlow13)) + 1.0f));
		float fSlow74 = (((fConst17 / fSlow15) + (18.1563148f * fSlow17)) + 1.0f);
		float fSlow75 = (((fSlow74 * fSlow21) * fSlow11) / fSlow15);
		float fSlow76 = (72.6252594f * fSlow13);
		float fSlow77 = (fSlow76 + (fConst18 / fSlow11));
		float fSlow78 = (1.0f / fSlow74);
		float fSlow79 = (72.6252594f * fSlow17);
		float fSlow80 = (fSlow79 + (fConst18 / fSlow15));
		float fSlow81 = (57.0899239f * fSlow13);
		float fSlow82 = (fSlow81 + (fConst19 / fSlow11));
		float fSlow83 = (1.0f / fSlow72);
		float fSlow84 = (57.0899239f * fSlow17);
		float fSlow85 = ((fConst19 / fSlow15) + fSlow84);
		float fSlow86 = (fConst14 / fSlow11);
		float fSlow87 = ((((((1.0f / fSlow15) + -1.0f) * fSlow21) + 1.0f) * fSlow11) * (1.0f - float(fCheckbox6)));
		float fSlow88 = faustpower2_f(fSlow4);
		float fSlow89 = (1.125f * (((fSlow88 * ((35.0f * fSlow88) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow90 = (2.5f * (((3.0f * fSlow88) + -1.0f) * fSlow35));
		float fSlow91 = (3.0f * ((fSlow1 * fSlow45) * fSlow3));
		float fSlow92 = faustpower3_f(fSlow4);
		float fSlow93 = (3.5f * (((5.0f * fSlow92) - (3.0f * fSlow4)) * fSlow52));
		float fSlow94 = (1.375f * (((15.0f * fSlow4) + (fSlow92 * ((63.0f * fSlow88) + -70.0f))) * fSlow66));
		float fSlow95 = cosf((fSlow0 + -1.57079637f));
		float fSlow96 = (fSlow95 * fSlow3);
		float fSlow97 = faustpower2_f(fSlow96);
		float fSlow98 = (1.125f * (((fSlow97 * ((35.0f * fSlow97) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow99 = (2.5f * (((3.0f * fSlow97) + -1.0f) * fSlow35));
		float fSlow100 = (3.0f * ((fSlow95 * fSlow45) * fSlow3));
		float fSlow101 = faustpower3_f(fSlow96);
		float fSlow102 = (3.5f * (((5.0f * fSlow101) - (3.0f * fSlow96)) * fSlow52));
		float fSlow103 = (1.375f * (((15.0f * fSlow96) + (fSlow101 * ((63.0f * fSlow97) + -70.0f))) * fSlow66));
		float fSlow104 = cosf((fSlow0 + -3.14159274f));
		float fSlow105 = (fSlow104 * fSlow3);
		float fSlow106 = faustpower2_f(fSlow105);
		float fSlow107 = (1.125f * (((fSlow106 * ((35.0f * fSlow106) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow108 = (2.5f * (((3.0f * fSlow106) + -1.0f) * fSlow35));
		float fSlow109 = (3.0f * ((fSlow104 * fSlow45) * fSlow3));
		float fSlow110 = faustpower3_f(fSlow105);
		float fSlow111 = (3.5f * (((5.0f * fSlow110) - (3.0f * fSlow105)) * fSlow52));
		float fSlow112 = (1.375f * (((15.0f * fSlow105) + (fSlow110 * ((63.0f * fSlow106) + -70.0f))) * fSlow66));
		float fSlow113 = cosf((fSlow0 + -4.71238899f));
		float fSlow114 = (fSlow113 * fSlow3);
		float fSlow115 = faustpower2_f(fSlow114);
		float fSlow116 = (1.125f * (((fSlow115 * ((35.0f * fSlow115) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow117 = (2.5f * (((3.0f * fSlow115) + -1.0f) * fSlow35));
		float fSlow118 = (3.0f * ((fSlow113 * fSlow45) * fSlow3));
		float fSlow119 = faustpower3_f(fSlow114);
		float fSlow120 = (3.5f * (((5.0f * fSlow119) - (3.0f * fSlow114)) * fSlow52));
		float fSlow121 = (1.375f * (((15.0f * fSlow114) + (fSlow119 * ((63.0f * fSlow115) + -70.0f))) * fSlow66));
		float fSlow122 = (fSlow5 - fSlow6);
		float fSlow123 = faustpower2_f(fSlow122);
		float fSlow124 = (1.125f * (((fSlow123 * ((35.0f * fSlow123) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow125 = (2.5f * (((3.0f * fSlow123) + -1.0f) * fSlow35));
		float fSlow126 = (3.5f * (((5.0f * fSlow123) + -3.0f) * fSlow52));
		float fSlow127 = (1.375f * (((fSlow123 * ((63.0f * fSlow123) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow128 = (0.707106769f * fSlow4);
		float fSlow129 = (0.707106769f * fSlow6);
		float fSlow130 = (fSlow128 + fSlow129);
		float fSlow131 = faustpower2_f(fSlow130);
		float fSlow132 = (1.125f * (((fSlow131 * ((35.0f * fSlow131) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow133 = (2.5f * (((3.0f * fSlow131) + -1.0f) * fSlow35));
		float fSlow134 = (3.5f * (((5.0f * fSlow131) + -3.0f) * fSlow52));
		float fSlow135 = (1.375f * (((fSlow131 * ((63.0f * fSlow131) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow136 = (0.707106769f * fSlow96);
		float fSlow137 = (fSlow129 + fSlow136);
		float fSlow138 = faustpower2_f(fSlow137);
		float fSlow139 = (1.125f * (((fSlow138 * ((35.0f * fSlow138) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow140 = (2.5f * (((3.0f * fSlow138) + -1.0f) * fSlow35));
		float fSlow141 = (3.5f * (((5.0f * fSlow138) + -3.0f) * fSlow52));
		float fSlow142 = (1.375f * (((fSlow138 * ((63.0f * fSlow138) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow143 = (0.707106769f * fSlow105);
		float fSlow144 = (fSlow129 + fSlow143);
		float fSlow145 = faustpower2_f(fSlow144);
		float fSlow146 = (1.125f * (((fSlow145 * ((35.0f * fSlow145) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow147 = (2.5f * (((3.0f * fSlow145) + -1.0f) * fSlow35));
		float fSlow148 = (3.5f * (((5.0f * fSlow145) + -3.0f) * fSlow52));
		float fSlow149 = (1.375f * (((fSlow145 * ((63.0f * fSlow145) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow150 = (0.707106769f * fSlow114);
		float fSlow151 = (fSlow129 + fSlow150);
		float fSlow152 = faustpower2_f(fSlow151);
		float fSlow153 = (1.125f * (((fSlow152 * ((35.0f * fSlow152) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow154 = (2.5f * (((3.0f * fSlow152) + -1.0f) * fSlow35));
		float fSlow155 = (3.5f * (((5.0f * fSlow152) + -3.0f) * fSlow52));
		float fSlow156 = (1.375f * (((fSlow152 * ((63.0f * fSlow152) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow157 = cosf((fSlow0 + -0.785398185f));
		float fSlow158 = (fSlow157 * fSlow3);
		float fSlow159 = faustpower2_f(fSlow158);
		float fSlow160 = (1.125f * (((fSlow159 * ((35.0f * fSlow159) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow161 = (2.5f * (((3.0f * fSlow159) + -1.0f) * fSlow35));
		float fSlow162 = (3.0f * ((fSlow157 * fSlow45) * fSlow3));
		float fSlow163 = faustpower3_f(fSlow158);
		float fSlow164 = (3.5f * (((5.0f * fSlow163) - (3.0f * fSlow158)) * fSlow52));
		float fSlow165 = (1.375f * (((15.0f * fSlow158) + (fSlow163 * ((63.0f * fSlow159) + -70.0f))) * fSlow66));
		float fSlow166 = cosf((fSlow0 + -2.3561945f));
		float fSlow167 = (fSlow166 * fSlow3);
		float fSlow168 = faustpower2_f(fSlow167);
		float fSlow169 = (1.125f * (((fSlow168 * ((35.0f * fSlow168) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow170 = (2.5f * (((3.0f * fSlow168) + -1.0f) * fSlow35));
		float fSlow171 = (3.0f * ((fSlow166 * fSlow45) * fSlow3));
		float fSlow172 = faustpower3_f(fSlow167);
		float fSlow173 = (3.5f * (((5.0f * fSlow172) - (3.0f * fSlow167)) * fSlow52));
		float fSlow174 = (1.375f * (((15.0f * fSlow167) + (fSlow172 * ((63.0f * fSlow168) + -70.0f))) * fSlow66));
		float fSlow175 = cosf((fSlow0 + -3.92699075f));
		float fSlow176 = (fSlow175 * fSlow3);
		float fSlow177 = faustpower2_f(fSlow176);
		float fSlow178 = (1.125f * (((fSlow177 * ((35.0f * fSlow177) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow179 = (2.5f * (((3.0f * fSlow177) + -1.0f) * fSlow35));
		float fSlow180 = (3.0f * ((fSlow175 * fSlow45) * fSlow3));
		float fSlow181 = faustpower3_f(fSlow176);
		float fSlow182 = (3.5f * (((5.0f * fSlow181) - (3.0f * fSlow176)) * fSlow52));
		float fSlow183 = (1.375f * (((15.0f * fSlow176) + (fSlow181 * ((63.0f * fSlow177) + -70.0f))) * fSlow66));
		float fSlow184 = cosf((fSlow0 + -5.497787f));
		float fSlow185 = (fSlow184 * fSlow3);
		float fSlow186 = faustpower2_f(fSlow185);
		float fSlow187 = (1.125f * (((fSlow186 * ((35.0f * fSlow186) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow188 = (2.5f * (((3.0f * fSlow186) + -1.0f) * fSlow35));
		float fSlow189 = (3.0f * ((fSlow184 * fSlow45) * fSlow3));
		float fSlow190 = faustpower3_f(fSlow185);
		float fSlow191 = (3.5f * (((5.0f * fSlow190) - (3.0f * fSlow185)) * fSlow52));
		float fSlow192 = (1.375f * (((15.0f * fSlow185) + (fSlow190 * ((63.0f * fSlow186) + -70.0f))) * fSlow66));
		float fSlow193 = (fSlow128 - fSlow129);
		float fSlow194 = faustpower2_f(fSlow193);
		float fSlow195 = (1.125f * (((fSlow194 * ((35.0f * fSlow194) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow196 = (2.5f * (((3.0f * fSlow194) + -1.0f) * fSlow35));
		float fSlow197 = (3.5f * (((5.0f * fSlow194) + -3.0f) * fSlow52));
		float fSlow198 = (1.375f * (((fSlow194 * ((63.0f * fSlow194) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow199 = (fSlow136 - fSlow129);
		float fSlow200 = faustpower2_f(fSlow199);
		float fSlow201 = (1.125f * (((fSlow200 * ((35.0f * fSlow200) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow202 = (2.5f * (((3.0f * fSlow200) + -1.0f) * fSlow35));
		float fSlow203 = (3.5f * (((5.0f * fSlow200) + -3.0f) * fSlow52));
		float fSlow204 = (1.375f * (((fSlow200 * ((63.0f * fSlow200) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow205 = (fSlow143 - fSlow129);
		float fSlow206 = faustpower2_f(fSlow205);
		float fSlow207 = (1.125f * (((fSlow206 * ((35.0f * fSlow206) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow208 = (2.5f * (((3.0f * fSlow206) + -1.0f) * fSlow35));
		float fSlow209 = (3.5f * (((5.0f * fSlow206) + -3.0f) * fSlow52));
		float fSlow210 = (1.375f * (((fSlow206 * ((63.0f * fSlow206) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow211 = (fSlow150 - fSlow129);
		float fSlow212 = faustpower2_f(fSlow211);
		float fSlow213 = (1.125f * (((fSlow212 * ((35.0f * fSlow212) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow214 = (2.5f * (((3.0f * fSlow212) + -1.0f) * fSlow35));
		float fSlow215 = (3.5f * (((5.0f * fSlow212) + -3.0f) * fSlow52));
		float fSlow216 = (1.375f * (((fSlow212 * ((63.0f * fSlow212) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow217 = (0.816496611f * fSlow158);
		float fSlow218 = (0.577350259f * fSlow6);
		float fSlow219 = (fSlow217 + fSlow218);
		float fSlow220 = faustpower2_f(fSlow219);
		float fSlow221 = (1.125f * (((fSlow220 * ((35.0f * fSlow220) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow222 = (2.5f * (((3.0f * fSlow220) + -1.0f) * fSlow35));
		float fSlow223 = (3.5f * (((5.0f * fSlow220) + -3.0f) * fSlow52));
		float fSlow224 = (1.375f * (((fSlow220 * ((63.0f * fSlow220) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow225 = (0.816496611f * fSlow167);
		float fSlow226 = (fSlow218 + fSlow225);
		float fSlow227 = faustpower2_f(fSlow226);
		float fSlow228 = (1.125f * (((fSlow227 * ((35.0f * fSlow227) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow229 = (2.5f * (((3.0f * fSlow227) + -1.0f) * fSlow35));
		float fSlow230 = (3.5f * (((5.0f * fSlow227) + -3.0f) * fSlow52));
		float fSlow231 = (1.375f * (((fSlow227 * ((63.0f * fSlow227) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow232 = (0.816496611f * fSlow176);
		float fSlow233 = (fSlow218 + fSlow232);
		float fSlow234 = faustpower2_f(fSlow233);
		float fSlow235 = (1.125f * (((fSlow234 * ((35.0f * fSlow234) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow236 = (2.5f * (((3.0f * fSlow234) + -1.0f) * fSlow35));
		float fSlow237 = (3.5f * (((5.0f * fSlow234) + -3.0f) * fSlow52));
		float fSlow238 = (1.375f * (((fSlow234 * ((63.0f * fSlow234) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow239 = (0.816496611f * fSlow185);
		float fSlow240 = (fSlow218 + fSlow239);
		float fSlow241 = faustpower2_f(fSlow240);
		float fSlow242 = (1.125f * (((fSlow241 * ((35.0f * fSlow241) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow243 = (2.5f * (((3.0f * fSlow241) + -1.0f) * fSlow35));
		float fSlow244 = (3.5f * (((5.0f * fSlow241) + -3.0f) * fSlow52));
		float fSlow245 = (1.375f * (((fSlow241 * ((63.0f * fSlow241) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow246 = (fSlow217 - fSlow218);
		float fSlow247 = faustpower2_f(fSlow246);
		float fSlow248 = (1.125f * (((fSlow247 * ((35.0f * fSlow247) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow249 = (2.5f * (((3.0f * fSlow247) + -1.0f) * fSlow35));
		float fSlow250 = (3.5f * (((5.0f * fSlow247) + -3.0f) * fSlow52));
		float fSlow251 = (1.375f * (((fSlow247 * ((63.0f * fSlow247) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow252 = (fSlow225 - fSlow218);
		float fSlow253 = faustpower2_f(fSlow252);
		float fSlow254 = (1.125f * (((fSlow253 * ((35.0f * fSlow253) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow255 = (2.5f * (((3.0f * fSlow253) + -1.0f) * fSlow35));
		float fSlow256 = (3.5f * (((5.0f * fSlow253) + -3.0f) * fSlow52));
		float fSlow257 = (1.375f * (((fSlow253 * ((63.0f * fSlow253) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow258 = (fSlow232 - fSlow218);
		float fSlow259 = faustpower2_f(fSlow258);
		float fSlow260 = (1.125f * (((fSlow259 * ((35.0f * fSlow259) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow261 = (2.5f * (((3.0f * fSlow259) + -1.0f) * fSlow35));
		float fSlow262 = (3.5f * (((5.0f * fSlow259) + -3.0f) * fSlow52));
		float fSlow263 = (1.375f * (((fSlow259 * ((63.0f * fSlow259) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow264 = (fSlow239 - fSlow218);
		float fSlow265 = faustpower2_f(fSlow264);
		float fSlow266 = (1.125f * (((fSlow265 * ((35.0f * fSlow265) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow267 = (2.5f * (((3.0f * fSlow265) + -1.0f) * fSlow35));
		float fSlow268 = (3.5f * (((5.0f * fSlow265) + -3.0f) * fSlow52));
		float fSlow269 = (1.375f * (((fSlow265 * ((63.0f * fSlow265) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow270 = (0.426401436f * (cosf((fSlow0 + -0.785398185f)) * fSlow3));
		float fSlow271 = (0.904534042f * fSlow6);
		float fSlow272 = (fSlow270 + fSlow271);
		float fSlow273 = faustpower2_f(fSlow272);
		float fSlow274 = (1.125f * (((fSlow273 * ((35.0f * fSlow273) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow275 = (2.5f * (((3.0f * fSlow273) + -1.0f) * fSlow35));
		float fSlow276 = (3.5f * (((5.0f * fSlow273) + -3.0f) * fSlow52));
		float fSlow277 = (1.375f * (((fSlow273 * ((63.0f * fSlow273) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow278 = (0.426401436f * fSlow167);
		float fSlow279 = (fSlow271 + fSlow278);
		float fSlow280 = faustpower2_f(fSlow279);
		float fSlow281 = (1.125f * (((fSlow280 * ((35.0f * fSlow280) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow282 = (2.5f * (((3.0f * fSlow280) + -1.0f) * fSlow35));
		float fSlow283 = (3.5f * (((5.0f * fSlow280) + -3.0f) * fSlow52));
		float fSlow284 = (1.375f * (((fSlow280 * ((63.0f * fSlow280) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow285 = (0.426401436f * fSlow176);
		float fSlow286 = (fSlow271 + fSlow285);
		float fSlow287 = faustpower2_f(fSlow286);
		float fSlow288 = (1.125f * (((fSlow287 * ((35.0f * fSlow287) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow289 = (2.5f * (((3.0f * fSlow287) + -1.0f) * fSlow35));
		float fSlow290 = (3.5f * (((5.0f * fSlow287) + -3.0f) * fSlow52));
		float fSlow291 = (1.375f * (((fSlow287 * ((63.0f * fSlow287) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow292 = (0.426401436f * fSlow185);
		float fSlow293 = (fSlow271 + fSlow292);
		float fSlow294 = faustpower2_f(fSlow293);
		float fSlow295 = (1.125f * (((fSlow294 * ((35.0f * fSlow294) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow296 = (2.5f * (((3.0f * fSlow294) + -1.0f) * fSlow35));
		float fSlow297 = (3.5f * (((5.0f * fSlow294) + -3.0f) * fSlow52));
		float fSlow298 = (1.375f * (((fSlow294 * ((63.0f * fSlow294) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow299 = (0.953462601f * (cosf((fSlow0 + -0.321750551f)) * fSlow3));
		float fSlow300 = (0.301511347f * fSlow6);
		float fSlow301 = (fSlow299 + fSlow300);
		float fSlow302 = faustpower2_f(fSlow301);
		float fSlow303 = (1.125f * (((fSlow302 * ((35.0f * fSlow302) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow304 = (2.5f * (((3.0f * fSlow302) + -1.0f) * fSlow35));
		float fSlow305 = (3.5f * (((5.0f * fSlow302) + -3.0f) * fSlow52));
		float fSlow306 = (1.375f * (((fSlow302 * ((63.0f * fSlow302) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow307 = (0.953462601f * (cosf((fSlow0 + -1.24904573f)) * fSlow3));
		float fSlow308 = (fSlow300 + fSlow307);
		float fSlow309 = faustpower2_f(fSlow308);
		float fSlow310 = (1.125f * (((fSlow309 * ((35.0f * fSlow309) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow311 = (2.5f * (((3.0f * fSlow309) + -1.0f) * fSlow35));
		float fSlow312 = (3.5f * (((5.0f * fSlow309) + -3.0f) * fSlow52));
		float fSlow313 = (1.375f * (((fSlow309 * ((63.0f * fSlow309) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow314 = (0.953462601f * (cosf((fSlow0 + -1.89254689f)) * fSlow3));
		float fSlow315 = (fSlow300 + fSlow314);
		float fSlow316 = faustpower2_f(fSlow315);
		float fSlow317 = (1.125f * (((fSlow316 * ((35.0f * fSlow316) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow318 = (2.5f * (((3.0f * fSlow316) + -1.0f) * fSlow35));
		float fSlow319 = (3.5f * (((5.0f * fSlow316) + -3.0f) * fSlow52));
		float fSlow320 = (1.375f * (((fSlow316 * ((63.0f * fSlow316) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow321 = (0.953462601f * (cosf((fSlow0 + -2.8198421f)) * fSlow3));
		float fSlow322 = (fSlow300 + fSlow321);
		float fSlow323 = faustpower2_f(fSlow322);
		float fSlow324 = (1.125f * (((fSlow323 * ((35.0f * fSlow323) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow325 = (2.5f * (((3.0f * fSlow323) + -1.0f) * fSlow35));
		float fSlow326 = (3.5f * (((5.0f * fSlow323) + -3.0f) * fSlow52));
		float fSlow327 = (1.375f * (((fSlow323 * ((63.0f * fSlow323) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow328 = (0.953462601f * (cosf((fSlow0 + -3.46334314f)) * fSlow3));
		float fSlow329 = (fSlow300 + fSlow328);
		float fSlow330 = faustpower2_f(fSlow329);
		float fSlow331 = (1.125f * (((fSlow330 * ((35.0f * fSlow330) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow332 = (2.5f * (((3.0f * fSlow330) + -1.0f) * fSlow35));
		float fSlow333 = (3.5f * (((5.0f * fSlow330) + -3.0f) * fSlow52));
		float fSlow334 = (1.375f * (((fSlow330 * ((63.0f * fSlow330) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow335 = (0.953462601f * (cosf((fSlow0 + -4.39063835f)) * fSlow3));
		float fSlow336 = (fSlow300 + fSlow335);
		float fSlow337 = faustpower2_f(fSlow336);
		float fSlow338 = (1.125f * (((fSlow337 * ((35.0f * fSlow337) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow339 = (2.5f * (((3.0f * fSlow337) + -1.0f) * fSlow35));
		float fSlow340 = (3.5f * (((5.0f * fSlow337) + -3.0f) * fSlow52));
		float fSlow341 = (1.375f * (((fSlow337 * ((63.0f * fSlow337) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow342 = (0.953462601f * (cosf((fSlow0 + -5.03413963f)) * fSlow3));
		float fSlow343 = (fSlow300 + fSlow342);
		float fSlow344 = faustpower2_f(fSlow343);
		float fSlow345 = (1.125f * (((fSlow344 * ((35.0f * fSlow344) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow346 = (2.5f * (((3.0f * fSlow344) + -1.0f) * fSlow35));
		float fSlow347 = (3.5f * (((5.0f * fSlow344) + -3.0f) * fSlow52));
		float fSlow348 = (1.375f * (((fSlow344 * ((63.0f * fSlow344) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow349 = (0.953462601f * (cosf((fSlow0 + -5.96143484f)) * fSlow3));
		float fSlow350 = (fSlow300 + fSlow349);
		float fSlow351 = faustpower2_f(fSlow350);
		float fSlow352 = (1.125f * (((fSlow351 * ((35.0f * fSlow351) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow353 = (2.5f * (((3.0f * fSlow351) + -1.0f) * fSlow35));
		float fSlow354 = (3.5f * (((5.0f * fSlow351) + -3.0f) * fSlow52));
		float fSlow355 = (1.375f * (((fSlow351 * ((63.0f * fSlow351) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow356 = (fSlow299 - fSlow300);
		float fSlow357 = faustpower2_f(fSlow356);
		float fSlow358 = (1.125f * (((fSlow357 * ((35.0f * fSlow357) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow359 = (2.5f * (((3.0f * fSlow357) + -1.0f) * fSlow35));
		float fSlow360 = (3.5f * (((5.0f * fSlow357) + -3.0f) * fSlow52));
		float fSlow361 = (1.375f * (((fSlow357 * ((63.0f * fSlow357) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow362 = (fSlow307 - fSlow300);
		float fSlow363 = faustpower2_f(fSlow362);
		float fSlow364 = (1.125f * (((fSlow363 * ((35.0f * fSlow363) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow365 = (2.5f * (((3.0f * fSlow363) + -1.0f) * fSlow35));
		float fSlow366 = (3.5f * (((5.0f * fSlow363) + -3.0f) * fSlow52));
		float fSlow367 = (1.375f * (((fSlow363 * ((63.0f * fSlow363) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow368 = (fSlow314 - fSlow300);
		float fSlow369 = faustpower2_f(fSlow368);
		float fSlow370 = (1.125f * (((fSlow369 * ((35.0f * fSlow369) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow371 = (2.5f * (((3.0f * fSlow369) + -1.0f) * fSlow35));
		float fSlow372 = (3.5f * (((5.0f * fSlow369) + -3.0f) * fSlow52));
		float fSlow373 = (1.375f * (((fSlow369 * ((63.0f * fSlow369) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow374 = (fSlow321 - fSlow300);
		float fSlow375 = faustpower2_f(fSlow374);
		float fSlow376 = (1.125f * (((fSlow375 * ((35.0f * fSlow375) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow377 = (2.5f * (((3.0f * fSlow375) + -1.0f) * fSlow35));
		float fSlow378 = (3.5f * (((5.0f * fSlow375) + -3.0f) * fSlow52));
		float fSlow379 = (1.375f * (((fSlow375 * ((63.0f * fSlow375) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow380 = (fSlow328 - fSlow300);
		float fSlow381 = faustpower2_f(fSlow380);
		float fSlow382 = (1.125f * (((fSlow381 * ((35.0f * fSlow381) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow383 = (2.5f * (((3.0f * fSlow381) + -1.0f) * fSlow35));
		float fSlow384 = (3.5f * (((5.0f * fSlow381) + -3.0f) * fSlow52));
		float fSlow385 = (1.375f * (((fSlow381 * ((63.0f * fSlow381) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow386 = (fSlow335 - fSlow300);
		float fSlow387 = faustpower2_f(fSlow386);
		float fSlow388 = (1.125f * (((fSlow387 * ((35.0f * fSlow387) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow389 = (2.5f * (((3.0f * fSlow387) + -1.0f) * fSlow35));
		float fSlow390 = (3.5f * (((5.0f * fSlow387) + -3.0f) * fSlow52));
		float fSlow391 = (1.375f * (((fSlow387 * ((63.0f * fSlow387) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow392 = (fSlow342 - fSlow300);
		float fSlow393 = faustpower2_f(fSlow392);
		float fSlow394 = (1.125f * (((fSlow393 * ((35.0f * fSlow393) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow395 = (2.5f * (((3.0f * fSlow393) + -1.0f) * fSlow35));
		float fSlow396 = (3.5f * (((5.0f * fSlow393) + -3.0f) * fSlow52));
		float fSlow397 = (1.375f * (((fSlow393 * ((63.0f * fSlow393) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow398 = (fSlow349 - fSlow300);
		float fSlow399 = faustpower2_f(fSlow398);
		float fSlow400 = (1.125f * (((fSlow399 * ((35.0f * fSlow399) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow401 = (2.5f * (((3.0f * fSlow399) + -1.0f) * fSlow35));
		float fSlow402 = (3.5f * (((5.0f * fSlow399) + -3.0f) * fSlow52));
		float fSlow403 = (1.375f * (((fSlow399 * ((63.0f * fSlow399) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow404 = (fSlow270 - fSlow271);
		float fSlow405 = faustpower2_f(fSlow404);
		float fSlow406 = (2.5f * (((3.0f * fSlow405) + -1.0f) * fSlow35));
		float fSlow407 = (3.5f * (((5.0f * fSlow405) + -3.0f) * fSlow52));
		float fSlow408 = (1.375f * (((fSlow405 * ((63.0f * fSlow405) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow409 = (1.125f * (((fSlow405 * ((35.0f * fSlow405) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow410 = (fSlow278 - fSlow271);
		float fSlow411 = faustpower2_f(fSlow410);
		float fSlow412 = (1.125f * (((fSlow411 * ((35.0f * fSlow411) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow413 = (2.5f * (((3.0f * fSlow411) + -1.0f) * fSlow35));
		float fSlow414 = (3.5f * (((5.0f * fSlow411) + -3.0f) * fSlow52));
		float fSlow415 = (1.375f * (((fSlow411 * ((63.0f * fSlow411) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow416 = (fSlow285 - fSlow271);
		float fSlow417 = faustpower2_f(fSlow416);
		float fSlow418 = (1.125f * (((fSlow417 * ((35.0f * fSlow417) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow419 = (2.5f * (((3.0f * fSlow417) + -1.0f) * fSlow35));
		float fSlow420 = (3.5f * (((5.0f * fSlow417) + -3.0f) * fSlow52));
		float fSlow421 = (1.375f * (((fSlow417 * ((63.0f * fSlow417) + -70.0f)) + 15.0f) * fSlow66));
		float fSlow422 = (fSlow292 - fSlow271);
		float fSlow423 = faustpower2_f(fSlow422);
		float fSlow424 = (1.125f * (((fSlow423 * ((35.0f * fSlow423) + -30.0f)) + 3.0f) * fSlow9));
		float fSlow425 = (2.5f * (((3.0f * fSlow423) + -1.0f) * fSlow35));
		float fSlow426 = (3.5f * (((5.0f * fSlow423) + -3.0f) * fSlow52));
		float fSlow427 = (1.375f * (((fSlow423 * ((63.0f * fSlow423) + -70.0f)) + 15.0f) * fSlow66));
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec3[0] = (fSlow23 + (0.999000013f * fRec3[1]));
			float fTemp0 = (fRec3[0] * float(input0[i]));
			fRec4[0] = (fRec2[1] + fRec4[1]);
			fRec5[0] = (fRec4[1] + fRec5[1]);
			fRec2[0] = (fSlow19 * ((fSlow22 * fTemp0) - ((fSlow25 * fRec4[0]) + (fSlow24 * fRec5[0]))));
			fRec6[0] = (fRec2[0] + fRec6[1]);
			float fTemp1 = (fRec6[0] - fRec2[0]);
			fVec0[0] = fTemp1;
			fRec7[0] = (fRec7[1] + fVec0[1]);
			fRec8[0] = (fRec1[1] + fRec8[1]);
			fRec9[0] = (fRec8[1] + fRec9[1]);
			fRec1[0] = (fSlow14 * ((fSlow18 * (fRec2[0] + (fSlow26 * ((fSlow28 * fTemp1) + (fSlow27 * fRec7[0]))))) - ((fSlow30 * fRec8[0]) + (fSlow29 * fRec9[0]))));
			fRec10[0] = (fRec1[0] + fRec10[1]);
			float fTemp2 = (fRec10[0] - fRec1[0]);
			fVec1[0] = fTemp2;
			fRec11[0] = (fRec11[1] + fVec1[1]);
			fRec13[0] = (fRec12[1] + fRec13[1]);
			fRec14[0] = (fRec13[1] + fRec14[1]);
			fRec16[0] = (fRec15[1] + fRec16[1]);
			fRec17[0] = (fRec16[1] + fRec17[1]);
			float fTemp3 = (fSlow34 * fTemp0);
			fRec15[0] = (0.0f - (fSlow19 * (((fSlow25 * fRec16[0]) + (fSlow24 * fRec17[0])) - fTemp3)));
			fRec12[0] = (0.0f - (fSlow14 * (((fSlow30 * fRec13[0]) + (fSlow29 * fRec14[0])) - fRec15[0])));
			float fTemp4 = ((fRec1[0] + (fSlow31 * ((fSlow33 * fTemp2) + (fSlow32 * fRec11[0])))) + fRec12[0]);
			fRec19[0] = (fRec18[1] + fRec19[1]);
			fRec20[0] = (fRec19[1] + fRec20[1]);
			fRec18[0] = (fSlow37 * ((fSlow39 * fTemp0) - ((fSlow41 * fRec19[0]) + (fSlow40 * fRec20[0]))));
			fRec21[0] = (fRec18[0] + fRec21[1]);
			float fTemp5 = (fRec21[0] - fRec18[0]);
			fVec2[0] = fTemp5;
			fRec22[0] = (fRec22[1] + fVec2[1]);
			fRec24[0] = (fRec23[1] + fRec24[1]);
			fRec25[0] = (fRec24[1] + fRec25[1]);
			fRec23[0] = (0.0f - (fSlow37 * (((fSlow41 * fRec24[0]) + (fSlow40 * fRec25[0])) - fTemp3)));
			float fTemp6 = ((fRec18[0] + (fSlow42 * ((fSlow44 * fTemp5) + (fSlow43 * fRec22[0])))) + fRec23[0]);
			fRec28[0] = (fRec27[1] + fRec28[1]);
			fRec27[0] = (fSlow49 * ((fSlow50 * fTemp0) - (fSlow51 * fRec28[0])));
			fRec26[0] = (fRec27[0] + fRec26[1]);
			fRec30[0] = (fRec29[1] + fRec30[1]);
			fRec29[0] = (fSlow49 * (fTemp3 - (fSlow51 * fRec30[0])));
			float fTemp7 = ((fSlow48 * (fRec26[0] - fRec27[0])) + (fRec27[0] + fRec29[0]));
			float fTemp8 = (fSlow46 * fTemp7);
			fRec34[0] = (fRec33[1] + fRec34[1]);
			fRec35[0] = (fRec34[1] + fRec35[1]);
			fRec33[0] = (fSlow57 * ((fSlow59 * fTemp0) - ((fSlow61 * fRec34[0]) + (fSlow60 * fRec35[0]))));
			fRec36[0] = (fRec33[0] + fRec36[1]);
			float fTemp9 = (fRec36[0] - fRec33[0]);
			fVec3[0] = fTemp9;
			fRec37[0] = (fRec37[1] + fVec3[1]);
			fRec38[0] = (fRec38[1] + fRec32[1]);
			fRec32[0] = (fSlow56 * ((fSlow54 * (fRec33[0] + (fSlow62 * ((fSlow64 * fTemp9) + (fSlow63 * fRec37[0]))))) - (fSlow65 * fRec38[0])));
			fRec31[0] = (fRec32[0] + fRec31[1]);
			fRec41[0] = (fRec40[1] + fRec41[1]);
			fRec42[0] = (fRec41[1] + fRec42[1]);
			fRec40[0] = (0.0f - (fSlow57 * (((fSlow61 * fRec41[0]) + (fSlow60 * fRec42[0])) - fTemp3)));
			fRec43[0] = (fRec39[1] + fRec43[1]);
			fRec39[0] = (fSlow56 * (fRec40[0] - (fSlow65 * fRec43[0])));
			float fTemp10 = ((fSlow55 * (fRec31[0] - fRec32[0])) + (fRec32[0] + fRec39[0]));
			fRec48[0] = (fRec47[1] + fRec48[1]);
			fRec49[0] = (fRec48[1] + fRec49[1]);
			fRec47[0] = (fSlow73 * ((fSlow75 * fTemp0) - ((fSlow77 * fRec48[0]) + (fSlow76 * fRec49[0]))));
			fRec50[0] = (fRec47[0] + fRec50[1]);
			float fTemp11 = (fRec50[0] - fRec47[0]);
			fVec4[0] = fTemp11;
			fRec51[0] = (fRec51[1] + fVec4[1]);
			fRec52[0] = (fRec46[1] + fRec52[1]);
			fRec53[0] = (fRec52[1] + fRec53[1]);
			fRec46[0] = (fSlow71 * ((fSlow72 * (fRec47[0] + (fSlow78 * ((fSlow80 * fTemp11) + (fSlow79 * fRec51[0]))))) - ((fSlow82 * fRec52[0]) + (fSlow81 * fRec53[0]))));
			fRec54[0] = (fRec46[0] + fRec54[1]);
			float fTemp12 = (fRec54[0] - fRec46[0]);
			fVec5[0] = fTemp12;
			fRec55[0] = (fRec55[1] + fVec5[1]);
			fRec56[0] = (fRec45[1] + fRec56[1]);
			fRec45[0] = (fSlow70 * ((fSlow68 * (fRec46[0] + (fSlow83 * ((fSlow85 * fTemp12) + (fSlow84 * fRec55[0]))))) - (fSlow86 * fRec56[0])));
			fRec44[0] = (fRec45[0] + fRec44[1]);
			fRec59[0] = (fRec58[1] + fRec59[1]);
			fRec60[0] = (fRec59[1] + fRec60[1]);
			fRec62[0] = (fRec61[1] + fRec62[1]);
			fRec63[0] = (fRec62[1] + fRec63[1]);
			fRec61[0] = (0.0f - (fSlow73 * (((fSlow77 * fRec62[0]) + (fSlow76 * fRec63[0])) - fTemp3)));
			fRec58[0] = (0.0f - (fSlow71 * (((fSlow82 * fRec59[0]) + (fSlow81 * fRec60[0])) - fRec61[0])));
			fRec64[0] = (fRec57[1] + fRec64[1]);
			fRec57[0] = (fSlow70 * (fRec58[0] - (fSlow86 * fRec64[0])));
			float fTemp13 = ((fSlow69 * (fRec44[0] - fRec45[0])) + (fRec45[0] + fRec57[0]));
			float fTemp14 = (fSlow87 * fTemp0);
			float fTemp15 = (0.0126984129f * ((fSlow10 * fTemp4) + ((fSlow36 * fTemp6) + ((fSlow7 * ((fTemp8 + (fSlow53 * fTemp10)) + (fSlow67 * fTemp13))) + fTemp14))));
			fRec0[0] = max((fRec0[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp15))))));
			fVbargraph0 = FAUSTFLOAT(fRec0[0]);
			output0[i] = FAUSTFLOAT(fTemp15);
			float fTemp16 = (0.0126984129f * (((fSlow89 * fTemp4) + (((fSlow90 * fTemp6) + (fTemp14 + (fSlow91 * fTemp7))) + (fSlow93 * fTemp10))) + (fSlow94 * fTemp13)));
			fRec65[0] = max((fRec65[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp16))))));
			fVbargraph1 = FAUSTFLOAT(fRec65[0]);
			output1[i] = FAUSTFLOAT(fTemp16);
			float fTemp17 = (0.0126984129f * (((fSlow98 * fTemp4) + (((fSlow99 * fTemp6) + (fTemp14 + (fSlow100 * fTemp7))) + (fSlow102 * fTemp10))) + (fSlow103 * fTemp13)));
			fRec66[0] = max((fRec66[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp17))))));
			fVbargraph2 = FAUSTFLOAT(fRec66[0]);
			output2[i] = FAUSTFLOAT(fTemp17);
			float fTemp18 = (0.0126984129f * (((fSlow107 * fTemp4) + (((fSlow108 * fTemp6) + (fTemp14 + (fSlow109 * fTemp7))) + (fSlow111 * fTemp10))) + (fSlow112 * fTemp13)));
			fRec67[0] = max((fRec67[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp18))))));
			fVbargraph3 = FAUSTFLOAT(fRec67[0]);
			output3[i] = FAUSTFLOAT(fTemp18);
			float fTemp19 = (0.0126984129f * (((fSlow116 * fTemp4) + (((fSlow117 * fTemp6) + (fTemp14 + (fSlow118 * fTemp7))) + (fSlow120 * fTemp10))) + (fSlow121 * fTemp13)));
			fRec68[0] = max((fRec68[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp19))))));
			fVbargraph4 = FAUSTFLOAT(fRec68[0]);
			output4[i] = FAUSTFLOAT(fTemp19);
			float fTemp20 = (0.0126984129f * ((fSlow124 * fTemp4) + ((fSlow125 * fTemp6) + (fTemp14 + (fSlow122 * ((fTemp8 + (fSlow126 * fTemp10)) + (fSlow127 * fTemp13)))))));
			fRec69[0] = max((fRec69[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp20))))));
			fVbargraph5 = FAUSTFLOAT(fRec69[0]);
			output5[i] = FAUSTFLOAT(fTemp20);
			float fTemp21 = (0.0225749556f * ((fSlow132 * fTemp4) + ((fSlow133 * fTemp6) + (fTemp14 + (fSlow130 * ((fTemp8 + (fSlow134 * fTemp10)) + (fSlow135 * fTemp13)))))));
			fRec70[0] = max((fRec70[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp21))))));
			fVbargraph6 = FAUSTFLOAT(fRec70[0]);
			output6[i] = FAUSTFLOAT(fTemp21);
			float fTemp22 = (0.0225749556f * ((fSlow139 * fTemp4) + ((fSlow140 * fTemp6) + (fTemp14 + (fSlow137 * ((fTemp8 + (fSlow141 * fTemp10)) + (fSlow142 * fTemp13)))))));
			fRec71[0] = max((fRec71[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp22))))));
			fVbargraph7 = FAUSTFLOAT(fRec71[0]);
			output7[i] = FAUSTFLOAT(fTemp22);
			float fTemp23 = (0.0225749556f * ((fSlow146 * fTemp4) + ((fSlow147 * fTemp6) + (fTemp14 + (fSlow144 * ((fTemp8 + (fSlow148 * fTemp10)) + (fSlow149 * fTemp13)))))));
			fRec72[0] = max((fRec72[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp23))))));
			fVbargraph8 = FAUSTFLOAT(fRec72[0]);
			output8[i] = FAUSTFLOAT(fTemp23);
			float fTemp24 = (0.0225749556f * ((fSlow153 * fTemp4) + ((fSlow154 * fTemp6) + (fTemp14 + (fSlow151 * ((fTemp8 + (fSlow155 * fTemp10)) + (fSlow156 * fTemp13)))))));
			fRec73[0] = max((fRec73[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp24))))));
			fVbargraph9 = FAUSTFLOAT(fRec73[0]);
			output9[i] = FAUSTFLOAT(fTemp24);
			float fTemp25 = (0.0225749556f * (((fSlow160 * fTemp4) + (((fSlow161 * fTemp6) + (fTemp14 + (fSlow162 * fTemp7))) + (fSlow164 * fTemp10))) + (fSlow165 * fTemp13)));
			fRec74[0] = max((fRec74[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp25))))));
			fVbargraph10 = FAUSTFLOAT(fRec74[0]);
			output10[i] = FAUSTFLOAT(fTemp25);
			float fTemp26 = (0.0225749556f * (((fSlow169 * fTemp4) + (((fSlow170 * fTemp6) + (fTemp14 + (fSlow171 * fTemp7))) + (fSlow173 * fTemp10))) + (fSlow174 * fTemp13)));
			fRec75[0] = max((fRec75[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp26))))));
			fVbargraph11 = FAUSTFLOAT(fRec75[0]);
			output11[i] = FAUSTFLOAT(fTemp26);
			float fTemp27 = (0.0225749556f * (((fSlow178 * fTemp4) + (((fSlow179 * fTemp6) + (fTemp14 + (fSlow180 * fTemp7))) + (fSlow182 * fTemp10))) + (fSlow183 * fTemp13)));
			fRec76[0] = max((fRec76[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp27))))));
			fVbargraph12 = FAUSTFLOAT(fRec76[0]);
			output12[i] = FAUSTFLOAT(fTemp27);
			float fTemp28 = (0.0225749556f * (((fSlow187 * fTemp4) + (((fSlow188 * fTemp6) + (fTemp14 + (fSlow189 * fTemp7))) + (fSlow191 * fTemp10))) + (fSlow192 * fTemp13)));
			fRec77[0] = max((fRec77[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp28))))));
			fVbargraph13 = FAUSTFLOAT(fRec77[0]);
			output13[i] = FAUSTFLOAT(fTemp28);
			float fTemp29 = (0.0225749556f * ((fSlow195 * fTemp4) + ((fSlow196 * fTemp6) + (fTemp14 + (fSlow193 * ((fTemp8 + (fSlow197 * fTemp10)) + (fSlow198 * fTemp13)))))));
			fRec78[0] = max((fRec78[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp29))))));
			fVbargraph14 = FAUSTFLOAT(fRec78[0]);
			output14[i] = FAUSTFLOAT(fTemp29);
			float fTemp30 = (0.0225749556f * ((fSlow201 * fTemp4) + ((fSlow202 * fTemp6) + (fTemp14 + (fSlow199 * ((fTemp8 + (fSlow203 * fTemp10)) + (fSlow204 * fTemp13)))))));
			fRec79[0] = max((fRec79[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp30))))));
			fVbargraph15 = FAUSTFLOAT(fRec79[0]);
			output15[i] = FAUSTFLOAT(fTemp30);
			float fTemp31 = (0.0225749556f * ((fSlow207 * fTemp4) + ((fSlow208 * fTemp6) + (fTemp14 + (fSlow205 * ((fTemp8 + (fSlow209 * fTemp10)) + (fSlow210 * fTemp13)))))));
			fRec80[0] = max((fRec80[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp31))))));
			fVbargraph16 = FAUSTFLOAT(fRec80[0]);
			output16[i] = FAUSTFLOAT(fTemp31);
			float fTemp32 = (0.0225749556f * ((fSlow213 * fTemp4) + ((fSlow214 * fTemp6) + (fTemp14 + (fSlow211 * ((fTemp8 + (fSlow215 * fTemp10)) + (fSlow216 * fTemp13)))))));
			fRec81[0] = max((fRec81[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp32))))));
			fVbargraph17 = FAUSTFLOAT(fRec81[0]);
			output17[i] = FAUSTFLOAT(fTemp32);
			float fTemp33 = (0.0210937504f * ((fSlow221 * fTemp4) + ((fSlow222 * fTemp6) + (fTemp14 + (fSlow219 * ((fTemp8 + (fSlow223 * fTemp10)) + (fSlow224 * fTemp13)))))));
			fRec82[0] = max((fRec82[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp33))))));
			fVbargraph18 = FAUSTFLOAT(fRec82[0]);
			output18[i] = FAUSTFLOAT(fTemp33);
			float fTemp34 = (0.0210937504f * ((fSlow228 * fTemp4) + ((fSlow229 * fTemp6) + (fTemp14 + (fSlow226 * ((fTemp8 + (fSlow230 * fTemp10)) + (fSlow231 * fTemp13)))))));
			fRec83[0] = max((fRec83[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp34))))));
			fVbargraph19 = FAUSTFLOAT(fRec83[0]);
			output19[i] = FAUSTFLOAT(fTemp34);
			float fTemp35 = (0.0210937504f * ((fSlow235 * fTemp4) + ((fSlow236 * fTemp6) + (fTemp14 + (fSlow233 * ((fTemp8 + (fSlow237 * fTemp10)) + (fSlow238 * fTemp13)))))));
			fRec84[0] = max((fRec84[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp35))))));
			fVbargraph20 = FAUSTFLOAT(fRec84[0]);
			output20[i] = FAUSTFLOAT(fTemp35);
			float fTemp36 = (0.0210937504f * ((fSlow242 * fTemp4) + ((fSlow243 * fTemp6) + (fTemp14 + (fSlow240 * ((fTemp8 + (fSlow244 * fTemp10)) + (fSlow245 * fTemp13)))))));
			fRec85[0] = max((fRec85[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp36))))));
			fVbargraph21 = FAUSTFLOAT(fRec85[0]);
			output21[i] = FAUSTFLOAT(fTemp36);
			float fTemp37 = (0.0210937504f * ((fSlow248 * fTemp4) + ((fSlow249 * fTemp6) + (fTemp14 + (fSlow246 * ((fTemp8 + (fSlow250 * fTemp10)) + (fSlow251 * fTemp13)))))));
			fRec86[0] = max((fRec86[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp37))))));
			fVbargraph22 = FAUSTFLOAT(fRec86[0]);
			output22[i] = FAUSTFLOAT(fTemp37);
			float fTemp38 = (0.0210937504f * ((fSlow254 * fTemp4) + ((fSlow255 * fTemp6) + (fTemp14 + (fSlow252 * ((fTemp8 + (fSlow256 * fTemp10)) + (fSlow257 * fTemp13)))))));
			fRec87[0] = max((fRec87[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp38))))));
			fVbargraph23 = FAUSTFLOAT(fRec87[0]);
			output23[i] = FAUSTFLOAT(fTemp38);
			float fTemp39 = (0.0210937504f * ((fSlow260 * fTemp4) + ((fSlow261 * fTemp6) + (fTemp14 + (fSlow258 * ((fTemp8 + (fSlow262 * fTemp10)) + (fSlow263 * fTemp13)))))));
			fRec88[0] = max((fRec88[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp39))))));
			fVbargraph24 = FAUSTFLOAT(fRec88[0]);
			output24[i] = FAUSTFLOAT(fTemp39);
			float fTemp40 = (0.0210937504f * ((fSlow266 * fTemp4) + ((fSlow267 * fTemp6) + (fTemp14 + (fSlow264 * ((fTemp8 + (fSlow268 * fTemp10)) + (fSlow269 * fTemp13)))))));
			fRec89[0] = max((fRec89[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp40))))));
			fVbargraph25 = FAUSTFLOAT(fRec89[0]);
			output25[i] = FAUSTFLOAT(fTemp40);
			float fTemp41 = (0.0201733354f * ((fSlow274 * fTemp4) + ((fSlow275 * fTemp6) + (fTemp14 + (fSlow272 * ((fTemp8 + (fSlow276 * fTemp10)) + (fSlow277 * fTemp13)))))));
			fRec90[0] = max((fRec90[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp41))))));
			fVbargraph26 = FAUSTFLOAT(fRec90[0]);
			output26[i] = FAUSTFLOAT(fTemp41);
			float fTemp42 = (0.0201733354f * ((fSlow281 * fTemp4) + ((fSlow282 * fTemp6) + (fTemp14 + (fSlow279 * ((fTemp8 + (fSlow283 * fTemp10)) + (fSlow284 * fTemp13)))))));
			fRec91[0] = max((fRec91[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp42))))));
			fVbargraph27 = FAUSTFLOAT(fRec91[0]);
			output27[i] = FAUSTFLOAT(fTemp42);
			float fTemp43 = (0.0201733354f * ((fSlow288 * fTemp4) + ((fSlow289 * fTemp6) + (fTemp14 + (fSlow286 * ((fTemp8 + (fSlow290 * fTemp10)) + (fSlow291 * fTemp13)))))));
			fRec92[0] = max((fRec92[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp43))))));
			fVbargraph28 = FAUSTFLOAT(fRec92[0]);
			output28[i] = FAUSTFLOAT(fTemp43);
			float fTemp44 = (0.0201733354f * ((fSlow295 * fTemp4) + ((fSlow296 * fTemp6) + (fTemp14 + (fSlow293 * ((fTemp8 + (fSlow297 * fTemp10)) + (fSlow298 * fTemp13)))))));
			fRec93[0] = max((fRec93[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp44))))));
			fVbargraph29 = FAUSTFLOAT(fRec93[0]);
			output29[i] = FAUSTFLOAT(fTemp44);
			float fTemp45 = (0.0201733354f * ((fSlow303 * fTemp4) + ((fSlow304 * fTemp6) + (fTemp14 + (fSlow301 * ((fTemp8 + (fSlow305 * fTemp10)) + (fSlow306 * fTemp13)))))));
			fRec94[0] = max((fRec94[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp45))))));
			fVbargraph30 = FAUSTFLOAT(fRec94[0]);
			output30[i] = FAUSTFLOAT(fTemp45);
			float fTemp46 = (0.0201733354f * ((fSlow310 * fTemp4) + ((fSlow311 * fTemp6) + (fTemp14 + (fSlow308 * ((fTemp8 + (fSlow312 * fTemp10)) + (fSlow313 * fTemp13)))))));
			fRec95[0] = max((fRec95[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp46))))));
			fVbargraph31 = FAUSTFLOAT(fRec95[0]);
			output31[i] = FAUSTFLOAT(fTemp46);
			float fTemp47 = (0.0201733354f * ((fSlow317 * fTemp4) + ((fSlow318 * fTemp6) + (fTemp14 + (fSlow315 * ((fTemp8 + (fSlow319 * fTemp10)) + (fSlow320 * fTemp13)))))));
			fRec96[0] = max((fRec96[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp47))))));
			fVbargraph32 = FAUSTFLOAT(fRec96[0]);
			output32[i] = FAUSTFLOAT(fTemp47);
			float fTemp48 = (0.0201733354f * ((fSlow324 * fTemp4) + ((fSlow325 * fTemp6) + (fTemp14 + (fSlow322 * ((fTemp8 + (fSlow326 * fTemp10)) + (fSlow327 * fTemp13)))))));
			fRec97[0] = max((fRec97[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp48))))));
			fVbargraph33 = FAUSTFLOAT(fRec97[0]);
			output33[i] = FAUSTFLOAT(fTemp48);
			float fTemp49 = (0.0201733354f * ((fSlow331 * fTemp4) + ((fSlow332 * fTemp6) + (fTemp14 + (fSlow329 * ((fTemp8 + (fSlow333 * fTemp10)) + (fSlow334 * fTemp13)))))));
			fRec98[0] = max((fRec98[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp49))))));
			fVbargraph34 = FAUSTFLOAT(fRec98[0]);
			output34[i] = FAUSTFLOAT(fTemp49);
			float fTemp50 = (0.0201733354f * ((fSlow338 * fTemp4) + ((fSlow339 * fTemp6) + (fTemp14 + (fSlow336 * ((fTemp8 + (fSlow340 * fTemp10)) + (fSlow341 * fTemp13)))))));
			fRec99[0] = max((fRec99[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp50))))));
			fVbargraph35 = FAUSTFLOAT(fRec99[0]);
			output35[i] = FAUSTFLOAT(fTemp50);
			float fTemp51 = (0.0201733354f * ((fSlow345 * fTemp4) + ((fSlow346 * fTemp6) + (fTemp14 + (fSlow343 * ((fTemp8 + (fSlow347 * fTemp10)) + (fSlow348 * fTemp13)))))));
			fRec100[0] = max((fRec100[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp51))))));
			fVbargraph36 = FAUSTFLOAT(fRec100[0]);
			output36[i] = FAUSTFLOAT(fTemp51);
			float fTemp52 = (0.0201733354f * ((fSlow352 * fTemp4) + ((fSlow353 * fTemp6) + (fTemp14 + (fSlow350 * ((fTemp8 + (fSlow354 * fTemp10)) + (fSlow355 * fTemp13)))))));
			fRec101[0] = max((fRec101[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp52))))));
			fVbargraph37 = FAUSTFLOAT(fRec101[0]);
			output37[i] = FAUSTFLOAT(fTemp52);
			float fTemp53 = (0.0201733354f * ((fSlow358 * fTemp4) + ((fSlow359 * fTemp6) + (fTemp14 + (fSlow356 * ((fTemp8 + (fSlow360 * fTemp10)) + (fSlow361 * fTemp13)))))));
			fRec102[0] = max((fRec102[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp53))))));
			fVbargraph38 = FAUSTFLOAT(fRec102[0]);
			output38[i] = FAUSTFLOAT(fTemp53);
			float fTemp54 = (0.0201733354f * ((fSlow364 * fTemp4) + ((fSlow365 * fTemp6) + (fTemp14 + (fSlow362 * ((fTemp8 + (fSlow366 * fTemp10)) + (fSlow367 * fTemp13)))))));
			fRec103[0] = max((fRec103[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp54))))));
			fVbargraph39 = FAUSTFLOAT(fRec103[0]);
			output39[i] = FAUSTFLOAT(fTemp54);
			float fTemp55 = (0.0201733354f * ((fSlow370 * fTemp4) + ((fSlow371 * fTemp6) + (fTemp14 + (fSlow368 * ((fTemp8 + (fSlow372 * fTemp10)) + (fSlow373 * fTemp13)))))));
			fRec104[0] = max((fRec104[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp55))))));
			fVbargraph40 = FAUSTFLOAT(fRec104[0]);
			output40[i] = FAUSTFLOAT(fTemp55);
			float fTemp56 = (0.0201733354f * ((fSlow376 * fTemp4) + ((fSlow377 * fTemp6) + (fTemp14 + (fSlow374 * ((fTemp8 + (fSlow378 * fTemp10)) + (fSlow379 * fTemp13)))))));
			fRec105[0] = max((fRec105[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp56))))));
			fVbargraph41 = FAUSTFLOAT(fRec105[0]);
			output41[i] = FAUSTFLOAT(fTemp56);
			float fTemp57 = (0.0201733354f * ((fSlow382 * fTemp4) + ((fSlow383 * fTemp6) + (fTemp14 + (fSlow380 * ((fTemp8 + (fSlow384 * fTemp10)) + (fSlow385 * fTemp13)))))));
			fRec106[0] = max((fRec106[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp57))))));
			fVbargraph42 = FAUSTFLOAT(fRec106[0]);
			output42[i] = FAUSTFLOAT(fTemp57);
			float fTemp58 = (0.0201733354f * ((fSlow388 * fTemp4) + ((fSlow389 * fTemp6) + (fTemp14 + (fSlow386 * ((fTemp8 + (fSlow390 * fTemp10)) + (fSlow391 * fTemp13)))))));
			fRec107[0] = max((fRec107[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp58))))));
			fVbargraph43 = FAUSTFLOAT(fRec107[0]);
			output43[i] = FAUSTFLOAT(fTemp58);
			float fTemp59 = (0.0201733354f * ((fSlow394 * fTemp4) + ((fSlow395 * fTemp6) + (fTemp14 + (fSlow392 * ((fTemp8 + (fSlow396 * fTemp10)) + (fSlow397 * fTemp13)))))));
			fRec108[0] = max((fRec108[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp59))))));
			fVbargraph44 = FAUSTFLOAT(fRec108[0]);
			output44[i] = FAUSTFLOAT(fTemp59);
			float fTemp60 = (0.0201733354f * ((fSlow400 * fTemp4) + ((fSlow401 * fTemp6) + (fTemp14 + (fSlow398 * ((fTemp8 + (fSlow402 * fTemp10)) + (fSlow403 * fTemp13)))))));
			fRec109[0] = max((fRec109[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp60))))));
			fVbargraph45 = FAUSTFLOAT(fRec109[0]);
			output45[i] = FAUSTFLOAT(fTemp60);
			float fTemp61 = (0.0201733354f * ((fSlow406 * fTemp6) + ((fSlow404 * ((fTemp8 + (fSlow407 * fTemp10)) + (fSlow408 * fTemp13))) + ((fSlow409 * fTemp4) + fTemp14))));
			fRec110[0] = max((fRec110[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp61))))));
			fVbargraph46 = FAUSTFLOAT(fRec110[0]);
			output46[i] = FAUSTFLOAT(fTemp61);
			float fTemp62 = (0.0201733354f * ((fSlow412 * fTemp4) + ((fSlow413 * fTemp6) + (fTemp14 + (fSlow410 * ((fTemp8 + (fSlow414 * fTemp10)) + (fSlow415 * fTemp13)))))));
			fRec111[0] = max((fRec111[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp62))))));
			fVbargraph47 = FAUSTFLOAT(fRec111[0]);
			output47[i] = FAUSTFLOAT(fTemp62);
			float fTemp63 = (0.0201733354f * ((fSlow418 * fTemp4) + ((fSlow419 * fTemp6) + (fTemp14 + (fSlow416 * ((fTemp8 + (fSlow420 * fTemp10)) + (fSlow421 * fTemp13)))))));
			fRec112[0] = max((fRec112[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp63))))));
			fVbargraph48 = FAUSTFLOAT(fRec112[0]);
			output48[i] = FAUSTFLOAT(fTemp63);
			float fTemp64 = (0.0201733354f * ((fSlow424 * fTemp4) + ((fSlow425 * fTemp6) + (fTemp14 + (fSlow422 * ((fTemp8 + (fSlow426 * fTemp10)) + (fSlow427 * fTemp13)))))));
			fRec113[0] = max((fRec113[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp64))))));
			fVbargraph49 = FAUSTFLOAT(fRec113[0]);
			output49[i] = FAUSTFLOAT(fTemp64);
			fRec3[1] = fRec3[0];
			fRec4[1] = fRec4[0];
			fRec5[1] = fRec5[0];
			fRec2[1] = fRec2[0];
			fRec6[1] = fRec6[0];
			fVec0[1] = fVec0[0];
			fRec7[1] = fRec7[0];
			fRec8[1] = fRec8[0];
			fRec9[1] = fRec9[0];
			fRec1[1] = fRec1[0];
			fRec10[1] = fRec10[0];
			fVec1[1] = fVec1[0];
			fRec11[1] = fRec11[0];
			fRec13[1] = fRec13[0];
			fRec14[1] = fRec14[0];
			fRec16[1] = fRec16[0];
			fRec17[1] = fRec17[0];
			fRec15[1] = fRec15[0];
			fRec12[1] = fRec12[0];
			fRec19[1] = fRec19[0];
			fRec20[1] = fRec20[0];
			fRec18[1] = fRec18[0];
			fRec21[1] = fRec21[0];
			fVec2[1] = fVec2[0];
			fRec22[1] = fRec22[0];
			fRec24[1] = fRec24[0];
			fRec25[1] = fRec25[0];
			fRec23[1] = fRec23[0];
			fRec28[1] = fRec28[0];
			fRec27[1] = fRec27[0];
			fRec26[1] = fRec26[0];
			fRec30[1] = fRec30[0];
			fRec29[1] = fRec29[0];
			fRec34[1] = fRec34[0];
			fRec35[1] = fRec35[0];
			fRec33[1] = fRec33[0];
			fRec36[1] = fRec36[0];
			fVec3[1] = fVec3[0];
			fRec37[1] = fRec37[0];
			fRec38[1] = fRec38[0];
			fRec32[1] = fRec32[0];
			fRec31[1] = fRec31[0];
			fRec41[1] = fRec41[0];
			fRec42[1] = fRec42[0];
			fRec40[1] = fRec40[0];
			fRec43[1] = fRec43[0];
			fRec39[1] = fRec39[0];
			fRec48[1] = fRec48[0];
			fRec49[1] = fRec49[0];
			fRec47[1] = fRec47[0];
			fRec50[1] = fRec50[0];
			fVec4[1] = fVec4[0];
			fRec51[1] = fRec51[0];
			fRec52[1] = fRec52[0];
			fRec53[1] = fRec53[0];
			fRec46[1] = fRec46[0];
			fRec54[1] = fRec54[0];
			fVec5[1] = fVec5[0];
			fRec55[1] = fRec55[0];
			fRec56[1] = fRec56[0];
			fRec45[1] = fRec45[0];
			fRec44[1] = fRec44[0];
			fRec59[1] = fRec59[0];
			fRec60[1] = fRec60[0];
			fRec62[1] = fRec62[0];
			fRec63[1] = fRec63[0];
			fRec61[1] = fRec61[0];
			fRec58[1] = fRec58[0];
			fRec64[1] = fRec64[0];
			fRec57[1] = fRec57[0];
			fRec0[1] = fRec0[0];
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
