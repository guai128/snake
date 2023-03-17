#include<iostream>
#include<string>
#include<fstream>
#include<windows.h>

using std::string;
std::pair<string,string> draw_walls(int size);
void write_map(int num);
void preview();


class Map{
public:
    //without wall
    Map() : size{5} {}

    //have wall
    Map(int x) : size{x} {}
    ~Map() {if (fout.is_open()){fout.close();}};

    //open file
    void open_print_file(string f);

    int get_size() {
        return size;
    }
private:
    //地图尺寸
    int size;
    std::ofstream fout;
};

void Map::open_print_file(string f){
    Sleep(100);
    fout.open(f.c_str(),std::ios::out);
    if (fout.bad()) {return;}
    std::pair<string,string> walls {draw_walls(size)};
    //fout.clear();

    fout<<walls.first<<'\n';
    for(int i=2; i!=size; ++i){
        fout<<walls.second<<'\n';
    }
    fout<<walls.first<<'\n';

    //std::cout<<"make map succed\n";
}

//draw_walls
std::pair<string,string> draw_walls(int size){
    if (size < 4){return {"  ","  "};}
    std::string up_down {};
    for(int i=0; i!=size; ++i){
        up_down += "~  ";
    }

    std::string left_right {"~"};
    for(int i=2; i!=size; ++i){
        left_right += "   ";
    }
    left_right += "  ~  ";

    return {up_down,left_right};
}

void write_map(int num){
    //一行有3size+2个
    Map m(num);
    m.open_print_file("draw.txt");
}

//choice best size
void preview(){
    int num {5};
    while(num<60){
        std::cout<<"do: "<<num<<std::endl;
        write_map(num);
        ++num;
    }

    system("pause");
}
