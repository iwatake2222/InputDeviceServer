#!/bin/env python
# coding: utf-8

from __future__ import print_function
__author__ = "takeshi"
__date__ = "$10-Apr-2016 12:43:45 AM$"


import os, sys
import json
from flask import Flask, render_template, request
import InputDeviceServer

app = Flask(__name__, static_url_path='/static')
# need to be false to avoid duplicate running
app.debug = False

def setGPIO(gpio, out):
    InputDeviceServer.outputTest(gpio, out)
    print ("Set " + str(gpio) + " as " + str(out), file=sys.stderr)

def getInputDeviceStatus():
    InputDeviceServer.getInputKeyNum()
    status = {
        "keys":[10, 20],
        "rotaryDegreeSpeed":123,
        "rotaryDegreeSum":-33,
        "rotaryTickSpeed":-33,
        "rotaryTickSum":-33,
        "accelX":-33,
        "accelY":-33,
        "accelZ":-33,
        "accelTapNum":3,
    };
    status["keys"][0] = InputDeviceServer.getInputKey(0)
    status["keys"][1] = InputDeviceServer.getInputKey(1)
    statusFromC = InputDeviceServer.getInputStatus()
    status["rotaryDegreeSpeed"] = statusFromC.rotaryDegreeSpeed
    status["rotaryDegreeSum"] = statusFromC.rotaryDegreeSum
    status["rotaryTickSpeed"] = statusFromC.rotaryTickSpeed
    status["rotaryTickSum"] = statusFromC.rotaryTickSum
    status["accelX"] = statusFromC.accelX
    status["accelY"] = statusFromC.accelY
    status["accelZ"] = statusFromC.accelZ
    status["accelTapNum"] = statusFromC.accelTapNum
    return status;

def getInputDeviceStatusMock():
    status = {
        "keys":[10, 20],
        "rotaryDegreeSpeed":123,
        "rotaryDegreeSum":-33,
        "rotaryTickSpeed":-33,
        "rotaryTickSum":-33,
        "accelX":-33,
        "accelY":-33,
        "accelZ":-33,
        "accelTapNum":3,
    };
    return status;

@app.route('/')
def index():
    title = "Input Device Server"
    return render_template('index.html', title=title)

@app.route('/getInputStatus')
def getInputStatus():
    statusJson = getInputDeviceStatus();
    return json.dumps(statusJson)


@app.route('/setLed')
def setLed():
    gpio = request.args.get('gpio', default=-1, type=int)
    out = request.args.get('out', default=-1, type=int)
    if gpio != -1 and out != -1:
        setGPIO(gpio, out)
        return "OK"
    else:
        return "ERR"
        

if __name__ == "__main__":
    print("Server Start")
    InputDeviceServer.startInput()
    app.run(host='0.0.0.0', port=2000)
    # never reach
    #InputDeviceServer.stopInput()

