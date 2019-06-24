#include "Server.h"
#include <thread>
#include <future>
#include <mutex>
#include <iomanip>
#include <chrono>
#include <sstream>
#include <Python.h>
#include <SFML/Graphics.hpp>
#include "map.h"
#include <random>
using namespace std;


int main()
{
	PyObject* pName, * pModule;
	PyObject* pArgs, * pValue;
	int i;
	Server server;
	Py_Initialize();
	pName = PyUnicode_FromString("main");
	pModule = PyImport_Import(pName);
	Py_DECREF(pName);
	/* Error checking of pName left out */
	if (pModule != NULL) {

		auto fut = std::async(std::launch::async, &Server::Start, &server, pModule);

		fut.wait();
		Py_DECREF(pModule);
	}
	else {
		PyErr_Print();
	}
	Py_Finalize();
	return 0;
}
