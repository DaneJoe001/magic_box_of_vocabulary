#ifndef RESOURCEALLWORDS_H
#define RESOURCEALLWORDS_H
#include"database_vocabulary.h"


class ResourceManager
{
public:
    ResourceManager()=default;
    //禁止拷贝和赋值
    ResourceManager(const ResourceManager&)=delete;
    ResourceManager& operator=(const ResourceManager&)=delete;
    static ResourceManager& get_resource_manager();
    QList<WordInfo> get_all_words();
private:
    QList<WordInfo> all_words;
    DatabaseVocabulary word_database;
};

#endif // RESOURCEALLWORDS_H
