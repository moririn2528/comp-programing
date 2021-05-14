#include<iostream>
#include<algorithm>
#include<vector>
#include<map>
using namespace std;
const int INF=1e9+7;
typedef pair<int,int> P;

int spot[1100];
int sensor[110000][3];
int dp[1100][1100];

void solve(int n){
    int a,b,c,d;
    int i,j,k;
    int s=0;
    int sx,sy,gx,gy;
    bool xrev=false,yrev=false;
    cin>>sx>>sy>>gx>>gy;
    gx-=sx,gy-=sy;
    if(gx<0)xrev=true,gx=-gx;
    if(gy<0)yrev=true,gy=-gy;
    memset(dp,0,sizeof(dp));
    vector<P> v1;
    for(i=0;i<n;i++){
        cin>>a>>b>>c;
        a-=sx,b-=sy;
        if(xrev)a=-a;
        if(yrev)b=-b;

        if(gx==0){
            if(a-c<=0 && a+c>=0 && b-c<=gy && b+c>=0)s++;
        }
        if(gy==0){
            if(b-c<=0 && b+c>=0 && a-c<=gx && a+c>=0)s++;
        }

        sensor[i][0]=a,sensor[i][1]=b,sensor[i][2]=c;
        if(a-c<=0 && b-c<=0 && a+c>=0 && b+c>=0){
            dp[0][0]++;
            continue;
        }
        if(a-c<=gx && b-c<=gy && a+c>=gx && b+c>=gy){
            dp[0][0]++;
            continue;
        }
        v1.push_back(make_pair(a+c,-i-1));
        v1.push_back(make_pair(a-c,i));
    }
    if(gx==0 || gy==0){
        cout<<s<<endl;
        return ;
    }
    memset(spot,0,sizeof(spot));
    sort(v1.begin(),v1.end());
    for(i=0,k=0;i<=gx;i++){
        for(;k<v1.size() && v1[k].first<=i;k++){
            a=v1[k].first,b=v1[k].second;
            if(b<0){
                b=-b-1;
                c=sensor[b][1]-sensor[b][2];
                if(c>=0)spot[c]--;
                continue;
            }
            c=sensor[b][1]-sensor[b][2];
            d=sensor[b][1]+sensor[b][2];
            if(c>=0)spot[c]++;
            c=max(c,0),d=min(d,gy+10);
            if(d<0)continue;
            if(a==i && i>0){
                dp[i][c]++,dp[i][d]--;
            }
        }
        if(i>0){
            for(j=0;j<=gy;j++){
                dp[i][j+1]+=dp[i][j];
            }
            for(j=0;j<=gy;j++){
                dp[i][j]+=dp[i-1][j];
            }
        }else{
            for(j=1;j<=gy;j++){
                dp[i][j]=INF;
            }
        }
        // cout<<i<<" ";
        for(j=0;j<=gy;j++){
            // cout<<dp[i][j]<<" ";
            dp[i][j+1]=min(dp[i][j+1],dp[i][j]+spot[j+1]);
        }
        // cout<<endl;
    }
    cout<<dp[gx-1][gy-1]<<endl;
}

int main(){
    int n;
    while(cin>>n){
        if(n==0)break;
        solve(n);
    }
}