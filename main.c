#include <stdio.h>
#include <stdlib.h>

typedef struct Haffmantree{
    struct Haffmantree* Right;
    int data;
    struct Haffmantree* Left;
}Haff;

typedef struct HaffHeap{
    Haff* pdata;//存储元素的动态数组
    int capacity;//容量
}HaffHeap;


//思路:两个数组，一个是包含所给数的最小堆HaffHeap[0](void*)，一个是包含访问过的树的数组HaffHeap[1](void*)，数组里面的元素都是Haff
//capacity是数组容量（固定）
//出堆：HaffHeap[0]中出堆一个元素，放入HaffHeap[1]中，并返回其地址
//入堆：HaffHeap[0]中增加一个元素，值是出堆两个元素之和，并附有左右儿子的指针
//结束：直到HaffHeap[0]只剩一个元素。其他元素全部通过指针链接到那一个元素中。

///出堆：HaffHeap[0]中出堆一个元素，放入HaffHeap[1]中，并返回其地址
Haff* OutHeap(HaffHeap** H){
    Haff temp=H[0]->pdata[1];//H[0]最小元素
    H[1]->pdata[H[1]->capacity++]=temp;//放入H[1]中


    H[0]->pdata[1]=H[0]->pdata[H[0]->capacity--];//H[0]最大元素放在首部
    //把换到第一位的pdata[size]放到合适的位置
    int i=1;
    for(;;){
        if(i>=H[0]->capacity){
            break;
        }
        if((H[0]->pdata[i].data)>(H[0]->pdata[2*i].data)){//如果父节点大于子节点，则交换
            if((H[0]->pdata[2*i+1].data)<(H[0]->pdata[2*i].data)){
                temp=H[0]->pdata[i];
                H[0]->pdata[i]=H[0]->pdata[2*i+1];
                H[0]->pdata[2*i+1]=temp;
                continue;
            }
            temp=H[0]->pdata[i];
            H[0]->pdata[i]=H[0]->pdata[2*i];
            H[0]->pdata[2*i]=temp;
        }else{
            break;
        }
        i=i*2;
    }
    return &H[1]->pdata[H[1]->capacity-1];
}

///建立一个哈夫曼树
Haff* BuildHaffTree(HaffHeap** H){
    Haff* NewHaff=malloc((H[0]->capacity)*sizeof(Haff));//NewHaff是包含capacity个Haff元素的数列
    int i=0;
    Haff* temp1;
    Haff* temp2;
    while(H[0]->capacity!=1){
        printf("\n--------------\n");
        temp1=OutHeap(H);
        temp2=OutHeap(H);
        printf("temp1=%d\n",temp1->data);
        printf("temp2=%d\n",temp2->data);
        NewHaff[i].data=temp1->data+temp2->data;
        NewHaff[i].Left=temp1;
        NewHaff[i].Right=temp2;
        printf("NewHaff=%d\n",NewHaff[i].data);
        EnterHeap(&NewHaff[i],H);
        printf("\n哈夫曼堆:\n");
        Output(H[0]);
        printf("\n哈夫曼数组:\n");
        OutputArray(H[1]);
        i++;
    }
    return &H[0]->pdata[1];
}

///创建两个二维哈夫曼数组(HaffHeap[0] HaffHeap[1])，并返回
HaffHeap** CreateHaffHeap(int capacity){
    int i=1;//char转int需要的权重
    int j=1;//全局的计数
    char c;////char转int的char数
    int n=0;//char转int的int数
    int temp;
    HaffHeap** NewHaffHeap=malloc(2*sizeof(HaffHeap*));//新建一个数组指针 NewHaffHeap(HaffHeap*)
    NewHaffHeap[0]=malloc(sizeof(HaffHeap));//哈夫曼堆：  NewHaffHeap[0](HaffHeap*)
    NewHaffHeap[1]=malloc(sizeof(HaffHeap));//哈夫曼数组：NewHaffHeap[0](HaffHeap*)

    NewHaffHeap[0]->pdata=malloc((2*capacity)*sizeof(Haff));//给NewHaffHeap[0]新建capacity*2个Haff元素
    NewHaffHeap[1]->pdata=malloc((2*capacity)*sizeof(Haff));//给NewHaffHeap[1]新建capacity*2个Haff元素

    NewHaffHeap[0]->pdata[0].data=0;//哨兵

    for(int r=0;r<=capacity;r++){//初始化指针
        NewHaffHeap[0]->pdata[r].Left=NULL;
        NewHaffHeap[0]->pdata[r].Right=NULL;
        NewHaffHeap[1]->pdata[r].Left=NULL;
        NewHaffHeap[1]->pdata[r].Right=NULL;
    }

    NewHaffHeap[0]->capacity=capacity;//哈夫曼堆：  NewHaffHeap[0](HaffHeap*)
    NewHaffHeap[1]->capacity=0;//哈夫曼数组：NewHaffHeap[0](HaffHeap*)

    //创建一个无序的新堆
    printf("请依次输入%d个数以组成哈夫曼树\n",capacity);
    while((c=getchar())!=EOF){
        if(c=='\n'){
            printf("n=%d\n",n);
            NewHaffHeap[0]->pdata[j++].data=n;
            n=0;
            i=1;
            continue;
        }
        n=n*i+c-'0';
        i=10;
    }

    //给无序的新堆排序
    i=NewHaffHeap[0]->capacity/2;
    for(;i>0;i--){//从序号i处开始做“下沉”操作
        Sink(NewHaffHeap[0],i);
    }
    //Output(NewHaffHeap[0]);
    return NewHaffHeap;
}


