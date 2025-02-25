/* === This file is part of Calamares - <https://calamares.io> ===
 *
 *   SPDX-FileCopyrightText: 2020 Corentin Noël <corentin.noel@collabora.com>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 *   Calamares is Free Software: see the License-Identifier above.
 *
 */

#include "CreateLayoutsTests.h"

#include "core/PartitionLayout.h"

#include "JobQueue.h"
#include "partition/KPMManager.h"
#include "utils/Logger.h"

#include <memory>

#include <QtTest/QtTest>

using namespace Calamares::Units;

class PartitionTable;
class SmartStatus;

QTEST_GUILESS_MAIN( CreateLayoutsTests )

static Calamares::Partition::KPMManager* kpmcore = nullptr;
static Calamares::JobQueue* jobqueue = nullptr;

#define LOGICAL_SIZE 512

CreateLayoutsTests::CreateLayoutsTests()
{
    Logger::setupLogLevel( Logger::LOGDEBUG );
}

void
CreateLayoutsTests::init()
{
    jobqueue = new Calamares::JobQueue( nullptr );
    kpmcore = new Calamares::Partition::KPMManager();
}

void
CreateLayoutsTests::cleanup()
{
    delete kpmcore;
    delete jobqueue;
}

void
CreateLayoutsTests::testFixedSizePartition()
{
    const PartitionRole role( PartitionRole::Role::Any );

    PartitionLayout layout = PartitionLayout();
    TestDevice dev( QString( "test" ), LOGICAL_SIZE, 5_GiB / LOGICAL_SIZE );
    PartitionTable table( PartitionTable::TableType::msdos, 0, 5_GiB );

    if ( !layout.addEntry( { FileSystem::Type::Ext4, QString( "/" ), QString( "5MiB" ) } ) )
    {
        QFAIL( qPrintable( "Unable to create / partition" ) );
    }

    const auto partitions = layout.createPartitions(
        static_cast< Device* >( &dev ), 0, dev.totalLogical(), Config::LuksGeneration::Luks1, nullptr, &table, role );

    QCOMPARE( partitions.count(), 1 );

    QCOMPARE( partitions[ 0 ]->length(), 5_MiB / LOGICAL_SIZE );
}

void
CreateLayoutsTests::testPercentSizePartition()
{
    const PartitionRole role( PartitionRole::Role::Any );

    PartitionLayout layout = PartitionLayout();
    TestDevice dev( QString( "test" ), LOGICAL_SIZE, 5_GiB / LOGICAL_SIZE );
    PartitionTable table( PartitionTable::TableType::msdos, 0, 5_GiB );

    if ( !layout.addEntry( { FileSystem::Type::Ext4, QString( "/" ), QString( "50%" ) } ) )
    {
        QFAIL( qPrintable( "Unable to create / partition" ) );
    }

    const auto partitions = layout.createPartitions(
        static_cast< Device* >( &dev ), 0, dev.totalLogical(), Config::LuksGeneration::Luks1, nullptr, &table, role );

    QCOMPARE( partitions.count(), 1 );

    QCOMPARE( partitions[ 0 ]->length(), ( 5_GiB / 2 ) / LOGICAL_SIZE );
}

void
CreateLayoutsTests::testMixedSizePartition()
{
    const PartitionRole role( PartitionRole::Role::Any );

    PartitionLayout layout = PartitionLayout();
    TestDevice dev( QString( "test" ), LOGICAL_SIZE, 5_GiB / LOGICAL_SIZE );
    PartitionTable table( PartitionTable::TableType::msdos, 0, 5_GiB );

    if ( !layout.addEntry( { FileSystem::Type::Ext4, QString( "/" ), QString( "5MiB" ) } ) )
    {
        QFAIL( qPrintable( "Unable to create / partition" ) );
    }

    if ( !layout.addEntry( { FileSystem::Type::Ext4, QString( "/data" ), QString( "50%" ) } ) )
    {
        QFAIL( qPrintable( "Unable to create /data partition" ) );
    }

    if ( !layout.addEntry( { FileSystem::Type::Ext4, QString( "/bkup" ), QString( "50%" ) } ) )
    {
        QFAIL( qPrintable( "Unable to create /bkup partition" ) );
    }

    const auto partitions = layout.createPartitions(
        static_cast< Device* >( &dev ), 0, dev.totalLogical(), Config::LuksGeneration::Luks1, nullptr, &table, role );

    QCOMPARE( partitions.count(), 3 );

    QCOMPARE( partitions[ 0 ]->length(), 5_MiB / LOGICAL_SIZE );
    QCOMPARE( partitions[ 1 ]->length(), ( ( 5_GiB - 5_MiB ) / 2 ) / LOGICAL_SIZE );
    QCOMPARE( partitions[ 2 ]->length(), ( ( 5_GiB - 5_MiB ) / 2 ) / LOGICAL_SIZE );
}

// TODO: Get a clean way to instantiate a test Device from KPMCore
class DevicePrivate
{
public:
    QString m_Name;
    QString m_DeviceNode;
    qint64 m_LogicalSectorSize;
    qint64 m_TotalLogical;
    PartitionTable* m_PartitionTable;
    QString m_IconName;
    std::shared_ptr< SmartStatus > m_SmartStatus;
    Device::Type m_Type;
};

TestDevice::TestDevice( const QString& name, const qint64 logicalSectorSize, const qint64 totalLogicalSectors )
    : Device( std::make_shared< DevicePrivate >(),
              name,
              QString( "node" ),
              logicalSectorSize,
              totalLogicalSectors,
              QString(),
              Device::Type::Unknown_Device )
{
}

TestDevice::~TestDevice() {}
