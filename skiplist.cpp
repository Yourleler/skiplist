//严格跳表，空间复杂度O(nlogn)，增删查时间复杂度O(logn)  初始化、重置O(nlogn)  空间复杂度O(nlogn)
//有序数组上的插入和删除时间复杂度O(n),因为要移动数组元素，查找是O(logn)  其他堆和二叉树什么的是O(logn)
#include "skiplist.h"
//template <class K, class E>
//Strictskiplist<K, E>::Strictskiplist(K maxkey, int maxnum, double prob)
//{
//    cutoff = prob;
//    maxlevel = static_cast<int>(log(maxnum + 1) / log(2)) + 1; //严格跳表每层是下一层的1/2，所以层高应为log_2(n+1)+1取整  以m为底：log(n)/log(m)
//    levels = 0;
//    dsize = 0;
//    tailKey = maxkey;
//    tailNode = new skipnode<K, E>({ maxkey, 0 }, maxlevel);
//    headerNode = new skipnode<K, E>({ maxkey, 0 }, maxlevel);
//    for (int i = 1; i <= maxlevel; ++i)
//        headerNode->next[i] = tailNode;
//    for (int i = 1; i <= maxlevel; ++i)
//        tailNode->next[i] = nullptr;//头尾结点的初始化
//    last = new skipnode<K, E> *[int(maxlevel + 1)]; // 0层不用，＋1
//    for (int i = 1; i <= maxlevel; ++i)
//        last[i] = nullptr;//初始化last数组
//}

template <class K, class E>
Strictskiplist<K, E>::Strictskiplist(int q,vector<pair<K, E>>arr, K maxkey, int maxnum, double prob)
{
    cutoff = prob;
    maxlevel = static_cast<int>(log(maxnum + 1) / log(2)) + 1; //严格跳表每层是下一层的1/2，所以层高应为log_2(n+1)+1取整  以m为底：log(n)/log(m)
    levels = 0;
    dsize = 0;
    tailKey = maxkey;
    tailNode = new skipnode<K, E>({ maxkey, 0 }, maxlevel);
    headerNode = new skipnode<K, E>({ maxkey, 0 }, maxlevel);
    for (int i = 1; i <= maxlevel; ++i)
        headerNode->next[i] = tailNode;
    for (int i = 1; i <= maxlevel; ++i)
        tailNode->next[i] = nullptr;//头尾结点的初始化
    last = new skipnode<K, E> *[int(maxlevel + 1)]; // 0层不用，＋1
    for (int i = 1; i <= maxlevel; ++i)
        last[i] = nullptr;//初始化last数组
    if (q == 1)
    {
        for (int i = 1; i <= int(arr.size() - 1); i++)//－1是因为arr[0]不用
        {
            int levelnew = getnewlevel(i) + 1;
            pair<K, E> temp = arr[i];
            insert1(temp, levelnew);
        }//以严格跳表形式插入，初始化
    }
    else
    {
        for (int i = 1; i <= int(arr.size() - 1); i++)//－1是因为arr[0]不用
        {
            int levelnew = getnewlevel(i) + 1;
            pair<K, E> temp = arr[i];
            insert1(temp, levelnew);
        }//以严格跳表形式插入，初始化
    }

}
template <class K, class E>
skipnode<K, E>* Strictskiplist<K, E>::search(K thekey)
{
    //组织last数组，返回节点
    skipnode<K, E>* beforeNode = headerNode;
    for (int i = levels; i >= 1; i--)
    {
        while (beforeNode->next[i]->element.first < thekey)
        {
            beforeNode = beforeNode->next[i];
            control++;
        }
        last[i] = beforeNode;
        control++;
    }
    return beforeNode->next[1]; //返回最后一个小于key的元素的下一个节点,跳表是从第1层开始
}

template <class K, class E>
skipnode<K, E>* Strictskiplist<K, E>::search1(K thekey)
{
    //组织last数组，返回节点
    skipnode<K, E>* beforeNode = headerNode;
    for (int i = levels; i >= 1; i--)
    {
        while (beforeNode->next[i]->element.first <= thekey)
        {
            beforeNode = beforeNode->next[i];
            control++;
            if (beforeNode->element.first == thekey)
                return beforeNode;

        }
        //control++;
    }
    return beforeNode->next[1]; //返回最后一个小于key的元素的下一个节点,跳表是从第1层开始
}

