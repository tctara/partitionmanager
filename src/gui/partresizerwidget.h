/***************************************************************************
 *   Copyright (C) 2008,2010 by Volker Lanz <vl@fidra.de>                  *
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

#if !defined(PARTRESIZERWIDGET__H)

#define PARTRESIZERWIDGET__H

#include <QWidget>
#include <QLabel>

class Partition;
class PartWidget;
class Device;

class QPaintEvent;
class QResizeEvent;
class QMouseEvent;

/** @brief Widget that allows the user to resize a Partition.
	@author vl@fidra.de
*/
class PartResizerWidget : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(PartResizerWidget)

	public:
		PartResizerWidget(QWidget* parent);

	public:
		void init(Device& d, Partition& p, qint64 minFirst, qint64 maxLast);

		qint64 totalSectors() const { return maximumLastSector() - minimumFirstSector() + 1; } /**< @return total sectors (free + Partition's length) */

		qint64 minimumFirstSector() const { return m_MinimumFirstSector; } /**< @return the lowest allowed first sector */
		void setMinimumFirstSector(qint64 s) { m_MinimumFirstSector = s; } /**< @param s the new lowest allowed first sector */

		qint64 maximumFirstSector() const { return m_MaximumFirstSector; } /**< @return the highest allowed first sector */
		void setMaximumFirstSector(qint64 s) { m_MaximumFirstSector = s; } /**< @param s the new highest allowed first sector */

		qint64 minimumLastSector() const { return m_MinimumLastSector; } /**< @return the lowest allowed last sector */
		void setMinimumLastSector(qint64 s) { m_MinimumLastSector = s; } /**< @param s the new lowest allowed last sector */

		qint64 maximumLastSector() const { return m_MaximumLastSector; } /**< @return the highest allowed last sector */
		void setMaximumLastSector(qint64 s) { m_MaximumLastSector = s; } /**< @param s the new highest allowed last sector */

		void setMinimumLength(qint64 s);
		qint64 minimumLength() const { return m_MinimumLength; } /**< @return minimum length for Partition */

		void setMaximumLength(qint64 s);
		qint64 maximumLength() const { return m_MaximumLength; } /**< @return maximum length for the Partition */

		void setMoveAllowed(bool b);
		bool moveAllowed() const { return m_MoveAllowed; } /**< @return true if moving the Partition is allowed */

		bool readOnly() const { return m_ReadOnly; } /**< @return true if the widget is read only */
		void setReadOnly(bool b) { m_ReadOnly = b; } /**< @param b the new value for read only */

		static qint32 handleWidth() { return m_HandleWidth; } /**< @return the handle width in pixels */
		static qint32 handleHeight() { return m_HandleHeight; } /**< @return the handle height in pixels */

	signals:
		void firstSectorChanged(qint64);
		void lastSectorChanged(qint64);
		void lengthChanged(qint64);

	public slots:
		bool updateLength(qint64 newLength);
		bool updateFirstSector(qint64 newFirstSector);
		bool updateLastSector(qint64 newLastSector);
		bool movePartition(qint64 newFirstSector);

	protected:
		Partition& partition() { Q_ASSERT(m_Partition); return *m_Partition; }
		const Partition& partition() const { Q_ASSERT(m_Partition); return *m_Partition; }
		void setPartition(Partition& p) { m_Partition = &p; }

		Device& device() { Q_ASSERT(m_Device); return *m_Device; }
		const Device& device() const { Q_ASSERT(m_Device); return *m_Device; }
		void setDevice(Device& d) { m_Device = &d; }

		void paintEvent(QPaintEvent* event);
		void resizeEvent(QResizeEvent* event);
		void mousePressEvent(QMouseEvent* event);
		void mouseMoveEvent(QMouseEvent* event);
		void mouseReleaseEvent(QMouseEvent* event);

		PartWidget& partWidget() { Q_ASSERT(m_PartWidget); return *m_PartWidget; }
		const PartWidget& partWidget() const { Q_ASSERT(m_PartWidget); return *m_PartWidget; }

		void updatePositions();

		int partWidgetStart() const;
		int partWidgetWidth() const;

		QLabel& leftHandle() { return m_LeftHandle; }
		QLabel& rightHandle() { return m_RightHandle; }

		qint64 sectorsPerPixel() const;

		void set(qint64 newCap, qint64 newFreeBefore, qint64 newFreeAfter);

		void resizeLogicals();

		bool checkAlignment(const Partition& child, qint64 delta) const;

		QWidget* draggedWidget() { return m_DraggedWidget; }
		const QWidget* draggedWidget() const { return m_DraggedWidget; }

	private:
		Device* m_Device;
		Partition* m_Partition;
		PartWidget* m_PartWidget;

		qint64 m_MinimumFirstSector;
		qint64 m_MaximumFirstSector;
		qint64 m_MinimumLastSector;
		qint64 m_MaximumLastSector;
		qint64 m_MinimumLength;
		qint64 m_MaximumLength;

		QLabel m_LeftHandle;
		QLabel m_RightHandle;

		QWidget* m_DraggedWidget;
		int m_Hotspot;

		bool m_MoveAllowed;
		bool m_ReadOnly;

		static const qint32 m_HandleWidth;
		static const qint32 m_HandleHeight;
};

#endif

