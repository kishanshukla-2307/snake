#include<bits/stdc++.h>
#include<ncurses.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<boost/algorithm/string.hpp> 
#include<time.h>
#include<iostream>
#include<string>


#define HEIGHT 30
#define WIDTH 60
#define LEVEL 0


using namespace std;



char MAP[HEIGHT][WIDTH];
string curr_direction="EAST";
int SCORE=0;
int DELAY=250000;      // microseconds unit
int VELOCITY=1.1;
int dV=1;     
pair<int, int> FOOD;
vector< pair<int,int> > SNAKE = { make_pair(HEIGHT/2, WIDTH/2-1), make_pair(HEIGHT/2, WIDTH/2), make_pair(HEIGHT/2, WIDTH/2+1)}; // snake of intial length 3
int NO_0F_USERS=0;

class Player{
	private:
		string name;
		int HighestScore;
		// double AvgScore;
		int NoOfGamesPlayed;
	public:
		Player(string name){
			this->name = name;
			NoOfGamesPlayed=0;
			// AvgScore=0;
			HighestScore=0;
		}
		string getName(){
			return this->name;
		}
		int getHighestScore(){
			return this->HighestScore;
		}
		// double getAvgScore(){
		// 	return this->AvgScore;
		// }
		int getNoOfGamesPlayed(){
			return this->NoOfGamesPlayed;
		}
		int setHighestScore(int hscr){
			return this->HighestScore=hscr;
		}
		// double setAvgScore(double ascr){
		// 	return this->AvgScore=ascr;
		// }
		int setNoOfGamesPlayed(int no){
			return this->NoOfGamesPlayed=no;
		}
};


const char* int2string(int val){
	string ans;
	while(val > 0){
		ans.push_back(val%10 + 48);
		val /= 10;
	}
	reverse(ans.begin(), ans.end());
	char* final;
	final = (char*)malloc(sizeof(char)*(ans.size()));
	for (int i = 0; i < ans.size(); ++i)
	{
		final[i] = ans[i];
	}
	const char* nfinal = final;
	return final;
}

void print(){
	clear();
	refresh();
	start_color();
	// init_pair(6,3,7);
	// wborder(stdscr, 0, 0, 0, 0, 0, 0, 0, 0);
	char temp[HEIGHT][WIDTH];
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			temp[i][j] = MAP[i][j];
		}
	}
	for (int i = 0; i < SNAKE.size(); ++i)
	{
		temp[SNAKE[i].first][SNAKE[i].second] = '@';
	}
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			if( make_pair(i,j) == SNAKE[SNAKE.size()-1] ){
				if(curr_direction=="EAST"){
					addch(ACS_RARROW);
				}else if(curr_direction=="WEST"){
					addch(ACS_LARROW);
				}else if(curr_direction=="SOUTH"){
					addch(ACS_DARROW);
				}else{
					addch(ACS_UARROW);
				}
				continue;
			}
			if(i==0 || j==0 || i==HEIGHT-1 || j==WIDTH-1){
				addch(ACS_BULLET);
				continue;
			}
			if(FOOD == make_pair(i,j)){
				waddch(stdscr, '$');
			}
			else
				waddch(stdscr, temp[i][j]);
		}
		waddch(stdscr, '\n');
	}
	wrefresh(stdscr);
	addstr("SCORE: ");
	refresh();
	const char* score = int2string(SCORE);
	addstr( score );
	refresh();
}

void intialize(){
	// input configuration
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);

	// MAP initialization
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			MAP[i][j] = ' ';
		}
	}
	//TOP BORDER
	for(int i=0; i<WIDTH; ++i){
		MAP[0][i] = '+';
	}
	//LEFT BORDER
	for(int i=0; i<HEIGHT;	++i){
		MAP[i][0] = '+';
	}
	//BOTTOM BORDER
	for(int i=0; i<WIDTH; ++i){
		MAP[HEIGHT-1][i] = '+';
	}
	//RIGHT BORDER
	for(int i=0; i<HEIGHT;	++i){
		MAP[i][WIDTH-1] = '+';
	}

	//initialise food
	FOOD = make_pair(rand()%(HEIGHT-2)+1, rand()%(WIDTH-2)+1);
	// FOOD = make_pair(4, 5);
	// OUTPUT ON SCREEN
	print();
}

