/*
	���ݽṹ�γ���� A-��Դ������
*/
#include <iostream>
#include <string>
#include <cstdlib>
#include <windows.h>
#include <fstream>
using namespace std;
//��Դ������ע������:1.ֻ��ִ�����ṩ������Ĳ���
//                 2.����ϵͳ�趨Ϊ������������ļ�����������������ļ���
//                 3.����ϵͳ��������ȥ����CMD������Խ���ʹ�ã���Ҫ���ɾ����Ҫ�ļ�
//                 4.ϵͳ��Ŀ¼��ϵͳ�����ļ��е�Ŀ¼һ�£���������ȫ��Windows��������ļ�ϵͳ�����
//                  ��������ϵͳ��ֻ�ܿ���D: E: F:�̷��Լ����ڱ�ϵͳ������Щ�̷��´������ļ��л��ļ���
//                  Ҳ����˵ֻ��������ϵͳ��Ŀ¼�ṹ��winodwsϵͳ����ȫӳ�䣬�����ܽ�windowsϵͳĿ¼ӳ�䵽��ϵͳ
//                 5.**��Ҫ ÿ���˳�����������-1�����˳������ֱ�ӹرճ��򣬻ᵼ�����ݶ�ʧ

//�ֵܺ������洢�ļ�ϵͳ�γɵĶ����
typedef struct TreeNode{
  string fileName; //�ļ���
  int fileType; //�ļ�����  fileTypeΪ1�����ļ���  Ϊ0�����ļ�(.txt)
  int nodeLevel; //���Ĳ�Σ�ע�Ⲣ���������е���ȣ���Ϊ���õ��Ǻ����ֵܱ�ʾ�� ���������Ҫ��Ϊ������ļ�ϵͳĿ¼
  struct TreeNode *pre; //ǰ��
  struct TreeNode *parent; //����
  struct TreeNode *firstChild; //��һ������
  struct TreeNode *nextSibling; //��һ���ֵ�
}TreeNode, *Tree;

//�ýṹ���Ǵ洢 �ļ�ϵͳ�Ŀ�������ݣ�����ָ������ģ� ���� ���� �� ���� �ļ�ϵͳ
typedef struct MainData{
  string filename;
  int fileType;
  int nodeLevel;
}MainData;
MainData *preorder; //��������õ��Ŀ������������
MainData *midorder; //��������õ��Ŀ������������
int LEN; //���������еĳ���

//��������ջ��ʵ��
typedef struct Stack{
  Tree data;
  struct Stack *next;
}Stack, *LinkStack;

bool initStack(LinkStack &S);
bool Push(LinkStack &S, Tree T);
bool Pop(LinkStack &S, Tree &T);
bool getTop(LinkStack S, Tree &T);
bool isStackEmpty(LinkStack S);
void createFileSystem(Tree &T);
void createNewTreeNode(Tree &T, string name, int type, int level);
bool initFilePath(Tree &T);
void saveFileSystem(Tree T);
void saveFileSystemAgain(Tree T);
void traverseFilePath(Tree T);
Tree findFileNode(Tree T, string filename);
Tree findFileNode(Tree T, string parentname, string filename);
Tree findFirstNextEmptyRoot(Tree T);
bool checkSameName(Tree T, string filename, int type);
string getFilePath(Tree T, string filename);
string getFilePath(Tree T, string parentname,string filename);
bool createNewFile(Tree &T, string filename, string newfilename, int type);
bool deleteFile(Tree &T,string parentname, string defilename);
bool deleteFile(Tree &T, string defilename);
bool reNameFile(Tree &T, string filename, string newfilename);
bool reNameFile(Tree &T, string parentname,string filename, string newfilename);
bool lsFileChild(Tree T, string filename);
bool moveFile(Tree &T, string mvparentname, string mvfilename, string tofilename);
bool copyFile(Tree &T, string parentname, string cpfilename, string tofilename);
void readFile();
Tree readSystemFromTxt(MainData *preorder, MainData *midorder, int len);
void connectPreParent(Tree &T);
void checkLocalWindows(string cmd);
void menu();
void run(Tree &T);

int main()
{
  Tree fileSystemManager;
  fileSystemManager = new TreeNode;
  //ÿ���������ȶ���������ݣ��ٽ������ٻָ�˫�׺�ǰ��ָ���ϵ
  readFile();
  fileSystemManager = readSystemFromTxt(preorder, midorder, LEN);
  connectPreParent(fileSystemManager);
  //
  system("COLOR e");
  while(true)
  {
    run(fileSystemManager);
  }
}

