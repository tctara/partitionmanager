/*************************************************************************
 *  Copyright (C) 2016 by Chantara Tith <tith.chantara@gmail.com>        *
 *                                                                       *
 *  This program is free software; you can redistribute it and/or        *
 *  modify it under the terms of the GNU General Public License as       *
 *  published by the Free Software Foundation; either version 3 of       *
 *  the License, or (at your option) any later version.                  *
 *                                                                       *
 *  This program is distributed in the hope that it will be useful,      *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 *  GNU General Public License for more details.                         *
 *                                                                       *
 *  You should have received a copy of the GNU General Public License    *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.*
 *************************************************************************/

#include "gui/listphysicalvolumes.h"

#include <fs/lvm2_pv.h>
#include <util/globallog.h>
#include <util/capacity.h>

class ListPhysicalVolumeWidgetItem : public QListWidgetItem
{
public:
    ListPhysicalVolumeWidgetItem(const Partition& p, bool checked) :
        QListWidgetItem(p.deviceNode() + QStringLiteral(" | ") + Capacity::formatByteSize(p.capacity()))
    {
        setToolTip(p.deviceNode());
        setSizeHint(QSize(0, 32));
        setCheckState( checked ? Qt::Checked : Qt::Unchecked);
    }
};

ListPhysicalVolumes::ListPhysicalVolumes(QWidget* parent) :
    QWidget(parent),
    Ui::ListPhysicalVolumesBase()
{
    setupUi(this);
}

void ListPhysicalVolumes::addPartition(const Partition& p, bool checked)
{
    ListPhysicalVolumeWidgetItem *item = new ListPhysicalVolumeWidgetItem(p, checked);
    listPhysicalVolumes().addItem(item);
}

QStringList ListPhysicalVolumes::checkedItems()
{
    QStringList rlist = QStringList();
    for (int i = 0; i < listPhysicalVolumes().count(); i++) {
        QListWidgetItem* item = listPhysicalVolumes().item(i);
        if(item && item->checkState() == Qt::Checked) {
            rlist << item->text().split(QStringLiteral("|"))[0].trimmed();
        }
    }
    return rlist;
}

