// #pragma GCC target("avx2")
// #pragma GCC optimize("O3")
// #pragma GCC optimize("unroll-loops")
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
#include<fstream>
#include<ctime>
#include<random>
#include<chrono>
using namespace std;
using namespace chrono;
typedef long long int LL;
typedef pair<int,int> P;
typedef pair<LL,int> LP;
typedef pair<double,int> DP;
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
    typedef tuple<int,int,int,int> T;
    const int N=10000,stop_time=4500;
    int n;
    int output[222][4];
    vector<int> X,Y,R;
    vector<T> block,block_save;
    int output_save[222][4];

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

    double calc_p(int r,int s){
        return 1-pow(1-(double)min(s,r)/max(s,r),2);
    }

    double calc_p(int r,int a,int b,int c,int d){
        int space=(c-a)*(d-b);
        return calc_p(r,space);
    }

    double calc_p(int i){
        assert(0<=i && i<n);
        return calc_p(R[i],output[i][0],output[i][1],output[i][2],output[i][3]);
    }

    double calc_p2(int r,int s){
        if(r<=s)return 1;
        return (double)s/r;
    }

    double calc_p2(int r,int a,int b,int c,int d){
        int space=(c-a)*(d-b);
        return calc_p2(r,space);
    }

    double calc_p2(int i){
        assert(0<=i && i<n);
        return calc_p2(R[i],output[i][0],output[i][1],output[i][2],output[i][3]);
    }

    void check_result(){
        int i,j,k;
        int a,b,c,d;
        for(i=0;i<n;i++){
            assert(0<=output[i][0] && output[i][2]<=N && 0<=output[i][1] && output[i][3]<=N);
            assert(output[i][0]<output[i][2] && output[i][1]<output[i][3]);
        }
        for(i=0;i<n;i++){
            for(j=0;j<n;j++){
                if(i==j)continue;
                bool x_lap=(output[i][0]<output[j][2] && output[j][0]<output[i][2]);
                bool y_lap=(output[i][1]<output[j][3] && output[j][1]<output[i][3]);
                assert(!x_lap || !y_lap);
            }
        }
    }

    int final_result(bool check=true){
        int i,j;
        if(check)check_result();
        long double res=0;
        for(i=0;i<n;i++){
            if(output[i][0]<=X[i] && X[i]<output[i][2] && output[i][1]<=Y[i] && Y[i]<output[i][3]){
                res+=calc_p(i);
            }
        }
        res/=n;
        res*=1e9;
        return (int)res;
    }

    void answer_output(){
        int i,j;
        for(i=0;i<n;i++){
            for(j=0;j<4;j++){
                if(j)cout<<" ";
                cout<<output[i][j];
            }
            cout<<endl;
        }
        #ifdef MORIRIN
        double res=final_result();
        cerr<<"score: "<<res/1e9<<endl;
        #endif //MORIRIN
    }

    void output_set(int i,int a,int b,int c,int d){
        assert(0<=i && i<n);
        output[i][0]=a,output[i][1]=b;
        output[i][2]=c,output[i][3]=d;
    }

    void rotate_ad_pos(int& x,int& y){
        swap(x,y);
        x=N-x-1;
    }
    void rotate_ad_pos(int id){
        rotate_ad_pos(X[id],Y[id]);
    }

    void rotate_pos(int& x,int& y){
        swap(x,y);
        x=N-x;
    }

    void rotate_rect(int& a,int& b,int& c,int& d){
        rotate_pos(a,b);
        rotate_pos(c,d);
        if(a>c)swap(a,c);
        if(b>d)swap(b,d);
    }

    void rotate_output_rect(int id){
        assert(0<=id && id<n);
        rotate_rect(output[id][0],output[id][1],output[id][2],output[id][3]);
    }

    double sub_rect_calc(int x1,int y1,int x2,int y2){
        int i,j,k;
        int a,b,c;
        double da,db;
        vector<P> vp_temp;
        for(i=0;i<n;i++){
            if(X[i]<x1 || x2<=X[i] || Y[i]<y1 || y2<=Y[i])continue;
            vp_temp.push_back(make_pair(X[i],i));
        }
        sort(vp_temp.begin(),vp_temp.end());
        int m=vp_temp.size();
        if(m==0)return -1;
        vector<int> vx(m+1),vi(m),vX(m+2),vR(m);
        vector<double> div(m);
        for(i=0;i<m;i++){
            if(i && vp_temp[i-1].first==vp_temp[i].first)return -1;
            vi[i]=vp_temp[i].second;
        }
        vX[0]=vx[0]=x1;
        for(i=1;i<m;i++)vx[i]=X[vi[i]];
        vX[m+1]=vx[m]=x2;
        for(i=0;i<m;i++)vX[i+1]=X[vi[i]];
        for(i=0;i<m;i++)vR[i]=R[vi[i]];

        for(i=0;i<m;i++){
            div[i]=calc_p2(vR[i],vx[i],y1,vx[i+1],y2);
            da=div[i],a=vR[i];
            for(j=i-1;j>=0;j--){
                if(div[j]<=da)break;
                a+=vR[j];
                da=calc_p2(a,vx[j],y1,vx[i+1],y2);
            }
            for(k=i;k>j+1;k--){
                a=round(vR[k]*da);
                b=vx[k+1]-a;
                b=max(b,vX[k]+1);
                b=min(b,vX[k+1]);
                vx[k]=b;
                div[k]=calc_p2(vR[k],vx[k],y1,vx[k+1],y2);
            }
            div[j+1]=calc_p2(vR[j+1],vx[j+1],y1,vx[j+2],y2);
        }
        assert(vx[0]==x1);
        assert(vx[m]==x2);
        for(i=0;i<m;i++)assert(vx[i]<=vx[i+1]);
        double ans=0;
        for(i=0;i<m;i++){
            if(calc_p2(vR[i],vx[i],y1,vx[i+1],y2)<1){
                output_set(vi[i],vx[i],y1,vx[i+1],y2);
            }else{
                //枠が大きいとき、y 軸方向の長さを変更するとさらに良くなる可能性あり
                a=round((double)vR[i]/(y2-y1));
                if(a<=0)a=1;
                if(a==1){
                    output_set(vi[i],vX[i+1],y1,vX[i+1]+1,min(y2,y1+vR[i]));
                }else{
                    assert(vx[i+1]-vx[i]>=a);
                    if(vx[i+1]-a<=vX[i+1])output_set(vi[i],vx[i+1]-a,y1,vx[i+1],y2);
                    else output_set(vi[i],vX[i+1],y1,vX[i+1]+a,y2);
                }
            }
            ans+=calc_p(vi[i]);
        }
        for(i=0;i<m;i++){
            j=vi[i];
            assert(x1<=output[j][0] && output[j][2]<=x2);
            assert(y1<=output[j][1] && output[j][3]<=y2);
        }
        return ans/m;
    }

    void save_output(){
        int i,j;
        for(i=0;i<n;i++){
            for(j=0;j<4;j++){
                output_save[i][j]=output[i][j];
            }
        }
        block_save=block;
    }

    void recover_output(){
        int i,j;
        for(i=0;i<n;i++){
            for(j=0;j<4;j++){
                output[i][j]=output_save[i][j];
            }
        }
        block=block_save;
    }

    bool expand_area(int id){
        int i,j,k;
        int a,b,c,d;
        int cnt=0;
        for(int rot=0;rot<4;rot++){
            for(i=0;i<n;i++){
                rotate_output_rect(i);
                rotate_ad_pos(i);
            }
            int x1=output[id][0],y1=output[id][1],x2=output[id][2],y2=output[id][3];
            a=0,b=N;
            for(i=0;i<n;i++){
                if(i==id)continue;
                if(output[i][2]<=x1 || x2<=output[i][0])continue;
                if(y2<=output[i][1])b=min(b,output[i][1]);
                else a=max(a,output[i][3]);
            }
            if(a==y1 && b==y2){
                cnt++;
                continue;
            }
            c=round(R[id]/(x2-x1));
            if(c<=1)c=1;
            if(y2-y1>c){
                d=min(y2-c,Y[id]);
            }else{
                c=min(c,b-a);//y_range
                d=max(a,y2-c);//y1_pos
            }
            if((y2-y1-c)<=1)cnt++;
            output[id][1]=d,output[id][3]=d+c;
        }
        return cnt<=1;
    }

    void expand_area_all(){
        int i,j,k;
        int a;
        priority_queue<DP,vector<DP>,greater<DP>> q1;
        for(i=0;i<n;i++){
            q1.push(make_pair(calc_p2(i),i));
        }
        j=0;
        while(!q1.empty() && q1.top().first<1){
            a=q1.top().second;
            q1.pop();
            if(expand_area(a))q1.push(make_pair(calc_p2(a),a));
        }
    }
    
    void calc(int a,int b,int c,int d){
        while(1){
            if(a>=c || b>=d)break;
            int i,j,k;
            int p,q;
            int rot;
            int save[10]={0};
            double save_score=0;
            for(rot=0;rot<4;rot++){
                vector<P> vp;
                for(i=0;i<n;i++){
                    if(X[i]<a || c<=X[i] || Y[i]<b || d<=Y[i])continue;
                    vp.push_back(make_pair(Y[i]-b,i));
                }
                sort(vp.begin(),vp.end());
                int m=vp.size();
                if(m==0)break;
                int sum_area=0;
                for(i=0;i<m;i++){
                    j=vp[i].second;
                    sum_area+=R[j];
                    p=round((double)sum_area/(c-a));
                    p=max(p,vp[i].first+1);
                    p=min(p,d-b);
                    if(i<m-1)p=min(p,vp[i+1].first);
                    double s=sub_rect_calc(a,b,c,b+p);
                    if(save_score<s){
                        save_score=s;
                        save[0]=a,save[1]=b,save[2]=c,save[3]=b+p;
                        save[4]=a,save[5]=b+p,save[6]=c,save[7]=d;
                        save[9]=rot;
                    }
                }
                rotate_rect(a,b,c,d);
                for(i=0;i<n;i++)rotate_ad_pos(X[i],Y[i]);
            }
            if(save_score==0)break;
            for(rot=0;rot<4;rot++){
                if(rot==save[9])sub_rect_calc(save[0],save[1],save[2],save[3]);
                if(rot>=save[9]){
                    rotate_rect(save[0],save[1],save[2],save[3]);
                    rotate_rect(save[4],save[5],save[6],save[7]);
                }
                for(i=0;i<n;i++){
                    rotate_ad_pos(i);
                    rotate_output_rect(i);
                }
            }
            block.push_back(make_tuple(save[0],save[1],save[2],save[3]));
            a=save[4],b=save[5],c=save[6],d=save[7];
        }
    }

    void solve(){
        int i,j,k;
        int a,b,c,d;
        mt19937_64 rnd;
        auto start_time=system_clock::now();
        input();
        calc(0,0,N,N);
        int score=final_result(false);
        save_output();
        
        for(int cnt=0;cnt<1e6;cnt++){
            auto end_time=system_clock::now();
            if(duration_cast<milliseconds>(end_time-start_time).count()>stop_time)break;
            if(block.empty())break;
            unsigned long long int r=rnd(),area_all=0;
            for(i=0;i<block.size();i++){
                tie(a,b,c,d)=block[i];
                area_all+=(c-a)*(d-b);
            }
            r%=area_all;
            for(i=0;i<block.size();i++){
                tie(a,b,c,d)=block[i];
                if(r<=(c-a)*(d-b))break;
                r-=(c-a)*(d-b);
            }
            assert(i<block.size());
            block.erase(block.begin()+i);
            calc(a,b,c,d);
            int next_score=final_result(false);
            if(score<next_score)score=next_score,save_output();
            else recover_output();
        }
        expand_area_all();
        expand_area_all();
        answer_output();
    }
}

int main(){
    sol::solve();
}