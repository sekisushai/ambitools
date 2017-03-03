/* ------------------------------------------------------------
author: "Pierre Lecomte"
copyright: "(c) Pierre Lecomte 2014"
license: "GPL"
name: "HOADecLebedev505"
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
	float fRec11[2];
	float fRec10[2];
	float fRec9[2];
	float fRec6[2];
	float fRec12[2];
	float fRec5[2];
	float fRec4[2];
	float fRec16[2];
	float fRec17[2];
	float fRec20[2];
	float fRec19[2];
	float fRec18[2];
	float fRec15[2];
	float fRec21[2];
	float fRec14[2];
	float fRec13[2];
	float fRec25[2];
	float fRec26[2];
	float fRec29[2];
	float fRec28[2];
	float fRec27[2];
	float fRec24[2];
	float fRec30[2];
	float fRec23[2];
	float fRec22[2];
	float fRec34[2];
	float fRec35[2];
	float fRec38[2];
	float fRec37[2];
	float fRec36[2];
	float fRec33[2];
	float fRec39[2];
	float fRec32[2];
	float fRec31[2];
	float fRec43[2];
	float fRec44[2];
	float fRec47[2];
	float fRec46[2];
	float fRec45[2];
	float fRec42[2];
	float fRec48[2];
	float fRec41[2];
	float fRec40[2];
	float fRec54[2];
	float fRec53[2];
	float fRec52[2];
	float fRec55[2];
	float fRec56[2];
	float fRec51[2];
	float fRec57[2];
	float fRec50[2];
	float fRec49[2];
	float fRec60[2];
	float fRec61[2];
	float fRec63[2];
	float fRec64[2];
	float fRec62[2];
	float fRec59[2];
	float fRec58[2];
	float fRec67[2];
	float fRec68[2];
	float fRec70[2];
	float fRec71[2];
	float fRec69[2];
	float fRec66[2];
	float fRec65[2];
	float fRec74[2];
	float fRec75[2];
	float fRec77[2];
	float fRec78[2];
	float fRec76[2];
	float fRec73[2];
	float fRec72[2];
	float fRec81[2];
	float fRec82[2];
	float fRec84[2];
	float fRec85[2];
	float fRec83[2];
	float fRec80[2];
	float fRec79[2];
	float fRec89[2];
	float fRec90[2];
	float fRec88[2];
	float fRec91[2];
	float fRec92[2];
	float fRec87[2];
	float fRec86[2];
	float fRec96[2];
	float fRec97[2];
	float fRec95[2];
	float fRec98[2];
	float fRec94[2];
	float fRec93[2];
	float fRec102[2];
	float fRec103[2];
	float fRec101[2];
	float fRec104[2];
	float fRec100[2];
	float fRec99[2];
	float fRec108[2];
	float fRec109[2];
	float fRec107[2];
	float fRec110[2];
	float fRec106[2];
	float fRec105[2];
	float fRec114[2];
	float fRec115[2];
	float fRec113[2];
	float fRec116[2];
	float fRec112[2];
	float fRec111[2];
	float fRec119[2];
	float fRec120[2];
	float fRec118[2];
	float fRec117[2];
	float fRec123[2];
	float fRec124[2];
	float fRec122[2];
	float fRec121[2];
	float fRec127[2];
	float fRec128[2];
	float fRec126[2];
	float fRec125[2];
	float fRec131[2];
	float fRec130[2];
	float fRec129[2];
	float fRec134[2];
	float fRec133[2];
	float fRec132[2];
	float fRec0[2];
	float fRec135[2];
	float fRec140[2];
	float fRec141[2];
	float fRec144[2];
	float fRec143[2];
	float fRec142[2];
	float fRec139[2];
	float fRec145[2];
	float fRec138[2];
	float fRec137[2];
	float fRec148[2];
	float fRec147[2];
	float fRec146[2];
	float fRec151[2];
	float fRec152[2];
	float fRec150[2];
	float fRec149[2];
	float fRec156[2];
	float fRec157[2];
	float fRec160[2];
	float fRec159[2];
	float fRec158[2];
	float fRec155[2];
	float fRec161[2];
	float fRec154[2];
	float fRec153[2];
	float fRec165[2];
	float fRec166[2];
	float fRec169[2];
	float fRec168[2];
	float fRec167[2];
	float fRec164[2];
	float fRec170[2];
	float fRec163[2];
	float fRec162[2];
	float fRec173[2];
	float fRec174[2];
	float fRec176[2];
	float fRec177[2];
	float fRec175[2];
	float fRec172[2];
	float fRec171[2];
	float fRec181[2];
	float fRec182[2];
	float fRec180[2];
	float fRec183[2];
	float fRec179[2];
	float fRec178[2];
	float fRec187[2];
	float fRec188[2];
	float fRec186[2];
	float fRec189[2];
	float fRec185[2];
	float fRec184[2];
	float fRec192[2];
	float fRec193[2];
	float fRec195[2];
	float fRec196[2];
	float fRec194[2];
	float fRec191[2];
	float fRec190[2];
	float fRec136[2];
	float fRec197[2];
	float fRec198[2];
	float fRec199[2];
	float fRec200[2];
	float fRec204[2];
	float fRec205[2];
	float fRec207[2];
	float fRec208[2];
	float fRec206[2];
	float fRec203[2];
	float fRec202[2];
	float fRec212[2];
	float fRec213[2];
	float fRec216[2];
	float fRec215[2];
	float fRec214[2];
	float fRec211[2];
	float fRec217[2];
	float fRec210[2];
	float fRec209[2];
	float fRec221[2];
	float fRec222[2];
	float fRec220[2];
	float fRec223[2];
	float fRec219[2];
	float fRec218[2];
	float fRec226[2];
	float fRec227[2];
	float fRec225[2];
	float fRec224[2];
	float fRec230[2];
	float fRec231[2];
	float fRec233[2];
	float fRec234[2];
	float fRec232[2];
	float fRec229[2];
	float fRec228[2];
	float fRec238[2];
	float fRec239[2];
	float fRec242[2];
	float fRec241[2];
	float fRec240[2];
	float fRec237[2];
	float fRec243[2];
	float fRec236[2];
	float fRec235[2];
	float fRec201[2];
	float fRec244[2];
	float fRec245[2];
	float fRec246[2];
	float fRec247[2];
	float fRec248[2];
	float fRec249[2];
	float fRec250[2];
	float fRec251[2];
	float fRec252[2];
	float fRec253[2];
	float fRec254[2];
	float fRec255[2];
	float fRec256[2];
	float fRec257[2];
	float fRec258[2];
	float fRec259[2];
	float fRec260[2];
	float fRec261[2];
	float fRec262[2];
	float fRec263[2];
	float fRec264[2];
	float fRec265[2];
	float fRec266[2];
	float fRec267[2];
	float fRec268[2];
	float fRec269[2];
	float fRec270[2];
	float fRec271[2];
	float fRec272[2];
	float fRec273[2];
	float fRec274[2];
	float fRec275[2];
	float fRec276[2];
	float fRec277[2];
	float fRec278[2];
	float fRec279[2];
	float fRec280[2];
	float fRec281[2];
	float fRec282[2];
	float fRec283[2];
	float fRec284[2];
	float fRec285[2];
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
	float fConst7;
	float fConst8;
	FAUSTFLOAT fVbargraph1;
	FAUSTFLOAT fVbargraph2;
	FAUSTFLOAT fVbargraph3;
	FAUSTFLOAT fVbargraph4;
	FAUSTFLOAT fVbargraph5;
	FAUSTFLOAT fVbargraph6;
	float fConst9;
	float fConst10;
	float fConst11;
	float fConst12;
	FAUSTFLOAT fVbargraph7;
	FAUSTFLOAT fVbargraph8;
	FAUSTFLOAT fVbargraph9;
	FAUSTFLOAT fVbargraph10;
	FAUSTFLOAT fVbargraph11;
	float fConst13;
	float fConst14;
	float fConst15;
	float fConst16;
	FAUSTFLOAT fVbargraph12;
	FAUSTFLOAT fVbargraph13;
	FAUSTFLOAT fVbargraph14;
	FAUSTFLOAT fVbargraph15;
	float fConst17;
	float fConst18;
	FAUSTFLOAT fVbargraph16;
	FAUSTFLOAT fVbargraph17;
	FAUSTFLOAT fVbargraph18;
	float fConst19;
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
	FAUSTFLOAT fVbargraph75;
	FAUSTFLOAT fVbargraph76;
	FAUSTFLOAT fVbargraph77;
	FAUSTFLOAT fVbargraph78;
	FAUSTFLOAT fVbargraph79;
	FAUSTFLOAT fVbargraph80;
	FAUSTFLOAT fVbargraph81;
	FAUSTFLOAT fVbargraph82;
	FAUSTFLOAT fVbargraph83;
	FAUSTFLOAT fVbargraph84;
	FAUSTFLOAT fVbargraph85;
	
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
		m->declare("name", "HOADecLebedev505");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
		m->declare("version", "1.0");
	}

	virtual int getNumInputs() {
		return 36;
		
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
		fConst2 = (619.945557f / fConst0);
		fConst3 = (1139.66516f / fConst0);
		fConst4 = (170.0f / fConst0);
		fConst5 = (2279.33032f / fConst0);
		fConst6 = (790.389282f / fConst0);
		fConst7 = (1580.77856f / fConst0);
		fConst8 = (1239.89111f / fConst0);
		fConst9 = (984.711609f / fConst0);
		fConst10 = (1969.42322f / fConst0);
		fConst11 = (715.288391f / fConst0);
		fConst12 = (1430.57678f / fConst0);
		fConst13 = (394.771515f / fConst0);
		fConst14 = (625.228516f / fConst0);
		fConst15 = (1250.45703f / fConst0);
		fConst16 = (789.54303f / fConst0);
		fConst17 = (510.0f / fConst0);
		fConst18 = (1020.0f / fConst0);
		fConst19 = (340.0f / fConst0);
		
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
			fRec11[i5] = 0.0f;
			
		}
		for (int i6 = 0; (i6 < 2); i6 = (i6 + 1)) {
			fRec10[i6] = 0.0f;
			
		}
		for (int i7 = 0; (i7 < 2); i7 = (i7 + 1)) {
			fRec9[i7] = 0.0f;
			
		}
		for (int i8 = 0; (i8 < 2); i8 = (i8 + 1)) {
			fRec6[i8] = 0.0f;
			
		}
		for (int i9 = 0; (i9 < 2); i9 = (i9 + 1)) {
			fRec12[i9] = 0.0f;
			
		}
		for (int i10 = 0; (i10 < 2); i10 = (i10 + 1)) {
			fRec5[i10] = 0.0f;
			
		}
		for (int i11 = 0; (i11 < 2); i11 = (i11 + 1)) {
			fRec4[i11] = 0.0f;
			
		}
		for (int i12 = 0; (i12 < 2); i12 = (i12 + 1)) {
			fRec16[i12] = 0.0f;
			
		}
		for (int i13 = 0; (i13 < 2); i13 = (i13 + 1)) {
			fRec17[i13] = 0.0f;
			
		}
		for (int i14 = 0; (i14 < 2); i14 = (i14 + 1)) {
			fRec20[i14] = 0.0f;
			
		}
		for (int i15 = 0; (i15 < 2); i15 = (i15 + 1)) {
			fRec19[i15] = 0.0f;
			
		}
		for (int i16 = 0; (i16 < 2); i16 = (i16 + 1)) {
			fRec18[i16] = 0.0f;
			
		}
		for (int i17 = 0; (i17 < 2); i17 = (i17 + 1)) {
			fRec15[i17] = 0.0f;
			
		}
		for (int i18 = 0; (i18 < 2); i18 = (i18 + 1)) {
			fRec21[i18] = 0.0f;
			
		}
		for (int i19 = 0; (i19 < 2); i19 = (i19 + 1)) {
			fRec14[i19] = 0.0f;
			
		}
		for (int i20 = 0; (i20 < 2); i20 = (i20 + 1)) {
			fRec13[i20] = 0.0f;
			
		}
		for (int i21 = 0; (i21 < 2); i21 = (i21 + 1)) {
			fRec25[i21] = 0.0f;
			
		}
		for (int i22 = 0; (i22 < 2); i22 = (i22 + 1)) {
			fRec26[i22] = 0.0f;
			
		}
		for (int i23 = 0; (i23 < 2); i23 = (i23 + 1)) {
			fRec29[i23] = 0.0f;
			
		}
		for (int i24 = 0; (i24 < 2); i24 = (i24 + 1)) {
			fRec28[i24] = 0.0f;
			
		}
		for (int i25 = 0; (i25 < 2); i25 = (i25 + 1)) {
			fRec27[i25] = 0.0f;
			
		}
		for (int i26 = 0; (i26 < 2); i26 = (i26 + 1)) {
			fRec24[i26] = 0.0f;
			
		}
		for (int i27 = 0; (i27 < 2); i27 = (i27 + 1)) {
			fRec30[i27] = 0.0f;
			
		}
		for (int i28 = 0; (i28 < 2); i28 = (i28 + 1)) {
			fRec23[i28] = 0.0f;
			
		}
		for (int i29 = 0; (i29 < 2); i29 = (i29 + 1)) {
			fRec22[i29] = 0.0f;
			
		}
		for (int i30 = 0; (i30 < 2); i30 = (i30 + 1)) {
			fRec34[i30] = 0.0f;
			
		}
		for (int i31 = 0; (i31 < 2); i31 = (i31 + 1)) {
			fRec35[i31] = 0.0f;
			
		}
		for (int i32 = 0; (i32 < 2); i32 = (i32 + 1)) {
			fRec38[i32] = 0.0f;
			
		}
		for (int i33 = 0; (i33 < 2); i33 = (i33 + 1)) {
			fRec37[i33] = 0.0f;
			
		}
		for (int i34 = 0; (i34 < 2); i34 = (i34 + 1)) {
			fRec36[i34] = 0.0f;
			
		}
		for (int i35 = 0; (i35 < 2); i35 = (i35 + 1)) {
			fRec33[i35] = 0.0f;
			
		}
		for (int i36 = 0; (i36 < 2); i36 = (i36 + 1)) {
			fRec39[i36] = 0.0f;
			
		}
		for (int i37 = 0; (i37 < 2); i37 = (i37 + 1)) {
			fRec32[i37] = 0.0f;
			
		}
		for (int i38 = 0; (i38 < 2); i38 = (i38 + 1)) {
			fRec31[i38] = 0.0f;
			
		}
		for (int i39 = 0; (i39 < 2); i39 = (i39 + 1)) {
			fRec43[i39] = 0.0f;
			
		}
		for (int i40 = 0; (i40 < 2); i40 = (i40 + 1)) {
			fRec44[i40] = 0.0f;
			
		}
		for (int i41 = 0; (i41 < 2); i41 = (i41 + 1)) {
			fRec47[i41] = 0.0f;
			
		}
		for (int i42 = 0; (i42 < 2); i42 = (i42 + 1)) {
			fRec46[i42] = 0.0f;
			
		}
		for (int i43 = 0; (i43 < 2); i43 = (i43 + 1)) {
			fRec45[i43] = 0.0f;
			
		}
		for (int i44 = 0; (i44 < 2); i44 = (i44 + 1)) {
			fRec42[i44] = 0.0f;
			
		}
		for (int i45 = 0; (i45 < 2); i45 = (i45 + 1)) {
			fRec48[i45] = 0.0f;
			
		}
		for (int i46 = 0; (i46 < 2); i46 = (i46 + 1)) {
			fRec41[i46] = 0.0f;
			
		}
		for (int i47 = 0; (i47 < 2); i47 = (i47 + 1)) {
			fRec40[i47] = 0.0f;
			
		}
		for (int i48 = 0; (i48 < 2); i48 = (i48 + 1)) {
			fRec54[i48] = 0.0f;
			
		}
		for (int i49 = 0; (i49 < 2); i49 = (i49 + 1)) {
			fRec53[i49] = 0.0f;
			
		}
		for (int i50 = 0; (i50 < 2); i50 = (i50 + 1)) {
			fRec52[i50] = 0.0f;
			
		}
		for (int i51 = 0; (i51 < 2); i51 = (i51 + 1)) {
			fRec55[i51] = 0.0f;
			
		}
		for (int i52 = 0; (i52 < 2); i52 = (i52 + 1)) {
			fRec56[i52] = 0.0f;
			
		}
		for (int i53 = 0; (i53 < 2); i53 = (i53 + 1)) {
			fRec51[i53] = 0.0f;
			
		}
		for (int i54 = 0; (i54 < 2); i54 = (i54 + 1)) {
			fRec57[i54] = 0.0f;
			
		}
		for (int i55 = 0; (i55 < 2); i55 = (i55 + 1)) {
			fRec50[i55] = 0.0f;
			
		}
		for (int i56 = 0; (i56 < 2); i56 = (i56 + 1)) {
			fRec49[i56] = 0.0f;
			
		}
		for (int i57 = 0; (i57 < 2); i57 = (i57 + 1)) {
			fRec60[i57] = 0.0f;
			
		}
		for (int i58 = 0; (i58 < 2); i58 = (i58 + 1)) {
			fRec61[i58] = 0.0f;
			
		}
		for (int i59 = 0; (i59 < 2); i59 = (i59 + 1)) {
			fRec63[i59] = 0.0f;
			
		}
		for (int i60 = 0; (i60 < 2); i60 = (i60 + 1)) {
			fRec64[i60] = 0.0f;
			
		}
		for (int i61 = 0; (i61 < 2); i61 = (i61 + 1)) {
			fRec62[i61] = 0.0f;
			
		}
		for (int i62 = 0; (i62 < 2); i62 = (i62 + 1)) {
			fRec59[i62] = 0.0f;
			
		}
		for (int i63 = 0; (i63 < 2); i63 = (i63 + 1)) {
			fRec58[i63] = 0.0f;
			
		}
		for (int i64 = 0; (i64 < 2); i64 = (i64 + 1)) {
			fRec67[i64] = 0.0f;
			
		}
		for (int i65 = 0; (i65 < 2); i65 = (i65 + 1)) {
			fRec68[i65] = 0.0f;
			
		}
		for (int i66 = 0; (i66 < 2); i66 = (i66 + 1)) {
			fRec70[i66] = 0.0f;
			
		}
		for (int i67 = 0; (i67 < 2); i67 = (i67 + 1)) {
			fRec71[i67] = 0.0f;
			
		}
		for (int i68 = 0; (i68 < 2); i68 = (i68 + 1)) {
			fRec69[i68] = 0.0f;
			
		}
		for (int i69 = 0; (i69 < 2); i69 = (i69 + 1)) {
			fRec66[i69] = 0.0f;
			
		}
		for (int i70 = 0; (i70 < 2); i70 = (i70 + 1)) {
			fRec65[i70] = 0.0f;
			
		}
		for (int i71 = 0; (i71 < 2); i71 = (i71 + 1)) {
			fRec74[i71] = 0.0f;
			
		}
		for (int i72 = 0; (i72 < 2); i72 = (i72 + 1)) {
			fRec75[i72] = 0.0f;
			
		}
		for (int i73 = 0; (i73 < 2); i73 = (i73 + 1)) {
			fRec77[i73] = 0.0f;
			
		}
		for (int i74 = 0; (i74 < 2); i74 = (i74 + 1)) {
			fRec78[i74] = 0.0f;
			
		}
		for (int i75 = 0; (i75 < 2); i75 = (i75 + 1)) {
			fRec76[i75] = 0.0f;
			
		}
		for (int i76 = 0; (i76 < 2); i76 = (i76 + 1)) {
			fRec73[i76] = 0.0f;
			
		}
		for (int i77 = 0; (i77 < 2); i77 = (i77 + 1)) {
			fRec72[i77] = 0.0f;
			
		}
		for (int i78 = 0; (i78 < 2); i78 = (i78 + 1)) {
			fRec81[i78] = 0.0f;
			
		}
		for (int i79 = 0; (i79 < 2); i79 = (i79 + 1)) {
			fRec82[i79] = 0.0f;
			
		}
		for (int i80 = 0; (i80 < 2); i80 = (i80 + 1)) {
			fRec84[i80] = 0.0f;
			
		}
		for (int i81 = 0; (i81 < 2); i81 = (i81 + 1)) {
			fRec85[i81] = 0.0f;
			
		}
		for (int i82 = 0; (i82 < 2); i82 = (i82 + 1)) {
			fRec83[i82] = 0.0f;
			
		}
		for (int i83 = 0; (i83 < 2); i83 = (i83 + 1)) {
			fRec80[i83] = 0.0f;
			
		}
		for (int i84 = 0; (i84 < 2); i84 = (i84 + 1)) {
			fRec79[i84] = 0.0f;
			
		}
		for (int i85 = 0; (i85 < 2); i85 = (i85 + 1)) {
			fRec89[i85] = 0.0f;
			
		}
		for (int i86 = 0; (i86 < 2); i86 = (i86 + 1)) {
			fRec90[i86] = 0.0f;
			
		}
		for (int i87 = 0; (i87 < 2); i87 = (i87 + 1)) {
			fRec88[i87] = 0.0f;
			
		}
		for (int i88 = 0; (i88 < 2); i88 = (i88 + 1)) {
			fRec91[i88] = 0.0f;
			
		}
		for (int i89 = 0; (i89 < 2); i89 = (i89 + 1)) {
			fRec92[i89] = 0.0f;
			
		}
		for (int i90 = 0; (i90 < 2); i90 = (i90 + 1)) {
			fRec87[i90] = 0.0f;
			
		}
		for (int i91 = 0; (i91 < 2); i91 = (i91 + 1)) {
			fRec86[i91] = 0.0f;
			
		}
		for (int i92 = 0; (i92 < 2); i92 = (i92 + 1)) {
			fRec96[i92] = 0.0f;
			
		}
		for (int i93 = 0; (i93 < 2); i93 = (i93 + 1)) {
			fRec97[i93] = 0.0f;
			
		}
		for (int i94 = 0; (i94 < 2); i94 = (i94 + 1)) {
			fRec95[i94] = 0.0f;
			
		}
		for (int i95 = 0; (i95 < 2); i95 = (i95 + 1)) {
			fRec98[i95] = 0.0f;
			
		}
		for (int i96 = 0; (i96 < 2); i96 = (i96 + 1)) {
			fRec94[i96] = 0.0f;
			
		}
		for (int i97 = 0; (i97 < 2); i97 = (i97 + 1)) {
			fRec93[i97] = 0.0f;
			
		}
		for (int i98 = 0; (i98 < 2); i98 = (i98 + 1)) {
			fRec102[i98] = 0.0f;
			
		}
		for (int i99 = 0; (i99 < 2); i99 = (i99 + 1)) {
			fRec103[i99] = 0.0f;
			
		}
		for (int i100 = 0; (i100 < 2); i100 = (i100 + 1)) {
			fRec101[i100] = 0.0f;
			
		}
		for (int i101 = 0; (i101 < 2); i101 = (i101 + 1)) {
			fRec104[i101] = 0.0f;
			
		}
		for (int i102 = 0; (i102 < 2); i102 = (i102 + 1)) {
			fRec100[i102] = 0.0f;
			
		}
		for (int i103 = 0; (i103 < 2); i103 = (i103 + 1)) {
			fRec99[i103] = 0.0f;
			
		}
		for (int i104 = 0; (i104 < 2); i104 = (i104 + 1)) {
			fRec108[i104] = 0.0f;
			
		}
		for (int i105 = 0; (i105 < 2); i105 = (i105 + 1)) {
			fRec109[i105] = 0.0f;
			
		}
		for (int i106 = 0; (i106 < 2); i106 = (i106 + 1)) {
			fRec107[i106] = 0.0f;
			
		}
		for (int i107 = 0; (i107 < 2); i107 = (i107 + 1)) {
			fRec110[i107] = 0.0f;
			
		}
		for (int i108 = 0; (i108 < 2); i108 = (i108 + 1)) {
			fRec106[i108] = 0.0f;
			
		}
		for (int i109 = 0; (i109 < 2); i109 = (i109 + 1)) {
			fRec105[i109] = 0.0f;
			
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
			fRec116[i113] = 0.0f;
			
		}
		for (int i114 = 0; (i114 < 2); i114 = (i114 + 1)) {
			fRec112[i114] = 0.0f;
			
		}
		for (int i115 = 0; (i115 < 2); i115 = (i115 + 1)) {
			fRec111[i115] = 0.0f;
			
		}
		for (int i116 = 0; (i116 < 2); i116 = (i116 + 1)) {
			fRec119[i116] = 0.0f;
			
		}
		for (int i117 = 0; (i117 < 2); i117 = (i117 + 1)) {
			fRec120[i117] = 0.0f;
			
		}
		for (int i118 = 0; (i118 < 2); i118 = (i118 + 1)) {
			fRec118[i118] = 0.0f;
			
		}
		for (int i119 = 0; (i119 < 2); i119 = (i119 + 1)) {
			fRec117[i119] = 0.0f;
			
		}
		for (int i120 = 0; (i120 < 2); i120 = (i120 + 1)) {
			fRec123[i120] = 0.0f;
			
		}
		for (int i121 = 0; (i121 < 2); i121 = (i121 + 1)) {
			fRec124[i121] = 0.0f;
			
		}
		for (int i122 = 0; (i122 < 2); i122 = (i122 + 1)) {
			fRec122[i122] = 0.0f;
			
		}
		for (int i123 = 0; (i123 < 2); i123 = (i123 + 1)) {
			fRec121[i123] = 0.0f;
			
		}
		for (int i124 = 0; (i124 < 2); i124 = (i124 + 1)) {
			fRec127[i124] = 0.0f;
			
		}
		for (int i125 = 0; (i125 < 2); i125 = (i125 + 1)) {
			fRec128[i125] = 0.0f;
			
		}
		for (int i126 = 0; (i126 < 2); i126 = (i126 + 1)) {
			fRec126[i126] = 0.0f;
			
		}
		for (int i127 = 0; (i127 < 2); i127 = (i127 + 1)) {
			fRec125[i127] = 0.0f;
			
		}
		for (int i128 = 0; (i128 < 2); i128 = (i128 + 1)) {
			fRec131[i128] = 0.0f;
			
		}
		for (int i129 = 0; (i129 < 2); i129 = (i129 + 1)) {
			fRec130[i129] = 0.0f;
			
		}
		for (int i130 = 0; (i130 < 2); i130 = (i130 + 1)) {
			fRec129[i130] = 0.0f;
			
		}
		for (int i131 = 0; (i131 < 2); i131 = (i131 + 1)) {
			fRec134[i131] = 0.0f;
			
		}
		for (int i132 = 0; (i132 < 2); i132 = (i132 + 1)) {
			fRec133[i132] = 0.0f;
			
		}
		for (int i133 = 0; (i133 < 2); i133 = (i133 + 1)) {
			fRec132[i133] = 0.0f;
			
		}
		for (int i134 = 0; (i134 < 2); i134 = (i134 + 1)) {
			fRec0[i134] = 0.0f;
			
		}
		for (int i135 = 0; (i135 < 2); i135 = (i135 + 1)) {
			fRec135[i135] = 0.0f;
			
		}
		for (int i136 = 0; (i136 < 2); i136 = (i136 + 1)) {
			fRec140[i136] = 0.0f;
			
		}
		for (int i137 = 0; (i137 < 2); i137 = (i137 + 1)) {
			fRec141[i137] = 0.0f;
			
		}
		for (int i138 = 0; (i138 < 2); i138 = (i138 + 1)) {
			fRec144[i138] = 0.0f;
			
		}
		for (int i139 = 0; (i139 < 2); i139 = (i139 + 1)) {
			fRec143[i139] = 0.0f;
			
		}
		for (int i140 = 0; (i140 < 2); i140 = (i140 + 1)) {
			fRec142[i140] = 0.0f;
			
		}
		for (int i141 = 0; (i141 < 2); i141 = (i141 + 1)) {
			fRec139[i141] = 0.0f;
			
		}
		for (int i142 = 0; (i142 < 2); i142 = (i142 + 1)) {
			fRec145[i142] = 0.0f;
			
		}
		for (int i143 = 0; (i143 < 2); i143 = (i143 + 1)) {
			fRec138[i143] = 0.0f;
			
		}
		for (int i144 = 0; (i144 < 2); i144 = (i144 + 1)) {
			fRec137[i144] = 0.0f;
			
		}
		for (int i145 = 0; (i145 < 2); i145 = (i145 + 1)) {
			fRec148[i145] = 0.0f;
			
		}
		for (int i146 = 0; (i146 < 2); i146 = (i146 + 1)) {
			fRec147[i146] = 0.0f;
			
		}
		for (int i147 = 0; (i147 < 2); i147 = (i147 + 1)) {
			fRec146[i147] = 0.0f;
			
		}
		for (int i148 = 0; (i148 < 2); i148 = (i148 + 1)) {
			fRec151[i148] = 0.0f;
			
		}
		for (int i149 = 0; (i149 < 2); i149 = (i149 + 1)) {
			fRec152[i149] = 0.0f;
			
		}
		for (int i150 = 0; (i150 < 2); i150 = (i150 + 1)) {
			fRec150[i150] = 0.0f;
			
		}
		for (int i151 = 0; (i151 < 2); i151 = (i151 + 1)) {
			fRec149[i151] = 0.0f;
			
		}
		for (int i152 = 0; (i152 < 2); i152 = (i152 + 1)) {
			fRec156[i152] = 0.0f;
			
		}
		for (int i153 = 0; (i153 < 2); i153 = (i153 + 1)) {
			fRec157[i153] = 0.0f;
			
		}
		for (int i154 = 0; (i154 < 2); i154 = (i154 + 1)) {
			fRec160[i154] = 0.0f;
			
		}
		for (int i155 = 0; (i155 < 2); i155 = (i155 + 1)) {
			fRec159[i155] = 0.0f;
			
		}
		for (int i156 = 0; (i156 < 2); i156 = (i156 + 1)) {
			fRec158[i156] = 0.0f;
			
		}
		for (int i157 = 0; (i157 < 2); i157 = (i157 + 1)) {
			fRec155[i157] = 0.0f;
			
		}
		for (int i158 = 0; (i158 < 2); i158 = (i158 + 1)) {
			fRec161[i158] = 0.0f;
			
		}
		for (int i159 = 0; (i159 < 2); i159 = (i159 + 1)) {
			fRec154[i159] = 0.0f;
			
		}
		for (int i160 = 0; (i160 < 2); i160 = (i160 + 1)) {
			fRec153[i160] = 0.0f;
			
		}
		for (int i161 = 0; (i161 < 2); i161 = (i161 + 1)) {
			fRec165[i161] = 0.0f;
			
		}
		for (int i162 = 0; (i162 < 2); i162 = (i162 + 1)) {
			fRec166[i162] = 0.0f;
			
		}
		for (int i163 = 0; (i163 < 2); i163 = (i163 + 1)) {
			fRec169[i163] = 0.0f;
			
		}
		for (int i164 = 0; (i164 < 2); i164 = (i164 + 1)) {
			fRec168[i164] = 0.0f;
			
		}
		for (int i165 = 0; (i165 < 2); i165 = (i165 + 1)) {
			fRec167[i165] = 0.0f;
			
		}
		for (int i166 = 0; (i166 < 2); i166 = (i166 + 1)) {
			fRec164[i166] = 0.0f;
			
		}
		for (int i167 = 0; (i167 < 2); i167 = (i167 + 1)) {
			fRec170[i167] = 0.0f;
			
		}
		for (int i168 = 0; (i168 < 2); i168 = (i168 + 1)) {
			fRec163[i168] = 0.0f;
			
		}
		for (int i169 = 0; (i169 < 2); i169 = (i169 + 1)) {
			fRec162[i169] = 0.0f;
			
		}
		for (int i170 = 0; (i170 < 2); i170 = (i170 + 1)) {
			fRec173[i170] = 0.0f;
			
		}
		for (int i171 = 0; (i171 < 2); i171 = (i171 + 1)) {
			fRec174[i171] = 0.0f;
			
		}
		for (int i172 = 0; (i172 < 2); i172 = (i172 + 1)) {
			fRec176[i172] = 0.0f;
			
		}
		for (int i173 = 0; (i173 < 2); i173 = (i173 + 1)) {
			fRec177[i173] = 0.0f;
			
		}
		for (int i174 = 0; (i174 < 2); i174 = (i174 + 1)) {
			fRec175[i174] = 0.0f;
			
		}
		for (int i175 = 0; (i175 < 2); i175 = (i175 + 1)) {
			fRec172[i175] = 0.0f;
			
		}
		for (int i176 = 0; (i176 < 2); i176 = (i176 + 1)) {
			fRec171[i176] = 0.0f;
			
		}
		for (int i177 = 0; (i177 < 2); i177 = (i177 + 1)) {
			fRec181[i177] = 0.0f;
			
		}
		for (int i178 = 0; (i178 < 2); i178 = (i178 + 1)) {
			fRec182[i178] = 0.0f;
			
		}
		for (int i179 = 0; (i179 < 2); i179 = (i179 + 1)) {
			fRec180[i179] = 0.0f;
			
		}
		for (int i180 = 0; (i180 < 2); i180 = (i180 + 1)) {
			fRec183[i180] = 0.0f;
			
		}
		for (int i181 = 0; (i181 < 2); i181 = (i181 + 1)) {
			fRec179[i181] = 0.0f;
			
		}
		for (int i182 = 0; (i182 < 2); i182 = (i182 + 1)) {
			fRec178[i182] = 0.0f;
			
		}
		for (int i183 = 0; (i183 < 2); i183 = (i183 + 1)) {
			fRec187[i183] = 0.0f;
			
		}
		for (int i184 = 0; (i184 < 2); i184 = (i184 + 1)) {
			fRec188[i184] = 0.0f;
			
		}
		for (int i185 = 0; (i185 < 2); i185 = (i185 + 1)) {
			fRec186[i185] = 0.0f;
			
		}
		for (int i186 = 0; (i186 < 2); i186 = (i186 + 1)) {
			fRec189[i186] = 0.0f;
			
		}
		for (int i187 = 0; (i187 < 2); i187 = (i187 + 1)) {
			fRec185[i187] = 0.0f;
			
		}
		for (int i188 = 0; (i188 < 2); i188 = (i188 + 1)) {
			fRec184[i188] = 0.0f;
			
		}
		for (int i189 = 0; (i189 < 2); i189 = (i189 + 1)) {
			fRec192[i189] = 0.0f;
			
		}
		for (int i190 = 0; (i190 < 2); i190 = (i190 + 1)) {
			fRec193[i190] = 0.0f;
			
		}
		for (int i191 = 0; (i191 < 2); i191 = (i191 + 1)) {
			fRec195[i191] = 0.0f;
			
		}
		for (int i192 = 0; (i192 < 2); i192 = (i192 + 1)) {
			fRec196[i192] = 0.0f;
			
		}
		for (int i193 = 0; (i193 < 2); i193 = (i193 + 1)) {
			fRec194[i193] = 0.0f;
			
		}
		for (int i194 = 0; (i194 < 2); i194 = (i194 + 1)) {
			fRec191[i194] = 0.0f;
			
		}
		for (int i195 = 0; (i195 < 2); i195 = (i195 + 1)) {
			fRec190[i195] = 0.0f;
			
		}
		for (int i196 = 0; (i196 < 2); i196 = (i196 + 1)) {
			fRec136[i196] = 0.0f;
			
		}
		for (int i197 = 0; (i197 < 2); i197 = (i197 + 1)) {
			fRec197[i197] = 0.0f;
			
		}
		for (int i198 = 0; (i198 < 2); i198 = (i198 + 1)) {
			fRec198[i198] = 0.0f;
			
		}
		for (int i199 = 0; (i199 < 2); i199 = (i199 + 1)) {
			fRec199[i199] = 0.0f;
			
		}
		for (int i200 = 0; (i200 < 2); i200 = (i200 + 1)) {
			fRec200[i200] = 0.0f;
			
		}
		for (int i201 = 0; (i201 < 2); i201 = (i201 + 1)) {
			fRec204[i201] = 0.0f;
			
		}
		for (int i202 = 0; (i202 < 2); i202 = (i202 + 1)) {
			fRec205[i202] = 0.0f;
			
		}
		for (int i203 = 0; (i203 < 2); i203 = (i203 + 1)) {
			fRec207[i203] = 0.0f;
			
		}
		for (int i204 = 0; (i204 < 2); i204 = (i204 + 1)) {
			fRec208[i204] = 0.0f;
			
		}
		for (int i205 = 0; (i205 < 2); i205 = (i205 + 1)) {
			fRec206[i205] = 0.0f;
			
		}
		for (int i206 = 0; (i206 < 2); i206 = (i206 + 1)) {
			fRec203[i206] = 0.0f;
			
		}
		for (int i207 = 0; (i207 < 2); i207 = (i207 + 1)) {
			fRec202[i207] = 0.0f;
			
		}
		for (int i208 = 0; (i208 < 2); i208 = (i208 + 1)) {
			fRec212[i208] = 0.0f;
			
		}
		for (int i209 = 0; (i209 < 2); i209 = (i209 + 1)) {
			fRec213[i209] = 0.0f;
			
		}
		for (int i210 = 0; (i210 < 2); i210 = (i210 + 1)) {
			fRec216[i210] = 0.0f;
			
		}
		for (int i211 = 0; (i211 < 2); i211 = (i211 + 1)) {
			fRec215[i211] = 0.0f;
			
		}
		for (int i212 = 0; (i212 < 2); i212 = (i212 + 1)) {
			fRec214[i212] = 0.0f;
			
		}
		for (int i213 = 0; (i213 < 2); i213 = (i213 + 1)) {
			fRec211[i213] = 0.0f;
			
		}
		for (int i214 = 0; (i214 < 2); i214 = (i214 + 1)) {
			fRec217[i214] = 0.0f;
			
		}
		for (int i215 = 0; (i215 < 2); i215 = (i215 + 1)) {
			fRec210[i215] = 0.0f;
			
		}
		for (int i216 = 0; (i216 < 2); i216 = (i216 + 1)) {
			fRec209[i216] = 0.0f;
			
		}
		for (int i217 = 0; (i217 < 2); i217 = (i217 + 1)) {
			fRec221[i217] = 0.0f;
			
		}
		for (int i218 = 0; (i218 < 2); i218 = (i218 + 1)) {
			fRec222[i218] = 0.0f;
			
		}
		for (int i219 = 0; (i219 < 2); i219 = (i219 + 1)) {
			fRec220[i219] = 0.0f;
			
		}
		for (int i220 = 0; (i220 < 2); i220 = (i220 + 1)) {
			fRec223[i220] = 0.0f;
			
		}
		for (int i221 = 0; (i221 < 2); i221 = (i221 + 1)) {
			fRec219[i221] = 0.0f;
			
		}
		for (int i222 = 0; (i222 < 2); i222 = (i222 + 1)) {
			fRec218[i222] = 0.0f;
			
		}
		for (int i223 = 0; (i223 < 2); i223 = (i223 + 1)) {
			fRec226[i223] = 0.0f;
			
		}
		for (int i224 = 0; (i224 < 2); i224 = (i224 + 1)) {
			fRec227[i224] = 0.0f;
			
		}
		for (int i225 = 0; (i225 < 2); i225 = (i225 + 1)) {
			fRec225[i225] = 0.0f;
			
		}
		for (int i226 = 0; (i226 < 2); i226 = (i226 + 1)) {
			fRec224[i226] = 0.0f;
			
		}
		for (int i227 = 0; (i227 < 2); i227 = (i227 + 1)) {
			fRec230[i227] = 0.0f;
			
		}
		for (int i228 = 0; (i228 < 2); i228 = (i228 + 1)) {
			fRec231[i228] = 0.0f;
			
		}
		for (int i229 = 0; (i229 < 2); i229 = (i229 + 1)) {
			fRec233[i229] = 0.0f;
			
		}
		for (int i230 = 0; (i230 < 2); i230 = (i230 + 1)) {
			fRec234[i230] = 0.0f;
			
		}
		for (int i231 = 0; (i231 < 2); i231 = (i231 + 1)) {
			fRec232[i231] = 0.0f;
			
		}
		for (int i232 = 0; (i232 < 2); i232 = (i232 + 1)) {
			fRec229[i232] = 0.0f;
			
		}
		for (int i233 = 0; (i233 < 2); i233 = (i233 + 1)) {
			fRec228[i233] = 0.0f;
			
		}
		for (int i234 = 0; (i234 < 2); i234 = (i234 + 1)) {
			fRec238[i234] = 0.0f;
			
		}
		for (int i235 = 0; (i235 < 2); i235 = (i235 + 1)) {
			fRec239[i235] = 0.0f;
			
		}
		for (int i236 = 0; (i236 < 2); i236 = (i236 + 1)) {
			fRec242[i236] = 0.0f;
			
		}
		for (int i237 = 0; (i237 < 2); i237 = (i237 + 1)) {
			fRec241[i237] = 0.0f;
			
		}
		for (int i238 = 0; (i238 < 2); i238 = (i238 + 1)) {
			fRec240[i238] = 0.0f;
			
		}
		for (int i239 = 0; (i239 < 2); i239 = (i239 + 1)) {
			fRec237[i239] = 0.0f;
			
		}
		for (int i240 = 0; (i240 < 2); i240 = (i240 + 1)) {
			fRec243[i240] = 0.0f;
			
		}
		for (int i241 = 0; (i241 < 2); i241 = (i241 + 1)) {
			fRec236[i241] = 0.0f;
			
		}
		for (int i242 = 0; (i242 < 2); i242 = (i242 + 1)) {
			fRec235[i242] = 0.0f;
			
		}
		for (int i243 = 0; (i243 < 2); i243 = (i243 + 1)) {
			fRec201[i243] = 0.0f;
			
		}
		for (int i244 = 0; (i244 < 2); i244 = (i244 + 1)) {
			fRec244[i244] = 0.0f;
			
		}
		for (int i245 = 0; (i245 < 2); i245 = (i245 + 1)) {
			fRec245[i245] = 0.0f;
			
		}
		for (int i246 = 0; (i246 < 2); i246 = (i246 + 1)) {
			fRec246[i246] = 0.0f;
			
		}
		for (int i247 = 0; (i247 < 2); i247 = (i247 + 1)) {
			fRec247[i247] = 0.0f;
			
		}
		for (int i248 = 0; (i248 < 2); i248 = (i248 + 1)) {
			fRec248[i248] = 0.0f;
			
		}
		for (int i249 = 0; (i249 < 2); i249 = (i249 + 1)) {
			fRec249[i249] = 0.0f;
			
		}
		for (int i250 = 0; (i250 < 2); i250 = (i250 + 1)) {
			fRec250[i250] = 0.0f;
			
		}
		for (int i251 = 0; (i251 < 2); i251 = (i251 + 1)) {
			fRec251[i251] = 0.0f;
			
		}
		for (int i252 = 0; (i252 < 2); i252 = (i252 + 1)) {
			fRec252[i252] = 0.0f;
			
		}
		for (int i253 = 0; (i253 < 2); i253 = (i253 + 1)) {
			fRec253[i253] = 0.0f;
			
		}
		for (int i254 = 0; (i254 < 2); i254 = (i254 + 1)) {
			fRec254[i254] = 0.0f;
			
		}
		for (int i255 = 0; (i255 < 2); i255 = (i255 + 1)) {
			fRec255[i255] = 0.0f;
			
		}
		for (int i256 = 0; (i256 < 2); i256 = (i256 + 1)) {
			fRec256[i256] = 0.0f;
			
		}
		for (int i257 = 0; (i257 < 2); i257 = (i257 + 1)) {
			fRec257[i257] = 0.0f;
			
		}
		for (int i258 = 0; (i258 < 2); i258 = (i258 + 1)) {
			fRec258[i258] = 0.0f;
			
		}
		for (int i259 = 0; (i259 < 2); i259 = (i259 + 1)) {
			fRec259[i259] = 0.0f;
			
		}
		for (int i260 = 0; (i260 < 2); i260 = (i260 + 1)) {
			fRec260[i260] = 0.0f;
			
		}
		for (int i261 = 0; (i261 < 2); i261 = (i261 + 1)) {
			fRec261[i261] = 0.0f;
			
		}
		for (int i262 = 0; (i262 < 2); i262 = (i262 + 1)) {
			fRec262[i262] = 0.0f;
			
		}
		for (int i263 = 0; (i263 < 2); i263 = (i263 + 1)) {
			fRec263[i263] = 0.0f;
			
		}
		for (int i264 = 0; (i264 < 2); i264 = (i264 + 1)) {
			fRec264[i264] = 0.0f;
			
		}
		for (int i265 = 0; (i265 < 2); i265 = (i265 + 1)) {
			fRec265[i265] = 0.0f;
			
		}
		for (int i266 = 0; (i266 < 2); i266 = (i266 + 1)) {
			fRec266[i266] = 0.0f;
			
		}
		for (int i267 = 0; (i267 < 2); i267 = (i267 + 1)) {
			fRec267[i267] = 0.0f;
			
		}
		for (int i268 = 0; (i268 < 2); i268 = (i268 + 1)) {
			fRec268[i268] = 0.0f;
			
		}
		for (int i269 = 0; (i269 < 2); i269 = (i269 + 1)) {
			fRec269[i269] = 0.0f;
			
		}
		for (int i270 = 0; (i270 < 2); i270 = (i270 + 1)) {
			fRec270[i270] = 0.0f;
			
		}
		for (int i271 = 0; (i271 < 2); i271 = (i271 + 1)) {
			fRec271[i271] = 0.0f;
			
		}
		for (int i272 = 0; (i272 < 2); i272 = (i272 + 1)) {
			fRec272[i272] = 0.0f;
			
		}
		for (int i273 = 0; (i273 < 2); i273 = (i273 + 1)) {
			fRec273[i273] = 0.0f;
			
		}
		for (int i274 = 0; (i274 < 2); i274 = (i274 + 1)) {
			fRec274[i274] = 0.0f;
			
		}
		for (int i275 = 0; (i275 < 2); i275 = (i275 + 1)) {
			fRec275[i275] = 0.0f;
			
		}
		for (int i276 = 0; (i276 < 2); i276 = (i276 + 1)) {
			fRec276[i276] = 0.0f;
			
		}
		for (int i277 = 0; (i277 < 2); i277 = (i277 + 1)) {
			fRec277[i277] = 0.0f;
			
		}
		for (int i278 = 0; (i278 < 2); i278 = (i278 + 1)) {
			fRec278[i278] = 0.0f;
			
		}
		for (int i279 = 0; (i279 < 2); i279 = (i279 + 1)) {
			fRec279[i279] = 0.0f;
			
		}
		for (int i280 = 0; (i280 < 2); i280 = (i280 + 1)) {
			fRec280[i280] = 0.0f;
			
		}
		for (int i281 = 0; (i281 < 2); i281 = (i281 + 1)) {
			fRec281[i281] = 0.0f;
			
		}
		for (int i282 = 0; (i282 < 2); i282 = (i282 + 1)) {
			fRec282[i282] = 0.0f;
			
		}
		for (int i283 = 0; (i283 < 2); i283 = (i283 + 1)) {
			fRec283[i283] = 0.0f;
			
		}
		for (int i284 = 0; (i284 < 2); i284 = (i284 + 1)) {
			fRec284[i284] = 0.0f;
			
		}
		for (int i285 = 0; (i285 < 2); i285 = (i285 + 1)) {
			fRec285[i285] = 0.0f;
			
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
		ui_interface->addVerticalBargraph("0x3e353f0", &fVbargraph0, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("1");
		ui_interface->openVerticalBox("1");
		ui_interface->declare(&fVbargraph24, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3fb0740", &fVbargraph24, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("2");
		ui_interface->declare(&fVbargraph19, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3f78960", &fVbargraph19, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("3");
		ui_interface->declare(&fVbargraph20, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3f82120", &fVbargraph20, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2");
		ui_interface->openVerticalBox("4");
		ui_interface->declare(&fVbargraph25, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3fbb8e0", &fVbargraph25, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("5");
		ui_interface->declare(&fVbargraph40, "unit", "dB");
		ui_interface->addVerticalBargraph("0x4091da0", &fVbargraph40, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("6");
		ui_interface->declare(&fVbargraph18, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3f6e1b0", &fVbargraph18, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("7");
		ui_interface->declare(&fVbargraph17, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3f62ec0", &fVbargraph17, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("8");
		ui_interface->declare(&fVbargraph16, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3f578b0", &fVbargraph16, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3");
		ui_interface->openVerticalBox("9");
		ui_interface->declare(&fVbargraph30, "unit", "dB");
		ui_interface->addVerticalBargraph("0x40132f0", &fVbargraph30, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("10");
		ui_interface->declare(&fVbargraph39, "unit", "dB");
		ui_interface->addVerticalBargraph("0x40867a0", &fVbargraph39, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("11");
		ui_interface->declare(&fVbargraph29, "unit", "dB");
		ui_interface->addVerticalBargraph("0x4004970", &fVbargraph29, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("12");
		ui_interface->declare(&fVbargraph15, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3f49c30", &fVbargraph15, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("13");
		ui_interface->declare(&fVbargraph14, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3f3b4e0", &fVbargraph14, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("14");
		ui_interface->declare(&fVbargraph13, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3f2cb50", &fVbargraph13, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("15");
		ui_interface->declare(&fVbargraph12, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3f1e0e0", &fVbargraph12, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4");
		ui_interface->openVerticalBox("16");
		ui_interface->declare(&fVbargraph31, "unit", "dB");
		ui_interface->addVerticalBargraph("0x4023c90", &fVbargraph31, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("17");
		ui_interface->declare(&fVbargraph41, "unit", "dB");
		ui_interface->addVerticalBargraph("0x40a7980", &fVbargraph41, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("18");
		ui_interface->declare(&fVbargraph28, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3ff6120", &fVbargraph28, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("19");
		ui_interface->declare(&fVbargraph37, "unit", "dB");
		ui_interface->addVerticalBargraph("0x4063700", &fVbargraph37, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("20");
		ui_interface->declare(&fVbargraph11, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3f0b760", &fVbargraph11, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("21");
		ui_interface->declare(&fVbargraph10, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3efb400", &fVbargraph10, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("22");
		ui_interface->declare(&fVbargraph9, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3eeac20", &fVbargraph9, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("23");
		ui_interface->declare(&fVbargraph8, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3eda440", &fVbargraph8, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("24");
		ui_interface->declare(&fVbargraph7, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3ec9b80", &fVbargraph7, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("5");
		ui_interface->openVerticalBox("25");
		ui_interface->declare(&fVbargraph26, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3fcf350", &fVbargraph26, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("26");
		ui_interface->declare(&fVbargraph42, "unit", "dB");
		ui_interface->addVerticalBargraph("0x40bba70", &fVbargraph42, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("27");
		ui_interface->declare(&fVbargraph23, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3fa6eb0", &fVbargraph23, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("28");
		ui_interface->declare(&fVbargraph38, "unit", "dB");
		ui_interface->addVerticalBargraph("0x40778d0", &fVbargraph38, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("29");
		ui_interface->declare(&fVbargraph27, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3fe3270", &fVbargraph27, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("30");
		ui_interface->declare(&fVbargraph6, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3eb4680", &fVbargraph6, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("31");
		ui_interface->declare(&fVbargraph5, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3ea0d90", &fVbargraph5, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("32");
		ui_interface->declare(&fVbargraph4, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3e8d020", &fVbargraph4, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("33");
		ui_interface->declare(&fVbargraph3, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3e792b0", &fVbargraph3, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("34");
		ui_interface->declare(&fVbargraph2, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3e65540", &fVbargraph2, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("35");
		ui_interface->declare(&fVbargraph1, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3e516f0", &fVbargraph1, -70.0f, 6.0f);
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
		ui_interface->declare(&fVbargraph21, "osc", "/output1");
		ui_interface->declare(&fVbargraph21, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3f89380", &fVbargraph21, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("2");
		ui_interface->declare(&fVbargraph22, "osc", "/output2");
		ui_interface->declare(&fVbargraph22, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3f93330", &fVbargraph22, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("3");
		ui_interface->declare(&fVbargraph32, "osc", "/output3");
		ui_interface->declare(&fVbargraph32, "unit", "dB");
		ui_interface->addVerticalBargraph("0x4029f80", &fVbargraph32, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("4");
		ui_interface->declare(&fVbargraph33, "osc", "/output4");
		ui_interface->declare(&fVbargraph33, "unit", "dB");
		ui_interface->addVerticalBargraph("0x40332f0", &fVbargraph33, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("5");
		ui_interface->declare(&fVbargraph34, "osc", "/output5");
		ui_interface->declare(&fVbargraph34, "unit", "dB");
		ui_interface->addVerticalBargraph("0x403c740", &fVbargraph34, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("6");
		ui_interface->declare(&fVbargraph35, "osc", "/output6");
		ui_interface->declare(&fVbargraph35, "unit", "dB");
		ui_interface->addVerticalBargraph("0x4043d00", &fVbargraph35, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("7");
		ui_interface->declare(&fVbargraph36, "osc", "/output7");
		ui_interface->declare(&fVbargraph36, "unit", "dB");
		ui_interface->addVerticalBargraph("0x4052cb0", &fVbargraph36, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("8");
		ui_interface->declare(&fVbargraph43, "osc", "/output8");
		ui_interface->declare(&fVbargraph43, "unit", "dB");
		ui_interface->addVerticalBargraph("0x40c4d80", &fVbargraph43, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("9");
		ui_interface->declare(&fVbargraph44, "osc", "/output9");
		ui_interface->declare(&fVbargraph44, "unit", "dB");
		ui_interface->addVerticalBargraph("0x40d32d0", &fVbargraph44, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("10");
		ui_interface->declare(&fVbargraph45, "osc", "/output10");
		ui_interface->declare(&fVbargraph45, "unit", "dB");
		ui_interface->addVerticalBargraph("0x40e1ad0", &fVbargraph45, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("11");
		ui_interface->declare(&fVbargraph46, "osc", "/output11");
		ui_interface->declare(&fVbargraph46, "unit", "dB");
		ui_interface->addVerticalBargraph("0x40ee4c0", &fVbargraph46, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("12");
		ui_interface->declare(&fVbargraph47, "osc", "/output12");
		ui_interface->declare(&fVbargraph47, "unit", "dB");
		ui_interface->addVerticalBargraph("0x40f8ae0", &fVbargraph47, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("13");
		ui_interface->declare(&fVbargraph48, "osc", "/output13");
		ui_interface->declare(&fVbargraph48, "unit", "dB");
		ui_interface->addVerticalBargraph("0x4102290", &fVbargraph48, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("14");
		ui_interface->declare(&fVbargraph49, "osc", "/output14");
		ui_interface->declare(&fVbargraph49, "unit", "dB");
		ui_interface->addVerticalBargraph("0x410c290", &fVbargraph49, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("15");
		ui_interface->declare(&fVbargraph50, "osc", "/output15");
		ui_interface->declare(&fVbargraph50, "unit", "dB");
		ui_interface->addVerticalBargraph("0x4113e70", &fVbargraph50, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("16");
		ui_interface->declare(&fVbargraph51, "osc", "/output16");
		ui_interface->declare(&fVbargraph51, "unit", "dB");
		ui_interface->addVerticalBargraph("0x411d570", &fVbargraph51, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("17");
		ui_interface->declare(&fVbargraph52, "osc", "/output17");
		ui_interface->declare(&fVbargraph52, "unit", "dB");
		ui_interface->addVerticalBargraph("0x4126b70", &fVbargraph52, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("18");
		ui_interface->declare(&fVbargraph53, "osc", "/output18");
		ui_interface->declare(&fVbargraph53, "unit", "dB");
		ui_interface->addVerticalBargraph("0x4130610", &fVbargraph53, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("19");
		ui_interface->declare(&fVbargraph54, "osc", "/output19");
		ui_interface->declare(&fVbargraph54, "unit", "dB");
		ui_interface->addVerticalBargraph("0x4146720", &fVbargraph54, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("20");
		ui_interface->declare(&fVbargraph55, "osc", "/output20");
		ui_interface->declare(&fVbargraph55, "unit", "dB");
		ui_interface->addVerticalBargraph("0x41573c0", &fVbargraph55, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("21");
		ui_interface->declare(&fVbargraph56, "osc", "/output21");
		ui_interface->declare(&fVbargraph56, "unit", "dB");
		ui_interface->addVerticalBargraph("0x41674e0", &fVbargraph56, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("22");
		ui_interface->declare(&fVbargraph57, "osc", "/output22");
		ui_interface->declare(&fVbargraph57, "unit", "dB");
		ui_interface->addVerticalBargraph("0x4177780", &fVbargraph57, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("23");
		ui_interface->declare(&fVbargraph58, "osc", "/output23");
		ui_interface->declare(&fVbargraph58, "unit", "dB");
		ui_interface->addVerticalBargraph("0x41816b0", &fVbargraph58, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("24");
		ui_interface->declare(&fVbargraph59, "osc", "/output24");
		ui_interface->declare(&fVbargraph59, "unit", "dB");
		ui_interface->addVerticalBargraph("0x418b2d0", &fVbargraph59, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("25");
		ui_interface->declare(&fVbargraph60, "osc", "/output25");
		ui_interface->declare(&fVbargraph60, "unit", "dB");
		ui_interface->addVerticalBargraph("0x4195570", &fVbargraph60, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Outputs 26-50");
		ui_interface->openVerticalBox("26");
		ui_interface->declare(&fVbargraph61, "osc", "/output26");
		ui_interface->declare(&fVbargraph61, "unit", "dB");
		ui_interface->addVerticalBargraph("0x419f710", &fVbargraph61, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("27");
		ui_interface->declare(&fVbargraph62, "osc", "/output27");
		ui_interface->declare(&fVbargraph62, "unit", "dB");
		ui_interface->addVerticalBargraph("0x41b4d10", &fVbargraph62, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("28");
		ui_interface->declare(&fVbargraph63, "osc", "/output28");
		ui_interface->declare(&fVbargraph63, "unit", "dB");
		ui_interface->addVerticalBargraph("0x41c2490", &fVbargraph63, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("29");
		ui_interface->declare(&fVbargraph64, "osc", "/output29");
		ui_interface->declare(&fVbargraph64, "unit", "dB");
		ui_interface->addVerticalBargraph("0x41d2f40", &fVbargraph64, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("30");
		ui_interface->declare(&fVbargraph65, "osc", "/output30");
		ui_interface->declare(&fVbargraph65, "unit", "dB");
		ui_interface->addVerticalBargraph("0x41e2d90", &fVbargraph65, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("31");
		ui_interface->declare(&fVbargraph66, "osc", "/output31");
		ui_interface->declare(&fVbargraph66, "unit", "dB");
		ui_interface->addVerticalBargraph("0x41f8bf0", &fVbargraph66, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("32");
		ui_interface->declare(&fVbargraph67, "osc", "/output32");
		ui_interface->declare(&fVbargraph67, "unit", "dB");
		ui_interface->addVerticalBargraph("0x420c310", &fVbargraph67, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("33");
		ui_interface->declare(&fVbargraph68, "osc", "/output33");
		ui_interface->declare(&fVbargraph68, "unit", "dB");
		ui_interface->addVerticalBargraph("0x421e980", &fVbargraph68, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("34");
		ui_interface->declare(&fVbargraph69, "osc", "/output34");
		ui_interface->declare(&fVbargraph69, "unit", "dB");
		ui_interface->addVerticalBargraph("0x422f230", &fVbargraph69, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("35");
		ui_interface->declare(&fVbargraph70, "osc", "/output35");
		ui_interface->declare(&fVbargraph70, "unit", "dB");
		ui_interface->addVerticalBargraph("0x4240960", &fVbargraph70, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("36");
		ui_interface->declare(&fVbargraph71, "osc", "/output36");
		ui_interface->declare(&fVbargraph71, "unit", "dB");
		ui_interface->addVerticalBargraph("0x4251d40", &fVbargraph71, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("37");
		ui_interface->declare(&fVbargraph72, "osc", "/output37");
		ui_interface->declare(&fVbargraph72, "unit", "dB");
		ui_interface->addVerticalBargraph("0x4263ba0", &fVbargraph72, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("38");
		ui_interface->declare(&fVbargraph73, "osc", "/output38");
		ui_interface->declare(&fVbargraph73, "unit", "dB");
		ui_interface->addVerticalBargraph("0x4277f20", &fVbargraph73, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("39");
		ui_interface->declare(&fVbargraph74, "osc", "/output39");
		ui_interface->declare(&fVbargraph74, "unit", "dB");
		ui_interface->addVerticalBargraph("0x4281af0", &fVbargraph74, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("40");
		ui_interface->declare(&fVbargraph75, "osc", "/output40");
		ui_interface->declare(&fVbargraph75, "unit", "dB");
		ui_interface->addVerticalBargraph("0x428b3b0", &fVbargraph75, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("41");
		ui_interface->declare(&fVbargraph76, "osc", "/output41");
		ui_interface->declare(&fVbargraph76, "unit", "dB");
		ui_interface->addVerticalBargraph("0x4294f30", &fVbargraph76, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("42");
		ui_interface->declare(&fVbargraph77, "osc", "/output42");
		ui_interface->declare(&fVbargraph77, "unit", "dB");
		ui_interface->addVerticalBargraph("0x429ebc0", &fVbargraph77, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("43");
		ui_interface->declare(&fVbargraph78, "osc", "/output43");
		ui_interface->declare(&fVbargraph78, "unit", "dB");
		ui_interface->addVerticalBargraph("0x42a8540", &fVbargraph78, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("44");
		ui_interface->declare(&fVbargraph79, "osc", "/output44");
		ui_interface->declare(&fVbargraph79, "unit", "dB");
		ui_interface->addVerticalBargraph("0x42b2360", &fVbargraph79, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("45");
		ui_interface->declare(&fVbargraph80, "osc", "/output45");
		ui_interface->declare(&fVbargraph80, "unit", "dB");
		ui_interface->addVerticalBargraph("0x42bc080", &fVbargraph80, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("46");
		ui_interface->declare(&fVbargraph81, "osc", "/output46");
		ui_interface->declare(&fVbargraph81, "unit", "dB");
		ui_interface->addVerticalBargraph("0x42c5e80", &fVbargraph81, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("47");
		ui_interface->declare(&fVbargraph82, "osc", "/output47");
		ui_interface->declare(&fVbargraph82, "unit", "dB");
		ui_interface->addVerticalBargraph("0x42cfb80", &fVbargraph82, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("48");
		ui_interface->declare(&fVbargraph83, "osc", "/output48");
		ui_interface->declare(&fVbargraph83, "unit", "dB");
		ui_interface->addVerticalBargraph("0x42d9960", &fVbargraph83, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("49");
		ui_interface->declare(&fVbargraph84, "osc", "/output49");
		ui_interface->declare(&fVbargraph84, "unit", "dB");
		ui_interface->addVerticalBargraph("0x42e3be0", &fVbargraph84, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("50");
		ui_interface->declare(&fVbargraph85, "osc", "/output50");
		ui_interface->declare(&fVbargraph85, "unit", "dB");
		ui_interface->addVerticalBargraph("0x42edf40", &fVbargraph85, -70.0f, 6.0f);
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
		FAUSTFLOAT* input25 = inputs[25];
		FAUSTFLOAT* input26 = inputs[26];
		FAUSTFLOAT* input27 = inputs[27];
		FAUSTFLOAT* input28 = inputs[28];
		FAUSTFLOAT* input29 = inputs[29];
		FAUSTFLOAT* input30 = inputs[30];
		FAUSTFLOAT* input31 = inputs[31];
		FAUSTFLOAT* input32 = inputs[32];
		FAUSTFLOAT* input33 = inputs[33];
		FAUSTFLOAT* input34 = inputs[34];
		FAUSTFLOAT* input35 = inputs[35];
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
		float fSlow8 = (1.0f / (((fConst3 / fSlow1) + (14.272481f * fSlow7)) + 1.0f));
		float fSlow9 = (57.0899239f * fSlow7);
		float fSlow10 = (fSlow9 + (fConst5 / fSlow1));
		float fSlow11 = (1.0f / (((fConst6 / fSlow1) + (18.1563148f * fSlow7)) + 1.0f));
		float fSlow12 = (72.6252594f * fSlow7);
		float fSlow13 = (fSlow12 + (fConst7 / fSlow1));
		float fSlow14 = (fSlow2 * fSlow1);
		float fSlow15 = (fConst8 / fSlow1);
		float fSlow16 = (1.0f - fSlow2);
		float fSlow17 = (1.0f / (((fConst9 / fSlow1) + (9.140131f * fSlow7)) + 1.0f));
		float fSlow18 = (36.560524f * fSlow7);
		float fSlow19 = (fSlow18 + (fConst10 / fSlow1));
		float fSlow20 = (1.0f / (((fConst11 / fSlow1) + (11.4878006f * fSlow7)) + 1.0f));
		float fSlow21 = (45.9512024f * fSlow7);
		float fSlow22 = (fSlow21 + (fConst12 / fSlow1));
		float fSlow23 = (1.0f / ((fConst13 / fSlow1) + 1.0f));
		float fSlow24 = (1.0f / (((fConst14 / fSlow1) + (6.4594326f * fSlow7)) + 1.0f));
		float fSlow25 = (25.8377304f * fSlow7);
		float fSlow26 = (fSlow25 + (fConst15 / fSlow1));
		float fSlow27 = (fConst16 / fSlow1);
		float fSlow28 = (1.0f / (((fConst17 / fSlow1) + (3.0f * fSlow7)) + 1.0f));
		float fSlow29 = (12.0f * fSlow7);
		float fSlow30 = (fSlow29 + (fConst18 / fSlow1));
		float fSlow31 = (1.0f / (fSlow6 + 1.0f));
		float fSlow32 = (fConst19 / fSlow1);
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec1[0] = (fSlow0 + (0.999000013f * fRec1[1]));
			fRec3[0] = (fSlow4 + (0.999000013f * fRec3[1]));
			float fTemp0 = (fSlow3 * (fRec3[0] * float(input0[i])));
			fRec2[0] = max((fRec2[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp0))))));
			fVbargraph0 = FAUSTFLOAT(fRec2[0]);
			float fTemp1 = (0.0126984129f * fTemp0);
			fRec7[0] = (fRec6[1] + fRec7[1]);
			fRec8[0] = (fRec7[1] + fRec8[1]);
			fRec11[0] = (fRec9[1] + fRec11[1]);
			fRec10[0] = (fRec10[1] + fRec11[1]);
			float fTemp2 = (fRec3[0] * float(input35[i]));
			fRec9[0] = (0.0f - (fSlow11 * (((fSlow12 * fRec10[0]) + (fSlow13 * fRec11[0])) - (fSlow14 * fTemp2))));
			fRec6[0] = (0.0f - (fSlow8 * (((fSlow10 * fRec7[0]) + (fSlow9 * fRec8[0])) - fRec9[0])));
			fRec12[0] = (fRec5[1] + fRec12[1]);
			fRec5[0] = (fSlow5 * (fRec6[0] - (fSlow15 * fRec12[0])));
			float fTemp3 = (fRec5[0] + (fSlow16 * fTemp2));
			fRec4[0] = max((fRec4[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp3))))));
			fVbargraph1 = FAUSTFLOAT(fRec4[0]);
			float fTemp4 = (0.0f * fTemp3);
			fRec16[0] = (fRec15[1] + fRec16[1]);
			fRec17[0] = (fRec16[1] + fRec17[1]);
			fRec20[0] = (fRec18[1] + fRec20[1]);
			fRec19[0] = (fRec19[1] + fRec20[1]);
			float fTemp5 = (fRec3[0] * float(input34[i]));
			fRec18[0] = (0.0f - (fSlow11 * (((fSlow12 * fRec19[0]) + (fSlow13 * fRec20[0])) - (fSlow14 * fTemp5))));
			fRec15[0] = (0.0f - (fSlow8 * (((fSlow10 * fRec16[0]) + (fSlow9 * fRec17[0])) - fRec18[0])));
			fRec21[0] = (fRec14[1] + fRec21[1]);
			fRec14[0] = (fSlow5 * (fRec15[0] - (fSlow15 * fRec21[0])));
			float fTemp6 = (fRec14[0] + (fSlow16 * fTemp5));
			fRec13[0] = max((fRec13[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp6))))));
			fVbargraph2 = FAUSTFLOAT(fRec13[0]);
			float fTemp7 = (0.0f * fTemp6);
			fRec25[0] = (fRec24[1] + fRec25[1]);
			fRec26[0] = (fRec25[1] + fRec26[1]);
			fRec29[0] = (fRec27[1] + fRec29[1]);
			fRec28[0] = (fRec28[1] + fRec29[1]);
			float fTemp8 = (fRec3[0] * float(input33[i]));
			fRec27[0] = (0.0f - (fSlow11 * (((fSlow12 * fRec28[0]) + (fSlow13 * fRec29[0])) - (fSlow14 * fTemp8))));
			fRec24[0] = (0.0f - (fSlow8 * (((fSlow10 * fRec25[0]) + (fSlow9 * fRec26[0])) - fRec27[0])));
			fRec30[0] = (fRec23[1] + fRec30[1]);
			fRec23[0] = (fSlow5 * (fRec24[0] - (fSlow15 * fRec30[0])));
			float fTemp9 = (fRec23[0] + (fSlow16 * fTemp8));
			fRec22[0] = max((fRec22[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp9))))));
			fVbargraph3 = FAUSTFLOAT(fRec22[0]);
			float fTemp10 = (0.0f * fTemp9);
			fRec34[0] = (fRec33[1] + fRec34[1]);
			fRec35[0] = (fRec34[1] + fRec35[1]);
			fRec38[0] = (fRec36[1] + fRec38[1]);
			fRec37[0] = (fRec37[1] + fRec38[1]);
			float fTemp11 = (fRec3[0] * float(input32[i]));
			fRec36[0] = (0.0f - (fSlow11 * (((fSlow12 * fRec37[0]) + (fSlow13 * fRec38[0])) - (fSlow14 * fTemp11))));
			fRec33[0] = (0.0f - (fSlow8 * (((fSlow10 * fRec34[0]) + (fSlow9 * fRec35[0])) - fRec36[0])));
			fRec39[0] = (fRec32[1] + fRec39[1]);
			fRec32[0] = (fSlow5 * (fRec33[0] - (fSlow15 * fRec39[0])));
			float fTemp12 = (fRec32[0] + (fSlow16 * fTemp11));
			fRec31[0] = max((fRec31[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp12))))));
			fVbargraph4 = FAUSTFLOAT(fRec31[0]);
			float fTemp13 = (8.09043997e-34f * fTemp12);
			fRec43[0] = (fRec42[1] + fRec43[1]);
			fRec44[0] = (fRec43[1] + fRec44[1]);
			fRec47[0] = (fRec45[1] + fRec47[1]);
			fRec46[0] = (fRec46[1] + fRec47[1]);
			float fTemp14 = (fRec3[0] * float(input31[i]));
			fRec45[0] = (0.0f - (fSlow11 * (((fSlow12 * fRec46[0]) + (fSlow13 * fRec47[0])) - (fSlow14 * fTemp14))));
			fRec42[0] = (0.0f - (fSlow8 * (((fSlow10 * fRec43[0]) + (fSlow9 * fRec44[0])) - fRec45[0])));
			fRec48[0] = (fRec41[1] + fRec48[1]);
			fRec41[0] = (fSlow5 * (fRec42[0] - (fSlow15 * fRec48[0])));
			float fTemp15 = (fRec41[0] + (fSlow16 * fTemp14));
			fRec40[0] = max((fRec40[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp15))))));
			fVbargraph5 = FAUSTFLOAT(fRec40[0]);
			float fTemp16 = (9.987856e-18f * fTemp15);
			float fTemp17 = (fRec3[0] * float(input30[i]));
			fRec54[0] = (fRec52[1] + fRec54[1]);
			fRec53[0] = (fRec53[1] + fRec54[1]);
			fRec52[0] = (fSlow11 * ((fSlow14 * fTemp17) - ((fSlow12 * fRec53[0]) + (fSlow13 * fRec54[0]))));
			fRec55[0] = (fRec51[1] + fRec55[1]);
			fRec56[0] = (fRec55[1] + fRec56[1]);
			fRec51[0] = (fSlow8 * (fRec52[0] - ((fSlow10 * fRec55[0]) + (fSlow9 * fRec56[0]))));
			fRec57[0] = (fRec50[1] + fRec57[1]);
			fRec50[0] = (fSlow5 * (fRec51[0] - (fSlow15 * fRec57[0])));
			float fTemp18 = (fRec50[0] + (fSlow16 * fTemp17));
			fRec49[0] = max((fRec49[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp18))))));
			fVbargraph6 = FAUSTFLOAT(fRec49[0]);
			float fTemp19 = (0.0421158709f * fTemp18);
			fRec60[0] = (fRec59[1] + fRec60[1]);
			fRec61[0] = (fRec60[1] + fRec61[1]);
			fRec63[0] = (fRec62[1] + fRec63[1]);
			fRec64[0] = (fRec63[1] + fRec64[1]);
			float fTemp20 = (fRec3[0] * float(input24[i]));
			fRec62[0] = (0.0f - (fSlow20 * (((fSlow22 * fRec63[0]) + (fSlow21 * fRec64[0])) - (fSlow14 * fTemp20))));
			fRec59[0] = (0.0f - (fSlow17 * (((fSlow19 * fRec60[0]) + (fSlow18 * fRec61[0])) - fRec62[0])));
			float fTemp21 = (fRec59[0] + (fSlow16 * fTemp20));
			fRec58[0] = max((fRec58[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp21))))));
			fVbargraph7 = FAUSTFLOAT(fRec58[0]);
			float fTemp22 = (0.0f * fTemp21);
			fRec67[0] = (fRec66[1] + fRec67[1]);
			fRec68[0] = (fRec67[1] + fRec68[1]);
			fRec70[0] = (fRec69[1] + fRec70[1]);
			fRec71[0] = (fRec70[1] + fRec71[1]);
			float fTemp23 = (fRec3[0] * float(input23[i]));
			fRec69[0] = (0.0f - (fSlow20 * (((fSlow22 * fRec70[0]) + (fSlow21 * fRec71[0])) - (fSlow14 * fTemp23))));
			fRec66[0] = (0.0f - (fSlow17 * (((fSlow19 * fRec67[0]) + (fSlow18 * fRec68[0])) - fRec69[0])));
			float fTemp24 = (fRec66[0] + (fSlow16 * fTemp23));
			fRec65[0] = max((fRec65[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp24))))));
			fVbargraph8 = FAUSTFLOAT(fRec65[0]);
			float fTemp25 = (0.0f * fTemp24);
			fRec74[0] = (fRec73[1] + fRec74[1]);
			fRec75[0] = (fRec74[1] + fRec75[1]);
			fRec77[0] = (fRec76[1] + fRec77[1]);
			fRec78[0] = (fRec77[1] + fRec78[1]);
			float fTemp26 = (fRec3[0] * float(input22[i]));
			fRec76[0] = (0.0f - (fSlow20 * (((fSlow22 * fRec77[0]) + (fSlow21 * fRec78[0])) - (fSlow14 * fTemp26))));
			fRec73[0] = (0.0f - (fSlow17 * (((fSlow19 * fRec74[0]) + (fSlow18 * fRec75[0])) - fRec76[0])));
			float fTemp27 = (fRec73[0] + (fSlow16 * fTemp26));
			fRec72[0] = max((fRec72[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp27))))));
			fVbargraph9 = FAUSTFLOAT(fRec72[0]);
			float fTemp28 = (4.79080666e-34f * fTemp27);
			fRec81[0] = (fRec80[1] + fRec81[1]);
			fRec82[0] = (fRec81[1] + fRec82[1]);
			fRec84[0] = (fRec83[1] + fRec84[1]);
			fRec85[0] = (fRec84[1] + fRec85[1]);
			float fTemp29 = (fRec3[0] * float(input21[i]));
			fRec83[0] = (0.0f - (fSlow20 * (((fSlow22 * fRec84[0]) + (fSlow21 * fRec85[0])) - (fSlow14 * fTemp29))));
			fRec80[0] = (0.0f - (fSlow17 * (((fSlow19 * fRec81[0]) + (fSlow18 * fRec82[0])) - fRec83[0])));
			float fTemp30 = (fRec80[0] + (fSlow16 * fTemp29));
			fRec79[0] = max((fRec79[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp30))))));
			fVbargraph10 = FAUSTFLOAT(fRec79[0]);
			float fTemp31 = (7.37652042e-18f * fTemp30);
			float fTemp32 = (fRec3[0] * float(input20[i]));
			fRec89[0] = (fRec88[1] + fRec89[1]);
			fRec90[0] = (fRec89[1] + fRec90[1]);
			fRec88[0] = (fSlow20 * ((fSlow14 * fTemp32) - ((fSlow22 * fRec89[0]) + (fSlow21 * fRec90[0]))));
			fRec91[0] = (fRec87[1] + fRec91[1]);
			fRec92[0] = (fRec91[1] + fRec92[1]);
			fRec87[0] = (fSlow17 * (fRec88[0] - ((fSlow19 * fRec91[0]) + (fSlow18 * fRec92[0]))));
			float fTemp33 = (fRec87[0] + (fSlow16 * fTemp32));
			fRec86[0] = max((fRec86[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp33))))));
			fVbargraph11 = FAUSTFLOAT(fRec86[0]);
			float fTemp34 = (0.0380952395f * fTemp33);
			fRec96[0] = (fRec95[1] + fRec96[1]);
			fRec97[0] = (fRec96[1] + fRec97[1]);
			float fTemp35 = (fRec3[0] * float(input15[i]));
			fRec95[0] = (0.0f - (fSlow24 * (((fSlow26 * fRec96[0]) + (fSlow25 * fRec97[0])) - (fSlow14 * fTemp35))));
			fRec98[0] = (fRec94[1] + fRec98[1]);
			fRec94[0] = (fSlow23 * (fRec95[0] - (fSlow27 * fRec98[0])));
			float fTemp36 = (fRec94[0] + (fSlow16 * fTemp35));
			fRec93[0] = max((fRec93[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp36))))));
			fVbargraph12 = FAUSTFLOAT(fRec93[0]);
			float fTemp37 = (0.0f * fTemp36);
			fRec102[0] = (fRec101[1] + fRec102[1]);
			fRec103[0] = (fRec102[1] + fRec103[1]);
			float fTemp38 = (fRec3[0] * float(input14[i]));
			fRec101[0] = (0.0f - (fSlow24 * (((fSlow26 * fRec102[0]) + (fSlow25 * fRec103[0])) - (fSlow14 * fTemp38))));
			fRec104[0] = (fRec100[1] + fRec104[1]);
			fRec100[0] = (fSlow23 * (fRec101[0] - (fSlow27 * fRec104[0])));
			float fTemp39 = (fRec100[0] + (fSlow16 * fTemp38));
			fRec99[0] = max((fRec99[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp39))))));
			fVbargraph13 = FAUSTFLOAT(fRec99[0]);
			float fTemp40 = (2.43935947e-34f * fTemp39);
			fRec108[0] = (fRec107[1] + fRec108[1]);
			fRec109[0] = (fRec108[1] + fRec109[1]);
			float fTemp41 = (fRec3[0] * float(input13[i]));
			fRec107[0] = (0.0f - (fSlow24 * (((fSlow26 * fRec108[0]) + (fSlow25 * fRec109[0])) - (fSlow14 * fTemp41))));
			fRec110[0] = (fRec106[1] + fRec110[1]);
			fRec106[0] = (fSlow23 * (fRec107[0] - (fSlow27 * fRec110[0])));
			float fTemp42 = (fRec106[0] + (fSlow16 * fTemp41));
			fRec105[0] = max((fRec105[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp42))))));
			fVbargraph14 = FAUSTFLOAT(fRec105[0]);
			float fTemp43 = (5.03912267e-18f * fTemp42);
			float fTemp44 = (fRec3[0] * float(input12[i]));
			fRec114[0] = (fRec113[1] + fRec114[1]);
			fRec115[0] = (fRec114[1] + fRec115[1]);
			fRec113[0] = (fSlow24 * ((fSlow14 * fTemp44) - ((fSlow26 * fRec114[0]) + (fSlow25 * fRec115[0]))));
			fRec116[0] = (fRec112[1] + fRec116[1]);
			fRec112[0] = (fSlow23 * (fRec113[0] - (fSlow27 * fRec116[0])));
			float fTemp45 = (fRec112[0] + (fSlow16 * fTemp44));
			fRec111[0] = max((fRec111[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp45))))));
			fVbargraph15 = FAUSTFLOAT(fRec111[0]);
			float fTemp46 = (0.0335968435f * fTemp45);
			fRec119[0] = (fRec118[1] + fRec119[1]);
			fRec120[0] = (fRec119[1] + fRec120[1]);
			float fTemp47 = (fRec3[0] * float(input8[i]));
			fRec118[0] = (0.0f - (fSlow28 * (((fSlow30 * fRec119[0]) + (fSlow29 * fRec120[0])) - (fSlow14 * fTemp47))));
			float fTemp48 = (fRec118[0] + (fSlow16 * fTemp47));
			fRec117[0] = max((fRec117[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp48))))));
			fVbargraph16 = FAUSTFLOAT(fRec117[0]);
			float fTemp49 = (9.21991188e-35f * fTemp48);
			fRec123[0] = (fRec122[1] + fRec123[1]);
			fRec124[0] = (fRec123[1] + fRec124[1]);
			float fTemp50 = (fRec3[0] * float(input7[i]));
			fRec122[0] = (0.0f - (fSlow28 * (((fSlow30 * fRec123[0]) + (fSlow29 * fRec124[0])) - (fSlow14 * fTemp50))));
			float fTemp51 = (fRec122[0] + (fSlow16 * fTemp50));
			fRec121[0] = max((fRec121[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp51))))));
			fVbargraph17 = FAUSTFLOAT(fRec121[0]);
			float fTemp52 = (3.01145194e-18f * fTemp51);
			float fTemp53 = (fRec3[0] * float(input6[i]));
			fRec127[0] = (fRec126[1] + fRec127[1]);
			fRec128[0] = (fRec127[1] + fRec128[1]);
			fRec126[0] = (fSlow28 * ((fSlow14 * fTemp53) - ((fSlow30 * fRec127[0]) + (fSlow29 * fRec128[0]))));
			float fTemp54 = (fRec126[0] + (fSlow16 * fTemp53));
			fRec125[0] = max((fRec125[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp54))))));
			fVbargraph18 = FAUSTFLOAT(fRec125[0]);
			float fTemp55 = (0.0283945147f * fTemp54);
			float fTemp56 = (fRec3[0] * float(input2[i]));
			fRec131[0] = (fRec130[1] + fRec131[1]);
			fRec130[0] = (fSlow31 * ((fSlow14 * fTemp56) - (fSlow32 * fRec131[0])));
			float fTemp57 = (fRec130[0] + (fSlow16 * fTemp56));
			fRec129[0] = max((fRec129[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp57))))));
			fVbargraph19 = FAUSTFLOAT(fRec129[0]);
			float fTemp58 = (0.0219942965f * fTemp57);
			float fTemp59 = (fRec3[0] * float(input3[i]));
			fRec134[0] = (fRec133[1] + fRec134[1]);
			fRec133[0] = (fSlow31 * ((fSlow14 * fTemp59) - (fSlow32 * fRec134[0])));
			float fTemp60 = (fRec133[0] + (fSlow16 * fTemp59));
			fRec132[0] = max((fRec132[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp60))))));
			fVbargraph20 = FAUSTFLOAT(fRec132[0]);
			float fTemp61 = (1.34676222e-18f * fTemp60);
			float fTemp62 = (fRec1[0] * (fTemp1 + (fTemp4 + (fTemp7 + (fTemp10 + (fTemp13 + (fTemp16 + (fTemp19 + (fTemp22 + (fTemp25 + (fTemp28 + (fTemp31 + (fTemp34 + (fTemp37 + (fTemp40 + (fTemp43 + (fTemp46 + (fTemp49 + (fTemp52 + (fTemp55 + (fTemp58 + fTemp61)))))))))))))))))))));
			fRec0[0] = max((fRec0[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp62))))));
			fVbargraph21 = FAUSTFLOAT(fRec0[0]);
			output0[i] = FAUSTFLOAT(fTemp62);
			float fTemp63 = (0.029546842f * fTemp3);
			float fTemp64 = (0.0203892589f * fTemp15);
			float fTemp65 = (0.0281718094f * fTemp21);
			float fTemp66 = (0.0142857144f * fTemp33);
			float fTemp67 = (0.0265606362f * fTemp36);
			float fTemp68 = (0.0219942965f * fTemp60);
			float fTemp69 = (0.0245903712f * fTemp48);
			float fTemp70 = (0.0141972573f * fTemp54);
			float fTemp71 = (0.0205737799f * fTemp42);
			float fTemp72 = (0.0212958846f * fTemp27);
			float fTemp73 = (0.022022916f * fTemp9);
			float fTemp74 = (fRec1[0] * ((fTemp1 + (fTemp63 + (fTemp64 + (fTemp65 + (fTemp66 + (fTemp67 + (fTemp68 + fTemp69))))))) - (((fTemp70 + fTemp71) + fTemp72) + fTemp73)));
			fRec135[0] = max((fRec135[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp74))))));
			fVbargraph22 = FAUSTFLOAT(fRec135[0]);
			output1[i] = FAUSTFLOAT(fTemp74);
			fRec140[0] = (fRec139[1] + fRec140[1]);
			fRec141[0] = (fRec140[1] + fRec141[1]);
			fRec144[0] = (fRec142[1] + fRec144[1]);
			fRec143[0] = (fRec143[1] + fRec144[1]);
			float fTemp75 = (fRec3[0] * float(input27[i]));
			fRec142[0] = (0.0f - (fSlow11 * (((fSlow12 * fRec143[0]) + (fSlow13 * fRec144[0])) - (fSlow14 * fTemp75))));
			fRec139[0] = (0.0f - (fSlow8 * (((fSlow10 * fRec140[0]) + (fSlow9 * fRec141[0])) - fRec142[0])));
			fRec145[0] = (fRec138[1] + fRec145[1]);
			fRec138[0] = (fSlow5 * (fRec139[0] - (fSlow15 * fRec145[0])));
			float fTemp76 = (fRec138[0] + (fSlow16 * fTemp75));
			fRec137[0] = max((fRec137[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp76))))));
			fVbargraph23 = FAUSTFLOAT(fRec137[0]);
			float fTemp77 = (0.022022916f * fTemp76);
			float fTemp78 = (fRec3[0] * float(input1[i]));
			fRec148[0] = (fRec147[1] + fRec148[1]);
			fRec147[0] = (fSlow31 * ((fSlow14 * fTemp78) - (fSlow32 * fRec148[0])));
			float fTemp79 = (fRec147[0] + (fSlow16 * fTemp78));
			fRec146[0] = max((fRec146[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp79))))));
			fVbargraph24 = FAUSTFLOAT(fRec146[0]);
			float fTemp80 = (0.0219942965f * fTemp79);
			fRec151[0] = (fRec150[1] + fRec151[1]);
			fRec152[0] = (fRec151[1] + fRec152[1]);
			float fTemp81 = (fRec3[0] * float(input4[i]));
			fRec150[0] = (0.0f - (fSlow28 * (((fSlow30 * fRec151[0]) + (fSlow29 * fRec152[0])) - (fSlow14 * fTemp81))));
			float fTemp82 = (fRec150[0] + (fSlow16 * fTemp81));
			fRec149[0] = max((fRec149[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp82))))));
			fVbargraph25 = FAUSTFLOAT(fRec149[0]);
			fRec156[0] = (fRec155[1] + fRec156[1]);
			fRec157[0] = (fRec156[1] + fRec157[1]);
			fRec160[0] = (fRec158[1] + fRec160[1]);
			fRec159[0] = (fRec159[1] + fRec160[1]);
			float fTemp83 = (fRec3[0] * float(input25[i]));
			fRec158[0] = (0.0f - (fSlow11 * (((fSlow12 * fRec159[0]) + (fSlow13 * fRec160[0])) - (fSlow14 * fTemp83))));
			fRec155[0] = (0.0f - (fSlow8 * (((fSlow10 * fRec156[0]) + (fSlow9 * fRec157[0])) - fRec158[0])));
			fRec161[0] = (fRec154[1] + fRec161[1]);
			fRec154[0] = (fSlow5 * (fRec155[0] - (fSlow15 * fRec161[0])));
			float fTemp84 = (fRec154[0] + (fSlow16 * fTemp83));
			fRec153[0] = max((fRec153[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp84))))));
			fVbargraph26 = FAUSTFLOAT(fRec153[0]);
			float fTemp85 = (0.029546842f * fTemp84);
			fRec165[0] = (fRec164[1] + fRec165[1]);
			fRec166[0] = (fRec165[1] + fRec166[1]);
			fRec169[0] = (fRec167[1] + fRec169[1]);
			fRec168[0] = (fRec168[1] + fRec169[1]);
			float fTemp86 = (fRec3[0] * float(input29[i]));
			fRec167[0] = (0.0f - (fSlow11 * (((fSlow12 * fRec168[0]) + (fSlow13 * fRec169[0])) - (fSlow14 * fTemp86))));
			fRec164[0] = (0.0f - (fSlow8 * (((fSlow10 * fRec165[0]) + (fSlow9 * fRec166[0])) - fRec167[0])));
			fRec170[0] = (fRec163[1] + fRec170[1]);
			fRec163[0] = (fSlow5 * (fRec164[0] - (fSlow15 * fRec170[0])));
			float fTemp87 = (fRec163[0] + (fSlow16 * fTemp86));
			fRec162[0] = max((fRec162[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp87))))));
			fVbargraph27 = FAUSTFLOAT(fRec162[0]);
			float fTemp88 = (0.0203892589f * fTemp87);
			fRec173[0] = (fRec172[1] + fRec173[1]);
			fRec174[0] = (fRec173[1] + fRec174[1]);
			fRec176[0] = (fRec175[1] + fRec176[1]);
			fRec177[0] = (fRec176[1] + fRec177[1]);
			float fTemp89 = (fRec3[0] * float(input18[i]));
			fRec175[0] = (0.0f - (fSlow20 * (((fSlow22 * fRec176[0]) + (fSlow21 * fRec177[0])) - (fSlow14 * fTemp89))));
			fRec172[0] = (0.0f - (fSlow17 * (((fSlow19 * fRec173[0]) + (fSlow18 * fRec174[0])) - fRec175[0])));
			float fTemp90 = (fRec172[0] + (fSlow16 * fTemp89));
			fRec171[0] = max((fRec171[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp90))))));
			fVbargraph28 = FAUSTFLOAT(fRec171[0]);
			fRec181[0] = (fRec180[1] + fRec181[1]);
			fRec182[0] = (fRec181[1] + fRec182[1]);
			float fTemp91 = (fRec3[0] * float(input11[i]));
			fRec180[0] = (0.0f - (fSlow24 * (((fSlow26 * fRec181[0]) + (fSlow25 * fRec182[0])) - (fSlow14 * fTemp91))));
			fRec183[0] = (fRec179[1] + fRec183[1]);
			fRec179[0] = (fSlow23 * (fRec180[0] - (fSlow27 * fRec183[0])));
			float fTemp92 = (fRec179[0] + (fSlow16 * fTemp91));
			fRec178[0] = max((fRec178[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp92))))));
			fVbargraph29 = FAUSTFLOAT(fRec178[0]);
			float fTemp93 = (0.0205737799f * fTemp92);
			fRec187[0] = (fRec186[1] + fRec187[1]);
			fRec188[0] = (fRec187[1] + fRec188[1]);
			float fTemp94 = (fRec3[0] * float(input9[i]));
			fRec186[0] = (0.0f - (fSlow24 * (((fSlow26 * fRec187[0]) + (fSlow25 * fRec188[0])) - (fSlow14 * fTemp94))));
			fRec189[0] = (fRec185[1] + fRec189[1]);
			fRec185[0] = (fSlow23 * (fRec186[0] - (fSlow27 * fRec189[0])));
			float fTemp95 = (fRec185[0] + (fSlow16 * fTemp94));
			fRec184[0] = max((fRec184[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp95))))));
			fVbargraph30 = FAUSTFLOAT(fRec184[0]);
			float fTemp96 = (0.0265606362f * fTemp95);
			fRec192[0] = (fRec191[1] + fRec192[1]);
			fRec193[0] = (fRec192[1] + fRec193[1]);
			fRec195[0] = (fRec194[1] + fRec195[1]);
			fRec196[0] = (fRec195[1] + fRec196[1]);
			float fTemp97 = (fRec3[0] * float(input16[i]));
			fRec194[0] = (0.0f - (fSlow20 * (((fSlow22 * fRec195[0]) + (fSlow21 * fRec196[0])) - (fSlow14 * fTemp97))));
			fRec191[0] = (0.0f - (fSlow17 * (((fSlow19 * fRec192[0]) + (fSlow18 * fRec193[0])) - fRec194[0])));
			float fTemp98 = (fRec191[0] + (fSlow16 * fTemp97));
			fRec190[0] = max((fRec190[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp98))))));
			fVbargraph31 = FAUSTFLOAT(fRec190[0]);
			float fTemp99 = (fRec1[0] * ((fTemp1 + (fTemp77 + (fTemp80 + ((3.01145194e-18f * fTemp82) + (fTemp85 + (fTemp88 + ((9.0461117e-18f * fTemp3) + ((4.04554392e-18f * fTemp9) + ((1.248482e-18f * fTemp15) + (fTemp65 + (fTemp72 + (fTemp61 + fTemp66)))))))))))) - ((2.60799377e-18f * fTemp90) + (fTemp93 + (fTemp96 + ((6.90010286e-18f * fTemp98) + ((4.87910954e-18f * fTemp36) + ((1.25978067e-18f * fTemp42) + (fTemp69 + fTemp70)))))))));
			fRec136[0] = max((fRec136[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp99))))));
			fVbargraph32 = FAUSTFLOAT(fRec136[0]);
			output2[i] = FAUSTFLOAT(fTemp99);
			float fTemp100 = (fRec1[0] * ((fTemp1 + ((5.21598753e-18f * fTemp90) + ((9.75821908e-18f * fTemp95) + ((2.69352445e-18f * fTemp79) + ((1.80922234e-17f * fTemp84) + ((2.496964e-18f * fTemp87) + (fTemp73 + (fTemp65 + (fTemp66 + (fTemp69 + fTemp71)))))))))) - ((2.51956133e-18f * fTemp92) + ((1.38002057e-17f * fTemp98) + ((8.09108784e-18f * fTemp76) + ((6.02290387e-18f * fTemp82) + (fTemp63 + (fTemp64 + (fTemp72 + (fTemp67 + (fTemp68 + fTemp70)))))))))));
			fRec197[0] = max((fRec197[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp100))))));
			fVbargraph33 = FAUSTFLOAT(fRec197[0]);
			output3[i] = FAUSTFLOAT(fTemp100);
			float fTemp101 = (fRec1[0] * ((fTemp1 + (fTemp93 + (fTemp96 + ((9.03435581e-18f * fTemp82) + (fTemp65 + (fTemp72 + (fTemp66 + ((3.7793419e-18f * fTemp42) + (1.46373282e-17f * fTemp36))))))))) - ((7.8239813e-18f * fTemp90) + ((2.0700309e-17f * fTemp98) + (fTemp77 + (fTemp80 + (fTemp85 + (fTemp88 + ((((fTemp69 + (fTemp70 + (4.04028677e-18f * fTemp60))) + (3.7454461e-18f * fTemp15)) + (1.21366322e-17f * fTemp9)) + (7.96240683e-17f * fTemp3))))))))));
			fRec198[0] = max((fRec198[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp101))))));
			fVbargraph34 = FAUSTFLOAT(fRec198[0]);
			output4[i] = FAUSTFLOAT(fTemp101);
			float fTemp102 = (fRec1[0] * ((fTemp1 + (fTemp4 + (fTemp10 + (fTemp16 + (fTemp22 + (fTemp28 + (fTemp34 + (fTemp37 + (fTemp43 + (fTemp49 + (fTemp61 + fTemp55))))))))))) - (fTemp7 + (fTemp13 + (fTemp19 + (fTemp25 + (fTemp31 + (fTemp40 + (fTemp46 + (fTemp58 + fTemp52))))))))));
			fRec199[0] = max((fRec199[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp102))))));
			fVbargraph35 = FAUSTFLOAT(fRec199[0]);
			output5[i] = FAUSTFLOAT(fTemp102);
			float fTemp103 = (0.0225749556f * fTemp0);
			float fTemp104 = (0.00928567629f * fTemp3);
			float fTemp105 = (0.0293638874f * fTemp6);
			float fTemp106 = (0.0484479442f * fTemp9);
			float fTemp107 = (0.0339064971f * fTemp12);
			float fTemp108 = (0.0125208041f * fTemp21);
			float fTemp109 = (0.0354141816f * fTemp24);
			float fTemp110 = (0.0473241918f * fTemp27);
			float fTemp111 = (0.0133853024f * fTemp30);
			float fTemp112 = (0.0166944042f * fTemp36);
			float fTemp113 = (0.0408927724f * fTemp39);
			float fTemp114 = (0.0387942903f * fTemp42);
			float fTemp115 = (0.0218581073f * fTemp48);
			float fTemp116 = (0.0437162146f * fTemp51);
			float fTemp117 = (0.0126197841f * fTemp54);
			float fTemp118 = (0.0276485607f * fTemp57);
			float fTemp119 = (0.0276485607f * fTemp60);
			float fTemp120 = (fTemp118 + fTemp119);
			float fTemp121 = (0.0105584692f * fTemp45);
			float fTemp122 = (0.0275132284f * fTemp33);
			float fTemp123 = (0.0281259492f * fTemp18);
			float fTemp124 = (0.0192231759f * fTemp15);
			float fTemp125 = (fRec1[0] * ((fTemp103 + (fTemp104 + (fTemp105 + (fTemp106 + (fTemp107 + (fTemp108 + (fTemp109 + (fTemp110 + (fTemp111 + (fTemp112 + (fTemp113 + (fTemp114 + (fTemp115 + (fTemp116 + (fTemp117 + fTemp120))))))))))))))) - (((fTemp121 + fTemp122) + fTemp123) + fTemp124)));
			fRec200[0] = max((fRec200[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp125))))));
			fVbargraph36 = FAUSTFLOAT(fRec200[0]);
			output6[i] = FAUSTFLOAT(fTemp125);
			fRec204[0] = (fRec203[1] + fRec204[1]);
			fRec205[0] = (fRec204[1] + fRec205[1]);
			fRec207[0] = (fRec206[1] + fRec207[1]);
			fRec208[0] = (fRec207[1] + fRec208[1]);
			float fTemp126 = (fRec3[0] * float(input19[i]));
			fRec206[0] = (0.0f - (fSlow20 * (((fSlow22 * fRec207[0]) + (fSlow21 * fRec208[0])) - (fSlow14 * fTemp126))));
			fRec203[0] = (0.0f - (fSlow17 * (((fSlow19 * fRec204[0]) + (fSlow18 * fRec205[0])) - fRec206[0])));
			float fTemp127 = (fRec203[0] + (fSlow16 * fTemp126));
			fRec202[0] = max((fRec202[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp127))))));
			fVbargraph37 = FAUSTFLOAT(fRec202[0]);
			float fTemp128 = (0.0133853024f * fTemp127);
			fRec212[0] = (fRec211[1] + fRec212[1]);
			fRec213[0] = (fRec212[1] + fRec213[1]);
			fRec216[0] = (fRec214[1] + fRec216[1]);
			fRec215[0] = (fRec215[1] + fRec216[1]);
			float fTemp129 = (fRec3[0] * float(input28[i]));
			fRec214[0] = (0.0f - (fSlow11 * (((fSlow12 * fRec215[0]) + (fSlow13 * fRec216[0])) - (fSlow14 * fTemp129))));
			fRec211[0] = (0.0f - (fSlow8 * (((fSlow10 * fRec212[0]) + (fSlow9 * fRec213[0])) - fRec214[0])));
			fRec217[0] = (fRec210[1] + fRec217[1]);
			fRec210[0] = (fSlow5 * (fRec211[0] - (fSlow15 * fRec217[0])));
			float fTemp130 = (fRec210[0] + (fSlow16 * fTemp129));
			fRec209[0] = max((fRec209[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp130))))));
			fVbargraph38 = FAUSTFLOAT(fRec209[0]);
			float fTemp131 = (4.15234824e-18f * fTemp130);
			fRec221[0] = (fRec220[1] + fRec221[1]);
			fRec222[0] = (fRec221[1] + fRec222[1]);
			float fTemp132 = (fRec3[0] * float(input10[i]));
			fRec220[0] = (0.0f - (fSlow24 * (((fSlow26 * fRec221[0]) + (fSlow25 * fRec222[0])) - (fSlow14 * fTemp132))));
			fRec223[0] = (fRec219[1] + fRec223[1]);
			fRec219[0] = (fSlow23 * (fRec220[0] - (fSlow27 * fRec223[0])));
			float fTemp133 = (fRec219[0] + (fSlow16 * fTemp132));
			fRec218[0] = max((fRec218[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp133))))));
			fVbargraph39 = FAUSTFLOAT(fRec218[0]);
			float fTemp134 = (5.00792059e-18f * fTemp133);
			fRec226[0] = (fRec225[1] + fRec226[1]);
			fRec227[0] = (fRec226[1] + fRec227[1]);
			float fTemp135 = (fRec3[0] * float(input5[i]));
			fRec225[0] = (0.0f - (fSlow28 * (((fSlow30 * fRec226[0]) + (fSlow29 * fRec227[0])) - (fSlow14 * fTemp135))));
			float fTemp136 = (fRec225[0] + (fSlow16 * fTemp135));
			fRec224[0] = max((fRec224[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp136))))));
			fVbargraph40 = FAUSTFLOAT(fRec224[0]);
			float fTemp137 = (0.0437162146f * fTemp136);
			float fTemp138 = (5.7955417e-18f * fTemp90);
			float fTemp139 = (0.0387942903f * fTemp92);
			float fTemp140 = (0.0276485607f * fTemp79);
			float fTemp141 = (2.676846e-18f * fTemp82);
			float fTemp142 = (0.00928567629f * fTemp84);
			float fTemp143 = (2.84291844e-18f * fTemp3);
			float fTemp144 = (8.19613358e-19f * fTemp30);
			float fTemp145 = (2.37546523e-18f * fTemp42);
			float fTemp146 = (2.676846e-18f * fTemp51);
			float fTemp147 = (1.69298611e-18f * fTemp60);
			fRec230[0] = (fRec229[1] + fRec230[1]);
			fRec231[0] = (fRec230[1] + fRec231[1]);
			fRec233[0] = (fRec232[1] + fRec233[1]);
			fRec234[0] = (fRec233[1] + fRec234[1]);
			float fTemp148 = (fRec3[0] * float(input17[i]));
			fRec232[0] = (0.0f - (fSlow20 * (((fSlow22 * fRec233[0]) + (fSlow21 * fRec234[0])) - (fSlow14 * fTemp148))));
			fRec229[0] = (0.0f - (fSlow17 * (((fSlow19 * fRec230[0]) + (fSlow18 * fRec231[0])) - fRec232[0])));
			float fTemp149 = (fRec229[0] + (fSlow16 * fTemp148));
			fRec228[0] = max((fRec228[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp149))))));
			fVbargraph41 = FAUSTFLOAT(fRec228[0]);
			float fTemp150 = (0.0354141816f * fTemp149);
			fRec238[0] = (fRec237[1] + fRec238[1]);
			fRec239[0] = (fRec238[1] + fRec239[1]);
			fRec242[0] = (fRec240[1] + fRec242[1]);
			fRec241[0] = (fRec241[1] + fRec242[1]);
			float fTemp151 = (fRec3[0] * float(input26[i]));
			fRec240[0] = (0.0f - (fSlow11 * (((fSlow12 * fRec241[0]) + (fSlow13 * fRec242[0])) - (fSlow14 * fTemp151))));
			fRec237[0] = (0.0f - (fSlow8 * (((fSlow10 * fRec238[0]) + (fSlow9 * fRec239[0])) - fRec240[0])));
			fRec243[0] = (fRec236[1] + fRec243[1]);
			fRec236[0] = (fSlow5 * (fRec237[0] - (fSlow15 * fRec243[0])));
			float fTemp152 = (fRec236[0] + (fSlow16 * fTemp151));
			fRec235[0] = max((fRec235[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp152))))));
			fVbargraph42 = FAUSTFLOAT(fRec235[0]);
			float fTemp153 = (7.19207817e-18f * fTemp152);
			float fTemp154 = (3.06671236e-18f * (fTemp36 + fTemp98));
			float fTemp155 = (0.0166944042f * fTemp95);
			float fTemp156 = (0.0484479442f * fTemp76);
			float fTemp157 = (0.0192231759f * fTemp87);
			float fTemp158 = (6.50547939e-18f * fTemp24);
			float fTemp159 = (1.17708008e-18f * fTemp15);
			float fTemp160 = (8.89974292e-18f * fTemp9);
			float fTemp161 = (fRec1[0] * ((fTemp103 + (fTemp128 + (fTemp131 + (fTemp134 + (fTemp137 + (fTemp138 + (fTemp139 + (fTemp140 + (fTemp141 + (fTemp142 + (fTemp143 + (fTemp105 + (fTemp108 + (fTemp144 + (fTemp145 + (fTemp146 + (fTemp117 + (fTemp118 + fTemp147)))))))))))))))))) - (fTemp150 + (fTemp153 + (fTemp154 + (fTemp155 + (fTemp156 + (fTemp157 + ((fTemp107 + ((fTemp123 + ((fTemp110 + (fTemp122 + (fTemp113 + (fTemp115 + fTemp121)))) + fTemp158)) + fTemp159)) + fTemp160)))))))));
			fRec201[0] = max((fRec201[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp161))))));
			fVbargraph43 = FAUSTFLOAT(fRec201[0]);
			output7[i] = FAUSTFLOAT(fTemp161);
			float fTemp162 = (6.13342471e-18f * (fTemp95 - fTemp98));
			float fTemp163 = (1.30109588e-17f * fTemp149);
			float fTemp164 = (1.63922672e-18f * fTemp127);
			float fTemp165 = (5.35369201e-18f * fTemp136);
			float fTemp166 = (4.75093046e-18f * fTemp92);
			float fTemp167 = (1.77994858e-17f * fTemp76);
			float fTemp168 = (3.38597223e-18f * fTemp79);
			float fTemp169 = (5.68583687e-18f * fTemp84);
			float fTemp170 = (fTemp118 + fTemp117);
			float fTemp171 = (1.43841563e-17f * fTemp152);
			float fTemp172 = (8.30469648e-18f * fTemp130);
			float fTemp173 = (1.00158412e-17f * fTemp133);
			float fTemp174 = (1.15910834e-17f * fTemp90);
			float fTemp175 = (5.35369201e-18f * fTemp82);
			float fTemp176 = (2.35416016e-18f * fTemp87);
			float fTemp177 = (fRec1[0] * ((fTemp103 + (fTemp162 + (fTemp163 + (fTemp164 + (fTemp165 + (fTemp166 + (fTemp167 + (fTemp168 + (fTemp169 + (fTemp105 + (fTemp107 + (fTemp124 + (fTemp108 + (fTemp110 + (fTemp113 + (fTemp115 + fTemp170)))))))))))))))) - (fTemp171 + (fTemp172 + (fTemp173 + (fTemp174 + (fTemp175 + (fTemp176 + (fTemp104 + (fTemp106 + (fTemp123 + (fTemp109 + (fTemp111 + (fTemp122 + (fTemp112 + (fTemp114 + (fTemp121 + (fTemp119 + fTemp116))))))))))))))))));
			fRec244[0] = max((fRec244[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp177))))));
			fVbargraph44 = FAUSTFLOAT(fRec244[0]);
			output8[i] = FAUSTFLOAT(fTemp177);
			float fTemp178 = (1.24570447e-17f * fTemp130);
			float fTemp179 = (1.50237622e-17f * fTemp133);
			float fTemp180 = (1.73866251e-17f * fTemp90);
			float fTemp181 = (8.03053822e-18f * fTemp82);
			float fTemp182 = (2.66992279e-17f * fTemp9);
			float fTemp183 = (3.53124024e-18f * fTemp15);
			float fTemp184 = (1.95164382e-17f * fTemp24);
			float fTemp185 = (9.20013769e-18f * fTemp36);
			float fTemp186 = (2.15762353e-17f * fTemp152);
			float fTemp187 = (9.20013769e-18f * fTemp98);
			float fTemp188 = (5.07895844e-18f * fTemp60);
			float fTemp189 = (8.03053822e-18f * fTemp51);
			float fTemp190 = (7.1263959e-18f * fTemp42);
			float fTemp191 = (2.45884007e-18f * fTemp30);
			float fTemp192 = (2.50234311e-17f * fTemp3);
			float fTemp193 = (fRec1[0] * ((fTemp103 + (fTemp150 + (fTemp178 + (fTemp179 + (fTemp180 + (fTemp155 + (fTemp156 + (fTemp181 + (fTemp157 + (fTemp105 + (fTemp182 + (fTemp183 + (fTemp108 + (fTemp184 + (fTemp185 + fTemp170))))))))))))))) - (fTemp186 + (fTemp128 + (fTemp137 + (fTemp139 + (fTemp187 + (fTemp140 + (fTemp142 + ((fTemp107 + (fTemp123 + (fTemp110 + ((fTemp122 + (fTemp113 + ((fTemp121 + (fTemp115 + (fTemp188 + fTemp189))) + fTemp190))) + fTemp191)))) + fTemp192))))))))));
			fRec245[0] = max((fRec245[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp193))))));
			fVbargraph45 = FAUSTFLOAT(fRec245[0]);
			output9[i] = FAUSTFLOAT(fTemp193);
			float fTemp194 = (0.0276485607f * fTemp79);
			float fTemp195 = (0.0437162146f * fTemp82);
			float fTemp196 = (0.0256309025f * fTemp87);
			float fTemp197 = (0.0256309025f * fTemp15);
			float fTemp198 = (0.0253968257f * fTemp33);
			float fTemp199 = (0.0378593504f * fTemp90);
			float fTemp200 = (0.0258628614f * fTemp92);
			float fTemp201 = (0.0252395682f * fTemp54);
			float fTemp202 = (0.0258628614f * fTemp42);
			float fTemp203 = (0.0500832163f * fTemp21);
			float fTemp204 = (fRec1[0] * ((fTemp103 + ((6.13342471e-18f * fTemp98) + (fTemp194 + (fTemp195 + (fTemp196 + ((0.0276845396f * fTemp9) + (fTemp197 + (fTemp198 + (fTemp119 + (2.676846e-18f * fTemp48)))))))))) - ((0.0333888084f * (fTemp36 - fTemp95)) + (fTemp199 + (fTemp200 + ((0.0276845396f * fTemp76) + ((0.0371427052f * fTemp84) + ((((fTemp201 + fTemp202) + (2.31821668e-18f * fTemp27)) + fTemp203) + (0.0371427052f * fTemp3)))))))));
			fRec246[0] = max((fRec246[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp204))))));
			fVbargraph46 = FAUSTFLOAT(fRec246[0]);
			output10[i] = FAUSTFLOAT(fTemp204);
			float fTemp205 = (0.0256309025f * fTemp87);
			float fTemp206 = (0.0258628614f * fTemp92);
			float fTemp207 = (fRec1[0] * ((fTemp103 + (fTemp199 + ((0.0333888084f * fTemp95) + ((1.84002754e-17f * fTemp98) + (fTemp140 + (fTemp205 + ((0.0371427052f * fTemp3) + ((6.95465004e-18f * fTemp27) + (fTemp198 + ((0.0258628614f * fTemp42) + (0.0333888084f * fTemp36))))))))))) - (fTemp206 + ((0.0276845396f * fTemp76) + (fTemp195 + ((0.0371427052f * fTemp84) + (((fTemp203 + ((fTemp201 + (0.0276485607f * fTemp60)) + (8.03053822e-18f * fTemp48))) + (0.0256309025f * fTemp15)) + (0.0276845396f * fTemp9))))))));
			fRec247[0] = max((fRec247[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp207))))));
			fVbargraph47 = FAUSTFLOAT(fRec247[0]);
			output11[i] = FAUSTFLOAT(fTemp207);
			float fTemp208 = (fRec1[0] * ((fTemp103 + (fTemp200 + ((3.06671256e-17f * fTemp98) + ((0.0276845396f * fTemp76) + (fTemp195 + ((0.0371427052f * fTemp84) + ((0.0371427052f * fTemp3) + (fTemp198 + ((0.0333888084f * fTemp36) + (fTemp202 + (1.33842306e-17f * fTemp48))))))))))) - (fTemp199 + ((0.0333888084f * fTemp95) + (fTemp194 + (fTemp196 + ((fTemp197 + (fTemp203 + ((fTemp119 + fTemp201) + (1.15910834e-17f * fTemp27)))) + (0.0276845396f * fTemp9))))))));
			fRec248[0] = max((fRec248[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp208))))));
			fVbargraph48 = FAUSTFLOAT(fRec248[0]);
			output12[i] = FAUSTFLOAT(fTemp208);
			float fTemp209 = (fRec1[0] * ((fTemp103 + (fTemp199 + (fTemp206 + ((4.29339759e-17f * fTemp98) + ((0.0276845396f * fTemp76) + ((0.0371427052f * fTemp84) + ((0.0276845396f * fTemp9) + ((0.0256309025f * fTemp15) + ((1.62275168e-17f * fTemp27) + (fTemp198 + (0.0276485607f * fTemp60))))))))))) - ((0.0333888084f * fTemp95) + (fTemp140 + (fTemp195 + (fTemp205 + ((fTemp203 + (((fTemp201 + (1.87379222e-17f * fTemp48)) + (0.0258628614f * fTemp42)) + (0.0333888084f * fTemp36))) + (0.0371427052f * fTemp3))))))));
			fRec249[0] = max((fRec249[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp209))))));
			fVbargraph49 = FAUSTFLOAT(fRec249[0]);
			output13[i] = FAUSTFLOAT(fTemp209);
			float fTemp210 = (fRec1[0] * ((fTemp103 + (fTemp104 + (fTemp106 + (fTemp123 + (fTemp108 + (fTemp110 + (fTemp112 + (fTemp114 + (fTemp121 + (fTemp115 + (fTemp119 + fTemp117))))))))))) - (fTemp105 + (fTemp107 + (fTemp124 + (fTemp109 + (fTemp111 + (fTemp122 + (fTemp113 + (fTemp118 + fTemp116))))))))));
			fRec250[0] = max((fRec250[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp210))))));
			fVbargraph50 = FAUSTFLOAT(fRec250[0]);
			output14[i] = FAUSTFLOAT(fTemp210);
			float fTemp211 = (fRec1[0] * ((fTemp103 + (fTemp150 + (fTemp153 + (fTemp138 + (fTemp139 + (fTemp140 + (fTemp141 + (fTemp142 + (fTemp143 + (fTemp107 + (fTemp123 + (fTemp108 + (fTemp158 + (fTemp113 + (fTemp145 + (fTemp121 + (fTemp117 + fTemp147))))))))))))))))) - (fTemp154 + (fTemp128 + (fTemp131 + (fTemp134 + (fTemp137 + (fTemp155 + (fTemp156 + (fTemp157 + (fTemp105 + (fTemp160 + (fTemp159 + (fTemp110 + (fTemp144 + (fTemp122 + (fTemp115 + (fTemp118 + fTemp146))))))))))))))))));
			fRec251[0] = max((fRec251[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp211))))));
			fVbargraph51 = FAUSTFLOAT(fRec251[0]);
			output15[i] = FAUSTFLOAT(fTemp211);
			float fTemp212 = (fRec1[0] * ((fTemp103 + (fTemp162 + (fTemp171 + (fTemp172 + (fTemp173 + (fTemp166 + (fTemp167 + (fTemp168 + (fTemp169 + (fTemp124 + (fTemp123 + (fTemp108 + (fTemp109 + (fTemp110 + (fTemp111 + (fTemp121 + (fTemp115 + (fTemp117 + fTemp116)))))))))))))))))) - (fTemp163 + (fTemp164 + (fTemp165 + (fTemp174 + (fTemp175 + (fTemp176 + (fTemp104 + (fTemp105 + (fTemp106 + (fTemp107 + (fTemp122 + (fTemp112 + (fTemp113 + (fTemp114 + fTemp120))))))))))))))));
			fRec252[0] = max((fRec252[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp212))))));
			fVbargraph52 = FAUSTFLOAT(fRec252[0]);
			output16[i] = FAUSTFLOAT(fTemp212);
			float fTemp213 = (fRec1[0] * ((fTemp103 + (fTemp186 + (fTemp128 + (fTemp137 + (fTemp180 + (fTemp155 + (fTemp156 + (fTemp181 + (fTemp157 + (fTemp182 + (fTemp107 + (fTemp183 + (fTemp123 + (fTemp108 + (fTemp191 + (fTemp185 + (fTemp113 + (fTemp121 + (fTemp117 + fTemp189))))))))))))))))))) - (fTemp150 + (fTemp178 + (fTemp179 + (fTemp139 + (fTemp187 + (fTemp140 + (fTemp142 + (fTemp192 + (fTemp105 + (fTemp184 + (fTemp110 + (fTemp122 + (fTemp190 + (fTemp115 + (fTemp118 + fTemp188)))))))))))))))));
			fRec253[0] = max((fRec253[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp213))))));
			fVbargraph53 = FAUSTFLOAT(fRec253[0]);
			output17[i] = FAUSTFLOAT(fTemp213);
			float fTemp214 = (fTemp57 + fTemp0);
			float fTemp215 = (0.0210937504f * fTemp214);
			float fTemp216 = (0.0294138286f * fTemp149);
			float fTemp217 = (4.87734103e-18f * fTemp152);
			float fTemp218 = (1.53169795e-17f * fTemp130);
			float fTemp219 = (0.0415974371f * fTemp133);
			float fTemp220 = (0.0272319149f * fTemp136);
			float fTemp221 = (0.031444706f * fTemp90);
			float fTemp222 = (0.0131542645f * fTemp92);
			float fTemp223 = (0.0169820823f * fTemp95);
			float fTemp224 = (2.54710831e-18f * fTemp98);
			float fTemp225 = (0.0281615965f * fTemp76);
			float fTemp226 = (0.0210937504f * fTemp79);
			float fTemp227 = (0.0272319149f * fTemp82);
			float fTemp228 = (5.23659549e-18f * fTemp54);
			float fTemp229 = ((0.0210937504f * fTemp60) + fTemp228);
			float fTemp230 = (0.0272319149f * fTemp51);
			float fTemp231 = (1.6674738e-18f * fTemp48);
			float fTemp232 = (0.0131542645f * fTemp42);
			float fTemp233 = (2.54710831e-18f * fTemp39);
			float fTemp234 = (1.92543286e-18f * fTemp27);
			float fTemp235 = (9.37894525e-34f * fTemp12);
			float fTemp236 = (0.011117382f * (fTemp30 + fTemp127));
			float fTemp237 = (0.0125942491f * fTemp84);
			float fTemp238 = (0.0260725729f * fTemp87);
			float fTemp239 = (0.0214808229f * fTemp45);
			float fTemp240 = (0.0169820823f * fTemp36);
			float fTemp241 = (0.0246093757f * fTemp33);
			float fTemp242 = (0.0294138286f * fTemp24);
			float fTemp243 = (0.0207987186f * fTemp21);
			float fTemp244 = (0.00673190923f * fTemp18);
			float fTemp245 = (0.0260725729f * fTemp15);
			float fTemp246 = (0.0281615965f * fTemp9);
			float fTemp247 = (0.0398265123f * fTemp6);
			float fTemp248 = (0.0125942491f * fTemp3);
			float fTemp249 = (fRec1[0] * ((fTemp215 + (fTemp216 + (fTemp217 + (fTemp218 + (fTemp219 + (fTemp220 + (fTemp221 + (fTemp222 + (fTemp223 + (fTemp224 + (fTemp225 + (fTemp226 + (fTemp227 + ((((((fTemp229 + fTemp230) + fTemp231) + fTemp232) + fTemp233) + fTemp234) + fTemp235)))))))))))))) - (fTemp236 + (fTemp237 + (fTemp238 + (((((((((fTemp239 + fTemp240) + fTemp241) + fTemp242) + fTemp243) + fTemp244) + fTemp245) + fTemp246) + fTemp247) + fTemp248))))));
			fRec254[0] = max((fRec254[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp249))))));
			fVbargraph54 = FAUSTFLOAT(fRec254[0]);
			output18[i] = FAUSTFLOAT(fTemp249);
			float fTemp250 = (0.011117382f * (fTemp30 - fTemp127));
			float fTemp251 = (0.0294138286f * fTemp149);
			float fTemp252 = (1.46320227e-17f * fTemp152);
			float fTemp253 = (0.0272319149f * fTemp136);
			float fTemp254 = (0.0131542645f * fTemp92);
			float fTemp255 = (0.0169820823f * fTemp95);
			float fTemp256 = (7.64132493e-18f * fTemp98);
			float fTemp257 = (0.0281615965f * fTemp76);
			float fTemp258 = (0.0210937504f * fTemp79);
			float fTemp259 = (0.0169820823f * fTemp36);
			float fTemp260 = (0.0294138286f * fTemp24);
			float fTemp261 = (0.0260725729f * fTemp15);
			float fTemp262 = (0.0281615965f * fTemp9);
			float fTemp263 = (0.0125942491f * fTemp3);
			float fTemp264 = (0.0125942491f * fTemp84);
			float fTemp265 = (0.0260725729f * fTemp87);
			float fTemp266 = (0.0210937504f * fTemp60);
			float fTemp267 = (0.0272319149f * fTemp51);
			float fTemp268 = (5.00242149e-18f * fTemp48);
			float fTemp269 = (0.0131542645f * fTemp42);
			float fTemp270 = (7.64132493e-18f * fTemp39);
			float fTemp271 = (5.77629858e-18f * fTemp27);
			float fTemp272 = (2.81368348e-33f * fTemp12);
			float fTemp273 = (fRec1[0] * ((fTemp215 + (fTemp250 + (fTemp251 + (fTemp252 + (fTemp253 + (fTemp254 + (fTemp255 + (fTemp256 + (fTemp257 + (fTemp258 + (((((fTemp228 + fTemp259) + fTemp260) + fTemp261) + fTemp262) + fTemp263))))))))))) - (fTemp218 + (fTemp219 + (fTemp221 + (fTemp227 + (fTemp264 + (fTemp265 + (fTemp247 + ((fTemp244 + (fTemp243 + ((fTemp241 + (((fTemp239 + ((fTemp266 + fTemp267) + fTemp268)) + fTemp269) + fTemp270)) + fTemp271))) + fTemp272))))))))));
			fRec255[0] = max((fRec255[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp273))))));
			fVbargraph55 = FAUSTFLOAT(fRec255[0]);
			output19[i] = FAUSTFLOAT(fTemp273);
			float fTemp274 = (2.43867056e-17f * fTemp152);
			float fTemp275 = (0.011117382f * fTemp127);
			float fTemp276 = (1.27355415e-17f * fTemp98);
			float fTemp277 = (0.0125942491f * fTemp84);
			float fTemp278 = (8.33736929e-18f * fTemp48);
			float fTemp279 = (1.27355415e-17f * fTemp39);
			float fTemp280 = (0.0169820823f * fTemp36);
			float fTemp281 = (0.011117382f * fTemp30);
			float fTemp282 = (9.62716472e-18f * fTemp27);
			float fTemp283 = (0.0294138286f * fTemp24);
			float fTemp284 = (0.0260725729f * fTemp15);
			float fTemp285 = (4.68947235e-33f * fTemp12);
			float fTemp286 = (0.0281615965f * fTemp9);
			float fTemp287 = (0.0125942491f * fTemp3);
			float fTemp288 = (0.0294138286f * fTemp149);
			float fTemp289 = (0.0169820823f * fTemp95);
			float fTemp290 = (0.0281615965f * fTemp76);
			float fTemp291 = (0.0131542645f * fTemp42);
			float fTemp292 = (fRec1[0] * ((fTemp274 + (fTemp275 + (fTemp218 + (fTemp219 + (fTemp221 + (fTemp276 + (fTemp227 + (fTemp277 + (fTemp238 + ((((((((((fTemp228 + fTemp278) + fTemp279) + fTemp280) + fTemp281) + fTemp282) + fTemp283) + fTemp284) + fTemp285) + fTemp286) + fTemp287)))))))))) - ((0.0210937504f * (fTemp60 - fTemp214)) + (fTemp288 + (fTemp220 + (fTemp222 + (fTemp289 + (fTemp290 + (fTemp226 + (fTemp247 + (fTemp244 + (fTemp243 + (fTemp241 + ((fTemp230 + fTemp239) + fTemp291))))))))))))));
			fRec256[0] = max((fRec256[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp292))))));
			fVbargraph56 = FAUSTFLOAT(fRec256[0]);
			output20[i] = FAUSTFLOAT(fTemp292);
			float fTemp293 = (3.41413868e-17f * fTemp152);
			float fTemp294 = (0.011117382f * fTemp127);
			float fTemp295 = (1.7829759e-17f * fTemp98);
			float fTemp296 = (0.0125942491f * fTemp84);
			float fTemp297 = (fTemp228 + (0.0210937504f * fTemp60));
			float fTemp298 = (0.0272319149f * fTemp51);
			float fTemp299 = (0.0260725729f * fTemp87);
			float fTemp300 = (0.0294138286f * fTemp149);
			float fTemp301 = (0.0131542645f * fTemp92);
			float fTemp302 = (0.0169820823f * fTemp95);
			float fTemp303 = (0.0281615965f * fTemp76);
			float fTemp304 = (1.16723167e-17f * fTemp48);
			float fTemp305 = (1.7829759e-17f * fTemp39);
			float fTemp306 = (0.011117382f * fTemp30);
			float fTemp307 = (1.34780304e-17f * fTemp27);
			float fTemp308 = (0.0294138286f * fTemp24);
			float fTemp309 = (0.0260725729f * fTemp15);
			float fTemp310 = (6.56526158e-33f * fTemp12);
			float fTemp311 = (0.0281615965f * fTemp9);
			float fTemp312 = (0.0125942491f * fTemp3);
			float fTemp313 = (fRec1[0] * (((0.0210937504f * (fTemp214 - fTemp79)) + (fTemp293 + (fTemp294 + (fTemp295 + (fTemp296 + ((fTemp269 + (fTemp297 + fTemp298)) + fTemp299)))))) - (fTemp300 + (fTemp218 + (fTemp219 + (fTemp253 + (fTemp221 + (fTemp301 + (fTemp302 + (fTemp303 + (fTemp227 + ((fTemp247 + ((((fTemp244 + (fTemp243 + ((((fTemp241 + (fTemp240 + ((fTemp239 + fTemp304) + fTemp305))) + fTemp306) + fTemp307) + fTemp308))) + fTemp309) + fTemp310) + fTemp311)) + fTemp312))))))))))));
			fRec257[0] = max((fRec257[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp313))))));
			fVbargraph57 = FAUSTFLOAT(fRec257[0]);
			output21[i] = FAUSTFLOAT(fTemp313);
			float fTemp314 = (0.0210937504f * (fTemp0 - fTemp57));
			float fTemp315 = (fRec1[0] * ((fTemp314 + (fTemp236 + (fTemp221 + (fTemp222 + (fTemp223 + (fTemp224 + (fTemp225 + (fTemp226 + (fTemp227 + (fTemp247 + (fTemp244 + (fTemp242 + (fTemp234 + (fTemp232 + (fTemp239 + (fTemp229 + fTemp231)))))))))))))))) - (fTemp216 + (fTemp217 + (fTemp218 + (fTemp219 + (fTemp220 + (fTemp237 + (fTemp238 + (fTemp248 + (fTemp246 + (fTemp235 + (fTemp245 + (fTemp243 + (fTemp241 + (fTemp240 + (fTemp230 + fTemp233)))))))))))))))));
			fRec258[0] = max((fRec258[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp315))))));
			fVbargraph58 = FAUSTFLOAT(fRec258[0]);
			output22[i] = FAUSTFLOAT(fTemp315);
			float fTemp316 = (fRec1[0] * ((fTemp314 + (fTemp218 + (fTemp219 + (fTemp254 + (fTemp255 + (fTemp256 + (fTemp257 + (fTemp258 + (fTemp263 + (fTemp247 + (fTemp262 + (fTemp272 + (fTemp261 + (fTemp244 + (fTemp259 + (fTemp270 + (fTemp239 + (fTemp228 + fTemp267)))))))))))))))))) - (fTemp250 + (fTemp251 + (fTemp252 + (fTemp253 + (fTemp221 + (fTemp227 + (fTemp264 + (fTemp265 + (fTemp243 + (fTemp260 + (fTemp271 + (fTemp241 + (fTemp269 + (fTemp266 + fTemp268))))))))))))))));
			fRec259[0] = max((fRec259[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp316))))));
			fVbargraph59 = FAUSTFLOAT(fRec259[0]);
			output23[i] = FAUSTFLOAT(fTemp316);
			float fTemp317 = (fRec1[0] * ((fTemp288 + (fTemp220 + (fTemp221 + (fTemp276 + (fTemp227 + (fTemp277 + (fTemp238 + (fTemp287 + (fTemp247 + (fTemp286 + (fTemp284 + (fTemp244 + (fTemp282 + (fTemp280 + (fTemp239 + (fTemp278 + (fTemp228 + fTemp230))))))))))))))))) - ((0.0210937504f * ((fTemp57 + fTemp60) - fTemp0)) + (fTemp274 + (fTemp275 + (fTemp218 + (fTemp219 + (fTemp222 + (fTemp289 + (fTemp290 + (fTemp226 + (fTemp285 + (fTemp243 + (fTemp283 + (fTemp281 + (fTemp241 + (fTemp279 + fTemp291)))))))))))))))));
			fRec260[0] = max((fRec260[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp317))))));
			fVbargraph60 = FAUSTFLOAT(fRec260[0]);
			output24[i] = FAUSTFLOAT(fTemp317);
			float fTemp318 = (fRec1[0] * ((fTemp300 + (fTemp218 + (fTemp219 + (fTemp253 + (fTemp295 + (fTemp296 + (fTemp299 + (fTemp247 + (fTemp310 + (fTemp244 + (fTemp308 + (fTemp306 + (fTemp305 + (fTemp269 + (fTemp239 + fTemp297))))))))))))))) - ((0.0210937504f * ((fTemp57 + fTemp79) - fTemp0)) + (fTemp293 + (fTemp294 + (fTemp221 + (fTemp301 + (fTemp302 + (fTemp303 + (fTemp227 + (fTemp312 + (fTemp311 + (fTemp309 + (fTemp243 + (fTemp307 + (fTemp241 + (fTemp240 + (fTemp298 + fTemp304))))))))))))))))));
			fRec261[0] = max((fRec261[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp318))))));
			fVbargraph61 = FAUSTFLOAT(fRec261[0]);
			output25[i] = FAUSTFLOAT(fTemp318);
			float fTemp319 = (0.0201733354f * fTemp0);
			float fTemp320 = (0.0105351806f * (fTemp60 + fTemp79));
			float fTemp321 = (0.0213084519f * (fTemp51 + fTemp136));
			float fTemp322 = (0.00627702521f * fTemp149);
			float fTemp323 = (0.035587389f * fTemp127);
			float fTemp324 = (0.0410013571f * fTemp130);
			float fTemp325 = (0.0169982649f * fTemp133);
			float fTemp326 = (0.0290784966f * fTemp90);
			float fTemp327 = (0.0304601658f * fTemp92);
			float fTemp328 = (0.00231317081f * fTemp95);
			float fTemp329 = (0.0122053269f * fTemp76);
			float fTemp330 = (0.0071028173f * fTemp82);
			float fTemp331 = (0.0351912305f * fTemp87);
			float fTemp332 = (0.0351912305f * fTemp15);
			float fTemp333 = (0.035587389f * fTemp30);
			float fTemp334 = (0.0142547125f * fTemp33);
			float fTemp335 = (0.0304601658f * fTemp42);
			float fTemp336 = (0.0263335984f * fTemp45);
			float fTemp337 = (0.0316055417f * fTemp57);
			float fTemp338 = (0.0328065082f * fTemp54);
			float fTemp339 = (fTemp337 + fTemp338);
			float fTemp340 = (fTemp336 + fTemp339);
			float fTemp341 = (1.4275392e-18f * fTemp152);
			float fTemp342 = (4.75846401e-19f * fTemp98);
			float fTemp343 = (0.000467861828f * fTemp84);
			float fTemp344 = (0.00443852693f * fTemp6);
			float fTemp345 = (1.14222021e-18f * fTemp48);
			float fTemp346 = (2.73352938e-18f * fTemp39);
			float fTemp347 = (0.00231317081f * fTemp36);
			float fTemp348 = (4.67617897e-18f * fTemp27);
			float fTemp349 = (0.00627702521f * fTemp24);
			float fTemp350 = (0.00147950905f * fTemp21);
			float fTemp351 = (0.000750248029f * fTemp18);
			float fTemp352 = (6.59352118e-18f * fTemp12);
			float fTemp353 = (0.0122053269f * fTemp9);
			float fTemp354 = (0.000467861828f * fTemp3);
			float fTemp355 = (fRec1[0] * ((fTemp319 + (fTemp320 + (fTemp321 + (fTemp322 + (fTemp323 + (fTemp324 + (fTemp325 + (fTemp326 + (fTemp327 + (fTemp328 + (fTemp329 + (fTemp330 + (fTemp331 + (fTemp332 + (fTemp333 + (fTemp334 + (fTemp335 + fTemp340))))))))))))))))) - (fTemp341 + (fTemp342 + (fTemp343 + ((fTemp344 + ((((((((fTemp345 + fTemp346) + fTemp347) + fTemp348) + fTemp349) + fTemp350) + fTemp351) + fTemp352) + fTemp353)) + fTemp354))))));
			fRec262[0] = max((fRec262[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp355))))));
			fVbargraph62 = FAUSTFLOAT(fRec262[0]);
			output26[i] = FAUSTFLOAT(fTemp355);
			float fTemp356 = (0.00627702521f * fTemp149);
			float fTemp357 = (1.63068845e-18f * fTemp152);
			float fTemp358 = (5.43562818e-19f * fTemp98);
			float fTemp359 = (0.0122053269f * fTemp76);
			float fTemp360 = (0.000467861828f * fTemp3);
			float fTemp361 = (0.00627702521f * fTemp24);
			float fTemp362 = (0.00231317081f * fTemp36);
			float fTemp363 = (0.0105351806f * (fTemp60 - fTemp79));
			float fTemp364 = (0.0213084519f * (fTemp51 - fTemp136));
			float fTemp365 = (0.000467861828f * fTemp84);
			float fTemp366 = (1.30476637e-18f * fTemp48);
			float fTemp367 = (3.1225305e-18f * fTemp39);
			float fTemp368 = (5.34163296e-18f * fTemp27);
			float fTemp369 = (7.53182689e-18f * fTemp12);
			float fTemp370 = (fRec1[0] * ((fTemp319 + (fTemp356 + (fTemp357 + (fTemp323 + (fTemp327 + (fTemp328 + (fTemp358 + (fTemp359 + (fTemp331 + (fTemp360 + (fTemp353 + (fTemp361 + (fTemp334 + (fTemp362 + fTemp340)))))))))))))) - (fTemp363 + (fTemp364 + (fTemp324 + (fTemp325 + (fTemp326 + (fTemp330 + (fTemp365 + (fTemp344 + ((fTemp332 + (fTemp351 + (fTemp350 + ((fTemp333 + ((fTemp335 + fTemp366) + fTemp367)) + fTemp368)))) + fTemp369)))))))))));
			fRec263[0] = max((fRec263[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp370))))));
			fVbargraph63 = FAUSTFLOAT(fRec263[0]);
			output27[i] = FAUSTFLOAT(fTemp370);
			float fTemp371 = (2.71781399e-18f * fTemp152);
			float fTemp372 = (9.0593803e-19f * fTemp98);
			float fTemp373 = (0.000467861828f * fTemp84);
			float fTemp374 = (0.000467861828f * fTemp3);
			float fTemp375 = (0.0122053269f * fTemp9);
			float fTemp376 = (1.25530448e-17f * fTemp12);
			float fTemp377 = (0.00627702521f * fTemp24);
			float fTemp378 = (8.90272159e-18f * fTemp27);
			float fTemp379 = (0.00231317081f * fTemp36);
			float fTemp380 = (5.20421758e-18f * fTemp39);
			float fTemp381 = (2.17461068e-18f * fTemp48);
			float fTemp382 = (0.00627702521f * fTemp149);
			float fTemp383 = (0.035587389f * fTemp127);
			float fTemp384 = (0.0213084519f * fTemp136);
			float fTemp385 = (0.0304601658f * fTemp92);
			float fTemp386 = (0.00231317081f * fTemp95);
			float fTemp387 = (0.0122053269f * fTemp76);
			float fTemp388 = (0.0105351806f * fTemp79);
			float fTemp389 = (0.0351912305f * fTemp87);
			float fTemp390 = (0.0105351806f * fTemp60);
			float fTemp391 = (0.0213084519f * fTemp51);
			float fTemp392 = (0.0304601658f * fTemp42);
			float fTemp393 = (0.035587389f * fTemp30);
			float fTemp394 = (0.0351912305f * fTemp15);
			float fTemp395 = (fRec1[0] * ((fTemp319 + (fTemp371 + (fTemp324 + (fTemp325 + (fTemp326 + (fTemp372 + (fTemp330 + (fTemp373 + (fTemp374 + (fTemp375 + (fTemp376 + (fTemp377 + (fTemp378 + (fTemp334 + (fTemp379 + (fTemp380 + (fTemp336 + (fTemp381 + fTemp339)))))))))))))))))) - (fTemp382 + (fTemp383 + (fTemp384 + (fTemp385 + (fTemp386 + (fTemp387 + (fTemp388 + (fTemp389 + (fTemp344 + ((fTemp351 + (fTemp350 + (((fTemp390 + fTemp391) + fTemp392) + fTemp393))) + fTemp394))))))))))));
			fRec264[0] = max((fRec264[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp395))))));
			fVbargraph64 = FAUSTFLOAT(fRec264[0]);
			output28[i] = FAUSTFLOAT(fTemp395);
			float fTemp396 = (3.80493941e-18f * fTemp152);
			float fTemp397 = (1.26831324e-18f * fTemp98);
			float fTemp398 = (0.000467861828f * fTemp84);
			float fTemp399 = (0.0351912305f * fTemp15);
			float fTemp400 = (0.035587389f * fTemp30);
			float fTemp401 = (0.0304601658f * fTemp42);
			float fTemp402 = (0.0213084519f * fTemp51);
			float fTemp403 = (0.0105351806f * fTemp60);
			float fTemp404 = (0.00627702521f * fTemp149);
			float fTemp405 = (0.0213084519f * fTemp136);
			float fTemp406 = (0.0304601658f * fTemp92);
			float fTemp407 = (0.00231317081f * fTemp95);
			float fTemp408 = (0.0122053269f * fTemp76);
			float fTemp409 = (0.0105351806f * fTemp79);
			float fTemp410 = (3.04445499e-18f * fTemp48);
			float fTemp411 = (7.28590444e-18f * fTemp39);
			float fTemp412 = (0.00231317081f * fTemp36);
			float fTemp413 = (1.24638111e-17f * fTemp27);
			float fTemp414 = (0.00627702521f * fTemp24);
			float fTemp415 = (1.75742628e-17f * fTemp12);
			float fTemp416 = (0.0122053269f * fTemp9);
			float fTemp417 = (0.000467861828f * fTemp3);
			float fTemp418 = (fRec1[0] * ((fTemp319 + (fTemp396 + (fTemp397 + (fTemp398 + (fTemp399 + (fTemp400 + (fTemp334 + (fTemp401 + (fTemp336 + (fTemp402 + (fTemp338 + (fTemp337 + fTemp403)))))))))))) - (fTemp404 + (fTemp323 + (fTemp324 + (fTemp325 + (fTemp405 + (fTemp326 + (fTemp406 + (fTemp407 + (fTemp408 + (fTemp409 + (fTemp330 + (fTemp331 + ((fTemp344 + (((fTemp351 + (fTemp350 + ((((fTemp410 + fTemp411) + fTemp412) + fTemp413) + fTemp414))) + fTemp415) + fTemp416)) + fTemp417)))))))))))))));
			fRec265[0] = max((fRec265[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp418))))));
			fVbargraph65 = FAUSTFLOAT(fRec265[0]);
			output29[i] = FAUSTFLOAT(fTemp418);
			float fTemp419 = (0.0272004437f * fTemp149);
			float fTemp420 = (0.0355082154f * fTemp152);
			float fTemp421 = (0.0169982649f * fTemp133);
			float fTemp422 = (0.0071028173f * fTemp136);
			float fTemp423 = (0.0300712213f * fTemp95);
			float fTemp424 = (0.0355082154f * fTemp98);
			float fTemp425 = (0.0105351806f * fTemp79);
			float fTemp426 = (0.0213084519f * fTemp82);
			float fTemp427 = (0.0369610861f * fTemp84);
			float fTemp428 = (0.0103565631f * fTemp6);
			float fTemp429 = (0.023090966f * fTemp18);
			float fTemp430 = (0.0103565631f * fTemp21);
			float fTemp431 = (0.0188310761f * fTemp24);
			float fTemp432 = (0.0042514056f * fTemp33);
			float fTemp433 = (0.0208185371f * fTemp36);
			float fTemp434 = (0.0226643533f * fTemp39);
			float fTemp435 = (0.0284112692f * fTemp48);
			float fTemp436 = (0.0213084519f * fTemp51);
			float fTemp437 = (0.0105351806f * fTemp57);
			float fTemp438 = (0.0316055417f * fTemp60);
			float fTemp439 = (fTemp437 + fTemp438);
			float fTemp440 = (0.010280801f * fTemp127);
			float fTemp441 = (0.0205006786f * fTemp130);
			float fTemp442 = (0.00671042223f * fTemp90);
			float fTemp443 = (0.00537532335f * fTemp92);
			float fTemp444 = (0.00453340728f * fTemp76);
			float fTemp445 = (0.000968566048f * fTemp87);
			float fTemp446 = (0.0164032541f * fTemp54);
			float fTemp447 = (0.020481687f * fTemp45);
			float fTemp448 = (fTemp446 + fTemp447);
			float fTemp449 = (0.0161259696f * fTemp42);
			float fTemp450 = (0.0308424029f * fTemp30);
			float fTemp451 = (0.00894722994f * fTemp27);
			float fTemp452 = (0.00290569803f * fTemp15);
			float fTemp453 = (0.0273342375f * fTemp12);
			float fTemp454 = (0.00313851261f * fTemp9);
			float fTemp455 = (0.00140358554f * fTemp3);
			float fTemp456 = (fRec1[0] * ((fTemp319 + (fTemp419 + (fTemp420 + (fTemp421 + (fTemp422 + (fTemp423 + (fTemp424 + (fTemp425 + (fTemp426 + (fTemp427 + (fTemp428 + (fTemp429 + (fTemp430 + (fTemp431 + (fTemp432 + (fTemp433 + (fTemp434 + (fTemp435 + (fTemp436 + fTemp439))))))))))))))))))) - (fTemp440 + (fTemp441 + (fTemp442 + (fTemp443 + (fTemp444 + (fTemp445 + (((((((fTemp448 + fTemp449) + fTemp450) + fTemp451) + fTemp452) + fTemp453) + fTemp454) + fTemp455)))))))));
			fRec266[0] = max((fRec266[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp456))))));
			fVbargraph66 = FAUSTFLOAT(fRec266[0]);
			output30[i] = FAUSTFLOAT(fTemp456);
			float fTemp457 = (0.0169982649f * fTemp133);
			float fTemp458 = (0.0213084519f * fTemp136);
			float fTemp459 = (0.00313851261f * fTemp76);
			float fTemp460 = (0.0316055417f * fTemp79);
			float fTemp461 = (0.0213084519f * fTemp82);
			float fTemp462 = (0.0369610861f * fTemp3);
			float fTemp463 = (0.0103565631f * fTemp6);
			float fTemp464 = (0.00453340728f * fTemp9);
			float fTemp465 = (0.0273342375f * fTemp12);
			float fTemp466 = (0.0103565631f * fTemp21);
			float fTemp467 = (0.00894722994f * fTemp27);
			float fTemp468 = (0.0071028173f * fTemp51);
			float fTemp469 = (0.0105351806f * fTemp60);
			float fTemp470 = (0.0188310761f * fTemp149);
			float fTemp471 = (0.0355082154f * fTemp152);
			float fTemp472 = (0.0308424029f * fTemp127);
			float fTemp473 = (0.0205006786f * fTemp130);
			float fTemp474 = (0.00671042223f * fTemp90);
			float fTemp475 = (0.0161259696f * fTemp92);
			float fTemp476 = (0.0208185371f * fTemp95);
			float fTemp477 = (0.0355082154f * fTemp98);
			float fTemp478 = (0.00140358554f * fTemp84);
			float fTemp479 = (0.00290569803f * fTemp87);
			float fTemp480 = (0.0284112692f * fTemp48);
			float fTemp481 = (0.00537532335f * fTemp42);
			float fTemp482 = (0.0226643533f * fTemp39);
			float fTemp483 = (0.0300712213f * fTemp36);
			float fTemp484 = (0.010280801f * fTemp30);
			float fTemp485 = (0.0272004437f * fTemp24);
			float fTemp486 = (0.000968566048f * fTemp15);
			float fTemp487 = (fRec1[0] * ((fTemp319 + (fTemp457 + (fTemp458 + (fTemp459 + (fTemp460 + (fTemp461 + (fTemp462 + (fTemp463 + (fTemp464 + (fTemp465 + (fTemp429 + (fTemp466 + (fTemp467 + (fTemp432 + (fTemp468 + (fTemp437 + fTemp469)))))))))))))))) - (fTemp470 + (fTemp471 + (fTemp472 + (fTemp473 + (fTemp474 + (fTemp475 + (fTemp476 + (fTemp477 + (fTemp478 + (fTemp479 + (((((((fTemp447 + (fTemp446 + fTemp480)) + fTemp481) + fTemp482) + fTemp483) + fTemp484) + fTemp485) + fTemp486)))))))))))));
			fRec267[0] = max((fRec267[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp487))))));
			fVbargraph67 = FAUSTFLOAT(fRec267[0]);
			output31[i] = FAUSTFLOAT(fTemp487);
			float fTemp488 = (0.0355082154f * fTemp152);
			float fTemp489 = (0.0205006786f * fTemp130);
			float fTemp490 = (0.0213084519f * fTemp136);
			float fTemp491 = (0.00671042223f * fTemp90);
			float fTemp492 = (0.00313851261f * fTemp76);
			float fTemp493 = (0.0103565631f * fTemp6);
			float fTemp494 = (0.0273342375f * fTemp12);
			float fTemp495 = (0.000968566048f * fTemp15);
			float fTemp496 = (0.0103565631f * fTemp21);
			float fTemp497 = (0.0272004437f * fTemp24);
			float fTemp498 = (0.00894722994f * fTemp27);
			float fTemp499 = (0.010280801f * fTemp30);
			float fTemp500 = (0.0300712213f * fTemp36);
			float fTemp501 = (0.00537532335f * fTemp42);
			float fTemp502 = (0.0188310761f * fTemp149);
			float fTemp503 = (0.0308424029f * fTemp127);
			float fTemp504 = (0.0169982649f * fTemp133);
			float fTemp505 = (0.0161259696f * fTemp92);
			float fTemp506 = (0.0208185371f * fTemp95);
			float fTemp507 = (0.0213084519f * fTemp82);
			float fTemp508 = (0.00140358554f * fTemp84);
			float fTemp509 = (0.00290569803f * fTemp87);
			float fTemp510 = (0.00453340728f * fTemp9);
			float fTemp511 = (0.0105351806f * fTemp60);
			float fTemp512 = (0.0071028173f * fTemp51);
			float fTemp513 = (0.0284112692f * fTemp48);
			float fTemp514 = (fRec1[0] * ((fTemp319 + (fTemp488 + (fTemp489 + (fTemp490 + (fTemp491 + (fTemp477 + (fTemp492 + (fTemp460 + (fTemp493 + (fTemp494 + (fTemp495 + (fTemp429 + (fTemp496 + (fTemp497 + (fTemp498 + (fTemp499 + (fTemp432 + (fTemp500 + (fTemp437 + fTemp501))))))))))))))))))) - (fTemp502 + (fTemp503 + (fTemp504 + (fTemp505 + (fTemp506 + (fTemp507 + (fTemp508 + (fTemp509 + (fTemp462 + (fTemp510 + (fTemp482 + (fTemp447 + (((fTemp446 + fTemp511) + fTemp512) + fTemp513)))))))))))))));
			fRec268[0] = max((fRec268[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp514))))));
			fVbargraph68 = FAUSTFLOAT(fRec268[0]);
			output32[i] = FAUSTFLOAT(fTemp514);
			float fTemp515 = (0.0272004437f * fTemp149);
			float fTemp516 = (0.0205006786f * fTemp130);
			float fTemp517 = (0.0071028173f * fTemp136);
			float fTemp518 = (0.00671042223f * fTemp90);
			float fTemp519 = (0.0300712213f * fTemp95);
			float fTemp520 = (0.0105351806f * fTemp79);
			float fTemp521 = (0.00140358554f * fTemp3);
			float fTemp522 = (0.0103565631f * fTemp6);
			float fTemp523 = (0.00313851261f * fTemp9);
			float fTemp524 = (0.0103565631f * fTemp21);
			float fTemp525 = (0.0226643533f * fTemp39);
			float fTemp526 = (0.0284112692f * fTemp48);
			float fTemp527 = (0.010280801f * fTemp127);
			float fTemp528 = (0.00537532335f * fTemp92);
			float fTemp529 = (0.00453340728f * fTemp76);
			float fTemp530 = (0.0213084519f * fTemp82);
			float fTemp531 = (0.000968566048f * fTemp87);
			float fTemp532 = (fTemp438 + fTemp446);
			float fTemp533 = (0.0208185371f * fTemp36);
			float fTemp534 = (0.0188310761f * fTemp24);
			float fTemp535 = (0.0273342375f * fTemp12);
			float fTemp536 = (fRec1[0] * ((fTemp319 + (fTemp515 + (fTemp516 + (fTemp517 + (fTemp518 + (fTemp519 + (fTemp520 + (fTemp427 + (fTemp521 + (fTemp522 + (fTemp523 + (fTemp452 + (fTemp429 + (fTemp524 + (fTemp450 + (fTemp432 + (fTemp525 + (fTemp449 + (fTemp437 + fTemp526))))))))))))))))))) - (fTemp420 + (fTemp527 + (fTemp421 + (fTemp528 + (fTemp424 + (fTemp529 + (fTemp530 + (fTemp531 + (((fTemp451 + ((fTemp447 + (fTemp436 + fTemp532)) + fTemp533)) + fTemp534) + fTemp535)))))))))));
			fRec269[0] = max((fRec269[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp536))))));
			fVbargraph69 = FAUSTFLOAT(fRec269[0]);
			output33[i] = FAUSTFLOAT(fTemp536);
			float fTemp537 = (0.010280801f * fTemp127);
			float fTemp538 = (0.0169982649f * fTemp133);
			float fTemp539 = (0.00537532335f * fTemp92);
			float fTemp540 = (0.0355082154f * fTemp98);
			float fTemp541 = (0.0213084519f * fTemp82);
			float fTemp542 = (0.000968566048f * fTemp87);
			float fTemp543 = (0.00140358554f * fTemp3);
			float fTemp544 = (0.0103565631f * fTemp6);
			float fTemp545 = (0.00313851261f * fTemp9);
			float fTemp546 = (0.00290569803f * fTemp15);
			float fTemp547 = (0.0103565631f * fTemp21);
			float fTemp548 = (0.0308424029f * fTemp30);
			float fTemp549 = (0.0226643533f * fTemp39);
			float fTemp550 = (0.0284112692f * fTemp48);
			float fTemp551 = (0.0205006786f * fTemp130);
			float fTemp552 = (0.0071028173f * fTemp136);
			float fTemp553 = (0.00671042223f * fTemp90);
			float fTemp554 = (0.0300712213f * fTemp95);
			float fTemp555 = (0.0105351806f * fTemp79);
			float fTemp556 = (0.0213084519f * fTemp51);
			float fTemp557 = (0.0208185371f * fTemp36);
			float fTemp558 = (0.00894722994f * fTemp27);
			float fTemp559 = (0.0188310761f * fTemp24);
			float fTemp560 = (0.0273342375f * fTemp12);
			float fTemp561 = (fRec1[0] * ((fTemp319 + (fTemp420 + (fTemp537 + (fTemp538 + (fTemp539 + (fTemp540 + (fTemp444 + (fTemp541 + (fTemp542 + (fTemp543 + (fTemp544 + (fTemp545 + (fTemp546 + (fTemp429 + (fTemp547 + (fTemp548 + (fTemp432 + (fTemp549 + (fTemp449 + (fTemp437 + fTemp550)))))))))))))))))))) - (fTemp419 + (fTemp551 + (fTemp552 + (fTemp553 + (fTemp554 + (fTemp555 + (fTemp427 + (((((fTemp447 + (fTemp532 + fTemp556)) + fTemp557) + fTemp558) + fTemp559) + fTemp560))))))))));
			fRec270[0] = max((fRec270[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp561))))));
			fVbargraph70 = FAUSTFLOAT(fRec270[0]);
			output34[i] = FAUSTFLOAT(fTemp561);
			float fTemp562 = (0.0188310761f * fTemp149);
			float fTemp563 = (0.0169982649f * fTemp133);
			float fTemp564 = (0.0208185371f * fTemp95);
			float fTemp565 = (0.0213084519f * fTemp82);
			float fTemp566 = (0.00140358554f * fTemp84);
			float fTemp567 = (0.0103565631f * fTemp6);
			float fTemp568 = (0.0273342375f * fTemp12);
			float fTemp569 = (0.000968566048f * fTemp15);
			float fTemp570 = (0.0103565631f * fTemp21);
			float fTemp571 = (0.0272004437f * fTemp24);
			float fTemp572 = (0.00894722994f * fTemp27);
			float fTemp573 = (0.010280801f * fTemp30);
			float fTemp574 = (0.0300712213f * fTemp36);
			float fTemp575 = (0.00537532335f * fTemp42);
			float fTemp576 = (0.0355082154f * fTemp152);
			float fTemp577 = (0.0205006786f * fTemp130);
			float fTemp578 = (0.00671042223f * fTemp90);
			float fTemp579 = (0.00313851261f * fTemp76);
			float fTemp580 = (0.0105351806f * fTemp60);
			float fTemp581 = (0.0071028173f * fTemp51);
			float fTemp582 = (0.0284112692f * fTemp48);
			float fTemp583 = (0.00453340728f * fTemp9);
			float fTemp584 = (fRec1[0] * ((fTemp319 + (fTemp562 + (fTemp472 + (fTemp563 + (fTemp475 + (fTemp564 + (fTemp565 + (fTemp566 + (fTemp479 + (fTemp567 + (fTemp568 + (fTemp569 + (fTemp429 + (fTemp570 + (fTemp571 + (fTemp572 + (fTemp573 + (fTemp432 + (fTemp574 + (fTemp437 + fTemp575)))))))))))))))))))) - (fTemp576 + (fTemp577 + (fTemp458 + (fTemp578 + (fTemp540 + (fTemp579 + (fTemp460 + (fTemp462 + ((fTemp549 + (fTemp447 + (((fTemp446 + fTemp580) + fTemp581) + fTemp582))) + fTemp583)))))))))));
			fRec271[0] = max((fRec271[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp584))))));
			fVbargraph71 = FAUSTFLOAT(fRec271[0]);
			output35[i] = FAUSTFLOAT(fTemp584);
			float fTemp585 = (0.0188310761f * fTemp149);
			float fTemp586 = (0.0355082154f * fTemp152);
			float fTemp587 = (0.0205006786f * fTemp130);
			float fTemp588 = (0.00671042223f * fTemp90);
			float fTemp589 = (0.0208185371f * fTemp95);
			float fTemp590 = (0.0355082154f * fTemp98);
			float fTemp591 = (0.00140358554f * fTemp84);
			float fTemp592 = (0.0103565631f * fTemp6);
			float fTemp593 = (0.00453340728f * fTemp9);
			float fTemp594 = (0.0273342375f * fTemp12);
			float fTemp595 = (0.0103565631f * fTemp21);
			float fTemp596 = (0.00894722994f * fTemp27);
			float fTemp597 = (0.0071028173f * fTemp51);
			float fTemp598 = (0.0105351806f * fTemp60);
			float fTemp599 = (0.0169982649f * fTemp133);
			float fTemp600 = (0.00313851261f * fTemp76);
			float fTemp601 = (0.0213084519f * fTemp82);
			float fTemp602 = (0.0284112692f * fTemp48);
			float fTemp603 = (0.00537532335f * fTemp42);
			float fTemp604 = (0.0226643533f * fTemp39);
			float fTemp605 = (0.0300712213f * fTemp36);
			float fTemp606 = (0.010280801f * fTemp30);
			float fTemp607 = (0.0272004437f * fTemp24);
			float fTemp608 = (0.000968566048f * fTemp15);
			float fTemp609 = (fRec1[0] * ((fTemp319 + (fTemp585 + (fTemp586 + (fTemp503 + (fTemp587 + (fTemp588 + (fTemp505 + (fTemp589 + (fTemp590 + (fTemp591 + (fTemp509 + (fTemp462 + (fTemp592 + (fTemp593 + (fTemp594 + (fTemp429 + (fTemp595 + (fTemp596 + (fTemp432 + (fTemp597 + (fTemp437 + fTemp598))))))))))))))))))))) - (fTemp599 + (fTemp490 + (fTemp600 + (fTemp460 + (fTemp601 + (((((((fTemp447 + (fTemp446 + fTemp602)) + fTemp603) + fTemp604) + fTemp605) + fTemp606) + fTemp607) + fTemp608))))))));
			fRec272[0] = max((fRec272[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp609))))));
			fVbargraph72 = FAUSTFLOAT(fRec272[0]);
			output36[i] = FAUSTFLOAT(fTemp609);
			float fTemp610 = (0.010280801f * fTemp127);
			float fTemp611 = (0.0205006786f * fTemp130);
			float fTemp612 = (0.00671042223f * fTemp90);
			float fTemp613 = (0.00537532335f * fTemp92);
			float fTemp614 = (0.00453340728f * fTemp76);
			float fTemp615 = (0.000968566048f * fTemp87);
			float fTemp616 = (0.0103565631f * fTemp6);
			float fTemp617 = (0.0103565631f * fTemp21);
			float fTemp618 = (0.0188310761f * fTemp24);
			float fTemp619 = (0.0208185371f * fTemp36);
			float fTemp620 = (0.0226643533f * fTemp39);
			float fTemp621 = (0.0284112692f * fTemp48);
			float fTemp622 = (0.0213084519f * fTemp51);
			float fTemp623 = (0.0316055417f * fTemp60);
			float fTemp624 = (0.0272004437f * fTemp149);
			float fTemp625 = (0.0355082154f * fTemp152);
			float fTemp626 = (0.0169982649f * fTemp133);
			float fTemp627 = (0.0071028173f * fTemp136);
			float fTemp628 = (0.0300712213f * fTemp95);
			float fTemp629 = (0.0355082154f * fTemp98);
			float fTemp630 = (0.0105351806f * fTemp79);
			float fTemp631 = (0.0213084519f * fTemp82);
			float fTemp632 = (0.0369610861f * fTemp84);
			float fTemp633 = (0.0161259696f * fTemp42);
			float fTemp634 = (0.0308424029f * fTemp30);
			float fTemp635 = (0.00894722994f * fTemp27);
			float fTemp636 = (0.00290569803f * fTemp15);
			float fTemp637 = (0.0273342375f * fTemp12);
			float fTemp638 = (0.00313851261f * fTemp9);
			float fTemp639 = (0.00140358554f * fTemp3);
			float fTemp640 = (fRec1[0] * ((fTemp319 + (fTemp610 + (fTemp611 + (fTemp612 + (fTemp613 + (fTemp614 + (fTemp615 + (fTemp616 + (fTemp429 + (fTemp617 + (fTemp618 + (fTemp432 + (fTemp619 + (fTemp620 + (fTemp621 + (fTemp622 + (fTemp437 + fTemp623))))))))))))))))) - (fTemp624 + (fTemp625 + (fTemp626 + (fTemp627 + (fTemp628 + (fTemp629 + (fTemp630 + (fTemp631 + (fTemp632 + (((((((fTemp448 + fTemp633) + fTemp634) + fTemp635) + fTemp636) + fTemp637) + fTemp638) + fTemp639))))))))))));
			fRec273[0] = max((fRec273[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp640))))));
			fVbargraph73 = FAUSTFLOAT(fRec273[0]);
			output37[i] = FAUSTFLOAT(fTemp640);
			float fTemp641 = (fTemp437 + fTemp446);
			float fTemp642 = (fRec1[0] * ((fTemp319 + (fTemp440 + (fTemp441 + (fTemp423 + (fTemp424 + (fTemp425 + (fTemp426 + (fTemp427 + (fTemp453 + (fTemp430 + (fTemp450 + (fTemp432 + (fTemp433 + (fTemp447 + (fTemp438 + fTemp435))))))))))))))) - (fTemp419 + (fTemp420 + (fTemp421 + (fTemp422 + (fTemp442 + (fTemp443 + (fTemp444 + (fTemp445 + (fTemp455 + (fTemp428 + (fTemp454 + (fTemp452 + (fTemp429 + (fTemp431 + (fTemp451 + (fTemp434 + (fTemp449 + (fTemp436 + fTemp641))))))))))))))))))));
			fRec274[0] = max((fRec274[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp642))))));
			fVbargraph74 = FAUSTFLOAT(fRec274[0]);
			output38[i] = FAUSTFLOAT(fTemp642);
			float fTemp643 = (fRec1[0] * ((fTemp319 + (fTemp470 + (fTemp471 + (fTemp472 + (fTemp473 + (fTemp459 + (fTemp460 + (fTemp461 + (fTemp462 + (fTemp464 + (fTemp466 + (fTemp485 + (fTemp467 + (fTemp484 + (fTemp432 + (fTemp482 + (fTemp447 + fTemp469))))))))))))))))) - (fTemp457 + (fTemp458 + (fTemp474 + (fTemp475 + (fTemp476 + (fTemp477 + (fTemp478 + (fTemp479 + (fTemp463 + (fTemp465 + (fTemp486 + (fTemp429 + (fTemp483 + (fTemp481 + (fTemp480 + (fTemp468 + fTemp641))))))))))))))))));
			fRec275[0] = max((fRec275[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp643))))));
			fVbargraph75 = FAUSTFLOAT(fRec275[0]);
			output39[i] = FAUSTFLOAT(fTemp643);
			float fTemp644 = (fRec1[0] * ((fTemp319 + (fTemp502 + (fTemp503 + (fTemp504 + (fTemp491 + (fTemp477 + (fTemp492 + (fTemp460 + (fTemp495 + (fTemp496 + (fTemp498 + (fTemp432 + (fTemp500 + (fTemp482 + (fTemp501 + (fTemp447 + fTemp512)))))))))))))))) - (fTemp488 + (fTemp489 + (fTemp490 + (fTemp505 + (fTemp506 + (fTemp507 + (fTemp508 + (fTemp509 + (fTemp462 + (fTemp493 + (fTemp510 + (fTemp494 + (fTemp429 + (fTemp497 + (fTemp499 + (fTemp513 + (fTemp446 + (fTemp437 + fTemp511))))))))))))))))))));
			fRec276[0] = max((fRec276[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp644))))));
			fVbargraph76 = FAUSTFLOAT(fRec276[0]);
			output40[i] = FAUSTFLOAT(fTemp644);
			float fTemp645 = (fTemp446 + fTemp439);
			float fTemp646 = (fRec1[0] * ((fTemp319 + (fTemp420 + (fTemp527 + (fTemp421 + (fTemp518 + (fTemp519 + (fTemp520 + (fTemp427 + (fTemp521 + (fTemp523 + (fTemp535 + (fTemp452 + (fTemp524 + (fTemp534 + (fTemp432 + (fTemp449 + (fTemp447 + (fTemp436 + fTemp526)))))))))))))))))) - (fTemp515 + (fTemp516 + (fTemp517 + (fTemp528 + (fTemp424 + (fTemp529 + (fTemp530 + (fTemp531 + (fTemp522 + (fTemp429 + (fTemp451 + (fTemp450 + (fTemp533 + (fTemp525 + fTemp645))))))))))))))));
			fRec277[0] = max((fRec277[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp646))))));
			fVbargraph77 = FAUSTFLOAT(fRec277[0]);
			output41[i] = FAUSTFLOAT(fTemp646);
			float fTemp647 = (fRec1[0] * ((fTemp319 + (fTemp419 + (fTemp551 + (fTemp552 + (fTemp539 + (fTemp540 + (fTemp444 + (fTemp541 + (fTemp542 + (fTemp543 + (fTemp545 + (fTemp560 + (fTemp546 + (fTemp547 + (fTemp559 + (fTemp432 + (fTemp449 + (fTemp447 + (fTemp550 + fTemp556))))))))))))))))))) - (fTemp420 + (fTemp537 + (fTemp538 + (fTemp553 + (fTemp554 + (fTemp555 + (fTemp427 + (fTemp544 + (fTemp429 + (fTemp558 + (fTemp548 + (fTemp557 + (fTemp549 + fTemp645)))))))))))))));
			fRec278[0] = max((fRec278[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp647))))));
			fVbargraph78 = FAUSTFLOAT(fRec278[0]);
			output42[i] = FAUSTFLOAT(fTemp647);
			float fTemp648 = (fRec1[0] * ((fTemp319 + (fTemp576 + (fTemp577 + (fTemp458 + (fTemp475 + (fTemp564 + (fTemp565 + (fTemp566 + (fTemp479 + (fTemp569 + (fTemp570 + (fTemp572 + (fTemp432 + (fTemp574 + (fTemp549 + (fTemp575 + (fTemp447 + fTemp581))))))))))))))))) - (fTemp562 + (fTemp472 + (fTemp563 + (fTemp578 + (fTemp540 + (fTemp579 + (fTemp460 + (fTemp462 + (fTemp567 + (fTemp583 + (fTemp568 + (fTemp429 + (fTemp571 + (fTemp573 + (fTemp582 + (fTemp446 + (fTemp437 + fTemp580)))))))))))))))))));
			fRec279[0] = max((fRec279[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp648))))));
			fVbargraph79 = FAUSTFLOAT(fRec279[0]);
			output43[i] = FAUSTFLOAT(fTemp648);
			float fTemp649 = (fRec1[0] * ((fTemp319 + (fTemp599 + (fTemp490 + (fTemp588 + (fTemp505 + (fTemp589 + (fTemp590 + (fTemp591 + (fTemp509 + (fTemp462 + (fTemp593 + (fTemp595 + (fTemp607 + (fTemp596 + (fTemp606 + (fTemp432 + (fTemp604 + (fTemp447 + fTemp598)))))))))))))))))) - (fTemp585 + (fTemp586 + (fTemp503 + (fTemp587 + (fTemp600 + (fTemp460 + (fTemp601 + (fTemp592 + (fTemp594 + (fTemp608 + (fTemp429 + (fTemp605 + (fTemp603 + (fTemp602 + (fTemp597 + fTemp641)))))))))))))))));
			fRec280[0] = max((fRec280[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp649))))));
			fVbargraph80 = FAUSTFLOAT(fRec280[0]);
			output44[i] = FAUSTFLOAT(fTemp649);
			float fTemp650 = (fRec1[0] * ((fTemp319 + (fTemp624 + (fTemp625 + (fTemp626 + (fTemp627 + (fTemp612 + (fTemp613 + (fTemp614 + (fTemp615 + (fTemp637 + (fTemp617 + (fTemp634 + (fTemp432 + (fTemp619 + (fTemp447 + (fTemp623 + fTemp621)))))))))))))))) - (fTemp610 + (fTemp611 + (fTemp628 + (fTemp629 + (fTemp630 + (fTemp631 + (fTemp632 + (fTemp639 + (fTemp616 + (fTemp638 + (fTemp636 + (fTemp429 + (fTemp618 + (fTemp635 + (fTemp620 + (fTemp633 + (fTemp622 + fTemp641)))))))))))))))))));
			fRec281[0] = max((fRec281[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp650))))));
			fVbargraph81 = FAUSTFLOAT(fRec281[0]);
			output45[i] = FAUSTFLOAT(fTemp650);
			float fTemp651 = (fRec1[0] * ((fTemp319 + (fTemp320 + (fTemp341 + (fTemp326 + (fTemp327 + (fTemp328 + (fTemp329 + (fTemp330 + (fTemp331 + (fTemp344 + (fTemp352 + (fTemp332 + (fTemp351 + (fTemp349 + (fTemp334 + (fTemp346 + (fTemp338 + fTemp335))))))))))))))))) - (fTemp321 + (fTemp322 + (fTemp323 + (fTemp324 + (fTemp325 + (fTemp342 + (fTemp343 + (fTemp354 + (fTemp353 + (fTemp350 + (fTemp348 + (fTemp333 + (fTemp347 + (fTemp336 + (fTemp337 + fTemp345)))))))))))))))));
			fRec282[0] = max((fRec282[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp651))))));
			fVbargraph82 = FAUSTFLOAT(fRec282[0]);
			output46[i] = FAUSTFLOAT(fTemp651);
			float fTemp652 = (fRec1[0] * ((fTemp319 + (fTemp364 + (fTemp324 + (fTemp325 + (fTemp327 + (fTemp328 + (fTemp358 + (fTemp359 + (fTemp331 + (fTemp360 + (fTemp344 + (fTemp353 + (fTemp369 + (fTemp351 + (fTemp333 + (fTemp334 + (fTemp362 + (fTemp338 + fTemp367)))))))))))))))))) - (fTemp363 + (fTemp356 + (fTemp357 + (fTemp323 + (fTemp326 + (fTemp330 + (fTemp365 + (fTemp332 + (fTemp350 + (fTemp361 + (fTemp368 + (fTemp335 + (fTemp336 + (fTemp337 + fTemp366))))))))))))))));
			fRec283[0] = max((fRec283[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp652))))));
			fVbargraph83 = FAUSTFLOAT(fRec283[0]);
			output47[i] = FAUSTFLOAT(fTemp652);
			float fTemp653 = (fRec1[0] * ((fTemp319 + (fTemp382 + (fTemp383 + (fTemp384 + (fTemp326 + (fTemp372 + (fTemp330 + (fTemp373 + (fTemp374 + (fTemp344 + (fTemp375 + (fTemp351 + (fTemp378 + (fTemp393 + (fTemp334 + (fTemp379 + (fTemp381 + (fTemp338 + fTemp391)))))))))))))))))) - (fTemp371 + (fTemp324 + (fTemp325 + (fTemp385 + (fTemp386 + (fTemp387 + (fTemp388 + (fTemp389 + (fTemp376 + (fTemp394 + (fTemp350 + (fTemp377 + (fTemp380 + (fTemp392 + (fTemp336 + (fTemp337 + fTemp390))))))))))))))))));
			fRec284[0] = max((fRec284[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp653))))));
			fVbargraph84 = FAUSTFLOAT(fRec284[0]);
			output48[i] = FAUSTFLOAT(fTemp653);
			float fTemp654 = (fRec1[0] * ((fTemp319 + (fTemp404 + (fTemp323 + (fTemp324 + (fTemp325 + (fTemp405 + (fTemp397 + (fTemp398 + (fTemp344 + (fTemp415 + (fTemp399 + (fTemp351 + (fTemp414 + (fTemp334 + (fTemp411 + (fTemp401 + (fTemp338 + fTemp403))))))))))))))))) - (fTemp396 + (fTemp326 + (fTemp406 + (fTemp407 + (fTemp408 + (fTemp409 + (fTemp330 + (fTemp331 + (fTemp417 + (fTemp416 + (fTemp350 + (fTemp413 + (fTemp400 + (fTemp412 + (fTemp336 + (fTemp410 + (fTemp337 + fTemp402)))))))))))))))))));
			fRec285[0] = max((fRec285[1] - fConst1), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp654))))));
			fVbargraph85 = FAUSTFLOAT(fRec285[0]);
			output49[i] = FAUSTFLOAT(fTemp654);
			fRec1[1] = fRec1[0];
			fRec3[1] = fRec3[0];
			fRec2[1] = fRec2[0];
			fRec7[1] = fRec7[0];
			fRec8[1] = fRec8[0];
			fRec11[1] = fRec11[0];
			fRec10[1] = fRec10[0];
			fRec9[1] = fRec9[0];
			fRec6[1] = fRec6[0];
			fRec12[1] = fRec12[0];
			fRec5[1] = fRec5[0];
			fRec4[1] = fRec4[0];
			fRec16[1] = fRec16[0];
			fRec17[1] = fRec17[0];
			fRec20[1] = fRec20[0];
			fRec19[1] = fRec19[0];
			fRec18[1] = fRec18[0];
			fRec15[1] = fRec15[0];
			fRec21[1] = fRec21[0];
			fRec14[1] = fRec14[0];
			fRec13[1] = fRec13[0];
			fRec25[1] = fRec25[0];
			fRec26[1] = fRec26[0];
			fRec29[1] = fRec29[0];
			fRec28[1] = fRec28[0];
			fRec27[1] = fRec27[0];
			fRec24[1] = fRec24[0];
			fRec30[1] = fRec30[0];
			fRec23[1] = fRec23[0];
			fRec22[1] = fRec22[0];
			fRec34[1] = fRec34[0];
			fRec35[1] = fRec35[0];
			fRec38[1] = fRec38[0];
			fRec37[1] = fRec37[0];
			fRec36[1] = fRec36[0];
			fRec33[1] = fRec33[0];
			fRec39[1] = fRec39[0];
			fRec32[1] = fRec32[0];
			fRec31[1] = fRec31[0];
			fRec43[1] = fRec43[0];
			fRec44[1] = fRec44[0];
			fRec47[1] = fRec47[0];
			fRec46[1] = fRec46[0];
			fRec45[1] = fRec45[0];
			fRec42[1] = fRec42[0];
			fRec48[1] = fRec48[0];
			fRec41[1] = fRec41[0];
			fRec40[1] = fRec40[0];
			fRec54[1] = fRec54[0];
			fRec53[1] = fRec53[0];
			fRec52[1] = fRec52[0];
			fRec55[1] = fRec55[0];
			fRec56[1] = fRec56[0];
			fRec51[1] = fRec51[0];
			fRec57[1] = fRec57[0];
			fRec50[1] = fRec50[0];
			fRec49[1] = fRec49[0];
			fRec60[1] = fRec60[0];
			fRec61[1] = fRec61[0];
			fRec63[1] = fRec63[0];
			fRec64[1] = fRec64[0];
			fRec62[1] = fRec62[0];
			fRec59[1] = fRec59[0];
			fRec58[1] = fRec58[0];
			fRec67[1] = fRec67[0];
			fRec68[1] = fRec68[0];
			fRec70[1] = fRec70[0];
			fRec71[1] = fRec71[0];
			fRec69[1] = fRec69[0];
			fRec66[1] = fRec66[0];
			fRec65[1] = fRec65[0];
			fRec74[1] = fRec74[0];
			fRec75[1] = fRec75[0];
			fRec77[1] = fRec77[0];
			fRec78[1] = fRec78[0];
			fRec76[1] = fRec76[0];
			fRec73[1] = fRec73[0];
			fRec72[1] = fRec72[0];
			fRec81[1] = fRec81[0];
			fRec82[1] = fRec82[0];
			fRec84[1] = fRec84[0];
			fRec85[1] = fRec85[0];
			fRec83[1] = fRec83[0];
			fRec80[1] = fRec80[0];
			fRec79[1] = fRec79[0];
			fRec89[1] = fRec89[0];
			fRec90[1] = fRec90[0];
			fRec88[1] = fRec88[0];
			fRec91[1] = fRec91[0];
			fRec92[1] = fRec92[0];
			fRec87[1] = fRec87[0];
			fRec86[1] = fRec86[0];
			fRec96[1] = fRec96[0];
			fRec97[1] = fRec97[0];
			fRec95[1] = fRec95[0];
			fRec98[1] = fRec98[0];
			fRec94[1] = fRec94[0];
			fRec93[1] = fRec93[0];
			fRec102[1] = fRec102[0];
			fRec103[1] = fRec103[0];
			fRec101[1] = fRec101[0];
			fRec104[1] = fRec104[0];
			fRec100[1] = fRec100[0];
			fRec99[1] = fRec99[0];
			fRec108[1] = fRec108[0];
			fRec109[1] = fRec109[0];
			fRec107[1] = fRec107[0];
			fRec110[1] = fRec110[0];
			fRec106[1] = fRec106[0];
			fRec105[1] = fRec105[0];
			fRec114[1] = fRec114[0];
			fRec115[1] = fRec115[0];
			fRec113[1] = fRec113[0];
			fRec116[1] = fRec116[0];
			fRec112[1] = fRec112[0];
			fRec111[1] = fRec111[0];
			fRec119[1] = fRec119[0];
			fRec120[1] = fRec120[0];
			fRec118[1] = fRec118[0];
			fRec117[1] = fRec117[0];
			fRec123[1] = fRec123[0];
			fRec124[1] = fRec124[0];
			fRec122[1] = fRec122[0];
			fRec121[1] = fRec121[0];
			fRec127[1] = fRec127[0];
			fRec128[1] = fRec128[0];
			fRec126[1] = fRec126[0];
			fRec125[1] = fRec125[0];
			fRec131[1] = fRec131[0];
			fRec130[1] = fRec130[0];
			fRec129[1] = fRec129[0];
			fRec134[1] = fRec134[0];
			fRec133[1] = fRec133[0];
			fRec132[1] = fRec132[0];
			fRec0[1] = fRec0[0];
			fRec135[1] = fRec135[0];
			fRec140[1] = fRec140[0];
			fRec141[1] = fRec141[0];
			fRec144[1] = fRec144[0];
			fRec143[1] = fRec143[0];
			fRec142[1] = fRec142[0];
			fRec139[1] = fRec139[0];
			fRec145[1] = fRec145[0];
			fRec138[1] = fRec138[0];
			fRec137[1] = fRec137[0];
			fRec148[1] = fRec148[0];
			fRec147[1] = fRec147[0];
			fRec146[1] = fRec146[0];
			fRec151[1] = fRec151[0];
			fRec152[1] = fRec152[0];
			fRec150[1] = fRec150[0];
			fRec149[1] = fRec149[0];
			fRec156[1] = fRec156[0];
			fRec157[1] = fRec157[0];
			fRec160[1] = fRec160[0];
			fRec159[1] = fRec159[0];
			fRec158[1] = fRec158[0];
			fRec155[1] = fRec155[0];
			fRec161[1] = fRec161[0];
			fRec154[1] = fRec154[0];
			fRec153[1] = fRec153[0];
			fRec165[1] = fRec165[0];
			fRec166[1] = fRec166[0];
			fRec169[1] = fRec169[0];
			fRec168[1] = fRec168[0];
			fRec167[1] = fRec167[0];
			fRec164[1] = fRec164[0];
			fRec170[1] = fRec170[0];
			fRec163[1] = fRec163[0];
			fRec162[1] = fRec162[0];
			fRec173[1] = fRec173[0];
			fRec174[1] = fRec174[0];
			fRec176[1] = fRec176[0];
			fRec177[1] = fRec177[0];
			fRec175[1] = fRec175[0];
			fRec172[1] = fRec172[0];
			fRec171[1] = fRec171[0];
			fRec181[1] = fRec181[0];
			fRec182[1] = fRec182[0];
			fRec180[1] = fRec180[0];
			fRec183[1] = fRec183[0];
			fRec179[1] = fRec179[0];
			fRec178[1] = fRec178[0];
			fRec187[1] = fRec187[0];
			fRec188[1] = fRec188[0];
			fRec186[1] = fRec186[0];
			fRec189[1] = fRec189[0];
			fRec185[1] = fRec185[0];
			fRec184[1] = fRec184[0];
			fRec192[1] = fRec192[0];
			fRec193[1] = fRec193[0];
			fRec195[1] = fRec195[0];
			fRec196[1] = fRec196[0];
			fRec194[1] = fRec194[0];
			fRec191[1] = fRec191[0];
			fRec190[1] = fRec190[0];
			fRec136[1] = fRec136[0];
			fRec197[1] = fRec197[0];
			fRec198[1] = fRec198[0];
			fRec199[1] = fRec199[0];
			fRec200[1] = fRec200[0];
			fRec204[1] = fRec204[0];
			fRec205[1] = fRec205[0];
			fRec207[1] = fRec207[0];
			fRec208[1] = fRec208[0];
			fRec206[1] = fRec206[0];
			fRec203[1] = fRec203[0];
			fRec202[1] = fRec202[0];
			fRec212[1] = fRec212[0];
			fRec213[1] = fRec213[0];
			fRec216[1] = fRec216[0];
			fRec215[1] = fRec215[0];
			fRec214[1] = fRec214[0];
			fRec211[1] = fRec211[0];
			fRec217[1] = fRec217[0];
			fRec210[1] = fRec210[0];
			fRec209[1] = fRec209[0];
			fRec221[1] = fRec221[0];
			fRec222[1] = fRec222[0];
			fRec220[1] = fRec220[0];
			fRec223[1] = fRec223[0];
			fRec219[1] = fRec219[0];
			fRec218[1] = fRec218[0];
			fRec226[1] = fRec226[0];
			fRec227[1] = fRec227[0];
			fRec225[1] = fRec225[0];
			fRec224[1] = fRec224[0];
			fRec230[1] = fRec230[0];
			fRec231[1] = fRec231[0];
			fRec233[1] = fRec233[0];
			fRec234[1] = fRec234[0];
			fRec232[1] = fRec232[0];
			fRec229[1] = fRec229[0];
			fRec228[1] = fRec228[0];
			fRec238[1] = fRec238[0];
			fRec239[1] = fRec239[0];
			fRec242[1] = fRec242[0];
			fRec241[1] = fRec241[0];
			fRec240[1] = fRec240[0];
			fRec237[1] = fRec237[0];
			fRec243[1] = fRec243[0];
			fRec236[1] = fRec236[0];
			fRec235[1] = fRec235[0];
			fRec201[1] = fRec201[0];
			fRec244[1] = fRec244[0];
			fRec245[1] = fRec245[0];
			fRec246[1] = fRec246[0];
			fRec247[1] = fRec247[0];
			fRec248[1] = fRec248[0];
			fRec249[1] = fRec249[0];
			fRec250[1] = fRec250[0];
			fRec251[1] = fRec251[0];
			fRec252[1] = fRec252[0];
			fRec253[1] = fRec253[0];
			fRec254[1] = fRec254[0];
			fRec255[1] = fRec255[0];
			fRec256[1] = fRec256[0];
			fRec257[1] = fRec257[0];
			fRec258[1] = fRec258[0];
			fRec259[1] = fRec259[0];
			fRec260[1] = fRec260[0];
			fRec261[1] = fRec261[0];
			fRec262[1] = fRec262[0];
			fRec263[1] = fRec263[0];
			fRec264[1] = fRec264[0];
			fRec265[1] = fRec265[0];
			fRec266[1] = fRec266[0];
			fRec267[1] = fRec267[0];
			fRec268[1] = fRec268[0];
			fRec269[1] = fRec269[0];
			fRec270[1] = fRec270[0];
			fRec271[1] = fRec271[0];
			fRec272[1] = fRec272[0];
			fRec273[1] = fRec273[0];
			fRec274[1] = fRec274[0];
			fRec275[1] = fRec275[0];
			fRec276[1] = fRec276[0];
			fRec277[1] = fRec277[0];
			fRec278[1] = fRec278[0];
			fRec279[1] = fRec279[0];
			fRec280[1] = fRec280[0];
			fRec281[1] = fRec281[0];
			fRec282[1] = fRec282[0];
			fRec283[1] = fRec283[0];
			fRec284[1] = fRec284[0];
			fRec285[1] = fRec285[0];
			
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
