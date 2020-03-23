/*
*********************************************************************************************************
*                              		(c) Copyright 2018-2020, Hexin
*                                           All Rights Reserved
* File    : _crc16module.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V0.1.6
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2017-08-09 09:52AM
*                       2017-08-17 [Heyn] Optimized Code.
*                                         Wheel 0.0.4 New CRC16-SICK / CRC16-DNP
*                       2017-08-21 [Heyn] Optimization code for the C99 standard.
*                                         for ( unsigned int i=0; i<256; i++ ) -> for ( i=0; i<256; i++ )
*                       2017-08-22 [Heyn] Bugfixes Parsing arguments
*                                           Change PyArg_ParseTuple(* , "y#|I")
*                                           To     PyArg_ParseTuple(* , "y#|H")
*                                           "H" : Convert a Python integer to a C unsigned short int,
*                                               without overflow checking.
*                       2017-09-19 [Heyn] New CRC16-X25 CRC16-USB CRC16-MAXIM16 CRC16-DECT. (V0.1.3)
*                                         Optimized Code.
*                       2020-03-13 [Heyn] New add hacker16 code.
*                       2020-03-20 [Heyn] New add UDP and TCP checksum.
*                       2020-03-23 [Blazin64] Adds support for the CRC method used by MCRF4XX RFID hardware.
*
*********************************************************************************************************
*/

#include <Python.h>
#include "_crc16tables.h"

/*
*********************************************************************************************************
                                    POLY=0xA001 [Modbus USB]
*********************************************************************************************************
*/

static PyObject * _crc16_modbus( PyObject *self, PyObject *args )
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00000000L;
    unsigned short crc16  = 0xFFFF;
    unsigned short result = 0x0000;

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTuple( args, "y#|H", &data, &data_len, &crc16 ) )
        return NULL;
#else
    if ( !PyArg_ParseTuple(args, "s#|H", &data, &data_len, &crc16 ) )
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hexin_calc_crc16_a001( data, data_len, crc16 );

    return Py_BuildValue( "H", result );
}

/*
*********************************************************************************************************
*                                   POLY=0x8005 [USB]
* Poly:    0x8005
* Init:    0xFFFF
* Refin:   True
* Refout:  True
* Xorout:  0xFFFF
* Alias:   CCITT_TRUE
*
* 0xA001 = reverse 0x8005
*********************************************************************************************************
*/
static PyObject * _crc16_usb( PyObject *self, PyObject *args )
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00000000L;
    unsigned short crc16  = 0xFFFF;
    unsigned short result = 0x0000;

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTuple( args, "y#|H", &data, &data_len, &crc16 ) )
        return NULL;
#else
    if ( !PyArg_ParseTuple( args, "s#|H", &data, &data_len, &crc16 ) )
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hexin_calc_crc16_a001( data, data_len, crc16 );
    result = result ^ 0xFFFF;
    return Py_BuildValue( "H", result );
}

/*
*********************************************************************************************************
*                                   POLY=0x8005 [IBM]
* Poly:    0x8005
* Init:    0x0000
* Refin:   True
* Refout:  True
* Xorout:  0x0000
*
* 0xA001 = reverse 0x8005
*********************************************************************************************************
*/

static PyObject * _crc16_ibm( PyObject *self, PyObject *args )
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00000000L;
    unsigned short poly   = CRC16_POLYNOMIAL_A001;
    unsigned short crc16  = 0x0000;
    unsigned short result = 0x0000;

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTuple( args, "y#|HH", &data, &data_len, &poly, &crc16 ) )
        return NULL;
#else
    if ( !PyArg_ParseTuple( args, "s#|HH", &data, &data_len, &poly, &crc16 ) )
        return NULL;
#endif /* PY_MAJOR_VERSION */

    if ( poly == CRC16_POLYNOMIAL_8005 ) {
        result = hexin_calc_crc16_8005( data, data_len, crc16 );
    } else {
        result = hexin_calc_crc16_a001( data, data_len, crc16 );
    }

    return Py_BuildValue( "H", result );
}

/*
*********************************************************************************************************
*                                  POLY=0x1021 [CCITT XModem]
* Poly:    0x1021
* Init:    0x0000
* Refin:   False
* Refout:  False
* Xorout:  0x0000
*********************************************************************************************************
*/

