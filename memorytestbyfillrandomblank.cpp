#include "memorytestbyfillrandomblank.h"
#include "ui_memorytestbyfillrandomblank.h"

MemoryTestByFillRandomBlank::MemoryTestByFillRandomBlank(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MemoryTestByFillRandomBlank)
{
    ui->setupUi(this);
    //安装事件过滤器
    this->installEventFilter(this);

    //初始化填充位置及单词长度
    fillPosition=0;
    recentWordLength=0;

    //查找当前界面符合条件的所有标签兵填充进容器
    for (int i = 1; i <= 20; ++i)
    {
        QLabel *label = findChild<QLabel*>(QString("Letter%1").arg(i));
        //判断是否为标签，是则添加，不是则提示未找到；
        if (label)
        {
            labels.append(label);
        }
        else
        {
            qWarning() << "Label Letter" << i << "not found!";
        }
        //设置当前标签字体
        QFont font;
        font.setFamily("荆南麦圆体");
        font.setPointSize(48);
        font.setBold(true);
        labels[i-1]->setFixedSize(60,80);
        labels[i-1]->setFont(font);

        //设置当前标签字体颜色
        QPalette palette = labels[i-1]->palette();
        palette.setColor(QPalette::WindowText, Qt::white); 
        labels[i-1]->setPalette(palette);
        labels[i-1]->hide();
    }
    //初始化清空所有字母标签
    clearLabel();

}

MemoryTestByFillRandomBlank::~MemoryTestByFillRandomBlank()
{
    delete ui;
}

void MemoryTestByFillRandomBlank::setWordLabel(QChar letter)
{
    //当传入字符为'#'时标志Enter事件进行检查结果；
    if(letter=='#')
    {
        //拼写正确退出窗口，错误则继续运行；
        if(checkAnswer())
        {
            QMessageBox::information(this, "结果", "拼写正确");
            //清空标签并退出，为下一次测试做准备
            clearLabel();
            this->close();
        }
        else
        {
            QMessageBox::information(this, "结果", "拼写错误");
            return;
        }
    }
    //当传入字符为'#'时标志Backspace事件，回退一格；
    if(letter==QChar('_'))
    {
        if(fillPosition>0)
        {
            //当前拼接的单词删除末尾字符
            answer.chop(1);
            //更新当前填充下标
            fillPosition--;
        }
        //以'_'标识清退位置
        labels[fillPosition]->setText(QString(letter));
    }
    //当传入字符为字母时进行填充
    else if(letter.isLetter())
    {
        //当填充位置为首字母时转大写；
        if(fillPosition==0)
        {
            labels[fillPosition++]->setText(QString(letter));
        }
        else
        {
            labels[fillPosition++]->setText(QString(letter).toLower());
        }
        //拼接当前单词；
        answer.append(letter.toLower());
        //在控制台展示信息；
        qDebug()<<QString("answer: %1,fillPosition: %2").arg(answer).arg(fillPosition);
        //当填充下标为目标单词长度时进行检查；
        if(fillPosition==recentWordLength)
        {
            if(checkAnswer())
            {
                //清空标签并退出，为下一次测试做准备
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
}

//从单词信息获取目标单词
void MemoryTestByFillRandomBlank::setDestWord()
{
    recentWordLength=word.wordText.length();
    qDebug()<<QString("recentWordLength:%1").arg(recentWordLength);
    for(quint32 i=0;i<recentWordLength;i++)
    {
        labels[i]->show();
        //labels[i]->setAlignment(Qt::AlignCenter);
    }
}

bool MemoryTestByFillRandomBlank::checkAnswer()
{
    //判断结果是否正确；
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
    //清空标签；
    for(int i=0;i<MAX_WORD_LENGTH;i++)
    {
        labels[i]->setText("_");
    }
    //清空当前单词；
    answer.clear();
    //重设当前单词位序；
    fillPosition=0;
    qDebug()<<"Tabel clear!";
}

void MemoryTestByFillRandomBlank::keyPressEvent(QKeyEvent *event)
{
    //获取key事件
    quint32 key = event->key();
    //区分字母、Enter、Backspace按键响应；
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
