#include "resource_manager.h"



ResourceManager &ResourceManager::get_resource_manager()
{
    static ResourceManager instance;
    return instance;
}

QList<WordInfo> ResourceManager::get_all_words()
{
    if(all_words.empty())
    {
        all_words=word_database.init_all_word_info();
    }
    return all_words;
}
