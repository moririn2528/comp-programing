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
#include<random>
#include<chrono>
#include<ctime>
using namespace std;
using namespace std::chrono;
typedef long long int LL;
typedef pair<int,int> P;
typedef pair<LL,LL> LP;
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
template<typename T> ostream& operator<<(ostream& os,const vector<T>& v1){
    int n=v1.size();
    for(int i=0;i<n;i++){
        if(i)os<<" ";
        os<<v1[i];
    }
    return os;
}
template<typename T1,typename T2> ostream& operator<<(ostream& os,const pair<T1,T2>& p){
    os<<p.first<<" "<<p.second;
    return os;
}
template<typename T> istream& operator>>(istream& is,vector<T>& v1){
    int n=v1.size();
    for(int i=0;i<n;i++)is>>v1[i];
    return is;
}



namespace sol{
    mt19937 rnd;
    uniform_real_distribution<> dist_real(0.0, 1.0);
    const int N=50;
    int SX,SY;
    vector<vector<int>> tile(N,vector<int>(N));
    vector<vector<int>> tile_p(N,vector<int>(N));
    vector<vector<P>> tile_pos;
    vector<vector<char>> tile_used(N,vector<char>(N));
    const int mv[4][2]={{1,0},{0,1},{-1,0},{0,-1}};
    const string direction="DRUL";
    
    void input(){
        cin>>SX>>SY;
        cin>>tile;
        cin>>tile_p;
        int i,j;
        int a=0;
        for(i=0;i<N;i++){
            for(j=0;j<N;j++){
                a=max(a,tile[i][j]);
            }
        }
        tile_pos.resize(a+1);
        for(i=0;i<N;i++){
            for(j=0;j<N;j++){
                tile_pos[tile[i][j]].push_back({i,j});
            }
        }
    }

    const int change_times=5e4;
    int score_one(int x,int y,int times){
        if(x<0 || y<0 || N<=x || N<=y)return 0;
        int k;
        int a,b,c;
        int s=0;
        c=0;
        for(k=0;k<4;k++){
            a=x+mv[k][0],b=y+mv[k][1];
            if(a<0 || b<0 || N<=a || N<=b)continue;
            if(tile_used[a][b])c++;
        }
        if(x==SX && y==SY){
            s-=abs(c-1)*10;
        }
        int grad=(2*N-(abs(SX-x)+abs(SY-y)))/5;
        if(c<=2)s+=grad*2*c;
        else s+=grad*3-grad*c;
        a=tile[x][y];
        if(tile_pos[a].size()==2){
            b=tile_pos[a][0].first+tile_pos[a][1].first-x;
            c=tile_pos[a][0].second+tile_pos[a][1].second-y;
            if(tile_used[b][c])s-=20;
        }
        if(times<change_times)s+=tile_p[x][y];
        return s;
    }

    int score(){
        int i,j,k;
        int x,y;
        int a;
        int s=0;
        for(i=0;i<N;i++){
            for(j=0;j<N;j++){
                s+=score_one(i,j,0);
            }
        }
        return s;
    }

    int change_and_calc_score(int i,int j,int s,char flag,int times){
        s-=score_one(i,j,times);
        for(int k=0;k<4;k++)s-=score_one(i+mv[k][0],j+mv[k][1],times);
        tile_used[i][j]=flag;
        s+=score_one(i,j,times);
        for(int k=0;k<4;k++)s+=score_one(i+mv[k][0],j+mv[k][1],times);
        return s;
    }

    string output_string;
    int output_score=0;
    vector<vector<char>> output_used;

    int output_extend(string& ss){
        vector<vector<int>> vt(N,vector<int>(N,-1));
        vector<char> used(tile_pos.size());
        int x=SX,y=SY;
        int i,j,k;
        int a,b,c,d;
        for(auto ca:ss){
            used[tile[x][y]]=1;
            for(i=0;i<4;i++){
                if(direction[i]==ca)break;
            }
            assert(i<4);
            vt[x][y]=i;
            x+=mv[i][0],y+=mv[i][1];
        }
        used[tile[x][y]]=1;
        x=SX,y=SY;
        auto check=[&](int p,int q){
            if(p<0 || N<=p || q<0 || N<=q)return false;
            if(used[tile[p][q]])return false;
            return true;
        };
        while(vt[x][y]!=-1){
            k=vt[x][y];
            int nx=x+mv[k][0],ny=y+mv[k][1];
            assert(0<=nx && nx<N && 0<=ny && ny<N);
            for(i=0;i<4;i++){
                a=x+mv[i][0],b=y+mv[i][1];
                c=nx+mv[i][0],d=ny+mv[i][1];
                if(check(a,b) && check(c,d) && tile[a][b]!=tile[c][d]){
                    a=x+mv[i][0],b=y+mv[i][1];
                    used[tile[a][b]]=1;
                    c=vt[a][b]=vt[x][y];
                    a+=mv[c][0],b+=mv[c][1];
                    used[tile[a][b]]=1;
                    vt[a][b]=(i+2)%4;
                    vt[x][y]=i;
                    break;
                }
            }
            if(i==4)x=nx,y=ny;
        }
        ss.clear();
        x=SX,y=SY;
        int sc=tile_p[x][y];
        while(vt[x][y]!=-1){
            a=vt[x][y];
            x+=mv[a][0],y+=mv[a][1];
            assert(0<=x && x<N && 0<=y && y<N);
            sc+=tile_p[x][y];
            ss.push_back(direction[a]);
        }
        return sc;
    }

