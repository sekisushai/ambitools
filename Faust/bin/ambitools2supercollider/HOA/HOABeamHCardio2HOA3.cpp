/* ------------------------------------------------------------
author: "Pierre Lecomte"
copyright: "(c) Pierre Lecomte 2016"
license: "GPL"
name: "HOABeamHCardio2HOA3"
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


float faustpower2_f(float value) {
	return (value * value);
	
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
#endif

class mydsp : public dsp {
	
 private:
	
	int fSamplingFreq;
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fHslider2;
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("author", "Pierre Lecomte");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.0");
		m->declare("copyright", "(c) Pierre Lecomte 2016");
		m->declare("lib/cijk.lib/author", "Pierre Lecomte");
		m->declare("lib/cijk.lib/copyright", "(c) Pierre Lecomte 2016");
		m->declare("lib/cijk.lib/license", "GPL");
		m->declare("lib/cijk.lib/name", "Cijk matrix");
		m->declare("lib/cijk.lib/version", "10");
		m->declare("lib/gui.lib/author", "Pierre Lecomte");
		m->declare("lib/gui.lib/copyright", "(c) Pierre Lecomte 2016");
		m->declare("lib/gui.lib/license", "GPL");
		m->declare("lib/gui.lib/name", "GUI Library");
		m->declare("lib/gui.lib/version", "1.0");
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
		m->declare("name", "HOABeamHCardio2HOA3");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
		m->declare("version", "1.0");
	}

	virtual int getNumInputs() {
		return 16;
		
	}
	virtual int getNumOutputs() {
		return 36;
		
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
		
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.0f);
		fHslider1 = FAUSTFLOAT(0.0f);
		fHslider2 = FAUSTFLOAT(0.0f);
		
	}
	
	virtual void instanceClear() {
		
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
		ui_interface->declare(0, "3", "");
		ui_interface->openHorizontalBox("Parameters");
		ui_interface->declare(&fHslider2, "style", "knob");
		ui_interface->addHorizontalSlider("Azimuth", &fHslider2, 0.0f, -3.14159274f, 3.14159274f, 0.100000001f);
		ui_interface->declare(&fHslider1, "style", "knob");
		ui_interface->addHorizontalSlider("Elevation", &fHslider1, 0.0f, -1.57079637f, 1.57079637f, 0.100000001f);
		ui_interface->declare(&fHslider0, "style", "knob");
		ui_interface->addHorizontalSlider("Order", &fHslider0, 0.0f, 0.0f, 2.0f, 1.0f);
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
		int iSlow0 = int(float(fHslider0));
		int iSlow1 = (iSlow0 >= 2);
		int iSlow2 = (iSlow0 >= 1);
		float fSlow3 = float(fHslider1);
		float fSlow4 = sinf(fSlow3);
		float fSlow5 = cosf(fSlow3);
		float fSlow6 = float(fHslider2);
		float fSlow7 = sinf(fSlow6);
		float fSlow8 = cosf(fSlow6);
		float fSlow9 = ((3.0f * faustpower2_f(fSlow4)) + -1.0f);
		float fSlow10 = (0.124224f * fSlow9);
		float fSlow11 = sinf((2.0f * fSlow3));
		float fSlow12 = faustpower2_f(fSlow5);
		float fSlow13 = (2.0f * fSlow6);
		float fSlow14 = sinf(fSlow13);
		float fSlow15 = cosf(fSlow13);
		float fSlow16 = (0.433016986f * fSlow7);
		float fSlow17 = (0.335413665f * fSlow8);
		float fSlow18 = (0.335413665f * fSlow4);
		float fSlow19 = (fSlow7 * fSlow5);
		float fSlow20 = (0.0f - (0.193651259f * fSlow19));
		float fSlow21 = (0.0f - (0.335413665f * fSlow19));
		float fSlow22 = (0.149071187f * fSlow8);
		float fSlow23 = (0.166664049f * fSlow14);
		float fSlow24 = (0.172513679f * fSlow15);
		float fSlow25 = (fSlow14 * fSlow12);
		float fSlow26 = (0.0f - (0.172513679f * fSlow25));
		float fSlow27 = (fSlow7 * fSlow11);
		float fSlow28 = (0.0f - (0.140856847f * fSlow27));
		float fSlow29 = (0.0f - (0.0445428938f * fSlow25));
		float fSlow30 = (0.0f - (0.109107286f * fSlow27));
		float fSlow31 = (0.0890856236f * fSlow9);
		float fSlow32 = (fSlow15 * fSlow12);
		float fSlow33 = (0.0445428938f * fSlow32);
		float fSlow34 = (fSlow31 + fSlow33);
		float fSlow35 = (0.0f - (0.149071187f * fSlow19));
		float fSlow36 = (0.192450002f * fSlow5);
		float fSlow37 = (0.166664049f * fSlow11);
		float fSlow38 = (0.0555547103f * fSlow9);
		float fSlow39 = (0.166664049f * fSlow32);
		float fSlow40 = (0.11112f - (fSlow38 + fSlow39));
		float fSlow41 = (0.149071187f * fSlow4);
		float fSlow42 = (0.0f - (0.0860663354f * fSlow19));
		float fSlow43 = (fSlow8 * fSlow11);
		float fSlow44 = (0.140856847f * fSlow43);
		float fSlow45 = (0.335413665f * fSlow5);
		float fSlow46 = ((0.111109301f * fSlow9) + 0.11112f);
		float fSlow47 = (0.109107181f * fSlow9);
		float fSlow48 = (0.140856847f * fSlow5);
		float fSlow49 = (fSlow8 * fSlow5);
		float fSlow50 = (0.0f - (0.193651259f * fSlow49));
		float fSlow51 = (0.335413665f * fSlow7);
		float fSlow52 = (0.140856847f * fSlow11);
		float fSlow53 = (fSlow31 - fSlow33);
		float fSlow54 = (0.0f - (0.109107286f * fSlow43));
		float fSlow55 = (0.149071187f * fSlow5);
		float fSlow56 = (0.0f - (0.0860663354f * fSlow49));
		float fSlow57 = (fSlow39 + (0.11112f - fSlow38));
		float fSlow58 = (0.192450002f * fSlow8);
		float fSlow59 = (0.149071187f * fSlow7);
		float fSlow60 = (0.347186089f * fSlow8);
		float fSlow61 = (0.283476323f * fSlow4);
		float fSlow62 = (0.0f - (0.0896431804f * fSlow49));
		float fSlow63 = (0.0f - (0.0896431804f * fSlow19));
		float fSlow64 = (0.0f - (0.347186089f * fSlow19));
		float fSlow65 = (fSlow14 * fSlow5);
		float fSlow66 = (0.215162277f * fSlow65);
		float fSlow67 = (0.154303327f * fSlow8);
		float fSlow68 = (0.0f - (0.154303327f * fSlow19));
		float fSlow69 = (0.0f - (0.039841f * fSlow19));
		float fSlow70 = (0.11904563f * fSlow11);
		float fSlow71 = (0.0f - (0.137462229f * fSlow25));
		float fSlow72 = (0.0793638527f * fSlow9);
		float fSlow73 = (0.11112f - fSlow72);
		float fSlow74 = (0.125988156f * fSlow4);
		float fSlow75 = (0.0f - (0.039841f * fSlow49));
		float fSlow76 = (0.283476323f * fSlow49);
		float fSlow77 = (0.358572274f * fSlow4);
		float fSlow78 = (0.0f - (0.21957989f * fSlow19));
		float fSlow79 = (0.0f - (0.283476323f * fSlow19));
		float fSlow80 = (0.0f - (0.125988156f * fSlow19));
		float fSlow81 = (0.0f - (0.0975900441f * fSlow19));
		float fSlow82 = (0.0f - (0.11904563f * fSlow27));
		float fSlow83 = (0.0396818668f * fSlow9);
		float fSlow84 = (0.11904563f * fSlow32);
		float fSlow85 = (fSlow83 + (0.11112f - fSlow84));
		float fSlow86 = (0.125988156f * fSlow49);
		float fSlow87 = (0.31053251f * fSlow5);
		float fSlow88 = (0.0f - (0.137462229f * fSlow32));
		float fSlow89 = (0.06873101f * fSlow11);
		float fSlow90 = (fSlow72 + 0.11112f);
		float fSlow91 = (0.13801302f * fSlow5);
		float fSlow92 = (0.283476323f * fSlow5);
		float fSlow93 = (0.0f - (0.21957989f * fSlow49));
		float fSlow94 = (0.0f - (0.0975900441f * fSlow49));
		float fSlow95 = ((fSlow83 + fSlow84) + 0.11112f);
		float fSlow96 = (0.125988156f * fSlow5);
		float fSlow97 = (fSlow15 * fSlow5);
		float fSlow98 = (0.215162277f * fSlow97);
		float fSlow99 = (0.154303327f * fSlow5);
		float fSlow100 = (0.347186089f * fSlow5);
		float fSlow101 = (0.172513679f * fSlow5);
		float fSlow102 = (0.113400422f * fSlow11);
		float fSlow103 = (0.0f - (0.0717206895f * fSlow25));
		float fSlow104 = (0.11112f - (0.0925911069f * fSlow9));
		float fSlow105 = (0.0f - (0.0717206895f * fSlow32));
		float fSlow106 = (0.0f - (0.113400422f * fSlow27));
		float fSlow107 = (0.0f - (0.143441588f * fSlow25));
		float fSlow108 = (0.0878395736f * fSlow7);
		float fSlow109 = (0.178171366f * fSlow11);
		float fSlow110 = (0.0f - (0.0878395736f * fSlow27));
		float fSlow111 = (0.0878395736f * fSlow8);
		float fSlow112 = (0.045360297f * fSlow7);
		float fSlow113 = (0.111109369f * fSlow32);
		float fSlow114 = (fSlow38 + (0.11112f - fSlow113));
		float fSlow115 = (0.159363806f * fSlow4);
		float fSlow116 = (0.380323172f * fSlow4);
		float fSlow117 = (0.045360297f * fSlow11);
		float fSlow118 = (0.169030756f * fSlow4);
		float fSlow119 = ((0.0740729049f * fSlow9) + 0.11112f);
		float fSlow120 = (0.0f - (0.143441588f * fSlow32));
		float fSlow121 = (0.31053251f * fSlow49);
		float fSlow122 = ((fSlow38 + fSlow113) + 0.11112f);
		float fSlow123 = (0.045360297f * fSlow8);
		float fSlow124 = (0.111109369f * fSlow25);
		float fSlow125 = (0.113400422f * fSlow7);
		float fSlow126 = (0.347186089f * fSlow49);
		float fSlow127 = (0.0717206895f * fSlow14);
		float fSlow128 = (0.113400422f * fSlow43);
		float fSlow129 = (0.35355708f * fSlow5);
		float fSlow130 = (0.181845263f * fSlow5);
		float fSlow131 = (0.250002354f * fSlow4);
		float fSlow132 = (0.306189358f * fSlow5);
		float fSlow133 = (0.111111008f * fSlow4);
		float fSlow134 = (0.136082739f * fSlow5);
		float fSlow135 = (0.0f - (0.0668158233f * fSlow49));
		float fSlow136 = (0.327330112f * fSlow4);
		float fSlow137 = (0.0668158233f * fSlow7);
		float fSlow138 = (0.0f - (0.0296956114f * fSlow49));
		float fSlow139 = (0.0687310249f * fSlow9);
		float fSlow140 = (0.137462229f * fSlow11);
		float fSlow141 = (0.145478532f * fSlow4);
		float fSlow142 = (0.11904563f * fSlow65);
		float fSlow143 = (0.0296956114f * fSlow7);
		float fSlow144 = (0.0f - (0.115728557f * fSlow49));
		float fSlow145 = (0.365966052f * fSlow4);
		float fSlow146 = (0.0f - (0.0514343791f * fSlow49));
		float fSlow147 = (0.0f - (0.0486002117f * fSlow25));
		float fSlow148 = (0.0972004384f * fSlow9);
		float fSlow149 = (0.0486002117f * fSlow32);
		float fSlow150 = (fSlow148 + fSlow149);
		float fSlow151 = (0.0f - (0.0486002117f * fSlow43));
		float fSlow152 = (0.162649885f * fSlow4);
		float fSlow153 = (0.0f - (0.231457114f * fSlow19));
		float fSlow154 = (0.377968401f * fSlow4);
		float fSlow155 = (0.0f - (0.231457114f * fSlow49));
		float fSlow156 = (0.0f - (0.102868758f * fSlow19));
		float fSlow157 = (0.0307374373f * fSlow12);
		float fSlow158 = (0.0f - (0.122949965f * fSlow43));
		float fSlow159 = (0.0f - (0.122949965f * fSlow27));
		float fSlow160 = (0.10647773f * fSlow9);
		float fSlow161 = (0.167984217f * fSlow4);
		float fSlow162 = (0.0f - (0.102868758f * fSlow49));
		float fSlow163 = (0.298810303f * fSlow49);
		float fSlow164 = (0.0f - (0.115728557f * fSlow19));
		float fSlow165 = (0.132803202f * fSlow5);
		float fSlow166 = (0.168356091f * fSlow11);
		float fSlow167 = (0.0f - (0.0514343791f * fSlow19));
		float fSlow168 = (fSlow148 - fSlow149);
		float fSlow169 = (0.0f - (0.0486002117f * fSlow27));
		float fSlow170 = (0.0f - (0.258777022f * fSlow19));
		float fSlow171 = (0.0f - (0.0668158233f * fSlow19));
		float fSlow172 = (0.258777022f * fSlow49);
		float fSlow173 = (0.115010813f * fSlow5);
		float fSlow174 = (0.0f - (0.115010813f * fSlow19));
		float fSlow175 = (0.0f - (0.0296956114f * fSlow19));
		float fSlow176 = (0.0f - (0.137462229f * fSlow27));
		float fSlow177 = (0.0f - (0.306189358f * fSlow19));
		float fSlow178 = (0.306189358f * fSlow49);
		float fSlow179 = (0.0f - (0.136082739f * fSlow19));
		float fSlow180 = (0.0f - (0.128583997f * fSlow25));
		float fSlow181 = (0.0f - (0.128583997f * fSlow27));
		float fSlow182 = (0.136082739f * fSlow49);
		float fSlow183 = (0.0f - (0.35355708f * fSlow19));
		float fSlow184 = (0.35355708f * fSlow49);
		float fSlow185 = (0.157134846f * fSlow8);
		float fSlow186 = (0.181845263f * fSlow97);
		float fSlow187 = (0.0f - (0.181845263f * fSlow25));
		float fSlow188 = (0.0f - (0.157134846f * fSlow19));
		float fSlow189 = (0.187274665f * fSlow12);
		float fSlow190 = (0.145062402f * fSlow12);
		float fSlow191 = (0.11844296f * fSlow11);
		float fSlow192 = (0.108123131f * fSlow12);
		float fSlow193 = (0.0558345877f * fSlow9);
		float fSlow194 = (0.13676618f * fSlow11);
		float fSlow195 = (0.0f - (0.0341914371f * fSlow43));
		float fSlow196 = (0.08375182f * fSlow9);
		float fSlow197 = (0.108123131f * fSlow25);
		float fSlow198 = (0.0341914371f * fSlow7);
		float fSlow199 = (0.0129231848f * fSlow32);
		float fSlow200 = (0.0f - (0.063310422f * fSlow43));
		float fSlow201 = (0.100102559f * fSlow9);
		float fSlow202 = (0.0500512645f * fSlow32);
		float fSlow203 = (fSlow201 + fSlow202);
		float fSlow204 = (0.0f - (0.0500512645f * fSlow25));
		float fSlow205 = (0.0f - (0.0129231848f * fSlow25));
		float fSlow206 = (0.040866632f * fSlow12);
		float fSlow207 = (0.0f - (0.12923184f * fSlow43));
		float fSlow208 = (0.0f - (0.12923184f * fSlow27));
		float fSlow209 = (0.105517358f * fSlow9);
		float fSlow210 = (0.0129231848f * fSlow12);
		float fSlow211 = (fSlow201 - fSlow202);
		float fSlow212 = (0.0f - (0.063310422f * fSlow27));
		float fSlow213 = (0.163466752f * fSlow43);
		float fSlow214 = (0.108123131f * fSlow32);
		float fSlow215 = (0.0f - (0.132423133f * fSlow27));
		float fSlow216 = (0.0f - (0.0341914371f * fSlow27));
		float fSlow217 = (0.132423133f * fSlow43);
		float fSlow218 = (0.0f - fSlow197);
		float fSlow219 = (0.0f - (0.13676618f * fSlow27));
		float fSlow220 = (0.13676618f * fSlow43);
		float fSlow221 = (0.0f - (0.145062402f * fSlow25));
		float fSlow222 = (0.0f - (0.11844296f * fSlow27));
		float fSlow223 = (0.145062402f * fSlow32);
		float fSlow224 = (0.11844296f * fSlow43);
		float fSlow225 = (0.0f - (0.187274665f * fSlow25));
		float fSlow226 = (0.187274665f * fSlow32);
		for (int i = 0; (i < count); i = (i + 1)) {
			float fTemp0 = float(input0[i]);
			float fTemp1 = float(input2[i]);
			float fTemp2 = float(input1[i]);
			float fTemp3 = float(input3[i]);
			float fTemp4 = ((fSlow7 * fTemp2) + (fSlow8 * fTemp3));
			float fTemp5 = ((fSlow4 * fTemp1) + (fSlow5 * fTemp4));
			float fTemp6 = float(input6[i]);
			float fTemp7 = float(input5[i]);
			float fTemp8 = float(input7[i]);
			float fTemp9 = ((fSlow7 * fTemp7) + (fSlow8 * fTemp8));
			float fTemp10 = float(input4[i]);
			float fTemp11 = float(input8[i]);
			float fTemp12 = ((fSlow14 * fTemp10) + (fSlow15 * fTemp11));
			output0[i] = FAUSTFLOAT((iSlow1?((0.11112f * fTemp0) + (((fSlow10 * fTemp6) + (0.192450002f * fTemp5)) + (0.215162277f * ((fSlow11 * fTemp9) + (fSlow12 * fTemp12))))):(iSlow2?((0.249929994f * fTemp0) + (0.433016986f * fTemp5)):fTemp0)));
			float fTemp13 = float(input9[i]);
			float fTemp14 = float(input15[i]);
			float fTemp15 = float(input14[i]);
			float fTemp16 = float(input13[i]);
			float fTemp17 = float(input12[i]);
			float fTemp18 = float(input11[i]);
			float fTemp19 = (fSlow37 * fTemp1);
			float fTemp20 = float(input10[i]);
			output1[i] = FAUSTFLOAT((iSlow1?((fSlow5 * ((fSlow22 * fTemp10) + (fSlow5 * ((fSlow23 * fTemp3) + (fSlow24 * fTemp13))))) + ((fSlow26 * fTemp14) + ((fSlow28 * fTemp15) + ((fSlow29 * fTemp16) + ((fSlow30 * fTemp17) + ((fSlow34 * fTemp18) + (((fSlow35 * fTemp11) + (((fSlow7 * ((fSlow36 * fTemp0) + fTemp19)) + ((fSlow40 * fTemp2) + (fSlow41 * fTemp7))) + (fSlow42 * fTemp6))) + (fSlow44 * fTemp20)))))))):(iSlow2?((0.249929994f * fTemp2) + ((((fSlow5 * ((fSlow16 * fTemp0) + (fSlow17 * fTemp10))) + (fSlow18 * fTemp7)) + (fSlow20 * fTemp6)) + (fSlow21 * fTemp11))):fTemp2)));
			float fTemp21 = (0.433016986f * fTemp0);
			float fTemp22 = ((fSlow7 * fTemp18) + (fSlow8 * fTemp16));
			float fTemp23 = ((fSlow14 * fTemp20) + (fSlow15 * fTemp15));
			output2[i] = FAUSTFLOAT((iSlow1?((fSlow11 * ((0.166664049f * fTemp4) + (0.178171366f * fTemp22))) + ((((fSlow46 * fTemp1) + (fSlow4 * ((0.192450002f * fTemp0) + (0.172132477f * fTemp6)))) + (fSlow47 * fTemp17)) + (fSlow5 * ((0.149071187f * fTemp9) + (fSlow48 * fTemp23))))):(iSlow2?((0.249929994f * fTemp1) + ((fSlow4 * (fTemp21 + (0.387302101f * fTemp6))) + (fSlow45 * fTemp9))):fTemp1)));
			float fTemp24 = ((fSlow7 * fTemp20) + (fSlow8 * fTemp15));
			output3[i] = FAUSTFLOAT((iSlow1?(((fSlow52 * fTemp24) + ((fSlow53 * fTemp16) + ((fSlow54 * fTemp17) + (((fSlow8 * (fTemp19 + (fSlow55 * fTemp11))) + ((fSlow56 * fTemp6) + ((fSlow57 * fTemp3) + (fSlow41 * fTemp8)))) + (fSlow29 * fTemp18))))) + (fSlow5 * (((fSlow58 * fTemp0) + (fSlow59 * fTemp10)) + (fSlow5 * ((fSlow14 * ((0.166664049f * fTemp2) + (0.172513679f * fTemp13))) + (fSlow24 * fTemp14)))))):(iSlow2?((0.249929994f * fTemp3) + (((fSlow50 * fTemp6) + (fSlow18 * fTemp8)) + (fSlow5 * ((fSlow51 * fTemp10) + (fSlow8 * (fTemp21 + (0.335413665f * fTemp11))))))):fTemp3)));
			float fTemp25 = ((fSlow8 * fTemp2) + (fSlow7 * fTemp3));
			float fTemp26 = ((fSlow8 * fTemp7) + (fSlow7 * fTemp8));
			output4[i] = FAUSTFLOAT((iSlow1?((fSlow5 * ((fSlow66 * fTemp0) + ((fSlow67 * fTemp13) + (0.149071187f * fTemp25)))) + ((fSlow68 * fTemp14) + ((fSlow69 * fTemp16) + (((fSlow70 * fTemp26) + ((fSlow71 * fTemp6) + ((fSlow73 * fTemp10) + (fSlow74 * fTemp20)))) + (fSlow75 * fTemp18))))):(iSlow2?((0.249929994f * fTemp10) + (((((fSlow5 * ((0.335413665f * fTemp25) + (fSlow60 * fTemp13))) + (fSlow61 * fTemp20)) + (fSlow62 * fTemp18)) + (fSlow63 * fTemp16)) + (fSlow64 * fTemp14))):fTemp10)));
			float fTemp27 = (fSlow55 * fTemp1);
			float fTemp28 = ((0.215162277f * fTemp0) + (0.06873101f * fTemp6));
			output5[i] = FAUSTFLOAT((iSlow1?((fSlow7 * (fTemp27 + (fSlow11 * fTemp28))) + ((fSlow80 * fTemp15) + ((fSlow81 * fTemp17) + ((fSlow4 * ((0.149071187f * fTemp2) + (0.159363806f * fTemp18))) + ((fSlow82 * fTemp11) + ((0.11904563f * ((fSlow43 * fTemp10) + (fSlow25 * fTemp8))) + ((fSlow85 * fTemp7) + (fSlow86 * fTemp20)))))))):(iSlow2?((0.249929994f * fTemp7) + (((((0.335413665f * ((fSlow4 * fTemp2) + (fSlow19 * fTemp1))) + (fSlow76 * fTemp20)) + (fSlow77 * fTemp18)) + (fSlow78 * fTemp17)) + (fSlow79 * fTemp15))):fTemp7)));
			output6[i] = FAUSTFLOAT((iSlow1?((fSlow4 * ((0.172132477f * fTemp1) + (0.169030756f * fTemp17))) + ((fSlow88 * fTemp11) + ((fSlow89 * fTemp9) + ((fSlow90 * fTemp6) + ((fSlow71 * fTemp10) + ((fSlow56 * fTemp3) + ((fSlow42 * fTemp2) + ((fSlow10 * fTemp0) + (fSlow91 * fTemp22))))))))):(iSlow2?((0.249929994f * fTemp6) + ((fSlow87 * fTemp22) + ((fSlow4 * ((0.387302101f * fTemp1) + (0.380323172f * fTemp17))) + ((fSlow20 * fTemp2) + (fSlow50 * fTemp3))))):fTemp6)));
			output7[i] = FAUSTFLOAT((iSlow1?((fSlow8 * (fTemp27 + (fSlow11 * (fTemp28 + (0.11904563f * fTemp11))))) + ((fSlow4 * ((0.149071187f * fTemp3) + (0.159363806f * fTemp16))) + ((fSlow94 * fTemp17) + ((fSlow95 * fTemp8) + ((0.11904563f * ((fSlow27 * fTemp10) + (fSlow25 * fTemp7))) + (fSlow96 * fTemp24)))))):(iSlow2?((0.249929994f * fTemp8) + ((fSlow92 * fTemp24) + ((fSlow77 * fTemp16) + ((0.335413665f * ((fSlow49 * fTemp1) + (fSlow4 * fTemp3))) + (fSlow93 * fTemp17))))):fTemp8)));
			output8[i] = FAUSTFLOAT((iSlow1?((fSlow5 * (((fSlow22 * fTemp3) + (fSlow98 * fTemp0)) + (fSlow7 * ((0.154303327f * fTemp13) + (0.039841f * fTemp18))))) + ((fSlow8 * ((fSlow70 * fTemp8) + (fSlow99 * fTemp14))) + ((fSlow75 * fTemp16) + ((fSlow73 * fTemp11) + ((fSlow82 * fTemp7) + ((fSlow88 * fTemp6) + ((fSlow35 * fTemp2) + (fSlow74 * fTemp15)))))))):(iSlow2?((0.249929994f * fTemp11) + (((fSlow61 * fTemp15) + ((fSlow21 * fTemp2) + (fSlow62 * fTemp16))) + (fSlow5 * ((fSlow7 * ((0.347186089f * fTemp13) + (0.0896431804f * fTemp18))) + (fSlow8 * ((0.335413665f * fTemp3) + (0.347186089f * fTemp14))))))):fTemp11)));
			float fTemp29 = ((fSlow8 * fTemp10) + (fSlow7 * fTemp11));
			float fTemp30 = ((fSlow8 * fTemp20) + (fSlow7 * fTemp15));
			output9[i] = FAUSTFLOAT((iSlow1?((fSlow5 * ((fSlow101 * ((fSlow14 * fTemp3) + (fSlow15 * fTemp2))) + (0.154303327f * fTemp29))) + ((fSlow102 * fTemp30) + ((fSlow103 * fTemp16) + ((fSlow104 * fTemp13) + (fSlow105 * fTemp18))))):(iSlow2?((fSlow100 * fTemp29) + (0.249929994f * fTemp13)):fTemp13)));
			float fTemp31 = ((fSlow4 * fTemp10) + (fSlow5 * fTemp26));
			output10[i] = FAUSTFLOAT((iSlow1?((0.11112f * fTemp20) + ((0.140856847f * ((fSlow25 * fTemp1) + (fSlow11 * fTemp25))) + ((0.125988156f * fTemp31) + ((fSlow106 * fTemp14) + ((fSlow107 * fTemp17) + (fSlow11 * ((fSlow8 * ((0.113400422f * fTemp13) + (0.0878395736f * fTemp18))) + (fSlow108 * fTemp16)))))))):(iSlow2?((0.249929994f * fTemp20) + (0.283476323f * fTemp31)):fTemp20)));
			output11[i] = FAUSTFLOAT((iSlow1?((fSlow7 * ((fSlow109 * fTemp1) + (fSlow5 * ((0.13801302f * fTemp6) + (0.039841f * fTemp11))))) + ((fSlow25 * ((0.111109369f * fTemp16) + (0.0717206895f * fTemp14))) + ((fSlow110 * fTemp15) + ((fSlow11 * ((fSlow111 * fTemp20) + (fSlow112 * fTemp17))) + ((fSlow114 * fTemp18) + (((fSlow75 * fTemp10) + ((fSlow29 * fTemp3) + ((fSlow34 * fTemp2) + (fSlow115 * fTemp7)))) + (fSlow105 * fTemp13))))))):(iSlow2?((((fSlow62 * fTemp10) + (fSlow77 * fTemp7)) + (fSlow19 * ((0.31053251f * fTemp6) + (0.0896431804f * fTemp11)))) + (0.249929994f * fTemp18)):fTemp18)));
			output12[i] = FAUSTFLOAT((iSlow1?(((fSlow117 * fTemp22) + (((fSlow94 * fTemp8) + ((fSlow81 * fTemp7) + ((fSlow54 * fTemp3) + ((fSlow47 * fTemp1) + ((fSlow30 * fTemp2) + ((fSlow107 * fTemp20) + (fSlow118 * fTemp6))))))) + (fSlow119 * fTemp17))) + (fSlow120 * fTemp15)):(iSlow2?((((fSlow78 * fTemp7) + (fSlow116 * fTemp6)) + (fSlow93 * fTemp8)) + (0.249929994f * fTemp17)):fTemp17)));
			output13[i] = FAUSTFLOAT((iSlow1?((fSlow8 * ((fSlow91 * fTemp6) + (fSlow11 * ((0.178171366f * fTemp1) + (0.0878395736f * fTemp15))))) + (((fSlow122 * fTemp16) + ((fSlow11 * ((fSlow108 * fTemp20) + (fSlow123 * fTemp17))) + ((fSlow124 * fTemp18) + ((fSlow103 * fTemp13) + (((fSlow69 * fTemp10) + ((fSlow53 * fTemp3) + ((fSlow29 * fTemp2) + (fSlow115 * fTemp8)))) + (fSlow75 * fTemp11)))))) + (fSlow105 * fTemp14))):(iSlow2?((((fSlow121 * fTemp6) + ((fSlow63 * fTemp10) + (fSlow77 * fTemp8))) + (fSlow62 * fTemp11)) + (0.249929994f * fTemp16)):fTemp16)));
			float fTemp32 = ((fSlow49 * fTemp8) + (fSlow4 * fTemp11));
			output14[i] = FAUSTFLOAT((iSlow1?((0.11112f * fTemp15) + (((fSlow120 * fTemp17) + (((fSlow80 * fTemp7) + ((0.140856847f * ((fSlow43 * fTemp3) + (fSlow32 * fTemp1))) + ((fSlow28 * fTemp2) + (0.125988156f * fTemp32)))) + (fSlow110 * fTemp18))) + (fSlow11 * ((fSlow125 * fTemp13) + (fSlow8 * ((0.0878395736f * fTemp16) + (0.113400422f * fTemp14))))))):(iSlow2?(((fSlow79 * fTemp7) + (0.283476323f * fTemp32)) + (0.249929994f * fTemp15)):fTemp15)));
			output15[i] = FAUSTFLOAT((iSlow1?((fSlow5 * ((fSlow67 * fTemp11) + (fSlow5 * ((fSlow24 * fTemp3) + (fSlow127 * fTemp18))))) + ((fSlow104 * fTemp14) + ((fSlow105 * fTemp16) + ((fSlow106 * fTemp20) + ((fSlow68 * fTemp10) + ((fSlow26 * fTemp2) + (fSlow128 * fTemp15))))))):(iSlow2?(((fSlow64 * fTemp10) + (fSlow126 * fTemp11)) + (0.249929994f * fTemp14)):fTemp14)));
			float fTemp33 = ((fSlow8 * fTemp13) + (fSlow7 * fTemp14));
			output16[i] = FAUSTFLOAT((iSlow1?(fSlow5 * ((fSlow130 * ((fSlow15 * fTemp10) + (fSlow14 * fTemp11))) + (0.157134846f * fTemp33))):(iSlow2?(fSlow129 * fTemp33):0.0f)));
			output17[i] = FAUSTFLOAT((iSlow1?(((fSlow133 * fTemp13) + (fSlow134 * fTemp30)) + (0.128583997f * ((fSlow12 * ((fSlow14 * fTemp8) + (fSlow15 * fTemp7))) + (fSlow11 * fTemp29)))):(iSlow2?((fSlow131 * fTemp13) + (fSlow132 * fTemp30)):0.0f)));
			float fTemp34 = ((fSlow7 * fTemp16) + (fSlow8 * fTemp18));
			output18[i] = FAUSTFLOAT((iSlow1?(((fSlow138 * fTemp13) + ((fSlow139 * fTemp10) + ((fSlow140 * fTemp26) + (fSlow141 * fTemp20)))) + (fSlow5 * ((fSlow142 * fTemp6) + ((fSlow143 * fTemp14) + (0.115010813f * fTemp34))))):(iSlow2?(((fSlow135 * fTemp13) + (fSlow136 * fTemp20)) + (fSlow5 * ((fSlow137 * fTemp14) + (0.258777022f * fTemp34)))):0.0f)));
			output19[i] = FAUSTFLOAT((iSlow1?(((fSlow146 * fTemp20) + ((fSlow147 * fTemp8) + ((fSlow150 * fTemp7) + ((fSlow151 * fTemp10) + (fSlow152 * fTemp18))))) + (fSlow7 * ((fSlow11 * ((0.168356091f * fTemp6) + (0.0486002117f * fTemp11))) + (fSlow5 * ((0.132803202f * fTemp17) + (0.0514343791f * fTemp15)))))):(iSlow2?(((fSlow144 * fTemp20) + (fSlow145 * fTemp18)) + (fSlow19 * ((0.298810303f * fTemp17) + (0.115728557f * fTemp15)))):0.0f)));
			output20[i] = FAUSTFLOAT((iSlow1?(((fSlow156 * fTemp18) + ((fSlow157 * fTemp12) + ((fSlow158 * fTemp8) + ((fSlow159 * fTemp7) + ((fSlow160 * fTemp6) + (fSlow161 * fTemp17)))))) + (fSlow162 * fTemp16)):(iSlow2?(((fSlow153 * fTemp18) + (fSlow154 * fTemp17)) + (fSlow155 * fTemp16)):0.0f)));
			output21[i] = FAUSTFLOAT((iSlow1?(((fSlow8 * ((fSlow165 * fTemp17) + (fSlow166 * fTemp6))) + ((fSlow167 * fTemp20) + ((fSlow151 * fTemp11) + ((fSlow168 * fTemp8) + ((fSlow147 * fTemp7) + ((fSlow169 * fTemp10) + (fSlow152 * fTemp16))))))) + (fSlow146 * fTemp15)):(iSlow2?(((fSlow163 * fTemp17) + ((fSlow164 * fTemp20) + (fSlow145 * fTemp16))) + (fSlow144 * fTemp15)):0.0f)));
			output22[i] = FAUSTFLOAT((iSlow1?(((fSlow8 * ((fSlow140 * fTemp8) + (fSlow173 * fTemp16))) + ((fSlow174 * fTemp18) + ((fSlow175 * fTemp13) + ((fSlow139 * fTemp11) + ((fSlow84 * fTemp6) + ((fSlow176 * fTemp7) + (fSlow141 * fTemp15))))))) + (fSlow138 * fTemp14)):(iSlow2?(((fSlow170 * fTemp18) + ((fSlow171 * fTemp13) + ((fSlow172 * fTemp16) + (fSlow136 * fTemp15)))) + (fSlow135 * fTemp14)):0.0f)));
			output23[i] = FAUSTFLOAT((iSlow1?((fSlow179 * fTemp20) + ((0.128583997f * ((fSlow43 * fTemp11) + (fSlow32 * fTemp8))) + ((fSlow180 * fTemp7) + ((fSlow181 * fTemp10) + ((fSlow182 * fTemp15) + (fSlow133 * fTemp14)))))):(iSlow2?(((fSlow177 * fTemp20) + (fSlow178 * fTemp15)) + (fSlow131 * fTemp14)):0.0f)));
			output24[i] = FAUSTFLOAT((iSlow1?((fSlow5 * ((fSlow185 * fTemp14) + (fSlow186 * fTemp11))) + ((fSlow187 * fTemp10) + (fSlow188 * fTemp13))):(iSlow2?((fSlow183 * fTemp13) + (fSlow184 * fTemp14)):0.0f)));
			output25[i] = FAUSTFLOAT((iSlow1?(fSlow189 * ((fSlow15 * fTemp13) + (fSlow14 * fTemp14))):0.0f));
			output26[i] = FAUSTFLOAT((iSlow1?((fSlow190 * ((fSlow15 * fTemp20) + (fSlow14 * fTemp15))) + (fSlow191 * fTemp33)):0.0f));
			output27[i] = FAUSTFLOAT((iSlow1?((fSlow192 * ((fSlow14 * fTemp16) + (fSlow15 * fTemp18))) + ((fSlow193 * fTemp13) + (fSlow194 * fTemp30))):0.0f));
			output28[i] = FAUSTFLOAT((iSlow1?((((fSlow195 * fTemp13) + (fSlow196 * fTemp20)) + (fSlow197 * fTemp17)) + (fSlow11 * ((fSlow198 * fTemp14) + (0.132423133f * fTemp34)))):0.0f));
			output29[i] = FAUSTFLOAT((iSlow1?(((fSlow27 * ((0.163466752f * fTemp17) + (0.063310422f * fTemp15))) + ((((fSlow199 * fTemp13) + (fSlow200 * fTemp20)) + (fSlow203 * fTemp18)) + (fSlow204 * fTemp16))) + (fSlow205 * fTemp14)):0.0f));
			output30[i] = FAUSTFLOAT((iSlow1?((fSlow206 * fTemp23) + ((fSlow207 * fTemp16) + ((fSlow208 * fTemp18) + (fSlow209 * fTemp17)))):0.0f));
			output31[i] = FAUSTFLOAT((iSlow1?((fSlow210 * ((fSlow14 * fTemp13) + (fSlow15 * fTemp14))) + ((fSlow200 * fTemp15) + ((fSlow211 * fTemp16) + ((fSlow204 * fTemp18) + ((fSlow212 * fTemp20) + (fSlow213 * fTemp17)))))):0.0f));
			output32[i] = FAUSTFLOAT((iSlow1?((((fSlow214 * fTemp17) + ((fSlow215 * fTemp18) + ((fSlow216 * fTemp13) + (fSlow217 * fTemp16)))) + (fSlow196 * fTemp15)) + (fSlow195 * fTemp14)):0.0f));
			output33[i] = FAUSTFLOAT((iSlow1?(((fSlow214 * fTemp16) + ((fSlow218 * fTemp18) + ((fSlow219 * fTemp20) + (fSlow220 * fTemp15)))) + (fSlow193 * fTemp14)):0.0f));
			output34[i] = FAUSTFLOAT((iSlow1?((fSlow221 * fTemp20) + ((fSlow222 * fTemp13) + ((fSlow223 * fTemp15) + (fSlow224 * fTemp14)))):0.0f));
			output35[i] = FAUSTFLOAT((iSlow1?((fSlow225 * fTemp13) + (fSlow226 * fTemp14)):0.0f));
			
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
