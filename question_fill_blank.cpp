#include "question_fill_blank.h"
#include"ui_question_fill_blank.h"

QuestionFillBlank::QuestionFillBlank(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MemoryTestByFillRandomBlank)
{
    ui->setupUi(this);
    //安装事件过滤器
    this->installEventFilter(this);

    //初始化填充位置及单词长度
    fill_position=0;
    recent_word_length=0;

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
    clear_label();

}

QuestionFillBlank::~QuestionFillBlank()
{
    delete ui;
}

void QuestionFillBlank::set_word_label(QChar letter)
{
    //当传入字符为'#'时标志Enter事件进行检查结果；
    if(letter=='#')
    {
        //拼写正确退出窗口，错误则继续运行；
        if(check_answer())
        {
            QMessageBox::information(this, "结果", "拼写正确");
            //清空标签并退出，为下一次测试做准备
            clear_label();
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
        if(fill_position>0)
        {
            //当前拼接的单词删除末尾字符
            answer.chop(1);
            //更新当前填充下标
            fill_position--;
        }
        //以'_'标识清退位置
        labels[fill_position]->setText(QString(letter));
    }
    //当传入字符为字母时进行填充
    else if(letter.isLetter())
    {
        //当填充位置为首字母时转大写；
        if(fill_position==0)
        {
            labels[fill_position++]->setText(QString(letter));
        }
        else
        {
            labels[fill_position++]->setText(QString(letter).toLower());
        }
        //拼接当前单词；
        answer.append(letter.toLower());

        //当填充下标为目标单词长度时进行检查；
        if(fill_position==recent_word_length)
        {
            if(check_answer())
            {
                //清空标签并退出，为下一次测试做准备
                QMessageBox::information(this, "结果", "拼写正确");
                clear_label();
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
void QuestionFillBlank::set_dest_word()
{
    recent_word_length=word.wordText.length();
    for(quint32 i=0;i<recent_word_length;i++)
    {
        labels[i]->show();
        labels[i]->setAlignment(Qt::AlignCenter);
    }
}

bool QuestionFillBlank::check_answer()
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

void QuestionFillBlank::clear_label()
{
    //清空标签；
    for(int i=0;i<MAX_WORD_LENGTH;i++)
    {
        labels[i]->setText("_");
        labels[i]->hide();
    }
    //清空当前单词；
    answer.clear();
    //重设当前单词位序；
    fill_position=0;
    qDebug()<<"Clearing the blank_filling question pasge!";
}

void QuestionFillBlank::keyPressEvent(QKeyEvent *event)
{
    //获取key事件
    quint32 key = event->key();
    //区分字母、Enter、Backspace按键响应；
    if(key==Qt::Key_Backspace)
    {
        set_word_label('_');
    }
    else if(QChar(key).isLetter())
    {
        set_word_label(QChar(key));
    }
    else if(key==Qt::Key_Enter)
    {
        set_word_label('#');
    }
}

void QuestionFillBlank::closeEvent(QCloseEvent *event)
{
    emit windowClosed();
    // 调用基类的closeEvent以执行默认的关闭逻辑
    QWidget::closeEvent(event);
}
