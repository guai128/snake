#include<iostream>
#include<string>
#include<fstream>
#include<windows.h>
#include<random>
#include<conio.h>
#include<thread>
#include"map.h"

struct Point;
struct Food;
class Snake;
void draw_snake_food(Snake& snake, Food& food, string& which_file, int map_size);
bool is_great_position(Point p, std::vector<Point>& live_snake);
Point make_food(std::vector<Point>& live_snake, int map_size);
void run_snake();
void check_input_requirement();
void minitor_keyboard(Snake& s);

struct Point{
    int x;
    int y;
    bool operator==(Point p1) {return x == p1.x && y == p1.y;}
};

//bool is_great_position(Point p, std::vector<Point>& live_snake);

struct Food{
    Point pos;
    Food(Point&& p){pos = p;}
};


class Snake{
public:
    enum Direction{up,down,left,right};    

    Snake(int x,int y){
        current.push_back({x,y});
        my_direction = Direction::left;
    }

    std::vector<Point> current_snake();
    void add_node_and_move();
    bool is_live(int map_size);
    void snake_move();
    bool success_eat(Food& f);
    void change_direction(string s);
private:
    Direction my_direction;
    std::vector<Point> current;
};

std::vector<Point> Snake::current_snake(){
    return current;
}

void Snake::change_direction(string s){
    if (s == "up"){
        my_direction = Direction::up;
    }
    else if(s == "down"){
        my_direction = Direction::down;
    }
    else if(s == "left"){
        my_direction = Direction::left;
    }
    else{
        my_direction = Direction::right;
    }
}

void Snake::add_node_and_move(){
    Point tmp = *(current.end()-1);
    snake_move();
    current.push_back(tmp);
}

void Snake::snake_move(){
    //forward the snake node
    for (int i=current.size()-1; i!=0; --i){
        current[i] = current[i-1];
    }

    //forward the snake'map_size head
    switch (my_direction)
    {
    case Direction::left:
        --current[0].x;
        break;
    case Direction::right:
        ++current[0].x;
        break;
    case Direction::down:
        ++current[0].y;
        break;
    case Direction::up:
        --current[0].y;
        break;
    default:
        break;
    }
}

bool Snake::success_eat(Food& f){
    return current[0].x == f.pos.x && current[0].y == f.pos.y;
}

bool Snake::is_live(int map_size){
    for (int i=3; i!=current.size(); ++i){if (current[0] == current[i]) return false;}
    return 1 < current[0].x && current[0].x < map_size*3-2 && 1 < current[0].y && current[0].y < map_size*3-2; 
}

bool is_great_position(Point p, std::vector<Point>& live_snake){
    if (p.x == 0){return false;}
    for(auto& x:live_snake){
        if (p == x){return false;}
    }

    return true;
}

void minitor_keyboard(Snake& my_snake){
    while (true){
        //change direction
        char ch {};
        if (_kbhit()){//如果有按键按下，则_kbhit()函数返回真
            ch = _getch();//使用_getch()函数获取按下的键值
            //当按下ESC时循环，ESC键的键值时27.
            if (ch == 'a'){my_snake.change_direction("left");}
            else if(ch == 'd'){my_snake.change_direction("right");}
            else if(ch == 'w'){my_snake.change_direction("up");}
            else if(ch == 's'){my_snake.change_direction("down");}
            else{
                std::cout<<"shiiriiiadfasdfs"<<'\n';
                continue;
            }
        }
    }
}

Point make_food(std::vector<Point>& live_snake, int map_size){
    Point re {20,24};
    /*
        std::random_device rd;  //如果可用的话，从一个随机数发生器上获得一个真正的随机数
    std::mt19937 gen(rd()); //gen是一个使用rd()作种子初始化的标准梅森旋转算法的随机数发生器
    std::uniform_int_distribution<> x_axis(2, 3*map_size-3);
    std::uniform_int_distribution<> y_axis(2, map_size - 1);
    while(!is_great_position(re, live_snake)){
        re.x = x_axis(gen);
        re.y = y_axis(gen);
    }
    */
    return re;
}

void run_snake(){
    using namespace std;
    cout<<"choice the size of map you wanted(32 recommended)\n";
    int wanted_map_size {};
    cin>>wanted_map_size;
    //make and draw map
    write_map(wanted_map_size);
    //make sanke
    Snake my_snake(3*(wanted_map_size - 1)/2,wanted_map_size/2);
    vector<Point> current_snake {my_snake.current_snake()};
    //make food
    Food snake_food {make_food(current_snake, wanted_map_size)};
    //which_file to print
    string where_to_print {"draw.txt"};
    draw_snake_food(my_snake,snake_food,where_to_print,wanted_map_size);
    thread t1(minitor_keyboard, ref(my_snake));
    t1.detach();
    while(true){
            Sleep(1000/60);
        //condition check
        if (1>2){ //my_snake.is_live(wanted_map_size)
            current_snake = my_snake.current_snake();
            cout<<"Game over"<<endl;
            cout<<"Your score is "<<current_snake.size()<<endl;
            cout<<"Do you want to try again?(Yes/No)   ";
            string input_string {};
            cin>>input_string;
            if (input_string == "Yes"){check_input_requirement();}
            else {cout<<"See you later\n";}
            break; 
        }
        //snake move and eat food
        if(my_snake.success_eat(snake_food)){
            current_snake = my_snake.current_snake();
            my_snake.add_node_and_move();
            snake_food = make_food(current_snake, wanted_map_size);
        }
        else {my_snake.snake_move();}
        //cout<<"live"<<endl;

        //snake draw 
        write_map(wanted_map_size);
        draw_snake_food(my_snake,snake_food,where_to_print,wanted_map_size);

    }
}

void check_input_requirement(){
    using namespace std;
    string out {"chioce time"};
    cout<<out<<endl;
    cout<<"do you wanna to preview the map?(Yes/No)   ";
    string in {};
    cin>>in;
    if(in == "Yes"){
        cout<<"\nOK, that is go, look at the document \"draw.txt\"\n";
        preview();
        run_snake();
    }
    else if(in == "No"){
        cout<<"\nOK, game begin, look at the document \"draw.txt\"\n";
        run_snake();
    }
    else{
        cout<<"\nSorry, bad input, input angin\n";
        check_input_requirement();
    }
}

void draw_snake_food(Snake& snake, Food& food, string& which_file, int map_size){
    using namespace std;

    ofstream fout;
    fout.open(which_file.c_str(), ios::binary | ios::in | ios::out);
    if (fout.bad()) {return;}   

    //draw food and subsitite the char in this position
    //just one point
    Point subsitite_food {food.pos};
    fout.seekp((subsitite_food.y - 1)*(3*map_size+2)+subsitite_food.x-1,ios_base::beg);
    fout.write("f",1);

    //draw snake and subsitite the char in this position
    vector<Point> subsitite_snake {snake.current_snake()};
    for(auto& p:subsitite_snake){
        fout.seekp((p.y - 1)*(3*map_size+2)+p.x-1,ios_base::beg);
        fout.write("s",1);
    }

    fout.close();
    //cout<<"make snake and food succed\n";
}

int main(){
    check_input_requirement();
    
    return 0;
}

//随机有问题
