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

#include "writerplugin.h"

#include <QDebug>
#include <QMetaMethod>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QCoreApplication>
#include <QGenericArgument>
#include <QFontMetrics>
#include <QTextCursor>
#include <QTextBlockFormat>
#include <QTextDocument>
#include <QStringList>
#include <QTextCodec>
#include <QtCore/qmath.h>

WriterPlugin::WriterPlugin()
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));

    browserItemDelegate = NULL;
    rootLevel = NULL;

#ifdef SCASE_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "WriterPlugin::WriterPlugin:pluginDirPath?" << getPluginPath();
#endif

    settings = new QSettings(getPluginPath() + SCASE_PLUGIN_WRITER_SETTINGS_FILE + ".ini", QSettings::IniFormat, this);

#ifdef SCASE_PLUGIN_WRITER_PREDICTION_ENABLED
    predictedItemsAdded = 0;
#endif

    rxBasePredictorValidator = QRegExp("\\w+");

    presentationWidget = new WriterTextEdit();

    presentationWidget->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    presentationWidget->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    presentationWidget->setUndoRedoEnabled(true);
    presentationWidget->ensureCursorVisible();
    presentationWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void WriterPlugin::setupOutputWidget() {
    QString configuredColor = settings->value("presentation/color", "000000").toString().trimmed();
    QString configuredBackgroundColor = settings->value("presentation/background_color", "ffffff").toString().trimmed();
    QString configuredSize = settings->value("presentation/size", "100%").toString().trimmed();
    int configuredLines = settings->value("presentation/lines", 5).toInt();

#ifdef SCASE_PLUGIN_DEBUG_LEVEL_VERBOSE
    QStringList keys = settings->allKeys();
    foreach (QString key, keys) {
        qDebug() << "WriterPlugin::setupOutputWidget:key?" << key.toUtf8();
    }
    qDebug() << "WriterPlugin::setupOutputWidget:configuredColor?" << configuredColor;
    qDebug() << "WriterPlugin::setupOutputWidget:configuredBackgroundColor?" << configuredBackgroundColor;
    qDebug() << "WriterPlugin::setupOutputWidget:configuredSize?" << configuredSize;
    qDebug() << "WriterPlugin::setupOutputWidget:configuredLines?" << configuredLines;
#endif

    int containerSize = presentationWidget->parentWidget()->size().height();

    QRegExp rxSizeIsPercentage("^[0-9]+%$");
    QRegExp rxSizeIsNumber("^[0-9]+$");

    bool sizeIsProportional = true;
    int size = 100;

    if ( rxSizeIsPercentage.exactMatch(configuredSize) ) {
        configuredSize.chop(1);
        size = configuredSize.toInt();
        sizeIsProportional = true;
    } else if ( rxSizeIsNumber.exactMatch(configuredSize) ) {
        size = configuredSize.toInt();
        sizeIsProportional = false;
    }

#ifdef SCASE_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "WriterPlugin::setupOutputWidget:sizeIsProportional?" << (sizeIsProportional ? "yes" : "no");
    qDebug() << "WriterPlugin::setupOutputWidget:containerSize?" << containerSize;
    qDebug() << "WriterPlugin::setupOutputWidget:size?" << size;
#endif

    if (sizeIsProportional) {
        size = qCeil((containerSize * size) / 100.0f);
#ifdef SCASE_PLUGIN_DEBUG_LEVEL_VERBOSE
        qDebug() << "WriterPlugin::setupOutputWidget:size(calculated)?" << size;
#endif
    }

    if (size > containerSize) {
        size = containerSize;
    }

    int fontSize = qCeil(size / (configuredLines * 1.5f));

    QString presentationWidgetStyle = QString("WriterTextEdit { font-family: Helvetica, Arial; font-size: %1px; background-color: #%2; color: #%3; }").arg(QString::number(fontSize), configuredBackgroundColor, configuredColor);

#ifdef SCASE_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "WriterPlugin::setupOutputWidget:fontSize?" << fontSize;
    qDebug() << "WriterPlugin::setupOutputWidget:presentationWidgetStyle?" << presentationWidgetStyle;
#endif

    presentationWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    presentationWidget->setStyleSheet(presentationWidgetStyle);
    presentationWidget->setCursorWidth(12);

    presentationWidget->setFixedHeight(size);

    QTextCursor cursor = presentationWidget->textCursor();
    QTextBlockFormat blockFormat = cursor.blockFormat();
    blockFormat.setLineHeight(150, QTextBlockFormat::ProportionalHeight);
    cursor.setBlockFormat(blockFormat);
    presentationWidget->setTextCursor(cursor);
}

