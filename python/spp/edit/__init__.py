"""Edit

Module to quickly open files with an editor.

"""

import textwrap
from os import listdir, system
from os.path import isfile, join, exists
from spp import error
from spp.config import get_cfg

DEFAULTS = {
    'editor': '$EDITOR',
    'path': '.'
}

DESC = """
Open a file if it is found in the path provided by the configuration
file."""


def valid_files(parsed_args, **_):
    """Return a list of valid files to edit."""
    cfg = get_cfg(parsed_args, 'edit', DEFAULTS)
    root = cfg['spp']['root']
    paths = cfg['edit']['path'].split(':')
    choices = []
    for path in paths:
        if path[0] in ['/']:
            abspath = path
        else:
            abspath = '%s/%s' % (root, path)
        try:
            choices.extend([fname for fname in listdir(abspath)
                            if isfile(join(abspath, fname))])
        except OSError:
            pass
    return choices


def add_parser(subp, raw):
    """Add a parser to the main subparser. """
    tmpp = subp.add_parser('edit', help='edit a file',
                           formatter_class=raw,
                           description=textwrap.dedent(DESC))
    tmpp.add_argument('filename', type=str,
                      help='name of the file to edit').completer = valid_files
    tmpp.add_argument('--path', type=str,
                      help='search path')
    tmpp.add_argument('--editor', type=str,
                      help='editor to open files')


def run(arg):
    """Run the edit command. """
    cfg = get_cfg(arg, 'edit', DEFAULTS)
    root = cfg['spp']['root']
    paths = cfg['edit']['path'].split(':')

    fname = arg.filename
    found = False
    for path in paths:
        if path[0] == '/':
            abspath = '%s/%s' % (path, fname)
        else:
            abspath = '%s/%s/%s' % (root, path, fname)
        if exists(abspath):
            found = True
            break
    if found:
        cmd = '%s "%s" > /dev/null' % (cfg['edit']['editor'], abspath)
    else:
        error("ERROR: file not found.\n")
    system(cmd)
