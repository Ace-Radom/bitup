#include"PyAPI_Encapsulation.h"

/**
 * @brief call file md5 calculate function (need hashfile.py)
 */
char* PyCall_CalcFileMD5( const char* __file ){

    Py_Initialize();
    if ( !Py_IsInitialized() )
    {
        PyErr_Print();
        return NULL;
    }

    PyModule_AppendPath( "../py" );
    PyImport_Module( hashfile );
    if ( Py_hashfile == nullptr )
    {
        PyErr_Print();
        return NULL;
    }

    PyImport_Function( Py_hashfile , hashfile_md5 );
    if ( !PyFunction_Callable( PyFunc_hashfile_md5 ) )
    {
        PyErr_Print();
        return NULL;
    }

    PyObject* args = Py_BuildValue( "(s)" , __file );
    PyObject* return_object = PyObject_CallObject( PyFunc_hashfile_md5 , args );
    char* return_value;
    PyArg_Parse( return_object , "s" , &return_value );
    Py_DecRef( args );
    Py_DecRef( return_object );
    
    return return_value;
}