#ifndef __MARS_ROOT_MODULE_H__
#define __MARS_ROOT_MODULE_H__ 

#include "Arduino.h"
#include "../debug/debug.h"
#include "./data_types.h"
#include "../ledDriver/ledDriver.h"
#include "../../payload_settings.h"

#define LEDPIN_R_1 22
#define LEDPIN_G_1 24
#define LEDPIN_B_1 26

#define LEDPIN_R_2 23
#define LEDPIN_G_2 25
#define LEDPIN_B_2 27

class MARSCrashableModule;
class RootModule : public UnCrashable {
    public:
        void criticalFailure(const char* func, const char* file, u16 failLine) override;
        void genericError(const char* func, const char* file, u16 failLine) override;
        void printDebug(String printValues = "NHTB") override;
        bool addModule(MARSCrashableModule &module);
        bool addModule(MARSCrashableModule &module, int id);

        bool init() override;

        fStoredData selectAltitude();
        bool updatePayloadData(bool forceDataUpdate);

    public:
        payloadData data;
        TriColourLED* cLED1;
        TriColourLED* cLED2;
        bool systemArmed = false; //\Is this needed??
    
    protected:
        MARSCrashableModule *modules[MIN_CHILDREN_LENGTH];
};

class MARSCrashableModule : public CrashableModule {
    public:
        MARSCrashableModule(RootModule &uncrashableParent, bool addSelfToParent);
        virtual bool updatePayloadData(bool forceDataUpdate);

    protected:
        RootModule *marsRoot;
};

payloadData generateRandomPayload(payloadData* pData = nullptr);

#endif