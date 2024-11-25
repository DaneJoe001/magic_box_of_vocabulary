#ifndef WINDOW_MAIN_H
#define WINDOW_MAIN_H

#include <QObject>
#include <QListWidget>
#include <QMainWindow>
#include <QListWidgetItem>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QSqlDatabase>

#include "dialog_user_login.h"
#include "database_vocabulary.h"
#include "question_fill_blank.h"
#include "window_word_card.h"
#include "resource_manager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class WindowMain : public QMainWindow
{
    Q_OBJECT

public:
    WindowMain(QWidget *parent = nullptr);
    ~WindowMain();
    //初始化窗口界面
    void init_main_window();

    //提供接口，设置单词查询界面单词列表
    void init_data_list(QList<WordInfo> temp);

    //单词序号列表设置查询界面单词列表
    void init_by_collection(WordCollectionInfo collectionInfo);

    //切换上一个单词信息
    void show_prev();

    //切换下一个单词信息
    void show_next();

    //设置界面事件响应
    void WordCard_panel_manage();

    //展示指定单词信息
    void show_WordCard(WordInfo word);

    //展示单词合集列表
    void show_collection_item_list();

    //展示词库管理界面单词表格
    void show_word_table(WordCollectionInfo tableList);

    //添加单词至指定单词合集
    bool add_word_to_collection(WordCollectionInfo collection);

    //从指定单词合集移除单词
    bool remove_word_from_collection(WordCollectionInfo collection);

    //设置单词表格；
    void set_word_table();

    //重置单词表格数据
    void clear_table();

    //刷新单词表格的具体逻辑
    void refresh_table();

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

    //表格点击事件；
    void on_cellClicked(int row, int column);

    //合集列表点击事件
    void list_item_clicked(QListWidgetItem *item);

    //表格刷新按钮
    void on_TableRefresh_clicked();

    //切换专业词典页面
    void on_standardDictionary_clicked();

    //专业词典查询按钮
    void on_pushButton_search_on_dictionary_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;

    //创建登录窗口对象；
    DialogUserLogin login_panel;

    //设置单词查询界面当前展示列表的当前序号；
    qint32 current_index;

    //设置单词查询界面当前展示列表的最大序号；
    qint32 max_index;

    //设置当前单词数量；
    quint32 word_quantity;

    //设置当前单词列表索引
    qint32 collection_current_index;

    //设置查询界面单词列表；
    //直接在构造函数初始化
    QList<WordInfo> word_list;

    //设置词库管理界面词库列表条目；
    QList<QListWidgetItem*> collection_item;

    //设置词库管理界面词库列表模型；
    QStringListModel *model;

    //创建单词数据库对象；
    DatabaseVocabulary word_database;

    //设置单词拼写测试对象；
    QuestionFillBlank test1;

    //设置表格单词内容
    QList<WordInfo> show_word_list;

    //创建单词卡对象
    DialogWordCard WordCard;

    QHash<QString,QString>dictionary;

    ResourceManager* manager_resource;

    QList<WordInfo>all_words;

};
#endif // WINDOW_MAIN_H