void move(char ch){
	switch(ch){
		case 'w':
			if(curr_direction!="SOUTH"){
				SNAKE.push_back( make_pair(SNAKE[SNAKE.size()-1].first-1, SNAKE[SNAKE.size()-1].second) );
				curr_direction = "NORTH";
			}else{
				SNAKE.push_back( make_pair(SNAKE[SNAKE.size()-1].first+1, SNAKE[SNAKE.size()-1].second) );
			}
			if(SNAKE[SNAKE.size()-1] == FOOD){
				FOOD = make_pair(rand()%(HEIGHT-2)+1, rand()%(WIDTH-2)+1);
				SCORE++;
				if(SCORE%5==0){
					VELOCITY += dV;
				}
			}else
				SNAKE.erase(SNAKE.begin());
			break;
		case 'a':
			if(curr_direction!="EAST"){
				SNAKE.push_back( make_pair(SNAKE[SNAKE.size()-1].first, SNAKE[SNAKE.size()-1].second-1) );
				curr_direction = "WEST";
			}else{
				SNAKE.push_back( make_pair(SNAKE[SNAKE.size()-1].first, SNAKE[SNAKE.size()-1].second+1) );
			}
			if(SNAKE[SNAKE.size()-1] == FOOD){
				FOOD = make_pair(rand()%(HEIGHT-2)+1, rand()%(WIDTH-2)+1);
				SCORE++;
				if(SCORE%5==0){
					VELOCITY += dV;
				}
			}else
				SNAKE.erase(SNAKE.begin());
			break;
		case 's':
			if(curr_direction!="NORTH"){
				SNAKE.push_back( make_pair(SNAKE[SNAKE.size()-1].first+1, SNAKE[SNAKE.size()-1].second) );
				curr_direction = "SOUTH";
			}else{
				SNAKE.push_back( make_pair(SNAKE[SNAKE.size()-1].first-1, SNAKE[SNAKE.size()-1].second) );
			}
			if(SNAKE[SNAKE.size()-1] == FOOD){
				FOOD = make_pair(rand()%(HEIGHT-2)+1, rand()%(WIDTH-2)+1);
				SCORE++;
				if(SCORE%5==0){
					VELOCITY += dV;
				}
			}else
				SNAKE.erase(SNAKE.begin());
			break;
		case 'd':
			if(curr_direction!="WEST"){
				SNAKE.push_back( make_pair(SNAKE[SNAKE.size()-1].first, SNAKE[SNAKE.size()-1].second+1) );
				curr_direction = "EAST";
			}else{
				SNAKE.push_back( make_pair(SNAKE[SNAKE.size()-1].first, SNAKE[SNAKE.size()-1].second-1) );
			}
			if(SNAKE[SNAKE.size()-1] == FOOD){
				FOOD = make_pair(rand()%(HEIGHT-2)+1, rand()%(WIDTH-2)+1);
				SCORE++;
				if(SCORE%5==0){
					VELOCITY += dV;
				}
			}else
				SNAKE.erase(SNAKE.begin());
			break;
		case 'W':
			if(curr_direction!="SOUTH"){
				SNAKE.push_back( make_pair(SNAKE[SNAKE.size()-1].first-1, SNAKE[SNAKE.size()-1].second) );
				curr_direction = "NORTH";
			}else{
				SNAKE.push_back( make_pair(SNAKE[SNAKE.size()-1].first+1, SNAKE[SNAKE.size()-1].second) );
			}
			if(SNAKE[SNAKE.size()-1] == FOOD){
				FOOD = make_pair(rand()%(HEIGHT-2)+1, rand()%(WIDTH-2)+1);
				SCORE++;
				if(SCORE%5==0){
					VELOCITY += dV;
				}
			}else
				SNAKE.erase(SNAKE.begin());
			break;
		case 'S':
			if(curr_direction!="NORTH"){
				SNAKE.push_back( make_pair(SNAKE[SNAKE.size()-1].first+1, SNAKE[SNAKE.size()-1].second) );
				curr_direction = "SOUTH";
			}else{
				SNAKE.push_back( make_pair(SNAKE[SNAKE.size()-1].first-1, SNAKE[SNAKE.size()-1].second) );
			}
			if(SNAKE[SNAKE.size()-1] == FOOD){
				FOOD = make_pair(rand()%(HEIGHT-2)+1, rand()%(WIDTH-2)+1);
				SCORE++;
				if(SCORE%5==0){
					VELOCITY += dV;
				}
			}else
				SNAKE.erase(SNAKE.begin());
			break;
		case 'A':
			if(curr_direction!="EAST"){
				SNAKE.push_back( make_pair(SNAKE[SNAKE.size()-1].first, SNAKE[SNAKE.size()-1].second-1) );
				curr_direction = "WEST";
			}else{
				SNAKE.push_back( make_pair(SNAKE[SNAKE.size()-1].first, SNAKE[SNAKE.size()-1].second+1) );
			}
			if(SNAKE[SNAKE.size()-1] == FOOD){
				FOOD = make_pair(rand()%(HEIGHT-2)+1, rand()%(WIDTH-2)+1);
				SCORE++;
				if(SCORE%5==0){
					VELOCITY += dV;
				}
			}else
				SNAKE.erase(SNAKE.begin());
			break;
		case 'D':
			if(curr_direction!="WEST"){
				SNAKE.push_back( make_pair(SNAKE[SNAKE.size()-1].first, SNAKE[SNAKE.size()-1].second+1) );
				curr_direction = "EAST";
			}else{
				SNAKE.push_back( make_pair(SNAKE[SNAKE.size()-1].first, SNAKE[SNAKE.size()-1].second-1) );
			}
			if(SNAKE[SNAKE.size()-1] == FOOD){
				FOOD = make_pair(rand()%(HEIGHT-2)+1, rand()%(WIDTH-2)+1);
				SCORE++;
				if(SCORE%5==0){
					VELOCITY += dV;
				}
			}else
				SNAKE.erase(SNAKE.begin());
			break;
	}
}

