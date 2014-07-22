# File generated on Thu Jan 30, 2014 12:27:53 AM by xcpp.
"""Negative FeedBack DSSA

Set of functions to perform simulations on the Negative Feedback
model using a delayed Stochastic Simulation Algorithm (DSSA)."""
from excentury import TextParser, TextDumper, XCStruct
from ctypes import c_char, c_size_t, c_char_p
import ctypes

LIB = ctypes.cdll.LoadLibrary("nfb_dssa_pylib.so")

DEFS = {
}
def xc_struct(name):
    """Return an XCStruct object using the entries of the dictionary
    DEFS in this module."""
    return XCStruct(name, DEFS[name])


LIB.rand_path_py.restype = None
LIB.rand_path_py_clear.restype = None
def rand_path(npoints, ntrials, delay, unpack=True):
    """Generate realizations from the Negative Feedback model by
    collecting each point that the DSSA generates. This function
    returns `t` and `x`.

    parameters:

      `npoints`: number of points per realization
      `ntrials`: number of realizations
      `delay`: time by which an event is delayed
    """
    tdump = TextDumper()
    tdump.dump(npoints, "npoints", ['I', '4'])
    tdump.dump(ntrials, "ntrials", ['I', '4'])
    tdump.dump(delay, "delay", ['R', '8'])
    in_str = tdump.close()
    len_in = len(in_str)
    out_str = ctypes.POINTER(c_char)()
    len_out = c_size_t(0)
    LIB.rand_path_py(c_size_t(len_in),
                     c_char_p(in_str),
                     ctypes.byref(len_out),
                     ctypes.byref(out_str))
    if out_str[:1] == 'E':
        xc_error_msg = out_str[1:len_out.value]
        raise RuntimeError(xc_error_msg)
    val = TextParser(out_str[:len_out.value]).parse()
    LIB.rand_path_py_clear()
    if unpack:
        if val:
            return val.values()[0] if len(val) == 1 else val.values()
        return None
    else:
        return val


LIB.time_series_py.restype = None
LIB.time_series_py_clear.restype = None
def time_series(npoints, ntrials, freq, delay, unpack=True):
    """Generate realizations from the Negative Feedback model by
    collecting points that the DSSA generates at a given frequency.
    This function returns `t` and `x`.

    parameters:

      `npoints`: number of points per realization
      `ntrials`: number of realizations
      `freq`: collection frequency
      `delay`: time by which an event is delayed
    """
    tdump = TextDumper()
    tdump.dump(npoints, "npoints", ['I', '4'])
    tdump.dump(ntrials, "ntrials", ['I', '4'])
    tdump.dump(freq, "freq", ['R', '8'])
    tdump.dump(delay, "delay", ['R', '8'])
    in_str = tdump.close()
    len_in = len(in_str)
    out_str = ctypes.POINTER(c_char)()
    len_out = c_size_t(0)
    LIB.time_series_py(c_size_t(len_in),
                       c_char_p(in_str),
                       ctypes.byref(len_out),
                       ctypes.byref(out_str))
    if out_str[:1] == 'E':
        xc_error_msg = out_str[1:len_out.value]
        raise RuntimeError(xc_error_msg)
    val = TextParser(out_str[:len_out.value]).parse()
    LIB.time_series_py_clear()
    if unpack:
        if val:
            return val.values()[0] if len(val) == 1 else val.values()
        return None
    else:
        return val


