/* 
MIT License 

Copyright (c) 2019 МГТУ им. Н.Э. Баумана, кафедра ИУ-6, Михаил Фетисов, 

Программа-заготовка для домашнего задания
*/

#ifndef HW_L3_DOMAIN_LAYER_H
#define HW_L3_DOMAIN_LAYER_H

#include "hw/l4_InfrastructureLayer.h"

#include <mutex>

const size_t MAX_STRING_LENGTH  = 50;

class MusicGroup
{
    std::string _name;

protected:
    bool invariant() const;

public:
    MusicGroup() = delete;
    MusicGroup(std::string name);

    std::string getName() const { return _name; }
};

class Day 
{
    int _beer;
    std::vector<MusicGroup> _groups;

protected:
    bool invariant() const;

public:
    Day() = delete;

    Day(int);
    Day(int, std::vector<MusicGroup>);

    int getBeer() const { return _beer; }
    const std::vector<MusicGroup> &getGroups() const { return _groups; }

    void setGroups(const std::vector<MusicGroup> & music_groups);

};

class Festival : public ICollectable
{
    std::string      _name;
    std::vector<Day> _days;
    mutable std::mutex _festival_mutex;

protected:
    bool invariant() const;

public:
    Festival() = delete;
    Festival(const Festival & p) = delete;

    Festival(const std::string & name);
    Festival(const std::string & name, std::vector<Day> days);

    const std::string & getName() const;

    void setDays(const std::vector<Day> & days);
    const std::vector<Day> &getDays() const;
    Day &getDay(int index){ return _days[index]; }


    virtual bool write(std::ostream& os) override;
};


class ItemCollector: public ACollector
{
public:
    virtual std::shared_ptr<ICollectable> read(std::istream& is) override;

    Festival & getFestival(size_t index);
};

#endif // HW_L3_DOMAIN_LAYER_H
