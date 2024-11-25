#include<QSqlQuery>
#include<QSqlError>
#include "window_main.h"
#include "ui_mainwindow.h"

WindowMain::WindowMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug()<<"显示登录窗口";
    //显示登录窗口；
    login_panel.show();

    //接收登录信号，显示主窗口；
    connect(&login_panel,&DialogUserLogin::login_signal,this,[this]()
    {
        current_index=0;
        qDebug()<<"打开主界面";
        //初始化资源管理器引用
        ResourceManager& manager_resource=ResourceManager::get_resource_manager();
        //显示主界面后关闭登录窗口；
        this->show();
        login_panel.close();    
        //调用界面管理函数，相应切换等按钮事件；
        WordCard_panel_manage();
        //设置进入时显示的界面；
        ui->functionPanel->setCurrentIndex(2);
        //初始化单词展示列表的序号；
        current_index=0;
        //将展示列表初始化为所有单词；
        all_words=manager_resource.get_all_words();
        init_data_list(all_words);
        qDebug()<<"单词数量"<<word_list.size();
        qDebug()<<"0号单词"<<word_list[0].wordText;
        show_WordCard(word_list[0]);
        ui->WordExplanation->setWordWrap(true);
        ui->WordExplanation->setFixedWidth(300);
        //设置个人信息界面账号；
        UserInfo current_user=DialogUserLogin::get_current_user();
        ui->currentUserAccount->setText(current_user.user_account);
        //设置个人信息界面最近登录时间；
        ui->currentUserLastLoginTime
            ->setText(QString("最近登陆时间：%1").
            arg(current_user.last_login_time.toString(Qt::ISODate)));
        //设置个人信息界面昵称；
        ui->currentUserNickName->setText(current_user.user_nick_name);
        //初始化要展示的单词合集信息列表；
        for(WordCollectionInfo temp:word_database.collection_list)
        {
            QListWidgetItem *item=new QListWidgetItem(temp.collectionName);
            collection_item.append(item);
        }
        //显示单词合集信息列表；
        show_collection_item_list();
        //初始化单元格
        show_word_list=manager_resource.get_all_words();
        set_word_table();
        //连接表格和槽函数
        QObject::connect(ui->tableWidget,&QTableWidget::cellClicked,this,&WindowMain::on_cellClicked);
        //连接合集表项和槽函数
        QObject::connect(ui->word_listWidget,&QListWidget::itemClicked,this,&WindowMain::list_item_clicked);

        //初始化单词名和对应数据表名的哈希表
        dictionary.insert("剑桥英汉双解词典", "cam_bridge_dictionary");
        dictionary.insert("柯林斯高阶英汉双解词典", "collins_dictionary");
        dictionary.insert("牛津高阶英汉双解词典", "oxford_dictionary");
        //需要使用原始字符串
        QString word_content=word_database.get_word_content_from_dictionary("a","cam_bridge_dictionary");
        ui->textBrowser->setHtml(word_content);
    });
}

WindowMain::~WindowMain()
{
    delete ui;
}

void WindowMain::init_data_list(QList<WordInfo> temp)
{
    //设置成员变量
    word_list=temp;
    word_quantity=temp.size();
    current_index=0;
    max_index=word_quantity-1;
}

void WindowMain::init_by_collection(WordCollectionInfo collectionInfo)
{
    qDebug()<<"Uploading table!";
    show_word_list.clear();
    //qDebug()<<collectionInfo.word_quantity;
    word_database.init_collection_list();
    //qDebug()<<"collectionInfo.wordIdList"<<collectionInfo.wordIdList.size();
    for(quint32 i=0;i<collectionInfo.word_quantity;i++)
    {
        show_word_list.append(all_words[(collectionInfo.wordIdList[i]-1)]);
        //qDebug()<<"Index:"<<index;
        //show_word_list.append(word_database.all_words[index]);
    }
    word_quantity=collectionInfo.word_quantity;
}

//Pass
void WindowMain::show_prev()
{
    //展示上一单词；
    //当展示首个单词时点击跳到末尾单词；
    current_index--;
    if(current_index<0)
    {
        current_index=max_index;
    }
    //qDebug()<<current_index;
    show_WordCard(word_list[current_index]);
}

