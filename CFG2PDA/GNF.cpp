#include <algorithm>
#include "Grammar.hpp"
//比较产生式的左部大小
bool comp(const GNFProduction &a,const GNFProduction &b){
    return a.left<b.left;
}
bool GNF::isInVset(QString str){
    bool flag=true;
    if(str.length()!=1){//右部是终结符
        for(int i=1;i<str.length();i++){
            QString v=trans[str[i]];
            if(!v_set.contains(v)){
                flag=false;
                break;
            }
        }
    }
     return flag;
}
void GNF::initialGNF(QVector<QChar> T, QVector<QChar> V, QVector<Production> p){
    for(auto i:T){//将终结符中'a'->"a"
       QString temp;
       temp+=i;
       t_vec.push_back(temp);
       t_set.insert(temp);
       trans[i]=temp;
    }
    for(auto i:V){//将变量中'S'->"A1"
        QString temp;
        temp="A"+QString::number(A_count);
        v_set.insert(temp);
        A_count++;
        trans[i]=temp;
    }
    for(auto i:p){
        GNFProduction gnf_p;
        gnf_p.left=trans[i.left];
        if(isInVset(i.right)){
            for(auto j:i.right)
                gnf_p.right.push_back(trans[j]);
        }
        else{
            gnf_p.right.push_back(trans[i.right[0]]);
            for(int j=1;j<i.right.size();j++){
                QChar ch=i.right[j];
                if(!(ch>='A'&&ch<='Z')){//新增非终结符替换终结符
                    if(!t_replace.contains(ch)){
                        QString temp="A"+QString::number(A_count);
                        v_set.insert(temp);
                        v_vec.push_back(temp);
                        A_count++;
                        t_replace[ch]=temp;
                        GNFProduction p;
                        p.left=temp;
                        p.right.push_back(QString(ch));
                        G1.push_back(p);
                    }
                  gnf_p.right.push_back(t_replace[ch]);
                }
                else
                 gnf_p.right.push_back(trans[ch]);
            }

        }
        G1.push_back(gnf_p);
    }
    generateG2();
    generateG3();
}
void GNF::generateG3(){
    int size=A_count-1;
    sort(G2.begin(),G2.end(),comp);
    QVector< QVector<GNFProduction> > v1(size);
    QVector<GNFProduction>  vb;
    QVector< QVector<GNFProduction> > v2(size);
    QVector<GNFProduction>  vb2;
    for(auto i:G2){
       if(i.left[0]=='A'){
           int n=i.left.mid(1).toInt();
           v1[n-1].push_back(i);
       }
       if(i.left[0]=='B'){
           vb.push_back(i);
       }
    }
    v2[size-1]=v1[size-1];//下标最大的Ai右侧第一个字符一定是终结符，直接加入
    for(int i=size-2;i>=0;i--){//从下标第二大的Ai开始遍历
         for(auto k:v1[i]){
             if(t_set.contains(k.right[0]))//右侧第一个字符为终结符，直接加入
             {
                 v2[i].push_back(k);
                 continue;
             }
             else
             {
                 int m=k.right[0].mid(1).toInt();//获取右侧第一个非终结符的下标，一定大于左侧非终结符的下标；
                 k.right.removeFirst();
                 for(auto n:v2[m-1]){//遍历下标为m的已经符合要求的产生式
                     GNFProduction p;
                     p.left=k.left;
                     p.right+=n.right;
                     p.right+=k.right;
                     v2[i].push_back(p);
                 }
             }
         }
    }
    for( auto i:vb){
       if(t_set.contains(i.right[0])){//右侧第一个字符非终结符，直接加入

           vb2.push_back(i);
           continue;
       }
       else{
           int n=i.right[0].mid(1).toInt();//获取右侧第一个字符的下标
           i.right.removeFirst();
           for(auto j:v2[n-1]){
              GNFProduction p;
              p.left=i.left;
              p.right+=j.right;
              p.right+=i.right;
              vb2.push_back(p);
           }
       }
    }
    for(auto i:v2)
        G3+=i;
   G3+=vb2;
}
int GNF::readNumber(QString s){
    QString num="";
    for(QString::iterator it=s.begin()+1;it!=s.end();it++){ //从第二个字符开始到结束全是数字
        num=num+(*it);
    }
    return num.toInt();
}
void GNF::generateG2(){
    QVector<GNFProduction> old;//上一次的产生式
    G2=G1;
    //生成所有的Ak->Ak;
    do{
        old=G2;//每次计算前赋值上一次产生式
        G2.clear();
    for(auto k :old){ //遍历产生式
        if((k.right[0].length()<=1)) { //左边第一个只有终结符
            if(!G2.contains(k))//去重
                G2.append(k);
        }
        else { //否则说明左边第一个是非终结符
            if((readNumber(k.left)>readNumber(k.right[0]))&&(k.right[0][0]==k.left[0])){//消除间接左递归
                for(auto j:old){
                    if(j.left==k.right[0]){ //找到所有Aj的产生式
                     QVector<QString> a=k.right;
                     a.removeFirst(); //保留右边第一个以后的式子

                     QVector<QString> r=j.right;

                     //添加Ak->ra
                     GNFProduction Ak;
                     Ak.left=k.left;
                     Ak.right=r+a;

                     if(!G2.contains(Ak))//去重
                     G2.append(Ak);

                    }
                }
            }
            else{
                //其他的产生式加入P2，不做处理
                if(!G2.contains(k))//去重
                    G2.append(k);
            }
        }

    }

    }while (old!=G2);//不再增加跳出循环
    //处理所有的Ak->Ak;
    do {
        old=G2;//每次计算前赋值上一次产生式
        for(auto k :old){ //遍历产生式
        if((readNumber(k.left)==readNumber(k.right[0]))&&(k.right[0][0]==k.left[0])){ //找到所有的Ak->Ak
            static int num=readNumber(k.left);
            if(num!=readNumber(k.left)){
                B_conut++;
            }
            QString B=QString("B%1").arg(B_conut);
            QVector<GNFProduction> Temp;
            for(QVector<GNFProduction>::iterator m=G2.begin();m!=G2.end();m++){//删除AK->Ak式子
                if((*m).left==(*m).right[0]&&(*m).left==k.left){
                    //满足则删除
                }
                else{
                    Temp.append(*m);
                }
            }
            G2=Temp;
            for(auto j:old){ //找到所有Ak->AK的其他产生式
                if(j.left==k.right[0]&&j.left!=j.right[0]){
                    QVector<QString> a=k.right;
                    a.removeFirst(); //保留右边第一个以后的式子


                    //添加所有的Ak->b
                    if(!G2.contains(j))//去重
                    G2.append(j);

                    //添加Ak->bB
                    GNFProduction temp;
                    temp.left=j.left;
                    temp.right=j.right;
                    temp.right.append(B);
                    if(!G2.contains(temp))//去重
                    G2.append(temp);


                    //添加B->a;
                    temp.left=B;
                    temp.right=a;
                    if(!G2.contains(temp))//去重
                    G2.append(temp);

                    //添加B->aB;
                    temp.right.append(B);
                    if(!G2.contains(temp))//去重
                    G2.append(temp);
                }
            }
        }
        }
   }while(old!=G2);
}
