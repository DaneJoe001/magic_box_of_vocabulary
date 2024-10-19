#include "vocabularydatabase.h"

VocabularyDatabase::VocabularyDatabase()
    :database(DatabaseOperation::returnDatabase())
{
    //初始化所有单词；
    initAllWordInfo();
    //初始化单词合集列表；
    initCollectionList();

}

void VocabularyDatabase::initAllWordInfo()
{
    //查询并存储all_words单词表数据；
    QSqlQuery sql=QSqlQuery(database);
    QString sqlStatement="select * from all_words;";
    qDebug()<<sqlStatement;
    if(sql.exec(sqlStatement)==false)
    {
        qDebug()<<"Sql query failed!";
        return;
    }
    WordInfo word;
    //依次便利查询结果；
    while(sql.next()==true)
    {
        word.wordID=sql.value(0).toUInt();
        word.wordText=sql.value(1).toString();
        word.partOfSpeech=sql.value(2).toString();
        word.britishPhoneticSymbol=sql.value(3).toString();
        word.americanPhoneticSymbol=sql.value(4).toString();
        word.wordExplanation=sql.value(5).toString();
        allWords.append(word);
    }
}

void VocabularyDatabase::initCollectionList()
{
    QSqlQuery sql=QSqlQuery(database);
    QString sqlStatement="select * from collection_info;";
    qDebug()<<sqlStatement;
    if(sql.exec(sqlStatement)==false)
    {
        qDebug()<<"Sql query failed!";
        return;
    }
    WordCollectionInfo collection;
    while(sql.next()==true)
    {
        collection.collectionID=sql.value(0).toUInt();
        collection.collectionName=sql.value(1).toString();
        collection.wordQuantity=sql.value(2).toUInt();
        collection.tableName=sql.value(3).toString();
        //初始化单词合集列表序号；
        initWordList(&collection);
        qDebug()<<"initCollectionList:"
                 <<collection.collectionName
                 <<collection.wordIdList.size();
        collectionList.append(collection);
    }
    for(quint32 i=0;i<listQuantity;i++)
    {
        if(collectionList[i].collectionID==collection.collectionID)
        {
            collectionList[i]=collection;
            //TODO:内存管理；
            return;
        }
    }
    //获取单词合集数量；
    listQuantity=collectionList.size();
    qDebug()<<QString("List quantity:%1").arg(listQuantity);

}

void VocabularyDatabase::initWordList(WordCollectionInfo* collection)
{
    QSqlQuery sql=QSqlQuery(database);
    QString sqlStatement=QString("select * from collection_info where table_name='%1'");
//    if(sql.exec(sqlStatement)==false)
//    {
//        qDebug()<<"Sql query failed!";
//        return;
//    }
//    quint32 collectionID=0;
//    if(sql.next()==true)
//    {
//        collectionID=sql.value(0).toUInt();
//    }
    //根据表名查询对应数据并存储；
    sqlStatement.clear();
    sqlStatement=QString("select * from %1;").arg(collection->tableName);
    qDebug()<<sqlStatement;
    if(sql.exec(sqlStatement)==false)
    {
        qDebug()<<"Sql query failed!";
        return;
    }
    //根据查询结果添加单词序号；
    quint32 wordID=0;
    while(sql.next()==true)
    {
        wordID=sql.value(0).toUInt();
        collection->wordIdList.append(wordID);
    }
    collection->wordQuantity=collection->wordIdList.size();
}

qint32 VocabularyDatabase::searchCollection(QString tableName)
{
    //查询单词数据表位序；
    qint32 index=-1;
    QSqlQuery sql=QSqlQuery(database);
    QString sqlStatement =
        QString("SELECT list_id FROM collection_info WHERE table_name='%1'").
                           arg(tableName);
    qDebug()<<sqlStatement;
    if(sql.exec(sqlStatement)==false)
    {
        qDebug() << "SQL query failed: " << sql.lastError().text();
        return -1;
    }
    if(sql.next()==true)
    {
        //将位置数据更新为下标；
        index=sql.value(0).toInt()-1;
    }
    return index;
}

qint32 VocabularyDatabase::searchCollectionByCollectionName(QString collectionName)
{
    //查询单词数据表位序；
    qint32 index=-1;
    QSqlQuery sql=QSqlQuery(database);
    QString sqlStatement =
        QString("SELECT list_id FROM collection_info WHERE collection_name='%1'").
        arg(collectionName);
    qDebug()<<sqlStatement;
    if(sql.exec(sqlStatement)==false)
    {
        qDebug() << "SQL query failed: " << sql.lastError().text();
        return -1;
    }
    if(sql.next()==true)
    {
        //将位置数据更新为下标；
        index=sql.value(0).toInt()-1;
        qDebug()<<"Collection:"<<index;
    }
    return index;
}