//��ʼ����ջ
bool initStack(LinkStack &S)
{
  S = NULL;
  return true;
}

//��ջ
bool Push(LinkStack &S, Tree T)
{
  Stack *p = new Stack;
  p->data = T;
  p->next = S;
  S = p;
  return true;
}

//��ջ
bool Pop(LinkStack &S, Tree &T)
{
  if(S == NULL) return false;
  Stack *p = S;
  T = S->data;
  S = S->next;
  free(p);
  return true;
}

//���ջ��Ԫ��
bool getTop(LinkStack S, Tree &T)
{
  if(S != NULL)
  {
    T = S->data;
    return true;
  }
  else
    return false;
}

//��ջ��
bool isStackEmpty(LinkStack S)
{
  if(S == NULL)
    return true;
  else
    return false;
}

//�ļ�ϵͳ���Ĳ���
//�����ļ�ϵͳ ��������--�˵���
void createFileSystem(Tree &T) //���������ļ�ϵͳ��������rootĿ¼-�˵���
{
  T = new TreeNode;
  T->fileName = "�˵���";
  T->fileType = 1;
  T->pre = NULL;
  T->parent = NULL;
  T->firstChild = NULL;
  T->nextSibling = NULL;
  T->nodeLevel = 1;
}

//���ڴ��������Ľ��
void createNewTreeNode(Tree &T, string name, int type, int level)
{
  T = new TreeNode;
  T->fileName = name;
  T->fileType = type;
  T->firstChild = NULL;
  T->nextSibling = NULL;
  T->pre = NULL;
  T->parent = NULL;
  T->nodeLevel = level;
}

//��ʼ��ϵͳĿ¼ ��Ŀ¼�̶�  �˵��� D: E: F: //���������ݺ�windowsϵͳĿ¼����Ӧ�ˣ����Ե�������������³�ʼ���ļ�ϵͳ
bool initFilePath(Tree &T) //������Ŀ¼�µ���Ҫ�ļ��� ��Ҫ���ݳ�ʼ�ļ�Ŀ¼��ͼ����
{
  if(T == NULL)
  {
    cerr<<"�ļ�ϵͳ��Ŀ¼������,���Ƚ�����Ŀ¼(�˵���)!"<<endl;
    return false;
  }
  //D��
  TreeNode *D;
  createNewTreeNode(D, "D:", 1, T->nodeLevel+1);
  T->firstChild = D;
  D->pre = T;
  D->parent = T;
  //E��
  TreeNode *E;
  createNewTreeNode(E, "E:", 1, T->nodeLevel+1);
  D->nextSibling = E;
  E->pre = D;
  E->parent = T;
  //F��
  TreeNode *F;
  createNewTreeNode(F, "F:", 1, T->nodeLevel+1);
  E->nextSibling = F;
  F->pre = E;
  F->parent = T;
  return true;
}

//������� �ѵ�ǰ�ļ�ϵͳ��������ݴ浽txt��
void saveFileSystem(Tree T)
{
  LinkStack S;
  initStack(S);
  TreeNode *p = T;
  TreeNode *q = new TreeNode;
  ofstream output("fileone.txt",ios::app);
  if(!output) cerr<<"�ļ���ʧ�ܣ�";
  while(p || !isStackEmpty(S))
  {
    if(p)
    {
      Push(S, p);
      p = p->firstChild;
    }
    else
    {
      Pop(S, q);
      output<<q->fileName<<" "<<q->fileType<<" "<<q->nodeLevel<<" "<<"\n";
      p = q->nextSibling;
    }
  }
  output.close();
}

//������� �ѵ�ǰ�ļ�ϵͳ��������ݴ浽txt��
void saveFileSystemAgain(Tree T)
{
  LinkStack S;
  initStack(S);
  TreeNode *p = T;
  TreeNode *q = new TreeNode;
  ofstream output("filetwo.txt",ios::app);
  if(!output) cerr<<"�ļ���ʧ�ܣ�";
  while(p || !isStackEmpty(S))
  {
    if(p)
    {
      output<<p->fileName<<" "<<p->fileType<<" "<<p->nodeLevel<<" "<<"\n";
      Push(S, p);
      p = p->firstChild;
    }
    else
    {
      Pop(S, q);
      p = q->nextSibling;
    }
  }
  output.close();
}

