#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
/************************************************************************
*** N.Kozak // Lviv'2019
*** path for CS-labs
*************************************************************************/

//#include <winnt.h>
//#define _MSC_VER 600
//typedef void* RPC_NS_HANDLE;
//#define RPCRTAPI __declspec(dllimport)
//typedef void * I_RPC_HANDLE;
//typedef long RPC_STATUS;
//#include <basetyps.h>
//#include <rpcdce.h>

#ifdef WIN32
#include "sc_cmnhdr.h"
#include <Windows.h>
//#include <winnt.h>
//PVOID __stdcall GetCurrentFiber( void );
#else
#include "qt/qt.h"
#endif

#include <iostream>
using std::ostream;
using std::istream;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::hex;

//using std::ios;
using namespace std;

//#define DISABLE_ERROR_REGION
#define SC_ISDB_TRACE_H

#define ConvertThreadToFiber(ARG) (ConvertThreadToFiber(ARG), (PVOID)-1)
