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
#include<numeric>
#include<functional>
#include<unordered_map>
#include<unordered_set>
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

namespace sol{
    int n;
    int output[222][4];
    vector<int> X,Y,R;

    void input(){
        cin>>n;
        int a,b,c;
        for(int i=0;i<n;i++){
            cin>>a>>b>>c;
            X.push_back(a);
            Y.push_back(b);
            R.push_back(c);
        }
    }

    int final_result(){
        int i,j;
        for(i=0;i<n;i++){
            for(j=0;j<n;j++){
                if(i==j)continue;
                bool x_lap=(output[i][0]<output[j][2] && output[j][0]<output[i][2]);
                bool y_lap=(output[i][1]<output[j][3] && output[j][1]<output[i][3]);
                assert(!x_lap || !y_lap);
            }
        }
        long double res=0;
        for(i=0;i<n;i++){
            if(output[i][0]<=X[i] && X[i]<output[i][2] && output[i][1]<=Y[i] && Y[i]<output[i][3]){
                int space=(output[i][2]-output[i][0])*(output[i][3]-output[i][1]);
                res+=1-powl(1-min(space,R[i])/max(space,R[i]),2);
            }
        }
        res/=n;
        res*=1e9;
        return (int)res;
    }

    void solve(){
        int n,m;
        int i,j,k;
        int a,b,c;
        
    }
}

int main(){
    sol::solve();
}