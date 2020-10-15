#include<iostream>
#include<cstdio>
#include<cstring>
#include<string>
#include<vector>
#include<cmath>
#include<algorithm>
#include<map>
#include<queue>
#include<deque>
#include<iomanip>
#include<tuple>
#include<cassert>
#include<set>
#include<complex>
using namespace std;
typedef long long int LL;
typedef pair<int,int> P;
typedef pair<LL,int> LP;
const int INF=1<<30;
const LL MAX=1e9+7;

void array_show(int *array,int array_n,char middle=' '){
    for(int i=0;i<array_n;i++)printf("%d%c",array[i],(i!=array_n-1?middle:'\n'));
}
void array_show(LL *array,int array_n,char middle=' '){
    for(int i=0;i<array_n;i++)printf("%lld%c",array[i],(i!=array_n-1?middle:'\n'));
}
void array_show(vector<int> &vec_s,int vec_n=-1,char middle=' '){
    if(vec_n==-1)vec_n=vec_s.size();
    for(int i=0;i<vec_n;i++)printf("%d%c",vec_s[i],(i!=vec_n-1?middle:'\n'));
}
void array_show(vector<LL> &vec_s,int vec_n=-1,char middle=' '){
    if(vec_n==-1)vec_n=vec_s.size();
    for(int i=0;i<vec_n;i++)printf("%lld%c",vec_s[i],(i!=vec_n-1?middle:'\n'));
}

typedef vector<vector<int>> M;
string form;
vector<M> matrix;

M calc(int& index,int mode);
void correct_matrix(M& va);

void print_matrix(M& va){
    correct_matrix(va);
    for(auto& v:va){
        for(int i=0;i<v.size();i++){
            if(i)cout<<" ";
            cout<<v[i];
        }
        cout<<endl;
    }
    #ifdef MORIRIN
    //cout<<endl;
    #endif//MORIRIN
}

bool is_num(int index){
    assert(0<=index && index<form.size());
    if('0'<=form[index] && form[index]<='9')return true;
    if('A'<=form[index] && form[index]<='Z')return true;
    if(form[index]=='[')return true;
    return false;
}

int get_num_only(int& index){
    int s=0;
    for(;index<form.size();index++){
        if(form[index]<'0' || '9'<form[index])break;
        s*=10;
        s+=form[index]-'0';
    }
    index--;
    return s;
}

M get_num(int& index){
    int a,b,c;
    assert(is_num(index));
    if('A'<=form[index] && form[index]<='Z'){
        a=form[index]-'A';
        return matrix[a];
    }
    if('0'<=form[index] && form[index]<='9'){
        return M{{get_num_only(index)}};
    }
    //form[index]=='['
    index++;
    M vs;
    int i=0,j=0;
    int x,y;
    a=0;
    for(;index<form.size();index++){
        if(form[index]==']')break;
        if(form[index]==' ')continue;
        if(form[index]==';'){
            i+=a,j=0,a=0;
            continue;
        }
        //form[index] == num
        M va=calc(index,1);
        a=va.size(),b=va[0].size();
        if(j==0)vs.resize(i+a);
        for(x=0;x<a;x++){
            for(y=0;y<b;y++){
                vs[i+x].push_back(va[x][y]);
            }
        }
        j+=b;
    }
    return vs;
}

bool is_one_num(M& va){
    return va.size()==1 && va[0].size()==1;
}

M prod(M& va,M& vb){
    M vs;
    int a,b;
    assert(!va.empty() && !vb.empty());
    if(is_one_num(va))vs=vb,a=va[0][0];
    else if(is_one_num(vb)) vs=va,a=vb[0][0];
    if(!vs.empty()){
        for(auto& v:vs){
            for(auto& num:v)num*=a;
        }
        return vs;
    }
    assert(va[0].size()==vb.size());
    vs.assign(va.size(),vector<int>(vb[0].size()));
    int i,j,k;
    for(i=0;i<vs.size();i++){
        for(j=0;j<vs[i].size();j++){
            for(k=0;k<vb.size();k++){
                vs[i][j]+=va[i][k]*vb[k][j];
            }
        }
    }
    return vs;
}