bool game_over(){
	if (SNAKE[SNAKE.size()-1].first == 0 || SNAKE[SNAKE.size()-1].second == 0 || SNAKE[SNAKE.size()-1].first == HEIGHT-1 || SNAKE[SNAKE.size()-1].second == WIDTH-1){
		return true;
	}else{
		bool flag=false;
		for (int i = 0; i < SNAKE.size()-1; ++i)
		{
			if(SNAKE[i] == SNAKE[SNAKE.size()-1]){
				flag=true;
				break;
			}
		}
		return flag;
	}
}


char input(){
	// INPUT FROM THE USER
	int ch = getch();
	if(ch == ERR){
		return '\0';
	}else{
		return ch;
	}
}


void play(){
	int x = HEIGHT, y = WIDTH;
	while(true){
		char ch = input();
		if(ch){
			move(ch);
			if(game_over()){
				clear();
				// getmaxyx(stdscr, x, y);
				attron(A_STANDOUT);
 				mvwaddstr(stdscr, x/2, y/2, "GAME OVER !!");
				for (int i = 0; i < 3; ++i)
				{
					waddch(stdscr, '.');
					refresh();
					sleep(0.7);
				}
 				refresh();
 				sleep(0.7);
				break;
			}
			print();
		}else{
			if(curr_direction=="EAST"){
				move('d');
				if(game_over()){
					clear();
					// getmaxyx(stdscr, x, y);
					attron(A_STANDOUT);
     				mvwaddstr(stdscr, x/2, y/2, "GAME OVER !!");
				for (int i = 0; i < 3; ++i)
				{
					waddch(stdscr, '.');
					refresh();
					sleep(0.7);
				}
     				refresh();
     				sleep(1);
					break;
				}
				print();
			}else if(curr_direction=="WEST"){
				move('a');
				if(game_over()){
					clear();
					// getmaxyx(stdscr, x, y);
					attron(A_STANDOUT);
     				mvwaddstr(stdscr, x/2, y/2, "GAME OVER !!");
				for (int i = 0; i < 3; ++i)
				{
					waddch(stdscr, '.');
					refresh();
					sleep(0.7);
				}
     				refresh();
     				sleep(1);
					break;
				}
				print();
			}else if(curr_direction=="NORTH"){
				move('w');
				if(game_over()){
					clear();
					// getmaxyx(stdscr, x, y);
					attron(A_STANDOUT);
     				mvwaddstr(stdscr, x/2, y/2, "GAME OVER !!");
				for (int i = 0; i < 3; ++i)
				{
					waddch(stdscr, '.');
					refresh();
					sleep(0.7);
				}
     				refresh();
     				sleep(1);
					break;
				}
				print();
			}else if(curr_direction=="SOUTH"){
				move('s');
				if(game_over()){
					clear();
					// getmaxyx(stdscr, x, y);
					attron(A_STANDOUT);
     				mvwaddstr(stdscr, x/2, y/2, "GAME OVER !!");
					for (int i = 0; i < 3; ++i)
					{
						waddch(stdscr, '.');
						refresh();
						sleep(0.7);
					}
     				refresh();
     				sleep(1);
					break;
				}
				print();
			}
		}
		usleep(DELAY/VELOCITY);
	}
}

