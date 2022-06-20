#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
#include <limits.h>
#include <algorithm>
#include <vector>
#include <set>

using namespace std;

enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

int player;
const int SIZE = 15;
std::array<std::array<int, SIZE>, SIZE> board;

struct Point
{
	int x, y;
	Point() : Point(0,0) {}
	Point(int x, int y) : x{x}, y{y} {}
	bool operator==(const Point &p) const{
		return (x==p.x && y==p.y);
	}
	bool operator!=(const Point &p) const{
		return (x!=p.x || y!=p.y);
	}
	Point operator+(const Point &p) const{
		return Point(x+p.x, y+p.y);
	}
	Point operator-(const Point &p) const{
		return Point(x-p.x, y-p.y);
	}
};

class State
{
	private:
		std::array<std::array<int, SIZE>, SIZE> Board;
		int h;
	
	public:
		State() : h(0) {}
		State(const std::array<std::array<int, SIZE>, SIZE> &board) : h(0){
			Board = board;
		}
		
		State(const State &S) : h(S.h){
			Board = S.Board;
		}
		
		void put_chess(Point &p, int Player){
			Board[p.x][p.y] = Player;
		}
		
		vector<Point> get_valid_point() const{
			vector<Point> valid_points;
			for(int i=0;i<SIZE;i++){
				for(int j=0;j<SIZE;j++){
					if(Board[i][j]!=0) continue;
					else valid_points.push_back(Point(i,j));
				}
			}
			return valid_points;
		}		
		
		int get_h(){
			//h = rand();
			for(int i=0;i<SIZE;i++){
				for(int j=0;j<SIZE;j++){
					if(Board[i][j]==player){
						if(j+1<SIZE && Board[i][j+1]==player){
							if(j+2<SIZE && Board[i][j+2]==player){
								h++;
								if(j+3<SIZE && Board[i][j+3]==player){
									h++;
									if(j+4<SIZE && Board[i][j+4]==player){
										h+=10;
									}
								}
							}
						}

						if(i-1>0 && j-1>0 && Board[i-1][j+1]==player){
							if(i-2>0 && j-2>0 && Board[i-2][j+2]==player){
								h++;
								if(i-3>0 && j-3>0 && Board[i-3][j+3]==player){
									h++;
									if(i-4>0 && j-4>0 && Board[i-4][j+4]==player){
										h+=10;
									}
								}
							}							
						}
						if(i+1<SIZE && Board[i+1][j]==player){
							if(i+2<SIZE && Board[i+2][j]==player){
								h++;
								if(i+3<SIZE && Board[i+3][j]==player){
									h++;
									if(i+4<SIZE && Board[i+4][j]==player){
										h+=10;
									}
								}
							}							
						}
						if(i+1<SIZE && j+1<SIZE && Board[i+1][j+1]==player){
							if(i+2<SIZE && j+2<SIZE && Board[i+2][j+2]==player){
								h++;
								if(i+3<SIZE && j+3<SIZE && Board[i+3][j+3]==player){
									h++;
									if(i+4<SIZE && j+4<SIZE && Board[i+4][j+4]==player){
										h+=10;
									}
								}
							}							
						}
					}
					else if(Board[i][j]== 3-player){
						if(j+1<SIZE && Board[i][j+1]== 3-player){
							if(j+2<SIZE && Board[i][j+2]== 3-player){
								h--;
								if(j+3<SIZE && Board[i][j+3]== 3-player){
									h--;
									if(j+4<SIZE && Board[i][j+4]== 3-player){
										h-=10;
									}
								}
							}
						}
						if(i-1>0 && j-1>0 && Board[i-1][j+1]== 3-player){
							if(i-2>0 && j-2>0 && Board[i-2][j+2]== 3-player){
								h--;
								if(i-3>0 && j-3>0 && Board[i-3][j+3]== 3-player){
									h--;
									if(i-4>0 && j-4>0 && Board[i-4][j+4]== 3-player){
										h-=10;
									}
								}
							}							
						}
						if(i+1<SIZE && Board[i+1][j]== 3-player){
							if(i+2<SIZE && Board[i+2][j]== 3-player){
								h--;
								if(i+3<SIZE && Board[i+3][j]== 3-player){
									h--;
									if(i+4<SIZE && Board[i+4][j]== 3-player){
										h-=10;
									}
								}
							}							
						}
						if(i+1<SIZE && j+1<SIZE && Board[i+1][j+1]== 3-player){
							if(i+2<SIZE && j+2<SIZE && Board[i+2][j+2]== 3-player){
								h--;
								if(i+3<SIZE && j+3<SIZE && Board[i+3][j+3]== 3-player){
									h--;
									if(i+4<SIZE && j+4<SIZE && Board[i+4][j+4]== 3-player){
										h-=10;
									}
								}
							}							
						}
						
					}
					else continue;
				}
			}
			return h;
		}
		
		int set_h(){
			return h;
		}
		
};

int alphabeta(State node, int depth, int alpha, int beta, int Player) {
	if(depth==0)
		return node.get_h();
	
	vector<Point> children = node.get_valid_point();	
	
	if(player == Player){
		int value = INT_MIN;
		for(auto i: children){
			State child = node;
			value = std::max(value, alphabeta(child, depth - 1, alpha, beta, 3-Player));
			alpha = std::max(alpha, value);
			if(alpha>=beta)
				break;
		}
		return value;
	}
	else{
		int value = INT_MAX;
		for(auto i: children){
			State child = node;
			value = std::min(value, alphabeta(child, depth - 1, alpha, beta, player));
			beta = std::min(beta, value);
			if(beta<=alpha)
				break;
		}
		return value;
	}
}

void read_board(std::ifstream& fin) {
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
        }
    }
}

void write_valid_spot(std::ofstream& fout) {
    srand(time(NULL));
    /*
    int x, y;
    // Keep updating the output until getting killed.
    while(true) {
        // Choose a random spot.
        int x = (rand() % SIZE);
        int y = (rand() % SIZE);
        if (board[x][y] == EMPTY) {
            fout << x << " " << y << std::endl;
            // Remember to flush the output to ensure the last action is written to file.
            fout.flush();
        }
    }*/
    int H;
    int h;
    State c(board);
    //H = c.get_h();
    H = INT_MIN;
    vector<Point> initial_points = c.get_valid_point();
    for(auto i: initial_points){
    	State n = c;
    	n.put_chess(i,player);
    	h = alphabeta(n, 2, INT_MIN, INT_MAX, player);
    	if(h > H){
    		//fout << h << endl;
            fout << i.x << " " << i.y << std::endl;
            // Remember to flush the output to ensure the last action is written to file.
            fout.flush();
			H = h;		
		}
	}	 
}




int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}
