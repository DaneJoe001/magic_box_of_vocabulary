#ifndef WORDCARD_H
#define WORDCARD_H

#include <QDialog>
#include "vocabularydatabase.h"
#include "memorytestbyfillrandomblank.h"

namespace Ui {
class WordCard;
}

class WordCard : public QDialog
{
    Q_OBJECT

public:
    explicit WordCard(QWidget *parent = nullptr);
    ~WordCard();

    //初始化单词列表
    void initDataList(QList<WordInfo> temp);

    //切换上一张图片
    void showPrev();

    //切换下一片图片
    void showNext();

    //用单词卡展示单词信息；
    void showWordCard(WordInfo word);

    //向指定合集添加单词；
    bool addWordToCollecting(WordCollectionInfo collection);

    //从指定集合移除单词；
    bool removeWordFromCollecting(WordCollectionInfo collection);

    //响应单词卡界面按钮；
    void wordCardPanelManage();

    //初始化测试数据；
    void testDataInit();

private slots:

    //确定按钮，用于关闭窗口；
    void on_ButtonOK_clicked();

    //收藏复选框；
    void on_StarWord_stateChanged(int arg1);

    //单词拼写测试；
    void on_MemoryCheck_clicked();

private:
    Ui::WordCard *ui;

    //记录当前展示单词下标；
    quint32 currentIndex;

    //当前列表最大下标；
    quint32 maxIndex;

    //当前列表单词数量；
    quint32 wordQuantity;

    //当前单词列表
    QList<WordInfo> wordList;

    //创建单词数据库对象；
    VocabularyDatabase wordDatabase;

    //创建单词拼写测试对象；
    //MemoryTestByFillRandomBlank test1;
};

#endif // WORDCARD_H