    int output_save(){
        vector<char> used(tile_pos.size());
        vector<vector<char>> visited(N,vector<char>(N));
        int x=SX,y=SY;
        int i,j,k;
        int a,b,c;
        int sc=0,max_sc=0;
        string ss,max_ss;
        vector<int> v1,v2;
        vector<P> vp;
        for(i=0;i<N*N;i++){
            a=tile[x][y];
            used[a]=true;
            if(!visited[x][y])sc+=tile_p[x][y];
            visited[x][y]=true;
            
            if(max_sc<sc){
                max_sc=sc;
                max_ss=ss;
            }
            v1.clear(),v2.clear();
            for(j=0;j<4;j++){
                b=x+mv[j][0],c=y+mv[j][1];
                if(b<0 ||c<0 || N<=b || N<=c)continue;
                if(used[tile[b][c]] || visited[b][c])continue;
                v2.push_back(j);
                if(!tile_used[b][c])continue;
                v1.push_back(j);
            }
            if(v1.empty())swap(v1,v2);
            if(v1.empty()){
                used[a]=false;
                sc-=tile_p[x][y];
                if(vp.empty())break;
                x=vp.back().first,y=vp.back().second;
                vp.pop_back();
                ss.pop_back();
                continue;
            }
            vp.push_back({x,y});
            j=v1[rnd()%(int)v1.size()];
            ss.push_back(direction[j]);
            x+=mv[j][0],y+=mv[j][1];
        }
        max_sc=output_extend(max_ss);
        if(output_score<max_sc){
            output_score=max_sc;
            output_string=max_ss;
            output_used=tile_used;
        }
        return sc;
    }
    void output(){
        int i,j;
        cout<<output_string<<endl;
        #ifdef MORIRIN
        // for(i=0;i<N;i++){
        //     for(j=0;j<N;j++){
        //         if(output_used[i][j])cerr<<1;
        //         else cerr<<0;
        //     }
        //     cerr<<endl;
        // }
        cerr<<output_score<<endl;
        #endif //MORIRIN
    }

    void init_used(){
        int x=SX,y=SY;
        int i,j,k;
        int a,b,c;
        vector<int> v1;
        int cnt=1;
        if(rnd()%5==0){
            tile_used.assign(N,vector<char>(N,0));
            for(i=0;i<N;i++){
                for(j=0;j<N;j++){
                    if(tile_p[i][j]>=70)tile_used[i][j]=1;
                }
            }
            cnt=10;
        }
        if(rnd()%3==0){
            for(i=0;i<N;i++){
                for(j=0;j<N;j++){
                    if(tile_p[i][j]>=90)tile_used[i][j]=1;
                }
            }
        }
        for(k=0;k<cnt;k++){
            vector<char> used(tile_pos.size());
            for(i=0;i<N*N;i++){
                a=tile[x][y];
                used[a]=true;
                tile_used[x][y]=true;
                v1.clear();
                for(j=0;j<4;j++){
                    b=x+mv[j][0],c=y+mv[j][1];
                    if(b<0 ||c<0 || N<=b || N<=c)continue;
                    if(used[tile[b][c]])continue;
                    v1.push_back(j);
                }
                if(v1.empty())break;
                j=v1[rnd()%(int)v1.size()];
                x+=mv[j][0],y+=mv[j][1];
            }
            x=rnd()%N,y=rnd()%N;
        }
    }

    void solve(){
        int n,m;
        int i,j,k;
        int a,b,c;
        int x,y;
        auto start_time=system_clock::now();
        const int stop_time=1900;
        const int repeat_times=3e6,same_limit=1e4;
        const double start_temp = 150;//(一回の遷移で動きうるスコア幅の最大値程度);
        const double end_temp = 0;//(一回の遷移で動きうるスコア幅の最小値程度);
        const int temp_N=1e4;
        double prob;
        double temp;//(線形でstart_tempからend_tempに減少する);
        input();

        int cnt;
        for(cnt=0;cnt<1e6;cnt++){
            auto end_time=system_clock::now();
            if(duration_cast<milliseconds>(end_time-start_time).count()>stop_time)break;
            init_used();
            int sc=score(),same_times=0;

            #ifdef MORIRIN
            cerr<<sc<<endl;
            #endif //MORIRIN

            for(i=0;i<repeat_times && same_times<same_limit;i++){
                if(i%100==0){
                    end_time=system_clock::now();
                    if(duration_cast<milliseconds>(end_time-start_time).count()>stop_time)break;
                }
                x=rnd()%N,y=rnd()%N;
                int new_sc=change_and_calc_score(x,y,sc,tile_used[x][y]^1,i);
                if(i>=temp_N){
                    if(new_sc>sc)prob=1;
                    else prob=0;
                }else{
                    temp=start_temp*(temp_N-i)+end_temp*i;
                    temp/=repeat_times;
                    prob=exp((new_sc-sc)/temp);
                }
                if(prob<dist_real(rnd)){
                    //return before
                    tile_used[x][y]^=1,same_times++;
                }else{
                    sc=new_sc,same_times=0;

                    #ifdef MORIRIN
                    cerr<<sc<<endl;
                    #endif //MORIRIN
                }
            }
            output_save();
            #ifdef MORIRIN
            cerr<<i<<endl;
            #endif //MORIRIN
        }

        #ifdef MORIRIN
        cerr<<cnt<<endl;
        #endif //MORIRIN
        output();
    }
}

int main(){
    sol::solve();
}