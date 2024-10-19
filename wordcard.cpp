#include <QDebug>

#include "wordcard.h"
#include "ui_wordcard.h"

WordCard::WordCard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WordCard)
{
    ui->setupUi(this);
    wordCardPanelManage();
    //设置单词卡当前展示位序
    currentIndex=0;
    //初始化单词数据；
    initDataList(wordDatabase.allWords);
    //显示单词卡默认单词
    showWordCard(wordList[0]);
}

WordCard::~WordCard()
{
    delete ui;
}

void WordCard::showPrev()
{
    if(currentIndex>0)
    {
        currentIndex--;
        showWordCard(wordList[currentIndex]);
    }
    else
    {
        qDebug()<<"Recent one has been the first word in list!";
        //TODO:Add UI tip information;
    }
}

void WordCard::showNext()
{
    if(currentIndex<maxIndex)
    {
        currentIndex++;
        showWordCard(wordList[currentIndex]);
    }
    else
    {
        qDebug()<<"Recent one has been the final word in list";
        //TODO:Add UI tip information;
    }
}

void WordCard::showWordCard(WordInfo word)
{
    //显示对应单词的基本内容；
    if(currentIndex>wordQuantity-1)
    {
        qDebug()<<"Invailed index!";
        return;
    }
    ui->WordText->setText(word.wordText);
    ui->PartOfSpeech->setText(word.partOfSpeech);
    ui->BritishPhoneticSymbol->setText(word.britishPhoneticSymbol);
    ui->AmericanPhoneticSymbol->setText(word.americanPhoneticSymbol);
    ui->WordExplanation->setText(word.wordExplanation);
    if(wordDatabase.isExistInCollection(wordDatabase.collectionList[STAR_LIST],word))
    {
        ui->StarWord->setCheckState(Qt::Checked);
    }
    else
    {
        ui->StarWord->setCheckState(Qt::Unchecked);
    }
    QString pageNumber=QString("%1/%2").arg(word.wordID).arg(wordList.size());
    ui->pageNumberLabel->setText(pageNumber);
}

bool WordCard::addWordToCollecting(WordCollectionInfo collection)
{
    return wordDatabase.addWordToCollection(collection,wordList[currentIndex]);
}

bool WordCard::removeWordFromCollecting(WordCollectionInfo collection)
{
    return wordDatabase.removeWordFromCollection(collection,wordList[currentIndex]);
}

void WordCard::wordCardPanelManage()
{
    QObject::connect(ui->ButtonPrev,&QPushButton::clicked,this,&WordCard::showPrev);
    QObject::connect(ui->ButtonNext,&QPushButton::clicked,this,&WordCard::showNext);
    QObject::connect(ui->StarWord,&QCheckBox::stateChanged,this,&WordCard::on_StarWord_stateChanged);
}

void WordCard::initDataList(QList<WordInfo> temp)
{
    wordList=temp;
    wordQuantity=temp.size();
    maxIndex=wordQuantity-1;
}

void WordCard::testDataInit()
{

    initDataList(wordDatabase.allWords);
    qDebug()<<"wordQuantity:"<<wordQuantity;
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
        addWordToCollecting(wordDatabase.collectionList[STAR_LIST]);
        return;
    }
    if (arg1 == Qt::Unchecked)
    {
        removeWordFromCollecting(wordDatabase.collectionList[STAR_LIST]);
        return;
    }
}


void WordCard::on_MemoryCheck_clicked()
{
    //this->hide();
    MemoryTestByFillRandomBlank test1;
    test1.word=wordList[currentIndex];
    test1.setDestWord();
    test1.exec();
}