QString WriterPlugin::getPluginPath() {
    return QString("plugins/");
}

bool WriterPlugin::invokeAction(const QString actionName_) {
    bool output = false;

    QStringList parts = actionName_.split(",");

    if (parts.size() > 0) {
        QString methodName = QString(parts.at(0));

        QGenericArgument argumentTable[ 10 ];

        parts.removeFirst();

        QStringList signatureArguments;

        int i = 0;

        for (; i < parts.size(); i++) {
            argumentTable[i] = Q_ARG(QString, parts.at(i));
#ifdef SCASE_PLUGIN_DEBUG_LEVEL_VERBOSE
            qDebug() << "argumentTable[" << i << "] comes from " << parts.at(i);
            qDebug() << "argumentTable[" << i << "] is " << argumentTable[i].name();
#endif
            signatureArguments.append("QString");
        }

        if (i < 10) {
            argumentTable[i] = QGenericArgument(0);
#ifdef SCASE_PLUGIN_DEBUG_LEVEL_VERBOSE
            qDebug() << "0:argumentTable[" << i << "] is " << argumentTable[i].name();
#endif
            for (i++; i < 10; i++) {
                argumentTable[i] = QGenericArgument();
#ifdef SCASE_PLUGIN_DEBUG_LEVEL_VERBOSE
                qDebug() << ":argumentTable[" << i << "] is " << argumentTable[i].name();
#endif
            }
        }

        QString signature = methodName + "(" + signatureArguments.join(",") + ")";

#ifdef SCASE_PLUGIN_DEBUG_LEVEL_VERBOSE
        qDebug() << "signature:" << signature;
#endif

        int methodIndex = metaObject()->indexOfMethod(signature.toLatin1().constData());

        if (methodIndex > -1) {
            QMetaMethod metaMethod = metaObject()->method(methodIndex);
            output = metaMethod.invoke(this, argumentTable[0], argumentTable[1], argumentTable[2], argumentTable[3], argumentTable[4], argumentTable[5], argumentTable[6], argumentTable[7], argumentTable[8], argumentTable[9]);
#ifdef SCASE_PLUGIN_DEBUG_LEVEL_VERBOSE
        } else {
            qDebug() << "signature:" << signature << "not found";
#endif
        }
    }

    return output;
}

QString WriterPlugin::getBrowserTree() {
    QString browserTreeFile = getPluginPath() + "lang/" + settings->value("presentation/language", "en").toString().trimmed() + "/" + SCASE_PLUGIN_WRITER_BROWSER_TREE_FILE + ".xml";
#ifdef SCASE_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "browserTreeFile:" << browserTreeFile;
#endif

    QFile xmlBrowserTree(browserTreeFile);

    if (!xmlBrowserTree.open(QIODevice::ReadOnly)) {
        return "";
    }

    QTextStream in(&xmlBrowserTree);

    QString output = in.readAll();

    xmlBrowserTree.close();

    return output;
}

QWidget *WriterPlugin::getOutputWidget() {
    return presentationWidget;
}

QString WriterPlugin::getName() {
    return QString(SCASE_PLUGIN_WRITER_NAME);
}

void WriterPlugin::write_special(QString value) {
    write_special(value, "1");
}

void WriterPlugin::write_special(QString value, QString repetitions) {
    QString dataToAdd = "";
    if (value == "space") {
        dataToAdd = " ";
    } else if (value == "period") {
        dataToAdd = ".";
    } else if (value == "comma") {
        dataToAdd = ",";
    } else if (value == "semicolon") {
        dataToAdd = ";";
    } else if (value == "colon") {
        dataToAdd = ":";
    } else if (value == "enter") {
        dataToAdd = "\n";
    } else if (value == "tab") {
        dataToAdd = "\t";
    }
    if (dataToAdd.size() > 0) {
        write(dataToAdd, repetitions);
    }
}

void WriterPlugin::write(QString value) {
    write(value, "1");
}

void WriterPlugin::write(QString value, QString repetitions) {
#ifdef SCASE_PLUGIN_DEBUG_LEVEL_VERBOSE
    qDebug() << "value:" << value;
    qDebug() << "repetitions:" << repetitions;
#endif

    int total = repetitions.toInt();

    if (total > 0) {
        QTextCursor cursor = presentationWidget->textCursor();
        cursor.clearSelection();
        for (int i = 0; i < total; i++) {
            cursor.insertText(value);
        }
        updatePresentationWidget();
    }
}