//�ǵݹ������  ��������Ϊ����
void traverseFilePath(Tree T)
{
  LinkStack S;
  initStack(S);
  TreeNode *p = T;
  TreeNode *q = new TreeNode;
  while(p || !isStackEmpty(S))
  {
    if(p)
    {
      for(int i=1;i<=p->nodeLevel;i++) //���Ʋ�θ�
        cout<<"   ";
      if(p->fileType == 1)
      	cout<<p->nodeLevel<<"->"<<p->fileName<<endl;
      else
      	cout<<p->nodeLevel<<"->"<<p->fileName<<".txt"<<endl;
      Push(S, p);
      p = p->firstChild;
    }
    else
    {
      Pop(S, q);
      p = q->nextSibling;
    }
  }
}

//�ҵ�T�� T->fileNameΪfilename�Ľ��
//1.����Ǹ��ļ��У�ֱ���ҵ��ļ��� ���ظ��ļ���ָ��
Tree findFileNode(Tree T, string filename)
{
  LinkStack S;
  initStack(S);
  TreeNode *p = T;
  TreeNode *q = new TreeNode;
  while(p || !isStackEmpty(S))
  {
    if(p)
    {
      if(p->fileName == filename)
        return p;
      Push(S, p);
      p = p->firstChild;
    }
    else
    {
      Pop(S, q);
      p = q->nextSibling;
    }
  }
  return NULL;
}

//2.����Ǹ��ļ������ҵ��ļ���ָ�룬���ҵ��ļ��������ļ�ָ��
Tree findFileNode(Tree T, string parentname, string filename)
{
  LinkStack S;
  initStack(S);
  TreeNode *p = T;
  TreeNode *q = new TreeNode;
  while(p || !isStackEmpty(S))
  {
    if(p)
    {
      if(p->fileName == parentname)
        break;
      Push(S, p);
      p = p->firstChild;
    }
    else
    {
      Pop(S, q);
      p = q->nextSibling;
    }
  }
  if(p == NULL) return NULL;

  if(p->firstChild->fileName == filename)
  {
    return p->firstChild;
  }
  else
  {
    p = p->firstChild;
    while(p->nextSibling != NULL)
    {
      if(p->nextSibling->fileName == filename)
        return p->nextSibling;
      p = p->nextSibling;
    }
  }
  return NULL;
}

//�ҵ���һ��nextSiblingΪ�յĽ��
Tree findFirstNextEmptyRoot(Tree T)
{
  T = T->firstChild;
  while(T->nextSibling != NULL)
  {
    T = T->nextSibling;
  }
  return T;
}

//����ļ�����
bool checkSameName(Tree T, string filename, int type)
{
  bool flag = false;
  if(type == 1) //�ļ��в���������
  {
    LinkStack S;
    initStack(S);
    TreeNode *p = T;
    TreeNode *q = new TreeNode;
    while(p || !isStackEmpty(S))
    {
      if(p)
      {
        if(p->fileType == 1 && p->fileName == filename)
        {
          flag = true;
          break;
        }
        Push(S, p);
        p = p->firstChild;
      }
      else
      {
        Pop(S, q);
        p = q->nextSibling;
      }
    }
  }
  else
  {
    TreeNode *p = T;
    p = p->firstChild;
    if(p != NULL)
    {
      if(p->fileName == filename)
        flag = true;
      while(p->nextSibling != NULL)
      {
        p = p->nextSibling;
        if(p->fileName == filename)
        {
          flag = true;
          break;
        }
      }
    }
  }
  return flag;
}

//�����Ϊfilename���ļ��е�·��
string getFilePath(Tree T, string filename)
{
  TreeNode *p = findFileNode(T, filename);
  if(p == NULL)
  {
    cout<<"�ļ������ڣ�"<<endl;
    return "NULL";
  }
  LinkStack S;
  initStack(S);
  Push(S, p);
  while(p->parent != NULL)
  {
    p = p->parent;
    Push(S,p);
  }
  string path;
  while(!isStackEmpty(S))
  {
    TreeNode *t;
    Pop(S, t);
    path += ("\\"+t->fileName);
  }
  return path;
}

//����ļ�����Ϊparentname����Ϊfilename���ļ���·��
string getFilePath(Tree T, string parentname,string filename)
{
  TreeNode *p = findFileNode(T, parentname,filename);
  if(p == NULL)
  {
    cout<<"�ļ������ڣ�"<<endl;
    return "NULL";
  }
  LinkStack S;
  initStack(S);
  Push(S, p);
  while(p->parent != NULL)
  {
    p = p->parent;
    Push(S,p);
  }
  string path;
  while(!isStackEmpty(S))
  {
    TreeNode *t;
    Pop(S, t);
    path += ("\\"+t->fileName);
  }
  return path;
}

