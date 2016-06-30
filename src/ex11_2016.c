/*
 ============================================================================
 Name        : ex01-2016.c
 Author      : Kosuke Shiromoto b5122025
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <time.h>

//clock_t clock(void);

typedef struct{
	float GPA;
	int credit;
	char name[200];
}REC;

typedef struct cell{
	REC val;
	struct cell *next;
}CELL;

typedef struct node{
	REC val;
	struct node *left,*right;
}NODE;


/*入力関数*/
CELL *input_func(char *INPUTFILE);
CELL *make_cell(REC _val,CELL *cp);

CELL *sort_func(CELL *_head,int (*_compare_func)(const void *comp1,const void *comp2));
CELL *insert_cell(CELL *sort_head,CELL *insert_cell);

NODE *make_tree(CELL *list_head,int (*_compare_func)(const void *comp1,const void *comp2));
NODE *insert_node(CELL *p,NODE *tree_head,int (*_compare_func)(const void *comp1,const void *comp2));
void inorder(NODE *p);

int compare_GPA(const void *comp1,const void *comp2);
int compare_credit(const void *comp1,const void *comp2);
int compare_name(const void *comp1,const void *comp2);

/*出力関数*/
void output_func(char *OUTPUTFILE,CELL *_head);

int main(int argc,char *argv[])
{
	CELL *head;
	//	CELL *sort_head;
	NODE *tree_head;
	int sort_mode=0;
	int (*compare_func)(const void *comp1,const void *comp2);

	/*コマンドライン引数についてのエラー処理*/
	if(argc != 4){
		printf("引数が少ないまたは多いです。実行構成を確認してください。\n");
		return 0;
	}

	/*ソート対象読み取り*/
	if(strcmp(argv[1],"GPA")==0){
		compare_func=compare_GPA;
		sort_mode=1;
	}
	else if(strcmp(argv[1],"credit")==0){
		compare_func=compare_credit;
		sort_mode=2;
	}
	else if(strcmp(argv[1],"name")==0){
		compare_func=compare_name;
		sort_mode=3;
	}
	else{
		puts("コマンドラインの第一引数には「GPA」「cregit」「name」のいずれかを入力してください。\n");
		return 0;
	}

	/*先頭要素のアドレスを返す*/
	head = input_func(argv[2]);
	if(head==0){
		puts("マイナス値が入っています。入力ファイルを確認してください。\n");
		return 0;
	}

	tree_head=make_tree(head,compare_func);
	puts("inorder");
	inorder(tree_head);

	puts("end");
	//    /*ソート処理*/
	//    sort_head = sort_func(head,compare_func);
	//
	//    /*出力ファイルに出力*/
	//    output_func(argv[3],sort_head);
	//
	//    switch(sort_mode){
	//    case 1:
	//        puts("GPAでソートしました。\n");
	//        break;
	//    case 2:
	//        puts("creditでソートしました。\n");
	//        break;
	//    case 3:
	//        puts("nameでソートしました。\n");
	//        break;
	//    }
	return 0;
}

CELL *input_func(char *INPUTFILE){
	FILE *ifp;
	CELL _head;//先頭セルの実体
	CELL *tail;//最後尾を常に追う
	CELL *new;//挿入されたセルへのポインタ
	REC new_val;//挿入されたセルの中の値

	ifp = fopen(INPUTFILE,"r");
	/*エラー処理*/
	if( ifp == NULL ){
		printf( "入力ファイルが開けません。ファイルの場所や名前を確認してください。\n");
	}
	else{
		/*ダミー生成*/
		tail=&_head;

		_head.next=NULL;
		/*リスト生成*/
		while(!feof(ifp)){
			fscanf(ifp,"%f %d %s",&new_val.GPA,&new_val.credit,new_val.name);
			if(new_val.GPA<0||new_val.credit<0){
				return 0;
			}
			new=make_cell(new_val,NULL);//セル生成
			tail->next=new;//１つ前と新規セルをつなぐ
			tail=new;//最後尾更新
		}
		fclose(ifp);
	}
	return _head.next;//先頭セルへのポインタを返す
}

CELL *make_cell(REC _val,CELL *cp){
	CELL *new_cell = malloc(sizeof(CELL));
	if(new_cell!=0){
		new_cell->val=_val;
		new_cell->next=cp;
	}
	return new_cell;
}

NODE *make_node(REC _val){
	NODE *new_node = malloc(sizeof(NODE));
	if(new_node!=0){
		new_node->val=_val;
		new_node->left=NULL;
		new_node->right=NULL;
	}
	return new_node;
}

