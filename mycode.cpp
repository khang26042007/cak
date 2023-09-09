#include<bits/stdc++.h>

using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

int board[9][9], num[9][9];
vector<pair<int,int>> group[9];

bitset<10> row[9], col[9], box[9];

int appear[10];

int CAK = 0;

int visited_row[9], visited_column[9], visited_group[9];

string inp;

void init() {
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
        {
            num[i][j] = i/3 * 3 + j/3;
            group[num[i][j]].emplace_back(i,j);
        }

    for (int i = 0; i < 9; ++i)
        for (int j = 1; j <= 9; ++j)
            row[i][j] = col[i][j] = box[i][j] = 1;
}

void Move(int &x, int &y, int &n, bool b) {
    board[x][y] = b ? 0 : n;
    row[x][n] = col[y][n] = box[num[x][y]][n] = b;
}

int Size;
pair<int,int> Empty[81];

bool cmp(pair<int,int> &x, pair<int,int> &y) {
    return (row[x.first] & col[x.second] & box[num[x.first][x.second]]).count() <
            (row[y.first] & col[y.second] & box[num[y.first][y.second]]).count();
}

int solve(int idx) {
    if (idx == Size) {
    	return 1;
    }

    nth_element(Empty+idx, Empty+idx, Empty+Size, cmp);

    auto &[x, y] = Empty[idx];
    bitset<10> tmp = row[x] & col[y] & box[num[x][y]];
    
    int cnt = 0;

    for (int n = tmp._Find_first(); n < 10; n = tmp._Find_next(n)) {
        Move(x, y, n, 0);
        cnt += solve(idx + 1);
        Move(x, y, n, 1);
        if(cnt >= 2) return 2;
    }

    return cnt;
}

int solve_sudoku() {
    Size = 0;
    for (int x = 0; x < 9; ++x)
        for (int y = 0; y < 9; ++y)
            if (!board[x][y])
                Empty[Size++] = {x, y};
    return solve(0);
}

void bucu(int x,int y,int n) {
	CAK += (visited_row[x] == 0) + (visited_column[y] == 0) + (visited_group[num[x][y]] == 0);
	appear[n] ++;
	visited_row[x] = visited_column[y] = visited_group[num[x][y]] = 1;
}

void handle(string s) {
	int x = s[0] - 'A', y = s[1] - 'a', n = s[2] - '0';
	Move(x,y,n,0); bucu(x,y,n);
}

void print(int x,int y,int n) {
	cout<<char(x+'A')<<char(y+'a')<<n;
	Move(x,y,n,0); bucu(x,y,n);
	
	int cur = solve_sudoku();
	if(cur == 1) cout<<'!';
	cout<<endl;
}

void pick() {
	vector< pair< pair<int,int>, pair<int,int> > > v;
	for(int i = 0; i < 9; ++i) for(int j = 0; j < 9; ++j)
	{
		if(board[i][j]) continue;

		bitset<10> tmp = row[i] & col[j] & box[num[i][j]];
		int occur = 100, number = 0;

		for (int n = tmp._Find_first(); n < 10; n = tmp._Find_next(n))
			if(appear[n] < occur) {
				occur = appear[n]; number = n;
			}

		Move(i,j,number,0);
		int check = solve_sudoku();
		Move(i,j,number,1);

		if(check == 0) continue;
		if(check == 1) {
			print(i,j,number); return;
		}

		v.push_back( make_pair( make_pair(occur,number), make_pair(i,j) ) );
	}

	sort(v.begin(), v.end());
	int pivot = rng()%v.size();
	pair< pair<int,int>, pair<int,int> > ok = v[pivot];
	int x = ok.second.first, y = ok.second.second, n = ok.first.second;

	print(x,y,n);
}

signed main() {
  	//freopen(".inp","r",stdin); freopen(".out","w",stdout);
  	
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	init();

	while(cin>>inp) {
		if(inp == "Start") {
			pick();
		}
		else {
			handle(inp); 
			pick();
		}
	}
}
