#pragma once

#include "Def.h"

//--------------------------------------------
/**
\brief Create an enumeration component class
*/
//---------------------------------------------
class CDxEnumerator
{
public:
    ///Constructor
    CDxEnumerator(void);
    ///Destructor
    virtual ~CDxEnumerator(void);

    ///Create an enumerator component
    bool CreateEnumerator(CComPtr<ICreateDevEnum> &pCreateDevEnum, CComPtr<IEnumMoniker> &pEnumMoniker);
};

