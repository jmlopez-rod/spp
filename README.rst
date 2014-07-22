Simulating Population Processes
===============================

This library is still a work in progress. At the moment, this
repository includes a version of excentury. Please note that this
will be removed in later versions.

Quick Example
-------------

Download this this repository as a zip file. Once you extract it
open a terminal and naviate to the ``spp`` directory.

Once there do ``source bashrc``. This will enable you to call the
version of excentury that comes included with spp.

Before you proceed to the next step please navigate to ``main`` and
create the directories: ``lib`` and ``bin``.

Now, if you are in the main directory you can type:

.. code-block:: sh

    xcpp lotka_volterra_ssa.xcpp to python

if you want to work with python

or

.. code-block:: sh

    xcpp lotka_volterra_ssa.xcpp to matlab

if you wish to work with MATLAB. Please note that depending on the
version of MATLAB that you have, the last step may not work.

Once you have compiled to python you can do

.. code-block:: sh

    >>> import lotka_volterra_ssa as lib
    >>> help(lib.time_series)

This will let you know how to create a time series for the
lotka_volterra model.

Once you are familar with it you can call the function

.. code-block:: sh

    >>> import lotka_volterra_ssa as lib
    >>> help(lib.time_series)
    >>> [t, x] = lib.time_series(1000, 1, 10)
    >>> from pylab import *
    >>> plot(t, x)
    >>> show()

If you are working with MATLAB you can do

.. code-block:: sh

    help lotka_volterra_ssa.time_series
    [t, x] = lotka_volterra_ssa.time_series(1000, 1, 10);
    plot(t, x)
