#ifndef DATABASE_VOCABULARY_H
#define DATABASE_VOCABULARY_H

#include <QString>
#include<QList>
#include<QSqlDatabase>


#define STAR_LIST 0
//TODO:使用QHash管理列表和数据库表名

struct WordInfo
{
    //单词序号
    quint32 wordID;
    //单词文本
    QString wordText;
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
    quint32 word_quantity;
    //对应数据库表名
    QString tableName;
    //合集单词序号列表
    QList<quint32> wordIdList;
};


class DatabaseVocabulary
{
public:
    //单词合集信息，包含所有单词列表
    QList<WordCollectionInfo> collection_list;
    //单词合集数量
    quint32 list_quantity;

public:
    DatabaseVocabulary();

    //初始化单词库
    QList<WordInfo> init_all_word_info();
    //初始化合集列表
    void init_collection_list();
    //初始化单词列表
    void init_word_list(WordCollectionInfo* collection);

    //通过表名查询单词合集，并返回单词合集序号
    qint32 search_collection(QString tableName);

    //通过合集名查询单词合集，并返回单词合集序号
    qint32 search_collection_by_collection_name(QString collectionName);

    //判断单词是否位于某合集
    bool is_exist_in_collection(WordCollectionInfo collection,WordInfo word);
    //添加单词至指定合集
    bool add_word_to_collection(WordCollectionInfo collection,WordInfo word);
    //从指定合集移除单词
    bool remove_word_from_collection(WordCollectionInfo collection,WordInfo word);
    //查找单词并返回单词信息
    WordInfo search_word(QString searchText);
    //获取单词列表
    QList<WordInfo>& get_all_word_list();
    //获取单词专业词典内容
    QString get_word_content_from_dictionary(QString word,QString dictionary_table_name);
    //返回数据库对象
    QSqlDatabase  get_database();

private:

    QSqlDatabase database;


};

#endif // DATABASE_VOCABULARY_H