//Pass
void WindowMain::show_next()
{
    //展示下一单词；
    //当展示末尾单词时点击跳到首个单词；
    current_index++;
    if(current_index>max_index)
    {
        current_index=0;
    }
    //qDebug()<<current_index;
    qDebug()<<"主界面单词列表数量:"<<word_list.size();
    qDebug()<<"current index"<<current_index;
    qDebug()<<"current word"<<word_list[current_index].wordText;
    show_WordCard(word_list[current_index]);
}

void WindowMain::WordCard_panel_manage()
{
    //连接单词查询界面按钮信号对应槽函数
    QObject::connect(ui->ButtonPrev,&QPushButton::clicked,this,&WindowMain::show_prev);
    QObject::connect(ui->ButtonNext,&QPushButton::clicked,this,&WindowMain::show_next);
    QObject::connect(ui->StarWord,&QCheckBox::stateChanged,this,&WindowMain::on_StarWord_stateChanged);
}

//TODO:检查结构设计，考虑将功能全部交由WordCard类实现;
void WindowMain::show_WordCard(WordInfo word)
{
    //判断下标是否有效
    if(current_index>max_index)
    {
        qDebug()<<"Invailed index!";
        return;
    }
    //设置界面元素；
    ui->WordText->setText(word.wordText);
    ui->BritishPhoneticSymbol->setText(word.britishPhoneticSymbol);
    ui->AmericanPhoneticSymbol->setText(word.americanPhoneticSymbol);
    ui->WordExplanation->setText(word.wordExplanation);
    qDebug()<<"收藏列表数量："<<word_database.collection_list.size();
    //设置收藏复选框状态；
    if(word_database.is_exist_in_collection(word_database.collection_list[STAR_LIST],word))
    {
        ui->StarWord->setCheckState(Qt::Checked);
    }
    else
    {
        ui->StarWord->setCheckState(Qt::Unchecked);
    }
    //设置页脚页码；
    QString pageNumber=QString("%1/%2").arg(word.wordID).arg(word_list.size());
    ui->pageNumberLabel->setText(pageNumber);
}

//在词库管理界面加载词库条目；
void WindowMain::show_collection_item_list()
{
    for(QListWidgetItem* temp:collection_item)
    {
        ui->word_listWidget->addItem(temp);
    }
}

bool WindowMain::add_word_to_collection(WordCollectionInfo collection)
{
    //调用单词数据库对象添加单词至指定列表；
    qDebug()<<"当前单词ID:"<<current_index;
    return word_database.add_word_to_collection(collection,word_list[current_index]);
}

bool WindowMain::remove_word_from_collection(WordCollectionInfo collection)
{
    //调用单词数据库对象移除指定列表单词；
    return word_database.remove_word_from_collection(collection,word_list[current_index]);
}

