#include <QApplication>
#include <QTimer>

#include "mainwindow.h"
#include "databaseoperation.h"
#include "wordcard.h"
#include "memorytestbyfillrandomblank.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    WordInfo testWord;
    testWord.wordID = 12345;
    testWord.wordText = "example";
    testWord.partOfSpeech = "noun";
    testWord.britishPhoneticSymbol = "/ɪɡˈzɑːmpl/";
    testWord.americanPhoneticSymbol = "/ɪɡˈzæmpl/";
    testWord.wordExplanation = "Something that serves as an instance for illustration or explanation.";
    //MainWindow w;
    //w.show();
    WordCard test;
    test.testDataInit();
    test.show();

    //DatabaseOperation db;
    return a.exec();
}
