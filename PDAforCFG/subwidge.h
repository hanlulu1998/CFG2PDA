#ifndef SUBWIDGE_H
#define SUBWIDGE_H
#include"GrammerAnalyzer.h"

#include <QWidget>
#include <QCloseEvent>

namespace Ui {
class subwidge;
}

class subwidge : public QWidget
{
    Q_OBJECT

public:
    explicit subwidge(QWidget *parent = nullptr);
    ~subwidge();
    void setResult(bool);
    void closeEvent(QCloseEvent *event);

signals:
    void MySingal(QString);

private slots:
    void on_pushButton_clicked();

private:
    Ui::subwidge *ui;
};

#endif // SUBWIDGE_H
