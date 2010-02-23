/***************************************************************************
 *   Copyright (C) 2008 by Volker Lanz <vl@fidra.de>                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 ***************************************************************************/

#if !defined(PARTITIONTABLE__H)

#define PARTITIONTABLE__H

#include "core/partitionnode.h"
#include "core/partitionrole.h"

#include <QList>
#include <qglobal.h>

class Device;
class Partition;
class LibPartedBackend;

/** @brief The partition table (a.k.a Disk Label)

	PartitionTable represents a partition table (or disk label). The current implementation makes quite
	a number of assumptions that are only true for MSDOS disk labels.

	PartitionTable has child nodes that represent Partitions.

	@author vl@fidra.de
*/
class PartitionTable : public PartitionNode
{
	Q_DISABLE_COPY(PartitionTable)

	friend class LibPartedBackend;

	public:
		enum LabelType
		{
			unknownLabel = -1,

			aix,
			bsd,
			dasd,
			msdos,
			msdos_vista,
			dvh,
			gpt,
			loop,
			mac,
			pc98,
			amiga,
			sun
		};

		/** Partition flags as defined by libparted */
		enum Flag
		{
			FlagNone = 0,
			FlagBoot = 1,
			FlagRoot = 2,
			FlagSwap = 4,
			FlagHidden = 8,
			FlagRaid = 16,
			FlagLvm = 32,
			FlagLba = 64,
			FlagHpService = 128,
			FlagPalo = 256,
			FlagPrep = 512,
			FlagMsftReserved = 1024
		};

		Q_DECLARE_FLAGS(Flags, Flag)

	public:
		PartitionTable(LabelType type, qint64 first_usable, qint64 last_usable);
		~PartitionTable();

	public:
		PartitionNode* parent() { return NULL; } /**< @return always NULL for PartitionTable */
		const PartitionNode* parent() const { return NULL; } /**< @return always NULL for PartitionTable */

		bool isRoot() const { return true; } /**< @return always true for PartitionTable */
		bool isReadOnly() const { return diskLabelIsReadOnly(type()); } /**< @return true if the PartitionTable is read only */

		Partitions& children() { return m_Children; } /**< @return the children in this PartitionTable */
		const Partitions& children() const { return m_Children; } /**< @return the children in this PartitionTable */

		void setType(const Device& d, LabelType t);

		void append(Partition* partition);

		qint64 freeSectorsBefore(const Partition& p) const;
		qint64 freeSectorsAfter(const Partition& p) const;

		bool hasExtended() const;
		Partition* extended();

		PartitionRole::Roles childRoles(const Partition& p) const;

		int numPrimaries() const;
		int maxPrimaries() const { return m_MaxPrimaries; } /**< @return max number of primary partitions this PartitionTable can handle */

		PartitionTable::LabelType type() const { return m_Type; } /**< @return the disk label type */
		const QString typeName() const { return labelTypeToName(type()); } /**< @return the name of this PartitionTable type according to libparted */

		qint64 firstUsable() const { return m_FirstUsable; }
		qint64 lastUsable() const { return m_LastUsable; }

		void updateUnallocated(const Device& d);
		void insertUnallocated(const Device& d, PartitionNode* p, qint64 start) const;

		bool isVistaDiskLabel() const;

		static QList<Flag> flagList();
		static QString flagName(Flag f);
		static QStringList flagNames(Flags f);

		static void removeUnallocated(PartitionNode* p);
		void removeUnallocated();

		static bool isSnapped(const Device& d, const Partition& p);
		static bool snap(const Device& d, Partition& p, const Partition* originalPartition = NULL);

		static qint64 defaultFirstUsable(const Device& d, LabelType t);
		static qint64 defaultLastUsable(const Device& d, LabelType t);

		static PartitionTable::LabelType nameToLabelType(const QString& n);
		static QString labelTypeToName(LabelType l);
		static qint64 maxPrimariesForLabelType(LabelType l);
		static bool diskLabelSupportsExtended(LabelType l);
		static bool diskLabelIsReadOnly(LabelType l);

	protected:
		void setMaxPrimaries(qint32 n) { m_MaxPrimaries = n; }
		void setFirstUsableSector(qint64 s) { m_FirstUsable = s; }
		void setLastUsableSector(qint64 s) { m_LastUsable = s; }

	private:
		Partitions m_Children;
		qint32 m_MaxPrimaries;
		LabelType m_Type;
		qint64 m_FirstUsable;
		qint64 m_LastUsable;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(PartitionTable::Flags)


#endif

