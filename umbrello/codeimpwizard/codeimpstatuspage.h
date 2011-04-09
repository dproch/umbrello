/*
    Copyright 2010  Umbrello UML Modeller Authors <uml-devel@uml.sf.net>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License or (at your option) version 3 or any later version
    accepted by the membership of KDE e.V. (or its successor approved
    by the membership of KDE e.V.), which shall act as a proxy
    defined in Section 14 of version 3 of the license.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef CODEIMPSTATUSPAGE_H
#define CODEIMPSTATUSPAGE_H

// app includes
#include "ui_codeimpstatuspage.h"

// kde includes
//#include <kled.h>

// qt includes
#include <QtGui/QWizardPage>
#include <QtGui/QSpacerItem>
#include <QtGui/QHBoxLayout>
#include <QtCore/QFileInfo>

/**
 * @author Andi Fischer
 */
class CodeImpStatusPage : public QWizardPage, private Ui::CodeImpStatusPage
{
    Q_OBJECT
public:
    CodeImpStatusPage(QWidget *parent = 0);
    ~CodeImpStatusPage();
    void initializePage();
    bool isComplete() const;

private:
    QList<QFileInfo> m_files;
    bool             m_workDone;
      
protected slots:
    void importCode();
    void importStop();
    void fileImported(const QFileInfo& file, const QString& text);
    void messageToApp(const QString& text);
    void populateStatusList();

};

//class LedStatus : public QWidget
//{
//        Q_OBJECT
//    public:
//        LedStatus(int width, int height) {
//            setFixedSize(width, height);
//            QHBoxLayout* layout = new QHBoxLayout();
//            layout->addItem(new QSpacerItem(20, 20));
//            m_led = new KLed(QColor(124, 252, 0), KLed::Off, KLed::Sunken, KLed::Circular);
//            layout->addWidget(m_led);
//            layout->addItem(new QSpacerItem(20, 20));
//            setLayout(layout);
//        };
//        ~LedStatus() {};
//        void setColor(const QColor& color) { m_led->setColor(color); };
//        void setOn(bool isOn) { isOn ? m_led->setState(KLed::On) : m_led->setState(KLed::Off); };
//    private:
//        KLed *m_led;
//};

#endif
