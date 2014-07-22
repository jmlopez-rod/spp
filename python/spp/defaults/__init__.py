"""Defaults

Print the default values of an spp subcommand.

"""

import os
import textwrap
from spp import error, import_mod
from spp.config import get_cfg

DEFAULTS = {}

DESC = """
print the default a subcommand uses in abscense of a configuration
file or options."""

def add_parser(subp, raw):
    """Add a parser to the main subparser. """
    tmpp = subp.add_parser('defaults', help='show default values',
                            formatter_class=raw,
                            description=textwrap.dedent(DESC))
    tmpp.add_argument('subcommand', type=str,
                      help='name of subcommand')


def run(arg):
    """Run the command. """
    try:
        mod = import_mod('spp.%s' % arg.subcommand)
    except ImportError:
        error("ERROR: `%s` is not a subcommand.\n" % arg.subcommand)
    for key, val in mod.DEFAULTS.iteritems():
        print '%s = %s' % (key, val)