M add(M& va,M& vb){
    assert(!va.empty() && va.size()==vb.size() && va[0].size()==vb[0].size());
    M vs=va;
    int i,j;
    for(i=0;i<va.size();i++){
        for(j=0;j<va[0].size();j++){
            vs[i][j]+=vb[i][j];
        }
    }
    return vs;
}

void neg(M& va){
    for(auto& v:va){
        for(auto& num:v)num=-num;
    }
}
M trans(M& va){
    assert(!va.empty() && !va[0].empty());
    M vs(va[0].size(),vector<int>(va.size()));
    int i,j;
    for(i=0;i<va.size();i++){
        for(j=0;j<va[i].size();j++){
            vs[j][i]=va[i][j];
        }
    }
    return vs;
}

void correct_matrix(M& va){
    if(va.empty())return;
    int n=va.size(),m=va[0].size();
    int i,j;
    for(i=0;i<n;i++){
        for(j=0;j<m;j++){
            va[i][j]&=(1<<15)-1;
        }
    }
}

M index_primary(M& vt,int& index){
    assert(form[index]=='(');
    index++;
    M va=calc(index,2);
    index++;
    M vb=calc(index,0);
    assert(form[index]==')');
    assert(va.size()==1 && vb.size()==1);
    correct_matrix(va),correct_matrix(vb);
    int n=va[0].size(),m=vb[0].size();
    M vs(n,vector<int>(m));
    int i,j;
    int a,b;
    for(i=0;i<n;i++){
        for(j=0;j<m;j++){
            a=va[0][i]-1,b=vb[0][j]-1;
            assert(0<=a && a<vt.size());
            assert(0<=b && b<vt[0].size());
            vs[i][j]=vt[a][b];
        }
    }
    return vs;
}

M calc(int& index,int mode){
    int i,j,k;
    int a,b,c;
    int minus_count=0;
    M va,vb,vc;
    for(;index<form.size();index++){
        if(form[index]=='.')break;
        if(form[index]==')'){
            assert(mode==0);
            break;
        }
        if(form[index]==' ' || form[index]==';' || form[index]==']'){
            assert(mode==1);
            index--;
            break;
        }
        if(form[index]==','){
            assert(mode==2);
            break;
        }
        if(form[index]=='('){
            if(va.empty()){
                index++;
                va=calc(index,0);
            }else{
                va=index_primary(va,index);
            }
            continue;
        }
        if(form[index]=='\''){
            va=trans(va);
            continue;
        }
        if(is_num(index)){
            assert(va.empty());
            va=get_num(index);
            continue;
        }
        //form[index] * or + or -
        if(form[index]=='-' && va.empty()){
            minus_count++;
            continue;
        }
        if(minus_count%2){
            neg(va);
        }
        minus_count=0;
        if(vb.empty())swap(va,vb);
        else vb=prod(vb,va);
        va.clear();
        if(form[index]=='+' || form[index]=='-'){
            if(vc.empty())swap(vb,vc);
            else vc=add(vc,vb);
            vb.clear();
        }
        if(form[index]=='-')minus_count++;
    }
    if(minus_count%2)neg(va);
    if(!vb.empty() && !va.empty())vb=prod(vb,va);
    else if(!va.empty())swap(va,vb);

    if(!vb.empty() && !vc.empty())vc=add(vc,vb);
    else if(!vb.empty())swap(vc,vb);
    return vc;
}

int main(){
    int n,m;
    int i,j,k;
    int a,b,c;
    M va,vb,vc;
    while(cin>>n){
        if(!n)return 0;
        matrix.clear();
        matrix.resize(35);
        getline(cin,form);
        for(i=0;i<n;i++){
            getline(cin,form);
            a=form[0]-'A',b=2;
            matrix[a]=calc(b,0);
            print_matrix(matrix[a]);
        }
        cout<<"-----"<<endl;
    }
}