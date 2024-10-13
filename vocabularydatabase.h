#ifndef VOCABULARYDATABASE_H
#define VOCABULARYDATABASE_H

#include <QString>

#include "databaseoperation.h"

//word_text, part_of_speech, british_phonetic_symbol,
//american_phonetic_symbol, word_explanation

#define STAR_LIST 0


struct WordInfo
{
    quint32 wordID;
    QString wordText;
    QString partOfSpeech;
    QString britishPhoneticSymbol;
    QString americanPhoneticSymbol;
    QString wordExplanation;
};

struct WordCollectionInfo
{
    quint32 collectionID;
    QString collectionName;
    quint32 wordQuantity;
    QString tableName;
    QList<quint32> wordIdList;
};


class VocabularyDatabase:public DatabaseOperation
{
public:
    QList<WordInfo> allWords;
    QList<WordCollectionInfo> collectionList;
    quint32 listQuantity;

public:
    VocabularyDatabase();

    void initAllWordInfo();
    void initCollectionList();
    void initWordList(WordCollectionInfo collection);

    qint32 searchCollection(QString tableName);

    bool isExistInCollection(WordCollectionInfo collection,WordInfo word);
    bool addWordToCollection(WordCollectionInfo collection,WordInfo word);
    bool removeWordFromCollection(WordCollectionInfo collection,WordInfo word);

private:
    QSqlDatabase database;

};

#endif // VOCABULARYDATABASE_H