//�������ļ�/�ļ���
bool createNewFile(Tree &T, string filename, string newfilename, int type) // �����ֱ�Ϊ �ļ�ϵͳ  Ҫ�������ļ����ڵ��ļ���(�����Ҳ�Ǹ����֣��������ļ������ļ��У����ܴ����ļ�������false) Ҫ�����ļ������� Ҫ�����ļ�������
{
  TreeNode *p = findFileNode(T, filename);
  if(type == 1 && checkSameName(T, newfilename, type))
  {
      cout<<"Warning:"<<endl;
      cout<<"�ļ�ϵͳ���Ѵ�����Ϊ "<<newfilename<<" ���ļ���!"<<endl;
      cout<<endl;
      return false;
  }
  else if(type == 0 && checkSameName(p, newfilename, type))
  {
      cout<<"Warning:"<<endl;
      cout<<"�ļ������Ѵ�����Ϊ "<<newfilename<<" ���ļ�!"<<endl;
      cout<<endl;
      return false;
  }
  if(p == NULL)
  {
    cout<<"�ļ��в����ڣ�"<<endl;
    return false;
  }
  else if(p->fileType == 0)
  {
    cout<<"�㲻����һ���ļ��´����ļ�����ѡ��һ���ļ���!"<<endl;
    return false;
  }
  int level = p->nodeLevel+1;
  if(p->firstChild == NULL)
  {
    createNewTreeNode(p->firstChild, newfilename, type, level);
    p->firstChild->pre = p;
    p->firstChild->parent = p;
  }
  else
  {
    TreeNode *q = findFirstNextEmptyRoot(p);
    createNewTreeNode(q->nextSibling, newfilename, type, level);
    q->nextSibling->pre = q;
    q->nextSibling->parent = p;
  }
  string path = getFilePath(T, filename,newfilename);
  string cdcmd = path.substr(8, 1)+":";
  system(cdcmd.c_str());
  if(type == 1) //�����ļ���
  {
    string cmd = "mkdir "+path.substr(8, path.length()-8);
    //cout<<"�������: "<<cmd<<endl;
    system(cmd.c_str());
  }
  else if(type == 0) //�����ı��ļ�
  {
    string cmd = "type nul>"+path.substr(8, path.length()-8)+".txt"; //������ļ�����д����TXT�ʹ������г��ļ�������ļ�
    //cout<<"�������: "<<cmd<<endl;
    system(cmd.c_str());
  }
  return true;
}

//ɾ���ļ�
bool deleteFile(Tree &T,string parentname, string defilename)
{
  TreeNode *p = findFileNode(T, parentname,defilename);
  if(p == NULL)
  {
    cout<<"Ҫɾ�����ļ�������!"<<endl;
    return false;
  }
  string path = getFilePath(T, parentname,defilename);
  string cdcmd = path.substr(8, 1)+":";
  system(cdcmd.c_str());
  string cmd = "del "+path.substr(8, path.length()-8)+".txt";
  //cout<<"�������: "<<cmd<<endl;
  system(cmd.c_str());
  if(p->pre->firstChild == p)
  {
    p->pre->firstChild = p->nextSibling;
    if(p->nextSibling != NULL)
      p->nextSibling->pre = p->pre;
    delete(p);
  }
  else
  {
    p->pre->nextSibling = p->nextSibling;
    if(p->nextSibling != NULL)
      p->nextSibling->pre = p->pre;
    delete(p);
  }
  return true;
}


//ɾ���ļ���
bool deleteFile(Tree &T, string defilename) //�ļ�ϵͳ      ɾ�����ļ�����
{
  if(defilename == "D:" || defilename == "E:" || defilename == "F")
  {
    cout<<"��ֹɾ�����ļ���!!"<<endl;
    return false;
  }
  TreeNode *p = findFileNode(T, defilename);
  if(p == NULL)
  {
    cout<<"Ҫɾ�����ļ�������!"<<endl;
    return false;
  }

  string ensure;
  cout<<"��ѡ��ɾ������һ���ļ��У���ȷ��Ҫ��ô���������ô���Ļ������ļ����µ��ļ����ļ��н�����ɾ��!"<<endl;
  cout<<"�������ȻҪ��ô����������yes�������Ҫ����������������no:";
  cin>>ensure;
  if(ensure == "no") return false;

  string path = getFilePath(T, defilename);
  string cdcmd = path.substr(8, 1)+":";
  system(cdcmd.c_str());
  LinkStack S;
  initStack(S);
  Tree rootDel = p;
  Tree tmp;
  if(p->firstChild != NULL)
  {
    Push(S, p->firstChild);
    p = p->firstChild;
    while(p->nextSibling != NULL)
    {
      Push(S, p->nextSibling);
      p = p->nextSibling;
    }
  }
  while(!isStackEmpty(S))
  {
    Pop(S, tmp);
    if(tmp->fileType == 1)
      deleteFile(T, tmp->fileName);
    else
      deleteFile(T, rootDel->fileName,tmp->fileName);
  }
  if(rootDel->pre->firstChild == rootDel)
  {
    rootDel->pre->firstChild = rootDel->nextSibling;
    if(rootDel->nextSibling != NULL)
      rootDel->nextSibling->pre = rootDel->pre;
    delete(rootDel);
  }
  else //tmp->pre->nextSibling == p
  {
    rootDel->pre->nextSibling = rootDel->nextSibling;
    if(rootDel->nextSibling !=NULL)
      rootDel->nextSibling->pre = rootDel->pre;
    delete(rootDel);
  }
  string cmd = "rmdir /s "+path.substr(8, path.length()-8);
  system(cmd.c_str());
  //cout<<"�������: "<<cmd<<endl;
  return true;
}

