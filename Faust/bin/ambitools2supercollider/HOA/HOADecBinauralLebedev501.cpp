/* ------------------------------------------------------------
author: "Pierre Lecomte"
copyright: "(c) Pierre Lecomte 2015"
license: "GPL)"
name: "HOADecBinauralLebedev501"
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


#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
#endif

class mydsp : public dsp {
	
 private:
	
	int IOTA;
	int fSamplingFreq;
	float fVec0[128];
	float fVec1[128];
	float fVec2[128];
	float fVec3[128];
	float fRec1[2];
	float fRec3[2];
	float fRec2[2];
	float fRec4[2];
	float fRec5[2];
	float fRec6[2];
	float fRec0[2];
	float fRec7[2];
	float fConst0;
	FAUSTFLOAT fVslider0;
	FAUSTFLOAT fVslider1;
	FAUSTFLOAT fVbargraph0;
	FAUSTFLOAT fVbargraph1;
	FAUSTFLOAT fVbargraph2;
	FAUSTFLOAT fVbargraph3;
	FAUSTFLOAT fHbargraph0;
	FAUSTFLOAT fHbargraph1;
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("author", "Pierre Lecomte");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.0");
		m->declare("copyright", "(c) Pierre Lecomte 2015");
		m->declare("filters.lib/name", "Faust Filter Library");
		m->declare("filters.lib/version", "2.0");
		m->declare("lib/gui.lib/author", "Pierre Lecomte");
		m->declare("lib/gui.lib/copyright", "(c) Pierre Lecomte 2016");
		m->declare("lib/gui.lib/license", "GPL");
		m->declare("lib/gui.lib/name", "GUI Library");
		m->declare("lib/gui.lib/version", "1.0");
		m->declare("license", "GPL)");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.0");
		m->declare("name", "HOADecBinauralLebedev501");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
		m->declare("version", "1.0");
	}

	virtual int getNumInputs() {
		return 4;
		
	}
	virtual int getNumOutputs() {
		return 2;
		
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
		fConst0 = (80.0f / min(192000.0f, max(1.0f, float(fSamplingFreq))));
		
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(0.0f);
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
		IOTA = 0;
		for (int i3 = 0; (i3 < 128); i3 = (i3 + 1)) {
			fVec0[i3] = 0.0f;
			
		}
		for (int i4 = 0; (i4 < 2); i4 = (i4 + 1)) {
			fRec4[i4] = 0.0f;
			
		}
		for (int i5 = 0; (i5 < 128); i5 = (i5 + 1)) {
			fVec1[i5] = 0.0f;
			
		}
		for (int i6 = 0; (i6 < 2); i6 = (i6 + 1)) {
			fRec5[i6] = 0.0f;
			
		}
		for (int i7 = 0; (i7 < 128); i7 = (i7 + 1)) {
			fVec2[i7] = 0.0f;
			
		}
		for (int i8 = 0; (i8 < 2); i8 = (i8 + 1)) {
			fRec6[i8] = 0.0f;
			
		}
		for (int i9 = 0; (i9 < 128); i9 = (i9 + 1)) {
			fVec3[i9] = 0.0f;
			
		}
		for (int i10 = 0; (i10 < 2); i10 = (i10 + 1)) {
			fRec0[i10] = 0.0f;
			
		}
		for (int i11 = 0; (i11 < 2); i11 = (i11 + 1)) {
			fRec7[i11] = 0.0f;
			
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
		ui_interface->openHorizontalBox("0");
		ui_interface->openVerticalBox("0");
		ui_interface->declare(&fVbargraph0, "unit", "dB");
		ui_interface->addVerticalBargraph("0x422e890", &fVbargraph0, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("1");
		ui_interface->openVerticalBox("1");
		ui_interface->declare(&fVbargraph1, "unit", "dB");
		ui_interface->addVerticalBargraph("0x427dcf0", &fVbargraph1, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("2");
		ui_interface->declare(&fVbargraph2, "unit", "dB");
		ui_interface->addVerticalBargraph("0x4298aa0", &fVbargraph2, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("3");
		ui_interface->declare(&fVbargraph3, "unit", "dB");
		ui_interface->addVerticalBargraph("0x42c7fc0", &fVbargraph3, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->declare(&fVslider1, "1", "");
		ui_interface->declare(&fVslider1, "osc", "/levelin -10 10");
		ui_interface->declare(&fVslider1, "unit", "dB");
		ui_interface->addVerticalSlider("Inputs Gain", &fVslider1, 0.0f, -10.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider0, "2", "");
		ui_interface->declare(&fVslider0, "osc", "/levelout -10 10");
		ui_interface->declare(&fVslider0, "unit", "dB");
		ui_interface->addVerticalSlider("Outputs Gain", &fVslider0, 0.0f, -10.0f, 10.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("Outputs");
		ui_interface->openHorizontalBox("Left");
		ui_interface->declare(&fHbargraph0, "unit", "dB");
		ui_interface->addHorizontalBargraph("0x4397e40", &fHbargraph0, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Right");
		ui_interface->declare(&fHbargraph1, "unit", "dB");
		ui_interface->addHorizontalBargraph("0x444d3c0", &fHbargraph1, -70.0f, 6.0f);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->closeBox();
		
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* input2 = inputs[2];
		FAUSTFLOAT* input3 = inputs[3];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = (0.00100000005f * powf(10.0f, (0.0500000007f * float(fVslider0))));
		float fSlow1 = (0.00100000005f * powf(10.0f, (0.0500000007f * float(fVslider1))));
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec1[0] = (fSlow0 + (0.999000013f * fRec1[1]));
			fRec3[0] = (fSlow1 + (0.999000013f * fRec3[1]));
			float fTemp0 = (fRec3[0] * float(input0[i]));
			fRec2[0] = max((fRec2[1] - fConst0), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp0))))));
			fVbargraph0 = FAUSTFLOAT(fRec2[0]);
			fVec0[(IOTA & 127)] = fTemp0;
			float fTemp1 = (fRec3[0] * float(input1[i]));
			fRec4[0] = max((fRec4[1] - fConst0), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp1))))));
			fVbargraph1 = FAUSTFLOAT(fRec4[0]);
			fVec1[(IOTA & 127)] = fTemp1;
			float fTemp2 = (fRec3[0] * float(input2[i]));
			fRec5[0] = max((fRec5[1] - fConst0), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp2))))));
			fVbargraph2 = FAUSTFLOAT(fRec5[0]);
			fVec2[(IOTA & 127)] = fTemp2;
			float fTemp3 = (fRec3[0] * float(input3[i]));
			fRec6[0] = max((fRec6[1] - fConst0), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp3))))));
			fVbargraph3 = FAUSTFLOAT(fRec6[0]);
			fVec3[(IOTA & 127)] = fTemp3;
			float fTemp4 = (fRec1[0] * (((2.06253003e-06f * fVec0[((IOTA - 126) & 127)]) + ((5.0813901e-06f * fVec0[((IOTA - 125) & 127)]) + ((5.71260989e-06f * fVec0[((IOTA - 124) & 127)]) + ((2.09786995e-05f * fVec0[((IOTA - 123) & 127)]) + ((1.87178994e-05f * fVec0[((IOTA - 122) & 127)]) + ((4.25413018e-05f * fVec0[((IOTA - 121) & 127)]) + ((0.000106546002f * fVec0[((IOTA - 120) & 127)]) + ((0.000164044002f * fVec0[((IOTA - 119) & 127)]) + ((0.000168725004f * fVec0[((IOTA - 118) & 127)]) + ((0.000258552987f * fVec0[((IOTA - 117) & 127)]) + ((0.000341600011f * fVec0[((IOTA - 116) & 127)]) + ((0.000453824003f * fVec0[((IOTA - 115) & 127)]) + ((0.000547365984f * fVec0[((IOTA - 114) & 127)]) + ((0.000533583981f * fVec0[((IOTA - 113) & 127)]) + ((0.000501019997f * fVec0[((IOTA - 112) & 127)]) + ((0.000676256022f * fVec0[((IOTA - 111) & 127)]) + ((0.000981663004f * fVec0[((IOTA - 110) & 127)]) + ((0.000834515027f * fVec0[((IOTA - 109) & 127)]) + ((0.000943651015f * fVec0[((IOTA - 108) & 127)]) + ((0.00129229994f * fVec0[((IOTA - 107) & 127)]) + ((0.00136699004f * fVec0[((IOTA - 106) & 127)]) + ((0.00140592002f * fVec0[((IOTA - 105) & 127)]) + ((0.00120654004f * fVec0[((IOTA - 104) & 127)]) + ((0.00185348f * fVec0[((IOTA - 103) & 127)]) + ((0.00154024002f * fVec0[((IOTA - 102) & 127)]) + ((0.000877276005f * fVec0[((IOTA - 101) & 127)]) + ((0.000506834011f * fVec0[((IOTA - 100) & 127)]) + ((0.000612824981f * fVec0[((IOTA - 99) & 127)]) + ((0.000584702007f * fVec0[((IOTA - 98) & 127)]) + ((0.000548739976f * fVec0[((IOTA - 96) & 127)]) + ((0.00272944989f * fVec0[((IOTA - 95) & 127)]) + ((0.00214614999f * fVec0[((IOTA - 94) & 127)]) + ((2.39550009e-05f * fVec0[((IOTA - 93) & 127)]) + ((0.000413299014f * fVec0[((IOTA - 90) & 127)]) + ((0.00370830996f * fVec0[((IOTA - 87) & 127)]) + ((0.00517576002f * fVec0[((IOTA - 86) & 127)]) + ((0.00167995004f * fVec0[((IOTA - 85) & 127)]) + ((0.00157017005f * fVec0[((IOTA - 83) & 127)]) + ((0.00056369201f * fVec0[((IOTA - 82) & 127)]) + ((0.00243653008f * fVec0[((IOTA - 80) & 127)]) + ((0.00419051992f * fVec0[((IOTA - 79) & 127)]) + ((0.00244050007f * fVec0[((IOTA - 78) & 127)]) + ((0.000828476972f * fVec0[((IOTA - 77) & 127)]) + ((0.00419458002f * fVec0[((IOTA - 76) & 127)]) + ((0.00637430977f * fVec0[((IOTA - 75) & 127)]) + ((0.00291290996f * fVec0[((IOTA - 74) & 127)]) + ((0.00308689009f * fVec0[((IOTA - 73) & 127)]) + ((0.00523563009f * fVec0[((IOTA - 72) & 127)]) + ((0.00321074994f * fVec0[((IOTA - 71) & 127)]) + ((0.00185048999f * fVec0[((IOTA - 70) & 127)]) + ((0.00146469998f * fVec0[((IOTA - 69) & 127)]) + ((0.0022559301f * fVec0[((IOTA - 68) & 127)]) + ((0.0034284601f * fVec0[((IOTA - 67) & 127)]) + ((0.0306246001f * fVec0[((IOTA - 27) & 127)]) + ((0.0528391004f * fVec0[((IOTA - 26) & 127)]) + ((0.0317463018f * fVec0[((IOTA - 25) & 127)]) + ((0.0212609004f * fVec0[((IOTA - 23) & 127)]) + ((0.0968950987f * fVec0[((IOTA - 22) & 127)]) + ((0.0578092001f * fVec0[((IOTA - 21) & 127)]) + ((0.0659684017f * fVec0[((IOTA - 20) & 127)]) + ((0.0557724983f * fVec0[((IOTA - 19) & 127)]) + ((0.0345147997f * fVec0[((IOTA - 18) & 127)]) + ((0.0734950975f * fVec0[((IOTA - 17) & 127)]) + ((0.0596065f * fVec0[((IOTA - 16) & 127)]) + ((0.0647490993f * fVec0[((IOTA - 15) & 127)]) + ((0.0449073985f * fVec0[((IOTA - 14) & 127)]) + ((0.0493423007f * fVec0[((IOTA - 13) & 127)]) + ((0.0565104f * fVec0[((IOTA - 12) & 127)]) + ((0.00363026001f * fVec0[((IOTA - 11) & 127)]) + ((0.000508795027f * fVec0[((IOTA - 10) & 127)]) + ((0.00115892f * fVec0[((IOTA - 9) & 127)]) + ((0.000249570992f * fVec0[((IOTA - 8) & 127)]) + ((0.00186342001f * fVec0[((IOTA - 7) & 127)]) + ((0.000141179f * fVec0[((IOTA - 6) & 127)]) + ((0.000767728023f * fVec0[((IOTA - 5) & 127)]) + ((0.000599490013f * fVec0[((IOTA - 4) & 127)]) + ((0.000968383974f * fVec0[((IOTA - 3) & 127)]) + ((0.000123708f * fVec0[((IOTA - 1) & 127)]) + ((0.00870641042f * fVec0[((IOTA - 29) & 127)]) + ((0.0266261008f * fVec0[((IOTA - 30) & 127)]) + ((0.0113663003f * fVec0[((IOTA - 31) & 127)]) + ((0.00570825022f * fVec0[((IOTA - 32) & 127)]) + ((0.0230020005f * fVec0[((IOTA - 33) & 127)]) + ((0.0200235993f * fVec0[((IOTA - 34) & 127)]) + ((0.00446806988f * fVec0[((IOTA - 35) & 127)]) + ((0.0141227003f * fVec0[((IOTA - 36) & 127)]) + ((0.00798119977f * fVec0[((IOTA - 37) & 127)]) + ((0.00693019014f * fVec0[((IOTA - 38) & 127)]) + ((0.0115890997f * fVec0[((IOTA - 39) & 127)]) + ((0.00649730023f * fVec0[((IOTA - 40) & 127)]) + ((0.00569457002f * fVec0[((IOTA - 41) & 127)]) + ((0.00743298978f * fVec0[((IOTA - 42) & 127)]) + ((0.00288578006f * fVec0[((IOTA - 43) & 127)]) + ((0.0079248203f * fVec0[((IOTA - 44) & 127)]) + ((0.0150057999f * fVec0[((IOTA - 45) & 127)]) + ((0.00250453991f * fVec0[((IOTA - 46) & 127)]) + ((0.0030678499f * fVec0[((IOTA - 48) & 127)]) + ((0.00175234f * fVec0[((IOTA - 49) & 127)]) + ((0.000863770023f * fVec0[((IOTA - 50) & 127)]) + ((0.00366525003f * fVec0[((IOTA - 51) & 127)]) + ((0.00288340007f * fVec0[((IOTA - 52) & 127)]) + ((0.00325793005f * fVec0[((IOTA - 53) & 127)]) + ((0.00305273011f * fVec0[((IOTA - 54) & 127)]) + ((0.00124616001f * fVec0[((IOTA - 55) & 127)]) + ((0.00121184997f * fVec0[((IOTA - 56) & 127)]) + ((0.00500281993f * fVec0[((IOTA - 57) & 127)]) + ((0.00361292996f * fVec0[((IOTA - 58) & 127)]) + ((0.00312119001f * fVec0[((IOTA - 59) & 127)]) + ((0.00422505988f * fVec0[((IOTA - 60) & 127)]) + ((0.00357752992f * fVec0[((IOTA - 61) & 127)]) + ((0.00680406997f * fVec0[((IOTA - 62) & 127)]) + ((0.0062385099f * fVec0[((IOTA - 63) & 127)]) + ((0.00421530008f * fVec0[((IOTA - 64) & 127)]) + ((0.00274035009f * fVec0[((IOTA - 65) & 127)]) + ((0.00230892003f * fVec0[((IOTA - 66) & 127)]) + ((2.51181991e-05f * fTemp0) + ((0.000154690002f * fVec1[((IOTA - 1) & 127)]) + ((0.00122990995f * fVec1[((IOTA - 3) & 127)]) + ((0.000586903014f * fVec1[((IOTA - 4) & 127)]) + ((0.00114688999f * fVec1[((IOTA - 5) & 127)]) + ((0.00194176997f * fVec1[((IOTA - 7) & 127)]) + ((0.00181103998f * fVec1[((IOTA - 9) & 127)]) + ((0.00545811979f * fVec1[((IOTA - 11) & 127)]) + ((0.090551503f * fVec1[((IOTA - 12) & 127)]) + ((0.0771318972f * fVec1[((IOTA - 13) & 127)]) + ((0.0608862005f * fVec1[((IOTA - 14) & 127)]) + ((0.0781434029f * fVec1[((IOTA - 15) & 127)]) + ((0.0636584014f * fVec1[((IOTA - 16) & 127)]) + ((0.0889547989f * fVec1[((IOTA - 17) & 127)]) + ((0.0293457005f * fVec1[((IOTA - 18) & 127)]) + ((0.0214629006f * fVec1[((IOTA - 19) & 127)]) + ((0.0413412005f * fVec1[((IOTA - 20) & 127)]) + ((0.0506356992f * fVec1[((IOTA - 21) & 127)]) + ((0.0585081987f * fVec1[((IOTA - 22) & 127)]) + ((0.000103767001f * fVec1[((IOTA - 71) & 127)]) + ((0.00161520997f * fVec1[((IOTA - 72) & 127)]) + ((0.000467950013f * fVec1[((IOTA - 73) & 127)]) + ((0.00177653006f * fVec1[((IOTA - 77) & 127)]) + ((0.00278163003f * fVec1[((IOTA - 78) & 127)]) + ((0.000587931019f * fVec1[((IOTA - 79) & 127)]) + ((2.36485994e-05f * fVec1[((IOTA - 80) & 127)]) + ((0.000331326999f * fVec1[((IOTA - 82) & 127)]) + ((0.00300030992f * fVec1[((IOTA - 83) & 127)]) + ((0.00104984001f * fVec1[((IOTA - 84) & 127)]) + ((0.000884337001f * fVec1[((IOTA - 85) & 127)]) + ((0.00136716f * fVec1[((IOTA - 86) & 127)]) + ((0.00133451005f * fVec1[((IOTA - 87) & 127)]) + ((0.000459751987f * fVec1[((IOTA - 88) & 127)]) + ((0.000264467002f * fVec1[((IOTA - 89) & 127)]) + ((6.79848017e-05f * fVec1[((IOTA - 90) & 127)]) + ((0.00108098995f * fVec1[((IOTA - 92) & 127)]) + ((0.00167779997f * fVec1[((IOTA - 93) & 127)]) + ((0.00214313995f * fVec1[((IOTA - 94) & 127)]) + ((0.00136594998f * fVec1[((IOTA - 95) & 127)]) + ((0.000623519009f * fVec1[((IOTA - 96) & 127)]) + ((2.85198003e-05f * fVec2[((IOTA - 1) & 127)]) + ((0.000813006016f * fVec1[((IOTA - 97) & 127)]) + ((0.000922270992f * fVec1[((IOTA - 98) & 127)]) + ((0.000456140988f * fVec1[((IOTA - 99) & 127)]) + ((0.000705384999f * fVec1[((IOTA - 100) & 127)]) + ((0.00070973899f * fVec1[((IOTA - 101) & 127)]) + ((0.00043236799f * fVec1[((IOTA - 102) & 127)]) + ((0.000158404f * fVec2[((IOTA - 2) & 127)]) + ((0.000477040012f * fVec1[((IOTA - 103) & 127)]) + ((0.000469696999f * fVec1[((IOTA - 104) & 127)]) + ((5.36059015e-05f * fVec1[((IOTA - 105) & 127)]) + ((0.000419816002f * fVec1[((IOTA - 107) & 127)]) + ((1.17965001e-05f * fVec1[((IOTA - 114) & 127)]) + ((3.39551007e-05f * fTemp1) + ((0.000494618027f * fVec2[((IOTA - 4) & 127)]) + ((0.000315070007f * fVec2[((IOTA - 5) & 127)]) + ((0.00165835f * fVec2[((IOTA - 8) & 127)]) + ((0.000868029019f * fVec2[((IOTA - 13) & 127)]) + ((0.000503397023f * fVec2[((IOTA - 14) & 127)]) + ((0.0325354002f * fVec2[((IOTA - 17) & 127)]) + ((0.0121098002f * fVec2[((IOTA - 20) & 127)]) + ((0.0197593998f * fVec2[((IOTA - 21) & 127)]) + ((0.00576227996f * fVec2[((IOTA - 24) & 127)]) + ((0.00579696987f * fVec2[((IOTA - 26) & 127)]) + ((0.0216850992f * fVec2[((IOTA - 28) & 127)]) + ((0.0215763003f * fVec2[((IOTA - 29) & 127)]) + ((0.00864830986f * fVec2[((IOTA - 32) & 127)]) + ((0.0037660799f * fVec2[((IOTA - 33) & 127)]) + ((0.0164669007f * fVec2[((IOTA - 35) & 127)]) + ((0.000534293998f * fVec2[((IOTA - 36) & 127)]) + ((0.00301502994f * fVec2[((IOTA - 39) & 127)]) + ((0.00531058991f * fVec2[((IOTA - 40) & 127)]) + ((0.00709709991f * fVec2[((IOTA - 41) & 127)]) + ((0.00285895006f * fVec2[((IOTA - 42) & 127)]) + ((0.00223294995f * fVec2[((IOTA - 44) & 127)]) + ((0.00214274996f * fVec2[((IOTA - 45) & 127)]) + ((0.00130771997f * fVec2[((IOTA - 46) & 127)]) + ((0.00918713957f * fVec2[((IOTA - 47) & 127)]) + ((0.00421730988f * fVec2[((IOTA - 48) & 127)]) + ((0.00162145996f * fVec2[((IOTA - 50) & 127)]) + ((0.000779086025f * fVec2[((IOTA - 51) & 127)]) + ((0.00279091997f * fVec2[((IOTA - 52) & 127)]) + ((0.00567660015f * fVec2[((IOTA - 53) & 127)]) + ((0.00243588001f * fVec2[((IOTA - 54) & 127)]) + ((0.000995481969f * fVec2[((IOTA - 58) & 127)]) + ((0.000791253988f * fVec2[((IOTA - 59) & 127)]) + ((0.000895198027f * fVec2[((IOTA - 60) & 127)]) + ((0.00166911003f * fVec2[((IOTA - 62) & 127)]) + ((3.37181009e-05f * fVec2[((IOTA - 63) & 127)]) + ((0.000259004009f * fVec2[((IOTA - 75) & 127)]) + ((0.000285454997f * fVec2[((IOTA - 77) & 127)]) + ((0.00206801994f * fVec2[((IOTA - 78) & 127)]) + ((0.00160799001f * fVec2[((IOTA - 79) & 127)]) + ((0.000383376988f * fVec2[((IOTA - 80) & 127)]) + ((0.000721581979f * fVec2[((IOTA - 83) & 127)]) + ((0.000893280027f * fVec2[((IOTA - 84) & 127)]) + ((0.00368325994f * fVec2[((IOTA - 85) & 127)]) + ((0.00327137997f * fVec2[((IOTA - 86) & 127)]) + ((0.00169944996f * fVec2[((IOTA - 87) & 127)]) + ((0.00375800999f * fVec2[((IOTA - 88) & 127)]) + ((0.00436899019f * fVec2[((IOTA - 89) & 127)]) + ((0.00195189996f * fVec2[((IOTA - 90) & 127)]) + ((0.00119739003f * fVec2[((IOTA - 92) & 127)]) + ((0.00229129009f * fVec2[((IOTA - 93) & 127)]) + ((0.00128729001f * fVec2[((IOTA - 94) & 127)]) + ((0.000324763008f * fVec2[((IOTA - 97) & 127)]) + ((5.11494e-05f * fVec2[((IOTA - 98) & 127)]) + ((9.50248022e-07f * fVec2[((IOTA - 116) & 127)]) + ((7.74526998e-05f * fVec2[((IOTA - 117) & 127)]) + ((5.80835003e-05f * fVec2[((IOTA - 118) & 127)]) + ((9.08166985e-05f * fVec2[((IOTA - 119) & 127)]) + ((9.9426099e-05f * fVec2[((IOTA - 120) & 127)]) + ((8.00667985e-05f * fVec2[((IOTA - 121) & 127)]) + ((2.43757004e-05f * fVec2[((IOTA - 122) & 127)]) + ((8.79674008e-06f * fVec2[((IOTA - 123) & 127)]) + ((8.88364957e-06f * fVec2[((IOTA - 124) & 127)]) + ((8.4641697e-06f * fVec2[((IOTA - 125) & 127)]) + ((2.40821009e-06f * fVec2[((IOTA - 126) & 127)]) + ((4.4578801e-06f * fTemp2) + ((0.00341281993f * fVec3[((IOTA - 36) & 127)]) + ((1.73415992e-05f * fVec3[((IOTA - 38) & 127)]) + ((0.00115935004f * fVec3[((IOTA - 37) & 127)]) + ((2.49699006e-05f * fVec3[((IOTA - 1) & 127)]) + ((0.000138739997f * fVec3[((IOTA - 2) & 127)]) + ((0.000694710005f * fVec3[((IOTA - 4) & 127)]) + ((0.000534433988f * fVec3[((IOTA - 6) & 127)]) + ((0.00148817f * fVec3[((IOTA - 8) & 127)]) + ((0.00201001996f * fVec3[((IOTA - 10) & 127)]) + ((0.00618924014f * fVec3[((IOTA - 13) & 127)]) + ((0.0025992f * fVec3[((IOTA - 15) & 127)]) + ((0.00253456994f * fVec3[((IOTA - 16) & 127)]) + ((0.0211909004f * fVec3[((IOTA - 17) & 127)]) + ((0.0174745992f * fVec3[((IOTA - 18) & 127)]) + ((0.000702438003f * fVec3[((IOTA - 20) & 127)]) + ((9.4065399e-05f * fVec3[((IOTA - 22) & 127)]) + ((0.00194897002f * fVec3[((IOTA - 23) & 127)]) + ((0.0306599997f * fVec3[((IOTA - 26) & 127)]) + ((0.00967743993f * fVec3[((IOTA - 29) & 127)]) + ((0.0362823009f * fVec3[((IOTA - 30) & 127)]) + ((0.00600682991f * fVec3[((IOTA - 33) & 127)]) + ((0.00536729023f * fVec3[((IOTA - 34) & 127)]) + ((0.00503769005f * fVec3[((IOTA - 44) & 127)]) + ((0.00681093009f * fVec3[((IOTA - 45) & 127)]) + ((0.00571346981f * fVec3[((IOTA - 48) & 127)]) + ((0.00534771988f * fVec3[((IOTA - 49) & 127)]) + ((0.00117635995f * fVec3[((IOTA - 51) & 127)]) + ((0.00122470001f * fVec3[((IOTA - 52) & 127)]) + ((0.00166342f * fVec3[((IOTA - 53) & 127)]) + ((0.0035919901f * fVec3[((IOTA - 54) & 127)]) + ((0.000881705026f * fVec3[((IOTA - 55) & 127)]) + ((0.00288331998f * fVec3[((IOTA - 56) & 127)]) + ((0.00206635008f * fVec3[((IOTA - 57) & 127)]) + ((0.000708838983f * fVec3[((IOTA - 62) & 127)]) + ((0.00146985997f * fVec3[((IOTA - 63) & 127)]) + ((0.00113331003f * fVec3[((IOTA - 64) & 127)]) + ((0.00103120005f * fVec3[((IOTA - 65) & 127)]) + ((0.000695453025f * fVec3[((IOTA - 83) & 127)]) + ((0.000367687986f * fVec3[((IOTA - 84) & 127)]) + ((0.00154416997f * fVec3[((IOTA - 85) & 127)]) + ((0.00527861994f * fVec3[((IOTA - 86) & 127)]) + ((0.00337025011f * fVec3[((IOTA - 87) & 127)]) + ((0.00140368997f * fVec3[((IOTA - 90) & 127)]) + ((0.000562855974f * fVec3[((IOTA - 91) & 127)]) + ((0.00068253401f * fVec3[((IOTA - 94) & 127)]) + ((0.00317137991f * fVec3[((IOTA - 95) & 127)]) + ((0.00195890991f * fVec3[((IOTA - 96) & 127)]) + ((0.000184011995f * fVec3[((IOTA - 115) & 127)]) + ((0.000184563003f * fVec3[((IOTA - 116) & 127)]) + ((3.85491003e-05f * fVec3[((IOTA - 117) & 127)]) + ((3.92965994e-05f * fVec3[((IOTA - 119) & 127)]) + ((0.000105639003f * fVec3[((IOTA - 120) & 127)]) + ((0.000101833f * fVec3[((IOTA - 121) & 127)]) + ((3.88275985e-05f * fVec3[((IOTA - 122) & 127)]) + ((8.0341697e-06f * fVec3[((IOTA - 123) & 127)]) + ((1.70272995e-06f * fVec3[((IOTA - 125) & 127)]) + ((2.45330989e-06f * fVec3[((IOTA - 126) & 127)]) + ((9.93627964e-06f * fTemp3) + ((0.00483835023f * fVec3[((IOTA - 39) & 127)]) + ((0.00814255979f * fVec3[((IOTA - 42) & 127)]) + (0.00336908991f * fVec3[((IOTA - 43) & 127)]))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))) - ((0.000315062003f * fVec1[((IOTA - 2) & 127)]) + ((0.000544864975f * fVec0[((IOTA - 97) & 127)]) + ((0.00249696989f * fVec0[((IOTA - 92) & 127)]) + ((0.00148382003f * fVec0[((IOTA - 91) & 127)]) + ((0.00134669f * fVec0[((IOTA - 89) & 127)]) + ((0.00141241995f * fVec0[((IOTA - 88) & 127)]) + ((0.00031191099f * fVec0[((IOTA - 84) & 127)]) + ((0.00106944004f * fVec0[((IOTA - 81) & 127)]) + ((0.00296739f * fVec0[((IOTA - 47) & 127)]) + ((0.0114163999f * fVec0[((IOTA - 28) & 127)]) + ((0.0154149998f * fVec0[((IOTA - 24) & 127)]) + ((0.000173780005f * fVec0[((IOTA - 2) & 127)]) + ((0.000414598995f * fVec1[((IOTA - 6) & 127)]) + ((0.000167328995f * fVec1[((IOTA - 8) & 127)]) + ((0.00117791002f * fVec1[((IOTA - 10) & 127)]) + ((0.0346875004f * fVec1[((IOTA - 23) & 127)]) + ((0.0427948013f * fVec1[((IOTA - 24) & 127)]) + ((0.000603284978f * fVec1[((IOTA - 25) & 127)]) + ((0.00951419957f * fVec1[((IOTA - 26) & 127)]) + ((0.0160175003f * fVec1[((IOTA - 27) & 127)]) + ((0.0394065008f * fVec1[((IOTA - 28) & 127)]) + ((0.0390073992f * fVec1[((IOTA - 29) & 127)]) + ((0.0125257f * fVec1[((IOTA - 30) & 127)]) + ((0.0052166502f * fVec1[((IOTA - 31) & 127)]) + ((0.0193806998f * fVec1[((IOTA - 32) & 127)]) + ((0.0357626006f * fVec1[((IOTA - 33) & 127)]) + ((0.0264369994f * fVec1[((IOTA - 34) & 127)]) + ((0.0180914998f * fVec1[((IOTA - 35) & 127)]) + ((0.0239675995f * fVec1[((IOTA - 36) & 127)]) + ((0.0269667003f * fVec1[((IOTA - 37) & 127)]) + ((0.00862273015f * fVec1[((IOTA - 40) & 127)]) + ((0.0252633002f * fVec1[((IOTA - 38) & 127)]) + ((0.0153689999f * fVec1[((IOTA - 39) & 127)]) + ((0.0203737002f * fVec1[((IOTA - 41) & 127)]) + ((0.0221845005f * fVec1[((IOTA - 42) & 127)]) + ((0.0188737996f * fVec1[((IOTA - 43) & 127)]) + ((0.0160792004f * fVec1[((IOTA - 44) & 127)]) + ((0.00887527969f * fVec1[((IOTA - 45) & 127)]) + ((0.0116756f * fVec1[((IOTA - 46) & 127)]) + ((0.0161942001f * fVec1[((IOTA - 47) & 127)]) + ((0.0166062005f * fVec1[((IOTA - 48) & 127)]) + ((0.0128559005f * fVec1[((IOTA - 49) & 127)]) + ((0.0151193999f * fVec1[((IOTA - 50) & 127)]) + ((0.0159131996f * fVec1[((IOTA - 51) & 127)]) + ((0.00958194956f * fVec1[((IOTA - 52) & 127)]) + ((0.00687643001f * fVec1[((IOTA - 53) & 127)]) + ((0.00766095007f * fVec1[((IOTA - 54) & 127)]) + ((0.0100582f * fVec1[((IOTA - 55) & 127)]) + ((0.0104416003f * fVec1[((IOTA - 56) & 127)]) + ((0.00454198988f * fVec1[((IOTA - 57) & 127)]) + ((0.00377103989f * fVec1[((IOTA - 58) & 127)]) + ((0.00475475984f * fVec1[((IOTA - 59) & 127)]) + ((0.00279510999f * fVec1[((IOTA - 60) & 127)]) + ((0.00474843988f * fVec1[((IOTA - 61) & 127)]) + ((0.0044470299f * fVec1[((IOTA - 62) & 127)]) + ((0.00246014004f * fVec1[((IOTA - 63) & 127)]) + ((0.00170560996f * fVec1[((IOTA - 64) & 127)]) + ((0.00243748003f * fVec1[((IOTA - 65) & 127)]) + ((0.00217469991f * fVec1[((IOTA - 66) & 127)]) + ((0.00157915999f * fVec1[((IOTA - 67) & 127)]) + ((0.00301045994f * fVec1[((IOTA - 68) & 127)]) + ((0.00238789991f * fVec1[((IOTA - 69) & 127)]) + ((0.00102902995f * fVec1[((IOTA - 70) & 127)]) + ((0.00184981001f * fVec1[((IOTA - 74) & 127)]) + ((0.00189912005f * fVec1[((IOTA - 75) & 127)]) + ((0.000929743983f * fVec1[((IOTA - 76) & 127)]) + ((0.00117495004f * fVec1[((IOTA - 81) & 127)]) + ((0.000138671996f * fVec1[((IOTA - 91) & 127)]) + ((0.000356530014f * fVec1[((IOTA - 106) & 127)]) + ((0.000192449996f * fVec2[((IOTA - 3) & 127)]) + ((8.74661964e-06f * fVec1[((IOTA - 108) & 127)]) + ((0.000273034006f * fVec1[((IOTA - 109) & 127)]) + ((0.000269356999f * fVec1[((IOTA - 110) & 127)]) + ((0.000314306992f * fVec1[((IOTA - 111) & 127)]) + ((0.000272806006f * fVec1[((IOTA - 112) & 127)]) + ((0.000250632991f * fVec1[((IOTA - 113) & 127)]) + ((0.000125621998f * fVec1[((IOTA - 115) & 127)]) + ((9.13990007e-05f * fVec1[((IOTA - 116) & 127)]) + ((0.000137362003f * fVec1[((IOTA - 117) & 127)]) + ((0.000163853998f * fVec1[((IOTA - 118) & 127)]) + ((1.11290001e-05f * fVec1[((IOTA - 119) & 127)]) + ((3.11750009e-05f * fVec1[((IOTA - 120) & 127)]) + ((4.81323004e-05f * fVec1[((IOTA - 121) & 127)]) + ((6.52305025e-05f * fVec1[((IOTA - 122) & 127)]) + ((3.88020017e-05f * fVec1[((IOTA - 123) & 127)]) + ((8.31850957e-06f * fVec1[((IOTA - 124) & 127)]) + ((2.56392991e-06f * fVec1[((IOTA - 125) & 127)]) + ((1.86560999e-06f * fVec1[((IOTA - 126) & 127)]) + ((0.000160361f * fVec2[((IOTA - 6) & 127)]) + ((0.000970588997f * fVec2[((IOTA - 7) & 127)]) + ((0.000181327006f * fVec2[((IOTA - 9) & 127)]) + ((0.00043871699f * fVec2[((IOTA - 10) & 127)]) + ((0.00036272101f * fVec2[((IOTA - 11) & 127)]) + ((0.00489909016f * fVec2[((IOTA - 12) & 127)]) + ((0.0158698f * fVec2[((IOTA - 15) & 127)]) + ((0.00926829968f * fVec2[((IOTA - 16) & 127)]) + ((0.0149072995f * fVec2[((IOTA - 18) & 127)]) + ((0.0371116996f * fVec2[((IOTA - 19) & 127)]) + ((0.00578039978f * fVec2[((IOTA - 22) & 127)]) + ((0.00321276998f * fVec2[((IOTA - 23) & 127)]) + ((0.0115484996f * fVec2[((IOTA - 25) & 127)]) + ((0.00436148001f * fVec2[((IOTA - 27) & 127)]) + ((0.0200181007f * fVec2[((IOTA - 30) & 127)]) + ((0.0161655005f * fVec2[((IOTA - 31) & 127)]) + ((0.0137585001f * fVec2[((IOTA - 34) & 127)]) + ((0.0193135999f * fVec2[((IOTA - 37) & 127)]) + ((0.00392636983f * fVec2[((IOTA - 38) & 127)]) + ((0.00432563014f * fVec2[((IOTA - 43) & 127)]) + ((0.00161132996f * fVec2[((IOTA - 49) & 127)]) + ((0.00029472899f * fVec2[((IOTA - 55) & 127)]) + ((0.00350461993f * fVec2[((IOTA - 56) & 127)]) + ((0.00123748998f * fVec2[((IOTA - 57) & 127)]) + ((0.00113341003f * fVec2[((IOTA - 61) & 127)]) + ((0.0100646997f * fVec3[((IOTA - 35) & 127)]) + ((0.0027169399f * fVec2[((IOTA - 64) & 127)]) + ((0.00177024002f * fVec2[((IOTA - 65) & 127)]) + ((0.00153815001f * fVec2[((IOTA - 66) & 127)]) + ((0.00260886992f * fVec2[((IOTA - 67) & 127)]) + ((0.00465625012f * fVec2[((IOTA - 68) & 127)]) + ((0.00336985011f * fVec2[((IOTA - 69) & 127)]) + ((0.000735849026f * fVec2[((IOTA - 70) & 127)]) + ((0.00191901997f * fVec2[((IOTA - 71) & 127)]) + ((0.00239256001f * fVec2[((IOTA - 72) & 127)]) + ((0.00113851996f * fVec2[((IOTA - 73) & 127)]) + ((0.000315111014f * fVec2[((IOTA - 74) & 127)]) + ((4.10987996e-05f * fVec2[((IOTA - 76) & 127)]) + ((0.000737536f * fVec2[((IOTA - 81) & 127)]) + ((0.000608302013f * fVec2[((IOTA - 82) & 127)]) + ((5.44478007e-06f * fVec2[((IOTA - 91) & 127)]) + ((0.00104970997f * fVec2[((IOTA - 95) & 127)]) + ((0.00121329003f * fVec2[((IOTA - 96) & 127)]) + ((0.00147963001f * fVec2[((IOTA - 99) & 127)]) + ((0.00155276002f * fVec2[((IOTA - 100) & 127)]) + ((0.0013151f * fVec2[((IOTA - 101) & 127)]) + ((0.000622008985f * fVec2[((IOTA - 102) & 127)]) + ((0.000964667008f * fVec2[((IOTA - 103) & 127)]) + ((0.00149514002f * fVec2[((IOTA - 104) & 127)]) + ((0.00126437005f * fVec2[((IOTA - 105) & 127)]) + ((0.000641906983f * fVec2[((IOTA - 106) & 127)]) + ((0.000260406989f * fVec2[((IOTA - 107) & 127)]) + ((0.00127534999f * fVec2[((IOTA - 108) & 127)]) + ((0.00107899995f * fVec2[((IOTA - 109) & 127)]) + ((0.000383553008f * fVec2[((IOTA - 110) & 127)]) + ((0.000388626009f * fVec2[((IOTA - 111) & 127)]) + ((0.000542304013f * fVec2[((IOTA - 112) & 127)]) + ((0.000550354016f * fVec2[((IOTA - 113) & 127)]) + ((7.58937967e-05f * fVec2[((IOTA - 114) & 127)]) + ((6.57487981e-05f * fVec2[((IOTA - 115) & 127)]) + ((0.000177736001f * fVec3[((IOTA - 3) & 127)]) + ((0.000362084014f * fVec3[((IOTA - 5) & 127)]) + ((0.00110817002f * fVec3[((IOTA - 7) & 127)]) + ((0.00131484994f * fVec3[((IOTA - 9) & 127)]) + ((0.00303202006f * fVec3[((IOTA - 11) & 127)]) + ((0.00721717998f * fVec3[((IOTA - 12) & 127)]) + ((0.00530957989f * fVec3[((IOTA - 14) & 127)]) + ((0.0116911f * fVec3[((IOTA - 19) & 127)]) + ((0.0120184002f * fVec3[((IOTA - 21) & 127)]) + ((0.0254584998f * fVec3[((IOTA - 24) & 127)]) + ((0.000698870979f * fVec3[((IOTA - 25) & 127)]) + ((0.000315637997f * fVec3[((IOTA - 27) & 127)]) + ((0.0391701013f * fVec3[((IOTA - 28) & 127)]) + ((0.0173412003f * fVec3[((IOTA - 31) & 127)]) + ((0.0301367007f * fVec3[((IOTA - 32) & 127)]) + ((0.00765874004f * fVec3[((IOTA - 40) & 127)]) + ((0.00303208991f * fVec3[((IOTA - 41) & 127)]) + ((0.00248879008f * fVec3[((IOTA - 46) & 127)]) + ((0.00299686007f * fVec3[((IOTA - 47) & 127)]) + ((0.000498921028f * fVec3[((IOTA - 50) & 127)]) + ((0.000550485973f * fVec3[((IOTA - 58) & 127)]) + ((0.000502175011f * fVec3[((IOTA - 59) & 127)]) + ((0.00149052998f * fVec3[((IOTA - 60) & 127)]) + ((0.000255875988f * fVec3[((IOTA - 61) & 127)]) + ((0.00103438995f * fVec3[((IOTA - 66) & 127)]) + ((0.00193402998f * fVec3[((IOTA - 67) & 127)]) + ((4.52415989e-05f * fVec3[((IOTA - 68) & 127)]) + ((0.00171474996f * fVec3[((IOTA - 69) & 127)]) + ((0.00312194997f * fVec3[((IOTA - 70) & 127)]) + ((0.00198432012f * fVec3[((IOTA - 71) & 127)]) + ((0.00163131999f * fVec3[((IOTA - 72) & 127)]) + ((0.00152867998f * fVec3[((IOTA - 73) & 127)]) + ((0.00121188001f * fVec3[((IOTA - 74) & 127)]) + ((0.000323087006f * fVec3[((IOTA - 75) & 127)]) + ((0.00190083997f * fVec3[((IOTA - 76) & 127)]) + ((0.00299122999f * fVec3[((IOTA - 77) & 127)]) + ((0.00146794005f * fVec3[((IOTA - 78) & 127)]) + ((0.000830327976f * fVec3[((IOTA - 79) & 127)]) + ((0.000829760975f * fVec3[((IOTA - 80) & 127)]) + ((0.00305920001f * fVec3[((IOTA - 81) & 127)]) + ((0.00200403994f * fVec3[((IOTA - 82) & 127)]) + ((0.00142103003f * fVec3[((IOTA - 88) & 127)]) + ((0.00103026f * fVec3[((IOTA - 89) & 127)]) + ((0.00201637996f * fVec3[((IOTA - 92) & 127)]) + ((0.00135488994f * fVec3[((IOTA - 93) & 127)]) + ((0.00126003998f * fVec3[((IOTA - 97) & 127)]) + ((0.00170268002f * fVec3[((IOTA - 98) & 127)]) + ((0.00103888998f * fVec3[((IOTA - 99) & 127)]) + ((0.00124352996f * fVec3[((IOTA - 100) & 127)]) + ((0.00124685001f * fVec3[((IOTA - 101) & 127)]) + ((0.000711990986f * fVec3[((IOTA - 102) & 127)]) + ((0.000446193008f * fVec3[((IOTA - 103) & 127)]) + ((0.000888254028f * fVec3[((IOTA - 104) & 127)]) + ((0.000654569012f * fVec3[((IOTA - 105) & 127)]) + ((0.000518167973f * fVec3[((IOTA - 106) & 127)]) + ((0.000563891022f * fVec3[((IOTA - 107) & 127)]) + ((0.000839626999f * fVec3[((IOTA - 108) & 127)]) + ((0.000541094982f * fVec3[((IOTA - 109) & 127)]) + ((0.000208049998f * fVec3[((IOTA - 110) & 127)]) + ((9.82971978e-05f * fVec3[((IOTA - 111) & 127)]) + ((0.000290241005f * fVec3[((IOTA - 112) & 127)]) + ((0.000428972999f * fVec3[((IOTA - 113) & 127)]) + ((0.000103578997f * fVec3[((IOTA - 114) & 127)]) + ((6.97165979e-06f * fVec3[((IOTA - 124) & 127)]) + (9.84851977e-06f * fVec3[((IOTA - 118) & 127)])))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))));
			fRec0[0] = max((fRec0[1] - fConst0), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp4))))));
			fHbargraph0 = FAUSTFLOAT(fRec0[0]);
			output0[i] = FAUSTFLOAT(fTemp4);
			float fTemp5 = (fRec1[0] * (((0.000275169004f * fVec1[((IOTA - 2) & 127)]) + ((0.00193882f * fVec0[((IOTA - 88) & 127)]) + ((0.00105508999f * fVec0[((IOTA - 84) & 127)]) + ((0.00153689005f * fVec0[((IOTA - 81) & 127)]) + ((0.000715699978f * fVec0[((IOTA - 47) & 127)]) + ((0.0123140998f * fVec0[((IOTA - 28) & 127)]) + ((0.0568950996f * fVec1[((IOTA - 24) & 127)]) + ((0.0105670001f * fVec1[((IOTA - 25) & 127)]) + ((0.0142101999f * fVec1[((IOTA - 26) & 127)]) + ((0.0254229009f * fVec1[((IOTA - 27) & 127)]) + ((0.0236018002f * fVec1[((IOTA - 28) & 127)]) + ((0.0330714993f * fVec1[((IOTA - 29) & 127)]) + ((0.0246581994f * fVec1[((IOTA - 30) & 127)]) + ((0.0128670996f * fVec1[((IOTA - 31) & 127)]) + ((0.0234980006f * fVec1[((IOTA - 32) & 127)]) + ((0.0302699003f * fVec1[((IOTA - 33) & 127)]) + ((0.0232958999f * fVec1[((IOTA - 34) & 127)]) + ((0.0186216999f * fVec1[((IOTA - 35) & 127)]) + ((0.0290731005f * fVec1[((IOTA - 36) & 127)]) + ((0.0333365016f * fVec1[((IOTA - 37) & 127)]) + ((0.0227995999f * fVec1[((IOTA - 40) & 127)]) + ((0.0176929999f * fVec1[((IOTA - 38) & 127)]) + ((0.016965f * fVec1[((IOTA - 39) & 127)]) + ((0.0160723999f * fVec1[((IOTA - 41) & 127)]) + ((0.0175694004f * fVec1[((IOTA - 42) & 127)]) + ((0.0173035003f * fVec1[((IOTA - 43) & 127)]) + ((0.0198068991f * fVec1[((IOTA - 44) & 127)]) + ((0.0170272999f * fVec1[((IOTA - 45) & 127)]) + ((0.0122635001f * fVec1[((IOTA - 46) & 127)]) + ((0.0116857002f * fVec1[((IOTA - 47) & 127)]) + ((0.0121521996f * fVec1[((IOTA - 48) & 127)]) + ((0.0166992005f * fVec1[((IOTA - 49) & 127)]) + ((0.0188990999f * fVec1[((IOTA - 50) & 127)]) + ((0.0143636996f * fVec1[((IOTA - 51) & 127)]) + ((0.00790243968f * fVec1[((IOTA - 52) & 127)]) + ((0.00468814978f * fVec1[((IOTA - 53) & 127)]) + ((0.00580003019f * fVec1[((IOTA - 54) & 127)]) + ((0.00740671018f * fVec1[((IOTA - 55) & 127)]) + ((0.0109804999f * fVec1[((IOTA - 56) & 127)]) + ((0.0110120997f * fVec1[((IOTA - 57) & 127)]) + ((0.00575488992f * fVec1[((IOTA - 58) & 127)]) + ((0.0051357802f * fVec1[((IOTA - 59) & 127)]) + ((0.00389261008f * fVec1[((IOTA - 60) & 127)]) + ((0.00204509008f * fVec1[((IOTA - 61) & 127)]) + ((0.00232848991f * fVec1[((IOTA - 62) & 127)]) + ((0.00303700007f * fVec1[((IOTA - 63) & 127)]) + ((0.00227326993f * fVec1[((IOTA - 64) & 127)]) + ((0.000377902004f * fVec1[((IOTA - 65) & 127)]) + ((0.00128867f * fVec1[((IOTA - 66) & 127)]) + ((0.00164630997f * fVec1[((IOTA - 67) & 127)]) + ((0.000542053021f * fVec1[((IOTA - 68) & 127)]) + ((0.00390024995f * fVec1[((IOTA - 69) & 127)]) + ((0.00445498992f * fVec1[((IOTA - 70) & 127)]) + ((0.00179050001f * fVec1[((IOTA - 76) & 127)]) + ((9.12387986e-06f * fVec1[((IOTA - 108) & 127)]) + ((0.000239083995f * fVec1[((IOTA - 109) & 127)]) + ((0.000227080003f * fVec1[((IOTA - 110) & 127)]) + ((0.000251321995f * fVec1[((IOTA - 111) & 127)]) + ((0.000368308014f * fVec1[((IOTA - 112) & 127)]) + ((0.000306277012f * fVec1[((IOTA - 113) & 127)]) + ((2.67516007e-05f * fVec1[((IOTA - 115) & 127)]) + ((0.000118887001f * fVec1[((IOTA - 116) & 127)]) + ((0.000158936004f * fVec1[((IOTA - 117) & 127)]) + ((0.000165502002f * fVec1[((IOTA - 118) & 127)]) + ((3.39826984e-05f * fVec1[((IOTA - 119) & 127)]) + ((3.71675997e-05f * fVec1[((IOTA - 121) & 127)]) + ((7.19017989e-05f * fVec1[((IOTA - 122) & 127)]) + ((4.14704991e-05f * fVec1[((IOTA - 123) & 127)]) + ((8.50585002e-06f * fVec1[((IOTA - 124) & 127)]) + ((3.69731993e-06f * fVec1[((IOTA - 125) & 127)]) + ((1.48337006e-06f * fVec1[((IOTA - 126) & 127)]) + ((7.08006992e-05f * fVec2[((IOTA - 9) & 127)]) + ((0.00927290041f * fVec2[((IOTA - 22) & 127)]) + ((0.00278365007f * fVec2[((IOTA - 23) & 127)]) + ((0.000330976007f * fVec2[((IOTA - 43) & 127)]) + ((0.00756405992f * fVec2[((IOTA - 49) & 127)]) + ((0.00227417005f * fVec2[((IOTA - 55) & 127)]) + ((7.66540961e-06f * fVec2[((IOTA - 61) & 127)]) + ((0.00102092f * fVec2[((IOTA - 74) & 127)]) + ((0.000139904005f * fVec2[((IOTA - 76) & 127)]) + ((0.000386787986f * fVec2[((IOTA - 81) & 127)]) + ((0.000876507023f * fVec2[((IOTA - 91) & 127)]) + ((1.91025993e-05f * fVec2[((IOTA - 114) & 127)]) + ((5.59008986e-05f * fVec2[((IOTA - 115) & 127)]) + ((0.000127546999f * fVec3[((IOTA - 9) & 127)]) + ((0.0115422001f * fVec3[((IOTA - 31) & 127)]) + ((0.00615266012f * fVec3[((IOTA - 46) & 127)]) + ((0.00119971996f * fVec3[((IOTA - 47) & 127)]) + ((0.00486999005f * fVec3[((IOTA - 50) & 127)]) + ((0.000408735999f * fVec3[((IOTA - 58) & 127)]) + ((0.000701283978f * fVec3[((IOTA - 59) & 127)]) + ((0.00198173011f * fVec3[((IOTA - 88) & 127)]) + ((0.00065997499f * fVec3[((IOTA - 92) & 127)]) + ((0.000115798f * fVec3[((IOTA - 111) & 127)]) + ((5.66913004e-05f * fVec3[((IOTA - 112) & 127)]) + ((4.22070007e-05f * fVec3[((IOTA - 118) & 127)]) + ((5.1482898e-06f * fVec3[((IOTA - 124) & 127)]) + ((1.74778995e-06f * fVec0[((IOTA - 126) & 127)]) + ((7.13748022e-06f * fVec0[((IOTA - 125) & 127)]) + ((1.69708001e-05f * fVec0[((IOTA - 124) & 127)]) + ((7.39806012e-07f * fVec0[((IOTA - 123) & 127)]) + ((5.59967993e-05f * fVec0[((IOTA - 121) & 127)]) + ((0.000117308002f * fVec0[((IOTA - 120) & 127)]) + ((0.000195057f * fVec0[((IOTA - 119) & 127)]) + ((0.000202163996f * fVec0[((IOTA - 118) & 127)]) + ((0.000161140997f * fVec0[((IOTA - 117) & 127)]) + ((0.000383523991f * fVec0[((IOTA - 116) & 127)]) + ((0.000502614013f * fVec0[((IOTA - 115) & 127)]) + ((0.000508035999f * fVec0[((IOTA - 114) & 127)]) + ((0.000426596991f * fVec0[((IOTA - 113) & 127)]) + ((0.000504220021f * fVec0[((IOTA - 112) & 127)]) + ((0.00103368005f * fVec0[((IOTA - 111) & 127)]) + ((0.000709244981f * fVec0[((IOTA - 110) & 127)]) + ((0.000779071008f * fVec0[((IOTA - 109) & 127)]) + ((0.000959788973f * fVec0[((IOTA - 108) & 127)]) + ((0.00130975002f * fVec0[((IOTA - 107) & 127)]) + ((0.00162599003f * fVec0[((IOTA - 106) & 127)]) + ((0.00100500998f * fVec0[((IOTA - 105) & 127)]) + ((0.0012236f * fVec0[((IOTA - 104) & 127)]) + ((0.00184199004f * fVec0[((IOTA - 103) & 127)]) + ((0.00147426f * fVec0[((IOTA - 102) & 127)]) + ((0.00132719998f * fVec0[((IOTA - 101) & 127)]) + ((0.000945037988f * fVec0[((IOTA - 100) & 127)]) + ((0.000594980025f * fVec0[((IOTA - 99) & 127)]) + ((0.00257389992f * fVec0[((IOTA - 96) & 127)]) + ((0.00335120992f * fVec0[((IOTA - 95) & 127)]) + ((0.00365475006f * fVec0[((IOTA - 87) & 127)]) + ((0.00267456006f * fVec0[((IOTA - 86) & 127)]) + ((0.00175057002f * fVec0[((IOTA - 85) & 127)]) + ((2.51563997e-05f * fVec0[((IOTA - 83) & 127)]) + ((0.00266924012f * fVec0[((IOTA - 80) & 127)]) + ((0.00144247001f * fVec0[((IOTA - 79) & 127)]) + ((0.00204095989f * fVec0[((IOTA - 78) & 127)]) + ((0.00201999f * fVec0[((IOTA - 77) & 127)]) + ((0.00463264016f * fVec0[((IOTA - 76) & 127)]) + ((0.00538216997f * fVec0[((IOTA - 75) & 127)]) + ((0.00360327004f * fVec0[((IOTA - 74) & 127)]) + ((0.00530226994f * fVec0[((IOTA - 73) & 127)]) + ((0.00390875014f * fVec0[((IOTA - 72) & 127)]) + ((0.00198287005f * fVec0[((IOTA - 71) & 127)]) + ((0.000724039972f * fVec0[((IOTA - 70) & 127)]) + ((0.00337264012f * fVec0[((IOTA - 68) & 127)]) + ((0.00281278999f * fVec0[((IOTA - 67) & 127)]) + ((0.00752294017f * fVec0[((IOTA - 27) & 127)]) + ((0.0424236991f * fVec0[((IOTA - 26) & 127)]) + ((0.0368012004f * fVec0[((IOTA - 25) & 127)]) + ((0.0544642992f * fVec0[((IOTA - 23) & 127)]) + ((0.111697003f * fVec0[((IOTA - 22) & 127)]) + ((0.0503805988f * fVec0[((IOTA - 21) & 127)]) + ((0.0697323009f * fVec0[((IOTA - 20) & 127)]) + ((0.0414059013f * fVec0[((IOTA - 19) & 127)]) + ((0.0474243015f * fVec0[((IOTA - 18) & 127)]) + ((0.0813459009f * fVec0[((IOTA - 17) & 127)]) + ((0.0439485982f * fVec0[((IOTA - 16) & 127)]) + ((0.0622112006f * fVec0[((IOTA - 15) & 127)]) + ((0.0466987006f * fVec0[((IOTA - 14) & 127)]) + ((0.0530011989f * fVec0[((IOTA - 13) & 127)]) + ((0.0489413999f * fVec0[((IOTA - 12) & 127)]) + ((0.000739153009f * fVec0[((IOTA - 11) & 127)]) + ((0.00206854008f * fVec0[((IOTA - 10) & 127)]) + ((0.000557527004f * fVec0[((IOTA - 9) & 127)]) + ((0.00107368f * fVec0[((IOTA - 8) & 127)]) + ((0.00105561002f * fVec0[((IOTA - 7) & 127)]) + ((0.000646816974f * fVec0[((IOTA - 6) & 127)]) + ((0.000782518007f * fVec0[((IOTA - 5) & 127)]) + ((0.00084005401f * fVec0[((IOTA - 4) & 127)]) + ((0.000724458019f * fVec0[((IOTA - 3) & 127)]) + ((0.000127139007f * fVec0[((IOTA - 1) & 127)]) + ((0.0216825996f * fVec0[((IOTA - 29) & 127)]) + ((0.0177305993f * fVec0[((IOTA - 30) & 127)]) + ((0.00801942032f * fVec0[((IOTA - 31) & 127)]) + ((0.0210740007f * fVec0[((IOTA - 33) & 127)]) + ((0.0272113997f * fVec0[((IOTA - 34) & 127)]) + ((0.00565641979f * fVec0[((IOTA - 35) & 127)]) + ((0.0105390996f * fVec0[((IOTA - 36) & 127)]) + ((0.00815984979f * fVec0[((IOTA - 37) & 127)]) + ((0.00606657006f * fVec0[((IOTA - 38) & 127)]) + ((0.00729383994f * fVec0[((IOTA - 39) & 127)]) + ((0.00145252002f * fVec0[((IOTA - 40) & 127)]) + ((0.00498628989f * fVec0[((IOTA - 41) & 127)]) + ((0.0140383998f * fVec0[((IOTA - 42) & 127)]) + ((0.0105579002f * fVec0[((IOTA - 43) & 127)]) + ((0.00431154016f * fVec0[((IOTA - 45) & 127)]) + ((0.0084629897f * fVec0[((IOTA - 46) & 127)]) + ((0.000671665999f * fVec0[((IOTA - 48) & 127)]) + ((0.0046483702f * fVec0[((IOTA - 49) & 127)]) + ((0.00143565994f * fVec0[((IOTA - 50) & 127)]) + ((0.000209300997f * fVec0[((IOTA - 51) & 127)]) + ((0.00139634998f * fVec0[((IOTA - 52) & 127)]) + ((0.00292298989f * fVec0[((IOTA - 53) & 127)]) + ((0.00425950019f * fVec0[((IOTA - 54) & 127)]) + ((0.00399739994f * fVec0[((IOTA - 55) & 127)]) + ((4.49753998e-06f * fVec0[((IOTA - 56) & 127)]) + ((0.00278078998f * fVec0[((IOTA - 58) & 127)]) + ((0.00380207994f * fVec0[((IOTA - 59) & 127)]) + ((0.00261221011f * fVec0[((IOTA - 60) & 127)]) + ((0.00338193006f * fVec0[((IOTA - 61) & 127)]) + ((0.00648151012f * fVec0[((IOTA - 62) & 127)]) + ((0.00672976021f * fVec0[((IOTA - 63) & 127)]) + ((0.0038801101f * fVec0[((IOTA - 64) & 127)]) + ((0.00293818f * fVec0[((IOTA - 65) & 127)]) + ((0.00169334002f * fVec0[((IOTA - 66) & 127)]) + ((1.96888996e-05f * fTemp0) + ((0.000155717004f * fVec1[((IOTA - 71) & 127)]) + ((0.000659941987f * fVec1[((IOTA - 77) & 127)]) + ((0.00116433005f * fVec1[((IOTA - 82) & 127)]) + ((0.000219726993f * fVec1[((IOTA - 89) & 127)]) + ((0.000154296999f * fVec1[((IOTA - 90) & 127)]) + ((1.52617995e-05f * fVec1[((IOTA - 114) & 127)]) + ((0.000147661005f * fVec2[((IOTA - 4) & 127)]) + ((0.000231145998f * fVec2[((IOTA - 5) & 127)]) + ((0.000769565988f * fVec2[((IOTA - 8) & 127)]) + ((0.0258154999f * fVec2[((IOTA - 17) & 127)]) + ((0.00120597996f * fVec2[((IOTA - 20) & 127)]) + ((0.0222989991f * fVec2[((IOTA - 21) & 127)]) + ((0.00438647997f * fVec2[((IOTA - 24) & 127)]) + ((0.000537327025f * fVec2[((IOTA - 26) & 127)]) + ((0.00923500024f * fVec2[((IOTA - 28) & 127)]) + ((0.0338639989f * fVec2[((IOTA - 29) & 127)]) + ((0.00403936999f * fVec2[((IOTA - 33) & 127)]) + ((0.0054465998f * fVec2[((IOTA - 35) & 127)]) + ((0.00884038955f * fVec2[((IOTA - 36) & 127)]) + ((0.00108066003f * fVec2[((IOTA - 40) & 127)]) + ((0.00817955006f * fVec2[((IOTA - 41) & 127)]) + ((0.00669066003f * fVec2[((IOTA - 42) & 127)]) + ((0.000184974997f * fVec2[((IOTA - 46) & 127)]) + ((0.00308119995f * fVec2[((IOTA - 47) & 127)]) + ((0.00796032045f * fVec2[((IOTA - 48) & 127)]) + ((0.00415928988f * fVec2[((IOTA - 50) & 127)]) + ((0.0032281999f * fVec2[((IOTA - 53) & 127)]) + ((0.0042530098f * fVec2[((IOTA - 54) & 127)]) + ((0.00215189997f * fVec2[((IOTA - 62) & 127)]) + ((0.00235811993f * fVec2[((IOTA - 63) & 127)]) + ((0.00174807f * fVec2[((IOTA - 75) & 127)]) + ((0.000827394018f * fVec2[((IOTA - 78) & 127)]) + ((0.00145085005f * fVec2[((IOTA - 79) & 127)]) + ((0.000906103f * fVec2[((IOTA - 80) & 127)]) + ((0.00118894002f * fVec2[((IOTA - 83) & 127)]) + ((0.00251957006f * fVec2[((IOTA - 86) & 127)]) + ((0.0029776101f * fVec2[((IOTA - 87) & 127)]) + ((0.00173031003f * fVec2[((IOTA - 88) & 127)]) + ((0.00132805004f * fVec2[((IOTA - 89) & 127)]) + ((0.00204059994f * fVec2[((IOTA - 90) & 127)]) + ((9.57012016e-05f * fVec2[((IOTA - 119) & 127)]) + ((0.000122408004f * fVec2[((IOTA - 120) & 127)]) + ((9.32295006e-05f * fVec2[((IOTA - 121) & 127)]) + ((3.49539987e-05f * fVec2[((IOTA - 122) & 127)]) + ((1.98733997e-05f * fVec2[((IOTA - 123) & 127)]) + ((1.89778002e-05f * fVec2[((IOTA - 124) & 127)]) + ((1.00449997e-05f * fVec2[((IOTA - 125) & 127)]) + ((1.86490001e-06f * fVec2[((IOTA - 126) & 127)]) + ((0.00127378001f * fVec3[((IOTA - 38) & 127)]) + ((2.32588009e-05f * fVec3[((IOTA - 1) & 127)]) + ((0.000107706001f * fVec3[((IOTA - 2) & 127)]) + ((0.000558749016f * fVec3[((IOTA - 4) & 127)]) + ((0.000569647003f * fVec3[((IOTA - 6) & 127)]) + ((0.000286004011f * fVec3[((IOTA - 8) & 127)]) + ((0.000654334028f * fVec3[((IOTA - 10) & 127)]) + ((0.00338210003f * fVec3[((IOTA - 13) & 127)]) + ((0.0131959002f * fVec3[((IOTA - 15) & 127)]) + ((0.0156110004f * fVec3[((IOTA - 17) & 127)]) + ((0.0292041004f * fVec3[((IOTA - 18) & 127)]) + ((0.0211759992f * fVec3[((IOTA - 22) & 127)]) + ((0.0130594f * fVec3[((IOTA - 23) & 127)]) + ((0.053748101f * fVec3[((IOTA - 26) & 127)]) + ((0.0243673995f * fVec3[((IOTA - 30) & 127)]) + ((0.0125606004f * fVec3[((IOTA - 34) & 127)]) + ((0.00287069008f * fVec3[((IOTA - 44) & 127)]) + ((0.00201686006f * fVec3[((IOTA - 49) & 127)]) + ((0.00207781f * fVec3[((IOTA - 51) & 127)]) + ((0.000178550996f * fVec3[((IOTA - 52) & 127)]) + ((0.00119524996f * fVec3[((IOTA - 53) & 127)]) + ((3.85171006e-05f * fVec3[((IOTA - 54) & 127)]) + ((0.00221503992f * fVec3[((IOTA - 55) & 127)]) + ((0.00354482001f * fVec3[((IOTA - 56) & 127)]) + ((0.00111367996f * fVec3[((IOTA - 57) & 127)]) + ((0.00102589f * fVec3[((IOTA - 62) & 127)]) + ((0.00390744023f * fVec3[((IOTA - 63) & 127)]) + ((0.00121048f * fVec3[((IOTA - 64) & 127)]) + ((0.00109229004f * fVec3[((IOTA - 84) & 127)]) + ((0.00150447001f * fVec3[((IOTA - 85) & 127)]) + ((0.00232614996f * fVec3[((IOTA - 86) & 127)]) + ((0.00406261999f * fVec3[((IOTA - 87) & 127)]) + ((0.00167906994f * fVec3[((IOTA - 91) & 127)]) + ((0.00275174994f * fVec3[((IOTA - 95) & 127)]) + ((0.00373391993f * fVec3[((IOTA - 96) & 127)]) + ((0.000116978998f * fVec3[((IOTA - 116) & 127)]) + ((3.24905013e-05f * fVec3[((IOTA - 117) & 127)]) + ((9.86781015e-05f * fVec3[((IOTA - 119) & 127)]) + ((0.000119185002f * fVec3[((IOTA - 120) & 127)]) + ((5.53095997e-05f * fVec3[((IOTA - 121) & 127)]) + ((5.56605983e-06f * fVec3[((IOTA - 122) & 127)]) + ((8.76003014e-06f * fVec3[((IOTA - 123) & 127)]) + ((3.31876004e-06f * fVec3[((IOTA - 125) & 127)]) + ((1.41881003e-06f * fVec3[((IOTA - 126) & 127)]) + ((1.50021997e-05f * fTemp3) + ((0.00842322968f * fVec3[((IOTA - 39) & 127)]) + ((0.0110774999f * fVec3[((IOTA - 43) & 127)]) + (0.00222904002f * fVec3[((IOTA - 42) & 127)]))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))) - ((0.000473650987f * fVec0[((IOTA - 97) & 127)]) + ((0.00196204009f * fVec0[((IOTA - 92) & 127)]) + ((1.77420006e-05f * fVec0[((IOTA - 91) & 127)]) + ((0.000875197002f * fVec0[((IOTA - 89) & 127)]) + ((0.0212955996f * fVec0[((IOTA - 24) & 127)]) + ((0.000160754003f * fVec0[((IOTA - 2) & 127)]) + ((0.000429698994f * fVec1[((IOTA - 6) & 127)]) + ((0.00101019f * fVec1[((IOTA - 8) & 127)]) + ((0.00149621f * fVec1[((IOTA - 10) & 127)]) + ((0.000929206028f * fVec1[((IOTA - 23) & 127)]) + ((0.000984566985f * fVec1[((IOTA - 74) & 127)]) + ((0.00115529995f * fVec1[((IOTA - 75) & 127)]) + ((0.000113130001f * fVec1[((IOTA - 81) & 127)]) + ((0.00130908005f * fVec1[((IOTA - 91) & 127)]) + ((2.38018001e-05f * fVec1[((IOTA - 106) & 127)]) + ((0.000270496006f * fVec2[((IOTA - 3) & 127)]) + ((2.50192011e-06f * fVec1[((IOTA - 120) & 127)]) + ((0.000249192002f * fVec2[((IOTA - 6) & 127)]) + ((0.00106220006f * fVec2[((IOTA - 7) & 127)]) + ((0.000950169982f * fVec2[((IOTA - 10) & 127)]) + ((0.000212469007f * fVec2[((IOTA - 11) & 127)]) + ((0.00564598013f * fVec2[((IOTA - 12) & 127)]) + ((0.0111431004f * fVec2[((IOTA - 15) & 127)]) + ((0.0141169f * fVec2[((IOTA - 16) & 127)]) + ((0.00249321992f * fVec2[((IOTA - 18) & 127)]) + ((0.043355599f * fVec2[((IOTA - 19) & 127)]) + ((0.00863904972f * fVec2[((IOTA - 25) & 127)]) + ((0.0196867008f * fVec2[((IOTA - 27) & 127)]) + ((0.00493340986f * fVec2[((IOTA - 30) & 127)]) + ((0.0143689001f * fVec2[((IOTA - 31) & 127)]) + ((0.00133630994f * fVec2[((IOTA - 34) & 127)]) + ((0.00786782987f * fVec2[((IOTA - 37) & 127)]) + ((0.0114674f * fVec2[((IOTA - 38) & 127)]) + ((0.000783797004f * fVec2[((IOTA - 56) & 127)]) + ((0.00220639003f * fVec2[((IOTA - 57) & 127)]) + ((0.00459737983f * fVec3[((IOTA - 35) & 127)]) + ((0.00134083f * fVec2[((IOTA - 64) & 127)]) + ((0.0025982901f * fVec2[((IOTA - 65) & 127)]) + ((0.00164578005f * fVec2[((IOTA - 66) & 127)]) + ((0.00275032991f * fVec2[((IOTA - 67) & 127)]) + ((0.00316541991f * fVec2[((IOTA - 68) & 127)]) + ((0.00238016993f * fVec2[((IOTA - 69) & 127)]) + ((0.00182602997f * fVec2[((IOTA - 70) & 127)]) + ((0.00196376001f * fVec2[((IOTA - 71) & 127)]) + ((0.00306352996f * fVec2[((IOTA - 72) & 127)]) + ((0.00137004f * fVec2[((IOTA - 73) & 127)]) + ((2.74177e-05f * fVec2[((IOTA - 82) & 127)]) + ((0.000465505989f * fVec2[((IOTA - 95) & 127)]) + ((0.00152734003f * fVec2[((IOTA - 96) & 127)]) + ((0.00103052997f * fVec2[((IOTA - 99) & 127)]) + ((0.00120271998f * fVec2[((IOTA - 100) & 127)]) + ((0.00136160001f * fVec2[((IOTA - 101) & 127)]) + ((0.00167489005f * fVec2[((IOTA - 102) & 127)]) + ((0.00186384004f * fVec2[((IOTA - 103) & 127)]) + ((0.00136554998f * fVec2[((IOTA - 104) & 127)]) + ((0.000828266027f * fVec2[((IOTA - 105) & 127)]) + ((0.00110552995f * fVec2[((IOTA - 106) & 127)]) + ((0.000979588018f * fVec2[((IOTA - 107) & 127)]) + ((0.000855899008f * fVec2[((IOTA - 108) & 127)]) + ((0.000947717985f * fVec2[((IOTA - 109) & 127)]) + ((0.000725086022f * fVec2[((IOTA - 110) & 127)]) + ((0.000665778993f * fVec2[((IOTA - 111) & 127)]) + ((0.000486784003f * fVec2[((IOTA - 112) & 127)]) + ((0.000263488997f * fVec2[((IOTA - 113) & 127)]) + ((6.42903033e-05f * fVec3[((IOTA - 3) & 127)]) + ((0.000197023997f * fVec3[((IOTA - 5) & 127)]) + ((0.000505773001f * fVec3[((IOTA - 7) & 127)]) + ((0.000216524f * fVec3[((IOTA - 11) & 127)]) + ((0.00373067008f * fVec3[((IOTA - 12) & 127)]) + ((0.000580219028f * fVec3[((IOTA - 14) & 127)]) + ((0.00439888984f * fVec3[((IOTA - 19) & 127)]) + ((0.0215706006f * fVec3[((IOTA - 21) & 127)]) + ((0.0575248003f * fVec3[((IOTA - 24) & 127)]) + ((0.00602849992f * fVec3[((IOTA - 25) & 127)]) + ((0.00144425f * fVec3[((IOTA - 27) & 127)]) + ((0.0391216986f * fVec3[((IOTA - 28) & 127)]) + ((0.0286455005f * fVec3[((IOTA - 32) & 127)]) + ((0.00434699003f * fVec3[((IOTA - 40) & 127)]) + ((0.0106381997f * fVec3[((IOTA - 41) & 127)]) + ((0.00166379998f * fVec3[((IOTA - 60) & 127)]) + ((0.00383124012f * fVec3[((IOTA - 61) & 127)]) + ((0.000509540027f * fVec3[((IOTA - 66) & 127)]) + ((0.00110707001f * fVec3[((IOTA - 67) & 127)]) + ((0.00233558007f * fVec3[((IOTA - 68) & 127)]) + ((0.00293582003f * fVec3[((IOTA - 69) & 127)]) + ((0.00181733002f * fVec3[((IOTA - 70) & 127)]) + ((0.00137873006f * fVec3[((IOTA - 71) & 127)]) + ((0.00103864004f * fVec3[((IOTA - 72) & 127)]) + ((0.000590750016f * fVec3[((IOTA - 73) & 127)]) + ((0.00190490996f * fVec3[((IOTA - 74) & 127)]) + ((0.00229879003f * fVec3[((IOTA - 75) & 127)]) + ((0.00100318005f * fVec3[((IOTA - 76) & 127)]) + ((0.00129805005f * fVec3[((IOTA - 77) & 127)]) + ((0.00124601997f * fVec3[((IOTA - 78) & 127)]) + ((0.00110995001f * fVec3[((IOTA - 79) & 127)]) + ((0.00113996002f * fVec3[((IOTA - 80) & 127)]) + ((0.00286674988f * fVec3[((IOTA - 81) & 127)]) + ((0.00273074f * fVec3[((IOTA - 82) & 127)]) + ((0.00170804001f * fVec3[((IOTA - 89) & 127)]) + ((0.00228614989f * fVec3[((IOTA - 93) & 127)]) + ((0.000409494998f * fVec3[((IOTA - 97) & 127)]) + ((0.00277688005f * fVec3[((IOTA - 98) & 127)]) + ((0.00130730995f * fVec3[((IOTA - 99) & 127)]) + ((0.000598906016f * fVec3[((IOTA - 100) & 127)]) + ((0.000739555981f * fVec3[((IOTA - 101) & 127)]) + ((0.000916639983f * fVec3[((IOTA - 102) & 127)]) + ((0.000640613027f * fVec3[((IOTA - 103) & 127)]) + ((0.000231076003f * fVec3[((IOTA - 104) & 127)]) + ((0.000829504977f * fVec3[((IOTA - 105) & 127)]) + ((0.000812940008f * fVec3[((IOTA - 106) & 127)]) + ((0.000643590989f * fVec3[((IOTA - 107) & 127)]) + ((0.000806326978f * fVec3[((IOTA - 108) & 127)]) + ((0.000702299993f * fVec3[((IOTA - 109) & 127)]) + ((0.000528144999f * fVec3[((IOTA - 110) & 127)]) + ((0.00028043901f * fVec3[((IOTA - 113) & 127)]) + ((0.000293775985f * fVec3[((IOTA - 114) & 127)]) + ((1.47262006e-06f * fVec0[((IOTA - 122) & 127)]) + ((0.000483849988f * fVec0[((IOTA - 98) & 127)]) + ((9.94087968e-05f * fVec0[((IOTA - 94) & 127)]) + ((0.00237059989f * fVec0[((IOTA - 93) & 127)]) + ((0.000611186028f * fVec0[((IOTA - 90) & 127)]) + ((0.000687956985f * fVec0[((IOTA - 82) & 127)]) + ((0.000362221996f * fVec0[((IOTA - 69) & 127)]) + ((0.00293952995f * fVec0[((IOTA - 32) & 127)]) + ((0.000920786988f * fVec0[((IOTA - 44) & 127)]) + ((0.000633171003f * fVec0[((IOTA - 57) & 127)]) + ((0.000150270003f * fVec1[((IOTA - 1) & 127)]) + ((0.000851531979f * fVec1[((IOTA - 3) & 127)]) + ((0.000849502976f * fVec1[((IOTA - 4) & 127)]) + ((0.00107232004f * fVec1[((IOTA - 5) & 127)]) + ((0.000453601999f * fVec1[((IOTA - 7) & 127)]) + ((0.000721279997f * fVec1[((IOTA - 9) & 127)]) + ((0.000673578004f * fVec1[((IOTA - 11) & 127)]) + ((0.0784898996f * fVec1[((IOTA - 12) & 127)]) + ((0.0829356015f * fVec1[((IOTA - 13) & 127)]) + ((0.0647670031f * fVec1[((IOTA - 14) & 127)]) + ((0.0772965029f * fVec1[((IOTA - 15) & 127)]) + ((0.0400927998f * fVec1[((IOTA - 16) & 127)]) + ((0.0977875963f * fVec1[((IOTA - 17) & 127)]) + ((0.0531960987f * fVec1[((IOTA - 18) & 127)]) + ((0.00881750975f * fVec1[((IOTA - 19) & 127)]) + ((0.0426613018f * fVec1[((IOTA - 20) & 127)]) + ((0.0413938016f * fVec1[((IOTA - 21) & 127)]) + ((0.080136098f * fVec1[((IOTA - 22) & 127)]) + ((0.00188931997f * fVec1[((IOTA - 72) & 127)]) + ((0.000597806997f * fVec1[((IOTA - 73) & 127)]) + ((0.00256507006f * fVec1[((IOTA - 78) & 127)]) + ((0.00247990992f * fVec1[((IOTA - 79) & 127)]) + ((0.00174382003f * fVec1[((IOTA - 80) & 127)]) + ((0.000615973026f * fVec1[((IOTA - 83) & 127)]) + ((0.00212734006f * fVec1[((IOTA - 84) & 127)]) + ((0.00144013006f * fVec1[((IOTA - 85) & 127)]) + ((0.000752906024f * fVec1[((IOTA - 86) & 127)]) + ((0.00167115999f * fVec1[((IOTA - 87) & 127)]) + ((0.00161011994f * fVec1[((IOTA - 88) & 127)]) + ((0.00127163006f * fVec1[((IOTA - 92) & 127)]) + ((0.00171690003f * fVec1[((IOTA - 93) & 127)]) + ((0.00205962011f * fVec1[((IOTA - 94) & 127)]) + ((0.00139563996f * fVec1[((IOTA - 95) & 127)]) + ((0.000696979987f * fVec1[((IOTA - 96) & 127)]) + ((3.75800009e-05f * fVec2[((IOTA - 1) & 127)]) + ((0.000650360016f * fVec1[((IOTA - 97) & 127)]) + ((0.000737778028f * fVec1[((IOTA - 98) & 127)]) + ((0.000778560003f * fVec1[((IOTA - 99) & 127)]) + ((0.00096599001f * fVec1[((IOTA - 100) & 127)]) + ((0.00105723995f * fVec1[((IOTA - 101) & 127)]) + ((0.000649337017f * fVec1[((IOTA - 102) & 127)]) + ((5.1123402e-06f * fVec2[((IOTA - 2) & 127)]) + ((0.000421801989f * fVec1[((IOTA - 103) & 127)]) + ((0.000406934007f * fVec1[((IOTA - 104) & 127)]) + ((0.000415783987f * fVec1[((IOTA - 105) & 127)]) + ((0.000154768f * fVec1[((IOTA - 107) & 127)]) + ((2.47097996e-05f * fTemp1) + ((0.000976993004f * fVec2[((IOTA - 13) & 127)]) + ((0.00184861f * fVec2[((IOTA - 14) & 127)]) + ((0.00898901001f * fVec2[((IOTA - 32) & 127)]) + ((0.0038962001f * fVec2[((IOTA - 39) & 127)]) + ((0.00263441005f * fVec2[((IOTA - 44) & 127)]) + ((0.00122610002f * fVec2[((IOTA - 45) & 127)]) + ((0.000785113021f * fVec2[((IOTA - 51) & 127)]) + ((0.000229878002f * fVec2[((IOTA - 52) & 127)]) + ((0.00152893004f * fVec2[((IOTA - 58) & 127)]) + ((0.00122217997f * fVec2[((IOTA - 59) & 127)]) + ((0.000680484984f * fVec2[((IOTA - 60) & 127)]) + ((0.00109607005f * fVec2[((IOTA - 77) & 127)]) + ((0.000216738001f * fVec2[((IOTA - 84) & 127)]) + ((0.000439841009f * fVec2[((IOTA - 85) & 127)]) + ((0.00113466999f * fVec2[((IOTA - 92) & 127)]) + ((0.00102503004f * fVec2[((IOTA - 93) & 127)]) + ((0.00038473701f * fVec2[((IOTA - 94) & 127)]) + ((0.00128148997f * fVec2[((IOTA - 97) & 127)]) + ((0.000403028011f * fVec2[((IOTA - 98) & 127)]) + ((3.48346002e-05f * fVec2[((IOTA - 116) & 127)]) + ((5.18511988e-05f * fVec2[((IOTA - 117) & 127)]) + ((7.89040996e-07f * fVec2[((IOTA - 118) & 127)]) + ((1.04261999e-05f * fTemp2) + ((0.00731860008f * fVec3[((IOTA - 36) & 127)]) + ((0.00157590001f * fVec3[((IOTA - 37) & 127)]) + ((0.00736801978f * fVec3[((IOTA - 16) & 127)]) + ((0.00147934002f * fVec3[((IOTA - 20) & 127)]) + ((0.00924876984f * fVec3[((IOTA - 29) & 127)]) + ((0.0131834f * fVec3[((IOTA - 33) & 127)]) + ((0.00139329f * fVec3[((IOTA - 45) & 127)]) + ((0.00437394017f * fVec3[((IOTA - 48) & 127)]) + ((0.000445109996f * fVec3[((IOTA - 65) & 127)]) + ((0.000162740005f * fVec3[((IOTA - 83) & 127)]) + ((0.00137700001f * fVec3[((IOTA - 90) & 127)]) + ((7.43291967e-05f * fVec3[((IOTA - 115) & 127)]) + (0.00145302003f * fVec3[((IOTA - 94) & 127)])))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))));
			fRec7[0] = max((fRec7[1] - fConst0), min(6.0f, (20.0f * log10f(max(0.000316227757f, fabsf(fTemp5))))));
			fHbargraph1 = FAUSTFLOAT(fRec7[0]);
			output1[i] = FAUSTFLOAT(fTemp5);
			fRec1[1] = fRec1[0];
			fRec3[1] = fRec3[0];
			fRec2[1] = fRec2[0];
			IOTA = (IOTA + 1);
			fRec4[1] = fRec4[0];
			fRec5[1] = fRec5[0];
			fRec6[1] = fRec6[0];
			fRec0[1] = fRec0[0];
			fRec7[1] = fRec7[0];
			
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
