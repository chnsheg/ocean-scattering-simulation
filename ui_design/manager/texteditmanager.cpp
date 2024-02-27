#include "texteditmanager.h"

TextEditManager::TextEditManager(QTextEdit *textEdit, QObject *parent)
    : QObject(parent)
{
    this->textEdit = textEdit;
    initTextEditStyle();
}

TextEditManager::~TextEditManager()
{
    delete textEdit;
}

TextEditManager::TextEditManager() {}

void TextEditManager::initTextEditStyle()
{
    // 初始化文本框样式
    // QString styleSheet = "QTextEdit {"
    //                      "    background-color: #f0f0f0;"
    //                      "    color: #333;"
    //                      "    font-family: 'Arial', sans-serif;"
    //                      "    font-size: 14px;"
    //                      "}"
    //                      "QTextEdit::ScrollBar {"
    //                      "    width: 10px;"
    //                      "}"
    //                      "QTextEdit::ScrollBar:vertical {"
    //                      "    border: 2px solid #ccc;"
    //                      "    background: #f0f0f0;"
    //                      "}"
    //                      "QTextEdit::ScrollBar::handle:vertical {"
    //                      "    background: #999;"
    //                      "    min-height: 20px;"
    //                      "}"
    //                      "QTextEdit::ScrollBar::add-line:vertical,"
    //                      "QTextEdit::ScrollBar::sub-line:vertical {"
    //                      "    height: 0px;"
    //                      "}"
    //                      "QTextEdit::ScrollBar:horizontal {"
    //                      "    border: 2px solid #ccc;"
    //                      "    background: #f0f0f0;"
    //                      "}"
    //                      "QTextEdit::ScrollBar::handle:horizontal {"
    //                      "    background: #999;"
    //                      "    min-width: 20px;"
    //                      "}"
    //                      "QTextEdit::ScrollBar::add-line:horizontal,"
    //                      "QTextEdit::ScrollBar::sub-line:horizontal {"
    //                      "    width: 0px;"
    //                      "}";

    // // 设置画布背景色
    // QLinearGradient plotGradient;
    // plotGradient.setStart(0, 0);
    // plotGradient.setFinalStop(0, 350);
    // plotGradient.setColorAt(0, QColor(80, 80, 80));
    // plotGradient.setColorAt(1, QColor(50, 50, 50));

    // QPalette palette = textEdit->palette();
    // palette.setBrush(QPalette::Base, plotGradient);
    // textEdit->setPalette(palette);
    // 设置背景、前景色以及边框
    // 设置背景、前景色以及边框
    QPalette palette;
    palette.setColor(QPalette::Base, QColor(240, 240, 240)); // 背景色
    palette.setColor(QPalette::Text, QColor(50, 50, 50));    // 前景色
    // palette.setColor(QPalette::Border, QColor(200, 200, 200)); // 边框色

    // 圆角和边框
    textEdit->setStyleSheet("QTextEdit {"
                            "    background-color: #F0F0F0;" // 使用QPalette中的颜色
                            "    color: #323232;"
                            "    border: 1px solid #C8C8C8;"
                            "    border-radius: 5px;"
                            "    padding: 8px;" // 增加内边距
                            "}"
                            "QScrollBar:vertical {"
                            "    border: 1px solid #C8C8C8;" // 设置垂直滚动条的边框
                            "    background: #F0F0F0;"       // 背景色
                            "    width: 10px;"               // 设置宽度
                            "    margin: 0px;"
                            "}"
                            "QScrollBar::handle:vertical {"
                            "    background: #C8C8C8;" // 滚动条的手柄颜色
                            "    min-height: 20px;"    // 最小高度
                            "    border-radius: 5px;"  // 圆角
                            "}"
                            "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
                            "    background: none;"
                            "}"
                            "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
                            "    background: none;"
                            "}");

    // 应用样式
    textEdit->setPalette(palette);

    // textEdit->setReadOnly(true);                                    // 设置文本框为只读
    textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);    // 设置垂直滚动条为一直显示
    textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 设置水平滚动条为不显示
    // 设置文本框自动换行
    textEdit->setWordWrapMode(QTextOption::WordWrap);
    textEdit->setUndoRedoEnabled(false);               // 禁用撤销和重做
    textEdit->setAcceptRichText(false);                // 禁用富文本
    textEdit->setTabChangesFocus(false);               // 禁用Tab键切换焦点
    textEdit->setContextMenuPolicy(Qt::NoContextMenu); // 禁用右键菜单
    // textEdit->setFocusPolicy(Qt::NoFocus);             // 禁用焦点, 使得文本框不可编辑
    textEdit->setFrameStyle(QFrame::NoFrame); // 禁用边框

    // 当文本框内容改变时，自动滚动到底部
    connect(textEdit, &QTextEdit::textChanged, [=]()
            { textEdit->moveCursor(QTextCursor::End); });
}

void TextEditManager::setTextEdit(QTextEdit *textEdit)
{
    this->textEdit = textEdit;
    initTextEditStyle();
}

void TextEditManager::clearText()
{
    textEdit->clear();
}

void TextEditManager::appendText(const QString &text, bool newLine)
{
    textEdit->append(text);
    if (newLine)
    {
        textEdit->append("\n");
    }
}

void TextEditManager::appendText(const QString &text, const QColor &color, bool newLine)
{
    textEdit->setTextColor(color);
    textEdit->append(text);
    if (newLine)
    {
        textEdit->append("\n");
    }
}