//�������ļ���
bool reNameFile(Tree &T, string filename, string newfilename)
{
  TreeNode *p = findFileNode(T, filename);
  if(checkSameName(T, newfilename, 1))
  {
    cout<<"Warning:"<<endl;
    cout<<"�ļ�ϵͳ���Ѿ�������Ϊ "<<newfilename<<" ���ļ���"<<endl;
    cout<<endl;
    return false;
  }
  if(p == NULL)
  {
    cout<<"Ҫ���������ļ�������!"<<endl;
    return false;
  }
  string path = getFilePath(T, filename);
  string cdcmd = path.substr(8, 1)+":";
  system(cdcmd.c_str());

  string cmd = "rename "+path.substr(8, path.length()-8)+" "+newfilename;
  //cout<<"�������: "<<cmd<<endl;
  system(cmd.c_str());
  p->fileName = newfilename;
  return true;
}

//�������ļ�
bool reNameFile(Tree &T, string parentname,string filename, string newfilename)
{
  TreeNode *p = findFileNode(T, parentname, filename);
  if(checkSameName(p, newfilename, 0))
  {
    cout<<"Warning:"<<endl;
    cout<<"�ļ������Ѿ�������Ϊ "<<newfilename<<" ���ļ�"<<endl;
    cout<<endl;
    return false;
  }
  if(p == NULL)
  {
    cout<<"Ҫ���������ļ�������!"<<endl;
    return false;
  }
  string path = getFilePath(T, parentname,filename);
  string cdcmd = path.substr(8, 1)+":";
  system(cdcmd.c_str());

  string cmd = "rename "+path.substr(8, path.length()-8)+".txt"+" "+newfilename+".txt";
  //cout<<"�������: "<<cmd<<endl;
  system(cmd.c_str());
  p->fileName = newfilename;
  return true;
}

//�г���Ϊfilename���ļ����µ������ļ�/�ļ���
bool lsFileChild(Tree T, string filename)
{
  TreeNode *p = findFileNode(T, filename);
  if(p == NULL)
  {
    cout<<"�ļ��в�����!"<<endl;
    return false;
  }
  else if(p->firstChild == NULL)
  {
    cout<<"���ļ����ǿյģ�"<<endl;
    return true;
  }
  cout<<p->fileName<<"�е��ļ���:"<<endl;
  p = p->firstChild;
  cout<<"  -->"<<p->fileName<<endl;
  while(p->nextSibling != NULL)
  {
    p = p->nextSibling;
    cout<<"  -->"<<p->fileName<<endl;
  }
  return true;
}

//�ƶ��ļ�
bool moveFile(Tree &T, string mvparentname, string mvfilename, string tofilename)
{
  TreeNode *p = findFileNode(T, mvparentname,mvfilename);
  TreeNode *q = findFileNode(T, tofilename);
  if(checkSameName(q, mvfilename, 0))
  {
    cout<<"Warning:"<<endl;
    cout<<"Ҫ�ƶ������ļ������Ѿ�������Ϊ "<<mvfilename<<" ���ļ���"<<endl;
    cout<<endl;
    return false;
  }
  if(p == NULL || q == NULL)
  {
    cout<<"���ƶ��ļ��л��ļ����ƶ������ļ��в�����!"<<endl;
    return false;
  }

  //ֻ���ƶ��ļ�
  if(p->fileType == 0)
  {
    string tmpfilename = q->fileName;
    string tmpnewfilename = p->fileName;
    deleteFile(T, mvparentname,p->fileName);
    createNewFile(T, tmpfilename, tmpnewfilename, 0);
  }
  else
    cout<<"��֧���ƶ��ļ���!"<<endl;
  return true;
}