template <class K, class E>
pair<K, E>* Strictskiplist<K, E>::find(const K& thekey)
{ //按照关键字查找,返回匹配的数对的指针，没有的话返回null
    if (thekey >= tailKey)
        return NULL;
    skipnode<K, E>* cur = search(thekey);
    if (cur != NULL && cur->element.first == thekey)
        return &(cur->element);
    return NULL;
}

template <class K, class E>
int Strictskiplist<K, E>::getlevel()
{
    //层级,最大不能超过跳表规定的最大层数
    int lev = 1;

    while ((double(rand() % 999)) / 999 < cutoff && lev < maxlevel)
        lev++;
    return lev;
}

template <class K, class E>
void Strictskiplist<K, E>::insert(pair<K, E> thepair, int level)
{ //如果没有指定层级，则随机分配一个层数
  //如果给定了层数，则按照所以给定的层数进行插入
  //对于给定的层数，如果大于跳表规定的最大层数，则按照最大层数进行插入
    skipnode<K, E>* cur = search(thepair.first);
    if ((cur->element.first)==(thepair.first))
    {
        cur->element.second = thepair.second;
        return;
    }
    int thelevel = level;
    if (thelevel == 0)
        thelevel = getlevel();
    else
        thelevel = level;
    if (thelevel > levels) //比现有的层数大，向上加
    {
        for (int i = levels + 1; i <= min(thelevel, maxlevel); i++)
            last[i] = headerNode;
        levels = min(thelevel, maxlevel);
        thelevel = levels;
    }
    skipnode<K, E>* newnode = new skipnode<K, E>(thepair, thelevel);
    for (int i = 1; i <= thelevel; ++i) //插入，thelevel现在是插入这个节点会影响的最高层，从此至下更改
    {
        newnode->next[i] = last[i]->next[i];
        last[i]->next[i] = newnode;
    }
    dsize++;
}

template <class K, class E>
void Strictskiplist<K, E>::insert1(pair<K, E> thepair, int level)
{ //如果没有指定层级，则随机分配一个层数
  //如果给定了层数，则按照所以给定的层数进行插入
  //对于给定的层数，如果大于跳表规定的最大层数，则按照最大层数进行插入
    skipnode<K, E>* cur = search(thepair.first);
    /*if ((cur->element.first)==(thepair.first))
    {
        cur->element.second = thepair.second;
        return;
    }*/
    int thelevel = level;
    if (thelevel == 0)
        thelevel = getlevel();
    else
        thelevel = level;
    if (thelevel > levels) //比现有的层数大，向上加
    {
        for (int i = levels + 1; i <= min(thelevel, maxlevel); i++)
            last[i] = headerNode;
        levels = min(thelevel, maxlevel);
        thelevel = levels;
    }
    skipnode<K, E>* newnode = new skipnode<K, E>(thepair, thelevel);
    for (int i = 1; i <= thelevel; ++i) //插入，thelevel现在是插入这个节点会影响的最高层，从此至下更改
    {
        newnode->next[i] = last[i]->next[i];
        last[i]->next[i] = newnode;
    }
    dsize++;
}



template <class K, class E>
void Strictskiplist<K, E>::erase(const K& key)
{
    if (key >= tailKey || dsize == 0) //如果越界
        return;
    skipnode<K, E>* cur = search(key);
    if (cur->element.first != key) //没有这个节点
        return;
    for (int i = 1; i <= levels && last[i]->next[i] == cur; i++)
        last[i]->next[i] = cur->next[i]; //把所有和此节点直接相连的节点更新

    while (levels > 0 && headerNode->next[levels] == tailNode) //所有头尾直接相连的空层级消掉
        --levels;

    delete cur;
    dsize--;
}

template <class K, class E>
void Strictskiplist<K, E>::erasemax()
{
    search(this->tailKey);
    erase(last[1]->element.first); //升序排列，直接找最大
}

template <class K, class E>
void Strictskiplist<K, E>::erasemin()
{
    erase(headerNode->next[1]->element.first);
}

template <class K, class E>
int Strictskiplist<K, E>::getnewlevel(int x)
{
    int count = 0; //计算层级
    while (1)
    {

        if (x % 2 == 1)//是奇数选中进入上层，然后/2来制造上一层也是一半奇一半偶
        {
            x /= 2;
            count++;
        }
        else//偶数的话直接停止
            return count;
    }
}

