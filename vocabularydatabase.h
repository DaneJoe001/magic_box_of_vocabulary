#ifndef VOCABULARYDATABASE_H
#define VOCABULARYDATABASE_H

#include <QString>

#include "databaseoperation.h"

#define STAR_LIST 0

struct WordInfo
{
    //单词序号
    quint32 wordID;
    //单词文本
    QString wordText;
    //单词词性
    QString partOfSpeech;
    //英式音标
    QString britishPhoneticSymbol;
    //美式音标
    QString americanPhoneticSymbol;
    //单词释义
    QString wordExplanation;
};

struct WordCollectionInfo
{
    //单词合集序号
    quint32 collectionID;
    //合集名字
    QString collectionName;
    //合集单词数量
    quint32 wordQuantity;
    //对应数据库表名
    QString tableName;
    //合集单词序号列表
    QList<quint32> wordIdList;
};


class VocabularyDatabase:public DatabaseOperation
{
public:
    //单词库，包含所有单词
    QList<WordInfo> allWords;
    //单词合集信息，包含所有单词列表
    QList<WordCollectionInfo> collectionList;
    //单词合集数量
    quint32 listQuantity;

public:
    VocabularyDatabase();

    //初始化单词库
    void initAllWordInfo();
    //初始化合集列表
    void initCollectionList();
    //初始化单词列表
    void initWordList(WordCollectionInfo* collection);

    //通过表名查询单词合集，并返回单词合集序号
    qint32 searchCollection(QString tableName);

    //通过合集名查询单词合集，并返回单词合集序号
    qint32 searchCollectionByCollectionName(QString collectionName);

    //判断单词是否位于某合集
    bool isExistInCollection(WordCollectionInfo collection,WordInfo word);
    //添加单词至指定合集
    bool addWordToCollection(WordCollectionInfo collection,WordInfo word);
    //从指定合集移除单词
    bool removeWordFromCollection(WordCollectionInfo collection,WordInfo word);
    //查找单词并返回单词信息
    WordInfo searchWord(QString searchText);

private:
    //创建数据库对象
    QSqlDatabase database;

};

#endif // VOCABULARYDATABASE_H