//�����ļ�
bool copyFile(Tree &T, string parentname, string cpfilename, string tofilename)
{
  TreeNode *p = findFileNode(T, parentname, cpfilename);
  TreeNode *q = findFileNode(T, tofilename);
  if(checkSameName(q, cpfilename, 0))
  {
    cout<<"Warning:"<<endl;
    cout<<"Ҫ�ƶ������ļ������Ѿ�������Ϊ "<<cpfilename<<" ���ļ���"<<endl;
    cout<<endl;
    return false;
  }
  if(p == NULL || q == NULL)
  {
    cout<<"��Ҫ���Ƶ��ļ����ļ��л�Ҫ�ƶ������ļ��в�����!"<<endl;
    return false;
  }

  //ֻ�ܸ����ļ�
  if(p->fileType == 0)
  {
    createNewFile(T, q->fileName, p->fileName, p->fileType);
  }
  else
    cout<<"��֧�ָ����ļ���!"<<endl;
  return true;
}

//������ж����ļ�ϵͳ������
void readFile()
{
  preorder = new MainData[100];
  midorder = new MainData[100];
  ifstream infile("fileone.txt");
  if(!infile) cerr<<"�ļ���ʧ�ܣ�";
  int k = 0;
  while(!infile.eof())
  {
    infile>>midorder[k].filename>>midorder[k].fileType>>midorder[k].nodeLevel;
    ++k;
  }
  infile.close();
  ifstream input("filetwo.txt");
  if(!input) cerr<<"�ļ���ʧ�ܣ�";
  int j = 0;
  while(!input.eof())
  {
    input>>preorder[j].filename>>preorder[j].fileType>>preorder[j].nodeLevel;
    ++j;
  }
  input.close();
  LEN = k-1;
}

//���ö���������ؽ���
Tree readSystemFromTxt(MainData *preorder, MainData *midorder, int len)
{
  MainData rootKey = preorder[0];
  Tree root = new TreeNode;
  root->fileName = rootKey.filename;
  root->fileType = rootKey.fileType;
  root->nodeLevel = rootKey.nodeLevel;
  root->firstChild = NULL;
  root->nextSibling = NULL;
  root->parent = NULL;
  root->pre = NULL;
  if(len == 1 && preorder->filename == midorder->filename)
    return root;

  MainData *rootMidOrder = midorder;
  int leftLen = 0;
  while(rootMidOrder->filename != rootKey.filename && rootMidOrder <= (midorder+len-1))
  {
    ++rootMidOrder;
    ++leftLen;
  }
  //if(rootMidOrder->filename != rootKey.filename ) return NULL; //error

  if(leftLen > 0)
  {
    root->firstChild = readSystemFromTxt(preorder+1, midorder, leftLen);
  }
  if(len-leftLen-1 >0)
  {
    root->nextSibling = readSystemFromTxt(preorder+leftLen+1, rootMidOrder+1, len-leftLen-1);
  }
  return root;
}

//���ؽ��������˫�׺�ǰ����ԭ
void connectPreParent(Tree &T)
{
  LinkStack S;
  initStack(S);
  TreeNode *par;
  TreeNode *now;
  TreeNode *prenow;
  TreeNode *p = T;
  TreeNode *q = new TreeNode;
  while(p || !isStackEmpty(S))
  {
    if(p)
    {

      Push(S, p);
      par = p;
      if(p->firstChild != NULL)
      {
        p->firstChild->parent = par;
        p->firstChild->pre = p;
        now = p->firstChild;
        while(now->nextSibling != NULL)
        {
          prenow = now;
          now = now->nextSibling;
          now->parent = par;
          now->pre = prenow;
        }
      }
      p = p->firstChild;
    }
    else
    {
      Pop(S, q);
      p = q->nextSibling;
    }
  }
}

void checkLocalWindows()
{
  string cmd;
  getline(cin, cmd);
  while(cmd != "exit")
  {
    cout<<"����������(Command):"<<endl;
    getline(cin, cmd);
    system(cmd.c_str());
  }
}

