/* 
 * File:   PythonWrapper.cpp
 * Author: takeshi
 *
 * Created on April 7, 2016, 5:15 AM
 */
//#define PYTHON_WRAPPER
#ifdef PYTHON_WRAPPER

#define BOOST_PYTHON_STATIC_LIB

#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/class.hpp>
#include <boost/python.hpp>
#include <stdint.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./InputDevices/InputDevices.h"
#include "Application.h"
#include "OutputTest.h"

void startInput()
{
    Application *application = Application::getInstance();
    application->start();    
}

void stopInput()
{
    Application *application = Application::getInstance();
    application->stopRendezvous();    
}

int getInputKeyNum()
{
    return InputDevices::MAX_KEY_NUM;
}

int getInputKey(int index)
{
    InputDevices *inputDevices = InputDevices::getInstance();
    InputDevices::INPUT_STATUS inputStatus;
    inputDevices->getInputStatus(&inputStatus);
    return inputStatus.key[index];
}

InputDevices::INPUT_STATUS getInputStatus()
{
    InputDevices *inputDevices = InputDevices::getInstance();
    InputDevices::INPUT_STATUS inputStatus;
    inputDevices->getInputStatus(&inputStatus);
    return inputStatus;
}

BOOST_PYTHON_MODULE( InputDeviceServer )
{
    boost::python::def( "outputTest", outputTest );
    boost::python::def( "startInput", startInput );
    boost::python::def( "stopInput", stopInput );
    boost::python::def( "getInputKeyNum", getInputKeyNum );
    boost::python::def( "getInputKey", getInputKey );
    boost::python::def( "getInputStatus", getInputStatus );

    boost::python::class_<InputDevices::INPUT_STATUS>("InputDevices::INPUT_STATUS", boost::python::init<>())
        //.add_property("key", &InputDevices::INPUT_STATUS::key)    // do not support return array for now
        .add_property("rotaryDegreeSpeed", &InputDevices::INPUT_STATUS::rotaryDegreeSpeed)
        .add_property("rotaryDegreeSum", &InputDevices::INPUT_STATUS::rotaryDegreeSum)
        .add_property("rotaryTickSpeed", &InputDevices::INPUT_STATUS::rotaryTickSpeed)
        .add_property("rotaryTickSum", &InputDevices::INPUT_STATUS::rotaryTickSum)
        .add_property("accelX", &InputDevices::INPUT_STATUS::accelX)
        .add_property("accelY", &InputDevices::INPUT_STATUS::accelY)
        .add_property("accelZ", &InputDevices::INPUT_STATUS::accelZ)
        .add_property("accelTapNum", &InputDevices::INPUT_STATUS::accelTapNum);
}
#endif