#include "Player.hh"

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME AIOakSaver
#include <list>
#define INT_MAX 99999

struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }
  struct Node {
	  int pare_i;
	  int pare_j;
	  int f;
	  int g;
	  int h;
  };
  struct base {
	  int dist;
	  Post p;
	  bool operator<(const base& b) const {
        return dist < b.dist;
      }
  };
  struct atac {
	  int amics;
	  int enemics;
	  int dist;
	  Position D;
  };
  int I_min[8] = { 1, 0, -1,  0 };
  int J_min[8] = { 0, 1,  0, -1 };
  int I[8] = { 1, 1, 0, -1, -1, -1,  0,  1 };
  int J[8] = { 0, 1, 1,  1,  0, -1, -1, -1 };
  int I_aux[24] = { 1, 1, 0, -1, -1, -1,  0,  1, -2, -2, -2, -1, 0, 1, 2, 2, 2, 2, 2, 1, 0, -1, -2, -2};
  int J_aux[24] = { 0, 1, 1,  1,  0, -1, -1, -1, 0, 1, 2, 2, 2, 2, 2, 1, 0, -1, -2, -2, -2, -2, -2, -1};
  typedef pair<int,Position> VP;  
  vector<Position> D_agafada;
  vector<Post> bases;
  vector<vector<int> > M;
  /**
   * Types and attributes for your player can be defined here.
   */

  /**
   * Play method, invoked once per each round.
   */
  int distance(Position A, Position B) { //DistĂ ncia entre 2 nodes
	  return max(abs(A.i-B.i),abs(A.j-B.j));
  }
  bool soldat_aprop(Position P) {
	int i = P.i;
	int j = P.j;
	for (int k = 0; k < 24; k++) {
				int ii = i + I_aux[k];
			    int jj = j + J_aux[k];
			    if (pos_ok(Position(ii,jj))) {
					int id = which_soldier(ii,jj);
					if (id > 0 and data(id).player != me()) {
						return true;
					}
				}
	}
	return false;
  }
  
  int cost_cell(Position P,bool baralla) {
	  int id = which_soldier(P.i,P.j);
	  if((fire_time(P.i,P.j) > 0)) return 1000;
	  if (id > 0 and data(id).player == me()) return 500;
	  if(soldat_aprop(P) and not baralla) return 500;
	  else {
		if (what(P.i,P.j) == FOREST) return 100;
		if (what(P.i,P.j) == GRASS) return 50;
	  }
	  
	  return INT_MAX;
  }  
  int busqueda(const vector<VP>& Open, VP S) {
	  for (int i = 0; i < int(Open.size()); ++i) {
		  if(not(Open[i].second != S.second)) return i; 
	  }
	  return -1;
  }
  
  Position Base_propera(Position Act) {
	  vector<Post> bases = posts();
	  vector<base> Q;
	  for (Post Po : bases) {
		  if (post_owner(Po.pos.i,Po.pos.j) != me()) {
			  int val = post_value(Po.pos.i,Po.pos.j);
			  base obj;
			  if (val == LOW_VALUE) {
				obj.dist = distance(Po.pos,Act);
				obj.p = Po;
				Q.push_back(obj);
			  }
			  if (val == HIGH_VALUE) {
				obj.dist = distance(Po.pos,Act) - distance(Po.pos,Act)*0.1;
				obj.p = Po;
				Q.push_back(obj);
			  }
		  }
	  }
	  sort(Q.begin(),Q.end());
	  return Q[0].p.pos;
  }

  Position d_helicopter(Position Act) {
		vector<Post> bases = posts();
		vector<base> Q;
		for (Post Po : bases) {
			if (post_owner(Po.pos.i,Po.pos.j) != me()) {
				int val = post_value(Po.pos.i,Po.pos.j);
				base obj;
				if (val != -1 and val == LOW_VALUE and round() < 40) {
				obj.dist = distance(Po.pos,Act);
				obj.p = Po;
				Q.push_back(obj);
				}
				if (val != -1 and val == HIGH_VALUE) {
				obj.dist = distance(Po.pos,Act) - distance(Po.pos,Act)*0.1;
				obj.p = Po;
				Q.push_back(obj);
				}
			}
		}
		sort(Q.begin(),Q.end());
		Position dest;
		if (Q.size() > 0) dest = Q[0].p.pos;
		else dest = Act;
		return dest;
  }
  	
  Position Path(const vector< vector<Node> > &cellDetails, Position dst, Position src) {
	  
	  int row = dst.i;
	  int col = dst.j;
	  stack<pair<int,int> > path;
	  path.push({row,col});
	  while (not (cellDetails[row][col].pare_i == src.i and cellDetails[row][col].pare_j == src.j)) {
	   	int row_aux = cellDetails[row][col].pare_i;
		col = cellDetails[row][col].pare_j;
		row = row_aux;
	   	path.push({row,col});
	  }
	  Position D = Position(row,col);
	  if (pos_ok(D.i,D.j)) return D;
	  else return src;
		
  }
  int Path_h(const vector< vector<Node> > &cellDetails, Position dst, Position src, int id) {
	  int mov = CLOCKWISE;
	  int row = dst.i;
	  int col = dst.j;
	  stack<pair<int,int> > path;
	  path.push({row,col});
	  while (not (cellDetails[row][col].pare_i == src.i and cellDetails[row][col].pare_j == src.j)) {
	   	int row_aux = cellDetails[row][col].pare_i;
		col = cellDetails[row][col].pare_j;
		row = row_aux;
	   	path.push({row,col});
	  }
	  	  int orientation = data(id).orientation;

	  if (orientation == 0) {
		  if (path.top().first - src.i == -1 and path.top().second == src.j) {
				mov = COUNTER_CLOCKWISE;
			}
			
			if (path.top().first - src.i == 1  and path.top().second == src.j) {
				mov = FORWARD1;
			}
			
			if (path.top().first == src.i and path.top().second - src.j == 1 ) {
				mov = COUNTER_CLOCKWISE;
			}
			
			if (path.top().first == src.i and path.top().second - src.j == -1) {
				mov = CLOCKWISE;
			}
			
	  }
	  else if (orientation == 1) {
		  if (path.top().first - src.i == -1 and path.top().second == src.j) {
				mov = COUNTER_CLOCKWISE;
			}
			
			if (path.top().first - src.i == 1  and path.top().second == src.j) {
				mov = CLOCKWISE;
			}
			
			if (path.top().first == src.i and path.top().second - src.j == 1 ) {
				mov = FORWARD1;
			}
			
			if (path.top().first == src.i and path.top().second - src.j == -1) {
				mov = CLOCKWISE;
			}
	  }
	  else if (orientation == 2) {

		  if (path.top().first - src.i == -1 and path.top().second == src.j) {
				mov = FORWARD1;
			}
			
			if (path.top().first - src.i == 1  and path.top().second == src.j) {
				mov = CLOCKWISE;
			}
			
			if (path.top().first == src.i and path.top().second - src.j == 1 ) {
				mov = CLOCKWISE;
			}
			
			if (path.top().first == src.i and path.top().second - src.j == -1) {
				mov = COUNTER_CLOCKWISE;
			}
			
	  }
	  else {

		  if (path.top().first - src.i == -1 and path.top().second == src.j) {
				mov = CLOCKWISE;
			}
			
			if (path.top().first - src.i == 1  and path.top().second == src.j) {
				mov = COUNTER_CLOCKWISE;
			}
			
			if (path.top().first == src.i and path.top().second - src.j == 1 ) {
				mov = CLOCKWISE;
			}
			
			if (path.top().first == src.i and path.top().second - src.j == -1) {
				mov = FORWARD1;
			}
			
	  }
      if (mov == FORWARD1 and distance(src,dst) > 1) mov = FORWARD2;
	  return mov;
		
  }
  int number_enemies(Position H_Act) {
	int number = 0;
	int i = H_Act.i;
	int j = H_Act.j;
	for (int k = 0; k < 24; k++) {
				int ii = i + I_aux[k];
			    int jj = j + J_aux[k];
			    if (pos_ok(Position(ii,jj))) {
					int id = which_soldier(ii,jj);
					if (id > 0 and data(id).player != me()) {
						++number;
					}
				}
	}
	if (which_soldier(i,j) > 0 and data(which_soldier(i,j)).player != me()) ++number;
	return number;
  }

  atac atacar(Position Act) {
	vector<vector<bool> > V(MAX,vector<bool>(MAX,false));
	V[Act.i][Act.j] = true;
	atac A;
	A.amics = 1;
	A.enemics = 0;
	A.dist = INT_MAX;
	A.D.i = -1;
	A.D.j = -1;
	queue<pair<int,int> > Q;
	Q.push({Act.i,Act.j});
	while(!Q.empty()) {
		pair<int,int> u = Q.front();
		Q.pop();
		int i = u.first;
		int j = u.second;

		for(int k=0; k<8; ++k) {
			int ii = i + I[k];
			int jj = j + J[k];
			if (pos_ok(Position(ii,jj)) and !V[ii][jj] and distance(Position(ii,jj),Act) <= 4) {
				 V[ii][jj] = true;
				 int id = which_soldier(ii,jj);
				 
				 if (id > 0 and data(id).player != me()) {
					if (distance(Position(ii,jj),Act) < A.dist) {
						A.dist = distance(Position(ii,jj),Act);
						A.D = Position(ii,jj);
					}
				 ++A.enemics;
				 }
				 
				 if (id > 0 and data(id).player == me()) {
				 ++A.amics;
				 }
				 Q.push({ii,jj});
			}
		}
	}
	  
	return A;
  }
  
  Position cerca(Position src, Position dst, bool baralla) {
		if (not pos_ok(src)) return MAX;
		if (not pos_ok(dst)) return MAX;
		if (not(src != dst)) return dst;
		// Closed list
		vector< vector<bool> > closedList(MAX, vector<bool> (MAX, false));
		
		// Path
		Node n;
		n.pare_i = -1;
		n.pare_j = -1;
		n.f = INT_MAX;
		n.g = INT_MAX;
		n.h = INT_MAX;
		vector< vector<Node> > cellDetails(MAX, vector<Node> (MAX, n));
		
		cellDetails[src.i][src.j].f = 0;
		cellDetails[src.i][src.j].g = 0;
		cellDetails[src.i][src.j].h = 0;
		cellDetails[src.i][src.j].pare_i = src.i;
		cellDetails[src.i][src.j].pare_j = src.j;
		
		set<pair<int, pair<int,int> > > openList;
		openList.insert(make_pair(0, make_pair(src.i, src.j)));
		while (not openList.empty()) {
			pair< int, pair<int, int> > p = *openList.begin();
			openList.erase(openList.begin());
			int i = p.second.first;
			int j = p.second.second;
			closedList[i][j] = true;
			int new_f;
			int new_g;
			int new_h;
			
			for (int k = 0; k < 8; k++)
			{
				int ii = i + I[k];
			    int jj = j + J[k];
			    Position pos(ii,jj);
				if (pos_ok(pos) and what(ii,jj) != WATER and what(ii,jj) != MOUNTAIN) {
					if (ii == dst.i and  jj == dst.j) {
						cellDetails[ii][jj].pare_i = i;
						cellDetails[ii][jj].pare_j = j;
						return Path(cellDetails, dst, src);
					}
					
					else if (not closedList[ii][jj]) {
						new_g = cellDetails[i][j].g + cost_cell(Position(ii,jj),baralla);
						new_h = cellDetails[i][j].h + distance(pos,dst);
						new_f = new_g + new_h;
						if (cellDetails[ii][jj].f == INT_MAX or cellDetails[ii][jj].f > new_f) {
							openList.insert(make_pair(new_f, make_pair(ii,jj)));
							cellDetails[ii][jj].f = new_f;
							cellDetails[ii][jj].g = new_g;
							cellDetails[ii][jj].h = new_h;
							cellDetails[ii][jj].pare_i = i;
							cellDetails[ii][jj].pare_j = j;
						}
					}
				}
			}
		}
		
		return src;
	}
	int muntanya_aprop(int i, int j) {
		for(int ii = -3; ii<=3; ++ii) {
			for (int jj =-3; jj<=3; ++jj){
				if (not pos_ok(i+ii,j+jj) or what(i+ii,j+jj) == MOUNTAIN) {
					return 60000;
				}
			}
		}
		return 0;
	}
	int cerca_h(Position src, Position dst, int id) {
		if (not pos_ok(src)) return CLOCKWISE;
		if (not pos_ok(dst)) return CLOCKWISE;
		if (not(src != dst)) return CLOCKWISE;
		// Closed list
		vector< vector<bool> > closedList(MAX, vector<bool> (MAX, false));
		
		// Path
		Node n;
		n.pare_i = -1;
		n.pare_j = -1;
		n.f = INT_MAX;
		n.g = INT_MAX;
		n.h = INT_MAX;
		vector< vector<Node> > cellDetails(MAX, vector<Node> (MAX, n));
		
		cellDetails[src.i][src.j].f = 0;
		cellDetails[src.i][src.j].g = 0;
		cellDetails[src.i][src.j].h = 0;
		cellDetails[src.i][src.j].pare_i = src.i;
		cellDetails[src.i][src.j].pare_j = src.j;
		
		set<pair<int, pair<int,int> > > openList;
		openList.insert(make_pair(0, make_pair(src.i, src.j)));
		
		while (not openList.empty()) {
			pair< int, pair<int, int> > p = *openList.begin();
			openList.erase(openList.begin());
			int i = p.second.first;
			int j = p.second.second;
			closedList[i][j] = true;
			int new_f;
			int new_g;
			int new_h;
			
				for (int k = 0; k < 4; k++)
					{
						int ii = i + I_min[k];
						int jj = j + J_min[k];
						Position pos(ii,jj);
						if (pos_ok(pos)) {
							if (ii == dst.i and  jj == dst.j) {
								cellDetails[ii][jj].pare_i = i;
								cellDetails[ii][jj].pare_j = j;
								return Path_h(cellDetails, dst, src,id);
							}
							
							else if (not closedList[ii][jj]) {
								new_g = cellDetails[i][j].g + muntanya_aprop(ii,jj);
								new_h = cellDetails[i][j].h + distance(pos,dst);
								new_f = new_g + new_h;
								if (cellDetails[ii][jj].f == INT_MAX or cellDetails[ii][jj].f > new_f) {
									openList.insert(make_pair(new_f, make_pair(ii,jj)));
									cellDetails[ii][jj].f = new_f;
									cellDetails[ii][jj].g = new_g;
									cellDetails[ii][jj].h = new_h;
									cellDetails[ii][jj].pare_i = i;
									cellDetails[ii][jj].pare_j = j;
								}
							}
						}
					}
			
		}
		
		return CLOCKWISE;
	}
  void tirar_parachuter(Position H, int n) {
	  int i = H.i;
	  int j = H.j;
	  for(int ii = -2; ii<=2 and n > 0; ++ii) {
			for (int jj =-2; jj<=2 and n > 0; ++jj){
				int id = which_soldier(i+ii,j+jj);
				if ( pos_ok(i+ii,j+jj) and which_soldier(i+ii, j+jj) == 0 and what(i+ii,j+jj) != WATER 
						and what(i+ii,j+jj) != MOUNTAIN) {
					command_parachuter(i+ii,j+jj);
					--n;
				}
			}
		}
  }
  void play_soldier(int id, int soldat) {
	//---- INFO SOLDAT ------	
	  Data Soldat = data(id);
	  Position Act = Soldat.pos;
	  
	//---- ACCIO RONDA ACTUAL ---
	atac A = atacar(Act);
	bool baralla = false;
	Position Dest = Base_propera(Act);
	if (A.D.i != -1 and A.D.j != -1) {
		if (A.amics >= A.enemics) Dest = A.D;
		baralla = true;
	}
	  Position go = cerca(Act,Dest,baralla);
	  command_soldier(id, go.i, go.j);
  }

  void play_helicopter(int id) {
   // If we can, we throw napalm.
    Data Helicopter = data(id);
    Position H_Act = Helicopter.pos;

		Position Desti = d_helicopter(H_Act);
    if(number_enemies(H_Act) > 3 and Helicopter.napalm == 0) {
		command_helicopter(id, NAPALM);
		return;
	}
		
	int paracaigudes = Helicopter.parachuters.size();
	if (paracaigudes > 0 and status(me()) < 0.85) {
		if (paracaigudes >= 4) tirar_parachuter(H_Act,4);
		else if (paracaigudes == 3)	tirar_parachuter(H_Act,3);
		else if (paracaigudes == 2)tirar_parachuter(H_Act,2);
		else tirar_parachuter(H_Act,1);
		return;
	}
		if (H_Act != Desti) {
			int go = cerca_h(H_Act,Desti,id);
			command_helicopter(id,go);
		}
  }
  
  virtual void play () {
	int player = me();
	vector<int> H = helicopters(player); //els meus helicopters.
	vector<int> S = soldiers(player);  // els meus soldats.
	for (int i = 0; i < (int)H.size(); ++i) play_helicopter(H[i]);
	for (int i = 0; i < (int)S.size(); ++i) play_soldier(S[i],i);
  }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);