///带数创建堆时用到的函数“下沉”
//如果节点比左右儿子都大，把节点和其左右子节点中较小的那个交换。。。直到为空
void Sink(HaffHeap* H, int n){
    int temp;
    if(2*n>H->capacity){//1. 没有子节点
        return;
    }else if(2*n==H->capacity){//2. 有且只有一个左儿子
        if(H->pdata[n].data>H->pdata[2*n].data){//2.1 子节点比原节点小
            temp=H->pdata[n].data;
            H->pdata[n].data=H->pdata[n*2].data;
            H->pdata[n*2].data=temp;
            return;
        }
    }else if(2*n<H->capacity){//3. 有左右儿子
        if(H->pdata[2*n].data<=H->pdata[2*n+1].data){//3.1 左儿子比较小
            if(H->pdata[n].data>H->pdata[n*2].data){//原节点比左儿子大
                temp=H->pdata[n].data;
                H->pdata[n].data=H->pdata[n*2].data;
                H->pdata[n*2].data=temp;
                Sink(H,n*2);
            }
        }else if(H->pdata[2*n].data>H->pdata[2*n+1].data){//3.2 右儿子比较小
            if(H->pdata[n].data>H->pdata[n*2+1].data){//原节点比右儿子大
                temp=H->pdata[n].data;
                H->pdata[n].data=H->pdata[n*2+1].data;
                H->pdata[n*2+1].data=temp;
                Sink(H,n*2+1);
            }
        }
    }
}

///以完全二叉树的形式输出动态数组
void Output(HaffHeap* H){
    printf("\n");
    int limt=1;
    int j=1;//总的计数值(1~size)
    int i=1;//每层树的计数值(1~limt)
    for(;;){

        for(;i<=limt;i++,j++){
            printf("[%d]%d ",j,H->pdata[j].data);
            if(j==H->capacity){
                return;
            }
        }
        i=1;
        printf("\n\n");
        limt=limt*2;
    }
    printf("\n");
}

///输出数组
void OutputArray(HaffHeap* H){
    printf("\n");
    int i=0;//总的计数值(1~size)
    for(;i<H->capacity;i++){
        printf("[%d]%d ",i,H->pdata[i].data);
    }
    printf("\n");
}

///一个哈夫曼树的先序遍历
void PreReadHaff(Haff* R){

    if(R==NULL){
        return -1;
    }
    printf("%d\n",R->data);
    PreReadHaff(R->Left);
    PreReadHaff(R->Right);
}



///入堆：HaffHeap[0]中增加一个元素并排序
void EnterHeap(Haff* NewHaff,HaffHeap** H){
    int i=++H[0]->capacity;
    Haff temp;
    H[0]->pdata[i]=*NewHaff;
    while(H[0]->pdata[i/2].data>NewHaff->data){
        temp=H[0]->pdata[i/2];
        H[0]->pdata[i/2]=H[0]->pdata[i];//pdata[i/2]:父节点
        H[0]->pdata[i]=temp;
        i=i/2;
    }
}

///求WPL路径长度
//思路：WPL=Σ深度*树值
int WPL(Haff* HTree,int depth){
    depth++;
    //printf("本轮HT=%d，深度=%d\n",HTree->data,depth);

    if(!HTree->Left&&!HTree->Right){//到头了
        printf("HT=%d，深度=%d\n",HTree->data,depth);
        return depth*HTree->data;
    }
    int WPL1=WPL(HTree->Left,depth);
    int WPL2=WPL(HTree->Right,depth);
    //printf("WPL=%d\n",WPL1+WPL2);
    return WPL1+WPL2;
}



int main()
{
    Haff* Haffmantree=BuildHaffTree(CreateHaffHeap(5));
    PreReadHaff(Haffmantree);
    printf("WPL=%d",WPL(Haffmantree,0));
}
