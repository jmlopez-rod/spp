# Excentury

Excentury is a collection of header files designed for
interoperability. It was born out of the necessity to exchange data
between programs written in c++ and MATLAB. Due to its data exchange
protocol it is now possible make c++ code interact with scripting
languages such as Python, MATLAB and Mathematica.

## Installation

You have two options when installing excentury.

### User Global Installation

By user global installation it is meant that you will be able to
access excentury as soon as you open your terminal. To perform this
installation simply excentury the `install` script that comes with
excentury:

    python install

The script will print messages information you of what it is doing.

### Temporary/Project Installation

`source` the `bashrc` file that comes with excentury.

## Possible issues

There are a few possible issues when trying to create programs that
interact with MATLAB and Mathematica.

### MATLAB in OS X

First make sure that you can access the mex program from MATLAB. Then
do

    mex -setup

this will create a file in the user directory which you can modify.
If you are using a mac then look for `CXX` and set it to g++ and set
`CC` to gcc. The last thing you will need is to set

    SDKROOT='/Applications/Xcode.app/Contents/Developer/Platforms'
    SDKROOT='/MacOSX.platform/Developer/SDKs/MacOSX10.7.sdk/'$SDKROOT
    MW_SDKROOT="$SDKROOT"

this is so that g++ can find the correct header files. If you are
using mex alone don't forget to use the -v option so that mex may
print messages informing you of the steps it is taking.

    mex -v file.cpp
