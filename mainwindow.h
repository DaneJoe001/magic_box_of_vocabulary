#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QObject>
#include <QListWidget>
#include <QListWidgetItem>
#include <QStandardItemModel>

#include "userlogin.h"
#include "vocabularydatabase.h"
#include "memorytestbyfillrandomblank.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //提供接口，设置单词查询界面单词列表
    void initDataList(QList<WordInfo> temp);

    //切换上一个单词信息
    void showPrev();

    //切换下一个单词信息
    void showNext();

    //设置界面事件响应
    void wordCardPanelManage();

    //展示指定单词信息
    void showWordCard(WordInfo word);

    //展示单词合集列表
    void showCollectionItemList();

    //展示词库管理界面单词表格
    void showWordTable(WordCollectionInfo tableList);

    //添加单词至指定单词合集
    bool addWordToCollecting(WordCollectionInfo collection);

    //从指定单词合集移除单词
    bool removeWordFromCollecting(WordCollectionInfo collection);

private slots:
    //退出按钮
    void on_pushButton_clicked();

    //单词查询界面切换
    void on_wordSearch_clicked();

    //词库管理切换
    void on_wordManage_clicked();

    //个人信息界面切换
    void on_personalPanel_clicked();

    //单词查询界面查询按钮
    void on_SearchButton_clicked();

    //单词拼写测试
    void on_MemoryCheck_clicked();

    //收藏复选框
    void on_StarWord_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;

    //创建登录窗口对象；
    UserLogin loginPanel;

    //设置单词查询界面当前展示列表的当前序号；
    quint32 currentIndex;

    //设置单词查询界面当前展示列表的最大序号；
    quint32 maxIndex;

    //设置当前单词数量；
    quint32 wordQuantity;

    //设置查询界面单词列表；
    QList<WordInfo> wordList;

    //设置词库管理界面词库列表条目；
    QList<QListWidgetItem*> collectionItem;

    //设置词库管理界面词库列表模型；
    QStringListModel *model;

    //创建单词数据库对象；
    VocabularyDatabase wordDatabase;

    //设置单词拼写测试对象；
    MemoryTestByFillRandomBlank test1;
};
#endif // MAINWINDOW_H
