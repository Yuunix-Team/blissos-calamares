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

_ = gettext.translation(
    "calamares-python",
    localedir=libcalamares.utils.gettext_path(),
    languages=libcalamares.utils.gettext_languages(),
    fallback=True,
).gettext


def pretty_name():
    return _("Pre-config before installing bootloader.")


# This is going to be changed from various methods
status = pretty_name()


def pretty_status_message():
    return status


def run():
    """
    Create misc.img and data.img in cases
    """
    root_mount_point = libcalamares.globalstorage.value("rootMountPoint")

    if not root_mount_point:
        libcalamares.utils.warning("No mount point for root partition")
        return (
            _("No mount point for root partition"),
            _('globalstorage does not contain a "rootMountPoint" key.'),
        )
    if not os.path.exists(root_mount_point):
        libcalamares.utils.warning('Bad root mount point "{}"'.format(root_mount_point))
        return (
            _("Bad mount point for root partition"),
            _('rootMountPoint is "{}", which does not exist.'.format(root_mount_point)),
        )

    os.replace(
        os.path.abspath("/usr/share/calamares/scripts/10_blissos"),
        os.path.join(root_mount_point, "etc/grub.d/10_linux"),
    )

    with open(os.path.abspath("/etc/default/grub"), "a") as grubConf:
        print("GRUB_DISABLE_OS_PROBER=false", file=grubConf)
        
        kernel_args = libcalamares.globalstorage.value("options")
        print("GRUB_CMDLINE_ANDROID='" + kernel_args + "'", file=grubConf)
        
        partitions = libcalamares.globalstorage.value("partitions")
        for partition in partitions:
            if partition["mountpoint"] == "/":
                print("DEVICE='" +partition["device"]+"'", file=grubConf)

        print("SRC=", file=grubConf)

    libcalamares.job.setprogress(1.0)
    return None
