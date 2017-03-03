/* ------------------------------------------------------------
author: "Pierre Lecomte"
copyright: "(c) Pierre Lecomte 2014"
license: "GPL"
name: "HOAEncLebedev504"
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



#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
#endif

class mydsp : public dsp {
	
 private:
	
	int fSamplingFreq;
	float fRec0[2];
	FAUSTFLOAT fHslider0;
	
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
		m->declare("name", "HOAEncLebedev504");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
		m->declare("version", "1.0");
	}

	virtual int getNumInputs() {
		return 50;
		
	}
	virtual int getNumOutputs() {
		return 25;
		
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
		
	}
	
	virtual void instanceClear() {
		for (int i0 = 0; (i0 < 2); i0 = (i0 + 1)) {
			fRec0[i0] = 0.0f;
			
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
		ui_interface->declare(0, "1", "");
		ui_interface->openHorizontalBox("Outputs");
		ui_interface->declare(&fHslider0, "1", "");
		ui_interface->declare(&fHslider0, "style", "knob");
		ui_interface->declare(&fHslider0, "unit", "dB");
		ui_interface->addHorizontalSlider("Gain", &fHslider0, 0.0f, -10.0f, 50.0f, 0.100000001f);
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
		FAUSTFLOAT* input36 = inputs[36];
		FAUSTFLOAT* input37 = inputs[37];
		FAUSTFLOAT* input38 = inputs[38];
		FAUSTFLOAT* input39 = inputs[39];
		FAUSTFLOAT* input40 = inputs[40];
		FAUSTFLOAT* input41 = inputs[41];
		FAUSTFLOAT* input42 = inputs[42];
		FAUSTFLOAT* input43 = inputs[43];
		FAUSTFLOAT* input44 = inputs[44];
		FAUSTFLOAT* input45 = inputs[45];
		FAUSTFLOAT* input46 = inputs[46];
		FAUSTFLOAT* input47 = inputs[47];
		FAUSTFLOAT* input48 = inputs[48];
		FAUSTFLOAT* input49 = inputs[49];
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
		float fSlow0 = (0.00100000005f * powf(10.0f, (0.0500000007f * float(fHslider0))));
		for (int i = 0; (i < count); i = (i + 1)) {
			float fTemp0 = float(input0[i]);
			float fTemp1 = float(input1[i]);
			float fTemp2 = float(input2[i]);
			float fTemp3 = float(input3[i]);
			float fTemp4 = float(input4[i]);
			float fTemp5 = float(input5[i]);
			float fTemp6 = float(input6[i]);
			float fTemp7 = float(input7[i]);
			float fTemp8 = float(input8[i]);
			float fTemp9 = float(input9[i]);
			float fTemp10 = (((fTemp6 + fTemp7) + fTemp8) + fTemp9);
			float fTemp11 = float(input10[i]);
			float fTemp12 = float(input11[i]);
			float fTemp13 = float(input12[i]);
			float fTemp14 = float(input13[i]);
			float fTemp15 = float(input14[i]);
			float fTemp16 = float(input15[i]);
			float fTemp17 = float(input16[i]);
			float fTemp18 = float(input17[i]);
			float fTemp19 = float(input18[i]);
			float fTemp20 = float(input19[i]);
			float fTemp21 = (fTemp19 + fTemp20);
			float fTemp22 = float(input20[i]);
			float fTemp23 = float(input21[i]);
			float fTemp24 = ((fTemp21 + fTemp22) + fTemp23);
			float fTemp25 = float(input22[i]);
			float fTemp26 = float(input23[i]);
			float fTemp27 = float(input24[i]);
			float fTemp28 = float(input25[i]);
			float fTemp29 = ((((fTemp24 + fTemp25) + fTemp26) + fTemp27) + fTemp28);
			float fTemp30 = float(input26[i]);
			float fTemp31 = float(input27[i]);
			float fTemp32 = (fTemp30 + fTemp31);
			float fTemp33 = float(input28[i]);
			float fTemp34 = float(input29[i]);
			float fTemp35 = ((fTemp32 + fTemp33) + fTemp34);
			float fTemp36 = float(input30[i]);
			float fTemp37 = float(input31[i]);
			float fTemp38 = float(input32[i]);
			float fTemp39 = float(input33[i]);
			float fTemp40 = float(input34[i]);
			float fTemp41 = float(input35[i]);
			float fTemp42 = float(input36[i]);
			float fTemp43 = float(input37[i]);
			float fTemp44 = float(input38[i]);
			float fTemp45 = float(input39[i]);
			float fTemp46 = float(input40[i]);
			float fTemp47 = float(input41[i]);
			float fTemp48 = float(input42[i]);
			float fTemp49 = float(input43[i]);
			float fTemp50 = float(input44[i]);
			float fTemp51 = float(input45[i]);
			float fTemp52 = float(input46[i]);
			float fTemp53 = float(input47[i]);
			float fTemp54 = float(input48[i]);
			float fTemp55 = float(input49[i]);
			fRec0[0] = (fSlow0 + (0.999000013f * fRec0[1]));
			output0[i] = FAUSTFLOAT((((((0.0126984129f * (((((fTemp0 + fTemp1) + fTemp2) + fTemp3) + fTemp4) + fTemp5)) + (0.0225749556f * ((((((((fTemp10 + fTemp11) + fTemp12) + fTemp13) + fTemp14) + fTemp15) + fTemp16) + fTemp17) + fTemp18))) + (0.0210937504f * fTemp29)) + (0.0201733354f * ((((((((((((((((((((fTemp35 + fTemp36) + fTemp37) + fTemp38) + fTemp39) + fTemp40) + fTemp41) + fTemp42) + fTemp43) + fTemp44) + fTemp45) + fTemp46) + fTemp47) + fTemp48) + fTemp49) + fTemp50) + fTemp51) + fTemp52) + fTemp53) + fTemp54) + fTemp55))) * fRec0[0]));
			float fTemp56 = (fTemp19 + fTemp25);
			float fTemp57 = (fTemp22 + fTemp27);
			float fTemp58 = (fTemp56 - fTemp57);
			float fTemp59 = (fTemp37 + fTemp38);
			float fTemp60 = (fTemp39 + fTemp47);
			float fTemp61 = (fTemp36 + fTemp44);
			float fTemp62 = (fTemp2 - fTemp4);
			float fTemp63 = (fTemp11 - fTemp13);
			float fTemp64 = (fTemp8 + fTemp17);
			float fTemp65 = (fTemp20 + fTemp26);
			float fTemp66 = ((fTemp32 + fTemp52) + fTemp53);
			float fTemp67 = (fTemp33 + fTemp54);
			float fTemp68 = (fTemp23 + fTemp28);
			float fTemp69 = (fTemp40 + fTemp48);
			float fTemp70 = (fTemp43 + fTemp51);
			float fTemp71 = (fTemp34 + fTemp55);
			output1[i] = FAUSTFLOAT((fRec0[0] * (((2.69352445e-18f * fTemp3) + (((0.0276485607f * (((fTemp7 + fTemp12) + fTemp16) - ((fTemp9 + fTemp14) + fTemp18))) + ((0.0210937504f * fTemp58) + ((0.0316055417f * (((fTemp59 + fTemp45) + fTemp46) - (((fTemp41 + fTemp42) + fTemp49) + fTemp50))) + ((0.0105351806f * fTemp60) + ((0.0105351806f * fTemp61) + ((((0.0219942965f * fTemp62) + (0.0276485607f * fTemp63)) + (3.38597223e-18f * fTemp64)) + (0.0210937504f * fTemp65))))))) + (0.0105351806f * (fTemp66 - fTemp67)))) - ((((0.0210937504f * fTemp68) + (0.0105351806f * fTemp69)) + (0.0105351806f * fTemp70)) + (0.0105351806f * fTemp71)))));
			float fTemp72 = (fTemp0 - fTemp5);
			float fTemp73 = (fTemp10 - (((fTemp15 + fTemp16) + fTemp17) + fTemp18));
			float fTemp74 = (fTemp24 - (((fTemp25 + fTemp26) + fTemp27) + fTemp28));
			float fTemp75 = (((((((fTemp36 + fTemp37) + fTemp38) + fTemp39) + fTemp40) + fTemp41) + fTemp42) + fTemp43);
			float fTemp76 = (fTemp75 - (((((((fTemp44 + fTemp45) + fTemp46) + fTemp47) + fTemp48) + fTemp49) + fTemp50) + fTemp51));
			float fTemp77 = (fTemp35 - (((fTemp52 + fTemp53) + fTemp54) + fTemp55));
			output2[i] = FAUSTFLOAT((fRec0[0] * (((((0.0219942965f * fTemp72) + (0.0276485607f * fTemp73)) + (0.0210937504f * fTemp74)) + (0.0105351806f * fTemp76)) + (0.0316055417f * fTemp77))));
			float fTemp78 = (fTemp61 - (((fTemp39 + fTemp40) + fTemp47) + fTemp48));
			float fTemp79 = (fTemp30 + fTemp52);
			float fTemp80 = (fTemp31 + fTemp53);
			float fTemp81 = (fTemp79 - fTemp80);
			float fTemp82 = (fTemp42 + fTemp50);
			float fTemp83 = (fTemp37 + fTemp45);
			float fTemp84 = (fTemp7 + fTemp16);
			float fTemp85 = (fTemp1 - fTemp3);
			float fTemp86 = (fTemp9 + fTemp18);
			float fTemp87 = (fTemp38 + fTemp46);
			float fTemp88 = (fTemp41 + fTemp49);
			output3[i] = FAUSTFLOAT((fRec0[0] * (((0.0276485607f * fTemp14) + ((0.0276485607f * (((fTemp6 + fTemp11) + fTemp15) - ((fTemp8 + fTemp13) + fTemp17))) + ((0.0316055417f * fTemp78) + ((0.0105351806f * fTemp81) + ((0.0105351806f * fTemp82) + ((0.0105351806f * fTemp83) + ((0.0210937504f * fTemp56) + ((1.69298611e-18f * fTemp84) + ((1.34676222e-18f * ((fTemp0 + fTemp2) + fTemp5)) + ((0.0219942965f * fTemp85) + (((0.0210937504f * fTemp68) + (0.0316055417f * fTemp70)) + (0.0105351806f * fTemp71)))))))))))) - ((0.0276485607f * fTemp12) + ((4.04028677e-18f * fTemp4) + ((((((5.07895844e-18f * fTemp86) + (0.0210937504f * fTemp65)) + (0.0210937504f * fTemp57)) + (0.0105351806f * fTemp87)) + (0.0105351806f * fTemp88)) + (0.0105351806f * fTemp67)))))));
			float fTemp89 = ((fTemp11 + fTemp13) - (fTemp12 + fTemp14));
			float fTemp90 = (fTemp19 + fTemp22);
			float fTemp91 = (fTemp20 + fTemp23);
			float fTemp92 = (((fTemp90 + fTemp25) + fTemp27) - ((fTemp91 + fTemp26) + fTemp28));
			float fTemp93 = (fTemp30 + fTemp33);
			float fTemp94 = (fTemp31 + fTemp34);
			float fTemp95 = (((fTemp93 + fTemp52) + fTemp54) - ((fTemp94 + fTemp53) + fTemp55));
			output4[i] = FAUSTFLOAT((fRec0[0] * (((9.03435581e-18f * fTemp4) + ((3.01145194e-18f * fTemp2) + (((0.0437162146f * fTemp89) + ((((0.0213084519f * fTemp83) + ((8.03053822e-18f * fTemp86) + ((2.676846e-18f * fTemp84) + ((0.0213084519f * fTemp61) + (0.0213084519f * fTemp69))))) + (0.0213084519f * fTemp88)) + (0.0272319149f * fTemp92))) + (0.0071028173f * fTemp95)))) - ((6.02290387e-18f * fTemp3) + ((0.0213084519f * fTemp82) + ((0.0213084519f * fTemp87) + (((5.35369201e-18f * fTemp64) + (0.0213084519f * fTemp60)) + (0.0213084519f * fTemp70))))))));
			float fTemp96 = (fTemp22 + fTemp25);
			float fTemp97 = ((fTemp19 + fTemp27) - fTemp96);
			float fTemp98 = ((fTemp7 + fTemp18) - (fTemp9 + fTemp16));
			float fTemp99 = ((fTemp37 + fTemp49) - (fTemp41 + fTemp45));
			float fTemp100 = ((fTemp38 + fTemp50) - (fTemp42 + fTemp46));
			float fTemp101 = (fTemp39 - fTemp47);
			float fTemp102 = (fTemp8 - fTemp17);
			float fTemp103 = (fTemp36 - fTemp44);
			float fTemp104 = (fTemp40 - fTemp48);
			float fTemp105 = (fTemp43 - fTemp51);
			float fTemp106 = (fTemp34 - fTemp55);
			output5[i] = FAUSTFLOAT((fRec0[0] * ((((0.0272319149f * fTemp97) + ((0.0437162146f * fTemp98) + ((0.0272319149f * ((fTemp20 + fTemp28) - (fTemp23 + fTemp26))) + ((0.0213084519f * fTemp99) + ((0.0213084519f * fTemp100) + ((0.0071028173f * fTemp101) + ((5.35369201e-18f * fTemp102) + (0.0071028173f * fTemp103)))))))) + (0.0213084519f * ((fTemp32 + fTemp54) - ((fTemp33 + fTemp52) + fTemp53)))) - (((0.0071028173f * fTemp104) + (0.0071028173f * fTemp105)) + (0.0213084519f * fTemp106)))));
			float fTemp107 = ((((fTemp10 + fTemp15) + fTemp16) + fTemp17) + fTemp18);
			float fTemp108 = (fTemp0 + fTemp5);
			float fTemp109 = ((((fTemp35 + fTemp52) + fTemp53) + fTemp54) + fTemp55);
			float fTemp110 = (((fTemp1 + fTemp2) + fTemp3) + fTemp4);
			float fTemp111 = (((fTemp11 + fTemp12) + fTemp13) + fTemp14);
			float fTemp112 = ((((((((fTemp75 + fTemp44) + fTemp45) + fTemp46) + fTemp47) + fTemp48) + fTemp49) + fTemp50) + fTemp51);
			output6[i] = FAUSTFLOAT((((((0.0126197841f * fTemp107) + ((0.0283945147f * fTemp108) + (5.23659549e-18f * fTemp29))) + (0.0328065082f * fTemp109)) - (((0.0141972573f * fTemp110) + (0.0252395682f * fTemp111)) + (0.0164032541f * fTemp112))) * fRec0[0]));
			float fTemp113 = ((fTemp6 + fTemp17) - (fTemp8 + fTemp15));
			float fTemp114 = ((fTemp36 + fTemp47) - (fTemp39 + fTemp44));
			float fTemp115 = ((fTemp30 + fTemp53) - (fTemp31 + fTemp52));
			float fTemp116 = (fTemp42 - fTemp50);
			float fTemp117 = (fTemp37 - fTemp45);
			float fTemp118 = (fTemp23 - fTemp28);
			float fTemp119 = (fTemp7 - fTemp16);
			float fTemp120 = (fTemp38 - fTemp46);
			float fTemp121 = (fTemp20 - fTemp26);
			float fTemp122 = (fTemp9 - fTemp18);
			float fTemp123 = (fTemp41 - fTemp49);
			float fTemp124 = (fTemp33 - fTemp54);
			output7[i] = FAUSTFLOAT((fRec0[0] * (((0.0437162146f * fTemp113) + ((0.0213084519f * fTemp114) + ((0.0213084519f * fTemp115) + ((0.0272319149f * fTemp97) + ((0.0071028173f * fTemp116) + ((0.0071028173f * fTemp117) + ((0.0272319149f * fTemp118) + ((2.676846e-18f * fTemp119) + ((0.0213084519f * fTemp106) + ((3.01145194e-18f * fTemp72) + (0.0213084519f * fTemp105))))))))))) - ((((0.0071028173f * fTemp120) + ((0.0272319149f * fTemp121) + ((8.03053822e-18f * fTemp122) + (0.0213084519f * fTemp104)))) + (0.0071028173f * fTemp123)) + (0.0213084519f * fTemp124)))));
			float fTemp125 = ((fTemp1 + fTemp3) - (fTemp2 + fTemp4));
			float fTemp126 = (fTemp6 + fTemp8);
			float fTemp127 = (fTemp7 + fTemp9);
			float fTemp128 = (((fTemp126 + fTemp15) + fTemp17) - ((fTemp127 + fTemp16) + fTemp18));
			output8[i] = FAUSTFLOAT((fRec0[0] * (((1.33842306e-17f * fTemp13) + ((2.676846e-18f * fTemp11) + ((0.0245903712f * fTemp125) + (((9.21991188e-35f * fTemp108) + (((8.33736929e-18f * fTemp57) + ((1.6674738e-18f * fTemp56) + ((((0.0284112692f * fTemp61) + (0.0284112692f * fTemp60)) + (0.0284112692f * fTemp69)) + (0.0284112692f * fTemp70)))) + (2.17461068e-18f * fTemp67))) + (0.0218581073f * fTemp128))))) - ((1.87379222e-17f * fTemp14) + ((8.03053822e-18f * fTemp12) + ((1.30476637e-18f * fTemp80) + ((1.14222021e-18f * fTemp79) + ((0.0284112692f * fTemp82) + ((0.0284112692f * fTemp88) + ((0.0284112692f * fTemp87) + ((0.0284112692f * fTemp83) + (((5.00242149e-18f * fTemp65) + (1.16723167e-17f * fTemp68)) + (3.04445499e-18f * fTemp71)))))))))))));
			float fTemp129 = (0.0333888084f * fTemp11);
			float fTemp130 = (6.13342471e-18f * fTemp64);
			float fTemp131 = (fTemp84 - fTemp86);
			output9[i] = FAUSTFLOAT((fRec0[0] * (((0.0333888084f * fTemp12) + (fTemp129 + ((9.75821908e-18f * fTemp3) + (((0.0208185371f * fTemp82) + ((0.0208185371f * fTemp88) + ((0.0169820823f * fTemp56) + ((0.0300712213f * fTemp60) + ((fTemp130 + (0.0169820823f * fTemp65)) + (0.0300712213f * fTemp61)))))) + (0.00231317081f * fTemp66))))) - ((0.0333888084f * fTemp14) + ((0.0333888084f * fTemp13) + (((0.00231317081f * fTemp67) + ((0.0208185371f * fTemp87) + ((0.0208185371f * fTemp83) + ((0.0169820823f * fTemp57) + (((((0.0265606362f * fTemp62) + (0.0169820823f * fTemp68)) + (0.0300712213f * fTemp69)) + (0.0300712213f * fTemp70)) + (0.00231317081f * fTemp71)))))) + (0.0166944042f * fTemp131)))))));
			output10[i] = FAUSTFLOAT((fRec0[0] * ((((0.0415974371f * (((fTemp90 + fTemp26) + fTemp28) - ((fTemp91 + fTemp25) + fTemp27))) + ((0.0169982649f * fTemp114) + ((0.0169982649f * fTemp123) + ((0.0169982649f * fTemp117) + ((1.50237622e-17f * fTemp122) + ((5.00792059e-18f * fTemp119) + (0.0169982649f * fTemp104))))))) + (0.0169982649f * (((fTemp93 + fTemp53) + fTemp55) - ((fTemp94 + fTemp52) + fTemp54)))) - ((0.0169982649f * fTemp116) + ((0.0169982649f * fTemp120) + ((1.00158412e-17f * fTemp102) + (0.0169982649f * fTemp105)))))));
			output11[i] = FAUSTFLOAT((fRec0[0] * (((0.0387942903f * fTemp131) + ((0.0131542645f * fTemp58) + ((0.0304601658f * fTemp66) + ((0.00537532335f * fTemp70) + (((4.75093046e-18f * fTemp64) + (0.0131542645f * fTemp65)) + (0.00537532335f * fTemp69)))))) - ((2.51956133e-18f * fTemp3) + ((0.0161259696f * (fTemp83 - fTemp88)) + (((0.0258628614f * (fTemp12 - fTemp14)) + ((0.0304601658f * fTemp67) + ((((((0.0205737799f * fTemp62) + (0.0258628614f * fTemp63)) + (0.0131542645f * fTemp68)) + (0.00537532335f * fTemp61)) + (0.00537532335f * fTemp60)) + (0.0304601658f * fTemp71)))) + (0.0161259696f * (fTemp87 - fTemp82))))))));
			output12[i] = FAUSTFLOAT((fRec0[0] * (((0.0335968435f * fTemp72) + (0.0263335984f * fTemp77)) - (((0.0105584692f * fTemp73) + (0.0214808229f * fTemp74)) + (0.020481687f * fTemp76)))));
			float fTemp132 = ((fTemp6 + fTemp15) - fTemp64);
			output13[i] = FAUSTFLOAT((fRec0[0] * (((0.0258628614f * fTemp12) + ((3.7793419e-18f * fTemp4) + ((0.0387942903f * fTemp132) + ((0.0304601658f * fTemp81) + ((5.03912267e-18f * fTemp108) + ((0.00537532335f * fTemp88) + ((0.00537532335f * fTemp87) + ((0.0131542645f * fTemp56) + ((2.37546523e-18f * fTemp84) + (0.0304601658f * fTemp71)))))))))) - ((0.0258628614f * fTemp14) + ((1.25978067e-18f * fTemp2) + ((0.0131542645f * (fTemp65 - fTemp68)) + ((0.0161259696f * fTemp78) + ((0.0304601658f * fTemp67) + ((0.00537532335f * fTemp82) + ((0.00537532335f * fTemp83) + ((0.0131542645f * fTemp57) + ((7.1263959e-18f * fTemp86) + ((0.0205737799f * fTemp85) + ((0.0258628614f * fTemp63) + (0.0161259696f * fTemp70)))))))))))))));
			float fTemp133 = (fTemp22 - fTemp27);
			float fTemp134 = (fTemp19 - fTemp25);
			float fTemp135 = (fTemp31 - fTemp53);
			float fTemp136 = (fTemp30 - fTemp52);
			output14[i] = FAUSTFLOAT((fRec0[0] * (((0.0408927724f * (((fTemp126 + fTemp16) + fTemp18) - ((fTemp127 + fTemp15) + fTemp17))) + ((0.0226643533f * ((fTemp40 + fTemp49) - (fTemp41 + fTemp48))) + ((1.27355415e-17f * fTemp133) + ((2.54710831e-18f * fTemp134) + (((((2.43935947e-34f * fTemp72) + (0.0226643533f * fTemp103)) + (0.0226643533f * fTemp101)) + (0.0226643533f * fTemp105)) + (5.20421758e-18f * fTemp124)))))) - ((0.0226643533f * (fTemp59 - (fTemp45 + fTemp46))) + ((3.1225305e-18f * fTemp135) + ((2.73352938e-18f * fTemp136) + ((0.0226643533f * fTemp116) + ((1.7829759e-17f * fTemp118) + ((7.64132493e-18f * fTemp121) + (7.28590444e-18f * fTemp106))))))))));
			float fTemp137 = (3.06671236e-18f * fTemp84);
			output15[i] = FAUSTFLOAT((fRec0[0] * (((0.0333888084f * fTemp13) + ((0.0333888084f * fTemp12) + ((1.46373282e-17f * fTemp4) + (((0.00231317081f * fTemp80) + ((0.0f * fTemp108) + (((0.0300712213f * fTemp88) + ((0.0300712213f * fTemp87) + ((0.0169820823f * fTemp57) + ((9.20013769e-18f * fTemp86) + ((0.0265606362f * fTemp85) + (((0.0169820823f * fTemp65) + (0.0208185371f * fTemp61)) + (0.0208185371f * fTemp70))))))) + (0.00231317081f * fTemp67)))) + (0.0166944042f * fTemp132))))) - ((0.0333888084f * fTemp14) + (fTemp129 + ((4.87910954e-18f * fTemp2) + ((0.0169820823f * (((fTemp19 + fTemp23) + fTemp25) + fTemp28)) + ((0.00231317081f * fTemp79) + ((0.0300712213f * fTemp82) + ((0.0300712213f * fTemp83) + (fTemp137 + (((0.0208185371f * fTemp60) + (0.0208185371f * fTemp69)) + (0.00231317081f * fTemp71)))))))))))));
			output16[i] = FAUSTFLOAT((fRec0[0] * (((4.29339759e-17f * fTemp14) + ((3.06671256e-17f * fTemp13) + ((1.84002754e-17f * fTemp12) + ((6.13342471e-18f * fTemp11) + ((0.0355082154f * (fTemp61 - fTemp60)) + (((5.43562818e-19f * fTemp80) + ((9.0593803e-19f * fTemp67) + ((0.0355082154f * fTemp82) + ((1.27355415e-17f * fTemp57) + ((2.54710831e-18f * fTemp56) + (((7.64132493e-18f * fTemp65) + (1.7829759e-17f * fTemp68)) + (1.26831324e-18f * fTemp71))))))) + (0.0355082154f * (fTemp69 - fTemp88)))))))) - ((2.0700309e-17f * fTemp4) + ((1.38002057e-17f * fTemp3) + ((6.90010286e-18f * fTemp2) + ((((9.20013769e-18f * fTemp86) + (fTemp137 + (fTemp130 + (0.0355082154f * fTemp70)))) + (4.75846401e-19f * fTemp79)) + (0.0355082154f * (fTemp83 - fTemp87)))))))));
			output17[i] = FAUSTFLOAT((fRec0[0] * ((((((0.0294138286f * fTemp134) + ((((0.0294138286f * fTemp121) + ((1.30109588e-17f * fTemp102) + (0.0272004437f * fTemp101))) + (0.0188310761f * fTemp123)) + (0.0188310761f * fTemp116))) + (0.00627702521f * fTemp136)) + (0.00627702521f * fTemp135)) + (0.0272004437f * ((fTemp36 + fTemp48) - (fTemp40 + fTemp44)))) - ((0.0354141816f * fTemp98) + ((0.0294138286f * fTemp133) + ((0.00627702521f * fTemp124) + ((0.0188310761f * fTemp120) + ((0.0188310761f * fTemp117) + ((0.0294138286f * fTemp118) + ((0.0272004437f * fTemp105) + (0.00627702521f * fTemp106)))))))))));
			output18[i] = FAUSTFLOAT((fRec0[0] * (((5.21598753e-18f * fTemp3) + ((((0.00671042223f * fTemp82) + ((0.00671042223f * fTemp87) + ((1.73866251e-17f * fTemp86) + ((5.7955417e-18f * fTemp84) + ((0.00671042223f * fTemp60) + (0.00671042223f * fTemp70)))))) + (0.031444706f * fTemp92)) + (0.0290784966f * fTemp95))) - ((7.8239813e-18f * fTemp4) + ((2.60799377e-18f * fTemp2) + ((((0.00671042223f * fTemp83) + (((1.15910834e-17f * fTemp64) + (0.00671042223f * fTemp61)) + (0.00671042223f * fTemp69))) + (0.00671042223f * fTemp88)) + (0.0378593504f * fTemp89)))))));
			output19[i] = FAUSTFLOAT((fRec0[0] * ((((0.0133853024f * fTemp98) + ((0.011117382f * fTemp118) + ((0.010280801f * fTemp105) + ((1.63922672e-18f * fTemp102) + (0.010280801f * fTemp104))))) + (0.035587389f * ((fTemp32 + fTemp55) - ((fTemp34 + fTemp52) + fTemp53)))) - ((0.011117382f * ((fTemp21 + fTemp27) - (fTemp96 + fTemp26))) + ((0.0308424029f * fTemp99) + ((0.0308424029f * fTemp100) + (((0.010280801f * fTemp103) + (0.010280801f * fTemp101)) + (0.035587389f * fTemp124))))))));
			output20[i] = FAUSTFLOAT((((((((0.0142857144f * fTemp110) + (0.0380952395f * fTemp108)) + (0.0253968257f * fTemp111)) + (0.0042514056f * fTemp112)) + (0.0142547125f * fTemp109)) - ((0.0275132284f * fTemp107) + (0.0246093757f * fTemp29))) * fRec0[0]));
			output21[i] = FAUSTFLOAT((fRec0[0] * (((0.0133853024f * fTemp113) + ((0.035587389f * fTemp115) + ((0.011117382f * fTemp133) + ((0.010280801f * fTemp123) + ((0.010280801f * fTemp120) + ((8.19613358e-19f * fTemp119) + (((7.37652042e-18f * fTemp72) + (0.0308424029f * fTemp104)) + (0.035587389f * fTemp106)))))))) - ((0.011117382f * ((fTemp19 + fTemp26) - (fTemp20 + fTemp25))) + ((0.0308424029f * fTemp114) + ((0.035587389f * fTemp124) + ((0.010280801f * fTemp116) + ((0.010280801f * fTemp117) + ((0.011117382f * fTemp118) + ((2.45884007e-18f * fTemp122) + (0.0308424029f * fTemp105)))))))))));
			output22[i] = FAUSTFLOAT((fRec0[0] * (((1.62275168e-17f * fTemp14) + ((6.95465004e-18f * fTemp12) + (((4.79080666e-34f * fTemp108) + (((((((1.92543286e-18f * fTemp56) + (9.62716472e-18f * fTemp57)) + (0.00894722994f * fTemp83)) + (0.00894722994f * fTemp87)) + (0.00894722994f * fTemp88)) + (0.00894722994f * fTemp82)) + (8.90272159e-18f * fTemp67))) + (0.0473241918f * fTemp128)))) - ((1.15910834e-17f * fTemp13) + ((2.31821668e-18f * fTemp11) + ((0.0212958846f * fTemp125) + ((0.00894722994f * (((fTemp36 + fTemp39) + fTemp44) + fTemp47)) + ((5.34163296e-18f * fTemp80) + ((4.67617897e-18f * fTemp79) + (((((5.77629858e-18f * fTemp65) + (1.34780304e-17f * fTemp68)) + (0.00894722994f * fTemp69)) + (0.00894722994f * fTemp70)) + (1.24638111e-17f * fTemp71)))))))))));
			output23[i] = FAUSTFLOAT((fRec0[0] * (((0.0354141816f * fTemp113) + ((0.00627702521f * fTemp135) + ((0.0294138286f * fTemp133) + ((0.00627702521f * fTemp124) + ((0.0272004437f * fTemp123) + ((0.0272004437f * fTemp120) + ((0.0294138286f * fTemp121) + ((1.95164382e-17f * fTemp122) + (((0.0f * fTemp72) + (0.0188310761f * fTemp103)) + (0.0188310761f * fTemp105)))))))))) - ((0.00627702521f * fTemp136) + ((0.0294138286f * fTemp134) + ((0.0272004437f * fTemp116) + ((0.0272004437f * fTemp117) + ((0.0294138286f * fTemp118) + ((6.50547939e-18f * fTemp119) + (((0.0188310761f * fTemp101) + (0.0188310761f * fTemp104)) + (0.00627702521f * fTemp106)))))))))));
			output24[i] = FAUSTFLOAT(((((0.0125208041f * fTemp107) + ((0.0f * fTemp108) + ((0.0281718094f * fTemp110) + ((0.0103565631f * fTemp82) + ((0.0103565631f * fTemp88) + ((0.0103565631f * fTemp87) + ((0.0103565631f * fTemp83) + ((((0.0103565631f * fTemp61) + (0.0103565631f * fTemp60)) + (0.0103565631f * fTemp69)) + (0.0103565631f * fTemp70))))))))) - (((0.0500832163f * fTemp111) + (0.0207987186f * fTemp29)) + (0.00147950905f * fTemp109))) * fRec0[0]));
			fRec0[1] = fRec0[0];
			
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