void menu()
{
  cout<<"              |                             |                "<<endl;
  cout<<"               ģ����Դ������Resource Manager                 "<<endl;
  cout<<"              |                             |                "<<endl;
  cout<<"|                  {}��ִ�������б�{}                        "<<endl;
  cout<<"|          []��������-.-                                     "<<endl;
  cout<<"|                  check->�鿴�ļ�ϵͳ                       "<<endl;
  cout<<"|                                                            "<<endl;
  cout<<"|                  new->�������ļ�                           "<<endl;
  cout<<"|                                                            "<<endl;
  cout<<"|                  delete->ɾ���ļ�                          "<<endl;
  cout<<"|                                                            "<<endl;
  cout<<"|                  cp->�����ļ�����һ���ļ���                 "<<endl;
  cout<<"|                                                            "<<endl;
  cout<<"|                  mv->�ƶ��ļ�����һ���ļ���                 "<<endl;
  cout<<"|                                                            "<<endl;
  cout<<"|                  path->��ȡ�ļ�·��                        "<<endl;
  cout<<"|                                                            "<<endl;
  cout<<"|                  ls->�鿴�ļ����µ������ļ�                  "<<endl;
  cout<<"|                                                            "<<endl;
  cout<<"|                  rename->�������ļ�                         "<<endl;
  cout<<"|                                                            "<<endl;
  cout<<"|                  cmd->����������                            "<<endl;
  cout<<"|                                                            "<<endl;
  cout<<"|          ()��������0.0                                      "<<endl;
  cout<<"|                  init->��ʼ���ļ�ϵͳ                       "<<endl;
  cout<<"|                                                            "<<endl;
  cout<<"|                  create->�����ļ�ϵͳ                      "<<endl;
  cout<<"|                                                            "<<endl;
  cout<<"|                  -1->�˳�ϵͳ                               "<<endl;
  cout<<"                                @windows =>����������:";
}

