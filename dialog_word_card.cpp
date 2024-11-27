#include <QDebug>

#include "dialog_word_card.h"
#include "ui_dialog_word_card.h"
#include "resource_manager.h"

DialogWordCard::DialogWordCard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWordCard)
{
    ui->setupUi(this);
    ResourceManager& resource_manager=ResourceManager::get_resource_manager();
    //初始化单词卡数据来源
    all_words=resource_manager.get_all_words();
    WordCard_panel_manage();
    //设置单词卡当前展示位序
    current_index=0;
    //初始化单词数据；
    init_data_list(resource_manager.get_all_words());
    //显示单词卡默认单词
    show_WordCard(word_list[0]);
}

DialogWordCard::~DialogWordCard()
{
    delete ui;
}

void DialogWordCard::tableWordCard(WordInfo word ,quint32 srcIndex)
{
    //显示对应单词的基本内容；
    if(current_index>max_index)
    {
        qDebug()<<"Error:Invaild index!";
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
void DialogWordCard::show_prev()
{
    //展示上一单词；
    //当展示首个单词时点击跳到末尾单词；
    current_index--;
    if(current_index<0)
    {
        current_index=max_index;
    }
    show_WordCard(word_list[current_index]);
}

void DialogWordCard::show_next()
{
    //展示下一单词；
    //当展示末尾单词时点击跳到首个单词；
    current_index++;
    if(current_index>max_index)
    {
        current_index=0;
    }
    show_WordCard(word_list[current_index]);
}

void DialogWordCard::show_WordCard(WordInfo word)
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

bool DialogWordCard::add_word_to_collection(WordCollectionInfo collection)
{
    return word_database.add_word_to_collection(collection,word_list[current_index]);
}

bool DialogWordCard::remove_word_from_collection(WordCollectionInfo collection)
{
    return word_database.remove_word_from_collection(collection,word_list[current_index]);
}

void DialogWordCard::WordCard_panel_manage()
{
    QObject::connect(ui->ButtonPrev,&QPushButton::clicked,this,&DialogWordCard::show_prev);
    QObject::connect(ui->ButtonNext,&QPushButton::clicked,this,&DialogWordCard::show_next);
    QObject::connect(ui->StarWord,&QCheckBox::stateChanged,this,&DialogWordCard::on_StarWord_stateChanged);
}

void DialogWordCard::init_data_list(QList<WordInfo> temp)
{
    word_list=temp;
    word_quantity=temp.size();
    max_index=word_quantity-1;
}

void DialogWordCard::init_by_collection(WordCollectionInfo collectionInfo)
{
    //清空当前单词
    word_list.clear();
    for(quint32 i=0;i<collectionInfo.word_quantity;i++)
    {
        word_list.append(all_words[(collectionInfo.wordIdList[i]-1)]);
    }
    word_quantity=collectionInfo.word_quantity;
    max_index=word_quantity-1;
    qDebug()<<"当前单词数量："<<word_quantity;
}

void DialogWordCard::set_current_index(qint32 word_index)
{
    current_index=word_index;
}


void DialogWordCard::on_ButtonOK_clicked()
{
    qDebug()<<"Push ButtonOK!";
    this->close();
}


void DialogWordCard::on_StarWord_stateChanged(int arg1)
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


void DialogWordCard::on_MemoryCheck_clicked()
{
    this->hide();
    test1.word=word_list[current_index];
    test1.set_dest_word();
    test1.show();
    QObject::connect(&test1, &QuestionFillBlank::windowClosed, this, &DialogWordCard::show);
}

