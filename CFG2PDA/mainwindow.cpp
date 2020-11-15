
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFileDialog>
#include<QFile>
#include<QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("CFG2PDA");
    this->setWindowIcon(QIcon(":/img/bitbug_favicon.ico"));
    ui->stackedWidget->setCurrentIndex(0);
    ui->actionRunChar->setEnabled(false);
    ui->actionRunStart->setEnabled(false);
    connect(ui->GrammerTextEdit,&QPlainTextEdit::textChanged,
            [=](){
        if(ui->GrammerTextEdit->toPlainText().isEmpty())
        ui->actionRunStart->setEnabled(false);
        ui->actionRunStart->setEnabled(true);
    });
    connect(ui->stringTextEdit,&QTextEdit::textChanged,
            [=](){
        if(ui->stringTextEdit->toPlainText().isEmpty())
         ui->actionRunChar->setEnabled(false);
        ui->actionRunChar->setEnabled(true);
    });
    connect(ui->actionopen,&QAction::triggered,
            [=](){
        QString path =QFileDialog::getOpenFileName(this,"open","./");
        if(path.isEmpty()==false){
            QFile file(path);
            bool openFlag=file.open(QIODevice::ReadOnly);
            if(openFlag==true){
    #if 0
               QByteArray array=file.readAll();
               ui->GrammerTextEdit->setPlainText(array);
    #endif
               QByteArray array;
               while(file.atEnd()==false){
                   array+=file.readLine();
               }
                ui->GrammerTextEdit->setPlainText(array);
            }
            file.close();
        }
            ui->displaytext->clear();
    }
            );

    connect(ui->actionsave,&QAction::triggered,
            [=](){

        QString path =QFileDialog::getSaveFileName(this,"save","../");
         if(path.isEmpty()==false){
             QFile file(path);
             bool saveFlag=file.open(QIODevice::WriteOnly);
             if(saveFlag==true){
                 file.write(fileString.toUtf8());
             }
             file.close();
         }
    });

    connect(ui->actionRunStart,&QAction::triggered,
            [=](){
        QTextDocument* doc =ui->GrammerTextEdit->document();
        CFG temp;
        temp.initialCFG(doc);
        cfg=temp;
        ui->displaytext->clear();
        ui->displaytext->append("文法转换成功！");
        //添加文件信息
        fileString.clear();
        fileString.append("输入的文法：\n");
        fileString.append(ui->GrammerTextEdit->toPlainText());
        QVector<Production>p;
        fileString.append("消除epsilon产生式：\n");
        p=cfg.NOepsi_products;
            for(auto i :p){
                     QString s=QString(i.left)+"->"+i.right+"\n";
                     fileString.append(s);
            }
        fileString.append("消除单一产生式：\n");
        p=cfg.NOsingle_products;
            for(auto i :p){
                     QString s=QString(i.left)+"->"+i.right+"\n";
                     fileString.append(s);
            }
        fileString.append("消除无用产生式：\n");
        p=cfg.Use_products;
            for(auto i :p){
                     QString s=QString(i.left)+"->"+i.right+"\n";
                     fileString.append(s);
            }

          QVector<GNFProduction>q;
          fileString.append("转换成G1文法：\n");

          q=cfg.gnf.getgnf_g1();
          for(auto i :q){
             QString t="";
             for(auto j:i.right){
                 t+=j;
             }
             QString s=QString(i.left)+"->"+t+"\n";
             fileString.append(s);
         }
          fileString.append("转换成G2文法：\n");

          q=cfg.gnf.getgnf_g2();

          for(auto i :q){
             QString t="";
             for(auto j:i.right){
                 t+=j;
             }
             QString s=QString(i.left)+"->"+t+"\n";
             fileString.append(s);
         }
          fileString.append("转换成G3文法：\n");

          q=cfg.gnf.getgnf_g3();

          for(auto i :q){
             QString t="";
             for(auto j:i.right){
                 t+=j;
             }
             QString s=QString(i.left)+"->"+t+"\n";
             fileString.append(s);
         }
          fileString.append("PDA转移函数：\n");
             QMap<current_input,QSet<QVector<QString>>> rule=cfg.pda.getrule();
             for(auto i:rule.keys())
             {  for(auto j:rule[i])
                 {
                     QString temp;
                  for(auto k:j)
                  {
                     temp+=k;
                  }
                  QString s="(q0,"+i.input_ch+","+i.stack_ch+")=(q0,"+temp+")"+"\n";
                  fileString.append(s);
                 }
             }
             ui->displaytext->append("==========================\n");
             ui->displaytext->append(fileString);
             ui->displaytext->append("==========================");
             ui->actionRunStart->setEnabled(false);
    }       );
    connect(ui->actioninput,&QAction::triggered,
            [=](){
        ui->stackedWidget->setCurrentIndex(1);
    }   );

    connect(ui->actionBack,&QAction::triggered,
            [=](){
        ui->stackedWidget->setCurrentIndex(0);
    }   );
    connect(ui->actionRunChar,&QAction::triggered,
            [=](){
        ui->stringTextBrowser->clear();
        cfg.pda.Result.clear();
        QString str=ui->stringTextEdit->toPlainText();
        bool flag=cfg.pda.inference(str,0,cfg.pda.Stack,0);
        fileString.append("输入的串为：\n");
        fileString.append(str+"\n");
        fileString.append("测试结果为：\n");
        if(flag){
            fileString.append("Accept\n");
            ui->stringTextBrowser->append("测试结果为：");
            ui->stringTextBrowser->append("Accept\n");
            cfg.pda.Result.append("#");
            ui->stringTextBrowser->append("推断过程为：");
            ui->stringTextBrowser->append(cfg.pda.Result);
            fileString.append("推断过程为：\n");
            fileString.append(cfg.pda.Result);
        }
        else{
            fileString.append("Reject\n");
            ui->stringTextBrowser->append("测试结果为：");
            ui->stringTextBrowser->append("Reject\n");
            ui->stringTextBrowser->append("推断过程为：");
            ui->stringTextBrowser->append(cfg.pda.Result);
            fileString.append("推断过程为：\n");
            fileString.append(cfg.pda.Result);
        }
        ui->actionRunChar->setEnabled(false);
    }   );

    connect(ui->actionClean,&QAction::triggered,
            [=](){
        ui->GrammerTextEdit->clear();
        ui->displaytext->clear();
        ui->stringTextBrowser->clear();
        ui->stringTextEdit->clear();
        fileString.clear();
    }
            );

    connect(ui->actionCleanFile,&QAction::triggered,
            [=](){
          fileString.clear();

    }
            );

}
MainWindow::~MainWindow()
{
    delete ui;
}