class CSVReader
{
	std::string fileName;
	std::string delimeter;
 
public:
	CSVReader(std::string filename, std::string delm = ",") :
			fileName(filename), delimeter(delm)
	{ }
 
	// Function to fetch data from a CSV File
	std::vector<std::vector<std::string> > getData();
};

std::vector<std::vector<std::string> > CSVReader::getData()
{
	std::ifstream file(fileName);
 
	std::vector<std::vector<std::string> > dataList;
 
	std::string line = "";
	// Iterate through each line and split the content using delimeter
	while (getline(file, line))
	{
		std::vector<std::string> vec;
		boost::algorithm::split(vec, line, boost::is_any_of(delimeter));
		dataList.push_back(vec);
	}
	// Close the File
	file.close();
 
	return dataList;
}

class CSVWriter
{
	std::string fileName;
	std::string delimeter;
	int linesCount;
 
public:
	CSVWriter(std::string filename, int count, std::string delm = ",") :
			fileName(filename), linesCount(count), delimeter(delm)
	{}
	/*
	 * Member function to store a range as comma seperated value
	 */
	template<typename T>
	void addDatainRow(T first, T last);
};

template<typename T>
void CSVWriter::addDatainRow(T first, T last)
{
	std::fstream file;
	// Open the file in truncate mode if first line else in Append Mode
	file.open(fileName, std::ios::out | (linesCount ? std::ios::app : std::ios::trunc));
 
	// Iterate over the range and add each lement to file seperated by delimeter.
	for (; first != last; )
	{
		file << *first;
		if (++first != last)
			file << delimeter;
	}
	file << "\n";
	linesCount++;
 
	// Close the file
	file.close();
}

bool CheckifAlreadyPresent(string name, Player &player, std::vector< std::vector< std::string > > &d){
	bool userAlreadypresent=false;
	int userID=0;
	CSVReader reader("playerData.csv");
	d = reader.getData();
	NO_0F_USERS = d.size();
	for (int i = 0; i < d.size(); ++i)
	{
		if(d[i][0] == name){
			userAlreadypresent=true;
			userID=i;
			break;
		}
	}
	if(userAlreadypresent){
		cout << "Welcome Back, " << d[userID][0] << " !!\n"; sleep(0.5);
		cout << "Following are your previous games performance:"; sleep(1);
		cout <<  "\n1. No of games played: " << stoi(d[userID][2]); sleep(1.5);
		cout << "\n2. Highest Score: " << stoi(d[userID][1]) << "\n"; sleep(1);
		// player.setAvgScore(stoi(data[userID][2]));
		player.setHighestScore(stoi(d[userID][1]));
		player.setNoOfGamesPlayed(stoi(d[userID][2]));
	}
	return userAlreadypresent;
}

