#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //显示登录窗口；
    loginPanel.show();

    //接收登录信号，显示主窗口；
    connect(&loginPanel,&UserLogin::loginSignal,this,[this]()
    {
        //显示主界面后关闭登录窗口；
        this->show();
        loginPanel.close();    
        //调用界面管理函数，相应切换等按钮事件；
        wordCardPanelManage();
        //设置进入时显示的界面；
        ui->functionPanel->setCurrentIndex(2);
        //初始化单词展示列表的序号；
        currentIndex=0;
        //将展示列表初始化为所有单词；
        initDataList(wordDatabase.allWords);
        //用单词1初始化单词展示信息；
        showWordCard(wordList[0]);
        //设置个人信息界面账号；
        ui->currentUserAccount->setText(loginPanel.currentUser.userAccount);
        //设置个人信息界面最近登录时间；
        ui->currentUserLastLoginTime
            ->setText(QString("最近登陆时间：%1").
            arg(loginPanel.currentUser.lastLoginTime.toString(Qt::ISODate)));
        //设置个人信息界面昵称；
        ui->currentUserNickName->setText(loginPanel.currentUser.userNickName);
        //初始化要展示的单词合集信息列表；
        for(WordCollectionInfo temp:wordDatabase.collectionList)
        {
            QListWidgetItem *item=new QListWidgetItem(temp.collectionName);
            collectionItem.append(item);
        }
        //显示单词合集信息列表；
        showCollectionItemList();
        //初始化单元格
        showWordList=wordDatabase.allWords;
        setWordTable();
        //连接表格和槽函数
        QObject::connect(ui->tableWidget,&QTableWidget::cellClicked,this,&MainWindow::onCellClicked);
        //连接合集表项和槽函数
        QObject::connect(ui->wordListWidget,&QListWidget::itemClicked,this,&MainWindow::listItemClicked);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initDataList(QList<WordInfo> temp)
{
    //设置成员变量
    wordList=temp;
    wordQuantity=temp.size();
    maxIndex=wordQuantity-1;
}

void MainWindow::initByCollection(WordCollectionInfo collectionInfo)
{
    qDebug()<<"Uploading table!";
    showWordList.clear();
    qDebug()<<collectionInfo.wordQuantity;
    wordDatabase.initCollectionList();
    qDebug()<<"collectionInfo.wordIdList"<<collectionInfo.wordIdList.size();
    for(quint32 i=0;i<collectionInfo.wordQuantity;i++)
    {
        showWordList.append(wordDatabase.allWords[(collectionInfo.wordIdList[i]-1)]);
        //qDebug()<<"Index:"<<index;
        //showWordList.append(wordDatabase.allWords[index]);
    }
    wordQuantity=collectionInfo.wordQuantity;
}

void MainWindow::showPrev()
{
    //在范围内递增当前序号
    if(currentIndex>0)
    {
        currentIndex--;
        showWordCard(wordList[currentIndex]);
    }
    else
    {
        qDebug()<<"Recent one has been the first word in list!";
        QMessageBox::information(this,"提示","已经是第一个单词！");
    }
}

void MainWindow::showNext()
{
    //在范围内递减当前序号
    if(currentIndex<maxIndex)
    {
        currentIndex++;
        showWordCard(wordList[currentIndex]);
    }
    else
    {
        qDebug()<<"Recent one has been the final word in list";
        QMessageBox::information(this,"提示","已经是最后一个单词！");
    }
}

void MainWindow::wordCardPanelManage()
{
    //连接单词查询界面按钮信号对应槽函数
    QObject::connect(ui->ButtonPrev,&QPushButton::clicked,this,&MainWindow::showPrev);
    QObject::connect(ui->ButtonNext,&QPushButton::clicked,this,&MainWindow::showNext);
    QObject::connect(ui->StarWord,&QCheckBox::stateChanged,this,&MainWindow::on_StarWord_stateChanged);
}

void MainWindow::showWordCard(WordInfo word)
{
    //判断下标是否有效
    if(currentIndex>maxIndex)
    {
        qDebug()<<"Invailed index!";
        return;
    }
    //设置界面元素；
    ui->WordText->setText(word.wordText);
    ui->PartOfSpeech->setText(word.partOfSpeech);
    ui->BritishPhoneticSymbol->setText(word.britishPhoneticSymbol);
    ui->AmericanPhoneticSymbol->setText(word.americanPhoneticSymbol);
    ui->WordExplanation->setText(word.wordExplanation);

    //设置收藏复选框状态；
    if(wordDatabase.isExistInCollection(wordDatabase.collectionList[STAR_LIST],word))
    {
        ui->StarWord->setCheckState(Qt::Checked);
    }
    else
    {
        ui->StarWord->setCheckState(Qt::Unchecked);
    }
    //设置页脚页码；
    QString pageNumber=QString("%1/%2").arg(word.wordID).arg(wordList.size());
    ui->pageNumberLabel->setText(pageNumber);
}

//在词库管理界面加载词库条目；
void MainWindow::showCollectionItemList()
{
    for(QListWidgetItem* temp:collectionItem)
    {
        ui->wordListWidget->addItem(temp);
        qDebug()<<"加载词库条目";
    }
}

bool MainWindow::addWordToCollecting(WordCollectionInfo collection)
{
    //调用单词数据库对象添加单词至指定列表；
    return wordDatabase.addWordToCollection(collection,wordList[currentIndex]);
}

bool MainWindow::removeWordFromCollecting(WordCollectionInfo collection)
{
    //调用单词数据库对象移除指定列表单词；
    return wordDatabase.removeWordFromCollection(collection,wordList[currentIndex]);
}

void MainWindow::setWordTable()
{
    ui->tableWidget->clear();
    //设置单词信息表表头
    QStringList headers;
    headers<<"单词"<<"词性"<<"英式音标"<<"美式音标"<<"单词释义"<<"wordID";
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setRowCount(showWordList.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    //填充表格内容；
    for(int row=0;row<showWordList.size();row++)
    {
        const WordInfo &word=showWordList[row];
        ui->tableWidget->setItem(row,0,new QTableWidgetItem(word.wordText));
        ui->tableWidget->setItem(row,1,new QTableWidgetItem(word.partOfSpeech));
        ui->tableWidget->setItem(row,2,new QTableWidgetItem(word.britishPhoneticSymbol));
        ui->tableWidget->setItem(row,3,new QTableWidgetItem(word.americanPhoneticSymbol));
        ui->tableWidget->setItem(row,4,new QTableWidgetItem(word.wordExplanation));
        ui->tableWidget->setItem(row,5,new QTableWidgetItem(QString::number(word.wordID)));
    }
    ui->tableWidget->setColumnHidden(5,true);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->tableWidget->resizeColumnsToContents();
    ui->WordQuantity->setText(QString("单词数量：%1").arg(showWordList.size()));
}

void MainWindow::on_pushButton_clicked()
{
    //优雅的结束程序doge
    QApplication::exit(0);
}


void MainWindow::on_wordSearch_clicked()
{
    //切换界面；
    ui->functionPanel->setCurrentIndex(2);
}


void MainWindow::on_wordManage_clicked()
{
    //切换界面；
    ui->functionPanel->setCurrentIndex(1);
}


void MainWindow::on_personalPanel_clicked()
{
    //切换界面；
    ui->functionPanel->setCurrentIndex(0);
}


void MainWindow::on_SearchButton_clicked()
{
    //获取搜索框输入内容
    QString searchText=ui->WordLineEdit->text();

    //调用单词数据库对象函数查询并获取单词信息；
    WordInfo word=wordDatabase.searchWord(searchText);
    if(word.wordID==0)
    {
        qDebug()<<"未找到！";
        QMessageBox::information(this,"结果","未找到");
        return;
    }
    showWordCard(word);
}


void MainWindow::on_MemoryCheck_clicked()
{
    //this->hide();
    //进行单词拼写测试；
    MemoryTestByFillRandomBlank test1;
    test1.word=wordList[currentIndex];
    qDebug()<<"Test word:"<<wordList[currentIndex].wordText;
    test1.setDestWord();
    test1.exec();
    //QObject::connect(this->show(),&)
}


void MainWindow::on_StarWord_stateChanged(int arg1)
{
    //根据收藏复选框状态切换收藏状态；
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

void MainWindow::onCellClicked(int row, int column)
{
    //展示表格对应元素单词卡；
    qDebug() << "Cell clicked at row" << row << "and column" << column;
    wordCard.showWordCard(wordDatabase.allWords[showWordList[row].wordID-1]);
    wordCard.show();
}

void MainWindow::listItemClicked(QListWidgetItem *item)
{
    qint32 collectionID=wordDatabase.searchCollectionByCollectionName(item->text());
    if(collectionID==-1)
    {
        return;
    }
    qDebug()<<"Collection Name:"<<wordDatabase.collectionList[collectionID].collectionName;
    initByCollection(wordDatabase.collectionList[collectionID]);
    setWordTable();
    //TODO:修改showWordList内容；

    //TODO:更新添加列表单词数量；
}


void MainWindow::on_TableRefresh_clicked()
{
    //initByCollection();
    setWordTable();
}

