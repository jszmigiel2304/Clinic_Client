#ifndef CV_PROCESSDATA_H
#define CV_PROCESSDATA_H

#include "c_mystructures.h"

class cv_ProcessData
{

public:
    cv_ProcessData();

    virtual void processData(threadData data);
};

#endif // CV_PROCESSDATA_H
