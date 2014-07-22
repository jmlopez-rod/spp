"""SPP

To run this script from the command line do the following:

    python -m spp <command>

Use the option --help for more information.

"""

import argparse
import textwrap
from os import listdir
from os.path import abspath, isdir, split, join
from spp.__version__ import VERSION
from spp import import_mod
try:
    import argcomplete
except ImportError:
    pass


def parse_options(mod):
    """Interpret the command line inputs and options. """
    desc = """
Utility script to aid in the creation of files for simulation of
population processes.

Each of the commands has a default configuration. These values can be
modified with configuration files. There are several ways in which
configuration files can be provided to spp:

 * spp.config: spp will read this file if it is located in the
   current working directory.

 * --cfg: this location of the configuration file can be specified
   via this option. Please note that all relative paths specified in
   the configuration files will relative to the location of the
   configuration file.

 * If there is no configuration file or the --cfg option was not used
   then spp will check the enviromental variable SPP_CONFIG_PATH to
   look for a configuration file.

Some subcommands will not be able to function properly if the c++ spp
header files are not found. spp will look for the header files in the
enviromental variable CPLUS_INCLUDE_PATH. Make sure that this
variable has a path where these header files are located.

"""
    ver = "spp %s" % VERSION
    epi = """
See the help option on each of the commands for more info.

NOTE: If you would like to use the TAB key for autocompleting when
using bash then consider installing [argcomplete] and adding the
following to your bashrc file:

    eval "$(register-python-argcomplete spp)"

[argcomplete]: https://argcomplete.readthedocs.org/en/latest/

version:
  This is spp version %s

""" % VERSION
    raw = argparse.RawDescriptionHelpFormatter
    argp = argparse.ArgumentParser(formatter_class=raw, version=ver,
                                   description=textwrap.dedent(desc),
                                   epilog=textwrap.dedent(epi))
    argp.add_argument('--cfg', type=str, default='.',
                      help='configuration file directory')
    subp = argp.add_subparsers(title='subcommands',
                               dest='parser_name',
                               help='additional help',
                               metavar="<command>")
    names = mod.keys()
    names.sort()
    for name in names:
        mod[name].add_parser(subp, raw)
    try:
        argcomplete.autocomplete(argp)
    except NameError:
        pass
    return argp.parse_args()


def run():
    """Run program. """
    dirpath = split(abspath(__file__))[0]
    mod_names = [name for name in listdir(dirpath)
                 if isdir(join(dirpath, name))]
    mod = dict()
    for name in mod_names:
        tmp_mod = import_mod('spp.%s' % name)
        if hasattr(tmp_mod, 'add_parser'):
            mod[name] = tmp_mod
    arg = parse_options(mod)
    mod[arg.parser_name].run(arg)


if __name__ == '__main__':
    run()
