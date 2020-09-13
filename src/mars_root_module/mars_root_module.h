#ifndef __MARS_ROOT_MODULE_H__
#define __MARS_ROOT_MODULE_H__ 

#include "Arduino.h"
#include "../debug/debug.h"
#include "./data_types.h"

class MARSCrashableModule;
class RootModule : public UnCrashable {
    public:
        void criticalFailure(const char* func, const char* file, u16 failLine) override;
        void genericError(const char* func, const char* file, u16 failLine) override;
        void printDebug(String printValues = "NHTB") override;
        bool addModule(MARSCrashableModule &module);
        bool addModule(MARSCrashableModule &module, int id);

        bool updatePayloadData(bool forceDataUpdate);

    public:
        payloadData data;
    
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

#endif