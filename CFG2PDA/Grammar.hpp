#ifndef GRAMMAR_HPP
#define GRAMMAR_HPP

#include<QVector>
#include<QMap>
#include<QSet>
#include<QTextDocument>
#include<QString>
#include<QTextBlock>
#include<QStack>
#include<QDebug>
#include<QQueue>
#include<QPair>
#include<iostream>

typedef QPair<int,QString> pdaMsg;
using namespace std;
struct current_input{
    QString input_ch;//输入带符号
    QString stack_ch;//栈顶符号
    current_input(QString input_ch,QString stack_ch)
    {
        this->input_ch=input_ch;
        this->stack_ch=stack_ch;
    }
    bool operator<(const current_input &t)const
    {
        return input_ch+stack_ch<t.input_ch+t.stack_ch;
    }
};//PDA转换函数左部
struct Production{
      QChar left;//产生式左部
      QString right;//产生式右部
      bool operator==(const Production p) const
      {
          return (this->left==p.left)&&(this->right==p.right);
      }
};

struct GNFProduction
{
    QString left;
    QVector<QString> right;
    bool operator<=(const GNFProduction p) const
    {
        return (this->left)<=p.left;
    }
    bool operator==(const GNFProduction & p)const{
        return (this->left==p.left)&&(this->right==p.right);
    }
};
class PDA{
private: QMap<current_input,QSet<QVector<QString>>> rule;//NPDA的转换规则，同一输入可能对应不同
         QVector<QString> Terminals;
         QSet<QString> t_set;
         QVector<GNFProduction> g3;
         QString EPSILON="#";

public:void generateRule();
       void initialPDA(QSet<QString>,QVector<GNFProduction>);
       void printRule();
       bool inference(QString,int, QStack<QString>, int);//判定字符串是否为可接受的语言
       QVector<QVector<QString>> delta(QString,QString);

       inline QStack<QString> getStack(){
          return Stack;
       }
       inline QMap<current_input,QSet<QVector<QString>>> getrule(){
                  return rule;
       }
       QStack<QString> Stack;
       QString Result;

};

class GNF{
private: QVector<QString> terminals;
         QSet<QString> t_set;
         QMap<QChar,QString> t_replace;
         QVector<QString> vars;
         QSet<QString> v_set;
         QVector<GNFProduction> gnf_g1;
         QVector<GNFProduction> gnf_g3;
         QMap<QChar,QString> trans; //'a'->"a",'S'->"A1"
         int vars_count=1;
         QVector<GNFProduction> gnf_g2;//G2产生式
         int B_conut=1;//B的顺序

public: void initialGNF(QVector<QChar> T,QVector<QChar> V,QVector<Production> p);
        bool f(QString str);
        void showProduction();
        void generateG3();
        void generateG2();
        int readNumber(QString s);//读出非终结符序号
        void showG2Production();
        void showG3Production();
        inline QVector<GNFProduction> getgnf_g2(){
            return gnf_g2;

        }
        inline QVector<GNFProduction> getgnf_g3(){
            return gnf_g3;

        }
        inline QVector<GNFProduction> getgnf_g1(){
            return gnf_g1;
        }
        inline  QSet<QString> getTset(){
            return t_set;
        }
};
class CFG{
private:
            friend class MainWindow;
            QVector<QChar> Terminals;//终结符向量
            QVector<QChar> Vars;//非终结符向量
            QChar epsilon='#';//空串
            QSet<QChar> T_set;//终结符集合
            QSet<QChar> V_set;//非终结符集合
            QSet<QChar> nullable_V;//可空非终结符集合
            QVector<Production> products;//原产生式
            QVector<Production> NOepsi_products;//去epsilon产生式
            QVector<Production> NOsingle_products;//去单一产生式
            QVector<Production> Use_products; //有用的产生式
            GNF gnf;
            PDA pda;

public: void initialCFG(QTextDocument * doc);//读文法，生成基本数据
        void addToV(QChar ch);//将ch加入非终结符集中
        void addToT(QChar ch);//将ch加入终结符集中
        QSet<QChar> nullAbleV();//返回可空非终结符集
        bool isInSet(QString ch,QSet<QChar> set);//判断ch是否在set中
        void showSet(QSet<QChar> set);
        void showProductions();
        void removeEpoProductions();//除去epoislon产生式
        void processProductions(Production p);//生成单个产生式派生的产生式
        void BFSProduction(Production p,QQueue<int> queue,int count);//对产生式中的可空符号广度优先搜索，派生产生式
        void showNoEposProductions();
        bool isInNOepsiPro(Production p);//判断一个产生式是否在去除epsilon的产生式集中
        void removeSingleProduction();//去除单一产生式
        bool isInNOSinglePro(Production p);
        void showNOSinglePro();
        void removeNotUseProductions();//去除无用的产生式

};
#endif // GRAMMAR_HPP
