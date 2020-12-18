#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <sstream>

struct record {
    int current_health = 0;
    int lowest_health = 0;
    bool Double = false;
    bool Prevent = false;
};


std::vector<std::pair<int, int>> track_last2(int i, int j) {
    std::vector<std::pair<int, int>> last2;
    if (j > 0)
        last2.emplace_back(i, j - 1); // left
    if (i > 0)
        last2.emplace_back(i - 1, j); // top
    return last2;
}

int get_number(std::string s) {
    std::stringstream ss;
    ss << s;
    int i;
    ss >> i;
    return i;
}

record set_record(std::string s, record previous) {
    record r = previous;
    if (s.compare("D") == 0) {
        r.Double = true;
    } else {
        if (s.compare("P") == 0) {
            r.Prevent = true;
        } else {
            int n = get_number(s);
            if (previous.Double && n > 0) {
                if (n > 0) {
                    n *= 2;
                    r.Double = false;
                }
            }
            if (previous.Prevent && n < 0) {
                n = 0;
                r.Prevent = false;
            }
            r.current_health += n;
            if (r.current_health < previous.lowest_health)
                r.lowest_health = r.current_health;
        }
    }
    return r;
}

record set_posPr(std::vector<record> &current_cell, std::string &current_string, std::vector<record>& left_cell, std::vector<record>& top_cell){
    if(!left_cell[3].Prevent && !top_cell[3].Prevent)
        return current_cell[0];
    else{
        record compute;
        if(left_cell[3].Prevent && !top_cell[3].Prevent){
            compute  = set_record(current_string, left_cell[3]);
        }
        else{
            if(!left_cell[3].Prevent && top_cell[3].Prevent)
                compute  = set_record(current_string, top_cell[3]);
            else{
                if(left_cell[3].current_health > top_cell[3].current_health)
                    compute =  set_record(current_string, left_cell[3]);
                else
                    compute =  set_record(current_string, top_cell[3]);
            }
        }
        if(compute.current_health > current_cell[0].current_health)
            return compute;
        else
            return current_cell[0];
    }
}

record set_negDb(std::vector<record> &current_cell, std::string &current_string, std::vector<record>& left_cell, std::vector<record>& top_cell){
    if(!left_cell[2].Double && !top_cell[2].Double)
        return current_cell[0];
    else{
        record compute;
        if(left_cell[2].Double && !top_cell[2].Double){
            compute  = set_record(current_string, left_cell[2]);
        }
        else{
            if(!left_cell[2].Double && top_cell[2].Double)
                compute  = set_record(current_string, top_cell[2]);
            else{
                if(left_cell[2].current_health > top_cell[2].current_health)
                    compute =  set_record(current_string, left_cell[2]);
                else
                    compute =  set_record(current_string, top_cell[2]);
            }
        }
        if(compute.current_health > current_cell[0].current_health)
            return compute;
        else
            return current_cell[0];
    }
}

record find_negDb(std::string &current, std::vector<record>& left_cell, std::vector<record>& top_cell){
    if(!left_cell[2].Double && !top_cell[2].Double)
        return {0, 0, false, false};
    else{
        if(left_cell[2].Double && !top_cell[2].Double){
            return set_record(current, left_cell[2]);
        }
        else{
            if(!left_cell[2].Double && top_cell[2].Double)
                return set_record(current, top_cell[2]);
            else{
                if(left_cell[2].current_health > top_cell[2].current_health)
                    return set_record(current, left_cell[2]);
                else
                    return set_record(current, top_cell[2]);
            }
        }
    }
}

record find_posPr(std::string &current, std::vector<record>& left_cell, std::vector<record>& top_cell){
    if(!left_cell[3].Prevent && !top_cell[3].Prevent)
        return {0, 0, false, false};
    else{
        if(left_cell[3].Prevent && !top_cell[3].Prevent){
            return set_record(current, left_cell[3]);
        }
        else{
            if(!left_cell[3].Prevent && top_cell[3].Prevent)
                return set_record(current, top_cell[3]);
            else{
                if(left_cell[3].current_health > top_cell[3].current_health)
                    return set_record(current, left_cell[3]);
                else
                    return set_record(current, top_cell[3]);
            }
        }
    }
}

