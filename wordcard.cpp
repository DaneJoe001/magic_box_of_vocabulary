#include <QDebug>

#include "WordCard.h"
#include "ui_WordCard.h"

WordCard::WordCard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WordCard)
{
    ui->setupUi(this);
    WordCard_panel_manage();
    //设置单词卡当前展示位序
    current_index=0;
    //初始化单词数据；
    init_data_list(word_database.all_words);
    //显示单词卡默认单词
    show_WordCard(word_list[0]);
}

WordCard::~WordCard()
{
    delete ui;
}

void WordCard::tableWordCard(WordInfo word ,quint32 srcIndex)
{
    //显示对应单词的基本内容；
    if(current_index>max_index)
    {
        qDebug()<<"Invailed index!";
        return;
    }
    ui->WordText->setText(word.wordText);
    ui->BritishPhoneticSymbol->setText(word.britishPhoneticSymbol);
    ui->AmericanPhoneticSymbol->setText(word.americanPhoneticSymbol);
    ui->WordExplanation->setText(word.wordExplanation);
    if(word_database.is_exist_in_collection(word_database.collection_list[STAR_LIST],word))
    {
        ui->StarWord->setCheckState(Qt::Checked);
    }
    else
    {
        ui->StarWord->setCheckState(Qt::Unchecked);
    }
    //隐藏不需要的按钮
    //ui->ButtonNext->hide();
    //ui->ButtonPrev->hide();
    test1.word=word;
    current_index=srcIndex;
    //页脚页码展示
    QString pageNumber=QString("%1/%2").arg(current_index+1).arg(word_list.size());
    ui->pageNumberLabel->setText(pageNumber);
}

//TODO：检查
void WordCard::show_prev()
{
    //展示上一单词；
    //当展示首个单词时点击跳到末尾单词；
    current_index--;
    if(current_index<0)
    {
        current_index=max_index;
    }
    qDebug()<<current_index;
    show_WordCard(word_list[current_index]);
}

void WordCard::show_next()
{
    //展示下一单词；
    //当展示末尾单词时点击跳到首个单词；
    current_index++;
    if(current_index>max_index)
    {
        current_index=0;
    }
    qDebug()<<current_index;
    show_WordCard(word_list[current_index]);
}

void WordCard::show_WordCard(WordInfo word)
{
    //显示对应单词的基本内容；
    if(current_index>max_index)
    {
        qDebug()<<"Invailed index!";
        return;
    }
    ui->WordText->setText(word.wordText);
    ui->BritishPhoneticSymbol->setText(word.britishPhoneticSymbol);
    ui->AmericanPhoneticSymbol->setText(word.americanPhoneticSymbol);
    ui->WordExplanation->setText(word.wordExplanation);
    if(word_database.is_exist_in_collection(word_database.collection_list[STAR_LIST],word))
    {
        ui->StarWord->setCheckState(Qt::Checked);
    }
    else
    {
        ui->StarWord->setCheckState(Qt::Unchecked);
    }
    //页脚页码展示
    QString pageNumber=QString("%1/%2").arg(current_index+1).arg(word_list.size());
    ui->pageNumberLabel->setText(pageNumber);
}

bool WordCard::add_word_to_collection(WordCollectionInfo collection)
{
    return word_database.add_word_to_collection(collection,word_list[current_index]);
}

bool WordCard::remove_word_from_collection(WordCollectionInfo collection)
{
    return word_database.remove_word_from_collection(collection,word_list[current_index]);
}

void WordCard::WordCard_panel_manage()
{
    QObject::connect(ui->ButtonPrev,&QPushButton::clicked,this,&WordCard::show_prev);
    QObject::connect(ui->ButtonNext,&QPushButton::clicked,this,&WordCard::show_next);
    QObject::connect(ui->StarWord,&QCheckBox::stateChanged,this,&WordCard::on_StarWord_stateChanged);
}

void WordCard::init_data_list(QList<WordInfo> temp)
{
    word_list=temp;
    word_quantity=temp.size();
    max_index=word_quantity-1;
}

void WordCard::init_by_collection(WordCollectionInfo collectionInfo)
{
    //清空当前单词
    word_list.clear();
    //qDebug()<<collectionInfo.word_quantity;
    //qDebug()<<"collectionInfo.wordIdList"<<collectionInfo.wordIdList.size();
    for(quint32 i=0;i<collectionInfo.word_quantity;i++)
    {
        word_list.append(word_database.all_words[(collectionInfo.wordIdList[i]-1)]);
        //qDebug()<<"Index:"<<index;
        //show_word_list.append(word_database.all_words[index]);
    }
    word_quantity=collectionInfo.word_quantity;
    max_index=word_quantity-1;
    //qDebug()<<"当前单词数量："<<word_quantity;
}

void WordCard::testDataInit()
{

    init_data_list(word_database.all_words);
    qDebug()<<"word_quantity:"<<word_quantity;
}


void WordCard::on_ButtonOK_clicked()
{
    qDebug()<<"Push ButtonOK!";
    this->close();
}


void WordCard::on_StarWord_stateChanged(int arg1)
{
    if(arg1==Qt::Checked)
    {
        add_word_to_collection(word_database.collection_list[STAR_LIST]);
        return;
    }
    if (arg1 == Qt::Unchecked)
    {
        remove_word_from_collection(word_database.collection_list[STAR_LIST]);
        return;
    }
}


void WordCard::on_MemoryCheck_clicked()
{
    this->hide();
    test1.word=word_list[current_index];
    test1.set_dest_word();
    test1.show();
    QObject::connect(&test1, &MemoryTestByFillRandomBlank::windowClosed, this, &WordCard::show);
}