static PyObject * _crc16_xmodem( PyObject *self, PyObject *args )
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00000000L;
    unsigned short crc16  = 0x0000;
    unsigned short result = 0x0000;

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTuple( args, "y#|H", &data, &data_len, &crc16 ) )
        return NULL;
#else
    if ( !PyArg_ParseTuple( args, "s#|H", &data, &data_len, &crc16 ) )
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hexin_calc_crc16_1021( data, data_len, crc16 );

    return Py_BuildValue( "H", result );
}

/*
*********************************************************************************************************
*                                  POLY=0x1021 [CCITT-FALSE]
* Poly:    0x1021
* Init:    0xFFFF
* Refin:   False
* Refout:  False
* Xorout:  0x0000
*********************************************************************************************************
*/
static PyObject * _crc16_ccitt( PyObject *self, PyObject *args )
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00000000L;
    unsigned short crc16  = 0xFFFF;
    unsigned short result = 0x0000;

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTuple( args, "y#|H", &data, &data_len, &crc16 ) )
        return NULL;
#else
    if ( !PyArg_ParseTuple( args, "s#|H", &data, &data_len, &crc16 ) )
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hexin_calc_crc16_1021( data, data_len, crc16 );

    return Py_BuildValue( "H", result );
}

/*
*********************************************************************************************************
*                                   POLY=0x8408 [Kermit]
* Poly:    0x1021
* Init:    0x0000
* Refin:   True
* Refout:  True
* Xorout:  0x0000
* Alias:   CCITT_TRUE
*
* 0x8408 = reverse 0x1021
*********************************************************************************************************
*/

static PyObject * _crc16_kermit( PyObject *self, PyObject *args )
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00000000L;
    unsigned short crc16  = 0x0000;
    unsigned short result = 0x0000;

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTuple(args, "y#|H", &data, &data_len, &crc16 ) )
        return NULL;
#else
    if ( !PyArg_ParseTuple(args, "s#|H", &data, &data_len, &crc16 ) )
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hexin_calc_crc16_8408( data, data_len, crc16 );

    return Py_BuildValue( "H", result );
}

/*
*********************************************************************************************************
*                                   POLY=0x1021 [MCRF4XX]
* Poly:    0x1021
* Init:    0xFFFF
* Refin:   True
* Refout:  True
* Xorout:  0x0000
*
* 0x8408 = reverse 0x1021
*********************************************************************************************************
*/

static PyObject * _crc16_mcrf4xx( PyObject *self, PyObject *args )
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00000000L;
    unsigned short crc16  = 0xFFFF;
    unsigned short result = 0x0000;

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTuple( args, "y#|H", &data, &data_len, &crc16 ) )
        return NULL;
#else
    if ( !PyArg_ParseTuple( args, "s#|H", &data, &data_len, &crc16 ) )
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hexin_calc_crc16_8408( data, data_len, crc16 );

    return Py_BuildValue( "H", result );
}

/*
*********************************************************************************************************
*                                   POLY=0x1021 [X25]
* Poly:    0x1021
* Init:    0xFFFF
* Refin:   True
* Refout:  True
* Xorout:  0xFFFF
*
* 0x8408 = reverse 0x1021
*********************************************************************************************************
*/
static PyObject * _crc16_x25( PyObject *self, PyObject *args )
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00000000L;
    unsigned short crc16  = 0xFFFF;
    unsigned short result = 0x0000;

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTuple( args, "y#|H", &data, &data_len, &crc16 ) )
        return NULL;
#else
    if ( !PyArg_ParseTuple( args, "s#|H", &data, &data_len, &crc16 ) )
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hexin_calc_crc16_8408( data, data_len, crc16 );
    result = result ^ 0xFFFF;
    return Py_BuildValue( "H", result );
}

/*
*********************************************************************************************************
                                    POLY=0x8005 [SICK]
*********************************************************************************************************
*/

static PyObject * _crc16_sick( PyObject *self, PyObject *args )
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00000000L;
    unsigned short crc16  = 0x0000;
    unsigned short result = 0x0000;

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTuple( args, "y#|H", &data, &data_len, &crc16 ) )
        return NULL;
#else
    if ( !PyArg_ParseTuple( args, "s#|H", &data, &data_len, &crc16 ) )
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hexin_calc_crc16_sick( data, data_len, crc16 );

    return Py_BuildValue( "H", result );
}

