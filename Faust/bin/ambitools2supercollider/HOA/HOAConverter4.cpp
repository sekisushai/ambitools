/* ------------------------------------------------------------
author: "Pierre Lecomte"
copyright: "(c) Pierre Lecomte 2016"
license: "GPL"
name: "HOAConverter4"
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

double rint(double dummy0);
double fabs(double dummy0);
double log10(double dummy0);

#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
#endif

class mydsp : public dsp {
	
 private:
	
	int fSamplingFreq;
	double fRec1[2];
	double fRec0[2];
	double fRec3[2];
	double fRec4[2];
	double fRec5[2];
	double fRec2[2];
	double fRec6[2];
	double fRec7[2];
	double fRec9[2];
	double fRec10[2];
	double fRec11[2];
	double fRec8[2];
	double fRec13[2];
	double fRec14[2];
	double fRec12[2];
	double fRec15[2];
	double fRec16[2];
	double fRec17[2];
	double fRec19[2];
	double fRec20[2];
	double fRec21[2];
	double fRec18[2];
	double fRec23[2];
	double fRec24[2];
	double fRec22[2];
	double fRec26[2];
	double fRec25[2];
	double fRec28[2];
	double fRec27[2];
	double fRec29[2];
	double fRec30[2];
	double fRec31[2];
	double fRec33[2];
	double fRec32[2];
	double fRec35[2];
	double fRec34[2];
	double fRec37[2];
	double fRec36[2];
	double fRec39[2];
	double fRec38[2];
	double fRec41[2];
	double fRec40[2];
	double fRec43[2];
	double fRec42[2];
	double fRec45[2];
	double fRec44[2];
	double fRec47[2];
	double fRec46[2];
	double fRec49[2];
	double fRec48[2];
	double fConst0;
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fHslider1;
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
		m->declare("copyright", "(c) Pierre Lecomte 2016");
		m->declare("lib/gui.lib/author", "Pierre Lecomte");
		m->declare("lib/gui.lib/copyright", "(c) Pierre Lecomte 2016");
		m->declare("lib/gui.lib/license", "GPL");
		m->declare("lib/gui.lib/name", "GUI Library");
		m->declare("lib/gui.lib/version", "1.0");
		m->declare("license", "GPL");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.0");
		m->declare("name", "HOAConverter4");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
		m->declare("version", "1.0");
	}

	virtual int getNumInputs() {
		return 25;
		
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
		fConst0 = (80.0 / min(192000.0, max(1.0, double(fSamplingFreq))));
		
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(1.0);
		fHslider1 = FAUSTFLOAT(1.0);
		
	}
	
	virtual void instanceClear() {
		for (int i0 = 0; (i0 < 2); i0 = (i0 + 1)) {
			fRec1[i0] = 0.0;
			
		}
		for (int i1 = 0; (i1 < 2); i1 = (i1 + 1)) {
			fRec0[i1] = 0.0;
			
		}
		for (int i2 = 0; (i2 < 2); i2 = (i2 + 1)) {
			fRec3[i2] = 0.0;
			
		}
		for (int i3 = 0; (i3 < 2); i3 = (i3 + 1)) {
			fRec4[i3] = 0.0;
			
		}
		for (int i4 = 0; (i4 < 2); i4 = (i4 + 1)) {
			fRec5[i4] = 0.0;
			
		}
		for (int i5 = 0; (i5 < 2); i5 = (i5 + 1)) {
			fRec2[i5] = 0.0;
			
		}
		for (int i6 = 0; (i6 < 2); i6 = (i6 + 1)) {
			fRec6[i6] = 0.0;
			
		}
		for (int i7 = 0; (i7 < 2); i7 = (i7 + 1)) {
			fRec7[i7] = 0.0;
			
		}
		for (int i8 = 0; (i8 < 2); i8 = (i8 + 1)) {
			fRec9[i8] = 0.0;
			
		}
		for (int i9 = 0; (i9 < 2); i9 = (i9 + 1)) {
			fRec10[i9] = 0.0;
			
		}
		for (int i10 = 0; (i10 < 2); i10 = (i10 + 1)) {
			fRec11[i10] = 0.0;
			
		}
		for (int i11 = 0; (i11 < 2); i11 = (i11 + 1)) {
			fRec8[i11] = 0.0;
			
		}
		for (int i12 = 0; (i12 < 2); i12 = (i12 + 1)) {
			fRec13[i12] = 0.0;
			
		}
		for (int i13 = 0; (i13 < 2); i13 = (i13 + 1)) {
			fRec14[i13] = 0.0;
			
		}
		for (int i14 = 0; (i14 < 2); i14 = (i14 + 1)) {
			fRec12[i14] = 0.0;
			
		}
		for (int i15 = 0; (i15 < 2); i15 = (i15 + 1)) {
			fRec15[i15] = 0.0;
			
		}
		for (int i16 = 0; (i16 < 2); i16 = (i16 + 1)) {
			fRec16[i16] = 0.0;
			
		}
		for (int i17 = 0; (i17 < 2); i17 = (i17 + 1)) {
			fRec17[i17] = 0.0;
			
		}
		for (int i18 = 0; (i18 < 2); i18 = (i18 + 1)) {
			fRec19[i18] = 0.0;
			
		}
		for (int i19 = 0; (i19 < 2); i19 = (i19 + 1)) {
			fRec20[i19] = 0.0;
			
		}
		for (int i20 = 0; (i20 < 2); i20 = (i20 + 1)) {
			fRec21[i20] = 0.0;
			
		}
		for (int i21 = 0; (i21 < 2); i21 = (i21 + 1)) {
			fRec18[i21] = 0.0;
			
		}
		for (int i22 = 0; (i22 < 2); i22 = (i22 + 1)) {
			fRec23[i22] = 0.0;
			
		}
		for (int i23 = 0; (i23 < 2); i23 = (i23 + 1)) {
			fRec24[i23] = 0.0;
			
		}
		for (int i24 = 0; (i24 < 2); i24 = (i24 + 1)) {
			fRec22[i24] = 0.0;
			
		}
		for (int i25 = 0; (i25 < 2); i25 = (i25 + 1)) {
			fRec26[i25] = 0.0;
			
		}
		for (int i26 = 0; (i26 < 2); i26 = (i26 + 1)) {
			fRec25[i26] = 0.0;
			
		}
		for (int i27 = 0; (i27 < 2); i27 = (i27 + 1)) {
			fRec28[i27] = 0.0;
			
		}
		for (int i28 = 0; (i28 < 2); i28 = (i28 + 1)) {
			fRec27[i28] = 0.0;
			
		}
		for (int i29 = 0; (i29 < 2); i29 = (i29 + 1)) {
			fRec29[i29] = 0.0;
			
		}
		for (int i30 = 0; (i30 < 2); i30 = (i30 + 1)) {
			fRec30[i30] = 0.0;
			
		}
		for (int i31 = 0; (i31 < 2); i31 = (i31 + 1)) {
			fRec31[i31] = 0.0;
			
		}
		for (int i32 = 0; (i32 < 2); i32 = (i32 + 1)) {
			fRec33[i32] = 0.0;
			
		}
		for (int i33 = 0; (i33 < 2); i33 = (i33 + 1)) {
			fRec32[i33] = 0.0;
			
		}
		for (int i34 = 0; (i34 < 2); i34 = (i34 + 1)) {
			fRec35[i34] = 0.0;
			
		}
		for (int i35 = 0; (i35 < 2); i35 = (i35 + 1)) {
			fRec34[i35] = 0.0;
			
		}
		for (int i36 = 0; (i36 < 2); i36 = (i36 + 1)) {
			fRec37[i36] = 0.0;
			
		}
		for (int i37 = 0; (i37 < 2); i37 = (i37 + 1)) {
			fRec36[i37] = 0.0;
			
		}
		for (int i38 = 0; (i38 < 2); i38 = (i38 + 1)) {
			fRec39[i38] = 0.0;
			
		}
		for (int i39 = 0; (i39 < 2); i39 = (i39 + 1)) {
			fRec38[i39] = 0.0;
			
		}
		for (int i40 = 0; (i40 < 2); i40 = (i40 + 1)) {
			fRec41[i40] = 0.0;
			
		}
		for (int i41 = 0; (i41 < 2); i41 = (i41 + 1)) {
			fRec40[i41] = 0.0;
			
		}
		for (int i42 = 0; (i42 < 2); i42 = (i42 + 1)) {
			fRec43[i42] = 0.0;
			
		}
		for (int i43 = 0; (i43 < 2); i43 = (i43 + 1)) {
			fRec42[i43] = 0.0;
			
		}
		for (int i44 = 0; (i44 < 2); i44 = (i44 + 1)) {
			fRec45[i44] = 0.0;
			
		}
		for (int i45 = 0; (i45 < 2); i45 = (i45 + 1)) {
			fRec44[i45] = 0.0;
			
		}
		for (int i46 = 0; (i46 < 2); i46 = (i46 + 1)) {
			fRec47[i46] = 0.0;
			
		}
		for (int i47 = 0; (i47 < 2); i47 = (i47 + 1)) {
			fRec46[i47] = 0.0;
			
		}
		for (int i48 = 0; (i48 < 2); i48 = (i48 + 1)) {
			fRec49[i48] = 0.0;
			
		}
		for (int i49 = 0; (i49 < 2); i49 = (i49 + 1)) {
			fRec48[i49] = 0.0;
			
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
		ui_interface->declare(&fHslider1, "0", "");
		ui_interface->declare(&fHslider1, "style", "knob");
		ui_interface->addHorizontalSlider("Input (1-ACN_N3D,2-ACN_SN3D, 3-FuMa)", &fHslider1, 1.0, 1.0, 3.0, 1.0);
		ui_interface->declare(&fHslider0, "0", "");
		ui_interface->declare(&fHslider0, "style", "knob");
		ui_interface->addHorizontalSlider("Output (1-ACN_N3D,2-ACN_SN3D, 3-FuMa)", &fHslider0, 1.0, 1.0, 3.0, 1.0);
		ui_interface->declare(0, "1", "");
		ui_interface->openHorizontalBox("Inputs");
		ui_interface->openHorizontalBox("0");
		ui_interface->openVerticalBox("0");
		ui_interface->declare(&fVbargraph0, "unit", "dB");
		ui_interface->addVerticalBargraph("0x33b9d10", &fVbargraph0, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("1");
		ui_interface->openVerticalBox("1");
		ui_interface->declare(&fVbargraph2, "unit", "dB");
		ui_interface->addVerticalBargraph("0x33c6da0", &fVbargraph2, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("2");
		ui_interface->declare(&fVbargraph4, "unit", "dB");
		ui_interface->addVerticalBargraph("0x33d3060", &fVbargraph4, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("3");
		ui_interface->declare(&fVbargraph3, "unit", "dB");
		ui_interface->addVerticalBargraph("0x33cc790", &fVbargraph3, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2");
		ui_interface->openVerticalBox("4");
		ui_interface->declare(&fVbargraph8, "unit", "dB");
		ui_interface->addVerticalBargraph("0x33ea1c0", &fVbargraph8, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("5");
		ui_interface->declare(&fVbargraph12, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3400f30", &fVbargraph12, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("6");
		ui_interface->declare(&fVbargraph9, "unit", "dB");
		ui_interface->addVerticalBargraph("0x33efa80", &fVbargraph9, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("7");
		ui_interface->declare(&fVbargraph13, "unit", "dB");
		ui_interface->addVerticalBargraph("0x34067f0", &fVbargraph13, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("8");
		ui_interface->declare(&fVbargraph10, "unit", "dB");
		ui_interface->addVerticalBargraph("0x33f5ed0", &fVbargraph10, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3");
		ui_interface->openVerticalBox("9");
		ui_interface->declare(&fVbargraph18, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3426410", &fVbargraph18, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("10");
		ui_interface->declare(&fVbargraph22, "unit", "dB");
		ui_interface->addVerticalBargraph("0x343d500", &fVbargraph22, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("11");
		ui_interface->declare(&fVbargraph25, "unit", "dB");
		ui_interface->addVerticalBargraph("0x344f160", &fVbargraph25, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("12");
		ui_interface->declare(&fVbargraph19, "unit", "dB");
		ui_interface->addVerticalBargraph("0x342bdb0", &fVbargraph19, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("13");
		ui_interface->declare(&fVbargraph23, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3442ea0", &fVbargraph23, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("14");
		ui_interface->declare(&fVbargraph27, "unit", "dB");
		ui_interface->addVerticalBargraph("0x345b9c0", &fVbargraph27, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("15");
		ui_interface->declare(&fVbargraph20, "unit", "dB");
		ui_interface->addVerticalBargraph("0x34322e0", &fVbargraph20, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4");
		ui_interface->openVerticalBox("16");
		ui_interface->declare(&fVbargraph32, "unit", "dB");
		ui_interface->addVerticalBargraph("0x347c880", &fVbargraph32, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("17");
		ui_interface->declare(&fVbargraph34, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3488590", &fVbargraph34, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("18");
		ui_interface->declare(&fVbargraph36, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3493e00", &fVbargraph36, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("19");
		ui_interface->declare(&fVbargraph38, "unit", "dB");
		ui_interface->addVerticalBargraph("0x349f830", &fVbargraph38, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("20");
		ui_interface->declare(&fVbargraph40, "unit", "dB");
		ui_interface->addVerticalBargraph("0x34ab420", &fVbargraph40, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("21");
		ui_interface->declare(&fVbargraph42, "unit", "dB");
		ui_interface->addVerticalBargraph("0x34b71d0", &fVbargraph42, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("22");
		ui_interface->declare(&fVbargraph44, "unit", "dB");
		ui_interface->addVerticalBargraph("0x34c3140", &fVbargraph44, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("23");
		ui_interface->declare(&fVbargraph46, "unit", "dB");
		ui_interface->addVerticalBargraph("0x34cf270", &fVbargraph46, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("24");
		ui_interface->declare(&fVbargraph48, "unit", "dB");
		ui_interface->addVerticalBargraph("0x34db560", &fVbargraph48, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->declare(0, "2", "");
		ui_interface->openHorizontalBox("Outputs");
		ui_interface->openHorizontalBox("0");
		ui_interface->openVerticalBox("0");
		ui_interface->declare(&fVbargraph1, "unit", "dB");
		ui_interface->addVerticalBargraph("0x33c0ce0", &fVbargraph1, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("1");
		ui_interface->openVerticalBox("1");
		ui_interface->declare(&fVbargraph5, "unit", "dB");
		ui_interface->addVerticalBargraph("0x33d8120", &fVbargraph5, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("2");
		ui_interface->declare(&fVbargraph6, "unit", "dB");
		ui_interface->addVerticalBargraph("0x33de660", &fVbargraph6, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("3");
		ui_interface->declare(&fVbargraph7, "unit", "dB");
		ui_interface->addVerticalBargraph("0x33e44a0", &fVbargraph7, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2");
		ui_interface->openVerticalBox("4");
		ui_interface->declare(&fVbargraph11, "unit", "dB");
		ui_interface->addVerticalBargraph("0x33fb2f0", &fVbargraph11, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("5");
		ui_interface->declare(&fVbargraph14, "unit", "dB");
		ui_interface->addVerticalBargraph("0x340c830", &fVbargraph14, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("6");
		ui_interface->declare(&fVbargraph15, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3412cf0", &fVbargraph15, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("7");
		ui_interface->declare(&fVbargraph16, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3419830", &fVbargraph16, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("8");
		ui_interface->declare(&fVbargraph17, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3420450", &fVbargraph17, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("3");
		ui_interface->openVerticalBox("9");
		ui_interface->declare(&fVbargraph21, "unit", "dB");
		ui_interface->addVerticalBargraph("0x34377e0", &fVbargraph21, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("10");
		ui_interface->declare(&fVbargraph24, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3449360", &fVbargraph24, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("11");
		ui_interface->declare(&fVbargraph26, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3455730", &fVbargraph26, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("12");
		ui_interface->declare(&fVbargraph28, "unit", "dB");
		ui_interface->addVerticalBargraph("0x34618e0", &fVbargraph28, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("13");
		ui_interface->declare(&fVbargraph29, "unit", "dB");
		ui_interface->addVerticalBargraph("0x34685e0", &fVbargraph29, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("14");
		ui_interface->declare(&fVbargraph30, "unit", "dB");
		ui_interface->addVerticalBargraph("0x346f760", &fVbargraph30, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("15");
		ui_interface->declare(&fVbargraph31, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3476620", &fVbargraph31, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("4");
		ui_interface->openVerticalBox("16");
		ui_interface->declare(&fVbargraph33, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3482790", &fVbargraph33, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("17");
		ui_interface->declare(&fVbargraph35, "unit", "dB");
		ui_interface->addVerticalBargraph("0x348df20", &fVbargraph35, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("18");
		ui_interface->declare(&fVbargraph37, "unit", "dB");
		ui_interface->addVerticalBargraph("0x3499870", &fVbargraph37, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("19");
		ui_interface->declare(&fVbargraph39, "unit", "dB");
		ui_interface->addVerticalBargraph("0x34a5380", &fVbargraph39, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("20");
		ui_interface->declare(&fVbargraph41, "unit", "dB");
		ui_interface->addVerticalBargraph("0x34b1050", &fVbargraph41, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("21");
		ui_interface->declare(&fVbargraph43, "unit", "dB");
		ui_interface->addVerticalBargraph("0x34bcee0", &fVbargraph43, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("22");
		ui_interface->declare(&fVbargraph45, "unit", "dB");
		ui_interface->addVerticalBargraph("0x34c8f30", &fVbargraph45, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("23");
		ui_interface->declare(&fVbargraph47, "unit", "dB");
		ui_interface->addVerticalBargraph("0x34d5140", &fVbargraph47, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("24");
		ui_interface->declare(&fVbargraph49, "unit", "dB");
		ui_interface->addVerticalBargraph("0x34e1510", &fVbargraph49, -70.0, 6.0);
		ui_interface->closeBox();
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
		int iSlow0 = int(((rint(double(fHslider0)) + (3.0 * rint(double(fHslider1)))) + -4.0));
		int iSlow1 = (iSlow0 >= 5);
		int iSlow2 = (iSlow0 >= 3);
		int iSlow3 = (iSlow0 >= 2);
		int iSlow4 = (iSlow0 >= 7);
		int iSlow5 = (iSlow0 >= 6);
		int iSlow6 = (iSlow0 >= 8);
		int iSlow7 = (iSlow0 >= 1);
		int iSlow8 = (iSlow0 >= 4);
		for (int i = 0; (i < count); i = (i + 1)) {
			double fTemp0 = double(input0[i]);
			fRec1[0] = max((fRec1[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp0))))));
			fVbargraph0 = FAUSTFLOAT(fRec1[0]);
			double fTemp1 = (0.70710678118654746 * fTemp0);
			double fTemp2 = (1.4142135623730951 * fTemp0);
			double fTemp3 = (iSlow1?(iSlow4?(iSlow6?fTemp0:fTemp2):(iSlow5?fTemp2:fTemp1)):(iSlow2?fTemp0:(iSlow3?fTemp1:fTemp0)));
			fRec0[0] = max((fRec0[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp3))))));
			fVbargraph1 = FAUSTFLOAT(fRec0[0]);
			output0[i] = FAUSTFLOAT(fTemp3);
			double fTemp4 = double(input1[i]);
			fRec3[0] = max((fRec3[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp4))))));
			fVbargraph2 = FAUSTFLOAT(fRec3[0]);
			double fTemp5 = (0.57735026918962584 * fTemp4);
			double fTemp6 = double(input3[i]);
			fRec4[0] = max((fRec4[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp6))))));
			fVbargraph3 = FAUSTFLOAT(fRec4[0]);
			double fTemp7 = (0.57735026918962584 * fTemp6);
			double fTemp8 = (1.7320508075688772 * fTemp4);
			double fTemp9 = double(input2[i]);
			fRec5[0] = max((fRec5[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp9))))));
			fVbargraph4 = FAUSTFLOAT(fRec5[0]);
			double fTemp10 = (1.7320508075688772 * fTemp9);
			double fTemp11 = (iSlow1?(iSlow4?(iSlow6?fTemp4:fTemp9):(iSlow5?fTemp10:fTemp6)):(iSlow2?(iSlow8?fTemp4:fTemp8):(iSlow3?fTemp7:(iSlow7?fTemp5:fTemp4))));
			fRec2[0] = max((fRec2[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp11))))));
			fVbargraph5 = FAUSTFLOAT(fRec2[0]);
			output1[i] = FAUSTFLOAT(fTemp11);
			double fTemp12 = (0.57735026918962584 * fTemp9);
			double fTemp13 = (1.7320508075688772 * fTemp6);
			double fTemp14 = (iSlow1?(iSlow4?(iSlow6?fTemp9:fTemp6):(iSlow5?fTemp13:fTemp4)):(iSlow2?(iSlow8?fTemp9:fTemp10):(iSlow3?fTemp5:(iSlow7?fTemp12:fTemp9))));
			fRec6[0] = max((fRec6[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp14))))));
			fVbargraph6 = FAUSTFLOAT(fRec6[0]);
			output2[i] = FAUSTFLOAT(fTemp14);
			double fTemp15 = (iSlow1?(iSlow4?(iSlow6?fTemp6:fTemp4):(iSlow5?fTemp8:fTemp9)):(iSlow2?(iSlow8?fTemp6:fTemp13):(iSlow3?fTemp12:(iSlow7?fTemp7:fTemp6))));
			fRec7[0] = max((fRec7[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp15))))));
			fVbargraph7 = FAUSTFLOAT(fRec7[0]);
			output3[i] = FAUSTFLOAT(fTemp15);
			double fTemp16 = double(input4[i]);
			fRec9[0] = max((fRec9[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp16))))));
			fVbargraph8 = FAUSTFLOAT(fRec9[0]);
			double fTemp17 = double(input6[i]);
			fRec10[0] = max((fRec10[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp17))))));
			fVbargraph9 = FAUSTFLOAT(fRec10[0]);
			double fTemp18 = (0.44721359549995793 * fTemp17);
			double fTemp19 = (2.2360679774997898 * fTemp16);
			double fTemp20 = double(input8[i]);
			fRec11[0] = max((fRec11[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp20))))));
			fVbargraph10 = FAUSTFLOAT(fRec11[0]);
			double fTemp21 = (iSlow1?(iSlow4?(iSlow6?fTemp16:(0.86602540378443871 * fTemp20)):(iSlow5?(1.9364916731037085 * fTemp20):fTemp17)):(iSlow2?(iSlow8?fTemp16:fTemp19):(iSlow3?fTemp18:(iSlow7?(0.44721359549995793 * fTemp16):fTemp16))));
			fRec8[0] = max((fRec8[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp21))))));
			fVbargraph11 = FAUSTFLOAT(fRec8[0]);
			output4[i] = FAUSTFLOAT(fTemp21);
			double fTemp22 = double(input5[i]);
			fRec13[0] = max((fRec13[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp22))))));
			fVbargraph12 = FAUSTFLOAT(fRec13[0]);
			double fTemp23 = double(input7[i]);
			fRec14[0] = max((fRec14[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp23))))));
			fVbargraph13 = FAUSTFLOAT(fRec14[0]);
			double fTemp24 = (iSlow1?(iSlow4?(iSlow6?fTemp22:(0.86602540378443871 * fTemp17)):(iSlow5?(1.9364916731037085 * fTemp17):(1.1547005383792515 * fTemp23))):(iSlow2?(iSlow8?fTemp22:(2.2360679774997898 * fTemp22)):(iSlow3?(0.5163977794943222 * fTemp23):(iSlow7?(0.44721359549995793 * fTemp22):fTemp22))));
			fRec12[0] = max((fRec12[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp24))))));
			fVbargraph14 = FAUSTFLOAT(fRec12[0]);
			output5[i] = FAUSTFLOAT(fTemp24);
			double fTemp25 = (iSlow1?(iSlow4?(iSlow6?fTemp17:fTemp16):(iSlow5?fTemp19:(1.1547005383792515 * fTemp22))):(iSlow2?(iSlow8?fTemp17:(2.2360679774997898 * fTemp17)):(iSlow3?(0.5163977794943222 * fTemp22):(iSlow7?fTemp18:fTemp17))));
			fRec15[0] = max((fRec15[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp25))))));
			fVbargraph15 = FAUSTFLOAT(fRec15[0]);
			output6[i] = FAUSTFLOAT(fTemp25);
			double fTemp26 = (iSlow1?(iSlow4?(iSlow6?fTemp23:(0.86602540378443871 * fTemp22)):(iSlow5?(1.9364916731037085 * fTemp22):(1.1547005383792515 * fTemp20))):(iSlow2?(iSlow8?fTemp23:(2.2360679774997898 * fTemp23)):(iSlow3?(0.5163977794943222 * fTemp20):(iSlow7?(0.44721359549995793 * fTemp23):fTemp23))));
			fRec16[0] = max((fRec16[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp26))))));
			fVbargraph16 = FAUSTFLOAT(fRec16[0]);
			output7[i] = FAUSTFLOAT(fTemp26);
			double fTemp27 = (iSlow1?(iSlow4?(iSlow6?fTemp20:(0.86602540378443871 * fTemp23)):(iSlow5?(1.9364916731037085 * fTemp23):(1.1547005383792515 * fTemp16))):(iSlow2?(iSlow8?fTemp20:(2.2360679774997898 * fTemp20)):(iSlow3?(0.5163977794943222 * fTemp16):(iSlow7?(0.44721359549995793 * fTemp20):fTemp20))));
			fRec17[0] = max((fRec17[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp27))))));
			fVbargraph17 = FAUSTFLOAT(fRec17[0]);
			output8[i] = FAUSTFLOAT(fTemp27);
			double fTemp28 = double(input9[i]);
			fRec19[0] = max((fRec19[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp28))))));
			fVbargraph18 = FAUSTFLOAT(fRec19[0]);
			double fTemp29 = double(input12[i]);
			fRec20[0] = max((fRec20[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp29))))));
			fVbargraph19 = FAUSTFLOAT(fRec20[0]);
			double fTemp30 = (0.3779644730092272 * fTemp29);
			double fTemp31 = (2.6457513110645907 * fTemp28);
			double fTemp32 = double(input15[i]);
			fRec21[0] = max((fRec21[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp32))))));
			fVbargraph20 = FAUSTFLOAT(fRec21[0]);
			double fTemp33 = (iSlow1?(iSlow4?(iSlow6?fTemp28:(0.79056941504209477 * fTemp32)):(iSlow5?(2.0916500663351889 * fTemp32):fTemp29)):(iSlow2?(iSlow8?fTemp28:fTemp31):(iSlow3?fTemp30:(iSlow7?(0.3779644730092272 * fTemp28):fTemp28))));
			fRec18[0] = max((fRec18[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp33))))));
			fVbargraph21 = FAUSTFLOAT(fRec18[0]);
			output9[i] = FAUSTFLOAT(fTemp33);
			double fTemp34 = double(input10[i]);
			fRec23[0] = max((fRec23[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp34))))));
			fVbargraph22 = FAUSTFLOAT(fRec23[0]);
			double fTemp35 = double(input13[i]);
			fRec24[0] = max((fRec24[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp35))))));
			fVbargraph23 = FAUSTFLOAT(fRec24[0]);
			double fTemp36 = (iSlow1?(iSlow4?(iSlow6?fTemp34:(0.7453559924999299 * fTemp35)):(iSlow5?(1.9720265943665387 * fTemp35):(1.1858541225631423 * fTemp35))):(iSlow2?(iSlow8?fTemp34:(2.6457513110645907 * fTemp34)):(iSlow3?(0.44821072850039761 * fTemp35):(iSlow7?(0.3779644730092272 * fTemp34):fTemp34))));
			fRec22[0] = max((fRec22[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp36))))));
			fVbargraph24 = FAUSTFLOAT(fRec22[0]);
			output10[i] = FAUSTFLOAT(fTemp36);
			double fTemp37 = double(input11[i]);
			fRec26[0] = max((fRec26[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp37))))));
			fVbargraph25 = FAUSTFLOAT(fRec26[0]);
			double fTemp38 = (iSlow1?(iSlow4?(iSlow6?fTemp37:(0.84327404271156781 * fTemp37)):(iSlow5?(2.2310934040908683 * fTemp37):(1.1858541225631423 * fTemp37))):(iSlow2?(iSlow8?fTemp37:(2.6457513110645907 * fTemp37)):(iSlow3?(0.44821072850039761 * fTemp37):(iSlow7?(0.3779644730092272 * fTemp37):fTemp37))));
			fRec25[0] = max((fRec25[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp38))))));
			fVbargraph26 = FAUSTFLOAT(fRec25[0]);
			output11[i] = FAUSTFLOAT(fTemp38);
			double fTemp39 = double(input14[i]);
			fRec28[0] = max((fRec28[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp39))))));
			fVbargraph27 = FAUSTFLOAT(fRec28[0]);
			double fTemp40 = (iSlow1?(iSlow4?(iSlow6?fTemp29:fTemp28):(iSlow5?fTemp31:(1.3416407864998738 * fTemp39))):(iSlow2?(iSlow8?fTemp29:(2.6457513110645907 * fTemp29)):(iSlow3?(0.50709255283710997 * fTemp39):(iSlow7?fTemp30:fTemp29))));
			fRec27[0] = max((fRec27[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp40))))));
			fVbargraph28 = FAUSTFLOAT(fRec27[0]);
			output12[i] = FAUSTFLOAT(fTemp40);
			double fTemp41 = (iSlow1?(iSlow4?(iSlow6?fTemp35:(0.84327404271156781 * fTemp34)):(iSlow5?(2.2310934040908683 * fTemp34):(1.3416407864998738 * fTemp34))):(iSlow2?(iSlow8?fTemp35:(2.6457513110645907 * fTemp35)):(iSlow3?(0.50709255283710997 * fTemp34):(iSlow7?(0.3779644730092272 * fTemp35):fTemp35))));
			fRec29[0] = max((fRec29[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp41))))));
			fVbargraph29 = FAUSTFLOAT(fRec29[0]);
			output13[i] = FAUSTFLOAT(fTemp41);
			double fTemp42 = (iSlow1?(iSlow4?(iSlow6?fTemp39:(0.7453559924999299 * fTemp29)):(iSlow5?(1.9720265943665387 * fTemp29):(1.2649110640673518 * fTemp32))):(iSlow2?(iSlow8?fTemp39:(2.6457513110645907 * fTemp39)):(iSlow3?(0.47809144373375745 * fTemp32):(iSlow7?(0.3779644730092272 * fTemp39):fTemp39))));
			fRec30[0] = max((fRec30[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp42))))));
			fVbargraph30 = FAUSTFLOAT(fRec30[0]);
			output14[i] = FAUSTFLOAT(fTemp42);
			double fTemp43 = (iSlow1?(iSlow4?(iSlow6?fTemp32:(0.79056941504209477 * fTemp39)):(iSlow5?(2.0916500663351889 * fTemp39):(1.2649110640673518 * fTemp28))):(iSlow2?(iSlow8?fTemp32:(2.6457513110645907 * fTemp32)):(iSlow3?(0.47809144373375745 * fTemp28):(iSlow7?(0.3779644730092272 * fTemp32):fTemp32))));
			fRec31[0] = max((fRec31[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp43))))));
			fVbargraph31 = FAUSTFLOAT(fRec31[0]);
			output15[i] = FAUSTFLOAT(fTemp43);
			double fTemp44 = double(input16[i]);
			fRec33[0] = max((fRec33[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp44))))));
			fVbargraph32 = FAUSTFLOAT(fRec33[0]);
			double fTemp45 = (iSlow1?(iSlow4?(iSlow6?fTemp44:0.0):0.0):(iSlow2?(iSlow8?fTemp44:(3.0 * fTemp44)):(iSlow3?0.0:(iSlow7?(0.33333333333333331 * fTemp44):fTemp44))));
			fRec32[0] = max((fRec32[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp45))))));
			fVbargraph33 = FAUSTFLOAT(fRec32[0]);
			output16[i] = FAUSTFLOAT(fTemp45);
			double fTemp46 = double(input17[i]);
			fRec35[0] = max((fRec35[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp46))))));
			fVbargraph34 = FAUSTFLOAT(fRec35[0]);
			double fTemp47 = (iSlow1?(iSlow4?(iSlow6?fTemp46:0.0):0.0):(iSlow2?(iSlow8?fTemp46:(3.0 * fTemp46)):(iSlow3?0.0:(iSlow7?(0.33333333333333331 * fTemp46):fTemp46))));
			fRec34[0] = max((fRec34[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp47))))));
			fVbargraph35 = FAUSTFLOAT(fRec34[0]);
			output17[i] = FAUSTFLOAT(fTemp47);
			double fTemp48 = double(input18[i]);
			fRec37[0] = max((fRec37[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp48))))));
			fVbargraph36 = FAUSTFLOAT(fRec37[0]);
			double fTemp49 = (iSlow1?(iSlow4?(iSlow6?fTemp48:0.0):0.0):(iSlow2?(iSlow8?fTemp48:(3.0 * fTemp48)):(iSlow3?0.0:(iSlow7?(0.33333333333333331 * fTemp48):fTemp48))));
			fRec36[0] = max((fRec36[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp49))))));
			fVbargraph37 = FAUSTFLOAT(fRec36[0]);
			output18[i] = FAUSTFLOAT(fTemp49);
			double fTemp50 = double(input19[i]);
			fRec39[0] = max((fRec39[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp50))))));
			fVbargraph38 = FAUSTFLOAT(fRec39[0]);
			double fTemp51 = (iSlow1?(iSlow4?(iSlow6?fTemp50:0.0):0.0):(iSlow2?(iSlow8?fTemp50:(3.0 * fTemp50)):(iSlow3?0.0:(iSlow7?(0.33333333333333331 * fTemp50):fTemp50))));
			fRec38[0] = max((fRec38[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp51))))));
			fVbargraph39 = FAUSTFLOAT(fRec38[0]);
			output19[i] = FAUSTFLOAT(fTemp51);
			double fTemp52 = double(input20[i]);
			fRec41[0] = max((fRec41[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp52))))));
			fVbargraph40 = FAUSTFLOAT(fRec41[0]);
			double fTemp53 = (iSlow1?(iSlow4?(iSlow6?fTemp52:0.0):0.0):(iSlow2?(iSlow8?fTemp52:(3.0 * fTemp52)):(iSlow3?0.0:(iSlow7?(0.33333333333333331 * fTemp52):fTemp52))));
			fRec40[0] = max((fRec40[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp53))))));
			fVbargraph41 = FAUSTFLOAT(fRec40[0]);
			output20[i] = FAUSTFLOAT(fTemp53);
			double fTemp54 = double(input21[i]);
			fRec43[0] = max((fRec43[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp54))))));
			fVbargraph42 = FAUSTFLOAT(fRec43[0]);
			double fTemp55 = (iSlow1?(iSlow4?(iSlow6?fTemp54:0.0):0.0):(iSlow2?(iSlow8?fTemp54:(3.0 * fTemp54)):(iSlow3?0.0:(iSlow7?(0.33333333333333331 * fTemp54):fTemp54))));
			fRec42[0] = max((fRec42[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp55))))));
			fVbargraph43 = FAUSTFLOAT(fRec42[0]);
			output21[i] = FAUSTFLOAT(fTemp55);
			double fTemp56 = double(input22[i]);
			fRec45[0] = max((fRec45[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp56))))));
			fVbargraph44 = FAUSTFLOAT(fRec45[0]);
			double fTemp57 = (iSlow1?(iSlow4?(iSlow6?fTemp56:0.0):0.0):(iSlow2?(iSlow8?fTemp56:(3.0 * fTemp56)):(iSlow3?0.0:(iSlow7?(0.33333333333333331 * fTemp56):fTemp56))));
			fRec44[0] = max((fRec44[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp57))))));
			fVbargraph45 = FAUSTFLOAT(fRec44[0]);
			output22[i] = FAUSTFLOAT(fTemp57);
			double fTemp58 = double(input23[i]);
			fRec47[0] = max((fRec47[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp58))))));
			fVbargraph46 = FAUSTFLOAT(fRec47[0]);
			double fTemp59 = (iSlow1?(iSlow4?(iSlow6?fTemp58:0.0):0.0):(iSlow2?(iSlow8?fTemp58:(3.0 * fTemp58)):(iSlow3?0.0:(iSlow7?(0.33333333333333331 * fTemp58):fTemp58))));
			fRec46[0] = max((fRec46[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp59))))));
			fVbargraph47 = FAUSTFLOAT(fRec46[0]);
			output23[i] = FAUSTFLOAT(fTemp59);
			double fTemp60 = double(input24[i]);
			fRec49[0] = max((fRec49[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp60))))));
			fVbargraph48 = FAUSTFLOAT(fRec49[0]);
			double fTemp61 = (iSlow1?(iSlow4?(iSlow6?fTemp60:0.0):0.0):(iSlow2?(iSlow8?fTemp60:(3.0 * fTemp60)):(iSlow3?0.0:(iSlow7?(0.33333333333333331 * fTemp60):fTemp60))));
			fRec48[0] = max((fRec48[1] - fConst0), min(6.0, (20.0 * log10(max(0.00031622776601683794, fabs(fTemp61))))));
			fVbargraph49 = FAUSTFLOAT(fRec48[0]);
			output24[i] = FAUSTFLOAT(fTemp61);
			fRec1[1] = fRec1[0];
			fRec0[1] = fRec0[0];
			fRec3[1] = fRec3[0];
			fRec4[1] = fRec4[0];
			fRec5[1] = fRec5[0];
			fRec2[1] = fRec2[0];
			fRec6[1] = fRec6[0];
			fRec7[1] = fRec7[0];
			fRec9[1] = fRec9[0];
			fRec10[1] = fRec10[0];
			fRec11[1] = fRec11[0];
			fRec8[1] = fRec8[0];
			fRec13[1] = fRec13[0];
			fRec14[1] = fRec14[0];
			fRec12[1] = fRec12[0];
			fRec15[1] = fRec15[0];
			fRec16[1] = fRec16[0];
			fRec17[1] = fRec17[0];
			fRec19[1] = fRec19[0];
			fRec20[1] = fRec20[0];
			fRec21[1] = fRec21[0];
			fRec18[1] = fRec18[0];
			fRec23[1] = fRec23[0];
			fRec24[1] = fRec24[0];
			fRec22[1] = fRec22[0];
			fRec26[1] = fRec26[0];
			fRec25[1] = fRec25[0];
			fRec28[1] = fRec28[0];
			fRec27[1] = fRec27[0];
			fRec29[1] = fRec29[0];
			fRec30[1] = fRec30[0];
			fRec31[1] = fRec31[0];
			fRec33[1] = fRec33[0];
			fRec32[1] = fRec32[0];
			fRec35[1] = fRec35[0];
			fRec34[1] = fRec34[0];
			fRec37[1] = fRec37[0];
			fRec36[1] = fRec36[0];
			fRec39[1] = fRec39[0];
			fRec38[1] = fRec38[0];
			fRec41[1] = fRec41[0];
			fRec40[1] = fRec40[0];
			fRec43[1] = fRec43[0];
			fRec42[1] = fRec42[0];
			fRec45[1] = fRec45[0];
			fRec44[1] = fRec44[0];
			fRec47[1] = fRec47[0];
			fRec46[1] = fRec46[0];
			fRec49[1] = fRec49[0];
			fRec48[1] = fRec48[0];
			
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
