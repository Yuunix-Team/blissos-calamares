#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# === This file is part of Calamares - <https://calamares.io> ===
#
#   SPDX-FileCopyrightText: 2024 Shadichy <shadichy.dev@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#
#   Calamares is Free Software: see the License-Identifier above.
#

import os
import re
import shutil
import subprocess
import sys
import tempfile

import libcalamares

import gettext
_ = gettext.translation("calamares-python",
                        localedir=libcalamares.utils.gettext_path(),
                        languages=libcalamares.utils.gettext_languages(),
                        fallback=True).gettext

def pretty_name():
    return _("Generating needed disk images.")

# This is going to be changed from various methods
status = pretty_name()

def pretty_status_message():
    return status


class HostError(Exception):
    """Exception raised when the call to returns a non-zero exit code

    Attributes:
        message -- explanation of the error
    """

    def __init__(self, message):
        self.message = message

def line_cb(line):
    """
    Writes every line to the debug log and displays it in calamares
    :param line: The line of output text from the command
    """
    global status_update_time
    libcalamares.utils.debug("gen-img: " + line.strip())
    if (time.time() - status_update_time) > 0.5:
        libcalamares.job.setprogress(0)
        status_update_time = time.time()

def run_in_host(command, line_func):
    proc = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, universal_newlines=True,
                            bufsize=1)
    for line in proc.stdout:
        if line.strip():
            line_func(line)
    proc.wait()
    if proc.returncode != 0:
        raise HostError("Failed to run gen-img")

def run():
    """
    Create misc.img and data.img in cases
    """
    root_mount_point = libcalamares.globalstorage.value("rootMountPoint")

    if not root_mount_point:
        libcalamares.utils.warning("No mount point for root partition")
        return (_("No mount point for root partition"),
                _("globalstorage does not contain a \"rootMountPoint\" key."))
    if not os.path.exists(root_mount_point):
        libcalamares.utils.warning("Bad root mount point \"{}\"".format(root_mount_point))
        return (_("Bad mount point for root partition"),
                _("rootMountPoint is \"{}\", which does not exist.".format(root_mount_point)))

    run_in_host(["/usr/share/calamares/scripts/gen-img", root_mount_point], line_cb)
    libcalamares.job.setprogress(1.0)
    return None
