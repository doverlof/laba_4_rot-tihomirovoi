/* Программа-заготовка для домашнего задания
*/

#include "hw/l4_InfrastructureLayer.h"

using namespace std;

string readString(istream& is, size_t max_string_length)
{
    uint16_t len = readNumber<uint16_t>(is);

    assert(len <= max_string_length);

    std::string str(len,' ');
    if (len > 0)
        is.read(str.data(), len);


    return str;
}

void writeString(ostream& os, const string& s)
{
    uint16_t len = s.length();

    writeNumber(os, len);

    os.write(s.c_str(), len);
}


shared_ptr<ICollectable> ACollector::getItem(size_t index) const
{
    std::lock_guard locker(_mutex);
    assert(index < _items.size());

    return _items[index];
}

bool ACollector::isRemoved(size_t index) const
{
    std::lock_guard locker(_mutex);
    assert(index < _removed_signs.size());

    return _removed_signs[index];
}

void ACollector::addItem(shared_ptr<ICollectable> item)
{
    std::lock_guard locker(_mutex);
    _items.emplace_back(item);
    _removed_signs.emplace_back(false);
}

void ACollector::removeItem(size_t index)
{
    std::lock_guard locker(_mutex);
    assert(index < _items.size());
    assert(index < _removed_signs.size());

    if (!_removed_signs[index])
    {
        _removed_signs[index] = true;
        _removed_count ++;
    }
}

void ACollector::updateItem(size_t index, const shared_ptr<ICollectable> & item)
{
    std::lock_guard locker(_mutex);
    assert(index < _items.size());

    _items[index] = item;
}

void ACollector::clean()
{
    std::lock_guard locker(_mutex);
    _items.clear();
    _removed_signs.clear();
}

bool ACollector::loadCollection(const string file_name)
{
    _file_name = file_name;
    ifstream ifs (file_name, ios_base::binary);

    if (!ifs)
        return false;

    size_t count = readNumber<size_t>(ifs);

    _items.reserve(count);

    for(size_t i=0; i < count; ++i)
        addItem(read(ifs));

    assert(invariant());

    return ifs.good();
}

bool ACollector::saveCollection() const
{
    assert(invariant());

    ofstream ofs (_file_name, ios_base::binary);

    if (!ofs)
        return false;

    assert(_items.size() >= _removed_count);
    size_t count = _items.size() - _removed_count;

    writeNumber(ofs, count);

    for(size_t i=0; i < _items.size(); ++i)
        if (!_removed_signs[i])
            _items[i]->write(ofs);

    return ofs.good();
}

std::shared_ptr<ICollectable> ACollector::getItem(size_t index)
{
    std::lock_guard locker(_mutex);
    assert(index < _items.size());

    return _items[index];

}


