/* ------------------------------------------------------------
author: "Pierre Lecomte"
copyright: "(c) Pierre Lecomte 2016"
license: "GPL"
name: "HOARotator3"
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
		m->declare("name", "HOARotator3");
		m->declare("version", "1.0");
	}

	virtual int getNumInputs() {
		return 16;
		
	}
	virtual int getNumOutputs() {
		return 16;
		
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
		ui_interface->openVerticalBox("0x00");
		ui_interface->declare(&fHslider2, "osc", "/picth 0 360");
		ui_interface->addHorizontalSlider("Pitch", &fHslider2, 0.0f, -3.14159274f, 3.14159274f, 0.00999999978f);
		ui_interface->declare(&fHslider1, "osc", "/roll 0 360");
		ui_interface->addHorizontalSlider("Roll", &fHslider1, 0.0f, -3.14159274f, 3.14159274f, 0.00999999978f);
		ui_interface->declare(&fHslider0, "osc", "/yaw 0 360");
		ui_interface->addHorizontalSlider("Yaw", &fHslider0, 0.0f, -3.14159274f, 3.14159274f, 0.00999999978f);
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
		float fSlow0 = float(fHslider0);
		float fSlow1 = cosf(fSlow0);
		float fSlow2 = float(fHslider1);
		float fSlow3 = cosf(fSlow2);
		float fSlow4 = (fSlow1 * fSlow3);
		float fSlow5 = sinf(fSlow0);
		float fSlow6 = sinf(fSlow2);
		float fSlow7 = (fSlow5 * fSlow6);
		float fSlow8 = float(fHslider2);
		float fSlow9 = sinf(fSlow8);
		float fSlow10 = (fSlow4 - (fSlow7 * fSlow9));
		float fSlow11 = cosf(fSlow8);
		float fSlow12 = (0.0f - fSlow11);
		float fSlow13 = (fSlow6 * fSlow12);
		float fSlow14 = (fSlow6 * fSlow9);
		float fSlow15 = (fSlow5 * fSlow3);
		float fSlow16 = ((fSlow14 * fSlow1) + fSlow15);
		float fSlow17 = (fSlow6 * fSlow1);
		float fSlow18 = (fSlow5 * fSlow9);
		float fSlow19 = (fSlow17 + (fSlow18 * fSlow3));
		float fSlow20 = (fSlow3 * fSlow11);
		float fSlow21 = (fSlow9 * fSlow1);
		float fSlow22 = ((fSlow21 * (0.0f - fSlow3)) + fSlow7);
		float fSlow23 = (fSlow5 * fSlow12);
		float fSlow24 = (fSlow1 * fSlow11);
		float fSlow25 = ((fSlow24 * fSlow10) + (fSlow23 * fSlow16));
		float fSlow26 = faustpower2_f(fSlow12);
		float fSlow27 = ((fSlow9 * fSlow10) + (fSlow7 * fSlow26));
		float fSlow28 = (1.73205078f * (fSlow14 * fSlow12));
		float fSlow29 = ((fSlow9 * fSlow16) + ((fSlow17 * fSlow11) * fSlow12));
		float fSlow30 = ((fSlow24 * fSlow16) - (fSlow23 * fSlow10));
		float fSlow31 = ((fSlow22 * fSlow10) + (fSlow19 * fSlow16));
		float fSlow32 = ((1.0f * ((fSlow6 * fSlow19) * fSlow12)) + (fSlow20 * fSlow10));
		float fSlow33 = (1.73205078f * (((fSlow6 * fSlow3) * fSlow11) * fSlow12));
		float fSlow34 = ((fSlow20 * fSlow16) + (1.0f * ((fSlow6 * fSlow22) * fSlow12)));
		float fSlow35 = ((fSlow22 * fSlow16) - (fSlow19 * fSlow10));
		float fSlow36 = (((fSlow5 * fSlow1) * fSlow11) * fSlow12);
		float fSlow37 = (fSlow16 * fSlow10);
		float fSlow38 = ((1.15470052f * (fSlow22 * fSlow19)) - (0.577350259f * (fSlow36 + fSlow37)));
		float fSlow39 = (fSlow6 * fSlow10);
		float fSlow40 = ((1.15470052f * ((fSlow3 * fSlow19) * fSlow11)) - (0.577350259f * (fSlow12 * (fSlow18 + fSlow39))));
		float fSlow41 = faustpower2_f(fSlow11);
		float fSlow42 = (faustpower2_f(fSlow3) * fSlow41);
		float fSlow43 = faustpower2_f(fSlow9);
		float fSlow44 = (faustpower2_f(fSlow6) * fSlow26);
		float fSlow45 = (fSlow43 + fSlow44);
		float fSlow46 = (fSlow42 - (0.5f * fSlow45));
		float fSlow47 = (fSlow21 * fSlow11);
		float fSlow48 = (fSlow13 * fSlow16);
		float fSlow49 = ((1.15470052f * ((fSlow3 * fSlow22) * fSlow11)) - (0.577350259f * (fSlow47 + fSlow48)));
		float fSlow50 = (faustpower2_f(fSlow1) * fSlow41);
		float fSlow51 = faustpower2_f(fSlow16);
		float fSlow52 = (faustpower2_f(fSlow5) * fSlow26);
		float fSlow53 = faustpower2_f(fSlow10);
		float fSlow54 = ((0.577350259f * (faustpower2_f(fSlow22) - faustpower2_f(fSlow19))) - (0.288675129f * ((fSlow50 + fSlow51) - (fSlow52 + fSlow53))));
		float fSlow55 = (((fSlow5 * fSlow22) * fSlow12) + ((fSlow1 * fSlow19) * fSlow11));
		float fSlow56 = ((1.0f * (fSlow9 * fSlow19)) + ((fSlow15 * fSlow11) * fSlow12));
		float fSlow57 = (1.73205078f * ((fSlow9 * fSlow3) * fSlow11));
		float fSlow58 = ((fSlow4 * fSlow41) + (1.0f * (fSlow9 * fSlow22)));
		float fSlow59 = (((fSlow1 * fSlow22) * fSlow11) - ((fSlow5 * fSlow19) * fSlow12));
		float fSlow60 = (fSlow36 - fSlow37);
		float fSlow61 = (fSlow18 - fSlow39);
		float fSlow62 = (fSlow12 * fSlow61);
		float fSlow63 = (fSlow43 - fSlow44);
		float fSlow64 = (0.866025388f * fSlow63);
		float fSlow65 = (fSlow47 - fSlow48);
		float fSlow66 = ((fSlow50 + fSlow53) - (fSlow52 + fSlow51));
		float fSlow67 = (0.5f * fSlow66);
		float fSlow68 = ((fSlow24 * fSlow25) + (fSlow23 * fSlow30));
		float fSlow69 = (fSlow16 * fSlow60);
		float fSlow70 = (0.5f * (fSlow10 * fSlow66));
		float fSlow71 = ((fSlow68 + fSlow69) + fSlow70);
		float fSlow72 = (fSlow24 * fSlow30);
		float fSlow73 = (0.5f * (fSlow16 * fSlow66));
		float fSlow74 = (fSlow23 * fSlow25);
		float fSlow75 = (fSlow10 * fSlow60);
		float fSlow76 = ((fSlow72 + fSlow73) - (fSlow74 + fSlow75));
		float fSlow77 = (fSlow9 * fSlow25);
		float fSlow78 = (fSlow13 * fSlow60);
		float fSlow79 = (fSlow77 + fSlow78);
		float fSlow80 = (fSlow9 * fSlow30);
		float fSlow81 = (0.5f * (fSlow13 * fSlow66));
		float fSlow82 = (fSlow80 + fSlow81);
		float fSlow83 = (1.73205078f * fSlow43);
		float fSlow84 = (0.912870944f * (((fSlow83 + fSlow64) * fSlow6) * fSlow12));
		float fSlow85 = (fSlow9 * fSlow27);
		float fSlow86 = ((fSlow6 * fSlow26) * fSlow61);
		float fSlow87 = (fSlow85 + fSlow86);
		float fSlow88 = (fSlow9 * fSlow29);
		float fSlow89 = (fSlow13 * fSlow65);
		float fSlow90 = (fSlow88 + fSlow89);
		float fSlow91 = (((fSlow20 * fSlow27) + (fSlow32 * fSlow9)) + ((fSlow56 * fSlow6) * fSlow12));
		float fSlow92 = (((fSlow20 * fSlow29) + (fSlow9 * fSlow34)) + (fSlow13 * fSlow58));
		float fSlow93 = ((((((fSlow22 * fSlow25) + (fSlow19 * fSlow30)) + (fSlow24 * fSlow31)) + (fSlow23 * fSlow35)) + (fSlow16 * fSlow55)) + (fSlow10 * fSlow59));
		float fSlow94 = ((((fSlow22 * fSlow30) + (fSlow24 * fSlow35)) + (fSlow16 * fSlow59)) - (((fSlow19 * fSlow25) + (fSlow23 * fSlow31)) + (fSlow10 * fSlow55)));
		float fSlow95 = ((fSlow13 * fSlow59) + ((fSlow20 * fSlow30) + (fSlow9 * fSlow35)));
		float fSlow96 = ((fSlow13 * fSlow55) + ((fSlow20 * fSlow25) + (fSlow9 * fSlow31)));
		float fSlow97 = (3.87298346f * (((fSlow14 * fSlow3) * fSlow11) * fSlow12));
		float fSlow98 = (((0.516397774f * ((fSlow22 * fSlow31) + (fSlow19 * fSlow35))) + (0.44721359f * ((fSlow16 * fSlow38) + (fSlow10 * fSlow54)))) - (0.129099444f * (fSlow68 - (fSlow69 + fSlow70))));
		float fSlow99 = (((1.26491106f * (fSlow20 * fSlow31)) + (1.09544516f * (fSlow13 * fSlow38))) - (0.316227764f * (fSlow77 - fSlow78)));
		float fSlow100 = ((((fSlow32 * fSlow3) * fSlow11) + (0.866025388f * (fSlow13 * fSlow40))) - (0.25f * (fSlow85 - fSlow86)));
		float fSlow101 = ((1.63299322f * fSlow42) + (0.816496611f * fSlow46));
		float fSlow102 = (((fSlow101 - (0.235702261f * (fSlow83 - fSlow64))) * fSlow6) * fSlow12);
		float fSlow103 = (((fSlow20 * fSlow34) + (0.866025388f * (fSlow13 * fSlow49))) - (0.25f * (fSlow88 - fSlow89)));
		float fSlow104 = (((1.26491106f * (fSlow20 * fSlow35)) + (1.09544516f * (fSlow13 * fSlow54))) - (0.316227764f * (fSlow80 - fSlow81)));
		float fSlow105 = (((0.516397774f * ((fSlow22 * fSlow35) - (fSlow19 * fSlow31))) + (0.44721359f * ((fSlow16 * fSlow54) - (fSlow10 * fSlow38)))) - (0.129099444f * ((fSlow72 + fSlow75) - (fSlow74 + fSlow73))));
		float fSlow106 = (((fSlow42 - (1.5f * fSlow45)) * fSlow3) * fSlow11);
		float fSlow107 = (fSlow24 * fSlow59);
		float fSlow108 = (fSlow16 * fSlow35);
		float fSlow109 = (fSlow23 * fSlow55);
		float fSlow110 = (fSlow10 * fSlow31);
		float fSlow111 = ((0.547722578f * ((fSlow22 * fSlow54) - (fSlow19 * fSlow38))) - (0.316227764f * ((fSlow107 + fSlow108) - (fSlow109 + fSlow110))));
		float fSlow112 = (fSlow9 * fSlow59);
		float fSlow113 = (fSlow13 * fSlow35);
		float fSlow114 = ((1.34164083f * (fSlow20 * fSlow54)) - (0.774596691f * (fSlow112 + fSlow113)));
		float fSlow115 = (fSlow10 * fSlow35);
		float fSlow116 = (fSlow24 * fSlow55);
		float fSlow117 = (fSlow23 * fSlow59);
		float fSlow118 = (fSlow16 * fSlow31);
		float fSlow119 = ((0.547722578f * ((fSlow22 * fSlow38) + (fSlow19 * fSlow54))) - (0.316227764f * (fSlow115 + ((fSlow116 + fSlow117) + fSlow118))));
		float fSlow120 = (fSlow9 * fSlow55);
		float fSlow121 = (fSlow13 * fSlow31);
		float fSlow122 = ((1.34164083f * (fSlow20 * fSlow38)) - (0.774596691f * (fSlow120 + fSlow121)));
		float fSlow123 = (fSlow56 * fSlow9);
		float fSlow124 = (fSlow32 * fSlow6);
		float fSlow125 = ((1.06066012f * (fSlow20 * fSlow40)) - (0.612372458f * (fSlow123 + (fSlow124 * fSlow12))));
		float fSlow126 = (fSlow9 * fSlow58);
		float fSlow127 = (fSlow13 * fSlow34);
		float fSlow128 = ((1.06066012f * (fSlow20 * fSlow49)) - (0.612372458f * (fSlow126 + fSlow127)));
		float fSlow129 = ((fSlow24 * fSlow60) + (0.5f * (fSlow23 * fSlow66)));
		float fSlow130 = (fSlow16 * fSlow25);
		float fSlow131 = (fSlow10 * fSlow30);
		float fSlow132 = (((0.516397774f * ((fSlow22 * fSlow55) + (fSlow19 * fSlow59))) + (0.44721359f * ((fSlow24 * fSlow38) + (fSlow23 * fSlow54)))) - (0.129099444f * ((fSlow129 + fSlow130) + fSlow131)));
		float fSlow133 = (fSlow9 * fSlow60);
		float fSlow134 = (fSlow13 * fSlow25);
		float fSlow135 = (((1.26491106f * (fSlow20 * fSlow55)) + (1.09544516f * (fSlow9 * fSlow38))) - (0.316227764f * (fSlow133 + fSlow134)));
		float fSlow136 = (fSlow9 * fSlow61);
		float fSlow137 = (fSlow6 * fSlow27);
		float fSlow138 = ((((fSlow56 * fSlow3) * fSlow11) + (0.866025388f * (fSlow9 * fSlow40))) - (0.25f * ((fSlow136 + fSlow137) * fSlow12)));
		float fSlow139 = (1.73205078f * fSlow44);
		float fSlow140 = ((fSlow101 - (0.235702261f * (fSlow64 + fSlow139))) * fSlow9);
		float fSlow141 = (fSlow9 * fSlow65);
		float fSlow142 = (fSlow13 * fSlow29);
		float fSlow143 = (((fSlow20 * fSlow58) + (0.866025388f * (fSlow9 * fSlow49))) - (0.25f * (fSlow141 + fSlow142)));
		float fSlow144 = (0.5f * (fSlow9 * fSlow66));
		float fSlow145 = (fSlow13 * fSlow30);
		float fSlow146 = (((1.26491106f * (fSlow20 * fSlow59)) + (1.09544516f * (fSlow9 * fSlow54))) - (0.316227764f * (fSlow144 + fSlow145)));
		float fSlow147 = (0.5f * (fSlow24 * fSlow66));
		float fSlow148 = (fSlow16 * fSlow30);
		float fSlow149 = (fSlow23 * fSlow60);
		float fSlow150 = (fSlow10 * fSlow25);
		float fSlow151 = (((0.516397774f * ((fSlow22 * fSlow59) - (fSlow19 * fSlow55))) + (0.44721359f * ((fSlow24 * fSlow54) - (fSlow23 * fSlow38)))) - (0.129099444f * ((fSlow147 + fSlow148) - (fSlow149 + fSlow150))));
		float fSlow152 = ((((fSlow20 * fSlow61) - fSlow124) * fSlow12) + fSlow123);
		float fSlow153 = (((fSlow20 * fSlow65) + fSlow126) - fSlow127);
		float fSlow154 = (((((fSlow22 * fSlow60) + (0.5f * (fSlow19 * fSlow66))) + fSlow116) + fSlow117) - (fSlow118 + fSlow115));
		float fSlow155 = ((((0.5f * (fSlow22 * fSlow66)) + fSlow107) + fSlow110) - (((fSlow19 * fSlow60) + fSlow109) + fSlow108));
		float fSlow156 = (((0.5f * (fSlow20 * fSlow66)) + fSlow112) - fSlow113);
		float fSlow157 = (((fSlow20 * fSlow60) + fSlow120) - fSlow121);
		float fSlow158 = (1.93649173f * (fSlow20 * fSlow63));
		float fSlow159 = (fSlow129 - (fSlow130 + fSlow131));
		float fSlow160 = ((fSlow147 + fSlow150) - (fSlow149 + fSlow148));
		float fSlow161 = (fSlow133 - fSlow134);
		float fSlow162 = (fSlow144 - fSlow145);
		float fSlow163 = (0.912870944f * (fSlow9 * (fSlow64 - fSlow139)));
		float fSlow164 = (fSlow12 * (fSlow136 - fSlow137));
		float fSlow165 = (fSlow141 - fSlow142);
		for (int i = 0; (i < count); i = (i + 1)) {
			output0[i] = FAUSTFLOAT(float(input0[i]));
			float fTemp0 = float(input1[i]);
			float fTemp1 = float(input2[i]);
			float fTemp2 = float(input3[i]);
			output1[i] = FAUSTFLOAT((((fSlow10 * fTemp0) + (fSlow13 * fTemp1)) + (fSlow16 * fTemp2)));
			output2[i] = FAUSTFLOAT((((fSlow19 * fTemp0) + (fSlow20 * fTemp1)) + (fSlow22 * fTemp2)));
			output3[i] = FAUSTFLOAT((((fSlow23 * fTemp0) + (fSlow9 * fTemp1)) + (fSlow24 * fTemp2)));
			float fTemp3 = float(input4[i]);
			float fTemp4 = float(input5[i]);
			float fTemp5 = float(input6[i]);
			float fTemp6 = float(input7[i]);
			float fTemp7 = float(input8[i]);
			output4[i] = FAUSTFLOAT((((((fSlow25 * fTemp3) + (fSlow27 * fTemp4)) + (fSlow28 * fTemp5)) + (fSlow29 * fTemp6)) + (fSlow30 * fTemp7)));
			output5[i] = FAUSTFLOAT((((((fSlow31 * fTemp3) + (fSlow32 * fTemp4)) + (fSlow33 * fTemp5)) + (fSlow34 * fTemp6)) + (fSlow35 * fTemp7)));
			output6[i] = FAUSTFLOAT((((((fSlow38 * fTemp3) + (fSlow40 * fTemp4)) + (fSlow46 * fTemp5)) + (fSlow49 * fTemp6)) + (fSlow54 * fTemp7)));
			output7[i] = FAUSTFLOAT((((((fSlow55 * fTemp3) + (fSlow56 * fTemp4)) + (fSlow57 * fTemp5)) + (fSlow58 * fTemp6)) + (fSlow59 * fTemp7)));
			output8[i] = FAUSTFLOAT((((((fSlow60 * fTemp3) + (fSlow62 * fTemp4)) + (fSlow64 * fTemp5)) + (fSlow65 * fTemp6)) + (fSlow67 * fTemp7)));
			float fTemp8 = float(input9[i]);
			float fTemp9 = float(input15[i]);
			float fTemp10 = float(input10[i]);
			float fTemp11 = float(input14[i]);
			float fTemp12 = float(input12[i]);
			float fTemp13 = float(input11[i]);
			float fTemp14 = float(input13[i]);
			output9[i] = FAUSTFLOAT(((0.5f * ((fSlow71 * fTemp8) + (fSlow76 * fTemp9))) + ((1.22474492f * ((fSlow79 * fTemp10) + (fSlow82 * fTemp11))) + ((fSlow84 * fTemp12) + (0.968245864f * ((fSlow87 * fTemp13) + (fSlow90 * fTemp14)))))));
			output10[i] = FAUSTFLOAT(((0.790569425f * ((fSlow91 * fTemp13) + (fSlow92 * fTemp14))) + ((0.408248305f * ((fSlow93 * fTemp8) + (fSlow94 * fTemp9))) + ((fSlow95 * fTemp11) + ((fSlow96 * fTemp10) + (fSlow97 * fTemp12))))));
			output11[i] = FAUSTFLOAT((((((((fSlow98 * fTemp8) + (fSlow99 * fTemp10)) + (fSlow100 * fTemp13)) + (fSlow102 * fTemp12)) + (fSlow103 * fTemp14)) + (fSlow104 * fTemp11)) + (fSlow105 * fTemp9)));
			output12[i] = FAUSTFLOAT(((fSlow106 * fTemp12) + ((fSlow111 * fTemp9) + ((fSlow114 * fTemp11) + ((((fSlow119 * fTemp8) + (fSlow122 * fTemp10)) + (fSlow125 * fTemp13)) + (fSlow128 * fTemp14))))));
			output13[i] = FAUSTFLOAT((((((((fSlow132 * fTemp8) + (fSlow135 * fTemp10)) + (fSlow138 * fTemp13)) + (fSlow140 * fTemp12)) + (fSlow143 * fTemp14)) + (fSlow146 * fTemp11)) + (fSlow151 * fTemp9)));
			output14[i] = FAUSTFLOAT(((0.790569425f * ((fSlow152 * fTemp13) + (fSlow153 * fTemp14))) + ((0.408248305f * ((fSlow154 * fTemp8) + (fSlow155 * fTemp9))) + ((fSlow156 * fTemp11) + ((fSlow157 * fTemp10) + (fSlow158 * fTemp12))))));
			output15[i] = FAUSTFLOAT(((0.5f * ((fSlow159 * fTemp8) + (fSlow160 * fTemp9))) + ((1.22474492f * ((fSlow161 * fTemp10) + (fSlow162 * fTemp11))) + ((fSlow163 * fTemp12) + (0.968245864f * ((fSlow164 * fTemp13) + (fSlow165 * fTemp14)))))));
			
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