/*
*********************************************************************************************************
                                    POLY=0xA6BC [DNP]
* Poly:    0x3D65
* Init:    0x0000
* Refin:   True
* Refout:  True
* Xorout:  0xFFFF
*
* 0xA6BC = reverse 0x3D65
*********************************************************************************************************
*/

static PyObject * _crc16_dnp( PyObject *self, PyObject *args )
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00000000L;
    unsigned short crc16  = 0x0000;
    unsigned short result = 0x0000;

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTuple( args, "y#|H", &data, &data_len, &crc16 ) )
        return NULL;
#else
    if ( !PyArg_ParseTuple( args, "s#|H", &data, &data_len, &crc16 ) )
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hexin_calc_crc16_a6bc( data, data_len, crc16 );

    return Py_BuildValue( "H", result );
}


/*
*********************************************************************************************************
*                                   POLY=0x8005 [MAXIM16]
* Poly:    0x8005
* Init:    0x0000
* Refin:   True
* Refout:  True
* Xorout:  0xFFFF
*
* 0xA001 = reverse 0x8005
*********************************************************************************************************
*/
static PyObject * _crc16_maxim( PyObject *self, PyObject *args )
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00000000L;
    unsigned short crc16  = 0x0000;
    unsigned short result = 0x0000;

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTuple( args, "y#|H", &data, &data_len, &crc16 ) )
        return NULL;
#else
    if ( !PyArg_ParseTuple( args, "s#|H", &data, &data_len, &crc16 ) )
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hexin_calc_crc16_a001( data, data_len, crc16 );
    result = result ^ 0xFFFF;

    return Py_BuildValue( "H", result );
}

/*
*********************************************************************************************************
*                                   POLY=0x0589 [DECT]
* Poly:    0x0589
* Init:    0x0000
* Refin:   True
* Refout:  True
* Xorout:  0x0000
*
* 0x91A0 = reverse 0x0589
*********************************************************************************************************
*/

static PyObject * _crc16_dect( PyObject *self, PyObject *args )
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00000000L;
    unsigned short crc16  = 0x0000;
    unsigned short result = 0x0000;

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTuple( args, "y#|H", &data, &data_len, &crc16 ) )
        return NULL;
#else
    if ( !PyArg_ParseTuple( args, "s#|H", &data, &data_len, &crc16 ) )
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hexin_calc_crc16_91a0( data, data_len, crc16 );

    return Py_BuildValue( "H", result );
}

/*
*********************************************************************************************************
                                    Print CRC16 table.
*********************************************************************************************************
*/
static PyObject * _crc16_table( PyObject *self, PyObject *args )
{
    unsigned int i = 0x00000000L;
    unsigned short poly = CRC16_POLYNOMIAL_A001;
    unsigned short table[MAX_TABLE_ARRAY] = { 0x0000 };
    PyObject* plist = PyList_New( MAX_TABLE_ARRAY );

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTuple( args, "H", &poly ) )
        return NULL;
#else
    if ( !PyArg_ParseTuple( args, "H", &poly ) )
        return NULL;
#endif /* PY_MAJOR_VERSION */

    if ( HEXIN_POLYNOMIAL_IS_HIGH( poly ) ) {
        hexin_crc16_init_table_poly_is_high( poly, table );
    } else {
        hexin_crc16_init_table_poly_is_low ( poly, table );
    }

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        PyList_SetItem( plist, i, Py_BuildValue( "H", table[i] ) );
    }

    return plist;
}

/*
*********************************************************************************************************
*                                   For hacker
*********************************************************************************************************
*/
static PyObject * _crc16_hacker( PyObject *self, PyObject *args, PyObject* kws )
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00000000L;
    unsigned short init   = 0xFFFF;
    unsigned short xorout = 0x0000;
    unsigned int   ref    = 0x00000000L;
    unsigned short result = 0x0000;
    unsigned short polynomial = CRC16_POLYNOMIAL_1021;
    static char* kwlist[]={ "data", "poly", "init", "xorout", "ref", NULL };

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTupleAndKeywords( args, kws, "y#|HHHp", kwlist, &data, &data_len, &polynomial, &init, &xorout, &ref ) )
        return NULL;
#else
    if ( !PyArg_ParseTupleAndKeywords( args, kws, "s#|HHHp", kwlist, &data, &data_len, &polynomial, &init, &xorout, &ref ) )
        return NULL;
