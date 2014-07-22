# SPP local installation
#
# This bash file is meant to be used for local installation.
#

# The path to this local installation
export SPPROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Access to spp
export PATH=$SPPROOT/bin:$PATH

# Python and C/C++
export PYTHONPATH=$SPPROOT/python:$PYTHONPATH
export C_INCLUDE_PATH=$SPPROOT/include:$C_INCLUDE_PATH
export CPLUS_INCLUDE_PATH=$SPPROOT/include:$CPLUS_INCLUDE_PATH

# SPP_CONFIG:
# If you have a configuration file for spp that you wish to
# use then define this variable
#export SPP_CONFIG_PATH=/path/to/spp.config
