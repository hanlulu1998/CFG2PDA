#include "Grammar.hpp"
//向变量集合以及向量中添加非终结字符
void CFG::addToV(QChar ch){
    if(!V_set.contains(ch)){
        V_vec.push_back(ch);
        V_set.insert(ch);
    }
}
//向终结符集合以及向量中添加终结字符
void CFG::addToT(QChar ch){
    if(!T_set.contains(ch)){
        T_vec.push_back(ch);
        T_set.insert(ch);
    }
}
//初始化CFG，读取用户输入的CFG表达式，并启动后面的操作
void CFG::initialCFG(QTextDocument* doc){
    int n=doc->blockCount();
    QString line,temp;
    QChar V,ch;
    Production p;
    for(int i=0;i<n;i++){
        line=doc->findBlockByNumber(i).text();
        if(!line.length())
            continue;
        V=line[0];
        temp=line.mid(3);
        p.left=V;
        p.right=temp;
        products.push_back(p);
        addToV(V);
        for(int j=0;j<temp.length();j++){
            ch=temp[j];
            if(ch>='A'&&ch<='Z')
                addToV(ch);
            else
                addToT(ch);
        }
    }
     removeEposilonPro();//去空产生式
     removeSinglePro();
     removeNotUsePro();
     gnf.initialGNF(T_vec,V_vec,useProducts);
     pda.initialPDA(gnf.getTset(),gnf.getG3());
}
//判断产生式是否在非空产生式集合中
bool CFG::isInNOepsilonPro(Production p){
    for(int i=0;i<noEpsilonPro.size();i++){
        if(p==noEpsilonPro[i])
            return true;
    }
    return false;
}
//判断字符串是否在集合中
bool CFG::isInSet(QString str, QSet<QChar> set){
    for(int i=0;i<str.length();i++){
        QChar ch=str[i];
        if(!set.contains(ch))
            return false;
    }
    return true;
}
//寻找所有的可以推导空字符的可空变量
QSet<QChar> CFG::nullAbleV(){
    QSet<QChar> old_set;
    QSet<QChar> new_set;
    for(int i=0;i<products.size();i++){
        if(products[i].right=="#")
            new_set.insert(products[i].left);
    }
    while(old_set!=new_set){
        old_set=new_set;
        for(int i=0;i<products.size();i++)
        {
            if(isInSet(products[i].right,old_set))
                new_set.insert(products[i].left);
        }
    }
    return new_set;
}
//对一条产生式进行去除空产生式并派生新的产生式的操作
void CFG::dealOnePro(Production p){
    QQueue<int> queue;//记录可空变量在产生式右边的位置
    for(int i=0;i<p.right.length();i++){
        QChar ch=p.right[i];
        if(nullable_V.contains(ch))
             queue.push_back(i);
    }
    if(!queue.size()){
        if(!isInNOepsilonPro(p))//产生式右边无可空变量，则直接添加
            noEpsilonPro.push_back(p);
        return;
    }
    else{
        int count=0;//count记录将符号删除的次数
        recurDerive(p,queue,count);
    }
}
//递归的对产生式右部的可空符号进行依次删除和派生新产生式
void CFG::recurDerive(Production p, QQueue<int> queue,int count)
{
    if(!queue.size())
        return;
    int pos=queue.front();//可空变量的原始位置
    queue.pop_front();
    Production temp=p;
    if(!isInNOepsilonPro(temp)){//如果没有可删除的可空变量
        noEpsilonPro.push_back(temp);
    }
    recurDerive(temp,queue,count);//原始式子递归
    int index=pos-count;//现在可空变量的位置是原始位置减去变短的长度
    temp.right=p.right.remove(index,1);//删除可空变量
    if(!isInNOepsilonPro(temp)&&temp.right.length()!=0)
    {
        noEpsilonPro.push_back(temp);
    }
    recurDerive(temp,queue,count++);//对删除可空变量的式子递归
}
//消除空产生
void CFG::removeEposilonPro(){
    nullable_V=nullAbleV();//返回可空符号集
    for(int i=0;i<products.size();i++){
        if(products[i].right=="#")
            continue;
         dealOnePro(products[i]);
    }
}
//消除单产生式
void CFG::removeSinglePro(){
    QMap<QChar,QSet<QChar>> map;
    QSet<QChar> new_set,old_set;
    for(int i=0;i<V_vec.length();i++){
        QChar ch=V_vec[i];
        old_set.clear();
        new_set.clear();
        new_set.insert(ch);
        //找到所有的单产生式
        while(new_set!=old_set){
            old_set=new_set;
            for(int j=0;j<noEpsilonPro.size();j++){
                QChar left=noEpsilonPro[j].left;
                QString right=noEpsilonPro[j].right;
                if(!old_set.contains(left)||right.length()>1)
                    continue;
                //如果产生式右边等于左边则添加进入单一映射的集合中
                QChar temp=right[0];
                if(V_set.contains(temp)){
                    new_set.insert(temp);
                }
                else
                continue;
            }
        }
        //添加到映射中
        map[ch]=old_set;
        Production p;
        p.left=ch;
        for(int j=0;j<noEpsilonPro.size();j++){
            Production p1=noEpsilonPro[j];
            if(!map[ch].contains(p1.left))
                continue;
            //将所有非单产生式加入
            if(p1.right.length()>1){
                p.right=p1.right;
                if(!isInNOsinglePro(p))
                    noSinglePro.push_back(p);
            }
            else{
                if(V_vec.contains(p1.right[0]))
                    continue;
                else
                    p.right=p1.right;
                if(!isInNOsinglePro(p))
                    noSinglePro.push_back(p);
            }
        }
    }
}
//判断产生式是否在非单产生式集合中
bool CFG::isInNOsinglePro(Production p){
    for(int i=0;i<noSinglePro.size();i++){
        if(p==noSinglePro[i])
            return true;
    }
    return false;
}
void  CFG::removeNotUsePro(){
    QSet<QChar> T_use;//有用终结符集合
    QSet<QChar> V_use;//有用非终结符集合
    QVector<Production> pTemp;
    QSet<QChar> vTemp;
    bool flag;
    //先删除非产生的式子和集合元素
    //遍历第一次找到A->a的可产生变量
    for (auto i :noSinglePro) { //遍历上一次处理的产生式
         if (isInSet(i.right, T_set)&&(!vTemp.contains(i.left))) //判断右边都是终结符
                 vTemp.insert(i.left);//添加可产生的变量
    }
     //循环迭代，反复查找在右边是否有可产生的变量，并添加
     do{
         flag=false;
         for (auto i :noSinglePro) { //遍历产生式
             if (isInSet(i.right, vTemp+T_set)&&(!vTemp.contains(i.left))){//判断右边只有可产生的变量和终结符
                 flag=true;
                 vTemp.insert(i.left);//添加可产生的变量
             }
         }
     }while(flag);
     for (auto i :noSinglePro) { //添加有用产生式
         if (isInSet(i.left,vTemp)&&isInSet(i.right,vTemp+T_set))
         pTemp.append(i);
     }
     //再删除不可达的式子和集合元素
     //首先放入S
     V_use.insert('S');
     do{
         flag=false;
         for (auto i :pTemp) //遍历产生式
             if(isInSet(i.left,V_use))//左边在可达的变量集合中
                 for(auto k:i.right){
                     if(isInSet(k,T_set)&&(!T_use.contains(k))){//把右边终结符放到可达终结符集合中
                         flag=true;
                         T_use.insert(k);
                     }
                     if(isInSet(k,vTemp)&&(!V_use.contains(k))){//把右边非终结符放到可达非终结符集合中
                         flag=true;
                         V_use.insert(k);
                     }
                }
         } while(flag);
       for (auto i :pTemp) { //添加有用产生式
           if (isInSet(i.left,V_use)&&isInSet(i.right,V_use+T_use))
           useProducts.append(i);
       }
       //删除符号向量中未出现的字符
       QVector<QChar> temp=T_vec;
       for(QVector<QChar>::iterator i=T_vec.begin();i!=T_vec.begin();i++){
           if(!T_use.contains(*i))
               temp.erase(i);
       }
       T_vec=temp;
       temp=V_vec;
       for(QVector<QChar>::iterator i=V_vec.begin();i!=V_vec.begin();i++){
           if(!V_use.contains(*i))
               temp.erase(i);
       }
       V_vec=temp;
}
