#ifndef BASEQUESTION_H
#define BASEQUESTION_H
#include"database_vocabulary.h"


class QuestionBase
{
public:
    QuestionBase()=delete;
    ~QuestionBase()=delete;
    static WordInfo set_current_word(WordInfo word);
    static WordCollectionInfo set_collection(WordCollectionInfo collection);
private:
    static WordInfo current_word;
    static WordCollectionInfo currrent_collection;
};

#endif // BASEQUESTION_H
