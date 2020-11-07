#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"subwidge.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void DealSingal(QString str);

signals:
    void MySingal(int,QString);

private slots:
    void on_translateButton_clicked();

private:
    Ui::MainWindow *ui;
    GrammerAnalyzer analyzer;
    subwidge sub;
    QString fileString;
};
#endif // MAINWINDOW_H
