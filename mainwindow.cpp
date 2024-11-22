#include "mainwindow.h"
#include "ui_mainwindow.h"

//QSqlDatabase DictionaryDatabase::db;
//QHash<QString,QString> DictionaryDatabase::dictionary;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //显示登录窗口；
    login_panel.show();

    //接收登录信号，显示主窗口；
    connect(&login_panel,&UserLogin::login_signal,this,[this]()
    {
        //显示主界面后关闭登录窗口；
        this->show();
        login_panel.close();    
        //调用界面管理函数，相应切换等按钮事件；
        WordCard_panel_manage();
        //设置进入时显示的界面；
        ui->functionPanel->setCurrentIndex(2);
        //初始化单词展示列表的序号；
        current_index=0;
        //DictionaryDatabase::get_database();
        //将展示列表初始化为所有单词；
        init_data_list(word_database.all_words);
        ui->WordExplanation->setWordWrap(true);
        ui->WordExplanation->setFixedWidth(300);
        //用单词1初始化单词展示信息；
        show_WordCard(word_list[0]);
        //设置个人信息界面账号；
        ui->currentUserAccount->setText(login_panel.current_user.user_account);
        //设置个人信息界面最近登录时间；
        ui->currentUserLastLoginTime
            ->setText(QString("最近登陆时间：%1").
            arg(login_panel.current_user.last_login_time.toString(Qt::ISODate)));
        //设置个人信息界面昵称；
        ui->currentUserNickName->setText(login_panel.current_user.user_nick_name);
        //初始化要展示的单词合集信息列表；
        for(WordCollectionInfo temp:word_database.collection_list)
        {
            QListWidgetItem *item=new QListWidgetItem(temp.collectionName);
            collection_item.append(item);
        }
        //显示单词合集信息列表；
        show_collection_item_list();
        //初始化单元格
        show_word_list=word_database.all_words;
        set_word_table();
        //连接表格和槽函数
        QObject::connect(ui->tableWidget,&QTableWidget::cellClicked,this,&MainWindow::on_cellClicked);
        //连接合集表项和槽函数
        QObject::connect(ui->word_listWidget,&QListWidget::itemClicked,this,&MainWindow::list_item_clicked);

        dictionary.insert("剑桥英汉双解词典", "cam_bridge_dictionary");
        dictionary.insert("柯林斯高阶英汉双解词典", "collins_dictionary");
        dictionary.insert("牛津高阶英汉双解词典", "oxford_dictionary");
        //需要使用原始字符串
        QString word_content=QString(R"delimiter(a	\n<font color=blue size=+1><b>a </b></font><font color=red>★</font><br> / <font color=red>ə</font><font color=gray><i>; strong form </i></font><font color=red>ei</font> / (also <font color=blue><b>an </b></font> / <font color=red>ən</font><font color=gray><i>; strong form </i></font><font color=red>æn</font> / )<b><i>indefinite article</i></b> <P><BR><span style="background-color: blue"><font color=white>　Help　</font></span></font> The form<font color=blue><b><i> a </i></b></font>is used before consonant sounds and the form<font color=blue><b><i> an </i></b></font>before vowel sounds. When saying abbreviations like 'FM' or 'UN', use<font color=blue><b><i> a </i></b></font>or<font color=blue><b><i> an </i></b></font>according to how the first letter is said. For example,<font color=blue><b><i> F </i></b></font>is a consonant, but begins with the sound / e / and so you say: an FM radio. <font color=blue><b><i>U </i></b></font>is a vowel but begins with / j / and so you say: a UN declaration. <br>· * a 用于辅音前，an 用于元音前。在 FM、UN 等缩略语前，用 a 还是 an，需视首字母如何发音而定。如 F 是辅音字母，但其发音以 <EPH>e</EPH> 开头，故应说：an FM radio。U 是元音字母，但其发音以 <EPH>j</EPH> 音开头，故应说：a UN declaration。 </P><br>1. used before countable or singular nouns referring to people or things that have not already been mentioned<br>· <font color=darkblue>(用于可数名词或单数名词前，表示未曾提到的)一(人、事、物)</font>：<font color=DodgerBlue><br>　a man / horse / unit </font><font color=darkblue size=-1><br>　一个人；一匹马；一个单位 </font><font color=DodgerBlue><br>　an aunt / egg / hour / x-ray </font><font color=darkblue size=-1><br>　一个姑母；一个鸡蛋；一小时；一张 X 光片 </font><font color=DodgerBlue><br>　I can only carry two at a time. </font><font color=darkblue size=-1><br>　我一次只能带两个。 </font><font color=DodgerBlue><br>　There's a visitor for you. </font><font color=darkblue size=-1><br>　有位客人找你。 </font><font color=DodgerBlue><br>　She's a friend of my father's (=<b><i> one of my father's friends</i></b>).</font><font color=darkblue size=-1><br>　她是我父亲的一位朋友。 </font><br>2. used before uncountable nouns when these have an adjective in front of them, or phrase following them<br>· <font color=darkblue>(用于前有形容词或后有短语的不可数名词前)</font>：<font color=DodgerBlue><br>　a good knowledge of French </font><font color=darkblue size=-1><br>　精通法语 </font><font color=DodgerBlue><br>　a sadness that won't go away </font><font color=darkblue size=-1><br>　挥之不去的悲愁 </font><br>3. any; every<br>· <font color=darkblue> 任何一；每一</font>：<font color=DodgerBlue><br>　A lion is a dangerous animal. </font><font color=darkblue size=-1><br>　狮子是猛兽。 </font><br>4. used to show that sb / sth is a member of a group or profession<br>· <font color=darkblue>(表示为某一群体或职业中的一员)</font>：<font color=DodgerBlue><br>　Their new car's a BMW. </font><font color=darkblue size=-1><br>　他们的新轿车是辆宝马。 </font><font color=DodgerBlue><br>　She's a Buddhist. </font><font color=darkblue size=-1><br>　她是个佛教徒。 </font><font color=DodgerBlue><br>　He's a teacher. </font><font color=darkblue size=-1><br>　他是个教师。 </font><font color=DodgerBlue><br>　Is that a Monet (=<b><i> a painting by Monet</i></b>)?</font><font color=darkblue size=-1><br>　那是莫奈的画吗？ </font><br>5. used in front of two nouns that are seen as a single unit<br>· <font color=darkblue>(用于视为一体的两个名词之前)</font>：<font color=DodgerBlue><br>　a knife and fork </font><font color=darkblue size=-1><br>　一副刀叉 </font><br>6. used instead of <font color=blue><i>one </i></font>before some numbers<br>· <font color=darkblue>(用于某些数字前，代替 one)</font>：<font color=DodgerBlue><br>　A thousand people were there. </font><font color=darkblue size=-1><br>　那里有一千人。 </font><br>7. used when talking about prices, quantities and rates<br>· <font color=darkblue>(用于价格、数量、比率等)一，每一 </font><br>　<span style="background-color: darkgreen"><font color=white>SYN</font></span> <b> <a href="bword://per">per</a> </b>：<font color=DodgerBlue><br>　They cost 50p a kilo. </font><font color=darkblue size=-1><br>　其价钱是一公斤 50 便士。 </font><font color=DodgerBlue><br>　I can type 50 words a minute. </font><font color=darkblue size=-1><br>　我每分钟能打 50 个单词。 </font><font color=DodgerBlue><br>　He was driving at 50 miles an hour. </font><font color=darkblue size=-1><br>　当时他正以每小时 50 英里的速度驾车。 </font><br>8. a person like sb<br>· <font color=darkblue> 像(某人)的人；…式的人物</font>：<font color=DodgerBlue><br>　She's a little Hitler. </font><font color=darkblue size=-1><br>　她是个小希特勒。 </font><br>9. used before sb's name to show that the speaker does not know the person<br>· <font color=darkblue>(用于某人姓名前，表示说话者不认识此人)有个</font>：<font color=DodgerBlue><br>　There's a Mrs Green to see you. </font><font color=darkblue size=-1><br>　有位格林太太要见你。 </font><br>10. used before the names of days of the week to talk about one particular day<br>· <font color=darkblue>(用于一星期中某天的名称前，表示具体某一天)</font>：<font color=DodgerBlue><br>　She died on a Tuesday. </font><font color=darkblue size=-1><br>　她是在一个星期二去世的。 </font>\n
)delimiter");
        ui->textBrowser->setHtml(word_content);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init_data_list(QList<WordInfo> temp)
{
    //设置成员变量
    word_list=temp;
    word_quantity=temp.size();
    max_index=word_quantity-1;
}

void MainWindow::init_by_collection(WordCollectionInfo collectionInfo)
{
    qDebug()<<"Uploading table!";
    show_word_list.clear();
    //qDebug()<<collectionInfo.word_quantity;
    word_database.init_collection_list();
    //qDebug()<<"collectionInfo.wordIdList"<<collectionInfo.wordIdList.size();
    for(quint32 i=0;i<collectionInfo.word_quantity;i++)
    {
        show_word_list.append(word_database.all_words[(collectionInfo.wordIdList[i]-1)]);
        //qDebug()<<"Index:"<<index;
        //show_word_list.append(word_database.all_words[index]);
    }
    word_quantity=collectionInfo.word_quantity;
}

//Pass
void MainWindow::show_prev()
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
void MainWindow::show_next()
{
    //展示下一单词；
    //当展示末尾单词时点击跳到首个单词；
    current_index++;
    if(current_index>max_index)
    {
        current_index=0;
    }
    //qDebug()<<current_index;
    show_WordCard(word_list[current_index]);
}

void MainWindow::WordCard_panel_manage()
{
    //连接单词查询界面按钮信号对应槽函数
    QObject::connect(ui->ButtonPrev,&QPushButton::clicked,this,&MainWindow::show_prev);
    QObject::connect(ui->ButtonNext,&QPushButton::clicked,this,&MainWindow::show_next);
    QObject::connect(ui->StarWord,&QCheckBox::stateChanged,this,&MainWindow::on_StarWord_stateChanged);
}

void MainWindow::show_WordCard(WordInfo word)
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
void MainWindow::show_collection_item_list()
{
    for(QListWidgetItem* temp:collection_item)
    {
        ui->word_listWidget->addItem(temp);
    }
}

bool MainWindow::add_word_to_collection(WordCollectionInfo collection)
{
    //调用单词数据库对象添加单词至指定列表；
    return word_database.add_word_to_collection(collection,word_list[current_index]);
}

bool MainWindow::remove_word_from_collection(WordCollectionInfo collection)
{
    //调用单词数据库对象移除指定列表单词；
    return word_database.remove_word_from_collection(collection,word_list[current_index]);
}

void MainWindow::set_word_table()
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

void MainWindow::clear_table()
{

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
    WordInfo word=word_database.search_word(searchText);
    if(word.wordID==0)
    {
        qDebug()<<"未找到！";
        QMessageBox::information(this,"结果","未找到");
        return;
    }
    current_index=word.wordID;
    show_WordCard(word);
}


void MainWindow::on_MemoryCheck_clicked()
{
    //this->hide();
    //进行单词拼写测试；
    test1.word=word_list[current_index];
    //qDebug()<<"Test word:"<<word_list[current_index].wordText;
    //清空界面，为测验准备
    test1.clear_label();
    test1.set_dest_word();
    test1.exec();
    //QObject::connect(this->show(),&)
}


void MainWindow::on_StarWord_stateChanged(int arg1)
{
    //根据收藏复选框状态切换收藏状态；
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

void MainWindow::on_cellClicked(int row, int column)
{
    //展示表格对应元素单词卡；
    //qDebug() << "Cell clicked at row" << row << "and column" << column;
    WordCard.tableWordCard(word_database.all_words[show_word_list[row].wordID-1],row);
    WordCard.show();
}

void MainWindow::list_item_clicked(QListWidgetItem *item)
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


void MainWindow::on_TableRefresh_clicked()
{
    //更新单词列表
    init_by_collection(word_database.collection_list[collection_current_index]);
    set_word_table();
}


void MainWindow::on_standardDictionary_clicked()
{
    //切换界面；
    ui->functionPanel->setCurrentIndex(4);
}


void MainWindow::on_pushButton_search_on_dictionary_clicked()
{
    //word_database.close_database();
    QString dest_word=ui->lineEdit_word_dictionary->text();
    qDebug()<<"查询的单词"<<dest_word;
    QString dest_dictionary=ui->comboBox_select_dictionary->currentText();
    qDebug()<<"词典"<<dest_dictionary;
    QString table_name=dictionary.value(dest_dictionary);


    QSqlQuery sql=QSqlQuery(word_database.database);
    qDebug()<<"查询的数据表"<<table_name;
    QString sql_statement=QString("select details from %1 where word='%2';").
                            arg(table_name).
                            arg(dest_word);
    //qDebug()<<"查询语句："<<sql_statement;
    if (!sql.exec(sql_statement))
    {
        qDebug() << "Sql query failed!";
        qDebug() << sql.lastError().text(); // 打印具体的错误信息
        return;
    }

    QString word_content;
    while(sql.next()==true)
    {
        word_content=sql.value(0).toString();
    }
    ui->textBrowser->setHtml(word_content);


}


void MainWindow::on_pushButton_5_clicked()
{
    //切换界面；
    ui->functionPanel->setCurrentIndex(3);
}

