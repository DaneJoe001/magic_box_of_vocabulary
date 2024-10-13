#include "vocabularydatabase.h"

VocabularyDatabase::VocabularyDatabase()
    :database(DatabaseOperation::returnDatabase())
{
    initAllWordInfo();
    initCollectionList();

}

void VocabularyDatabase::initAllWordInfo()
{
    QSqlQuery sql=QSqlQuery(database);
    QString sqlStatement="select * from all_words;";
    qDebug()<<sqlStatement;
    if(sql.exec(sqlStatement)==false)
    {
        qDebug()<<"Sql query failed!";
        return;
    }
    WordInfo word;
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
        initWordList(collection);
        collectionList.append(collection);
    }
    listQuantity=collectionList.size();
    qDebug()<<QString("List quantity:%1").arg(listQuantity);

}

void VocabularyDatabase::initWordList(WordCollectionInfo collection)
{
    QSqlQuery sql=QSqlQuery(database);
    QString sqlStatement=QString("select * from %1;").arg(collection.tableName);
    qDebug()<<sqlStatement;
    if(sql.exec(sqlStatement)==false)
    {
        qDebug()<<"Sql query failed!";
        return;
    }
    quint32 wordID=-1;
    while(sql.next()==true)
    {
        wordID=sql.value(0).toUInt();
        collection.wordIdList.append(wordID);
    }
}

qint32 VocabularyDatabase::searchCollection(QString tableName)
{
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
        index=sql.value(0).toInt()-1;
    }
    return index;
}

bool VocabularyDatabase::isExistInCollection(WordCollectionInfo collection, WordInfo word)
{
    qint32 index=searchCollection(collection.tableName);
    if(index==-1)
    {
        qDebug()<<QString("Table %1 not found!").
                    arg(collection.collectionName);
        return false;
    }
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
    return true;
}

bool VocabularyDatabase::removeWordFromCollection(WordCollectionInfo collection, WordInfo word)
{
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
    return true;
}
