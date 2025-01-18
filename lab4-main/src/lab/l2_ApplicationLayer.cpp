/* Программа-заготовка для домашнего задания
*/

#include "hw/l2_ApplicationLayer.h"

#include <algorithm>
#include <vector>
#include <thread>

using namespace std;

inline const string DATA_DEFAULT_NAME = "lab.data";

void Application::work()
{
    std::vector<std::string> args = split(_command);

    if (args.empty()) return;

    if (args[0] == "c" || args[0] == "clean")
    {
        if (args.size() != 1)
        {
            _out.Output("Некорректное количество аргументов команды clean");
            return;
        }

        _col.clean();

        return;
    }

    if (args[0] == "af" || args[0] == "add_festival")
    {
        if (args.size() != 2)
        {
            _out.Output("Некорректное количество аргументов команды add_festival");
            return;
        }

        _col.addItem(make_shared<Festival>(args[1].c_str()));
        return;
    }

    if (args[0] == "ad" || args[0] == "add_day")
    {
        if (args.size() != 3)
        {
            _out.Output("Некорректное количество аргументов команды add_day");
            return;
        }

        Festival & festival = _col.getFestival(stoul(args[1]));

        vector<Day> days = festival.getDays();
        days.push_back(Day(stoul(args[2])));
        festival.setDays(days);

        return;
    }

    if (args[0] == "ag" || args[0] == "add_group")
    {
        if (args.size() != 4)
        {
            _out.Output("Некорректное количество аргументов команды add_group");
            return;
        }

        Festival & festival = _col.getFestival(stoul(args[1]));
        Day & day = festival.getDay(stoul(args[2]));

        vector<MusicGroup> group = day.getGroups();
        group.push_back(MusicGroup(args[3]));
        day.setGroups(group);

        return;
    }


    if (args[0] == "r" || args[0] == "remove")
    {
        if (args.size() != 2)
        {
            _out.Output("Некорректное количество аргументов команды remove");
            return;
        }

        _col.removeItem(stoul(args[1]));
        return;
    }

    if (args[0] == "u" || args[0] == "update")
    {
        if (args.size() != 3)
        {
            _out.Output("Некорректное количество аргументов команды update");
            return;
        }

        _col.updateItem(stoul(args[1]), make_shared<Festival>(args[2].c_str()));
        return;
    }

    if (args[0] == "v" || args[0] == "view")
    {
        if (args.size() != 1)
        {
            _out.Output("Некорректное количество аргументов команды view");
            return;
        }

        size_t festival_count = 0;
        for(size_t i=0; i < _col.getSize(); ++i)
        {
            if (!_col.isRemoved(i))
            {
                const Festival & item = _col.getFestival(i);

                _out.Output("[" + to_string(i) + "] "+ item.getName());

                size_t day_count = 0;
                const vector<Day> days = item.getDays();
                for(const Day & day : days){
                    _out.Output(" [" + to_string(day_count) + "] "+ to_string(day.getBeer()));

                    size_t group_count = 0;
                    const vector<MusicGroup> groups = day.getGroups();
                    for (const MusicGroup & group : groups){
                        _out.Output("   [" + to_string(group_count) + "] "+ group.getName());
                        group_count ++;
                    }
                    day_count ++;
                }

                festival_count ++;
            }
        }

        _out.Output("Количество элементов в коллекции: " + to_string(festival_count));
        return;
    }

    if (args[0] == "rp" || args[0] == "report")
    {
        if (args.size() != 1)
        {
            _out.Output("Некорректное количество аргументов команды report");
            return;
        }

        vector<pair<size_t, std::vector<MusicGroup>>> day_set;
        std::string groupNames;

        _out.Output("Report:");

        for(size_t i=0; i < _col.getSize(); ++i)
        {
            if (!_col.isRemoved(i))
            {
                const Festival & p = _col.getFestival(i);
                for(const Day & k : p.getDays()){
                    day_set.push_back({k.getBeer(), k.getGroups()});
                }

                sort(day_set.begin(),day_set.end(),[](pair<size_t,std::vector<MusicGroup>> p1, pair<size_t,std::vector<MusicGroup>> p2){
                    return p1.first > p2.first;
                });

                size_t day_count = 1;
                _out.Output("Festival: " +  p.getName() +  "\n  days:");
                for(const auto & [beer, groups] : day_set){

                    for (size_t j = 0; j < groups.size(); ++j){
                        groupNames = groupNames + groups[j].getName() + ' ';
                    }
                    _out.Output("  day" + to_string(day_count) + ": " + to_string(beer) + " sold beer, groups: " + groupNames);
                    groupNames.clear();
                    day_count ++;
                }
                day_set.clear();
            }
        }
    
        return;
    }



    

    _out.Output("Недопустимая команда '" + args[0] + "'");
    return;
}

std::vector<std::string> Application::split(const std::string & str)
{
    std::vector<std::string> res;
    size_t                   start_pos = 0, 
                             end_pos   = 0;

    while(end_pos < str.size()) 
    {
        for(start_pos = end_pos; start_pos < str.size(); ++start_pos)
            if (str[start_pos] != ' ')
                break;

        if (start_pos == str.size())
            return res;

        for(end_pos = start_pos; end_pos < str.size(); ++end_pos)
            if (str[end_pos] == ' ')
                break;

        res.push_back(str.substr(start_pos,end_pos-start_pos));
    }

    return res;
}
