/*************************************************************************
 *  Copyright (C) 2009, 2010 by Volker Lanz <vl@fidra.de>                *
 *  Copyright (C) 2015 by Teo Mrnjavac <teo@kde.org>                     *
 *  Copyright (C) 2016 by Andrius Štikonas <andrius@stikonas.eu>         *
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

#include "gui/editmountpointdialog.h"
#include "gui/editmountpointdialogwidget.h"

#include <core/partition.h>

#include <KConfigGroup>
#include <KGuiItem>
#include <KLocalizedString>
#include <KMessageBox>
#include <KSharedConfig>
#include <KStandardGuiItem>

#include <QDialogButtonBox>

EditMountPointDialog::EditMountPointDialog(QWidget* parent, Partition& p) :
    QDialog(parent),
    m_Partition(p),
    m_DialogWidget(new EditMountPointDialogWidget(this, partition()))
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);
    mainLayout->addWidget(&widget());
    setWindowTitle(xi18nc("@title:window", "Edit mount point for <filename>%1</filename>", p.deviceNode()));

    KConfigGroup kcg(KSharedConfig::openConfig(), "editMountPointDialog");
    restoreGeometry(kcg.readEntry<QByteArray>("Geometry", QByteArray()));

    QDialogButtonBox* dbb = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                                  Qt::Horizontal,
                                                  this );
    mainLayout->addWidget(dbb);
    connect(dbb, &QDialogButtonBox::accepted,
            this, &EditMountPointDialog::accept);
    connect(dbb, &QDialogButtonBox::rejected,
            this, &EditMountPointDialog::reject);
}

/** Destroys an EditMOuntOptionsDialog instance */
EditMountPointDialog::~EditMountPointDialog()
{
    KConfigGroup kcg(KSharedConfig::openConfig(), "editMountPointDialog");
    kcg.writeEntry("Geometry", saveGeometry());
}

void EditMountPointDialog::accept()
{
    if (KMessageBox::warningContinueCancel(this,
                                           xi18nc("@info", "<para>Are you sure you want to save the changes you made to the system table file <filename>/etc/fstab</filename>?</para>"
                                                   "<para><warning>This will overwrite the existing file on your hard drive now. This <strong>can not be undone</strong>.</warning></para>"),
                                           xi18nc("@title:window", "Really save changes?"),
                                           KGuiItem(xi18nc("@action:button", "Save changes"), QStringLiteral("arrow-right")),
                                           KStandardGuiItem::cancel(),
                                           QStringLiteral("reallyWriteMountPoints")) == KMessageBox::Cancel)
        return;

    if (widget().acceptChanges() && widget().writeMountpoints(QStringLiteral("/etc/fstab")))
        partition().setMountPoint(widget().editPath().text());

    QDialog::accept();
}