void WindowMain::set_word_table()
{
    ui->tableWidget->clear();
    //设置单词信息表表头
    QStringList headers;
    headers<<"单词"<<"英式音标"<<"美式音标"<<"单词释义"<<"wordID";
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setRowCount(show_word_list.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    //填充表格内容；
    for(int row=0;row<show_word_list.size();row++)
    {
        const WordInfo &word=show_word_list[row];
        ui->tableWidget->setItem(row,0,new QTableWidgetItem(word.wordText));
        ui->tableWidget->setItem(row,1,new QTableWidgetItem(word.britishPhoneticSymbol));
        ui->tableWidget->setItem(row,2,new QTableWidgetItem(word.americanPhoneticSymbol));
        ui->tableWidget->setItem(row,3,new QTableWidgetItem(word.wordExplanation));
        ui->tableWidget->setItem(row,4,new QTableWidgetItem(QString::number(word.wordID)));
    }
    //隐藏wordID列表
    ui->tableWidget->setColumnHidden(4,true);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->tableWidget->resizeColumnsToContents();
    ui->word_quantity->setText(QString("单词数量：%1").arg(show_word_list.size()));
}

void WindowMain::clear_table()
{

}

void WindowMain::refresh_table()
{
    //更新单词列表
    init_by_collection(word_database.collection_list[collection_current_index]);
    set_word_table();
}

void WindowMain::on_pushButton_clicked()
{
    DialogUserLogin::update_login_time();
    //优雅的结束程序doge
    QApplication::exit(0);
}


void WindowMain::on_wordSearch_clicked()
{
    //切换界面；
    ui->functionPanel->setCurrentIndex(2);
}


void WindowMain::on_wordManage_clicked()
{
    //切换界面；
    ui->functionPanel->setCurrentIndex(1);
}


void WindowMain::on_personalPanel_clicked()
{
    //切换界面；
    ui->functionPanel->setCurrentIndex(0);
}


void WindowMain::on_SearchButton_clicked()
{
    //获取搜索框输入内容
    QString searchText=ui->WordLineEdit->text();

    //调用单词数据库对象函数查询并获取单词信息；
    WordInfo word=word_database.search_word(searchText);
    if(word.wordID==0)
    {
        qDebug()<<"未找到！";
        QMessageBox::information(this,"结果","未找到");
        return;
    }
    //更新查询单词后的当前下标
    current_index=word.wordID-1;
    show_WordCard(word);
}


void WindowMain::on_MemoryCheck_clicked()
{
    //this->hide();
    //进行单词拼写测试；
    test1.word=word_list[current_index];
    //清空界面，为测验准备
    test1.clear_label();
    test1.set_dest_word();
    test1.exec();
    //QObject::connect(this->show(),&)
}


void WindowMain::on_StarWord_stateChanged(int arg1)
{
    //根据收藏复选框状态切换收藏状态；
    if(arg1==Qt::Checked)
    {
        add_word_to_collection(word_database.collection_list[STAR_LIST]);
    }
    if (arg1 == Qt::Unchecked)
    {
        remove_word_from_collection(word_database.collection_list[STAR_LIST]);
    }
    //刷新单词表格
    refresh_table();
}

void WindowMain::on_cellClicked(int row, int column)
{
    //展示表格对应元素单词卡；
    qDebug() << "Cell clicked at row" << row << "and column" << column;
    WordCard.set_current_index(row);
    WordCard.tableWordCard(all_words[show_word_list[row].wordID-1],row);
    WordCard.show();
}

void WindowMain::list_item_clicked(QListWidgetItem *item)
{
    //获取单词合集位序
    collection_current_index=word_database.search_collection_by_collection_name(item->text());
    if(collection_current_index==-1)
    {
        return;
    }
    //qDebug()<<"Collection Name:"<<word_database.collection_list[collectionID].collectionName;
    qDebug()<<word_database.collection_list[1].tableName;
    qDebug()<<word_database.collection_list[1].word_quantity;
    qDebug()<<word_database.collection_list[1].wordIdList.size();


    init_by_collection(word_database.collection_list[collection_current_index]);
    set_word_table();
    //更新单词卡
    WordCard.init_by_collection(word_database.collection_list[collection_current_index]);
}


void WindowMain::on_TableRefresh_clicked()
{
    refresh_table();
}


void WindowMain::on_standardDictionary_clicked()
{
    //切换界面；
    ui->functionPanel->setCurrentIndex(4);
}


void WindowMain::on_pushButton_search_on_dictionary_clicked()
{
    //word_database.close_database();
    QString dest_word=ui->lineEdit_word_dictionary->text();
    qDebug()<<"查询的单词"<<dest_word;
    QString dest_dictionary=ui->comboBox_select_dictionary->currentText();
    qDebug()<<"词典"<<dest_dictionary;
    QString table_name=dictionary.value(dest_dictionary);

    QString word_content=word_database.get_word_content_from_dictionary(dest_word,table_name);
    //TODO:分离数据库操作逻辑，数据库操作只应该置于database_vocabulary

    qDebug()<<"查询的数据表"<<table_name;

    ui->textBrowser->setHtml(word_content);

}


void WindowMain::on_pushButton_5_clicked()
{
    //切换界面；
    ui->functionPanel->setCurrentIndex(3);
}