template <class K, class E>
void Strictskiplist<K, E>::clear()
{ //清空跳表函数
    skipnode<K, E>* cur = headerNode->next[1];
    while (cur != NULL && cur != tailNode)
    {
        skipnode<K, E>* temp = cur->next[1];
        delete cur;
        cur = temp;
    }//所有节点删除
    dsize = 0;
    for (int i = 0; i < int(headerNode->next.size()); ++i)
        headerNode->next[i] = tailNode;//让头的每层指向尾
}
template <class K, class E>
Strictskiplist<K, E>::~Strictskiplist()
{ //析构函数
    clear();
}


template <class K, class E>
void Strictskiplist<K, E>::reset()
{ //跳表重构函数
    vector<pair<K, E>> arr(dsize + 1);//arr[0]不用
    skipnode<K, E>* cur = headerNode->next[1];
    for (int i = 1; i <= dsize; ++i)
    {
        arr[i] = cur->element;
        cur = cur->next[1]; //arr存每一个元素的值的大小
    }
    clear();
    for (int i = 1; i <= int(arr.size() - 1); i++)//－1是因为arr[0]不用   时间复杂度这层是n，每次有一个insert,O（n*logn）
    {
        int levelnew = getnewlevel(i) + 1;
        insert(arr[i], levelnew);
    }
}



template<class K, class E>
void Strictskiplist<K, E>::output()
{//跳表打印函数
    if (dsize == 0)
        return;
    skipnode<K, E>* pNode = headerNode;
    pair<int, int>* lev = new pair<int, int>[dsize + 10];
    for (int i = 0; pNode; ++i)
    {
        lev[i] = { pNode->element.first,(int)pNode->next.size() - 1 };
        pNode = pNode->next[1];
    }
    //lev数组下标的范围是从0到dsize+1 其中0表示headerNode，dsize+1表示tailNode
    for (int j = levels; j >= 1; j--)
    {
        cout << "第 " << j << " 层 : headerNode-->";
        for (int i = 1; i <= dsize; ++i)//此循环用于遍历每一个元素，dsize表示跳表中的元素个数。
        {
            if (lev[i].second >= j)//在每个元素中，通过检查 lev[i].second >= j 确定该元素是否位于当前层。如果是，打印元素的关键字，否则打印横线表示空缺。
                cout << lev[i].first << "-->";
            else
            {
                int temp = static_cast<int>(to_string(lev[i].first).size());
                for (int i = 1; i <= temp; ++i)
                    cout << "-";//temp 表示节点值的位数，然后在循环中输出相应数量的短横线
                cout << "---";
            }
        }
        cout << "tailNode" << endl;
    }
}

////验证正确性的mian
//int main()
//{
//    
//    // 初始化随机数生成器的种子
//    srand((unsigned int)time(NULL));
//    cout << "输入5数字，分别是初始元素个数，最大元素值，要增删查的三个数" << endl;
//    int n;
//    cin >> n;//先插入n个
//    int m;
//    cin >> m;//最大数是m
//    vector<pair<int, int>> arr(n + 1);//用来初始化的
//    //for (int i = 0; i < n; i++)
//    //{
//    //    int a;
//    //    cin >> a;
//    //    arr[i + 1] = pair<int,int>(a,0);//arr[0]不用
//
//    //}//手动输入
//
//    for (int i = 0; i < n; ++i)
//    {
//        // 生成1至m之间的随机数
//        int num = (rand() % m) + 1;//生成随机数
//        arr[i + 1] = pair<int, int>(num, 0);
//    }
//
//    sort(arr.begin() + 1, arr.end());//排为正序
//
//    Strictskiplist<int, int>ss(1,arr, m, m);
//    //for (int i = 0; i < n; i++)
//    //{
//    //    int a;
//    //    cin >> a;
//    //    ss.insert(pair<int, int>(a, 0));
//
//    //}
//    //ss.reset();  //初始化笨办法
//    cout << "初始：" << endl;
//    ss.output();
//    int z = 0, s = 0, c = 0;
//    cin >> z >> s >> c;//增删查
//    cout << endl;
//
//    ss.control = 0;
//    ss.insert({z, 0});
//    cout << "插入" << z << "时遍历了" << ss.control << "个节点" << endl;
//    ss.control = 0;
//    cout << "跳表：" << endl;
//    ss.output();
//    cout << endl;
//
//
//    ss.erase(s);
//    cout << "删除" << s << "时遍历了" << ss.control << "个节点" << endl;
//    ss.control = 0;
//    cout << "跳表：" << endl;
//    ss.output();
//    cout << endl;
//
//
//    cin.clear();
//    pair<int, int>* aa = ss.find(c);
//    cout << "查找" << c << "时遍历了" << ss.control << "个节点" << endl;
//    if (aa == NULL)
//        cout << "未查找到" << endl;
//    ss.control = 0;
//    cout << "跳表：" << endl;
//    ss.output();
//    cout << endl;
//
//    ss.erasemax();
//    ss.erasemin();
//    cout << "删除最大最小后：" << endl;
//    ss.output();
//    cout << endl;
//
//    cout << "重置：" << endl;
//    ss.reset();
//    ss.output();
//}




