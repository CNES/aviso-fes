#!/usr/bin/env python3
"""Handle the library version number."""
import argparse
import datetime
import os
import re
import subprocess
import sys


def execute(cmd):
    """
    Executes a command and returns the lines displayed on the standard output
    """
    process = subprocess.Popen(
        cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout = process.stdout.read()
    if sys.version_info[0] == 3:
        stdout = stdout.decode('utf8')
    return stdout


def revision(path, update=False):
    """
    Creation of the file describing the library version.
    """
    stdout = execute("git describe --tags --dirty --long --always").strip()
    pattern = re.compile(r'([\w\d\.]+)-(\d+)-g([\w\d]+)(?:-(dirty))?')
    match = pattern.search(stdout)

    # If the information is unavailable (execution of this function outside the
    # development environment), file generation is not possible
    if not stdout:
        pattern = re.compile(r'\s+return "(\d+)"')
        with open(path, "r") as stream:
            for line in stream:
                line = line.strip()
                if "#define FES_VERSION" in line:
                    return line.split()[-1]

    (major, minor, patch) = (int(item) for item in match.group(1).split("."))

    # sha1 = match.group(3)
    # stdout = execute("git log  %s -1 --format='%%H %%at'" % sha1)
    # stdout = stdout.strip().split()
    # date = datetime.datetime.fromtimestamp(int(stdout[1])).isoformat()

    if update:
        with open(path, "r") as stream:
            lines = stream.readlines()

        for idx, line in enumerate(lines):
            if "#define VERSION" in line:
                lines[idx] = "#define FES_VERSION %d.%d.%d" % (major, minor,
                                                               patch)

        with open(path, "w") as stream:
            stream.writelines(lines)

    return (major, minor, patch)


def usage():
    """Parse arguments"""
    parser = argparse.ArgumentParser(
        description="Handle the version of the library")
    parser.add_argument(
        "--update",
        help="Update the version number inf the FES Header file",
        action="store_true")
    return parser.parse_args()


def main():
    """Main function"""
    args = usage()
    path = os.path.normpath(
        os.path.join(
            os.path.dirname(os.path.abspath(__file__)), "include", "fes.h"))
    sys.stdout.write("%d %d %d" % revision(path, args.update))


if __name__ == "__main__":
    main()
