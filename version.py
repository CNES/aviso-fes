#!/usr/bin/env python3
"""Handle the library version number."""
import argparse
import os
import re
import subprocess
import sys

PATTERN = '#define FES_VERSION'


def execute(cmd):
    """
    Executes a command and returns the lines displayed on the standard output
    """
    process = subprocess.Popen(cmd,
                               shell=True,
                               stdout=subprocess.PIPE,
                               stderr=subprocess.PIPE)
    stdout = process.stdout.read()
    if sys.version_info[0] == 3:
        stdout = stdout.decode('utf8')
    return stdout


def update_version(path, version, pattern, replaced_line):
    """Updating the version number description"""
    with open(path) as stream:
        lines = stream.readlines()
    pattern = re.compile(pattern)

    for idx, line in enumerate(lines):
        match = pattern.search(line)
        if match is not None:
            lines[idx] = replaced_line % version

    with open(path, 'w') as stream:
        stream.write(''.join(lines))


def update_meta(path, version):
    """Updating the version number description in conda/meta.yaml."""
    update_version(path, version, r'{% set version = ".*" %}',
                   '{%% set version = "%s" %%}\n')


def update_python_module(path, version):
    """Updating the version number in the python module."""
    update_version(path, version, r'm.attr\(__version__\) = "(.*)";',
                   'm.attr(__version__) = "%s";\n')


def revision(path, update=False):
    """
    Creation of the file describing the library version.
    """
    if not update:
        pattern = re.compile(PATTERN + r' "(.*)"').search
        with open(path) as stream:
            for line in stream:
                match = pattern(line)
                if match is not None:
                    return tuple(
                        int(item) for item in match.group(1).split('.'))

    stdout = execute('git describe --tags --dirty --long --always').strip()
    pattern = re.compile(r'([\w\d\.]+)-(\d+)-g([\w\d]+)(?:-(dirty))?')
    match = pattern.search(stdout)
    assert match is not None
    (major, minor, patch) = (int(item) for item in match.group(1).split('.'))

    if update:
        with open(path) as stream:
            lines = stream.readlines()

        for idx, line in enumerate(lines):
            if PATTERN in line:
                lines[idx] = PATTERN + " \"%d.%d.%d\"\n" % (major, minor,
                                                            patch)

        with open(path, 'w') as stream:
            stream.writelines(lines)

        update_meta(
            os.path.join(os.path.dirname(os.path.abspath(__file__)),
                         'conda/meta.yaml'),
            '%d.%d.%d' % (major, minor, patch))
        update_python_module(
            os.path.join(os.path.dirname(os.path.abspath(__file__)),
                         'python/main.cpp'),
            '%d.%d.%d' % (major, minor, patch))
    return (major, minor, patch)


def usage():
    """Parse arguments"""
    parser = argparse.ArgumentParser(
        description='Handle the version of the library')
    parser.add_argument(
        '--update',
        help='Update the version number inf the FES Header file',
        action='store_true')
    return parser.parse_args()


def main():
    """Main function"""
    args = usage()
    path = os.path.normpath(
        os.path.join(os.path.dirname(os.path.abspath(__file__)), 'include',
                     'fes.h'))
    sys.stdout.write('%d %d %d' % revision(path, args.update))


if __name__ == '__main__':
    main()