////最后全随机的输出
//int main()
//{
//    // 初始化随机数生成器的种子
//    srand((unsigned int)time(NULL));
//
//    for (int run = 1; run <= 10; ++run)
//    {
//        int n;
//        //cin >> n; // 先插入n个
//        n = pow(2, run)*1024;//使得n和m是以2的(run+10)次方,如果最后每个操作平均是以个位数增加，说明是log_2(2^(run+10))=run+10,所以时间复杂度是O(log(n))
//        int m;
//        //cin >> m; // 最大数是m
//        m = 2147483640;
//
//        vector<pair<int, int>> arr(n + 1); // 用来初始化的
//
//        for (int i = 0; i < n; ++i)
//        {
//            // 生成1至m之间的随机数
//            int num = (rand() % m) + 1; // 生成随机数
//            arr[i + 1] = pair<int, int>(num, 0);
//        }
//
//        sort(arr.begin() + 1, arr.end()); // 排为正序
//
//        Strictskiplist<int, int> ss(0,arr, m, m,0.5); // 随机生成n个数，初始化为严格跳表
//        double arrinitsum = ss.control;
//        // 操作记录输出文件
//        string outputFile = "output" + std::to_string(run) + ".txt"; // 构建文件名
//
//        ofstream output(outputFile); // 打开文件用于输出结果
//
//        // 统计每种操作的总涉及节点数
//        double insertNodeCount = 0, eraseNodeCount = 0, findNodeCount = 0;
//        int insertCount = 0, eraseCount = 0, findCount = 0;
//
//        for (int i = 0; i < 3000; ++i)//3000个操作
//        {
//            int operationType = rand() % 3; // 随机选择操作类型：0为插入，1为删除，2为查找
//            int num = (rand() % m) + 1;//操作数
//
//            ss.control = 0; // 重置control为零
//
//            if (operationType == 0)
//            {
//                // 插入操作
//                ss.insert1(pair<int, int>(num, 0));
//                //output << "Insert " << num << ": " << ss.control << " nodes" << endl;
//                insertNodeCount += ss.control;
//                insertCount++;
//                if (ss.control > ss.dsize * 0.2)
//                    ss.reset();
//            }
//            else if (operationType == 1)
//            {
//                // 删除操作
//                ss.erase(num);
//                //output << "Erase " << num << ": " << ss.control << " nodes" << endl;
//                eraseNodeCount += ss.control;
//                eraseCount++;
//
//                if (ss.control > ss.dsize * 0.2)
//                    ss.reset();
//            }
//            else
//            {
//                // 查找操作
//                pair<int, int>* result = ss.find(num);
//                //if (result == NULL)
//                //    output << "Find " << num << ": Not Found, " << ss.control << " nodes" << endl;
//                //else
//                //    output << "Find " << num << ": " << result->first << ", " << ss.control << " nodes" << endl;
//                findNodeCount += ss.control;
//                findCount++;
//
//                if (ss.control > ss.dsize * 0.2)
//                    ss.reset();
//            }
//        }
//
//        // 输出每种操作的总涉及节点数
//        output << "n= " << n << endl;
//        output << "平均初始化时遍历节点数： " << arrinitsum/n << endl;//平均是以个位数增加,说明是n*log_2(2^(run+10))/n=run+10*n/n,(/n是因为输出本来就/n了)说明时间复杂度是O(nlogn)
//        output << "平均插入时遍历节点数: " << insertNodeCount/insertCount << endl;//最后每个操作平均是以个位数增加，说明是log_2(2^(run+10))=run+10,所以时间复杂度是O(log(n))
//        output << "平均删除时遍历节点数: " << eraseNodeCount / eraseCount << endl;
//        output << "平均查找时遍历节点数: " << findNodeCount/findCount << endl;
//
//        output.close(); // 关闭文件
//    }
//
//    // 将文件拼接起来
//    ofstream finalOutput("final_output.txt");
//    for (int run = 1; run <= 10; ++run)
//    {
//        std::string inputFile = "output" + std::to_string(run) + ".txt";
//        ifstream input(inputFile);
//        finalOutput << input.rdbuf();
//        if (run < 10) // 不是最后一次循环时，加入换行符
//            finalOutput << endl;
//        input.close();
//    }
//    finalOutput.close();
//
//    return 0;
//}   //全部随机的时候操作用





