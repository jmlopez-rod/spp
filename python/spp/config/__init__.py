"""Config

This module is in charge of providing all the necessary settings to
the rest of the modules in spp.

"""

import os
import textwrap
import configparser
from spp import error, import_mod

DESC = """Edit a configuration file for spp.

Some actions performed by spp can be overwritten by using
configuration files.

To see the values that the configuration file can overwrite use the
`defaults` command. This will print a list of the keys and values spp
uses for the given command.

"""


def add_parser(subp, raw):
    "Add a parser to the main subparser. "
    tmpp = subp.add_parser('config', help='configure spp',
                           formatter_class=raw,
                           description=textwrap.dedent(DESC))
    tmpp.add_argument('var', type=str,
                      help='Must be in the form of command.var')
    tmpp.add_argument('value', type=str,  nargs='?', default=None,
                      help='var value')
    tmpp.add_argument('-v', action='store_true',
                      help='print config file location')


def read_config(arg):
    """Read the configuration file spp.config"""
    path = arg.cfg
    if path == '.' and not os.path.exists('spp.config'):
        if 'SPP_CONFIG_PATH' in os.environ:
            tmp_path = os.environ['SPP_CONFIG_PATH']
            if os.path.exists('%s/spp.config' % tmp_path):
                path = tmp_path
    elif not os.path.exists('%s/spp.config' % path):
        error("ERROR: %s/spp.config does not exist\n" % path)
    arg.cfg = path
    config = configparser.ConfigParser(allow_no_value=True)
    config.read('%s/spp.config' % path)
    return config


def write_config(config, arg):
    """Write the configuration file. """
    with open('%s/spp.config' % arg.cfg, 'w') as cfile:
        config.write(cfile)


def run(arg):
    "Configure spp by editing configuration file available to spp."
    config = read_config(arg)
    try:
        command, var = arg.var.split('.', 1)
    except ValueError:
        error("ERROR: '%s' is not of the form command.var\n" % arg.var)
    if arg.v:
        print 'path to spp.config: "%s"' % arg.cfg
    if arg.value is None:
        try:
            print config[command][var]
        except KeyError:
            pass
        return
    try:
        config[command][var] = arg.value
    except KeyError:
        config.add_section(command)
        config[command][var] = arg.value
    write_config(config, arg)


def _update_single(cfg, name, defaults=None):
    "Helper function for get_cfg."
    if defaults:
        for var, val in defaults.iteritems():
            cfg[name][var] = os.path.expandvars(val)
    else:
        mod = import_mod('spp.%s' % name)
        if hasattr(mod, "DEFAULTS"):
            for var, val in mod.DEFAULTS.iteritems():
                cfg[name][var] = os.path.expandvars(val)


def _update_from_file(cfg, name, cfg_file):
    "Helper function for get_cfg."
    if name in cfg_file:
        for var, val in cfg_file[name].iteritems():
            cfg[name][var] = os.path.expandvars(val)


def _update_from_arg(cfg, arg):
    "Helper function for get_cfg."
    argdict = vars(arg)
    for var in cfg[arg.parser_name]:
        if var in argdict and argdict[var] is not None:
            cfg[arg.parser_name][var] = argdict[var]
    for var in cfg['spp']:
        if var in argdict and argdict[var] is not None:
            cfg[arg.parser_name][var] = argdict[var]


def get_cfg(arg, names, defaults=None):
    """Obtain the settings for a command. """
    cfg = {
        'spp': {
            'root': '.',
            'include_path': '.'
        }
    }
    paths = os.environ['CPLUS_INCLUDE_PATH'].split(':')
    for path in paths:
        if 'spp' in os.listdir(path):
            cfg['spp']['include_path'] = path
            break
    cfg_file = read_config(arg)
    if 'spp' in cfg_file:
        for var, val in cfg_file['spp'].iteritems():
            cfg['spp'][var] = os.path.expandvars(val)
    cfg['spp']['root'] = arg.cfg
    if isinstance(names, list):
        for name in names:
            cfg[name] = dict()
            _update_single(cfg, name)
            _update_from_file(cfg, name, cfg_file)
    else:
        cfg[names] = dict()
        _update_single(cfg, names, defaults)
        _update_from_file(cfg, names, cfg_file)
    _update_from_arg(cfg, arg)
    return cfg
