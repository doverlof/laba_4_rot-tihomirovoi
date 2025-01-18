/* Программа-заготовка для домашнего задания
*/

#include "hw/l3_DomainLayer.h"

using namespace std;

bool MusicGroup::invariant() const
{
    return !_name.empty() && _name.size() <= MAX_STRING_LENGTH;
}

MusicGroup::MusicGroup(string name)
    : _name(name)
{
    assert(invariant());
}

bool Day::invariant() const
{
    return _beer >= 0;
}

Day::Day(int beer, std::vector<MusicGroup> groups)
    : _beer(beer)
    , _groups(groups)
{
    assert(invariant());
}

Day::Day(int beer)
    : _beer(beer)
{
    assert(invariant());
}

void Day::setGroups(const std::vector<MusicGroup> & groups)
{
    _groups = groups;
}


bool Festival::invariant() const
{
    std::lock_guard locker(_festival_mutex);

    return !_name.empty() && _name.size() <= MAX_STRING_LENGTH;
}

Festival::Festival(const std::string & name)
    : _name(name)
{
    assert(invariant());
}

Festival::Festival(const std::string & name, std::vector<Day> days)
    : _name(name)
    , _days(days)
{
    assert(invariant());
}

const string &Festival::getName() const
{
    std::lock_guard locker(_festival_mutex);
    
    return _name;
}

void Festival::setDays(const std::vector<Day> &days)
{
    std::lock_guard locker(_festival_mutex);

    _days = days;
}

const std::vector<Day> &Festival::getDays() const
{
    std::lock_guard locker(_festival_mutex);

    return _days;
}

bool Festival::write(ostream& os)
{
    std::lock_guard locker(_festival_mutex);
    
    writeString(os, _name);

    size_t day_count = _days.size();
    writeNumber(os, day_count);

    for(const Day & day : _days)
    {
        writeNumber(os, day.getBeer());

        writeNumber(os, day.getGroups().size());

        for (const MusicGroup & group : day.getGroups()){
            writeString(os, group.getName());
        }
    }

    return os.good();
}

shared_ptr<ICollectable> ItemCollector::read(istream& is)
{
    string name      = readString(is, MAX_STRING_LENGTH);
    size_t day_count = readNumber<size_t>(is);

    vector <Day> days;
    vector <MusicGroup> groups;

    days.reserve(day_count);

    for(size_t i=0; i < day_count; ++i)
    {
        int beer = readNumber<int>(is);;

        size_t group_count = readNumber<size_t>(is);   
        groups.reserve(group_count);
        for(size_t j=0; j < group_count; ++j){
            string group_name = readString(is, MAX_STRING_LENGTH);
            groups.push_back(MusicGroup(group_name));
        }
        days.push_back(Day(beer,groups));
        groups.clear();
    }

    shared_ptr<ICollectable> p = std::make_shared<Festival>(name, days);

    return p;
}

Festival &ItemCollector::getFestival(size_t index)
{
    Festival & p = *static_cast<Festival *>(getItem(index).get());

    return p;
}


