#ifndef __MARS_ROOT_MODULE_H__
#define __MARS_ROOT_MODULE_H__ 

#include "Arduino.h"
#include "../debug/debug.h"
#include "./data_types.h"

class RootModule : public UnCrashable {
    public:
        void criticalFailure(const char* func, const char* file, u16 failLine) override;
        void genericError(const char* func, const char* file, u16 failLine) override;
        void printDebug(String printValues = "NHTB") override;

        void updatePayloadData(bool forceDataUpdate);
    public:
        payloadData data;
};

class MARSCrashableModule : public CrashableModule {
    public:
        MARSCrashableModule(RootModule &uncrashableParent, bool addSelfToParent) 
            : CrashableModule(uncrashableParent, addSelfToParent) {};
            
        virtual bool updatePayloadData(bool forceDataUpdate);

    protected:
        RootModule *parent;
};

#endif