//测试重置时机的时候从固定文件读取用

int main() 
{
    std::string outputFileName = "test.txt";
    std::ofstream output(outputFileName); // 打开文件用于输出结果
    srand(static_cast<unsigned int>(time(nullptr)));

    const int numOperations = 3000;
    const int maxValue = 1000; // 最大操作数值
    
    //生成测试集：
    std::ofstream outputFile("input.txt");
    outputFile << 1000 << " " << 100000 << " ";//1000个数，最大数100000   接着是30000个数对a b，a是operationType0为插入，1为删除，2为查找，b是操作数，对应num
    for (int i = 0; i < numOperations; ++i) {
        int operationType = rand() % 3;
        int operand = rand() % maxValue + 1;

        outputFile << operationType << " " << operand << " ";
    }
    //生成完了
    outputFile.close();

    //开始找什么时候reset合适
    for (double run = 0.0; run <= 1; run+=0.01)  
    {
        auto start = std::chrono::high_resolution_clock::now(); // 记录开始时间
        // 构建输入文件名
        std::string inputFileName = "input.txt";//从文件中读取，替代随机生成，来控制变量测试重置时应该取多少参考值

        std::ifstream inputFile(inputFileName);
        if (!inputFile.is_open()) {
            std::cerr << "Error opening input file." << std::endl;
            return 1;
        }

        int n, m;
        inputFile >> n >> m;

        std::vector<std::pair<int, int>> arr(n + 1); // 用来初始化的

        for (int i = 0; i < n; ++i) {
            // 生成1至m之间的随机数
            int num = (rand() % m) + 1; // 生成随机数
            arr[i + 1] = std::make_pair(num, 0);
        }

        std::sort(arr.begin() + 1, arr.end()); // 排为正序

        Strictskiplist<int, int> ss(0,arr, m, m); // 随机生成n个数，初始化为严格跳表

        // 构建输出文件名
        

        // 统计每种操作的总涉及节点数
        int insertNodeCount = 0, eraseNodeCount = 0, findNodeCount = 0;

        for (int i = 0; i < 3000; ++i) {
            int operationType, num;
            inputFile >> operationType >> num;

            ss.control = 0; // 重置control为零

            if (operationType == 0) {
                // 插入操作
                ss.insert(std::make_pair(num, 0));
                insertNodeCount += ss.control;
                if (ss.control > ss.dsize * run)
                    ss.reset();
            }
            else if (operationType == 1) {
                // 删除操作
                ss.erase(num);
                eraseNodeCount += ss.control;
                if (ss.control > ss.dsize * run)
                    ss.reset();
            }
            else {
                // 查找操作
                pair<int, int>* result = ss.find(num);
                findNodeCount += ss.control;
                if (ss.control > ss.dsize * run)
                    ss.reset();
            }
        }


        auto end = std::chrono::high_resolution_clock::now(); // 记录结束时间
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start); // 计算耗时

        /*if (duration.count() != 12)
            continue;*/
        // 输出每种操作的总涉及节点数
        output << run << std::endl;//当参考值为run时
        output << "Time: " << duration.count() << " milliseconds" << std::endl;//花费时间
        //output << "Insert Node Count: " << insertNodeCount << std::endl;
        //output << "Erase Node Count: " << eraseNodeCount << std::endl;
        //output << "Find Node Count: " << findNodeCount << std::endl;
        output << findNodeCount + eraseNodeCount + insertNodeCount<<endl;//总涉及节点
        output << std::endl;
        
        inputFile.close(); // 关闭输入文件
    }

    output.close(); // 关闭文件

    return 0;
}

