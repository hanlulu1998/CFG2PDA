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
using namespace std;
//自动机模拟输出结构
struct AutomachineInput{ //AutomachineInput
    QString inputChar;//输入带符号
    QString stackChar;//栈顶符号
    AutomachineInput(QString inputChar,QString stackChar){
        this->inputChar=inputChar;
        this->stackChar=stackChar;
    }
    inline bool operator<(const AutomachineInput &t)const{
        return inputChar+stackChar<t.inputChar+t.stackChar;
    }
};
//CFG产生式结构
struct Production{
    QChar left;//产生式左部
    QString right;//产生式右部
    inline bool operator==(const Production p) const{
      return (this->left==p.left)&&(this->right==p.right);
    }
};
//GNF产生式结构
struct GNFProduction{
    QString left;
    QVector<QString> right;
    inline bool operator<=(const GNFProduction p) const{
        return (this->left)<=p.left;
    }
    inline bool operator==(const GNFProduction & p)const{
        return (this->left==p.left)&&(this->right==p.right);
    }
};
class PDA{
private:
    QMap<AutomachineInput,QSet<QVector<QString>>> rule;//NPDA的转换规则
    QVector<QString> t_vec;
    QSet<QString> t_set;
    QVector<GNFProduction> G3;
    QString EPSILON="#";

public:
    void generateRule();//产生PDA转换规则
    void initialPDA(QSet<QString>,QVector<GNFProduction>);//初始化PDA
    bool inference(QString,int, QStack<QString>, int);//判定字符串是否为可接受的语言
    QVector<QVector<QString>> delta(QString,QString);//产生式映射
    inline QStack<QString> getStack(){
        return Stack;
    }
    inline QMap<AutomachineInput,QSet<QVector<QString>>> getrule(){
        return rule;
    }
    QStack<QString> Stack;//自动机内部栈
    QString Result;//运行log信息
};
class GNF{
private:
    //终结符向量和集合
    QVector<QString> t_vec;
    QSet<QString> t_set;
    QMap<QChar,QString> t_replace;
    //非终结符向量和集合
    QVector<QString> v_vec;
    QSet<QString> v_set;
    QVector<GNFProduction> G1;//G1产生式
    QVector<GNFProduction> G2;//G2产生式
    QVector<GNFProduction> G3;//G3产生式
    QMap<QChar,QString> trans; //转换元素映射
    int A_count=1;//A的下标号
    int B_conut=1;//B的下标号
public:
    void initialGNF(QVector<QChar> T,QVector<QChar> V,QVector<Production> p);//初始化GNF
    bool isInVset(QString str);//判断字符串在不在变量集合中
    void generateG3(); //生成G3文法
    void generateG2();//生成G2文法
    int readNumber(QString s);//读出非终结符序号
    inline QVector<GNFProduction> getG2(){
        return G2;

    }
    inline QVector<GNFProduction> getG3(){
        return G3;

    }
    inline QVector<GNFProduction> getG1(){
        return G1;
    }
    inline  QSet<QString> getTset(){
        return t_set;
    }
};
class CFG{
private:
    friend class MainWindow;//友元类：Qt的MainWindow类
    //终结符向量和集合
    QVector<QChar> T_vec;
    QSet<QChar> T_set;
    //非终结符向量和集合
    QVector<QChar> V_vec;
    QSet<QChar> V_set;
    QChar epsilon='#';//空串
    QSet<QChar> nullable_V;//可空非终结符集合
    QVector<Production> products;//原产生式
    QVector<Production> noEpsilonPro;//去空的产生式
    QVector<Production> noSinglePro;//去单一的产生式
    QVector<Production> useProducts; //去无用的产生式
    GNF gnf;//调用gnf对象
    PDA pda;//调用pda对象
public:
    void initialCFG(QTextDocument * doc);//读用户数据，初始化CFG并进行后续的转换
    void addToV(QChar ch);//将字符加入变量集中
    void addToT(QChar ch);//将字符加入终结符集中
    QSet<QChar> nullAbleV();//产生可空变量集
    bool isInSet(QString ch,QSet<QChar> set);//判断ch是否在set中
    void removeEposilonPro();//除去空产生式
    void dealOnePro(Production p);//生成单个产生式的派生的产生式
    void recurDerive(Production p,QQueue<int> queue,int count);//对产生式中的可空符号递归删除并派生新的产生式
    bool isInNOepsilonPro(Production p);//判断一个产生式是否在去除空产生式集中
    void removeSinglePro();//去除单一产生式
    bool isInNOsinglePro(Production p);//判断一个产生式是否在去除单一产生式集中
    void removeNotUsePro();//去除无用的产生式
};
#endif // GRAMMAR_HPP