void UpdatePlayerInfo(string name, int score, Player &player, bool oldUser, std::vector< std::vector< std::string > > data){
	if(score>player.getHighestScore())
		player.setHighestScore(score);
	// player.setAvgScore((player.getAvgScore()*player.getNoOfGamesPlayed()+score)/(1+player.getNoOfGamesPlayed()));
	CSVWriter writer("playerData.csv", NO_0F_USERS);
	player.setNoOfGamesPlayed(player.getNoOfGamesPlayed()+1);
	stringstream hscr, no;
	hscr << player.getHighestScore();
	no << player.getNoOfGamesPlayed();
	string hs = hscr.str();
	string n = no.str();
	std::vector< std::string > v = {name, hs, n};
	if(!oldUser)
		writer.addDatainRow(v.begin(), v.end());
	else{
		NO_0F_USERS=0;
		CSVWriter writer("new.csv", NO_0F_USERS);
		for (int i = 0; i < data.size(); ++i)
		{
			if(data[i][0] == name){
				writer.addDatainRow(v.begin(), v.end());
				NO_0F_USERS++;
			}else{
				writer.addDatainRow(data[i].begin(), data[i].end());
				NO_0F_USERS++;
			}
		}
		rename("new.csv", "playerData.csv");
	}
}

void UpdateLeaderBoard(Player &player, bool oldUser){
	CSVReader reader("leaderboard.csv");
	std::vector< std::vector< std::string > > data = reader.getData();
	int userID=0;
	CSVWriter writer("newlb.csv", 0);
	if(!oldUser){
		if(data.size()==0){
			stringstream hscr;
			hscr << player.getHighestScore();
			std::vector< std::string > vec = {player.getName(), hscr.str()};
			writer.addDatainRow(vec.begin(), vec.end());
		}else{
			bool flag=0;
			for (int i = 0; i < data.size(); ++i)
			{
				if(stoi(data[i][1]) >= player.getHighestScore()){
					writer.addDatainRow(data[i].begin(), data[i].end());
				}else{
					stringstream hscr;
					hscr << player.getHighestScore();
					std::vector< std::string > vec = {player.getName(), hscr.str()};
					writer.addDatainRow(vec.begin(), vec.end());
					for (int j = i; j < data.size(); ++j)
					{
						writer.addDatainRow(data[j].begin(), data[j].end());
					}
					flag=1;
					break;
				}
			}
			if(!flag){
				stringstream hscr;
				hscr << player.getHighestScore();
				std::vector< std::string > vec = {player.getName(), hscr.str()};
				writer.addDatainRow(vec.begin(), vec.end());
			}
		}
	}else
	{
		for (int i = 0; i < data.size(); ++i)
		{
			if(player.getName() == data[i][0]){
				userID=i;
				break;
			}
		}
		for (int i = 0; i <= userID; ++i)
		{
			if(stoi(data[i][1]) >= player.getHighestScore()){
				writer.addDatainRow(data[i].begin(), data[i].end());
			}else{
				stringstream hscr;
				hscr << player.getHighestScore();
				data[userID][1] = hscr.str();
				writer.addDatainRow(data[userID].begin(), data[userID].end());
				for (int j = i; j < data.size(); ++j)
				{
					if(j==userID)
						continue;
					else{
						writer.addDatainRow(data[j].begin(), data[j].end());
					}
				}
				break;
			}
		}
	}
	rename("newlb.csv", "leaderboard.csv");
}

int main(){
	cout<<"Enter your name: ";
	string name;
	cin >> name ;
	Player player(name);
	bool oldUser=true;
	std::vector< std::vector< std::string > > data;
	if(!CheckifAlreadyPresent(name, player, data)){
		printf("Welcome "); sleep(0.5);
		cout << name << " !!\n";
		oldUser=false;
		sleep(1.5);
	}
	sleep(0.4);
	cout << "Starting the Game";
	for (int i = 0; i < 3; ++i)
	{
		printf(".");
		sleep(0.4);
	}
	srand(time(NULL));
	intialize();
	play();
	UpdatePlayerInfo(name, SCORE, player, oldUser, data);
	UpdateLeaderBoard(player, oldUser);
	endwin();
	return 0;
}