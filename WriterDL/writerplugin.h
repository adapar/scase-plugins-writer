/*************

    SCASE: extendable prototype of an assistive communication tool for people with Locked-in Syndrome

    Copyright (C) 2014  Andrés Aparicio

    This file is part of SCASE.

    SCASE is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SCASE is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SCASE.  If not, see <http://www.gnu.org/licenses/>.

    **********/

#ifndef WRITERPLUGIN_H
#define WRITERPLUGIN_H

#include <QObject>
#include <QString>
#include <QList>
#include <QRegExp>
#include <QSettings>
#include <QtPlugin>

#include "scase_plugin_writer_global.h"

#include "interfaces/IPlugin.h"
#include "interfaces/IBrowserLevel.h"
#include "interfaces/IBrowserItem.h"

#include "writertextedit.h"

class SCASE_PLUGIN_WRITERSHARED_EXPORT WriterPlugin : public QObject, public IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID SCASE_PLUGIN_WRITER_NAME)
    Q_INTERFACES(IPlugin)

public:
    WriterPlugin();

    //IPlugin methods
    bool invokeAction(const QString actionName_);
    QString getBrowserTree();
    QWidget *getOutputWidget();
    QString getName();
    QString getPluginPath();
    void setupOutputWidget();

private:
    //WriterPlugin methods
    Q_INVOKABLE void write(QString value);
    Q_INVOKABLE void write(QString value, QString repetitions);
    Q_INVOKABLE void write_special(QString value);
    Q_INVOKABLE void write_special(QString value, QString repetitions);
    Q_INVOKABLE void delete_previous_character();
    Q_INVOKABLE void delete_previous_word();
    Q_INVOKABLE void delete_all();
    Q_INVOKABLE void undo();
    Q_INVOKABLE void move_cursor(QString direction);
    Q_INVOKABLE void move_cursor(QString direction, QString type);

    void updatePresentationWidget();
    void updateRootLevel();

    void delete_content(QTextCursor::MoveOperation);

    QSettings *settings;

    QRegExp rxBasePredictorValidator;

    WriterTextEdit *presentationWidget;

    IBrowserLevel *rootLevel;

#ifdef SCASE_PLUGIN_WRITER_PREDICTION_ENABLED
    int predictedItemsAdded;
#endif

};

#endif // WRITERPLUGIN_H
