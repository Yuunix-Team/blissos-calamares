/* === This file is part of Calamares - <https://calamares.io> ===
 *
 *   SPDX-FileCopyrightText: 2016 Luca Giambonini <almack@chakraos.org>
 *   SPDX-FileCopyrightText: 2016 Lisa Vitolo <shainer@chakraos.org>
 *   SPDX-FileCopyrightText: 2017 Kyle Robbertze  <krobbertze@gmail.com>
 *   SPDX-FileCopyrightText: 2017-2018 2020, Adriaan de Groot <groot@kde.org>
 *   SPDX-FileCopyrightText: 2023 Vladislav Nepogodin <nepogodin.vlad@gmail.com>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 *   Calamares is Free Software: see the License-Identifier above.
 *
 */

#include "KernelArgChooserViewStep.h"

#include "KernelArgChooserPage.h"

CALAMARES_PLUGIN_FACTORY_DEFINITION( KernelArgChooserViewStepFactory, registerPlugin< KernelArgChooserViewStep >(); )

KernelArgChooserViewStep::KernelArgChooserViewStep( QObject* parent )
    : Calamares::ViewStep( parent )
    , m_widget( new KernelArgChooserPage( &m_config ) )
    , m_nextEnabled( false )
{
    connect( &m_config, &Config::statusReady, this, &KernelArgChooserViewStep::nextIsReady );

    m_config.model()->setUpdateNextCall( [this]( bool enabled ) {
        this->updateNextEnabled( enabled );
    });
}


KernelArgChooserViewStep::~KernelArgChooserViewStep()
{
    if ( m_widget && m_widget->parent() == nullptr )
    {
        m_widget->deleteLater();
    }
}


QString
KernelArgChooserViewStep::prettyName() const
{
    return m_config.sidebarLabel();

#if defined( TABLE_OF_TRANSLATIONS )
    __builtin_unreachable();
    // This is a table of "standard" labels for this module. If you use them
    // in the label: sidebar: section of the config file, the existing
    // translations can be used.
    //
    // These translations still live here, even though the lookup
    // code is in the Config class.
    tr( "Option selection" );
    tr( "Office software" );
    tr( "Office option" );
    tr( "Browser software" );
    tr( "Browser option" );
    tr( "Web browser" );
    tr( "Kernel", "label for kernelargchooser module, Linux kernel" );
    tr( "Services", "label for kernelargchooser module, system services" );
    tr( "Login", "label for kernelargchooser module, choose login manager" );
    tr( "Desktop", "label for kernelargchooser module, choose desktop environment" );
    tr( "Applications" );
    tr( "Communication", "label for kernelargchooser module" );
    tr( "Development", "label for kernelargchooser module" );
    tr( "Office", "label for kernelargchooser module" );
    tr( "Multimedia", "label for kernelargchooser module" );
    tr( "Internet", "label for kernelargchooser module" );
    tr( "Theming", "label for kernelargchooser module" );
    tr( "Gaming", "label for kernelargchooser module" );
    tr( "Utilities", "label for kernelargchooser module" );
#endif
}


QWidget*
KernelArgChooserViewStep::widget()
{
    return m_widget;
}


bool
KernelArgChooserViewStep::isNextEnabled() const
{
    return !m_config.required() || m_nextEnabled;
}


bool
KernelArgChooserViewStep::isBackEnabled() const
{
    return true;
}


bool
KernelArgChooserViewStep::isAtBeginning() const
{
    return true;
}


bool
KernelArgChooserViewStep::isAtEnd() const
{
    return true;
}


Calamares::JobList
KernelArgChooserViewStep::jobs() const
{
    return Calamares::JobList();
}


void
KernelArgChooserViewStep::onActivate()
{
    m_widget->onActivate();
}

void
KernelArgChooserViewStep::onLeave()
{
    m_config.finalizeGlobalStorage();
}

void
KernelArgChooserViewStep::nextIsReady()
{
    m_nextEnabled = true;
    emit nextStatusChanged( true );
}

void
KernelArgChooserViewStep::setConfigurationMap( const QVariantMap& configurationMap )
{
    m_config.setConfigurationMap( configurationMap );
}

void
KernelArgChooserViewStep::updateNextEnabled( bool enabled )
{
    m_nextEnabled = enabled;
    emit nextStatusChanged( enabled );
}