NODE *insert_node(CELL *p,NODE *tree_head,int (*_compare_func)(const void *comp1,const void *comp2)){
	NODE *q=tree_head;
	if(_compare_func(&p->val,&q->val)>=0){
		if(q->right==NULL){//tree先頭より大きければ右へ、その際右にNODEがなければNODE生成してループ終了
			q->right=make_node(p->val);
			printf("right %f\n",q->right->val.GPA);
			return NULL;
		}else{
			q=q->right;
			puts("right");
			return q;
		}
	}
	else{
		if(q->left==NULL){//小さい場合
			q->left=make_node(p->val);
			printf("left %f\n",q->left->val.GPA);
			return NULL;
		}else{
			q=q->left;
			puts("left");
			return q;
		}
	}
}

NODE *make_tree(CELL *list_head,int (*_compare_func)(const void *comp1,const void *comp2)){
	CELL *list_p=list_head;//list_head移動用
	NODE *tree_head=NULL;//tree先頭を指すポインタ
	NODE *tree_p;//tree移動用
	tree_p=tree_head;
	/*リストのセルを全て参照するまでループ*/
	while(list_p!=NULL){
		/*tree先頭にリスト先頭要素を格納*/
		if(tree_head==NULL){
			tree_head=make_node(list_p->val);
			tree_p=tree_head;
			printf("make %f\n",list_p->val.GPA);
		}
		else {
			/*insert関数でNULLが帰ってくるまで（make関数が呼び出されるまで）ループ*/
			tree_p=tree_head;
			while(tree_p!=NULL){
				tree_p=insert_node(list_p,tree_p,_compare_func);
			}
		}
		list_p=list_p->next;
	}
	puts("make_tree end\n");
	return tree_head;
}

void inorder(NODE *p){
	if(p==NULL){
		return;
	}
	inorder(p->left);
	printf("%f\n",p->val.GPA);
        free(p);
	inorder(p->right);
}

CELL *sort_func(CELL *_head,int (*_compare_func)(const void *comp1,const void *comp2)){
	CELL *MAX_cell=_head;
	CELL *MAX_cell_before;
	CELL *p=_head;//移動しながら参照
	CELL dummy;

	/*dummy初期化*/
	dummy.val.GPA=0;
	dummy.val.credit=0;
	int i=0;
	while(i<200){
		dummy.val.name[i]='\0';
		i++;
	}

	/*リスト要素数取得*/
	int element_num=0;
	while(p!=NULL){
		p=p->next;
		element_num++;
	}

	CELL *sort_head=malloc(sizeof(CELL));
	sort_head=NULL;
	dummy.next=_head;

	printf("sorting...\n");
	while(element_num>0){
		i=1;
		p=&dummy;
		MAX_cell=&dummy;
		MAX_cell_before=&dummy;
		while(i<=element_num){
			if(_compare_func(&p->next->val,&MAX_cell->val)>0){
				MAX_cell=p->next;
				MAX_cell_before=p;
			}
			i++;
			p=p->next;
		}
		sort_head=insert_cell(sort_head,MAX_cell);
		/*リストから削除*/
		p=MAX_cell_before->next;
		MAX_cell_before->next=p->next;
		free(p);

		element_num--;
	}
	return sort_head;
}

/*挿入（スタック）*/
CELL *insert_cell(CELL *sort_head,CELL *insert_cell){
	CELL *new_cell=make_cell(insert_cell->val,NULL);
	new_cell->next=sort_head;
	sort_head=new_cell;
	return sort_head;
}

int compare_GPA(const void *comp1,const void *comp2){
	float x,y;
	x=((REC*)comp1)->GPA;
	y=((REC*)comp2)->GPA;
	if(x-y>0)return 1;
	else if(x-y==0)return 0;
	else return -1;
}
int compare_credit(const void *comp1,const void *comp2){
	return ((REC*)comp1)->credit - ((REC*)comp2)->credit;
}
int compare_name(const void *comp1,const void *comp2){
	return strcmp(((REC*)comp1)->name,((REC*)comp2)->name);
}

void output_func(char *OUTPUTFILE,CELL *_head){
	FILE *ofp;
	CELL * delete;
	ofp = fopen(OUTPUTFILE,"w");
	if( ofp == NULL ){
		printf( "出力ファイルが開けません。ファイルの場所や名前を確認してください。\n");
	}
	else{
		while(_head!=NULL){
			fprintf(ofp,"%f %d %s\n",
					_head->val.GPA,_head->val.credit,_head->val.name);
			delete = _head;
			_head=_head->next;
			free(delete);
		}
	}
	fclose(ofp);
}
