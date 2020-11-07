#include"GrammerAnalyzer.h"
void PDA::generateRule(QVector<GNFProduction> g3)
{
   for(auto i:g3)
   {   QVector<QString>temp;
       current_input s(i.right[0],i.left);
       //s.stack_ch=i.left;
      // s.input_ch=i.right[0];
       if(i.right.length()==1)
       {
           temp.clear();
           rule[s].insert(temp);
       }
       else
        {
           i.right.removeFirst();
           temp+=i.right;
           rule[s].insert(temp);
       }
   }
}
void PDA::initialPDA(QSet<QString> t_set, QVector<GNFProduction> g3)
{
    this->t_set=t_set;
    this->g3=g3;
    generateRule(g3);
    printRule();
    //压进初始元素
    Stack.push("A1");

}
void PDA::printRule(){
    for(auto i:rule.keys())
    {  for(auto j:rule[i])
        {
            QString temp;
         for(auto k:j)
         {
            temp+=k;
         }
         qDebug()<<"(q0,"<<i.input_ch<<","<<i.stack_ch<<")=(q0,"<<temp<<")";
        }
    }
}

QVector<QVector<QString>> PDA::delta(QString input_chr,QString stack_top){
    QVector<QVector<QString>> re;
    QVector<QString> temp;

    if(input_chr=="#"){
        for(auto i:g3){
            if(i.left==stack_top){
                re.append(i.right);
            }
        }
    }
    if(input_chr==stack_top){
        temp.append("#");
        re.append(temp);
    }
    return re;
}


bool PDA::inference(QString str,int ptr, QStack<QString> stack, int count)
{
    bool flag = false;
    QVector<QVector<QString>> deltas;
    if(!str.length())
        return false;
    for(auto i:str){
        if(!this->t_set.contains(i))
            return false;
    }
    //如果匹配完成,且栈内没有残留的非终结符,且栈内符号都能变成ε,则可以清空栈
   if (ptr == str.length()&&count==0) {
       //因为count=0,所以栈内都是非终结符
       QStack<QString> stk = stack;

       while (!stk.empty()){

           bool epsilonFlag=false;
           QString tos = stk.pop();
           deltas = delta(EPSILON, tos);
           for (auto delta : deltas) {
               if (delta[0]==EPSILON){
                   epsilonFlag=true;
                   break;
               }
           }
           if (!epsilonFlag){
               return false;
           }
       }
       return true;
   }

    if (!stack.empty()) {
        if (!t_set.contains(stack.top())) {
            //空移动
            QString tos = stack.pop();
            deltas=delta(EPSILON,tos);
            for (auto delta : deltas) { //遍历所有的表达式
                QStack<QString> stk=stack;
                int cnt = count;
                for (int i = 0; i < delta.length(); ++i) {
                    //从右向左入栈
                    if (this->t_set.contains(delta[delta.length() - i - 1])) {//如果是终结符
                        cnt++;
                    }
                    stk.push(delta[delta.length() - i - 1]);
                }
                if (cnt > str.length() - ptr) {
                    return false;
                }
                flag = inference(str, ptr, stk, cnt);
                if (flag) {
                    break;
                }
            }
        } else {
            //判断是否符合输入串
            if (stack.top() == str[ptr]) {
                QStack<QString> stk=stack;
                stk.pop();
                ptr++;
                count--;
                flag=inference(str,ptr,stk,count);
            } else {
                return false;
            }
        }
    }
    return flag;
}
