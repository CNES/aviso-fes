# Copyright (c) 2025 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
import os
import pathlib
import shutil
import sys
import sysconfig
import time
import urllib.request
import zipfile

# Check Python requirement
MAJOR = sys.version_info[0]
MINOR = sys.version_info[1]

# Working directory
WORKING_DIRECTORY = pathlib.Path(__file__).parent.absolute()

# Dataset URL
DATASET_URL = 'https://osf.io/download/yfr24'

# Dataset target directory
DATASET_DIR = pathlib.Path(__file__).parent / 'tests' / 'python' / 'dataset'

# Cache file for dataset download status
CACHE_FILE = DATASET_DIR / '.download_cache'


def is_download_cached() -> bool:
    """Check if dataset was successfully downloaded and cached recently."""
    if not CACHE_FILE.exists() or not DATASET_DIR.exists():
        return False
    return True


def create_cache() -> None:
    """Create a cache file to indicate successful download."""
    CACHE_FILE.write_text(str(int(time.time())))


def progress_hook(count, block_size, total_size):
    """Progress hook for urllib to show download progress."""
    downloaded = count * block_size
    percent = min(100, (downloaded * 100) // total_size)

    # Show progress bar with percentage and size info
    bar_length = 50
    filled_length = int(bar_length * percent // 100)
    bar = '█' * filled_length + '░' * (bar_length - filled_length)

    # Format file sizes
    def format_bytes(bytes_val):
        for unit in ['B', 'KB', 'MB', 'GB']:
            if bytes_val < 1024.0:
                return f"{bytes_val:.1f} {unit}"
            bytes_val /= 1024.0
        return f"{bytes_val:.1f} TB"

    downloaded_str = format_bytes(downloaded)
    total_str = format_bytes(total_size)

    # Print progress on same line using carriage return to stderr for pytest
    # visibility
    sys.stderr.write(f"\rDownloading test dataset: {bar} {percent}% "
                     f"({downloaded_str}/{total_str})")
    sys.stderr.flush()


def download_test_data() -> None:
    """Download and extract test dataset if not already present."""
    # Check cache first for quick exit (unless forced)
    if is_download_cached():
        return

    # Create dataset directory if it doesn't exist
    DATASET_DIR.mkdir(parents=True, exist_ok=True)

    # Download the dataset
    zip_path = DATASET_DIR / 'dataset.zip'
    try:
        urllib.request.urlretrieve(
            DATASET_URL,
            zip_path,
            reporthook=progress_hook,
        )
        sys.stderr.write('\n')  # Add newline after progress bar

        # Extract the dataset
        with zipfile.ZipFile(zip_path, 'r') as zip_ref:
            zip_ref.extractall(DATASET_DIR.parent)

        # Remove the zip file
        zip_path.unlink()

        # Create cache to indicate successful download
        create_cache()

    except Exception:
        shutil.rmtree(DATASET_DIR, ignore_errors=True)
        raise


def build_dirname(extname=None):
    """Returns the name of the build directory."""
    extname = '' if extname is None else os.sep.join(extname.split('.')[:-1])
    path = pathlib.Path(WORKING_DIRECTORY, 'build',
                        f'lib.{sysconfig.get_platform()}-{MAJOR}.{MINOR}',
                        extname)
    if path.exists():
        return path
    return pathlib.Path(
        WORKING_DIRECTORY, 'build',
        f'lib.{sysconfig.get_platform()}-{sys.implementation.cache_tag}',
        extname)


def pytest_sessionstart(session):
    """Hook to run before any tests are collected."""
    download_test_data()


def push_front_syspath():
    """Add the build directory to the front of sys.path."""
    if WORKING_DIRECTORY.joinpath('setup.py').exists():
        # We are in the root directory of the development tree
        sys.path.insert(0, str(build_dirname().resolve()))


push_front_syspath()