bool VocabularyDatabase::isExistInCollection(WordCollectionInfo collection, WordInfo word)
{
    //查询合集是否存在；
    qint32 index=searchCollection(collection.tableName);
    if(index==-1)
    {
        qDebug()<<QString("Table %1 not found!").
                    arg(collection.collectionName);
        return false;
    }
    //查询单词是否存在对应合集内；
    QSqlQuery sql=QSqlQuery(database);
    QString sqlStatement = QString("SELECT * FROM %1 WHERE word_id=%2").
                           arg(collection.tableName).
                           arg(word.wordID);
    qDebug()<<sqlStatement;
    if(sql.exec(sqlStatement)==false)
    {
        qDebug() << "SQL query failed: " << sql.lastError().text();
        return false;
    }
    if(sql.next()==true)
    {
        return true;
    }
    return false;
}

bool VocabularyDatabase::addWordToCollection(WordCollectionInfo collection, WordInfo word)
{
    qint32 index=searchCollection(collection.tableName);
    if(index==-1)
    {
        qDebug()<<QString("Table %1 not found!").arg(collection.collectionName);
        return false;
    }
    //检查单词是否存在，不存在则添加；
    if(isExistInCollection(collection,word))
    {
        qDebug()<<QString("Word %1 has been added to table %2").
                    arg(word.wordText).
                    arg(collection.collectionName);
        //TODO:show tip information on ui;
        return false;
    }
    QSqlQuery sql=QSqlQuery(database);
    QString sqlStatement=QString("insert into %1 (word_id) values(%2);").
                           arg(collection.tableName).
                           arg(word.wordID);
    qDebug()<<sqlStatement;
    if(sql.exec(sqlStatement)==false)
    {
        qDebug() << "Failed to add word "
                 << word.wordText << " to table "
                 << collection.collectionName
                 << ": " << sql.lastError().text();
        //TODO:show tip information on ui;
        return false;
    }
    collection.wordQuantity++;
    sqlStatement=QString("UPDATE collection_info SET word_quantity = %1 WHERE table_name = '%2';").
                   arg(collection.wordQuantity).
                   arg(collection.tableName);
    qDebug()<<sqlStatement;
    if(!sql.exec(sqlStatement))
    {
        qDebug()<<"Update error!";
    }
    return true;
}

bool VocabularyDatabase::removeWordFromCollection(WordCollectionInfo collection, WordInfo word)
{
    //在表和单词存在的情况下删除单词；
    qint32 index=searchCollection(collection.tableName);
    if(index==-1)
    {
        qDebug()<<QString("Table %1 not found!").arg(collection.collectionName);
        return false;
    }
    if(isExistInCollection(collection,word)==false)
    {
        qDebug()<<QString("Word %1 hasn't been added to table %2").
                    arg(word.wordText).
                    arg(collection.collectionName);
        //TODO:show tip information on ui;
        return false;
    }
    QSqlQuery sql=QSqlQuery(database);
    QString sqlStatement=QString("delete from %1 where word_id=%2;").
                           arg(collection.tableName).
                           arg(word.wordID);
    qDebug()<<sqlStatement;
    if(sql.exec(sqlStatement)==false)
    {
        qDebug() << "Failed to delete word "
                 << word.wordText << " from table "
                 << collection.collectionName
                 << ": " << sql.lastError().text();
        //TODO:show tip information on ui;
        return false;
    }
    collection.wordQuantity--;
    sqlStatement=QString("UPDATE collection_info SET word_quantity = %1 WHERE table_name = '%2';").
                   arg(collection.wordQuantity).
                   arg(collection.tableName);
    qDebug()<<sqlStatement;
    if(!sql.exec(sqlStatement))
    {
        qDebug()<<"Update error!";
    }
    return true;
}

WordInfo VocabularyDatabase::searchWord(QString searchText)
{
    WordInfo word;
    word.wordID=0;
    //查询单词并获取单词信息；
    QSqlQuery sql=QSqlQuery(database);
    QString sqlStatement=QString("select * from all_words where word_text='%1'").
                           arg(searchText);
    qDebug()<<sqlStatement;
    if(sql.exec(sqlStatement)==false)
    {
        qDebug()<<"Sql query failed!";
        return word;
    }
    if(sql.next()==true)
    {
        word.wordID=sql.value(0).toUInt();
        word.wordText=sql.value(1).toString();
        word.partOfSpeech=sql.value(2).toString();
        word.britishPhoneticSymbol=sql.value(3).toString();
        word.americanPhoneticSymbol=sql.value(4).toString();
        word.wordExplanation=sql.value(5).toString();
        allWords.append(word);
    }
    return word;
}
