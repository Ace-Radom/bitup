#ifndef _PYAPI_ENCAPSULATION_H_
#define _PYAPI_ENCAPSULATION_H_

#include<Python.h>
#include<string>

#if PY_MAJOR_VERSION < 3

    #define PyImport_Module( module ) PyObject* Py_##module = PyImport_Import( PyString_FromString( #module ) )

#else

    #define PyImport_Module( module ) PyObject* Py_##module = PyImport_Import( PyUnicode_FromString( #module ) )

#endif

#define PyModule_AppendPath( path )                             \
{                                                               \
    PyRun_SimpleString( "import sys" );                         \
    PyRun_SimpleString( "sys.path.append( '" path "')" );       \
}

#define PyImport_Function( module , function ) PyObject* PyFunc_##function = PyObject_GetAttrString( module , #function )
#define PyFunction_Callable( function )                         \
        ( function && PyCallable_Check( function ) ) ? true     \
                                                     : false

typedef int errno_t;

char* PyCall_CalcFileMD5( const char* );

#endif