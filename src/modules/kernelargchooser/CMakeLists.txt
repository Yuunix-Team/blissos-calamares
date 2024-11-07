# === This file is part of Calamares - <https://calamares.io> ===
#
#   SPDX-FileCopyrightText: 2020 Adriaan de Groot <groot@kde.org>
#   SPDX-License-Identifier: BSD-2-Clause
#
calamares_add_plugin(kernelargchooser
    TYPE viewmodule
    EXPORT_MACRO PLUGINDLLEXPORT_PRO
    SOURCES
        Config.cpp
        groupstreeview.cpp
        LoaderQueue.cpp
        KernelArgChooserViewStep.cpp
        KernelArgChooserPage.cpp
        OptionTreeItem.cpp
        OptionModel.cpp
    UI
        page_chooser.ui
    LINK_PRIVATE_LIBRARIES
        ${qtname}::Network
        ${qtname}::Widgets
    SHARED_LIB
)

if(TARGET ${kfname}::CoreAddons)
    #calamares_add_test(
    #    kernelargchoosertest
    #    SOURCES Tests.cpp Config.cpp LoaderQueue.cpp OptionTreeItem.cpp OptionModel.cpp
    #    LIBRARIES ${qtname}::Widgets ${qtname}::Gui ${qtname}::Network ${kfname}::CoreAddons
    #)
endif()