void run(Tree &T)
{
  menu();
  string cmd;
  cin>>cmd;
  if(cmd == "create")
  {
    system("CLS");
    cout<<"���ڽ����ļ�ϵͳ"<<endl;
    Sleep(1000);
    createFileSystem(T);
    cout<<"ִ����ϣ�"<<endl;
    cout<<"���������ַ��س������ϲ�:";
    string s;
    cin>>s;
    system("CLS");
  }
  else if(cmd == "init")
  {
    system("CLS");
    cout<<"���ڳ�ʼ���ļ�ϵͳ"<<endl;
    Sleep(1000);
    initFilePath(T);
    cout<<"ִ����ϣ���ʼ������ļ�ϵͳ:"<<endl;
    traverseFilePath(T);
    cout<<"���������ַ��س������ϲ�:";
    string s;
    cin>>s;
    system("CLS");
  }
  else if(cmd == "cmd")
  {
    system("CLS");
    checkLocalWindows();
    cout<<"ִ����ϣ�"<<endl;
    cout<<"���������ַ��س������ϲ�:";
    string s;
    cin>>s;
    system("CLS");
  }
  else if(cmd == "new")
  {
    system("CLS");
    cout<<"����ִ�д����ļ�����"<<endl;
    cout<<"�������Ŵ����ļ����ļ�������Ҫ�������ļ����ֺ�����(1Ϊ�ļ��� 0Ϊ�ļ�):";
    int type;
    string filename, newfilename;
    cout<<"����������:";
    cin>>type;
    cout<<"������Դ�ļ��������½��ļ���:";
    cin>>filename>>newfilename;
    createNewFile(T, filename, newfilename, type);
    cout<<"ִ����ϣ��������ļ�����ļ�ϵͳ:"<<endl;
    traverseFilePath(T);
    cout<<"���������ַ��س������ϲ�:";
    string s;
    cin>>s;
    system("CLS");
  }
  else if(cmd == "delete")
  {
    system("CLS");
    cout<<"����ִ��ɾ���ļ�����"<<endl;
    cout<<"������Ҫɾ�����ļ����ļ��е���:";
    int type;
    cout<<"������ɾ���ļ�������:";
    cin>>type;
    if(type == 1)
    {
      string defilename;
      cout<<"������Ҫɾ�����ļ�����:";
      cin>>defilename;
      deleteFile(T, defilename);
    }
    else
    {
      string defilename, delparname;
      cout<<"������Ҫɾ���ļ����ڵ��ļ��м�Ҫɾ�����ļ���:";
      cin>>delparname>>defilename;
      deleteFile(T, delparname, defilename);
    }
    cout<<"ִ����ϣ�ɾ���ļ�����ļ�ϵͳ:"<<endl;
    traverseFilePath(T);
    cout<<"���������ַ��س������ϲ�:";
    string s;
    cin>>s;
    system("CLS");
  }
  else if(cmd == "path")
  {
    system("CLS");
    cout<<"����ִ�л�ȡ�ļ�·������"<<endl;
    int type;
    cout<<"�������ļ�����:";
    cin>>type;
    if(type == 1)
    {
      cout<<"������Ҫ��ȡ·�����ļ�����:";
      string filename;
      cin>>filename;
      string path = getFilePath(T, filename);
      cout<<"�ļ���"<<filename<<"��·��Ϊ:";
      cout<<path<<endl;
    }
    else
    {
      cout<<"������Ҫ��ȡ·�����ļ����ڵ��ļ��������ļ���:";
      string parname,filename;
      cin>>parname>>filename;
      string path = getFilePath(T, parname,filename);
      cout<<"�ļ�"<<filename<<"��·��Ϊ:";
      cout<<path<<endl;
    }
    cout<<"���������ַ��س������ϲ�:";
    string s;
    cin>>s;
    system("CLS");
  }
  else if(cmd == "ls")
  {
    system("CLS");
    cout<<"����ִ�в鿴�ļ����µ������ļ�����"<<endl;
    cout<<"������Ҫ�鿴���ļ�����:";
    string filename;
    cin>>filename;
    cout<<"�ļ� "<<filename<<" �µ��ļ���:"<<endl;
    lsFileChild(T, filename);
    cout<<"���������ַ��س������ϲ�:";
    string s;
    cin>>s;
    system("CLS");
  }
  else if(cmd == "rename")
  {
    system("CLS");
    cout<<"����ִ���ļ�����������"<<endl;
    int type;
    cout<<"������Ҫ�������ļ�������:";
    cin>>type;
    if(type == 1)
    {
      string filename, newfilename;
      cout<<"�������ļ���ԭ�����µ��ļ�����:";
      cin>>filename>>newfilename;
      reNameFile(T, filename, newfilename);
    }
    else
    {
      string parname, filename, newfilename;
      cout<<"�������ļ����ڵ��ļ�����,�ļ�ԭ�����µ��ļ���:";
      cin>>parname>>filename>>newfilename;
      reNameFile(T, parname, filename, newfilename);
    }
    cout<<"ִ����ϣ��ļ�����������ļ�ϵͳ:"<<endl;
    traverseFilePath(T);
    cout<<"���������ַ��س������ϲ�:";
    string s;
    cin>>s;
    system("CLS");
  }
  else if(cmd == "mv")
  {
    system("CLS");
    cout<<"����ִ���ļ��ƶ�����"<<endl;
    cout<<"��������ʼ�ļ����ڵ��ļ������Լ��ļ��� ĩ�ļ�����:";
    string parname, filename, newfilename;
    cin>>parname>>filename>>newfilename;
    moveFile(T, parname,filename, newfilename);
    cout<<"ִ����ϣ��ļ��ƶ�����ļ�ϵͳ:"<<endl;
    traverseFilePath(T);
    cout<<"���������ַ��س������ϲ�:";
    string s;
    cin>>s;
    system("CLS");
  }
  else if(cmd == "cp")
  {
    system("CLS");
    cout<<"����ִ���ļ����Ʋ���"<<endl;
    cout<<"��������ʼ�ļ����ڵ��ļ������Լ��ļ��� ĩ�ļ�����:";
    string parname, filename, tofilename;
    cin>>parname>>filename>>tofilename;
    copyFile(T, parname, filename, tofilename);
    cout<<"ִ����ϣ��ļ��ƶ�����ļ�ϵͳ:"<<endl;
    traverseFilePath(T);
    cout<<"���������ַ��س������ϲ�:";
    string s;
    cin>>s;
    system("CLS");
  }
  else if(cmd == "check")
  {
    system("CLS");
    traverseFilePath(T);
    cout<<"���������ַ��س������ϲ�:";
    string s;
    cin>>s;
    system("CLS");
  }
  else if(cmd == "-1")
  {
    fstream output1("fileone.txt", ios::out|ios::trunc);
    if(!output1)
    {
      cerr<<"�ļ���ʧ��!"<<endl;
      exit(0);
    }
    fstream output2("filetwo.txt", ios::out|ios::trunc);
    if(!output2)
    {
      cerr<<"�ļ���ʧ��!"<<endl;
      exit(0);
    }
    output1.close();
    output2.close();
    saveFileSystem(T);
    saveFileSystemAgain(T);
    cout<<"��ӭ�´�ʹ��~"<<endl;
    exit(1);
  }
  else
  {
    system("CLS");
    cout<<"����������������ʽ��ƴд!"<<endl;
    cout<<"���������ַ��س������ϲ�:";
    string s;
    cin>>s;
    system("CLS");
  }
}
