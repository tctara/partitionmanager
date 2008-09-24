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

#if !defined(SIZEDIALOGWIDGET__H)

#define SIZEDIALOGWIDGET__H

#include "ui_sizedialogwidgetbase.h"

#include <QWidget>
#include <qdebug.h>

/** @brief Central widget for the SizeDialogBase
	@author vl@fidra.de
*/
class SizeDialogWidget : public QWidget, public Ui::SizeDialogWidgetBase
{
	Q_OBJECT

	public:
		SizeDialogWidget(QWidget* parent) : QWidget(parent), Ui::SizeDialogWidgetBase() { setupUi(this); }

	public:
		PartResizerWidget& partResizerWidget() { Q_ASSERT(m_PartResizerWidget); return *m_PartResizerWidget; }

		QSpinBox& spinFreeBefore() { Q_ASSERT(m_SpinFreeBefore); return *m_SpinFreeBefore; }
		QSpinBox& spinFreeAfter() { Q_ASSERT(m_SpinFreeAfter); return *m_SpinFreeAfter; }
		QSpinBox& spinCapacity() { Q_ASSERT(m_SpinCapacity); return *m_SpinCapacity; }

		QLabel& labelMinSize() { Q_ASSERT(m_LabelMinSize); return *m_LabelMinSize; }
		QLabel& labelMaxSize() { Q_ASSERT(m_LabelMaxSize); return *m_LabelMaxSize; }

		QComboBox& comboRole() { Q_ASSERT(m_ComboRole); return *m_ComboRole; }
		QComboBox& comboFileSystem() { Q_ASSERT(m_ComboFileSystem); return *m_ComboFileSystem; }

		void hideRole() { delete m_LabelRole; m_LabelRole = NULL; delete m_ComboRole; m_ComboRole = NULL; }
		void hideFileSystem() { delete m_LabelFileSystem; m_LabelFileSystem = NULL; delete m_ComboFileSystem; m_ComboFileSystem = NULL; }
};

#endif