#endif /* PY_MAJOR_VERSION */

    if ( HEXIN_REFIN_OR_REFOUT_IS_TRUE( ref ) ) {
        polynomial = hexin_reverse16( polynomial );
    }

    result = hexin_calc_crc16_hacker( data, data_len, init, polynomial );
    result = result ^ xorout;
    return Py_BuildValue( "H", result );
}

/*
*********************************************************************************************************
                                    For network(UDP/TCP) checksum
*********************************************************************************************************
*/
static PyObject * _crc16_network( PyObject *self, PyObject *args )
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00000000L;
    unsigned short result = 0x0000;

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTuple( args, "y#|H", &data, &data_len ) )
        return NULL;
#else
    if ( !PyArg_ParseTuple(args, "s#", &data, &data_len ) )
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hexin_calc_crc16_network( data, data_len );

    return Py_BuildValue( "H", result );
}

/*
*********************************************************************************************************
                                    For fletcher16 checksum
*********************************************************************************************************
*/

static PyObject * _crc16_fletcher( PyObject *self, PyObject *args )
{
    const unsigned char *data = NULL;
    unsigned int data_len = 0x00000000L;
    unsigned short result = 0x0000;

#if PY_MAJOR_VERSION >= 3
    if ( !PyArg_ParseTuple( args, "y#|H", &data, &data_len ) )
        return NULL;
#else
    if ( !PyArg_ParseTuple(args, "s#", &data, &data_len ) )
        return NULL;
#endif /* PY_MAJOR_VERSION */

    result = hexin_calc_crc16_fletcher( data, data_len );

    return Py_BuildValue( "H", result );
}

/* method table */
static PyMethodDef _crc16Methods[] = {
    { "modbus",      _crc16_modbus, METH_VARARGS, "Calculate Modbus of CRC16              [Poly=0xA001, Init=0xFFFF Xorout=0x0000 Refin=False Refout=False]" },
    { "usb16",       _crc16_usb,    METH_VARARGS, "Calculate USB    of CRC16              [Poly=0xA001, Init=0xFFFF Xorout=0xFFFF Refin=False Refout=False]" },
    { "ibm",         _crc16_ibm,    METH_VARARGS, "Calculate IBM (Alias:ARC/LHA) of CRC16 [Poly=0x8005, Init=0x0000 Xorout=0x0000 Refin=True Refout=True]" },
    { "xmodem",      _crc16_xmodem, METH_VARARGS, "Calculate XMODEM of CRC16              [Poly=0x1021, Init=0x0000 Xorout=0x0000 Refin=False Refout=False]" },
    { "ccitt",       _crc16_kermit, METH_VARARGS, "Calculate CCITT-TRUE of CRC16          [Poly=0x1021, Init=0x0000 Xorout=0x0000 Refin=True Refout=True]" },
    { "ccitt_false", _crc16_ccitt,  METH_VARARGS, "Calculate CCITT-FALSE of CRC16         [Poly=0x1021, Init=0xFFFF or 0x1D0F]" },
    { "kermit",      _crc16_kermit, METH_VARARGS, "Calculate Kermit (CCITT-TRUE) of CRC16 [Poly=0x8408, Init=0x0000]" },
    { "mcrf4xx",     _crc16_mcrf4xx,METH_VARARGS, "Calculate MCRF4XX of CRC16             [Poly=0x8408, Init=0xFFFF]" },
    { "sick",        _crc16_sick,   METH_VARARGS, "Calculate Sick of CRC16                [Poly=0x8005, Init=0x0000]" },
    { "dnp",         _crc16_dnp,    METH_VARARGS, "Calculate DNP (Ues:M-Bus, ICE870)  of CRC16    [Poly=0x3D65, Init=0x0000 Xorout=0xFFFF Refin=True Refout=True]" },
    { "x25",         _crc16_x25,    METH_VARARGS, "Calculate X25  of CRC16                [Poly=0x1021, Init=0xFFFF Xorout=0xFFFF Refin=True Refout=True]" },
    { "maxim16",     _crc16_maxim,  METH_VARARGS, "Calculate MAXIM of CRC16               [Poly=0x8005, Init=0x0000 Xorout=0xFFFF Refin=True Refout=True]" },
    { "dect",        _crc16_dect,   METH_VARARGS, "Calculate DECT of CRC16                [Poly=0x0589, Init=0x0000 Xorout=0x0000 Refin=True Refout=True]" },
    { "table16",     _crc16_table,  METH_VARARGS, "Print CRC16 table to list. libscrc.table16( polynomial )" },
    { "hacker16",    _crc16_hacker, METH_KEYWORDS|METH_VARARGS, "User calculation CRC16\n"
                                                                "@data   : bytes\n"
                                                                "@poly   : default=0x1021\n"
                                                                "@init   : default=0xFFFF\n"
                                                                "@xorout : default=0x0000\n"
                                                                "@ref    : default=False" },
    { "udp",         _crc16_network,    METH_VARARGS, "Calculate UDP checksum." },
    { "tcp",         _crc16_network,    METH_VARARGS, "Calculate TCP checksum." },
    { "fletcher16",  _crc16_fletcher,   METH_VARARGS, "Calculate fletcher16" },
    { NULL, NULL, 0, NULL }        /* Sentinel */
};


