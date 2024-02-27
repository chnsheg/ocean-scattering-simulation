#ifndef TEXTEDITMANAGER_H
#define TEXTEDITMANAGER_H

#include <QObject>
#include <QTextEdit>
#include "base/singleton.h"

class TextEditManager : public QObject
{
    Q_OBJECT
    SINGLETON(TextEditManager)
public:
    void appendText(const QString &text, bool newLine = true);
    void appendText(const QString &text, const QColor &color, bool newLine = true);
    void appendText(const QString &text, const QTextCharFormat &format, bool newLine = true);
    void clearText();
    void initTextEditStyle();
    void setTextEdit(QTextEdit *textEdit);

private:
    QTextEdit *textEdit;
    TextEditManager(QTextEdit *_textEdit, QObject *parent = nullptr);
signals:
};

#endif // TEXTEDITMANAGER_H
