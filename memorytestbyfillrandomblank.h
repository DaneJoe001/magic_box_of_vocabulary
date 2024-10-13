#ifndef MEMORYTESTBYFILLRANDOMBLANK_H
#define MEMORYTESTBYFILLRANDOMBLANK_H

#include <QDialog>
#include <QEvent>
#include <QKeyEvent>
#include <QChar>
#include <QLabel>
#include <QList>
#include <QString>
#include <QMessageBox>

#include "vocabularydatabase.h"

#define MAX_WORD_LENGTH 20

namespace Ui {
class MemoryTestByFillRandomBlank;
}

class MemoryTestByFillRandomBlank : public QDialog
{
    Q_OBJECT
public:
    WordInfo word;

public:
    explicit MemoryTestByFillRandomBlank(QWidget *parent = nullptr);
    ~MemoryTestByFillRandomBlank();

    void setWordLabel(QChar letter);

    void setDestWord();
    bool checkAnswer();
    void clearLabel();

    // 重写 eventFilter() 方法
    bool eventFilter(QObject *obj, QEvent *event) override
    {
        // 检查事件是否为目标对象（本例中为 MyWidget 实例）的键盘事件
        if (obj == this && event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            quint32 key = keyEvent->key();

            if (key == Qt::Key_Enter || key == Qt::Key_Return) {
                qDebug() << "Enter key pressed!";
                // 阻止事件进一步传播（可选）
                return true;
            }
        }
        // 如果不处理事件，则返回 false 以允许事件继续传播
        return QWidget::eventFilter(obj, event);
    };



    //TODO:键盘事件监听，按下按键，设置label标签，backspace回退
    //TODO:随机设置空白区域
    //TODO:全拼
    //TODO:正确率计算和保存，单独写一个类

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::MemoryTestByFillRandomBlank *ui;

    quint32 fillPosition;
    quint32 recentWordLength;
    QList<QLabel*> labels;
    QString answer;


};

#endif // MEMORYTESTBYFILLRANDOMBLANK_H
