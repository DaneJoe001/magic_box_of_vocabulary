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

    void initDataList(QList<WordInfo> temp);

    void showPrev();
    void showNext();
    void showWordCard(WordInfo word);

    bool addWordToCollecting(WordCollectionInfo collection);
    bool removeWordFromCollecting(WordCollectionInfo collection);

    void wordCardPanelManage();

    void testDataInit();

private slots:

    void on_ButtonOK_clicked();

    void on_StarWord_stateChanged(int arg1);

    void on_MemoryCheck_clicked();

private:
    Ui::WordCard *ui;
    quint32 currentIndex;
    quint32 maxIndex;
    quint32 wordQuantity;
    QList<WordInfo> wordList;
    VocabularyDatabase wordDatabase;
    MemoryTestByFillRandomBlank test1;
};

#endif // WORDCARD_H
