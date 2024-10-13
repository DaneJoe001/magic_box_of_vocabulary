#include "memorytestbyfillrandomblank.h"
#include "ui_memorytestbyfillrandomblank.h"

MemoryTestByFillRandomBlank::MemoryTestByFillRandomBlank(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MemoryTestByFillRandomBlank)
{
    ui->setupUi(this);
    this->installEventFilter(this);
    //安装事件过滤器
    fillPosition=0;
    recentWordLength=0;
    for (int i = 1; i <= 20; ++i)
    {
        QLabel *label = findChild<QLabel*>(QString("Letter%1").arg(i));
        if (label)
        {
            labels.append(label);
        }
        else
        {
            qWarning() << "Label Letter" << i << "not found!";
        }
        QFont font;
        font.setFamily("荆南麦圆体");
        font.setPointSize(48);
        font.setBold(true);
        labels[i-1]->setFixedSize(60,80);
        labels[i-1]->setFont(font);

        QPalette palette = labels[i-1]->palette();
        palette.setColor(QPalette::WindowText, Qt::white); // 将文本颜色设置为红色
        label->setPalette(palette);
        labels[i-1]->hide();
    }
    clearLabel();

}

MemoryTestByFillRandomBlank::~MemoryTestByFillRandomBlank()
{
    delete ui;
}

void MemoryTestByFillRandomBlank::setWordLabel(QChar letter)
{
    if(letter=='#')
    {
        if(checkAnswer())
        {
            QMessageBox::information(this, "结果", "拼写正确");
            clearLabel();
            this->close();
        }
        else
        {
            QMessageBox::information(this, "结果", "拼写错误");
            return;
        }
    }
    if(letter==QChar('_'))
    {
        if(fillPosition>0)
        {
            answer.chop(1);
            fillPosition--;
        }
        labels[fillPosition]->setText(QString(letter));
    }
    else if(letter.isLetter())
    {
        if(fillPosition==0)
        {
            labels[fillPosition++]->setText(QString(letter));
        }
        else
        {
            labels[fillPosition++]->setText(QString(letter).toLower());
        }
        answer.append(letter.toLower());
        if(fillPosition==recentWordLength)
        {
            if(checkAnswer())
            {
                QMessageBox::information(this, "结果", "拼写正确");
                clearLabel();
                this->close();
            }
            else
            {
                QMessageBox::information(this, "结果", "拼写错误");
                return;
            }
        }
    }
    qDebug()<<QString("answer: %1,fillPosition: %2").arg(answer).arg(fillPosition);
}

void MemoryTestByFillRandomBlank::setDestWord()
{
    recentWordLength=word.wordText.length();
    qDebug()<<QString("recentWordLength:%1").arg(recentWordLength);
    for(int i=0;i<recentWordLength;i++)
    {
        labels[i]->show();
        labels[i]->setAlignment(Qt::AlignCenter);
    }
}

bool MemoryTestByFillRandomBlank::checkAnswer()
{
    if(answer==word.wordText)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void MemoryTestByFillRandomBlank::clearLabel()
{
    for(int i=0;i<MAX_WORD_LENGTH;i++)
    {
        labels[i]->setText("_");
    }
    answer.clear();
    fillPosition=0;
    qDebug()<<"Tabel clear!";
}

void MemoryTestByFillRandomBlank::keyPressEvent(QKeyEvent *event)
{
    quint32 key = event->key();
    if(key==Qt::Key_Backspace)
    {
        setWordLabel('_');
    }
    else if(QChar(key).isLetter())
    {
        setWordLabel(QChar(key));
    }
    else if(key==Qt::Key_Enter)
    {
        qDebug()<<"Push Enter!";
        setWordLabel('#');
    }
}