void set_cell(std::string& current_string, std::vector<record> &current_cell, std::vector<record>& left_cell, std::vector<record>& top_cell){
    record left_max = set_record(current_string, left_cell[0]);
    record left_min = set_record(current_string, left_cell[1]);
    record top_max = set_record(current_string, top_cell[0]);
    record top_min = set_record(current_string, top_cell[1]);
    if (left_max.current_health > top_max.current_health)
        current_cell[0] = left_max;
    else
        current_cell[0] = top_max;
    if (left_min.lowest_health > top_min.lowest_health)
        current_cell[1] = left_min;
    else
        current_cell[1] = top_min;
    if(current_string == "D"){
        current_cell[2] = current_cell[0];
    }else{
        if(current_string == "P"){
            current_cell[3] = current_cell[0];
        }else{
            if(get_number(current_string) < 0){
                current_cell[2] = find_negDb(current_string, left_cell, top_cell);
                current_cell[0] = set_posPr(current_cell, current_string, left_cell, top_cell);
            }
            else{
                current_cell[2] = find_posPr(current_string, left_cell, top_cell);
                current_cell[0] = set_negDb(current_cell, current_string, left_cell, top_cell);
            }
        }
    }

}

unsigned solve(std::vector<std::vector<std::string> > grid) {
    int size = grid.size();
    record init = {0, 0, false, false};
    std::vector<std::vector<std::vector<record>>> r(size,
                                                    std::vector<std::vector<record>>(size,
                                                                                     std::vector<record>(4, init)));
    r[0][0][0] = set_record(grid[0][0], init);
    if (grid[0][0] == "D") {
        r[0][0][2] = r[0][0][0];
    }
    for (int i = 1; i < size; i++) {
        r[0][i][0] = set_record(grid[0][i], r[0][i - 1][0]);
        r[0][i][1] = set_record(grid[0][i], r[0][i - 1][0]);
        r[i][0][0] = set_record(grid[i][0], r[i - 1][0][0]);
        r[i][0][1] = set_record(grid[i][0], r[i - 1][0][0]);
        if(grid[0][i] == "D"){
            r[0][i][2] = r[0][i][0];
        }
        if(grid[i][0] == "D"){
            r[i][0][2] = r[i][0][0];
        }
        if(get_number(grid[i][0]) < 0 || r[i - 1][0][2].Double){
            r[i][0][2] = r[i][0][0];
        }

    }

    for (int i = 1; i < size; i++) {
        for (int j = 1; j < size; j++) {
            std::vector<std::pair<int, int>> last2 = track_last2(i, j);

            record left_max = set_record(grid[i][j], r[last2[0].first][last2[0].second][0]);
            record left_min = set_record(grid[i][j], r[last2[0].first][last2[0].second][1]);
            record top_max = set_record(grid[i][j], r[last2[1].first][last2[1].second][0]);
            record top_min = set_record(grid[i][j], r[last2[1].first][last2[1].second][1]);

            if (left_max.current_health > top_max.current_health)
                r[i][j][0] = left_max;
            else
                r[i][j][0] = top_max;
            if (left_min.lowest_health > top_min.lowest_health)
                r[i][j][1] = left_min;
            else
                r[i][j][1] = top_min;
            if (grid[i][j] == "D") {
                r[i][j][2] = r[i][j][0];
            }
            if (get_number(grid[i][j]) < 0) {
                if (r[last2[0].first][last2[0].second][2].Double) {
                    r[i][j][2] = set_record(grid[i][j], r[last2[0].first][last2[0].second][2]);
                }
                if (r[last2[1].first][last2[1].second][2].Double) {
                    r[i][j][2] = set_record(grid[i][j], r[last2[1].first][last2[1].second][2]);
                }
            } else {
                if (r[last2[0].first][last2[0].second][2].Double) {
                    r[i][j][2] = set_record(grid[i][j], r[last2[0].first][last2[0].second][2]);
                    if (r[i][j][2].current_health > r[i][j][0].current_health) {
                        r[i][j][0] = r[i][j][2];
                    }
                    r[i][j][2] = init;
                }
                if (r[last2[1].first][last2[1].second][2].Double) {
                    r[i][j][2] = set_record(grid[i][j], r[last2[1].first][last2[1].second][2]);
                    if (r[i][j][2].current_health > r[i][j][0].current_health) {
                        r[i][j][0] = r[i][j][2];
                    }
                    r[i][j][2] = init;
                }
            }
        }
    }

    if (r[size - 1][size - 1][0].lowest_health >= 0 || r[size - 1][size - 1][1].lowest_health >= 0)
        return 1;
    else {
        return 0 - std::max(r[size - 1][size - 1][0].lowest_health, r[size - 1][size - 1][1].lowest_health) + 1;
    }
}


int main() {
    std::vector<std::vector<std::string> > grid = {
            {"D", "1", "1"},
            {"-1", "-1", "1"},
            {"1", "10", "-100"}
    };
    std::cout << solve(grid) << std::endl;
    return 0;
}
