#ifndef WordCard_H
#define WordCard_H

#include <QDialog>
#include "database_vocabulary.h"
#include "question_fill_blank.h"

namespace Ui {
class DialogWordCard;
}

class DialogWordCard : public QDialog
{
    Q_OBJECT

public:
    explicit DialogWordCard(QWidget *parent = nullptr);
    ~DialogWordCard();
    //表格任意位置单击显示的单词卡
    void tableWordCard(WordInfo word ,quint32 srcIndex);

    //初始化单词列表
    void init_data_list(QList<WordInfo> temp);

    //单词序号列表设置查询界面单词列表
    void init_by_collection(WordCollectionInfo collectionInfo);

    //切换上一张图片
    void show_prev();

    //切换下一片图片
    void show_next();

    //用单词卡展示单词信息；
    void show_WordCard(WordInfo word);

    //向指定合集添加单词；
    bool add_word_to_collection(WordCollectionInfo collection);

    //从指定集合移除单词；
    bool remove_word_from_collection(WordCollectionInfo collection);

    //响应单词卡界面按钮；
    void WordCard_panel_manage();

    //初始化测试数据；
    void testDataInit();

    void set_current_index(qint32 word_index);

private slots:

    //确定按钮，用于关闭窗口；
    void on_ButtonOK_clicked();

    //收藏复选框；
    void on_StarWord_stateChanged(int arg1);

    //单词拼写测试；
    void on_MemoryCheck_clicked();

private:
    Ui::DialogWordCard *ui;

    //记录当前展示单词下标；
    qint32 current_index;

    //当前列表最大下标；
    qint32 max_index;

    //当前列表单词数量；
    quint32 word_quantity;

    //当前单词列表
    QList<WordInfo> word_list;

    //创建单词数据库对象；
    DatabaseVocabulary word_database;

    //创建单词拼写测试对象；
    QuestionFillBlank test1;

    QList<WordInfo>all_words;
};

#endif // WordCard_H