/* module documentation */
PyDoc_STRVAR( _crc16_doc,
"Calculation of CRC16 \n"
"libscrc.modbus     -> Calculate Modbus of CRC16              [Poly=0xA001, Init=0xFFFF Xorout=0x0000 Refin=True Refout=True]\n"
"libscrc.usb16      -> Calculate USB    of CRC16              [Poly=0xA001, Init=0xFFFF Xorout=0xFFFF Refin=True Refout=True]\n"
"libscrc.ibm        -> Calculate IBM (Alias:ARC/LHA) of CRC16 [Poly=0x8005, Init=0x0000 Xorout=0x0000 Refin=True Refout=True]\n"
"libscrc.xmodem     -> Calculate XMODEM of CRC16              [Poly=0x1021, Init=0x0000 Xorout=0x0000 Refin=False Refout=False]\n"
"libscrc.ccitt      -> Calculate CCITT-TRUE of CRC16          [Poly=0x1021, Init=0x0000 Xorout=0x0000 Refin=True Refout=True]\n"
"libscrc.ccitt_false-> Calculate CCITT-FALSE of CRC16         [Poly=0x1021, Init=0xFFFF or 0x1D0F]\n"
"libscrc.kermit     -> Calculate Kermit (CCITT-TRUE) of CRC16 [Poly=0x8408, Init=0x0000]\n"
"libscrc.mcrf4xx    -> Calculate MCRF4XX of CRC16             [Poly=0x8408, Init=0xFFFF]\n"
"libscrc.sick       -> Calculate Sick of CRC16                [Poly=0x8005, Init=0x0000]\n"
"libscrc.dnp        -> Calculate DNP (Ues:M-Bus, ICE870)  of CRC16    [Poly=0x3D65, Init=0x0000 Xorout=0xFFFF Refin=True Refout=True]\n"
"libscrc.x25        -> Calculate X25  of CRC16                [Poly=0x1021, Init=0xFFFF Xorout=0xFFFF Refin=True Refout=True]\n"
"libscrc.maxim16    -> Calculate MAXIM of CRC16               [Poly=0x8005, Init=0x0000 Xorout=0xFFFF Refin=True Refout=True]\n"
"libscrc.dect       -> Calculate DECT of CRC16                [Poly=0x0589, Init=0x0000 Xorout=0x0000 Refin=True Refout=True]\n"
"libscrc.hacker16   -> Free calculation CRC16 (not support python2 series)\n"
"libscrc.fletcher16 -> Calculate fletcher16\n"
"\n" );


#if PY_MAJOR_VERSION >= 3

/* module definition structure */
static struct PyModuleDef _crc16module = {
   PyModuleDef_HEAD_INIT,
   "_crc16",                    /* name of module */
   _crc16_doc,                  /* module documentation, may be NULL */
   -1,                          /* size of per-interpreter state of the module */
   _crc16Methods
};

/* initialization function for Python 3 */
PyMODINIT_FUNC
PyInit__crc16( void )
{
    PyObject *m;

    m = PyModule_Create( &_crc16module );
    if (m == NULL) {
        return NULL;
    }

    PyModule_AddStringConstant( m, "__version__", "0.1.6" );
    PyModule_AddStringConstant( m, "__author__",  "Heyn"  );

    return m;
}

#else

/* initialization function for Python 2 */
PyMODINIT_FUNC
init_crc16( void )
{
    (void) Py_InitModule3( "_crc16", _crc16Methods, _crc16_doc );
}

#endif /* PY_MAJOR_VERSION */