void WriterPlugin::delete_previous_character() {
    delete_content(QTextCursor::PreviousCharacter);
}

void WriterPlugin::delete_previous_word() {
    delete_content(QTextCursor::PreviousWord);
}

void WriterPlugin::delete_all() {
    presentationWidget->moveCursor(QTextCursor::End);
    delete_content(QTextCursor::Start);
}

void WriterPlugin::delete_content(QTextCursor::MoveOperation moveOperation) {
    QTextCursor cursor = presentationWidget->textCursor();
    cursor.clearSelection();
    cursor.movePosition(moveOperation, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    cursor.clearSelection();
    presentationWidget->setTextCursor(cursor);
    updatePresentationWidget();
}

void WriterPlugin::undo() {
    presentationWidget->undo();
    updatePresentationWidget();
}

void WriterPlugin::move_cursor(QString direction) {
    move_cursor(direction, "");
}

void WriterPlugin::move_cursor(QString direction, QString type) {
    QTextCursor::MoveOperation moveOperation = QTextCursor::NoMove;

    if (direction == "up") {
        moveOperation = QTextCursor::Up;
    } else if (direction == "down") {
        moveOperation = QTextCursor::Down;
    } else if (direction == "left") {
        if (type == "character") {
            moveOperation = QTextCursor::Left;
        } else if (type == "word") {
            moveOperation = QTextCursor::WordLeft;
        }
    } else if (direction == "right") {
        if (type == "character") {
            moveOperation = QTextCursor::Right;
        } else if (type == "word") {
            moveOperation = QTextCursor::WordRight;
        }
    }
    QTextCursor cursor = presentationWidget->textCursor();
    cursor.clearSelection();
    cursor.movePosition(moveOperation, QTextCursor::MoveAnchor);
    presentationWidget->setTextCursor(cursor);
    updatePresentationWidget();
}

void WriterPlugin::updateRootLevel() {
    if ((browserItemDelegate != NULL) && browserItemDelegate->hasLevelBelow()) {
        rootLevel = browserItemDelegate->getLevelBelow();
    } else {
        rootLevel = NULL;
    }
}

void WriterPlugin::updatePresentationWidget() {
    QTextCursor cursor = presentationWidget->textCursor();

#ifdef SCASE_PLUGIN_WRITER_PREDICTION_ENABLED
    if (predictedItemsAdded > 0) {
        if (rootLevel != NULL) {
            for (int i = 0; i < predictedItemsAdded; i++) {
                rootLevel->removeItemAtPos(0);
            }
        }
        predictedItemsAdded = 0;
    }

#endif

    updateRootLevel();

    int oldPosition = cursor.position();

#ifdef SCASE_PLUGIN_WRITER_PREDICTION_ENABLED
    cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
    if (cursor.selectedText().trimmed().size() > 0) {
        if( rxBasePredictorValidator.exactMatch(cursor.selectedText().trimmed()) ) {
            cursor.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);
            QString predictionBase = cursor.selectedText().trimmed();
#ifdef SCASE_PLUGIN_DEBUG_LEVEL_VERBOSE
            qDebug() << "WriterPlugin.predictionBase:" << predictionBase;
#endif
            if (predictionBase.size() > 0) {
                if (rootLevel != NULL) {
                    //TODO: get predictions, add top N predictions to tree
                    predictedItemsAdded = 3;
                    browserDelegate->addItemToLevel(rootLevel, 0, QString("%1_3").arg(predictionBase), getName(), QString("write,%1_3").arg(predictionBase), true);
                    browserDelegate->addItemToLevel(rootLevel, 0, QString("%1_2").arg(predictionBase), getName(), QString("write,%1_2").arg(predictionBase), true);
                    browserDelegate->addItemToLevel(rootLevel, 0, QString("%1_1").arg(predictionBase), getName(), QString("write,%1_1").arg(predictionBase), true);
                }
            }
        }
    }
#endif

    if (rootLevel != NULL) {
        browserDelegate->goToLevel(rootLevel);
    }

    cursor.setPosition(oldPosition);
    presentationWidget->setTextCursor(cursor);
}

//Q_EXPORT_PLUGIN2(WriterPlugin, WriterPlugin)
