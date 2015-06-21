/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2006-2014                                               *
 *   Umbrello UML Modeller Authors <umbrello-devel@kde.org>                *
 ***************************************************************************/

#include "optionstate.h"

#if QT_VERSION < 0x050000
#include <kglobal.h>
#endif

namespace Settings {

    /*
     * Impt: This ensures creation of OptionState object after
     * QApplication thereby avoiding nasty font rendering issues
     * which occurs due to creation of QFont objects before
     * QApplication object is created.
     */
#if QT_VERSION >= 0x050000
    Q_GLOBAL_STATIC(OptionState, opState)
#else
    K_GLOBAL_STATIC(OptionState, opState)
#endif

    /**
     * Save instance into a QDomElement.
     * @param element A QDomElement representing xml element data.
     */
    void ClassState::saveToXMI(QDomElement &element)
    {
        element.setAttribute(QLatin1String("showattribassocs"), showAttribAssocs);
        element.setAttribute(QLatin1String("showatts"),         showAtts);
        element.setAttribute(QLatin1String("showattsig"),       showAttSig);
        element.setAttribute(QLatin1String("showops"),          showOps);
        element.setAttribute(QLatin1String("showopsig"),        showOpSig);
        element.setAttribute(QLatin1String("showpackage"),      showPackage);
        element.setAttribute(QLatin1String("showpubliconly"),   showPublicOnly);
        element.setAttribute(QLatin1String("showscope"),        showVisibility);
        element.setAttribute(QLatin1String("showstereotype"),   showStereoType);
    }

    /**
     * Load instance from a QDomElement.
     * @param element A QDomElement representing xml element data.
     * @return true on success
     * @return false on error
     */
    bool ClassState::loadFromXMI(QDomElement &element)
    {
        QString temp = element.attribute(QLatin1String("showattribassocs"), QLatin1String("0"));
        showAttribAssocs = (bool)temp.toInt();
        temp = element.attribute(QLatin1String("showatts"), QLatin1String("0"));
        showAtts = (bool)temp.toInt();
        temp = element.attribute(QLatin1String("showattsig"), QLatin1String("0"));
        showAttSig = (bool)temp.toInt();
        temp = element.attribute(QLatin1String("showops"), QLatin1String("0"));
        showOps = (bool)temp.toInt();
        temp = element.attribute(QLatin1String("showopsig"), QLatin1String("0"));
        showOpSig = (bool)temp.toInt();
        temp = element.attribute(QLatin1String("showpackage"), QLatin1String("0"));
        showPackage = (bool)temp.toInt();
        temp = element.attribute(QLatin1String("showpubliconly"), QLatin1String("0"));
        showPublicOnly = (bool)temp.toInt();
        temp = element.attribute(QLatin1String("showscope"), QLatin1String("0"));
        showVisibility = (bool)temp.toInt();
        temp = element.attribute(QLatin1String("showstereotype"), QLatin1String("0"));
        showStereoType = (bool)temp.toInt();
        return true;
    }

    /**
     * Save instance into a QDomElement.
     * @param element A QDomElement representing xml element data.
     */
    void UIState::saveToXMI(QDomElement &element)
    {
        element.setAttribute(QLatin1String("backgroundcolor"),  backgroundColor.name());
        element.setAttribute(QLatin1String("fillcolor"),        fillColor.name());
        element.setAttribute(QLatin1String("font"),             font.toString());
        element.setAttribute(QLatin1String("griddotcolor"),     gridDotColor.name());
        element.setAttribute(QLatin1String("linecolor"),        lineColor.name());
        element.setAttribute(QLatin1String("linewidth"),        lineWidth);
        element.setAttribute(QLatin1String("textcolor"),        textColor.name());
        element.setAttribute(QLatin1String("usefillcolor"),     useFillColor);
    }

    /**
     * Load instance from a QDomElement.
     * @param element A QDomElement representing xml element data.
     * @return true on success
     * @return false on error
     */
    bool UIState::loadFromXMI(QDomElement &element)
    {
        QString backgroundColor = element.attribute(QLatin1String("backgroundcolor"));
        if (!backgroundColor.isEmpty())
            this->backgroundColor = QColor(backgroundColor);
        QString fillcolor = element.attribute(QLatin1String("fillcolor"));
        if (!fillcolor.isEmpty())
            this->fillColor = QColor(fillcolor);
        QString font = element.attribute(QLatin1String("font"));
        if (!font.isEmpty()) {
            this->font.fromString(font);
            this->font.setUnderline(false);
        }
        QString gridDotColor = element.attribute(QLatin1String("griddotcolor"));
        if (!gridDotColor.isEmpty())
            this->gridDotColor = QColor(gridDotColor);
        QString linecolor = element.attribute(QLatin1String("linecolor"));
        if (!linecolor.isEmpty())
            this->lineColor = QColor(linecolor);
        QString linewidth = element.attribute(QLatin1String("linewidth"));
        if (!linewidth.isEmpty())
            this->lineWidth = linewidth.toInt();
        QString textColor = element.attribute(QLatin1String("textcolor"));
        if (!textColor.isEmpty())
            this->textColor = QColor(textColor);
        QString usefillcolor = element.attribute(QLatin1String("usefillcolor"), QLatin1String("0"));
        this->useFillColor = (bool)usefillcolor.toInt();

        return true;
    }

    OptionState& optionState()
    {
        return *opState;
    }

    void setOptionState(const OptionState& optstate)
    {
        *opState = optstate;
    }

    OptionState::OptionState()
    {
    }

    /**
     * Save instance into a QDomElement.
     * @param element A QDomElement representing xml element data.
     */
    void OptionState::saveToXMI(QDomElement& element)
    {
        uiState.saveToXMI(element);
        classState.saveToXMI(element);
    }

    /**
     * Load instance from a QDomElement.
     * @param element A QDomElement representing xml element data.
     * @return true on success
     * @return false on error
     */
    bool OptionState::loadFromXMI(QDomElement& element)
    {
        uiState.loadFromXMI(element);
        classState.loadFromXMI(element);

        return true;
    }

}  // namespace Settings
