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
    //定义当前测试单词；
    WordInfo word;

public:
    explicit MemoryTestByFillRandomBlank(QWidget *parent = nullptr);
    ~MemoryTestByFillRandomBlank();

    //设置当前序号标签字母；
    void setWordLabel(QChar letter);

    //获取设置目标单词长度
    void setDestWord();

    //检查填写情况；
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

    //TODO:随机设置空白区域
    //TODO:正确率计算和保存，单独写一个类

protected:
    //重写事件监听，键盘字母输入；
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::MemoryTestByFillRandomBlank *ui;

    //当前填充位置；
    quint32 fillPosition;

    //当前单词长度；
    quint32 recentWordLength;

    //label标签组
    QList<QLabel*> labels;

    //当前拼接的单词
    QString answer;

};

#endif // MEMORYTESTBYFILLRANDOMBLANK_H
