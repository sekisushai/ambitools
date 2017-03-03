/* ------------------------------------------------------------
author: "Pierre Lecomte"
copyright: "(c) Pierre Lecomte 2014"
license: "GPL"
name: "HOADecLebedev504"
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
	float fRec9[2];
	float fRec10[2];
	float fRec8[2];
	float fRec5[2];
	float fRec4[2];
	float fRec13[2];
	float fRec14[2];
	float fRec16[2];
	float fRec17[2];
	float fRec15[2];
	float fRec12[2];
	float fRec11[2];
	float fRec20[2];
	float fRec21[2];
	float fRec23[2];
	float fRec24[2];
	float fRec22[2];
	float fRec19[2];
	float fRec18[2];
	float fRec27[2];
	float fRec28[2];
	float fRec30[2];
	float fRec31[2];
	float fRec29[2];
	float fRec26[2];
	float fRec25[2];
	float fRec35[2];
	float fRec36[2];
	float fRec34[2];
	float fRec37[2];
	float fRec38[2];
	float fRec33[2];
	float fRec32[2];
	float fRec42[2];
	float fRec43[2];
	float fRec41[2];
	float fRec44[2];
	float fRec40[2];
	float fRec39[2];
	float fRec48[2];
	float fRec49[2];
	float fRec47[2];
	float fRec50[2];
	float fRec46[2];
	float fRec45[2];
	float fRec54[2];
	float fRec55[2];
	float fRec53[2];
	float fRec56[2];
	float fRec52[2];
	float fRec51[2];
	float fRec60[2];
	float fRec61[2];
	float fRec59[2];
	float fRec62[2];
	float fRec58[2];
	float fRec57[2];
	float fRec65[2];
	float fRec66[2];
	float fRec64[2];
	float fRec63[2];
	float fRec69[2];
	float fRec70[2];
	float fRec68[2];
	float fRec67[2];
	float fRec73[2];
	float fRec74[2];
	float fRec72[2];
	float fRec71[2];
	float fRec77[2];
	float fRec76[2];
	float fRec75[2];
	float fRec80[2];
	float fRec79[2];
	float fRec78[2];
	float fRec0[2];
	float fRec81[2];
	float fRec85[2];
	float fRec84[2];
	float fRec83[2];
	float fRec88[2];
	float fRec89[2];
	float fRec87[2];
	float fRec86[2];
	float fRec92[2];
	float fRec93[2];
	float fRec95[2];
	float fRec96[2];
	float fRec94[2];
	float fRec91[2];
	float fRec90[2];
	float fRec100[2];
	float fRec101[2];
	float fRec99[2];
	float fRec102[2];
	float fRec98[2];
	float fRec97[2];
	float fRec106[2];
	float fRec107[2];
	float fRec105[2];
	float fRec108[2];
	float fRec104[2];
	float fRec103[2];
	float fRec111[2];
	float fRec112[2];
	float fRec114[2];
	float fRec115[2];
	float fRec113[2];
	float fRec110[2];
	float fRec109[2];
	float fRec82[2];
	float fRec116[2];
	float fRec117[2];
	float fRec118[2];
	float fRec119[2];
	float fRec123[2];
	float fRec124[2];
	float fRec126[2];
	float fRec127[2];
	float fRec125[2];
	float fRec122[2];
	float fRec121[2];
	float fRec131[2];
	float fRec132[2];
	float fRec130[2];
	float fRec133[2];
	float fRec129[2];
	float fRec128[2];
	float fRec136[2];
	float fRec137[2];
	float fRec135[2];
	float fRec134[2];
	float fRec140[2];
	float fRec141[2];
	float fRec143[2];
	float fRec144[2];
	float fRec142[2];
	float fRec139[2];
	float fRec138[2];
	float fRec120[2];
	float fRec145[2];
	float fRec146[2];
	float fRec147[2];
	float fRec148[2];
	float fRec149[2];
	float fRec150[2];
	float fRec151[2];
	float fRec152[2];
	float fRec153[2];
	float fRec154[2];
	float fRec155[2];
	float fRec156[2];
	float fRec157[2];
	float fRec158[2];
	float fRec159[2];
	float fRec160[2];
	float fRec161[2];
	float fRec162[2];
	float fRec163[2];
	float fRec164[2];
	float fRec165[2];
	float fRec166[2];
	float fRec167[2];
	float fRec168[2];
	float fRec169[2];
	float fRec170[2];
	float fRec171[2];
	float fRec172[2];
	float fRec173[2];
	float fRec174[2];
	float fRec175[2];
	float fRec176[2];
	float fRec177[2];
	float fRec178[2];
	float fRec179[2];
	float fRec180[2];
	float fRec181[2];
	float fRec182[2];
	float fRec183[2];
	float fRec184[2];
	float fRec185[2];
	float fRec186[2];
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
	FAUSTFLOAT fVbargraph5;
	float fConst7;
	float fConst8;
	float fConst9;
	float fConst10;
	FAUSTFLOAT fVbargraph6;
	FAUSTFLOAT fVbargraph7;
	FAUSTFLOAT fVbargraph8;
	FAUSTFLOAT fVbargraph9;
	float fConst11;
	float fConst12;
	FAUSTFLOAT fVbargraph10;
	FAUSTFLOAT fVbargraph11;
	FAUSTFLOAT fVbargraph12;
	float fConst13;
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
	FAUSTFLOAT fVbargraph66;
	FAUSTFLOAT fVbargraph67;
	FAUSTFLOAT fVbargraph68;
	FAUSTFLOAT fVbargraph69;
	FAUSTFLOAT fVbargraph70;
	FAUSTFLOAT fVbargraph71;
	FAUSTFLOAT fVbargraph72;
	FAUSTFLOAT fVbargraph73;
	FAUSTFLOAT fVbargraph74;
	
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
		m->declare("name", "HOADecLebedev504");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
		m->declare("version", "1.0");
	}

	virtual int getNumInputs() {
		return 25;
		
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
		fConst4 = (1969.42322f / fConst0);
		fConst5 = (715.288391f / fConst0);
		fConst6 = (1430.57678f / fConst0);
		fConst7 = (394.771515f / fConst0);
		fConst8 = (625.228516f / fConst0);
		fConst9 = (1250.45703f / fConst0);
		fConst10 = (789.54303f / fConst0);
		fConst11 = (510.0f / fConst0);
		fConst12 = (1020.0f / fConst0);
		fConst13 = (340.0f / fConst0);
		
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
			fRec9[i5] = 0.0f;
			
		}
		for (int i6 = 0; (i6 < 2); i6 = (i6 + 1)) {
			fRec10[i6] = 0.0f;
			
		}
		for (int i7 = 0; (i7 < 2); i7 = (i7 + 1)) {
			fRec8[i7] = 0.0f;
			
		}
		for (int i8 = 0; (i8 < 2); i8 = (i8 + 1)) {
			fRec5[i8] = 0.0f;
			
		}
		for (int i9 = 0; (i9 < 2); i9 = (i9 + 1)) {
			fRec4[i9] = 0.0f;
			
		}
		for (int i10 = 0; (i10 < 2); i10 = (i10 + 1)) {
			fRec13[i10] = 0.0f;
			
		}
		for (int i11 = 0; (i11 < 2); i11 = (i11 + 1)) {
			fRec14[i11] = 0.0f;
			
		}
		for (int i12 = 0; (i12 < 2); i12 = (i12 + 1)) {
			fRec16[i12] = 0.0f;
			
		}
		for (int i13 = 0; (i13 < 2); i13 = (i13 + 1)) {
			fRec17[i13] = 0.0f;
			
		}
		for (int i14 = 0; (i14 < 2); i14 = (i14 + 1)) {
			fRec15[i14] = 0.0f;
			
		}
		for (int i15 = 0; (i15 < 2); i15 = (i15 + 1)) {
			fRec12[i15] = 0.0f;
			
		}
		for (int i16 = 0; (i16 < 2); i16 = (i16 + 1)) {
			fRec11[i16] = 0.0f;
			
		}
		for (int i17 = 0; (i17 < 2); i17 = (i17 + 1)) {
			fRec20[i17] = 0.0f;
			
		}
		for (int i18 = 0; (i18 < 2); i18 = (i18 + 1)) {
			fRec21[i18] = 0.0f;
			
		}
		for (int i19 = 0; (i19 < 2); i19 = (i19 + 1)) {
			fRec23[i19] = 0.0f;
			
		}
		for (int i20 = 0; (i20 < 2); i20 = (i20 + 1)) {
			fRec24[i20] = 0.0f;
			
		}
		for (int i21 = 0; (i21 < 2); i21 = (i21 + 1)) {
			fRec22[i21] = 0.0f;
			
		}
		for (int i22 = 0; (i22 < 2); i22 = (i22 + 1)) {
			fRec19[i22] = 0.0f;
			
		}
		for (int i23 = 0; (i23 < 2); i23 = (i23 + 1)) {
			fRec18[i23] = 0.0f;
			
		}
		for (int i24 = 0; (i24 < 2); i24 = (i24 + 1)) {
			fRec27[i24] = 0.0f;
			
		}
		for (int i25 = 0; (i25 < 2); i25 = (i25 + 1)) {
			fRec28[i25] = 0.0f;
			
		}
		for (int i26 = 0; (i26 < 2); i26 = (i26 + 1)) {
			fRec30[i26] = 0.0f;
			
		}
		for (int i27 = 0; (i27 < 2); i27 = (i27 + 1)) {
			fRec31[i27] = 0.0f;
			
		}
		for (int i28 = 0; (i28 < 2); i28 = (i28 + 1)) {
			fRec29[i28] = 0.0f;
			
		}
		for (int i29 = 0; (i29 < 2); i29 = (i29 + 1)) {
			fRec26[i29] = 0.0f;
			
		}
		for (int i30 = 0; (i30 < 2); i30 = (i30 + 1)) {
			fRec25[i30] = 0.0f;
			
		}
		for (int i31 = 0; (i31 < 2); i31 = (i31 + 1)) {
			fRec35[i31] = 0.0f;
			
		}
		for (int i32 = 0; (i32 < 2); i32 = (i32 + 1)) {
			fRec36[i32] = 0.0f;
			
		}
		for (int i33 = 0; (i33 < 2); i33 = (i33 + 1)) {
			fRec34[i33] = 0.0f;
			
		}
		for (int i34 = 0; (i34 < 2); i34 = (i34 + 1)) {
			fRec37[i34] = 0.0f;
			
		}
		for (int i35 = 0; (i35 < 2); i35 = (i35 + 1)) {
			fRec38[i35] = 0.0f;
			
		}
		for (int i36 = 0; (i36 < 2); i36 = (i36 + 1)) {
			fRec33[i36] = 0.0f;
			
		}
		for (int i37 = 0; (i37 < 2); i37 = (i37 + 1)) {
			fRec32[i37] = 0.0f;
			
		}
		for (int i38 = 0; (i38 < 2); i38 = (i38 + 1)) {
			fRec42[i38] = 0.0f;
			
		}
		for (int i39 = 0; (i39 < 2); i39 = (i39 + 1)) {
			fRec43[i39] = 0.0f;
			
		}
		for (int i40 = 0; (i40 < 2); i40 = (i40 + 1)) {
			fRec41[i40] = 0.0f;
			
		}
		for (int i41 = 0; (i41 < 2); i41 = (i41 + 1)) {
			fRec44[i41] = 0.0f;
			
		}
		for (int i42 = 0; (i42 < 2); i42 = (i42 + 1)) {
			fRec40[i42] = 0.0f;
			
		}
		for (int i43 = 0; (i43 < 2); i43 = (i43 + 1)) {
			fRec39[i43] = 0.0f;
			
		}
		for (int i44 = 0; (i44 < 2); i44 = (i44 + 1)) {
			fRec48[i44] = 0.0f;
			
		}
		for (int i45 = 0; (i45 < 2); i45 = (i45 + 1)) {
			fRec49[i45] = 0.0f;
			
		}
		for (int i46 = 0; (i46 < 2); i46 = (i46 + 1)) {
			fRec47[i46] = 0.0f;
			
		}
		for (int i47 = 0; (i47 < 2); i47 = (i47 + 1)) {
			fRec50[i47] = 0.0f;
			
		}
		for (int i48 = 0; (i48 < 2); i48 = (i48 + 1)) {
			fRec46[i48] = 0.0f;
			
		}
		for (int i49 = 0; (i49 < 2); i49 = (i49 + 1)) {
			fRec45[i49] = 0.0f;
			
		}
		for (int i50 = 0; (i50 < 2); i50 = (i50 + 1)) {
			fRec54[i50] = 0.0f;
			
		}
		for (int i51 = 0; (i51 < 2); i51 = (i51 + 1)) {
			fRec55[i51] = 0.0f;
			
		}
		for (int i52 = 0; (i52 < 2); i52 = (i52 + 1)) {
			fRec53[i52] = 0.0f;
			
		}
		for (int i53 = 0; (i53 < 2); i53 = (i53 + 1)) {
			fRec56[i53] = 0.0f;
			
		}
		for (int i54 = 0; (i54 < 2); i54 = (i54 + 1)) {
			fRec52[i54] = 0.0f;
			
		}
		for (int i55 = 0; (i55 < 2); i55 = (i55 + 1)) {
			fRec51[i55] = 0.0f;
			
		}
		for (int i56 = 0; (i56 < 2); i56 = (i56 + 1)) {
			fRec60[i56] = 0.0f;
			
		}
		for (int i57 = 0; (i57 < 2); i57 = (i57 + 1)) {
			fRec61[i57] = 0.0f;
			
		}
		for (int i58 = 0; (i58 < 2); i58 = (i58 + 1)) {
			fRec59[i58] = 0.0f;
			
		}
		for (int i59 = 0; (i59 < 2); i59 = (i59 + 1)) {
			fRec62[i59] = 0.0f;
			
		}
		for (int i60 = 0; (i60 < 2); i60 = (i60 + 1)) {
			fRec58[i60] = 0.0f;
			
		}
		for (int i61 = 0; (i61 < 2); i61 = (i61 + 1)) {
			fRec57[i61] = 0.0f;
			
		}
		for (int i62 = 0; (i62 < 2); i62 = (i62 + 1)) {
			fRec65[i62] = 0.0f;
			
		}
		for (int i63 = 0; (i63 < 2); i63 = (i63 + 1)) {
			fRec66[i63] = 0.0f;
			
		}
		for (int i64 = 0; (i64 < 2); i64 = (i64 + 1)) {
			fRec64[i64] = 0.0f;
			
		}
		for (int i65 = 0; (i65 < 2); i65 = (i65 + 1)) {
			fRec63[i65] = 0.0f;
			
		}
		for (int i66 = 0; (i66 < 2); i66 = (i66 + 1)) {
			fRec69[i66] = 0.0f;
			
		}
		for (int i67 = 0; (i67 < 2); i67 = (i67 + 1)) {
			fRec70[i67] = 0.0f;
			
		}
		for (int i68 = 0; (i68 < 2); i68 = (i68 + 1)) {
			fRec68[i68] = 0.0f;
			
		}
		for (int i69 = 0; (i69 < 2); i69 = (i69 + 1)) {
			fRec67[i69] = 0.0f;
			
		}
		for (int i70 = 0; (i70 < 2); i70 = (i70 + 1)) {
			fRec73[i70] = 0.0f;
			
		}
		for (int i71 = 0; (i71 < 2); i71 = (i71 + 1)) {
			fRec74[i71] = 0.0f;
			
		}
		for (int i72 = 0; (i72 < 2); i72 = (i72 + 1)) {
			fRec72[i72] = 0.0f;
			
		}
		for (int i73 = 0; (i73 < 2); i73 = (i73 + 1)) {
			fRec71[i73] = 0.0f;
			
		}
		for (int i74 = 0; (i74 < 2); i74 = (i74 + 1)) {
			fRec77[i74] = 0.0f;
			
		}
		for (int i75 = 0; (i75 < 2); i75 = (i75 + 1)) {
			fRec76[i75] = 0.0f;
			
		}
		for (int i76 = 0; (i76 < 2); i76 = (i76 + 1)) {
			fRec75[i76] = 0.0f;
			
		}
		for (int i77 = 0; (i77 < 2); i77 = (i77 + 1)) {
			fRec80[i77] = 0.0f;
			
		}
		for (int i78 = 0; (i78 < 2); i78 = (i78 + 1)) {
			fRec79[i78] = 0.0f;
			
		}
		for (int i79 = 0; (i79 < 2); i79 = (i79 + 1)) {
			fRec78[i79] = 0.0f;
			
		}
		for (int i80 = 0; (i80 < 2); i80 = (i80 + 1)) {
			fRec0[i80] = 0.0f;
			
		}
		for (int i81 = 0; (i81 < 2); i81 = (i81 + 1)) {
			fRec81[i81] = 0.0f;
			
		}
		for (int i82 = 0; (i82 < 2); i82 = (i82 + 1)) {
			fRec85[i82] = 0.0f;
			
		}
		for (int i83 = 0; (i83 < 2); i83 = (i83 + 1)) {
			fRec84[i83] = 0.0f;
			
		}
		for (int i84 = 0; (i84 < 2); i84 = (i84 + 1)) {
			fRec83[i84] = 0.0f;
			
		}
		for (int i85 = 0; (i85 < 2); i85 = (i85 + 1)) {
			fRec88[i85] = 0.0f;
			
		}
		for (int i86 = 0; (i86 < 2); i86 = (i86 + 1)) {
			fRec89[i86] = 0.0f;
			
		}
		for (int i87 = 0; (i87 < 2); i87 = (i87 + 1)) {
			fRec87[i87] = 0.0f;
			
		}
		for (int i88 = 0; (i88 < 2); i88 = (i88 + 1)) {
			fRec86[i88] = 0.0f;
			
		}
		for (int i89 = 0; (i89 < 2); i89 = (i89 + 1)) {
			fRec92[i89] = 0.0f;
			
		}
		for (int i90 = 0; (i90 < 2); i90 = (i90 + 1)) {
			fRec93[i90] = 0.0f;
			
		}
		for (int i91 = 0; (i91 < 2); i91 = (i91 + 1)) {
			fRec95[i91] = 0.0f;
			
		}
		for (int i92 = 0; (i92 < 2); i92 = (i92 + 1)) {
			fRec96[i92] = 0.0f;
			
		}
		for (int i93 = 0; (i93 < 2); i93 = (i93 + 1)) {
			fRec94[i93] = 0.0f;
			
		}
		for (int i94 = 0; (i94 < 2); i94 = (i94 + 1)) {
			fRec91[i94] = 0.0f;
			
		}
		for (int i95 = 0; (i95 < 2); i95 = (i95 + 1)) {
			fRec90[i95] = 0.0f;
			
		}
		for (int i96 = 0; (i96 < 2); i96 = (i96 + 1)) {
			fRec100[i96] = 0.0f;
			
		}
		for (int i97 = 0; (i97 < 2); i97 = (i97 + 1)) {
			fRec101[i97] = 0.0f;
			
		}
		for (int i98 = 0; (i98 < 2); i98 = (i98 + 1)) {
			fRec99[i98] = 0.0f;
			
		}
		for (int i99 = 0; (i99 < 2); i99 = (i99 + 1)) {
			fRec102[i99] = 0.0f;
			
		}
		for (int i100 = 0; (i100 < 2); i100 = (i100 + 1)) {
			fRec98[i100] = 0.0f;
			
		}
		for (int i101 = 0; (i101 < 2); i101 = (i101 + 1)) {
			fRec97[i101] = 0.0f;
			
		}
		for (int i102 = 0; (i102 < 2); i102 = (i102 + 1)) {
			fRec106[i102] = 0.0f;
			
		}
		for (int i103 = 0; (i103 < 2); i103 = (i103 + 1)) {
			fRec107[i103] = 0.0f;
			
		}
		for (int i104 = 0; (i104 < 2); i104 = (i104 + 1)) {
			fRec105[i104] = 0.0f;
			
		}
		for (int i105 = 0; (i105 < 2); i105 = (i105 + 1)) {
			fRec108[i105] = 0.0f;
			
		}
		for (int i106 = 0; (i106 < 2); i106 = (i106 + 1)) {
			fRec104[i106] = 0.0f;
			
		}
		for (int i107 = 0; (i107 < 2); i107 = (i107 + 1)) {
			fRec103[i107] = 0.0f;
			
		}
		for (int i108 = 0; (i108 < 2); i108 = (i108 + 1)) {
			fRec111[i108] = 0.0f;
			
		}
		for (int i109 = 0; (i109 < 2); i109 = (i109 + 1)) {
			fRec112[i109] = 0.0f;
			
		}
		for (int i110 = 0; (i110 < 2); i110 = (i110 + 1)) {
			fRec114[i110] = 0.0f;
			
		}
		for (int i111 = 0; (i111 < 2); i111 = (i111 + 1)) {
			fRec115[i111] = 0.0f;
			
		}
		for (int i112 = 0; (i112 < 2); i112 = (i112 + 1)) {
			fRec113[i112] = 0.0f;
			
		}
		for (int i113 = 0; (i113 < 2); i113 = (i113 + 1)) {
			fRec110[i113] = 0.0f;
			
		}
		for (int i114 = 0; (i114 < 2); i114 = (i114 + 1)) {
			fRec109[i114] = 0.0f;
			
		}
		for (int i115 = 0; (i115 < 2); i115 = (i115 + 1)) {
			fRec82[i115] = 0.0f;
			
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
			fRec123[i120] = 0.0f;
			
		}
		for (int i121 = 0; (i121 < 2); i121 = (i121 + 1)) {
			fRec124[i121] = 0.0f;
			
		}
		for (int i122 = 0; (i122 < 2); i122 = (i122 + 1)) {
			fRec126[i122] = 0.0f;
			
		}
		for (int i123 = 0; (i123 < 2); i123 = (i123 + 1)) {
			fRec127[i123] = 0.0f;
			
		}
		for (int i124 = 0; (i124 < 2); i124 = (i124 + 1)) {
			fRec125[i124] = 0.0f;
			
		}
		for (int i125 = 0; (i125 < 2); i125 = (i125 + 1)) {
			fRec122[i125] = 0.0f;
			
		}
		for (int i126 = 0; (i126 < 2); i126 = (i126 + 1)) {
			fRec121[i126] = 0.0f;
			
		}
		for (int i127 = 0; (i127 < 2); i127 = (i127 + 1)) {
			fRec131[i127] = 0.0f;
			
		}
		for (int i128 = 0; (i128 < 2); i128 = (i128 + 1)) {
			fRec132[i128] = 0.0f;
			
		}
		for (int i129 = 0; (i129 < 2); i129 = (i129 + 1)) {
			fRec130[i129] = 0.0f;
			
		}
		for (int i130 = 0; (i130 < 2); i130 = (i130 + 1)) {
			fRec133[i130] = 0.0f;
			
		}
		for (int i131 = 0; (i131 < 2); i131 = (i131 + 1)) {
			fRec129[i131] = 0.0f;
			
		}
		for (int i132 = 0; (i132 < 2); i132 = (i132 + 1)) {
			fRec128[i132] = 0.0f;
			
		}
		for (int i133 = 0; (i133 < 2); i133 = (i133 + 1)) {
			fRec136[i133] = 0.0f;
			
		}
		for (int i134 = 0; (i134 < 2); i134 = (i134 + 1)) {
			fRec137[i134] = 0.0f;
			
		}
		for (int i135 = 0; (i135 < 2); i135 = (i135 + 1)) {
			fRec135[i135] = 0.0f;
			
		}
		for (int i136 = 0; (i136 < 2); i136 = (i136 + 1)) {
			fRec134[i136] = 0.0f;
			
		}
		for (int i137 = 0; (i137 < 2); i137 = (i137 + 1)) {
			fRec140[i137] = 0.0f;
			
		}
		for (int i138 = 0; (i138 < 2); i138 = (i138 + 1)) {
			fRec141[i138] = 0.0f;
			
		}
		for (int i139 = 0; (i139 < 2); i139 = (i139 + 1)) {
			fRec143[i139] = 0.0f;
			
		}
		for (int i140 = 0; (i140 < 2); i140 = (i140 + 1)) {
			fRec144[i140] = 0.0f;
			
		}
		for (int i141 = 0; (i141 < 2); i141 = (i141 + 1)) {
			fRec142[i141] = 0.0f;
			
		}
		for (int i142 = 0; (i142 < 2); i142 = (i142 + 1)) {
			fRec139[i142] = 0.0f;
			
		}
		for (int i143 = 0; (i143 < 2); i143 = (i143 + 1)) {
			fRec138[i143] = 0.0f;
			
		}
		for (int i144 = 0; (i144 < 2); i144 = (i144 + 1)) {
			fRec120[i144] = 0.0f;
			
		}
		for (int i145 = 0; (i145 < 2); i145 = (i145 + 1)) {
			fRec145[i145] = 0.0f;
			
		}
		for (int i146 = 0; (i146 < 2); i146 = (i146 + 1)) {
			fRec146[i146] = 0.0f;
			
		}
		for (int i147 = 0; (i147 < 2); i147 = (i147 + 1)) {
			fRec147[i147] = 0.0f;
			
		}
		for (int i148 = 0; (i148 < 2); i148 = (i148 + 1)) {
			fRec148[i148] = 0.0f;
			
		}
		for (int i149 = 0; (i149 < 2); i149 = (i149 + 1)) {
			fRec149[i149] = 0.0f;
			
		}
		for (int i150 = 0; (i150 < 2); i150 = (i150 + 1)) {
			fRec150[i150] = 0.0f;
			
		}
		for (int i151 = 0; (i151 < 2); i151 = (i151 + 1)) {
			fRec151[i151] = 0.0f;
			
		}
		for (int i152 = 0; (i152 < 2); i152 = (i152 + 1)) {
			fRec152[i152] = 0.0f;
			
		}
		for (int i153 = 0; (i153 < 2); i153 = (i153 + 1)) {
			fRec153[i153] = 0.0f;
			
		}
		for (int i154 = 0; (i154 < 2); i154 = (i154 + 1)) {
			fRec154[i154] = 0.0f;
			
		}
		for (int i155 = 0; (i155 < 2); i155 = (i155 + 1)) {
			fRec155[i155] = 0.0f;
			
		}
		for (int i156 = 0; (i156 < 2); i156 = (i156 + 1)) {
			fRec156[i156] = 0.0f;
			
		}
		for (int i157 = 0; (i157 < 2); i157 = (i157 + 1)) {
			fRec157[i157] = 0.0f;
			
		}
		for (int i158 = 0; (i158 < 2); i158 = (i158 + 1)) {
			fRec158[i158] = 0.0f;
			
		}
		for (int i159 = 0; (i159 < 2); i159 = (i159 + 1)) {
			fRec159[i159] = 0.0f;
			
		}
		for (int i160 = 0; (i160 < 2); i160 = (i160 + 1)) {
			fRec160[i160] = 0.0f;
			
		}
		for (int i161 = 0; (i161 < 2); i161 = (i161 + 1)) {
			fRec161[i161] = 0.0f;
			
		}
		for (int i162 = 0; (i162 < 2); i162 = (i162 + 1)) {
			fRec162[i162] = 0.0f;
			
		}
		for (int i163 = 0; (i163 < 2); i163 = (i163 + 1)) {
			fRec163[i163] = 0.0f;
			
		}
		for (int i164 = 0; (i164 < 2); i164 = (i164 + 1)) {
			fRec164[i164] = 0.0f;
			
		}
		for (int i165 = 0; (i165 < 2); i165 = (i165 + 1)) {
			fRec165[i165] = 0.0f;
			
		}
		for (int i166 = 0; (i166 < 2); i166 = (i166 + 1)) {
			fRec166[i166] = 0.0f;
			
		}
		for (int i167 = 0; (i167 < 2); i167 = (i167 + 1)) {
			fRec167[i167] = 0.0f;
			
		}
		for (int i168 = 0; (i168 < 2); i168 = (i168 + 1)) {
			fRec168[i168] = 0.0f;
			
		}
		for (int i169 = 0; (i169 < 2); i169 = (i169 + 1)) {
			fRec169[i169] = 0.0f;
			
		}
		for (int i170 = 0; (i170 < 2); i170 = (i170 + 1)) {
			fRec170[i170] = 0.0f;
			
		}
		for (int i171 = 0; (i171 < 2); i171 = (i171 + 1)) {
			fRec171[i171] = 0.0f;
			
		}
		for (int i172 = 0; (i172 < 2); i172 = (i172 + 1)) {
			fRec172[i172] = 0.0f;
			
		}
		for (int i173 = 0; (i173 < 2); i173 = (i173 + 1)) {
			fRec173[i173] = 0.0f;
			
		}
		for (int i174 = 0; (i174 < 2); i174 = (i174 + 1)) {
			fRec174[i174] = 0.0f;
			
		}
		for (int i175 = 0; (i175 < 2); i175 = (i175 + 1)) {
			fRec175[i175] = 0.0f;
			
		}
		for (int i176 = 0; (i176 < 2); i176 = (i176 + 1)) {
			fRec176[i176] = 0.0f;
			
		}
		for (int i177 = 0; (i177 < 2); i177 = (i177 + 1)) {
			fRec177[i177] = 0.0f;
			
		}
		for (int i178 = 0; (i178 < 2); i178 = (i178 + 1)) {
			fRec178[i178] = 0.0f;
			
		}
		for (int i179 = 0; (i179 < 2); i179 = (i179 + 1)) {
			fRec179[i179] = 0.0f;
			
		}
		for (int i180 = 0; (i180 < 2); i180 = (i180 + 1)) {
			fRec180[i180] = 0.0f;
			
		}
		for (int i181 = 0; (i181 < 2); i181 = (i181 + 1)) {
			fRec181[i181] = 0.0f;
			
		}
		for (int i182 = 0; (i182 < 2); i182 = (i182 + 1)) {
			fRec182[i182] = 0.0f;
			
		}
		for (int i183 = 0; (i183 < 2); i183 = (i183 + 1)) {
			fRec183[i183] = 0.0f;
			
		}
		for (int i184 = 0; (i184 < 2); i184 = (i184 + 1)) {
			fRec184[i184] = 0.0f;
			
		}
		for (int i185 = 0; (i185 < 2); i185 = (i185 + 1)) {
			fRec185[i185] = 0.0f;
			
		}
		for (int i186 = 0; (i186 < 2); i186 = (i186 + 1)) {
			fRec186[i186] = 0.0f;
			
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
		ui_interface->addVerticalBargraph("0x2cdabb0", &fVbargraph0, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("1");
		ui_interface->openVerticalBox("1");
		ui_interface->declare(&fVbargraph17, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2dc2a40", &fVbargraph17, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("2");
		ui_interface->declare(&fVbargraph13, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2da0da0", &fVbargraph13, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("3");
		ui_interface->declare(&fVbargraph14, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2daa560", &fVbargraph14, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2");
		ui_interface->openVerticalBox("4");
		ui_interface->declare(&fVbargraph18, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2dcdbe0", &fVbargraph18, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("5");
		ui_interface->declare(&fVbargraph30, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2e60200", &fVbargraph30, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("6");
		ui_interface->declare(&fVbargraph12, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2d965f0", &fVbargraph12, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("7");
		ui_interface->declare(&fVbargraph11, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2d8b300", &fVbargraph11, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("8");
		ui_interface->declare(&fVbargraph10, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2d7fcf0", &fVbargraph10, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3");
		ui_interface->openVerticalBox("9");
		ui_interface->declare(&fVbargraph21, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2dfbfa0", &fVbargraph21, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("10");
		ui_interface->declare(&fVbargraph29, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2e54c00", &fVbargraph29, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("11");
		ui_interface->declare(&fVbargraph20, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2ded620", &fVbargraph20, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("12");
		ui_interface->declare(&fVbargraph9, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2d72070", &fVbargraph9, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("13");
		ui_interface->declare(&fVbargraph8, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2d63920", &fVbargraph8, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("14");
		ui_interface->declare(&fVbargraph7, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2d54f90", &fVbargraph7, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("15");
		ui_interface->declare(&fVbargraph6, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2d46520", &fVbargraph6, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4");
		ui_interface->openVerticalBox("16");
		ui_interface->declare(&fVbargraph22, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2e0c940", &fVbargraph22, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("17");
		ui_interface->declare(&fVbargraph31, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2e74cb0", &fVbargraph31, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("18");
		ui_interface->declare(&fVbargraph19, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2ddedd0", &fVbargraph19, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("19");
		ui_interface->declare(&fVbargraph28, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2e45e10", &fVbargraph28, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("20");
		ui_interface->declare(&fVbargraph5, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2d33ba0", &fVbargraph5, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("21");
		ui_interface->declare(&fVbargraph4, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2d23840", &fVbargraph4, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("22");
		ui_interface->declare(&fVbargraph3, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2d13060", &fVbargraph3, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("23");
		ui_interface->declare(&fVbargraph2, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2d02880", &fVbargraph2, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("24");
		ui_interface->declare(&fVbargraph1, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2cf1fc0", &fVbargraph1, -70.0f, 6.0f);
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
		ui_interface->declare(&fVbargraph15, "osc", "/output1");
		ui_interface->declare(&fVbargraph15, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2db0c80", &fVbargraph15, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("2");
		ui_interface->declare(&fVbargraph16, "osc", "/output2");
		ui_interface->declare(&fVbargraph16, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2db9550", &fVbargraph16, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("3");
		ui_interface->declare(&fVbargraph23, "osc", "/output3");
		ui_interface->declare(&fVbargraph23, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2e12c30", &fVbargraph23, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("4");
		ui_interface->declare(&fVbargraph24, "osc", "/output4");
		ui_interface->declare(&fVbargraph24, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2e1a950", &fVbargraph24, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("5");
		ui_interface->declare(&fVbargraph25, "osc", "/output5");
		ui_interface->declare(&fVbargraph25, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2e22750", &fVbargraph25, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("6");
		ui_interface->declare(&fVbargraph26, "osc", "/output6");
		ui_interface->declare(&fVbargraph26, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2e291d0", &fVbargraph26, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("7");
		ui_interface->declare(&fVbargraph27, "osc", "/output7");
		ui_interface->declare(&fVbargraph27, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2e353c0", &fVbargraph27, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("8");
		ui_interface->declare(&fVbargraph32, "osc", "/output8");
		ui_interface->declare(&fVbargraph32, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2e7be60", &fVbargraph32, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("9");
		ui_interface->declare(&fVbargraph33, "osc", "/output9");
		ui_interface->declare(&fVbargraph33, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2e87250", &fVbargraph33, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("10");
		ui_interface->declare(&fVbargraph34, "osc", "/output10");
		ui_interface->declare(&fVbargraph34, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2e928f0", &fVbargraph34, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("11");
		ui_interface->declare(&fVbargraph35, "osc", "/output11");
		ui_interface->declare(&fVbargraph35, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2e9cd60", &fVbargraph35, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("12");
		ui_interface->declare(&fVbargraph36, "osc", "/output12");
		ui_interface->declare(&fVbargraph36, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2ea53b0", &fVbargraph36, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("13");
		ui_interface->declare(&fVbargraph37, "osc", "/output13");
		ui_interface->declare(&fVbargraph37, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2ead500", &fVbargraph37, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("14");
		ui_interface->declare(&fVbargraph38, "osc", "/output14");
		ui_interface->declare(&fVbargraph38, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2eb5af0", &fVbargraph38, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("15");
		ui_interface->declare(&fVbargraph39, "osc", "/output15");
		ui_interface->declare(&fVbargraph39, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2ebcb90", &fVbargraph39, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("16");
		ui_interface->declare(&fVbargraph40, "osc", "/output16");
		ui_interface->declare(&fVbargraph40, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2ec4df0", &fVbargraph40, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("17");
		ui_interface->declare(&fVbargraph41, "osc", "/output17");
		ui_interface->declare(&fVbargraph41, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2eccf50", &fVbargraph41, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("18");
		ui_interface->declare(&fVbargraph42, "osc", "/output18");
		ui_interface->declare(&fVbargraph42, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2ed5550", &fVbargraph42, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("19");
		ui_interface->declare(&fVbargraph43, "osc", "/output19");
		ui_interface->declare(&fVbargraph43, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2ee6280", &fVbargraph43, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("20");
		ui_interface->declare(&fVbargraph44, "osc", "/output20");
		ui_interface->declare(&fVbargraph44, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2ef3020", &fVbargraph44, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("21");
		ui_interface->declare(&fVbargraph45, "osc", "/output21");
		ui_interface->declare(&fVbargraph45, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2eff630", &fVbargraph45, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("22");
		ui_interface->declare(&fVbargraph46, "osc", "/output22");
		ui_interface->declare(&fVbargraph46, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2f0bba0", &fVbargraph46, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("23");
		ui_interface->declare(&fVbargraph47, "osc", "/output23");
		ui_interface->declare(&fVbargraph47, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2f14630", &fVbargraph47, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("24");
		ui_interface->declare(&fVbargraph48, "osc", "/output24");
		ui_interface->declare(&fVbargraph48, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2f1cdb0", &fVbargraph48, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("25");
		ui_interface->declare(&fVbargraph49, "osc", "/output25");
		ui_interface->declare(&fVbargraph49, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2f25bb0", &fVbargraph49, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Outputs 26-50");
		ui_interface->openVerticalBox("26");
		ui_interface->declare(&fVbargraph50, "osc", "/output26");
		ui_interface->declare(&fVbargraph50, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2f2ea70", &fVbargraph50, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("27");
		ui_interface->declare(&fVbargraph51, "osc", "/output27");
		ui_interface->declare(&fVbargraph51, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2f3ee50", &fVbargraph51, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("28");
		ui_interface->declare(&fVbargraph52, "osc", "/output28");
		ui_interface->declare(&fVbargraph52, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2f49630", &fVbargraph52, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("29");
		ui_interface->declare(&fVbargraph53, "osc", "/output29");
		ui_interface->declare(&fVbargraph53, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2f563a0", &fVbargraph53, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("30");
		ui_interface->declare(&fVbargraph54, "osc", "/output30");
		ui_interface->declare(&fVbargraph54, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2f62a70", &fVbargraph54, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("31");
		ui_interface->declare(&fVbargraph55, "osc", "/output31");
		ui_interface->declare(&fVbargraph55, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2f736b0", &fVbargraph55, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("32");
		ui_interface->declare(&fVbargraph56, "osc", "/output32");
		ui_interface->declare(&fVbargraph56, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2f82510", &fVbargraph56, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("33");
		ui_interface->declare(&fVbargraph57, "osc", "/output33");
		ui_interface->declare(&fVbargraph57, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2f904e0", &fVbargraph57, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("34");
		ui_interface->declare(&fVbargraph58, "osc", "/output34");
		ui_interface->declare(&fVbargraph58, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2f9d270", &fVbargraph58, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("35");
		ui_interface->declare(&fVbargraph59, "osc", "/output35");
		ui_interface->declare(&fVbargraph59, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2fab050", &fVbargraph59, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("36");
		ui_interface->declare(&fVbargraph60, "osc", "/output36");
		ui_interface->declare(&fVbargraph60, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2fb8350", &fVbargraph60, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("37");
		ui_interface->declare(&fVbargraph61, "osc", "/output37");
		ui_interface->declare(&fVbargraph61, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2fc60e0", &fVbargraph61, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("38");
		ui_interface->declare(&fVbargraph62, "osc", "/output38");
		ui_interface->declare(&fVbargraph62, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2fd5640", &fVbargraph62, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("39");
		ui_interface->declare(&fVbargraph63, "osc", "/output39");
		ui_interface->declare(&fVbargraph63, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2fddd70", &fVbargraph63, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("40");
		ui_interface->declare(&fVbargraph64, "osc", "/output40");
		ui_interface->declare(&fVbargraph64, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2fe6190", &fVbargraph64, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("41");
		ui_interface->declare(&fVbargraph65, "osc", "/output41");
		ui_interface->declare(&fVbargraph65, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2fee870", &fVbargraph65, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("42");
		ui_interface->declare(&fVbargraph66, "osc", "/output42");
		ui_interface->declare(&fVbargraph66, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2ff7060", &fVbargraph66, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("43");
		ui_interface->declare(&fVbargraph67, "osc", "/output43");
		ui_interface->declare(&fVbargraph67, "unit", "dB");
		ui_interface->addVerticalBargraph("0x2fff540", &fVbargraph67, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("44");
		ui_interface->declare(&fVbargraph68, "osc", "/output44");
		ui_interface->declare(&fVbargraph68, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3007ec0", &fVbargraph68, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("45");
		ui_interface->declare(&fVbargraph69, "osc", "/output45");
		ui_interface->declare(&fVbargraph69, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3010740", &fVbargraph69, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("46");
		ui_interface->declare(&fVbargraph70, "osc", "/output46");
		ui_interface->declare(&fVbargraph70, "unit", "dB");
		ui_interface->addVerticalBargraph("0x30190a0", &fVbargraph70, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("47");
		ui_interface->declare(&fVbargraph71, "osc", "/output47");
		ui_interface->declare(&fVbargraph71, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3021900", &fVbargraph71, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("48");
		ui_interface->declare(&fVbargraph72, "osc", "/output48");
		ui_interface->declare(&fVbargraph72, "unit", "dB");
		ui_interface->addVerticalBargraph("0x302a240", &fVbargraph72, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("49");
		ui_interface->declare(&fVbargraph73, "osc", "/output49");
		ui_interface->declare(&fVbargraph73, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3033020", &fVbargraph73, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("50");
		ui_interface->declare(&fVbargraph74, "osc", "/output50");
		ui_interface->declare(&fVbargraph74, "unit", "dB");
		ui_interface->addVerticalBargraph("0x303bee0", &fVbargraph74, -70.0f, 6.0f);
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
		FAUSTFLOAT* input16 = inputs[16];
		FAUSTFLOAT* input17 = inputs[17];
		FAUSTFLOAT* input18 = inputs[18];
		FAUSTFLOAT* input19 = inputs[19];
		FAUSTFLOAT* input20 = inputs[20];
		FAUSTFLOAT* input21 = inputs[21];
		FAUSTFLOAT* input22 = inputs[22];
		FAUSTFLOAT* input23 = inputs[23];
		FAUSTFLOAT* input24 = inputs[24];
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
		float fSlow7 = (1.0f / (((fConst2 / fSlow1) + (9.140131f * fSlow6)) + 1.0f));
		float fSlow8 = (36.560524f * fSlow6);
		float fSlow9 = (fSlow8 + (fConst4 / fSlow1));
		float fSlow10 = (1.0f / (((fConst5 / fSlow1) + (11.4878006f * fSlow6)) + 1.0f));
		float fSlow11 = (45.9512024f * fSlow6);
		float fSlow12 = (fSlow11 + (fConst6 / fSlow1));
		float fSlow13 = (fSlow2 * fSlow1);
		float fSlow14 = (1.0f - fSlow2);
		float fSlow15 = (1.0f / ((fConst7 / fSlow1) + 1.0f));
		float fSlow16 = (1.0f / (((fConst8 / fSlow1) + (6.4594326f * fSlow6)) + 1.0f));
		float fSlow17 = (25.8377304f * fSlow6);
		float fSlow18 = (fSlow17 + (fConst9 / fSlow1));
		float fSlow19 = (fConst10 / fSlow1);
		float fSlow20 = (1.0f / (((fConst11 / fSlow1) + (3.0f * fSlow6)) + 1.0f));
		float fSlow21 = (12.0f * fSlow6);
		float fSlow22 = (fSlow21 + (fConst12 / fSlow1));
		float fSlow23 = (1.0f / (fSlow5 + 1.0f));
		float fSlow24 = (fConst13 / fSlow1);
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec1[0] = (fSlow0 + (0.999000013f * fRec1[1]));
			fRec3[0] = (fSlow4 + (0.999000013f * fRec3[1]));
			float fTemp0 = (fSlow3 * (fRec3[0] * float(input0[i])));
			fRec2[0] = max((fRec2[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp0))))));
			fVbargraph0 = FAUSTFLOAT(fRec2[0]);
			float fTemp1 = (0.0126984129f * fTemp0);
			fRec6[0] = (fRec5[1] + fRec6[1]);
			fRec7[0] = (fRec6[1] + fRec7[1]);
			fRec9[0] = (fRec8[1] + fRec9[1]);
			fRec10[0] = (fRec9[1] + fRec10[1]);
			float fTemp2 = (fRec3[0] * float(input24[i]));
			fRec8[0] = (0.0f - (fSlow10 * (((fSlow12 * fRec9[0]) + (fSlow11 * fRec10[0])) - (fSlow13 * fTemp2))));
			fRec5[0] = (0.0f - (fSlow7 * (((fSlow9 * fRec6[0]) + (fSlow8 * fRec7[0])) - fRec8[0])));
			float fTemp3 = (fRec5[0] + (fSlow14 * fTemp2));
			fRec4[0] = max((fRec4[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp3))))));
			fVbargraph1 = FAUSTFLOAT(fRec4[0]);
			float fTemp4 = (0.0f * fTemp3);
			fRec13[0] = (fRec12[1] + fRec13[1]);
			fRec14[0] = (fRec13[1] + fRec14[1]);
			fRec16[0] = (fRec15[1] + fRec16[1]);
			fRec17[0] = (fRec16[1] + fRec17[1]);
			float fTemp5 = (fRec3[0] * float(input23[i]));
			fRec15[0] = (0.0f - (fSlow10 * (((fSlow12 * fRec16[0]) + (fSlow11 * fRec17[0])) - (fSlow13 * fTemp5))));
			fRec12[0] = (0.0f - (fSlow7 * (((fSlow9 * fRec13[0]) + (fSlow8 * fRec14[0])) - fRec15[0])));
			float fTemp6 = (fRec12[0] + (fSlow14 * fTemp5));
			fRec11[0] = max((fRec11[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp6))))));
			fVbargraph2 = FAUSTFLOAT(fRec11[0]);
			float fTemp7 = (0.0f * fTemp6);
			fRec20[0] = (fRec19[1] + fRec20[1]);
			fRec21[0] = (fRec20[1] + fRec21[1]);
			fRec23[0] = (fRec22[1] + fRec23[1]);
			fRec24[0] = (fRec23[1] + fRec24[1]);
			float fTemp8 = (fRec3[0] * float(input22[i]));
			fRec22[0] = (0.0f - (fSlow10 * (((fSlow12 * fRec23[0]) + (fSlow11 * fRec24[0])) - (fSlow13 * fTemp8))));
			fRec19[0] = (0.0f - (fSlow7 * (((fSlow9 * fRec20[0]) + (fSlow8 * fRec21[0])) - fRec22[0])));
			float fTemp9 = (fRec19[0] + (fSlow14 * fTemp8));
			fRec18[0] = max((fRec18[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp9))))));
			fVbargraph3 = FAUSTFLOAT(fRec18[0]);
			float fTemp10 = (4.79080666e-34f * fTemp9);
			fRec27[0] = (fRec26[1] + fRec27[1]);
			fRec28[0] = (fRec27[1] + fRec28[1]);
			fRec30[0] = (fRec29[1] + fRec30[1]);
			fRec31[0] = (fRec30[1] + fRec31[1]);
			float fTemp11 = (fRec3[0] * float(input21[i]));
			fRec29[0] = (0.0f - (fSlow10 * (((fSlow12 * fRec30[0]) + (fSlow11 * fRec31[0])) - (fSlow13 * fTemp11))));
			fRec26[0] = (0.0f - (fSlow7 * (((fSlow9 * fRec27[0]) + (fSlow8 * fRec28[0])) - fRec29[0])));
			float fTemp12 = (fRec26[0] + (fSlow14 * fTemp11));
			fRec25[0] = max((fRec25[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp12))))));
			fVbargraph4 = FAUSTFLOAT(fRec25[0]);
			float fTemp13 = (7.37652042e-18f * fTemp12);
			float fTemp14 = (fRec3[0] * float(input20[i]));
			fRec35[0] = (fRec34[1] + fRec35[1]);
			fRec36[0] = (fRec35[1] + fRec36[1]);
			fRec34[0] = (fSlow10 * ((fSlow13 * fTemp14) - ((fSlow12 * fRec35[0]) + (fSlow11 * fRec36[0]))));
			fRec37[0] = (fRec33[1] + fRec37[1]);
			fRec38[0] = (fRec37[1] + fRec38[1]);
			fRec33[0] = (fSlow7 * (fRec34[0] - ((fSlow9 * fRec37[0]) + (fSlow8 * fRec38[0]))));
			float fTemp15 = (fRec33[0] + (fSlow14 * fTemp14));
			fRec32[0] = max((fRec32[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp15))))));
			fVbargraph5 = FAUSTFLOAT(fRec32[0]);
			float fTemp16 = (0.0380952395f * fTemp15);
			fRec42[0] = (fRec41[1] + fRec42[1]);
			fRec43[0] = (fRec42[1] + fRec43[1]);
			float fTemp17 = (fRec3[0] * float(input15[i]));
			fRec41[0] = (0.0f - (fSlow16 * (((fSlow18 * fRec42[0]) + (fSlow17 * fRec43[0])) - (fSlow13 * fTemp17))));
			fRec44[0] = (fRec40[1] + fRec44[1]);
			fRec40[0] = (fSlow15 * (fRec41[0] - (fSlow19 * fRec44[0])));
			float fTemp18 = (fRec40[0] + (fSlow14 * fTemp17));
			fRec39[0] = max((fRec39[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp18))))));
			fVbargraph6 = FAUSTFLOAT(fRec39[0]);
			float fTemp19 = (0.0f * fTemp18);
			fRec48[0] = (fRec47[1] + fRec48[1]);
			fRec49[0] = (fRec48[1] + fRec49[1]);
			float fTemp20 = (fRec3[0] * float(input14[i]));
			fRec47[0] = (0.0f - (fSlow16 * (((fSlow18 * fRec48[0]) + (fSlow17 * fRec49[0])) - (fSlow13 * fTemp20))));
			fRec50[0] = (fRec46[1] + fRec50[1]);
			fRec46[0] = (fSlow15 * (fRec47[0] - (fSlow19 * fRec50[0])));
			float fTemp21 = (fRec46[0] + (fSlow14 * fTemp20));
			fRec45[0] = max((fRec45[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp21))))));
			fVbargraph7 = FAUSTFLOAT(fRec45[0]);
			float fTemp22 = (2.43935947e-34f * fTemp21);
			fRec54[0] = (fRec53[1] + fRec54[1]);
			fRec55[0] = (fRec54[1] + fRec55[1]);
			float fTemp23 = (fRec3[0] * float(input13[i]));
			fRec53[0] = (0.0f - (fSlow16 * (((fSlow18 * fRec54[0]) + (fSlow17 * fRec55[0])) - (fSlow13 * fTemp23))));
			fRec56[0] = (fRec52[1] + fRec56[1]);
			fRec52[0] = (fSlow15 * (fRec53[0] - (fSlow19 * fRec56[0])));
			float fTemp24 = (fRec52[0] + (fSlow14 * fTemp23));
			fRec51[0] = max((fRec51[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp24))))));
			fVbargraph8 = FAUSTFLOAT(fRec51[0]);
			float fTemp25 = (5.03912267e-18f * fTemp24);
			float fTemp26 = (fRec3[0] * float(input12[i]));
			fRec60[0] = (fRec59[1] + fRec60[1]);
			fRec61[0] = (fRec60[1] + fRec61[1]);
			fRec59[0] = (fSlow16 * ((fSlow13 * fTemp26) - ((fSlow18 * fRec60[0]) + (fSlow17 * fRec61[0]))));
			fRec62[0] = (fRec58[1] + fRec62[1]);
			fRec58[0] = (fSlow15 * (fRec59[0] - (fSlow19 * fRec62[0])));
			float fTemp27 = (fRec58[0] + (fSlow14 * fTemp26));
			fRec57[0] = max((fRec57[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp27))))));
			fVbargraph9 = FAUSTFLOAT(fRec57[0]);
			float fTemp28 = (0.0335968435f * fTemp27);
			fRec65[0] = (fRec64[1] + fRec65[1]);
			fRec66[0] = (fRec65[1] + fRec66[1]);
			float fTemp29 = (fRec3[0] * float(input8[i]));
			fRec64[0] = (0.0f - (fSlow20 * (((fSlow22 * fRec65[0]) + (fSlow21 * fRec66[0])) - (fSlow13 * fTemp29))));
			float fTemp30 = (fRec64[0] + (fSlow14 * fTemp29));
			fRec63[0] = max((fRec63[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp30))))));
			fVbargraph10 = FAUSTFLOAT(fRec63[0]);
			float fTemp31 = (9.21991188e-35f * fTemp30);
			fRec69[0] = (fRec68[1] + fRec69[1]);
			fRec70[0] = (fRec69[1] + fRec70[1]);
			float fTemp32 = (fRec3[0] * float(input7[i]));
			fRec68[0] = (0.0f - (fSlow20 * (((fSlow22 * fRec69[0]) + (fSlow21 * fRec70[0])) - (fSlow13 * fTemp32))));
			float fTemp33 = (fRec68[0] + (fSlow14 * fTemp32));
			fRec67[0] = max((fRec67[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp33))))));
			fVbargraph11 = FAUSTFLOAT(fRec67[0]);
			float fTemp34 = (3.01145194e-18f * fTemp33);
			float fTemp35 = (fRec3[0] * float(input6[i]));
			fRec73[0] = (fRec72[1] + fRec73[1]);
			fRec74[0] = (fRec73[1] + fRec74[1]);
			fRec72[0] = (fSlow20 * ((fSlow13 * fTemp35) - ((fSlow22 * fRec73[0]) + (fSlow21 * fRec74[0]))));
			float fTemp36 = (fRec72[0] + (fSlow14 * fTemp35));
			fRec71[0] = max((fRec71[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp36))))));
			fVbargraph12 = FAUSTFLOAT(fRec71[0]);
			float fTemp37 = (0.0283945147f * fTemp36);
			float fTemp38 = (fRec3[0] * float(input2[i]));
			fRec77[0] = (fRec76[1] + fRec77[1]);
			fRec76[0] = (fSlow23 * ((fSlow13 * fTemp38) - (fSlow24 * fRec77[0])));
			float fTemp39 = (fRec76[0] + (fSlow14 * fTemp38));
			fRec75[0] = max((fRec75[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp39))))));
			fVbargraph13 = FAUSTFLOAT(fRec75[0]);
			float fTemp40 = (0.0219942965f * fTemp39);
			float fTemp41 = (fRec3[0] * float(input3[i]));
			fRec80[0] = (fRec79[1] + fRec80[1]);
			fRec79[0] = (fSlow23 * ((fSlow13 * fTemp41) - (fSlow24 * fRec80[0])));
			float fTemp42 = (fRec79[0] + (fSlow14 * fTemp41));
			fRec78[0] = max((fRec78[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp42))))));
			fVbargraph14 = FAUSTFLOAT(fRec78[0]);
			float fTemp43 = (1.34676222e-18f * fTemp42);
			float fTemp44 = (fRec1[0] * (fTemp1 + (fTemp4 + (fTemp7 + (fTemp10 + (fTemp13 + (fTemp16 + (fTemp19 + (fTemp22 + (fTemp25 + (fTemp28 + (fTemp31 + (fTemp34 + (fTemp37 + (fTemp40 + fTemp43)))))))))))))));
			fRec0[0] = max((fRec0[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp44))))));
			fVbargraph15 = FAUSTFLOAT(fRec0[0]);
			output0[i] = FAUSTFLOAT(fTemp44);
			float fTemp45 = (0.0281718094f * fTemp3);
			float fTemp46 = (0.0142857144f * fTemp15);
			float fTemp47 = (0.0265606362f * fTemp18);
			float fTemp48 = (0.0219942965f * fTemp42);
			float fTemp49 = (0.0245903712f * fTemp30);
			float fTemp50 = (0.0141972573f * fTemp36);
			float fTemp51 = (0.0205737799f * fTemp24);
			float fTemp52 = (0.0212958846f * fTemp9);
			float fTemp53 = (fRec1[0] * ((fTemp1 + (fTemp45 + (fTemp46 + (fTemp47 + (fTemp48 + fTemp49))))) - ((fTemp50 + fTemp51) + fTemp52)));
			fRec81[0] = max((fRec81[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp53))))));
			fVbargraph16 = FAUSTFLOAT(fRec81[0]);
			output1[i] = FAUSTFLOAT(fTemp53);
			float fTemp54 = (fRec3[0] * float(input1[i]));
			fRec85[0] = (fRec84[1] + fRec85[1]);
			fRec84[0] = (fSlow23 * ((fSlow13 * fTemp54) - (fSlow24 * fRec85[0])));
			float fTemp55 = (fRec84[0] + (fSlow14 * fTemp54));
			fRec83[0] = max((fRec83[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp55))))));
			fVbargraph17 = FAUSTFLOAT(fRec83[0]);
			float fTemp56 = (0.0219942965f * fTemp55);
			fRec88[0] = (fRec87[1] + fRec88[1]);
			fRec89[0] = (fRec88[1] + fRec89[1]);
			float fTemp57 = (fRec3[0] * float(input4[i]));
			fRec87[0] = (0.0f - (fSlow20 * (((fSlow22 * fRec88[0]) + (fSlow21 * fRec89[0])) - (fSlow13 * fTemp57))));
			float fTemp58 = (fRec87[0] + (fSlow14 * fTemp57));
			fRec86[0] = max((fRec86[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp58))))));
			fVbargraph18 = FAUSTFLOAT(fRec86[0]);
			fRec92[0] = (fRec91[1] + fRec92[1]);
			fRec93[0] = (fRec92[1] + fRec93[1]);
			fRec95[0] = (fRec94[1] + fRec95[1]);
			fRec96[0] = (fRec95[1] + fRec96[1]);
			float fTemp59 = (fRec3[0] * float(input18[i]));
			fRec94[0] = (0.0f - (fSlow10 * (((fSlow12 * fRec95[0]) + (fSlow11 * fRec96[0])) - (fSlow13 * fTemp59))));
			fRec91[0] = (0.0f - (fSlow7 * (((fSlow9 * fRec92[0]) + (fSlow8 * fRec93[0])) - fRec94[0])));
			float fTemp60 = (fRec91[0] + (fSlow14 * fTemp59));
			fRec90[0] = max((fRec90[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp60))))));
			fVbargraph19 = FAUSTFLOAT(fRec90[0]);
			fRec100[0] = (fRec99[1] + fRec100[1]);
			fRec101[0] = (fRec100[1] + fRec101[1]);
			float fTemp61 = (fRec3[0] * float(input11[i]));
			fRec99[0] = (0.0f - (fSlow16 * (((fSlow18 * fRec100[0]) + (fSlow17 * fRec101[0])) - (fSlow13 * fTemp61))));
			fRec102[0] = (fRec98[1] + fRec102[1]);
			fRec98[0] = (fSlow15 * (fRec99[0] - (fSlow19 * fRec102[0])));
			float fTemp62 = (fRec98[0] + (fSlow14 * fTemp61));
			fRec97[0] = max((fRec97[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp62))))));
			fVbargraph20 = FAUSTFLOAT(fRec97[0]);
			float fTemp63 = (0.0205737799f * fTemp62);
			fRec106[0] = (fRec105[1] + fRec106[1]);
			fRec107[0] = (fRec106[1] + fRec107[1]);
			float fTemp64 = (fRec3[0] * float(input9[i]));
			fRec105[0] = (0.0f - (fSlow16 * (((fSlow18 * fRec106[0]) + (fSlow17 * fRec107[0])) - (fSlow13 * fTemp64))));
			fRec108[0] = (fRec104[1] + fRec108[1]);
			fRec104[0] = (fSlow15 * (fRec105[0] - (fSlow19 * fRec108[0])));
			float fTemp65 = (fRec104[0] + (fSlow14 * fTemp64));
			fRec103[0] = max((fRec103[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp65))))));
			fVbargraph21 = FAUSTFLOAT(fRec103[0]);
			float fTemp66 = (0.0265606362f * fTemp65);
			fRec111[0] = (fRec110[1] + fRec111[1]);
			fRec112[0] = (fRec111[1] + fRec112[1]);
			fRec114[0] = (fRec113[1] + fRec114[1]);
			fRec115[0] = (fRec114[1] + fRec115[1]);
			float fTemp67 = (fRec3[0] * float(input16[i]));
			fRec113[0] = (0.0f - (fSlow10 * (((fSlow12 * fRec114[0]) + (fSlow11 * fRec115[0])) - (fSlow13 * fTemp67))));
			fRec110[0] = (0.0f - (fSlow7 * (((fSlow9 * fRec111[0]) + (fSlow8 * fRec112[0])) - fRec113[0])));
			float fTemp68 = (fRec110[0] + (fSlow14 * fTemp67));
			fRec109[0] = max((fRec109[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp68))))));
			fVbargraph22 = FAUSTFLOAT(fRec109[0]);
			float fTemp69 = (fRec1[0] * ((fTemp1 + (fTemp56 + ((3.01145194e-18f * fTemp58) + (fTemp45 + (fTemp52 + (fTemp43 + fTemp46)))))) - ((2.60799377e-18f * fTemp60) + (fTemp63 + (fTemp66 + ((6.90010286e-18f * fTemp68) + ((4.87910954e-18f * fTemp18) + ((1.25978067e-18f * fTemp24) + (fTemp49 + fTemp50)))))))));
			fRec82[0] = max((fRec82[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp69))))));
			fVbargraph23 = FAUSTFLOAT(fRec82[0]);
			output2[i] = FAUSTFLOAT(fTemp69);
			float fTemp70 = (fRec1[0] * ((fTemp1 + ((5.21598753e-18f * fTemp60) + ((9.75821908e-18f * fTemp65) + ((2.69352445e-18f * fTemp55) + (fTemp45 + (fTemp46 + (fTemp49 + fTemp51))))))) - ((2.51956133e-18f * fTemp62) + ((1.38002057e-17f * fTemp68) + ((6.02290387e-18f * fTemp58) + (fTemp52 + (fTemp47 + (fTemp48 + fTemp50))))))));
			fRec116[0] = max((fRec116[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp70))))));
			fVbargraph24 = FAUSTFLOAT(fRec116[0]);
			output3[i] = FAUSTFLOAT(fTemp70);
			float fTemp71 = (fRec1[0] * ((fTemp1 + (fTemp63 + (fTemp66 + ((9.03435581e-18f * fTemp58) + (fTemp45 + (fTemp52 + (fTemp46 + ((3.7793419e-18f * fTemp24) + (1.46373282e-17f * fTemp18))))))))) - ((7.8239813e-18f * fTemp60) + ((2.0700309e-17f * fTemp68) + (fTemp56 + (fTemp49 + (fTemp50 + (4.04028677e-18f * fTemp42))))))));
			fRec117[0] = max((fRec117[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp71))))));
			fVbargraph25 = FAUSTFLOAT(fRec117[0]);
			output4[i] = FAUSTFLOAT(fTemp71);
			float fTemp72 = (fRec1[0] * ((fTemp1 + (fTemp4 + (fTemp10 + (fTemp16 + (fTemp19 + (fTemp25 + (fTemp31 + (fTemp43 + fTemp37)))))))) - (fTemp7 + (fTemp13 + (fTemp22 + (fTemp28 + (fTemp40 + fTemp34)))))));
			fRec118[0] = max((fRec118[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp72))))));
			fVbargraph26 = FAUSTFLOAT(fRec118[0]);
			output5[i] = FAUSTFLOAT(fTemp72);
			float fTemp73 = (0.0225749556f * fTemp0);
			float fTemp74 = (0.0125208041f * fTemp3);
			float fTemp75 = (0.0354141816f * fTemp6);
			float fTemp76 = (0.0473241918f * fTemp9);
			float fTemp77 = (0.0133853024f * fTemp12);
			float fTemp78 = (0.0166944042f * fTemp18);
			float fTemp79 = (0.0408927724f * fTemp21);
			float fTemp80 = (0.0387942903f * fTemp24);
			float fTemp81 = (0.0218581073f * fTemp30);
			float fTemp82 = (0.0437162146f * fTemp33);
			float fTemp83 = (0.0126197841f * fTemp36);
			float fTemp84 = (0.0276485607f * fTemp39);
			float fTemp85 = (0.0276485607f * fTemp42);
			float fTemp86 = (fTemp84 + fTemp85);
			float fTemp87 = (0.0105584692f * fTemp27);
			float fTemp88 = (0.0275132284f * fTemp15);
			float fTemp89 = (fRec1[0] * ((fTemp73 + (fTemp74 + (fTemp75 + (fTemp76 + (fTemp77 + (fTemp78 + (fTemp79 + (fTemp80 + (fTemp81 + (fTemp82 + (fTemp83 + fTemp86))))))))))) - (fTemp87 + fTemp88)));
			fRec119[0] = max((fRec119[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp89))))));
			fVbargraph27 = FAUSTFLOAT(fRec119[0]);
			output6[i] = FAUSTFLOAT(fTemp89);
			fRec123[0] = (fRec122[1] + fRec123[1]);
			fRec124[0] = (fRec123[1] + fRec124[1]);
			fRec126[0] = (fRec125[1] + fRec126[1]);
			fRec127[0] = (fRec126[1] + fRec127[1]);
			float fTemp90 = (fRec3[0] * float(input19[i]));
			fRec125[0] = (0.0f - (fSlow10 * (((fSlow12 * fRec126[0]) + (fSlow11 * fRec127[0])) - (fSlow13 * fTemp90))));
			fRec122[0] = (0.0f - (fSlow7 * (((fSlow9 * fRec123[0]) + (fSlow8 * fRec124[0])) - fRec125[0])));
			float fTemp91 = (fRec122[0] + (fSlow14 * fTemp90));
			fRec121[0] = max((fRec121[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp91))))));
			fVbargraph28 = FAUSTFLOAT(fRec121[0]);
			float fTemp92 = (0.0133853024f * fTemp91);
			fRec131[0] = (fRec130[1] + fRec131[1]);
			fRec132[0] = (fRec131[1] + fRec132[1]);
			float fTemp93 = (fRec3[0] * float(input10[i]));
			fRec130[0] = (0.0f - (fSlow16 * (((fSlow18 * fRec131[0]) + (fSlow17 * fRec132[0])) - (fSlow13 * fTemp93))));
			fRec133[0] = (fRec129[1] + fRec133[1]);
			fRec129[0] = (fSlow15 * (fRec130[0] - (fSlow19 * fRec133[0])));
			float fTemp94 = (fRec129[0] + (fSlow14 * fTemp93));
			fRec128[0] = max((fRec128[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp94))))));
			fVbargraph29 = FAUSTFLOAT(fRec128[0]);
			float fTemp95 = (5.00792059e-18f * fTemp94);
			fRec136[0] = (fRec135[1] + fRec136[1]);
			fRec137[0] = (fRec136[1] + fRec137[1]);
			float fTemp96 = (fRec3[0] * float(input5[i]));
			fRec135[0] = (0.0f - (fSlow20 * (((fSlow22 * fRec136[0]) + (fSlow21 * fRec137[0])) - (fSlow13 * fTemp96))));
			float fTemp97 = (fRec135[0] + (fSlow14 * fTemp96));
			fRec134[0] = max((fRec134[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp97))))));
			fVbargraph30 = FAUSTFLOAT(fRec134[0]);
			float fTemp98 = (0.0437162146f * fTemp97);
			float fTemp99 = (5.7955417e-18f * fTemp60);
			float fTemp100 = (0.0387942903f * fTemp62);
			float fTemp101 = (0.0276485607f * fTemp55);
			float fTemp102 = (2.676846e-18f * fTemp58);
			float fTemp103 = (8.19613358e-19f * fTemp12);
			float fTemp104 = (2.37546523e-18f * fTemp24);
			float fTemp105 = (2.676846e-18f * fTemp33);
			float fTemp106 = (1.69298611e-18f * fTemp42);
			fRec140[0] = (fRec139[1] + fRec140[1]);
			fRec141[0] = (fRec140[1] + fRec141[1]);
			fRec143[0] = (fRec142[1] + fRec143[1]);
			fRec144[0] = (fRec143[1] + fRec144[1]);
			float fTemp107 = (fRec3[0] * float(input17[i]));
			fRec142[0] = (0.0f - (fSlow10 * (((fSlow12 * fRec143[0]) + (fSlow11 * fRec144[0])) - (fSlow13 * fTemp107))));
			fRec139[0] = (0.0f - (fSlow7 * (((fSlow9 * fRec140[0]) + (fSlow8 * fRec141[0])) - fRec142[0])));
			float fTemp108 = (fRec139[0] + (fSlow14 * fTemp107));
			fRec138[0] = max((fRec138[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp108))))));
			fVbargraph31 = FAUSTFLOAT(fRec138[0]);
			float fTemp109 = (0.0354141816f * fTemp108);
			float fTemp110 = (3.06671236e-18f * (fTemp18 + fTemp68));
			float fTemp111 = (0.0166944042f * fTemp65);
			float fTemp112 = (6.50547939e-18f * fTemp6);
			float fTemp113 = (fRec1[0] * ((fTemp73 + (fTemp92 + (fTemp95 + (fTemp98 + (fTemp99 + (fTemp100 + (fTemp101 + (fTemp102 + (fTemp74 + (fTemp103 + (fTemp104 + (fTemp105 + (fTemp83 + (fTemp84 + fTemp106)))))))))))))) - (fTemp109 + (fTemp110 + (fTemp111 + ((fTemp76 + (fTemp88 + (fTemp79 + (fTemp81 + fTemp87)))) + fTemp112))))));
			fRec120[0] = max((fRec120[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp113))))));
			fVbargraph32 = FAUSTFLOAT(fRec120[0]);
			output7[i] = FAUSTFLOAT(fTemp113);
			float fTemp114 = (6.13342471e-18f * (fTemp65 - fTemp68));
			float fTemp115 = (1.30109588e-17f * fTemp108);
			float fTemp116 = (1.63922672e-18f * fTemp91);
			float fTemp117 = (5.35369201e-18f * fTemp97);
			float fTemp118 = (4.75093046e-18f * fTemp62);
			float fTemp119 = (3.38597223e-18f * fTemp55);
			float fTemp120 = (fTemp84 + fTemp83);
			float fTemp121 = (1.00158412e-17f * fTemp94);
			float fTemp122 = (1.15910834e-17f * fTemp60);
			float fTemp123 = (5.35369201e-18f * fTemp58);
			float fTemp124 = (fRec1[0] * ((fTemp73 + (fTemp114 + (fTemp115 + (fTemp116 + (fTemp117 + (fTemp118 + (fTemp119 + (fTemp74 + (fTemp76 + (fTemp79 + (fTemp81 + fTemp120))))))))))) - (fTemp121 + (fTemp122 + (fTemp123 + (fTemp75 + (fTemp77 + (fTemp88 + (fTemp78 + (fTemp80 + (fTemp87 + (fTemp85 + fTemp82))))))))))));
			fRec145[0] = max((fRec145[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp124))))));
			fVbargraph33 = FAUSTFLOAT(fRec145[0]);
			output8[i] = FAUSTFLOAT(fTemp124);
			float fTemp125 = (1.50237622e-17f * fTemp94);
			float fTemp126 = (1.73866251e-17f * fTemp60);
			float fTemp127 = (8.03053822e-18f * fTemp58);
			float fTemp128 = (1.95164382e-17f * fTemp6);
			float fTemp129 = (9.20013769e-18f * fTemp18);
			float fTemp130 = (9.20013769e-18f * fTemp68);
			float fTemp131 = (5.07895844e-18f * fTemp42);
			float fTemp132 = (8.03053822e-18f * fTemp33);
			float fTemp133 = (7.1263959e-18f * fTemp24);
			float fTemp134 = (2.45884007e-18f * fTemp12);
			float fTemp135 = (fRec1[0] * ((fTemp73 + (fTemp109 + (fTemp125 + (fTemp126 + (fTemp111 + (fTemp127 + (fTemp74 + (fTemp128 + (fTemp129 + fTemp120))))))))) - (fTemp92 + (fTemp98 + (fTemp100 + (fTemp130 + (fTemp101 + (fTemp76 + ((fTemp88 + (fTemp79 + ((fTemp87 + (fTemp81 + (fTemp131 + fTemp132))) + fTemp133))) + fTemp134)))))))));
			fRec146[0] = max((fRec146[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp135))))));
			fVbargraph34 = FAUSTFLOAT(fRec146[0]);
			output9[i] = FAUSTFLOAT(fTemp135);
			float fTemp136 = (0.0276485607f * fTemp55);
			float fTemp137 = (0.0437162146f * fTemp58);
			float fTemp138 = (0.0253968257f * fTemp15);
			float fTemp139 = (0.0378593504f * fTemp60);
			float fTemp140 = (0.0258628614f * fTemp62);
			float fTemp141 = (0.0252395682f * fTemp36);
			float fTemp142 = (0.0258628614f * fTemp24);
			float fTemp143 = (0.0500832163f * fTemp3);
			float fTemp144 = (fRec1[0] * ((fTemp73 + ((6.13342471e-18f * fTemp68) + (fTemp136 + (fTemp137 + (fTemp138 + (fTemp85 + (2.676846e-18f * fTemp30))))))) - ((0.0333888084f * (fTemp18 - fTemp65)) + (fTemp139 + (fTemp140 + (((fTemp141 + fTemp142) + (2.31821668e-18f * fTemp9)) + fTemp143))))));
			fRec147[0] = max((fRec147[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp144))))));
			fVbargraph35 = FAUSTFLOAT(fRec147[0]);
			output10[i] = FAUSTFLOAT(fTemp144);
			float fTemp145 = (0.0258628614f * fTemp62);
			float fTemp146 = (fRec1[0] * ((fTemp73 + (fTemp139 + ((0.0333888084f * fTemp65) + ((1.84002754e-17f * fTemp68) + (fTemp101 + ((6.95465004e-18f * fTemp9) + (fTemp138 + ((0.0258628614f * fTemp24) + (0.0333888084f * fTemp18))))))))) - (fTemp145 + (fTemp137 + (fTemp143 + ((fTemp141 + (0.0276485607f * fTemp42)) + (8.03053822e-18f * fTemp30)))))));
			fRec148[0] = max((fRec148[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp146))))));
			fVbargraph36 = FAUSTFLOAT(fRec148[0]);
			output11[i] = FAUSTFLOAT(fTemp146);
			float fTemp147 = (fRec1[0] * ((fTemp73 + (fTemp140 + ((3.06671256e-17f * fTemp68) + (fTemp137 + (fTemp138 + ((0.0333888084f * fTemp18) + (fTemp142 + (1.33842306e-17f * fTemp30)))))))) - (fTemp139 + ((0.0333888084f * fTemp65) + (fTemp136 + (fTemp143 + ((1.15910834e-17f * fTemp9) + (fTemp85 + fTemp141))))))));
			fRec149[0] = max((fRec149[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp147))))));
			fVbargraph37 = FAUSTFLOAT(fRec149[0]);
			output12[i] = FAUSTFLOAT(fTemp147);
			float fTemp148 = (fRec1[0] * ((fTemp73 + (fTemp139 + (fTemp145 + ((4.29339759e-17f * fTemp68) + ((1.62275168e-17f * fTemp9) + (fTemp138 + (0.0276485607f * fTemp42))))))) - ((0.0333888084f * fTemp65) + (fTemp101 + (fTemp137 + (fTemp143 + (((fTemp141 + (1.87379222e-17f * fTemp30)) + (0.0258628614f * fTemp24)) + (0.0333888084f * fTemp18))))))));
			fRec150[0] = max((fRec150[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp148))))));
			fVbargraph38 = FAUSTFLOAT(fRec150[0]);
			output13[i] = FAUSTFLOAT(fTemp148);
			float fTemp149 = (fRec1[0] * ((fTemp73 + (fTemp74 + (fTemp76 + (fTemp78 + (fTemp80 + (fTemp87 + (fTemp81 + (fTemp85 + fTemp83)))))))) - (fTemp75 + (fTemp77 + (fTemp88 + (fTemp79 + (fTemp84 + fTemp82)))))));
			fRec151[0] = max((fRec151[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp149))))));
			fVbargraph39 = FAUSTFLOAT(fRec151[0]);
			output14[i] = FAUSTFLOAT(fTemp149);
			float fTemp150 = (fRec1[0] * ((fTemp73 + (fTemp109 + (fTemp99 + (fTemp100 + (fTemp101 + (fTemp102 + (fTemp74 + (fTemp112 + (fTemp79 + (fTemp104 + (fTemp87 + (fTemp83 + fTemp106)))))))))))) - (fTemp110 + (fTemp92 + (fTemp95 + (fTemp98 + (fTemp111 + (fTemp76 + (fTemp103 + (fTemp88 + (fTemp81 + (fTemp84 + fTemp105))))))))))));
			fRec152[0] = max((fRec152[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp150))))));
			fVbargraph40 = FAUSTFLOAT(fRec152[0]);
			output15[i] = FAUSTFLOAT(fTemp150);
			float fTemp151 = (fRec1[0] * ((fTemp73 + (fTemp114 + (fTemp121 + (fTemp118 + (fTemp119 + (fTemp74 + (fTemp75 + (fTemp76 + (fTemp77 + (fTemp87 + (fTemp81 + (fTemp83 + fTemp82)))))))))))) - (fTemp115 + (fTemp116 + (fTemp117 + (fTemp122 + (fTemp123 + (fTemp88 + (fTemp78 + (fTemp79 + (fTemp80 + fTemp86)))))))))));
			fRec153[0] = max((fRec153[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp151))))));
			fVbargraph41 = FAUSTFLOAT(fRec153[0]);
			output16[i] = FAUSTFLOAT(fTemp151);
			float fTemp152 = (fRec1[0] * ((fTemp73 + (fTemp92 + (fTemp98 + (fTemp126 + (fTemp111 + (fTemp127 + (fTemp74 + (fTemp134 + (fTemp129 + (fTemp79 + (fTemp87 + (fTemp83 + fTemp132)))))))))))) - (fTemp109 + (fTemp125 + (fTemp100 + (fTemp130 + (fTemp101 + (fTemp128 + (fTemp76 + (fTemp88 + (fTemp133 + (fTemp81 + (fTemp84 + fTemp131)))))))))))));
			fRec154[0] = max((fRec154[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp152))))));
			fVbargraph42 = FAUSTFLOAT(fRec154[0]);
			output17[i] = FAUSTFLOAT(fTemp152);
			float fTemp153 = (fTemp39 + fTemp0);
			float fTemp154 = (0.0210937504f * fTemp153);
			float fTemp155 = (0.0294138286f * fTemp108);
			float fTemp156 = (0.0415974371f * fTemp94);
			float fTemp157 = (0.0272319149f * fTemp97);
			float fTemp158 = (0.031444706f * fTemp60);
			float fTemp159 = (0.0131542645f * fTemp62);
			float fTemp160 = (0.0169820823f * fTemp65);
			float fTemp161 = (2.54710831e-18f * fTemp68);
			float fTemp162 = (0.0210937504f * fTemp55);
			float fTemp163 = (0.0272319149f * fTemp58);
			float fTemp164 = (5.23659549e-18f * fTemp36);
			float fTemp165 = ((0.0210937504f * fTemp42) + fTemp164);
			float fTemp166 = (0.0272319149f * fTemp33);
			float fTemp167 = (1.6674738e-18f * fTemp30);
			float fTemp168 = (0.0131542645f * fTemp24);
			float fTemp169 = (2.54710831e-18f * fTemp21);
			float fTemp170 = (1.92543286e-18f * fTemp9);
			float fTemp171 = (0.011117382f * (fTemp12 + fTemp91));
			float fTemp172 = (0.0214808229f * fTemp27);
			float fTemp173 = (0.0169820823f * fTemp18);
			float fTemp174 = (0.0246093757f * fTemp15);
			float fTemp175 = (0.0294138286f * fTemp6);
			float fTemp176 = (0.0207987186f * fTemp3);
			float fTemp177 = (fRec1[0] * ((fTemp154 + (fTemp155 + (fTemp156 + (fTemp157 + (fTemp158 + (fTemp159 + (fTemp160 + (fTemp161 + (fTemp162 + (fTemp163 + (((((fTemp165 + fTemp166) + fTemp167) + fTemp168) + fTemp169) + fTemp170))))))))))) - (fTemp171 + ((((fTemp172 + fTemp173) + fTemp174) + fTemp175) + fTemp176))));
			fRec155[0] = max((fRec155[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp177))))));
			fVbargraph43 = FAUSTFLOAT(fRec155[0]);
			output18[i] = FAUSTFLOAT(fTemp177);
			float fTemp178 = (0.011117382f * (fTemp12 - fTemp91));
			float fTemp179 = (0.0294138286f * fTemp108);
			float fTemp180 = (0.0272319149f * fTemp97);
			float fTemp181 = (0.0131542645f * fTemp62);
			float fTemp182 = (0.0169820823f * fTemp65);
			float fTemp183 = (7.64132493e-18f * fTemp68);
			float fTemp184 = (0.0210937504f * fTemp55);
			float fTemp185 = (0.0169820823f * fTemp18);
			float fTemp186 = (0.0294138286f * fTemp6);
			float fTemp187 = (0.0210937504f * fTemp42);
			float fTemp188 = (0.0272319149f * fTemp33);
			float fTemp189 = (5.00242149e-18f * fTemp30);
			float fTemp190 = (0.0131542645f * fTemp24);
			float fTemp191 = (7.64132493e-18f * fTemp21);
			float fTemp192 = (5.77629858e-18f * fTemp9);
			float fTemp193 = (fRec1[0] * ((fTemp154 + (fTemp178 + (fTemp179 + (fTemp180 + (fTemp181 + (fTemp182 + (fTemp183 + (fTemp184 + ((fTemp164 + fTemp185) + fTemp186))))))))) - (fTemp156 + (fTemp158 + (fTemp163 + (fTemp176 + ((fTemp174 + (((fTemp172 + ((fTemp187 + fTemp188) + fTemp189)) + fTemp190) + fTemp191)) + fTemp192)))))));
			fRec156[0] = max((fRec156[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp193))))));
			fVbargraph44 = FAUSTFLOAT(fRec156[0]);
			output19[i] = FAUSTFLOAT(fTemp193);
			float fTemp194 = (0.011117382f * fTemp91);
			float fTemp195 = (1.27355415e-17f * fTemp68);
			float fTemp196 = (8.33736929e-18f * fTemp30);
			float fTemp197 = (1.27355415e-17f * fTemp21);
			float fTemp198 = (0.0169820823f * fTemp18);
			float fTemp199 = (0.011117382f * fTemp12);
			float fTemp200 = (9.62716472e-18f * fTemp9);
			float fTemp201 = (0.0294138286f * fTemp6);
			float fTemp202 = (0.0294138286f * fTemp108);
			float fTemp203 = (0.0169820823f * fTemp65);
			float fTemp204 = (0.0131542645f * fTemp24);
			float fTemp205 = (fRec1[0] * ((fTemp194 + (fTemp156 + (fTemp158 + (fTemp195 + (fTemp163 + ((((((fTemp164 + fTemp196) + fTemp197) + fTemp198) + fTemp199) + fTemp200) + fTemp201)))))) - ((0.0210937504f * (fTemp42 - fTemp153)) + (fTemp202 + (fTemp157 + (fTemp159 + (fTemp203 + (fTemp162 + (fTemp176 + (fTemp174 + ((fTemp166 + fTemp172) + fTemp204)))))))))));
			fRec157[0] = max((fRec157[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp205))))));
			fVbargraph45 = FAUSTFLOAT(fRec157[0]);
			output20[i] = FAUSTFLOAT(fTemp205);
			float fTemp206 = (0.011117382f * fTemp91);
			float fTemp207 = (1.7829759e-17f * fTemp68);
			float fTemp208 = (fTemp164 + (0.0210937504f * fTemp42));
			float fTemp209 = (0.0272319149f * fTemp33);
			float fTemp210 = (0.0294138286f * fTemp108);
			float fTemp211 = (0.0131542645f * fTemp62);
			float fTemp212 = (0.0169820823f * fTemp65);
			float fTemp213 = (1.16723167e-17f * fTemp30);
			float fTemp214 = (1.7829759e-17f * fTemp21);
			float fTemp215 = (0.011117382f * fTemp12);
			float fTemp216 = (1.34780304e-17f * fTemp9);
			float fTemp217 = (0.0294138286f * fTemp6);
			float fTemp218 = (fRec1[0] * (((0.0210937504f * (fTemp153 - fTemp55)) + (fTemp206 + (fTemp207 + (fTemp190 + (fTemp208 + fTemp209))))) - (fTemp210 + (fTemp156 + (fTemp180 + (fTemp158 + (fTemp211 + (fTemp212 + (fTemp163 + (fTemp176 + ((((fTemp174 + (fTemp173 + ((fTemp172 + fTemp213) + fTemp214))) + fTemp215) + fTemp216) + fTemp217)))))))))));
			fRec158[0] = max((fRec158[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp218))))));
			fVbargraph46 = FAUSTFLOAT(fRec158[0]);
			output21[i] = FAUSTFLOAT(fTemp218);
			float fTemp219 = (0.0210937504f * (fTemp0 - fTemp39));
			float fTemp220 = (fRec1[0] * ((fTemp219 + (fTemp171 + (fTemp158 + (fTemp159 + (fTemp160 + (fTemp161 + (fTemp162 + (fTemp163 + (fTemp175 + (fTemp170 + (fTemp168 + (fTemp172 + (fTemp165 + fTemp167))))))))))))) - (fTemp155 + (fTemp156 + (fTemp157 + (fTemp176 + (fTemp174 + (fTemp173 + (fTemp166 + fTemp169)))))))));
			fRec159[0] = max((fRec159[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp220))))));
			fVbargraph47 = FAUSTFLOAT(fRec159[0]);
			output22[i] = FAUSTFLOAT(fTemp220);
			float fTemp221 = (fRec1[0] * ((fTemp219 + (fTemp156 + (fTemp181 + (fTemp182 + (fTemp183 + (fTemp184 + (fTemp185 + (fTemp191 + (fTemp172 + (fTemp164 + fTemp188)))))))))) - (fTemp178 + (fTemp179 + (fTemp180 + (fTemp158 + (fTemp163 + (fTemp176 + (fTemp186 + (fTemp192 + (fTemp174 + (fTemp190 + (fTemp187 + fTemp189)))))))))))));
			fRec160[0] = max((fRec160[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp221))))));
			fVbargraph48 = FAUSTFLOAT(fRec160[0]);
			output23[i] = FAUSTFLOAT(fTemp221);
			float fTemp222 = (fRec1[0] * ((fTemp202 + (fTemp157 + (fTemp158 + (fTemp195 + (fTemp163 + (fTemp200 + (fTemp198 + (fTemp172 + (fTemp196 + (fTemp164 + fTemp166)))))))))) - ((0.0210937504f * ((fTemp39 + fTemp42) - fTemp0)) + (fTemp194 + (fTemp156 + (fTemp159 + (fTemp203 + (fTemp162 + (fTemp176 + (fTemp201 + (fTemp199 + (fTemp174 + (fTemp197 + fTemp204)))))))))))));
			fRec161[0] = max((fRec161[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp222))))));
			fVbargraph49 = FAUSTFLOAT(fRec161[0]);
			output24[i] = FAUSTFLOAT(fTemp222);
			float fTemp223 = (fRec1[0] * ((fTemp210 + (fTemp156 + (fTemp180 + (fTemp207 + (fTemp217 + (fTemp215 + (fTemp214 + (fTemp190 + (fTemp172 + fTemp208))))))))) - ((0.0210937504f * ((fTemp39 + fTemp55) - fTemp0)) + (fTemp206 + (fTemp158 + (fTemp211 + (fTemp212 + (fTemp163 + (fTemp176 + (fTemp216 + (fTemp174 + (fTemp173 + (fTemp209 + fTemp213)))))))))))));
			fRec162[0] = max((fRec162[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp223))))));
			fVbargraph50 = FAUSTFLOAT(fRec162[0]);
			output25[i] = FAUSTFLOAT(fTemp223);
			float fTemp224 = (0.0201733354f * fTemp0);
			float fTemp225 = (0.0105351806f * (fTemp42 + fTemp55));
			float fTemp226 = (0.0213084519f * (fTemp33 + fTemp97));
			float fTemp227 = (0.00627702521f * fTemp108);
			float fTemp228 = (0.035587389f * fTemp91);
			float fTemp229 = (0.0169982649f * fTemp94);
			float fTemp230 = (0.0290784966f * fTemp60);
			float fTemp231 = (0.0304601658f * fTemp62);
			float fTemp232 = (0.00231317081f * fTemp65);
			float fTemp233 = (0.0071028173f * fTemp58);
			float fTemp234 = (0.035587389f * fTemp12);
			float fTemp235 = (0.0142547125f * fTemp15);
			float fTemp236 = (0.0304601658f * fTemp24);
			float fTemp237 = (0.0263335984f * fTemp27);
			float fTemp238 = (0.0316055417f * fTemp39);
			float fTemp239 = (0.0328065082f * fTemp36);
			float fTemp240 = (fTemp238 + fTemp239);
			float fTemp241 = (fTemp237 + fTemp240);
			float fTemp242 = (4.75846401e-19f * fTemp68);
			float fTemp243 = (1.14222021e-18f * fTemp30);
			float fTemp244 = (2.73352938e-18f * fTemp21);
			float fTemp245 = (0.00231317081f * fTemp18);
			float fTemp246 = (4.67617897e-18f * fTemp9);
			float fTemp247 = (0.00627702521f * fTemp6);
			float fTemp248 = (0.00147950905f * fTemp3);
			float fTemp249 = (fRec1[0] * ((fTemp224 + (fTemp225 + (fTemp226 + (fTemp227 + (fTemp228 + (fTemp229 + (fTemp230 + (fTemp231 + (fTemp232 + (fTemp233 + (fTemp234 + (fTemp235 + (fTemp236 + fTemp241))))))))))))) - (fTemp242 + (((((fTemp243 + fTemp244) + fTemp245) + fTemp246) + fTemp247) + fTemp248))));
			fRec163[0] = max((fRec163[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp249))))));
			fVbargraph51 = FAUSTFLOAT(fRec163[0]);
			output26[i] = FAUSTFLOAT(fTemp249);
			float fTemp250 = (0.00627702521f * fTemp108);
			float fTemp251 = (5.43562818e-19f * fTemp68);
			float fTemp252 = (0.00627702521f * fTemp6);
			float fTemp253 = (0.00231317081f * fTemp18);
			float fTemp254 = (0.0105351806f * (fTemp42 - fTemp55));
			float fTemp255 = (0.0213084519f * (fTemp33 - fTemp97));
			float fTemp256 = (1.30476637e-18f * fTemp30);
			float fTemp257 = (3.1225305e-18f * fTemp21);
			float fTemp258 = (5.34163296e-18f * fTemp9);
			float fTemp259 = (fRec1[0] * ((fTemp224 + (fTemp250 + (fTemp228 + (fTemp231 + (fTemp232 + (fTemp251 + (fTemp252 + (fTemp235 + (fTemp253 + fTemp241))))))))) - (fTemp254 + (fTemp255 + (fTemp229 + (fTemp230 + (fTemp233 + (fTemp248 + ((fTemp234 + ((fTemp236 + fTemp256) + fTemp257)) + fTemp258)))))))));
			fRec164[0] = max((fRec164[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp259))))));
			fVbargraph52 = FAUSTFLOAT(fRec164[0]);
			output27[i] = FAUSTFLOAT(fTemp259);
			float fTemp260 = (9.0593803e-19f * fTemp68);
			float fTemp261 = (0.00627702521f * fTemp6);
			float fTemp262 = (8.90272159e-18f * fTemp9);
			float fTemp263 = (0.00231317081f * fTemp18);
			float fTemp264 = (5.20421758e-18f * fTemp21);
			float fTemp265 = (2.17461068e-18f * fTemp30);
			float fTemp266 = (0.00627702521f * fTemp108);
			float fTemp267 = (0.035587389f * fTemp91);
			float fTemp268 = (0.0213084519f * fTemp97);
			float fTemp269 = (0.0304601658f * fTemp62);
			float fTemp270 = (0.00231317081f * fTemp65);
			float fTemp271 = (0.0105351806f * fTemp55);
			float fTemp272 = (0.0105351806f * fTemp42);
			float fTemp273 = (0.0213084519f * fTemp33);
			float fTemp274 = (0.0304601658f * fTemp24);
			float fTemp275 = (0.035587389f * fTemp12);
			float fTemp276 = (fRec1[0] * ((fTemp224 + (fTemp229 + (fTemp230 + (fTemp260 + (fTemp233 + (fTemp261 + (fTemp262 + (fTemp235 + (fTemp263 + (fTemp264 + (fTemp237 + (fTemp265 + fTemp240)))))))))))) - (fTemp266 + (fTemp267 + (fTemp268 + (fTemp269 + (fTemp270 + (fTemp271 + (fTemp248 + (((fTemp272 + fTemp273) + fTemp274) + fTemp275))))))))));
			fRec165[0] = max((fRec165[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp276))))));
			fVbargraph53 = FAUSTFLOAT(fRec165[0]);
			output28[i] = FAUSTFLOAT(fTemp276);
			float fTemp277 = (1.26831324e-18f * fTemp68);
			float fTemp278 = (0.035587389f * fTemp12);
			float fTemp279 = (0.0304601658f * fTemp24);
			float fTemp280 = (0.0213084519f * fTemp33);
			float fTemp281 = (0.0105351806f * fTemp42);
			float fTemp282 = (0.00627702521f * fTemp108);
			float fTemp283 = (0.0213084519f * fTemp97);
			float fTemp284 = (0.0304601658f * fTemp62);
			float fTemp285 = (0.00231317081f * fTemp65);
			float fTemp286 = (0.0105351806f * fTemp55);
			float fTemp287 = (3.04445499e-18f * fTemp30);
			float fTemp288 = (7.28590444e-18f * fTemp21);
			float fTemp289 = (0.00231317081f * fTemp18);
			float fTemp290 = (1.24638111e-17f * fTemp9);
			float fTemp291 = (0.00627702521f * fTemp6);
			float fTemp292 = (fRec1[0] * ((fTemp224 + (fTemp277 + (fTemp278 + (fTemp235 + (fTemp279 + (fTemp237 + (fTemp280 + (fTemp239 + (fTemp238 + fTemp281))))))))) - (fTemp282 + (fTemp228 + (fTemp229 + (fTemp283 + (fTemp230 + (fTemp284 + (fTemp285 + (fTemp286 + (fTemp233 + (fTemp248 + ((((fTemp287 + fTemp288) + fTemp289) + fTemp290) + fTemp291)))))))))))));
			fRec166[0] = max((fRec166[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp292))))));
			fVbargraph54 = FAUSTFLOAT(fRec166[0]);
			output29[i] = FAUSTFLOAT(fTemp292);
			float fTemp293 = (0.0272004437f * fTemp108);
			float fTemp294 = (0.0169982649f * fTemp94);
			float fTemp295 = (0.0071028173f * fTemp97);
			float fTemp296 = (0.0300712213f * fTemp65);
			float fTemp297 = (0.0355082154f * fTemp68);
			float fTemp298 = (0.0105351806f * fTemp55);
			float fTemp299 = (0.0213084519f * fTemp58);
			float fTemp300 = (0.0103565631f * fTemp3);
			float fTemp301 = (0.0188310761f * fTemp6);
			float fTemp302 = (0.0042514056f * fTemp15);
			float fTemp303 = (0.0208185371f * fTemp18);
			float fTemp304 = (0.0226643533f * fTemp21);
			float fTemp305 = (0.0284112692f * fTemp30);
			float fTemp306 = (0.0213084519f * fTemp33);
			float fTemp307 = (0.0105351806f * fTemp39);
			float fTemp308 = (0.0316055417f * fTemp42);
			float fTemp309 = (fTemp307 + fTemp308);
			float fTemp310 = (0.010280801f * fTemp91);
			float fTemp311 = (0.00671042223f * fTemp60);
			float fTemp312 = (0.00537532335f * fTemp62);
			float fTemp313 = (0.0164032541f * fTemp36);
			float fTemp314 = (0.020481687f * fTemp27);
			float fTemp315 = (fTemp313 + fTemp314);
			float fTemp316 = (0.0161259696f * fTemp24);
			float fTemp317 = (0.0308424029f * fTemp12);
			float fTemp318 = (0.00894722994f * fTemp9);
			float fTemp319 = (fRec1[0] * ((fTemp224 + (fTemp293 + (fTemp294 + (fTemp295 + (fTemp296 + (fTemp297 + (fTemp298 + (fTemp299 + (fTemp300 + (fTemp301 + (fTemp302 + (fTemp303 + (fTemp304 + (fTemp305 + (fTemp306 + fTemp309))))))))))))))) - (fTemp310 + (fTemp311 + (fTemp312 + (((fTemp315 + fTemp316) + fTemp317) + fTemp318))))));
			fRec167[0] = max((fRec167[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp319))))));
			fVbargraph55 = FAUSTFLOAT(fRec167[0]);
			output30[i] = FAUSTFLOAT(fTemp319);
			float fTemp320 = (0.0169982649f * fTemp94);
			float fTemp321 = (0.0213084519f * fTemp97);
			float fTemp322 = (0.0316055417f * fTemp55);
			float fTemp323 = (0.0213084519f * fTemp58);
			float fTemp324 = (0.0103565631f * fTemp3);
			float fTemp325 = (0.00894722994f * fTemp9);
			float fTemp326 = (0.0071028173f * fTemp33);
			float fTemp327 = (0.0105351806f * fTemp42);
			float fTemp328 = (0.0188310761f * fTemp108);
			float fTemp329 = (0.0308424029f * fTemp91);
			float fTemp330 = (0.00671042223f * fTemp60);
			float fTemp331 = (0.0161259696f * fTemp62);
			float fTemp332 = (0.0208185371f * fTemp65);
			float fTemp333 = (0.0355082154f * fTemp68);
			float fTemp334 = (0.0284112692f * fTemp30);
			float fTemp335 = (0.00537532335f * fTemp24);
			float fTemp336 = (0.0226643533f * fTemp21);
			float fTemp337 = (0.0300712213f * fTemp18);
			float fTemp338 = (0.010280801f * fTemp12);
			float fTemp339 = (0.0272004437f * fTemp6);
			float fTemp340 = (fRec1[0] * ((fTemp224 + (fTemp320 + (fTemp321 + (fTemp322 + (fTemp323 + (fTemp324 + (fTemp325 + (fTemp302 + (fTemp326 + (fTemp307 + fTemp327)))))))))) - (fTemp328 + (fTemp329 + (fTemp330 + (fTemp331 + (fTemp332 + (fTemp333 + ((((((fTemp314 + (fTemp313 + fTemp334)) + fTemp335) + fTemp336) + fTemp337) + fTemp338) + fTemp339)))))))));
			fRec168[0] = max((fRec168[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp340))))));
			fVbargraph56 = FAUSTFLOAT(fRec168[0]);
			output31[i] = FAUSTFLOAT(fTemp340);
			float fTemp341 = (0.0213084519f * fTemp97);
			float fTemp342 = (0.00671042223f * fTemp60);
			float fTemp343 = (0.0103565631f * fTemp3);
			float fTemp344 = (0.0272004437f * fTemp6);
			float fTemp345 = (0.00894722994f * fTemp9);
			float fTemp346 = (0.010280801f * fTemp12);
			float fTemp347 = (0.0300712213f * fTemp18);
			float fTemp348 = (0.00537532335f * fTemp24);
			float fTemp349 = (0.0188310761f * fTemp108);
			float fTemp350 = (0.0308424029f * fTemp91);
			float fTemp351 = (0.0169982649f * fTemp94);
			float fTemp352 = (0.0161259696f * fTemp62);
			float fTemp353 = (0.0208185371f * fTemp65);
			float fTemp354 = (0.0213084519f * fTemp58);
			float fTemp355 = (0.0105351806f * fTemp42);
			float fTemp356 = (0.0071028173f * fTemp33);
			float fTemp357 = (0.0284112692f * fTemp30);
			float fTemp358 = (fRec1[0] * ((fTemp224 + (fTemp341 + (fTemp342 + (fTemp333 + (fTemp322 + (fTemp343 + (fTemp344 + (fTemp345 + (fTemp346 + (fTemp302 + (fTemp347 + (fTemp307 + fTemp348)))))))))))) - (fTemp349 + (fTemp350 + (fTemp351 + (fTemp352 + (fTemp353 + (fTemp354 + (fTemp336 + (fTemp314 + (((fTemp313 + fTemp355) + fTemp356) + fTemp357)))))))))));
			fRec169[0] = max((fRec169[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp358))))));
			fVbargraph57 = FAUSTFLOAT(fRec169[0]);
			output32[i] = FAUSTFLOAT(fTemp358);
			float fTemp359 = (0.0272004437f * fTemp108);
			float fTemp360 = (0.0071028173f * fTemp97);
			float fTemp361 = (0.00671042223f * fTemp60);
			float fTemp362 = (0.0300712213f * fTemp65);
			float fTemp363 = (0.0105351806f * fTemp55);
			float fTemp364 = (0.0103565631f * fTemp3);
			float fTemp365 = (0.0226643533f * fTemp21);
			float fTemp366 = (0.0284112692f * fTemp30);
			float fTemp367 = (0.010280801f * fTemp91);
			float fTemp368 = (0.00537532335f * fTemp62);
			float fTemp369 = (0.0213084519f * fTemp58);
			float fTemp370 = (fTemp308 + fTemp313);
			float fTemp371 = (0.0208185371f * fTemp18);
			float fTemp372 = (0.0188310761f * fTemp6);
			float fTemp373 = (fRec1[0] * ((fTemp224 + (fTemp359 + (fTemp360 + (fTemp361 + (fTemp362 + (fTemp363 + (fTemp364 + (fTemp317 + (fTemp302 + (fTemp365 + (fTemp316 + (fTemp307 + fTemp366)))))))))))) - (fTemp367 + (fTemp294 + (fTemp368 + (fTemp297 + (fTemp369 + ((fTemp318 + ((fTemp314 + (fTemp306 + fTemp370)) + fTemp371)) + fTemp372))))))));
			fRec170[0] = max((fRec170[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp373))))));
			fVbargraph58 = FAUSTFLOAT(fRec170[0]);
			output33[i] = FAUSTFLOAT(fTemp373);
			float fTemp374 = (0.010280801f * fTemp91);
			float fTemp375 = (0.0169982649f * fTemp94);
			float fTemp376 = (0.00537532335f * fTemp62);
			float fTemp377 = (0.0355082154f * fTemp68);
			float fTemp378 = (0.0213084519f * fTemp58);
			float fTemp379 = (0.0103565631f * fTemp3);
			float fTemp380 = (0.0308424029f * fTemp12);
			float fTemp381 = (0.0226643533f * fTemp21);
			float fTemp382 = (0.0284112692f * fTemp30);
			float fTemp383 = (0.0071028173f * fTemp97);
			float fTemp384 = (0.00671042223f * fTemp60);
			float fTemp385 = (0.0300712213f * fTemp65);
			float fTemp386 = (0.0105351806f * fTemp55);
			float fTemp387 = (0.0213084519f * fTemp33);
			float fTemp388 = (0.0208185371f * fTemp18);
			float fTemp389 = (0.00894722994f * fTemp9);
			float fTemp390 = (0.0188310761f * fTemp6);
			float fTemp391 = (fRec1[0] * ((fTemp224 + (fTemp374 + (fTemp375 + (fTemp376 + (fTemp377 + (fTemp378 + (fTemp379 + (fTemp380 + (fTemp302 + (fTemp381 + (fTemp316 + (fTemp307 + fTemp382)))))))))))) - (fTemp293 + (fTemp383 + (fTemp384 + (fTemp385 + (fTemp386 + ((((fTemp314 + (fTemp370 + fTemp387)) + fTemp388) + fTemp389) + fTemp390))))))));
			fRec171[0] = max((fRec171[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp391))))));
			fVbargraph59 = FAUSTFLOAT(fRec171[0]);
			output34[i] = FAUSTFLOAT(fTemp391);
			float fTemp392 = (0.0188310761f * fTemp108);
			float fTemp393 = (0.0169982649f * fTemp94);
			float fTemp394 = (0.0208185371f * fTemp65);
			float fTemp395 = (0.0213084519f * fTemp58);
			float fTemp396 = (0.0103565631f * fTemp3);
			float fTemp397 = (0.0272004437f * fTemp6);
			float fTemp398 = (0.00894722994f * fTemp9);
			float fTemp399 = (0.010280801f * fTemp12);
			float fTemp400 = (0.0300712213f * fTemp18);
			float fTemp401 = (0.00537532335f * fTemp24);
			float fTemp402 = (0.00671042223f * fTemp60);
			float fTemp403 = (0.0105351806f * fTemp42);
			float fTemp404 = (0.0071028173f * fTemp33);
			float fTemp405 = (0.0284112692f * fTemp30);
			float fTemp406 = (fRec1[0] * ((fTemp224 + (fTemp392 + (fTemp329 + (fTemp393 + (fTemp331 + (fTemp394 + (fTemp395 + (fTemp396 + (fTemp397 + (fTemp398 + (fTemp399 + (fTemp302 + (fTemp400 + (fTemp307 + fTemp401)))))))))))))) - (fTemp321 + (fTemp402 + (fTemp377 + (fTemp322 + (fTemp381 + (fTemp314 + (((fTemp313 + fTemp403) + fTemp404) + fTemp405)))))))));
			fRec172[0] = max((fRec172[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp406))))));
			fVbargraph60 = FAUSTFLOAT(fRec172[0]);
			output35[i] = FAUSTFLOAT(fTemp406);
			float fTemp407 = (0.0188310761f * fTemp108);
			float fTemp408 = (0.00671042223f * fTemp60);
			float fTemp409 = (0.0208185371f * fTemp65);
			float fTemp410 = (0.0355082154f * fTemp68);
			float fTemp411 = (0.0103565631f * fTemp3);
			float fTemp412 = (0.00894722994f * fTemp9);
			float fTemp413 = (0.0071028173f * fTemp33);
			float fTemp414 = (0.0105351806f * fTemp42);
			float fTemp415 = (0.0169982649f * fTemp94);
			float fTemp416 = (0.0213084519f * fTemp58);
			float fTemp417 = (0.0284112692f * fTemp30);
			float fTemp418 = (0.00537532335f * fTemp24);
			float fTemp419 = (0.0226643533f * fTemp21);
			float fTemp420 = (0.0300712213f * fTemp18);
			float fTemp421 = (0.010280801f * fTemp12);
			float fTemp422 = (0.0272004437f * fTemp6);
			float fTemp423 = (fRec1[0] * ((fTemp224 + (fTemp407 + (fTemp350 + (fTemp408 + (fTemp352 + (fTemp409 + (fTemp410 + (fTemp411 + (fTemp412 + (fTemp302 + (fTemp413 + (fTemp307 + fTemp414)))))))))))) - (fTemp415 + (fTemp341 + (fTemp322 + (fTemp416 + ((((((fTemp314 + (fTemp313 + fTemp417)) + fTemp418) + fTemp419) + fTemp420) + fTemp421) + fTemp422)))))));
			fRec173[0] = max((fRec173[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp423))))));
			fVbargraph61 = FAUSTFLOAT(fRec173[0]);
			output36[i] = FAUSTFLOAT(fTemp423);
			float fTemp424 = (0.010280801f * fTemp91);
			float fTemp425 = (0.00671042223f * fTemp60);
			float fTemp426 = (0.00537532335f * fTemp62);
			float fTemp427 = (0.0103565631f * fTemp3);
			float fTemp428 = (0.0188310761f * fTemp6);
			float fTemp429 = (0.0208185371f * fTemp18);
			float fTemp430 = (0.0226643533f * fTemp21);
			float fTemp431 = (0.0284112692f * fTemp30);
			float fTemp432 = (0.0213084519f * fTemp33);
			float fTemp433 = (0.0316055417f * fTemp42);
			float fTemp434 = (0.0272004437f * fTemp108);
			float fTemp435 = (0.0169982649f * fTemp94);
			float fTemp436 = (0.0071028173f * fTemp97);
			float fTemp437 = (0.0300712213f * fTemp65);
			float fTemp438 = (0.0355082154f * fTemp68);
			float fTemp439 = (0.0105351806f * fTemp55);
			float fTemp440 = (0.0213084519f * fTemp58);
			float fTemp441 = (0.0161259696f * fTemp24);
			float fTemp442 = (0.0308424029f * fTemp12);
			float fTemp443 = (0.00894722994f * fTemp9);
			float fTemp444 = (fRec1[0] * ((fTemp224 + (fTemp424 + (fTemp425 + (fTemp426 + (fTemp427 + (fTemp428 + (fTemp302 + (fTemp429 + (fTemp430 + (fTemp431 + (fTemp432 + (fTemp307 + fTemp433)))))))))))) - (fTemp434 + (fTemp435 + (fTemp436 + (fTemp437 + (fTemp438 + (fTemp439 + (fTemp440 + (((fTemp315 + fTemp441) + fTemp442) + fTemp443))))))))));
			fRec174[0] = max((fRec174[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp444))))));
			fVbargraph62 = FAUSTFLOAT(fRec174[0]);
			output37[i] = FAUSTFLOAT(fTemp444);
			float fTemp445 = (fTemp307 + fTemp313);
			float fTemp446 = (fRec1[0] * ((fTemp224 + (fTemp310 + (fTemp296 + (fTemp297 + (fTemp298 + (fTemp299 + (fTemp300 + (fTemp317 + (fTemp302 + (fTemp303 + (fTemp314 + (fTemp308 + fTemp305)))))))))))) - (fTemp293 + (fTemp294 + (fTemp295 + (fTemp311 + (fTemp312 + (fTemp301 + (fTemp318 + (fTemp304 + (fTemp316 + (fTemp306 + fTemp445))))))))))));
			fRec175[0] = max((fRec175[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp446))))));
			fVbargraph63 = FAUSTFLOAT(fRec175[0]);
			output38[i] = FAUSTFLOAT(fTemp446);
			float fTemp447 = (fRec1[0] * ((fTemp224 + (fTemp328 + (fTemp329 + (fTemp322 + (fTemp323 + (fTemp324 + (fTemp339 + (fTemp325 + (fTemp338 + (fTemp302 + (fTemp336 + (fTemp314 + fTemp327)))))))))))) - (fTemp320 + (fTemp321 + (fTemp330 + (fTemp331 + (fTemp332 + (fTemp333 + (fTemp337 + (fTemp335 + (fTemp334 + (fTemp326 + fTemp445))))))))))));
			fRec176[0] = max((fRec176[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp447))))));
			fVbargraph64 = FAUSTFLOAT(fRec176[0]);
			output39[i] = FAUSTFLOAT(fTemp447);
			float fTemp448 = (fRec1[0] * ((fTemp224 + (fTemp349 + (fTemp350 + (fTemp351 + (fTemp342 + (fTemp333 + (fTemp322 + (fTemp343 + (fTemp345 + (fTemp302 + (fTemp347 + (fTemp336 + (fTemp348 + (fTemp314 + fTemp356)))))))))))))) - (fTemp341 + (fTemp352 + (fTemp353 + (fTemp354 + (fTemp344 + (fTemp346 + (fTemp357 + (fTemp313 + (fTemp307 + fTemp355)))))))))));
			fRec177[0] = max((fRec177[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp448))))));
			fVbargraph65 = FAUSTFLOAT(fRec177[0]);
			output40[i] = FAUSTFLOAT(fTemp448);
			float fTemp449 = (fTemp313 + fTemp309);
			float fTemp450 = (fRec1[0] * ((fTemp224 + (fTemp367 + (fTemp294 + (fTemp361 + (fTemp362 + (fTemp363 + (fTemp364 + (fTemp372 + (fTemp302 + (fTemp316 + (fTemp314 + (fTemp306 + fTemp366)))))))))))) - (fTemp359 + (fTemp360 + (fTemp368 + (fTemp297 + (fTemp369 + (fTemp318 + (fTemp317 + (fTemp371 + (fTemp365 + fTemp449)))))))))));
			fRec178[0] = max((fRec178[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp450))))));
			fVbargraph66 = FAUSTFLOAT(fRec178[0]);
			output41[i] = FAUSTFLOAT(fTemp450);
			float fTemp451 = (fRec1[0] * ((fTemp224 + (fTemp293 + (fTemp383 + (fTemp376 + (fTemp377 + (fTemp378 + (fTemp379 + (fTemp390 + (fTemp302 + (fTemp316 + (fTemp314 + (fTemp382 + fTemp387)))))))))))) - (fTemp374 + (fTemp375 + (fTemp384 + (fTemp385 + (fTemp386 + (fTemp389 + (fTemp380 + (fTemp388 + (fTemp381 + fTemp449)))))))))));
			fRec179[0] = max((fRec179[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp451))))));
			fVbargraph67 = FAUSTFLOAT(fRec179[0]);
			output42[i] = FAUSTFLOAT(fTemp451);
			float fTemp452 = (fRec1[0] * ((fTemp224 + (fTemp321 + (fTemp331 + (fTemp394 + (fTemp395 + (fTemp396 + (fTemp398 + (fTemp302 + (fTemp400 + (fTemp381 + (fTemp401 + (fTemp314 + fTemp404)))))))))))) - (fTemp392 + (fTemp329 + (fTemp393 + (fTemp402 + (fTemp377 + (fTemp322 + (fTemp397 + (fTemp399 + (fTemp405 + (fTemp313 + (fTemp307 + fTemp403)))))))))))));
			fRec180[0] = max((fRec180[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp452))))));
			fVbargraph68 = FAUSTFLOAT(fRec180[0]);
			output43[i] = FAUSTFLOAT(fTemp452);
			float fTemp453 = (fRec1[0] * ((fTemp224 + (fTemp415 + (fTemp341 + (fTemp408 + (fTemp352 + (fTemp409 + (fTemp410 + (fTemp411 + (fTemp422 + (fTemp412 + (fTemp421 + (fTemp302 + (fTemp419 + (fTemp314 + fTemp414)))))))))))))) - (fTemp407 + (fTemp350 + (fTemp322 + (fTemp416 + (fTemp420 + (fTemp418 + (fTemp417 + (fTemp413 + fTemp445))))))))));
			fRec181[0] = max((fRec181[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp453))))));
			fVbargraph69 = FAUSTFLOAT(fRec181[0]);
			output44[i] = FAUSTFLOAT(fTemp453);
			float fTemp454 = (fRec1[0] * ((fTemp224 + (fTemp434 + (fTemp435 + (fTemp436 + (fTemp425 + (fTemp426 + (fTemp427 + (fTemp442 + (fTemp302 + (fTemp429 + (fTemp314 + (fTemp433 + fTemp431)))))))))))) - (fTemp424 + (fTemp437 + (fTemp438 + (fTemp439 + (fTemp440 + (fTemp428 + (fTemp443 + (fTemp430 + (fTemp441 + (fTemp432 + fTemp445))))))))))));
			fRec182[0] = max((fRec182[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp454))))));
			fVbargraph70 = FAUSTFLOAT(fRec182[0]);
			output45[i] = FAUSTFLOAT(fTemp454);
			float fTemp455 = (fRec1[0] * ((fTemp224 + (fTemp225 + (fTemp230 + (fTemp231 + (fTemp232 + (fTemp233 + (fTemp247 + (fTemp235 + (fTemp244 + (fTemp239 + fTemp236)))))))))) - (fTemp226 + (fTemp227 + (fTemp228 + (fTemp229 + (fTemp242 + (fTemp248 + (fTemp246 + (fTemp234 + (fTemp245 + (fTemp237 + (fTemp238 + fTemp243)))))))))))));
			fRec183[0] = max((fRec183[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp455))))));
			fVbargraph71 = FAUSTFLOAT(fRec183[0]);
			output46[i] = FAUSTFLOAT(fTemp455);
			float fTemp456 = (fRec1[0] * ((fTemp224 + (fTemp255 + (fTemp229 + (fTemp231 + (fTemp232 + (fTemp251 + (fTemp234 + (fTemp235 + (fTemp253 + (fTemp239 + fTemp257)))))))))) - (fTemp254 + (fTemp250 + (fTemp228 + (fTemp230 + (fTemp233 + (fTemp248 + (fTemp252 + (fTemp258 + (fTemp236 + (fTemp237 + (fTemp238 + fTemp256)))))))))))));
			fRec184[0] = max((fRec184[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp456))))));
			fVbargraph72 = FAUSTFLOAT(fRec184[0]);
			output47[i] = FAUSTFLOAT(fTemp456);
			float fTemp457 = (fRec1[0] * ((fTemp224 + (fTemp266 + (fTemp267 + (fTemp268 + (fTemp230 + (fTemp260 + (fTemp233 + (fTemp262 + (fTemp275 + (fTemp235 + (fTemp263 + (fTemp265 + (fTemp239 + fTemp273))))))))))))) - (fTemp229 + (fTemp269 + (fTemp270 + (fTemp271 + (fTemp248 + (fTemp261 + (fTemp264 + (fTemp274 + (fTemp237 + (fTemp238 + fTemp272))))))))))));
			fRec185[0] = max((fRec185[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp457))))));
			fVbargraph73 = FAUSTFLOAT(fRec185[0]);
			output48[i] = FAUSTFLOAT(fTemp457);
			float fTemp458 = (fRec1[0] * ((fTemp224 + (fTemp282 + (fTemp228 + (fTemp229 + (fTemp283 + (fTemp277 + (fTemp291 + (fTemp235 + (fTemp288 + (fTemp279 + (fTemp239 + fTemp281))))))))))) - (fTemp230 + (fTemp284 + (fTemp285 + (fTemp286 + (fTemp233 + (fTemp248 + (fTemp290 + (fTemp278 + (fTemp289 + (fTemp237 + (fTemp287 + (fTemp238 + fTemp280))))))))))))));
			fRec186[0] = max((fRec186[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp458))))));
			fVbargraph74 = FAUSTFLOAT(fRec186[0]);
			output49[i] = FAUSTFLOAT(fTemp458);
			fRec1[1] = fRec1[0];
			fRec3[1] = fRec3[0];
			fRec2[1] = fRec2[0];
			fRec6[1] = fRec6[0];
			fRec7[1] = fRec7[0];
			fRec9[1] = fRec9[0];
			fRec10[1] = fRec10[0];
			fRec8[1] = fRec8[0];
			fRec5[1] = fRec5[0];
			fRec4[1] = fRec4[0];
			fRec13[1] = fRec13[0];
			fRec14[1] = fRec14[0];
			fRec16[1] = fRec16[0];
			fRec17[1] = fRec17[0];
			fRec15[1] = fRec15[0];
			fRec12[1] = fRec12[0];
			fRec11[1] = fRec11[0];
			fRec20[1] = fRec20[0];
			fRec21[1] = fRec21[0];
			fRec23[1] = fRec23[0];
			fRec24[1] = fRec24[0];
			fRec22[1] = fRec22[0];
			fRec19[1] = fRec19[0];
			fRec18[1] = fRec18[0];
			fRec27[1] = fRec27[0];
			fRec28[1] = fRec28[0];
			fRec30[1] = fRec30[0];
			fRec31[1] = fRec31[0];
			fRec29[1] = fRec29[0];
			fRec26[1] = fRec26[0];
			fRec25[1] = fRec25[0];
			fRec35[1] = fRec35[0];
			fRec36[1] = fRec36[0];
			fRec34[1] = fRec34[0];
			fRec37[1] = fRec37[0];
			fRec38[1] = fRec38[0];
			fRec33[1] = fRec33[0];
			fRec32[1] = fRec32[0];
			fRec42[1] = fRec42[0];
			fRec43[1] = fRec43[0];
			fRec41[1] = fRec41[0];
			fRec44[1] = fRec44[0];
			fRec40[1] = fRec40[0];
			fRec39[1] = fRec39[0];
			fRec48[1] = fRec48[0];
			fRec49[1] = fRec49[0];
			fRec47[1] = fRec47[0];
			fRec50[1] = fRec50[0];
			fRec46[1] = fRec46[0];
			fRec45[1] = fRec45[0];
			fRec54[1] = fRec54[0];
			fRec55[1] = fRec55[0];
			fRec53[1] = fRec53[0];
			fRec56[1] = fRec56[0];
			fRec52[1] = fRec52[0];
			fRec51[1] = fRec51[0];
			fRec60[1] = fRec60[0];
			fRec61[1] = fRec61[0];
			fRec59[1] = fRec59[0];
			fRec62[1] = fRec62[0];
			fRec58[1] = fRec58[0];
			fRec57[1] = fRec57[0];
			fRec65[1] = fRec65[0];
			fRec66[1] = fRec66[0];
			fRec64[1] = fRec64[0];
			fRec63[1] = fRec63[0];
			fRec69[1] = fRec69[0];
			fRec70[1] = fRec70[0];
			fRec68[1] = fRec68[0];
			fRec67[1] = fRec67[0];
			fRec73[1] = fRec73[0];
			fRec74[1] = fRec74[0];
			fRec72[1] = fRec72[0];
			fRec71[1] = fRec71[0];
			fRec77[1] = fRec77[0];
			fRec76[1] = fRec76[0];
			fRec75[1] = fRec75[0];
			fRec80[1] = fRec80[0];
			fRec79[1] = fRec79[0];
			fRec78[1] = fRec78[0];
			fRec0[1] = fRec0[0];
			fRec81[1] = fRec81[0];
			fRec85[1] = fRec85[0];
			fRec84[1] = fRec84[0];
			fRec83[1] = fRec83[0];
			fRec88[1] = fRec88[0];
			fRec89[1] = fRec89[0];
			fRec87[1] = fRec87[0];
			fRec86[1] = fRec86[0];
			fRec92[1] = fRec92[0];
			fRec93[1] = fRec93[0];
			fRec95[1] = fRec95[0];
			fRec96[1] = fRec96[0];
			fRec94[1] = fRec94[0];
			fRec91[1] = fRec91[0];
			fRec90[1] = fRec90[0];
			fRec100[1] = fRec100[0];
			fRec101[1] = fRec101[0];
			fRec99[1] = fRec99[0];
			fRec102[1] = fRec102[0];
			fRec98[1] = fRec98[0];
			fRec97[1] = fRec97[0];
			fRec106[1] = fRec106[0];
			fRec107[1] = fRec107[0];
			fRec105[1] = fRec105[0];
			fRec108[1] = fRec108[0];
			fRec104[1] = fRec104[0];
			fRec103[1] = fRec103[0];
			fRec111[1] = fRec111[0];
			fRec112[1] = fRec112[0];
			fRec114[1] = fRec114[0];
			fRec115[1] = fRec115[0];
			fRec113[1] = fRec113[0];
			fRec110[1] = fRec110[0];
			fRec109[1] = fRec109[0];
			fRec82[1] = fRec82[0];
			fRec116[1] = fRec116[0];
			fRec117[1] = fRec117[0];
			fRec118[1] = fRec118[0];
			fRec119[1] = fRec119[0];
			fRec123[1] = fRec123[0];
			fRec124[1] = fRec124[0];
			fRec126[1] = fRec126[0];
			fRec127[1] = fRec127[0];
			fRec125[1] = fRec125[0];
			fRec122[1] = fRec122[0];
			fRec121[1] = fRec121[0];
			fRec131[1] = fRec131[0];
			fRec132[1] = fRec132[0];
			fRec130[1] = fRec130[0];
			fRec133[1] = fRec133[0];
			fRec129[1] = fRec129[0];
			fRec128[1] = fRec128[0];
			fRec136[1] = fRec136[0];
			fRec137[1] = fRec137[0];
			fRec135[1] = fRec135[0];
			fRec134[1] = fRec134[0];
			fRec140[1] = fRec140[0];
			fRec141[1] = fRec141[0];
			fRec143[1] = fRec143[0];
			fRec144[1] = fRec144[0];
			fRec142[1] = fRec142[0];
			fRec139[1] = fRec139[0];
			fRec138[1] = fRec138[0];
			fRec120[1] = fRec120[0];
			fRec145[1] = fRec145[0];
			fRec146[1] = fRec146[0];
			fRec147[1] = fRec147[0];
			fRec148[1] = fRec148[0];
			fRec149[1] = fRec149[0];
			fRec150[1] = fRec150[0];
			fRec151[1] = fRec151[0];
			fRec152[1] = fRec152[0];
			fRec153[1] = fRec153[0];
			fRec154[1] = fRec154[0];
			fRec155[1] = fRec155[0];
			fRec156[1] = fRec156[0];
			fRec157[1] = fRec157[0];
			fRec158[1] = fRec158[0];
			fRec159[1] = fRec159[0];
			fRec160[1] = fRec160[0];
			fRec161[1] = fRec161[0];
			fRec162[1] = fRec162[0];
			fRec163[1] = fRec163[0];
			fRec164[1] = fRec164[0];
			fRec165[1] = fRec165[0];
			fRec166[1] = fRec166[0];
			fRec167[1] = fRec167[0];
			fRec168[1] = fRec168[0];
			fRec169[1] = fRec169[0];
			fRec170[1] = fRec170[0];
			fRec171[1] = fRec171[0];
			fRec172[1] = fRec172[0];
			fRec173[1] = fRec173[0];
			fRec174[1] = fRec174[0];
			fRec175[1] = fRec175[0];
			fRec176[1] = fRec176[0];
			fRec177[1] = fRec177[0];
			fRec178[1] = fRec178[0];
			fRec179[1] = fRec179[0];
			fRec180[1] = fRec180[0];
			fRec181[1] = fRec181[0];
			fRec182[1] = fRec182[0];
			fRec183[1] = fRec183[0];
			fRec184[1] = fRec184[0];
			fRec185[1] = fRec185[0];
			fRec186[1] = fRec186[0];
			
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
