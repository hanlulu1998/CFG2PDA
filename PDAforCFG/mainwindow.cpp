
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


            fileString.clear();
            fileString.append("输入的文法：\n");
            fileString.append(ui->GrammerTextEdit->toPlainText());
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
    }
            );




    connect(ui->actionepsilon,&QAction::triggered,
            [=](){
        ui->displaytext->clear();
          ui->displaytext->append("消除epsilon产生式：");
         QVector<Production>p=analyzer.getNOepsi_products();
         for(auto i :p){
             QString s=QString(i.left)+"->"+i.right;
             ui->displaytext->append(s);
         }
    });

    connect(ui->actionunit,&QAction::triggered,
            [=](){
        ui->displaytext->clear();
         ui->displaytext->append("消除单一产生式：");
         QVector<Production>p=analyzer.getNOsingle_products();
         for(auto i :p){
             QString s=QString(i.left)+"->"+i.right;
             ui->displaytext->append(s);
         }
    });

    connect(ui->actionuse,&QAction::triggered,
            [=](){
        ui->displaytext->clear();
        ui->displaytext->append("消除无用产生式：");
         QVector<Production>p=analyzer.getUse_products();
         for(auto i :p){
             QString s=QString(i.left)+"->"+i.right;
             ui->displaytext->append(s);
         }
    });

    connect(ui->actionG1,&QAction::triggered,
            [=](){
        ui->displaytext->clear();
         ui->displaytext->append("转换成G1文法：");
         QVector<GNFProduction>p=analyzer.gnf.getgnf_pro();
         for(auto i :p){
             QString t="";
             for(auto j:i.right){
                 t+=j;
             }
             QString s=QString(i.left)+"->"+t;
             ui->displaytext->append(s);
         }
    });

    connect(ui->actionG2,&QAction::triggered,
            [=](){
        ui->displaytext->clear();
         ui->displaytext->append("转换成G2文法：");
         QVector<GNFProduction>p=analyzer.gnf.getgnf_g2();
         for(auto i :p){
             QString t="";
             for(auto j:i.right){
                 t+=j;
             }
             QString s=QString(i.left)+"->"+t;
             ui->displaytext->append(s);
         }
    });

    connect(ui->actionG3,&QAction::triggered,
            [=](){
        ui->displaytext->clear();
        ui->displaytext->append("转换成G3文法：");
        QVector<GNFProduction>p=analyzer.gnf.getgnf_g3();
         for(auto i :p){
             QString t="";
             for(auto j:i.right){
                 t+=j;
             }
             QString s=QString(i.left)+"->"+t;
             ui->displaytext->append(s);
         }
    });
    connect(ui->actionpda,&QAction::triggered,
                [=](){
            ui->displaytext->clear();
            ui->displaytext->append("PDA转移函数：");
             QMap<current_input,QSet<QVector<QString>>> rule=analyzer.pda.getrule();
             for(auto i:rule.keys())
             {  for(auto j:rule[i])
                 {
                     QString temp;
                  for(auto k:j)
                  {
                     temp+=k;
                  }
                  QString s="(q0,"+i.input_ch+","+i.stack_ch+")=(q0,"+temp+")";
                  ui->displaytext->append(s);
                 }
             }
        });

    void (subwidge::*SingalFun)(QString)=&subwidge::MySingal;
    connect(&sub,SingalFun,this,&MainWindow::DealSingal);


    connect(ui->actioninput,&QAction::triggered,
               [=](){
           sub.show();
       });

    connect(ui->actionMsg,&QAction::triggered,
            [=](){
        QString str="       制作人:\n叶剑波、韩露露\n    版本:v2.0";
        QMessageBox::about(this,"关于软件",str);
    }

            );
    connect(ui->actionqt,&QAction::triggered,
            [=](){
        QMessageBox::aboutQt(this,"关于Qt");
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_translateButton_clicked()
{
    QTextDocument* doc =ui->GrammerTextEdit->document();
    GrammerAnalyzer temp;
    temp.readGrammer(doc);
    analyzer=temp;
    ui->displaytext->clear();
    ui->displaytext->append("文法转换成功！");
    ui->displaytext->append("请在菜单里选择需要查看的内容！");



    //添加文件信息
    QVector<Production>p;
    fileString.append("消除epsilon产生式：\n");
    p=analyzer.getNOepsi_products();
        for(auto i :p){
                 QString s=QString(i.left)+"->"+i.right+"\n";
                 fileString.append(s);
        }
        fileString.append("消除单一产生式：\n");
        p=analyzer.getNOsingle_products();
            for(auto i :p){
                     QString s=QString(i.left)+"->"+i.right+"\n";
                     fileString.append(s);
            }
        fileString.append("消除无用产生式：\n");
        p=analyzer.getUse_products();
            for(auto i :p){
                     QString s=QString(i.left)+"->"+i.right+"\n";
                     fileString.append(s);
            }

          QVector<GNFProduction>q;
          fileString.append("转换成G1文法：\n");

          q=analyzer.gnf.getgnf_pro();

          for(auto i :q){
             QString t="";
             for(auto j:i.right){
                 t+=j;
             }
             QString s=QString(i.left)+"->"+t+"\n";
             fileString.append(s);
         }
          fileString.append("转换成G2文法：\n");

          q=analyzer.gnf.getgnf_g2();

          for(auto i :q){
             QString t="";
             for(auto j:i.right){
                 t+=j;
             }
             QString s=QString(i.left)+"->"+t+"\n";
             fileString.append(s);
         }
          fileString.append("转换成G3文法：\n");

          q=analyzer.gnf.getgnf_g3();

          for(auto i :q){
             QString t="";
             for(auto j:i.right){
                 t+=j;
             }
             QString s=QString(i.left)+"->"+t+"\n";
             fileString.append(s);
         }
          fileString.append("PDA转移函数：\n");
         QMap<current_input,QSet<QVector<QString>>> rule=analyzer.pda.getrule();
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

}
void MainWindow::DealSingal(QString str){
    bool flag=analyzer.pda.inference(str,0,analyzer.pda.Stack,0);
    sub.setResult(flag);
    fileString.append("输入的串为：\n");
    fileString.append(str+"\n");
    fileString.append("测试结果为：\n");
    if(flag){
        fileString.append("Accept\n");

    }
    else{
        fileString.append("Reject\n");
    }



}
