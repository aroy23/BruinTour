#ifndef HASHMAP_H
#define HASHMAP_H

#include <string>
#include <list>
#include <iostream>
#include <functional>

template <typename T>
class HashMap
{
public:
    HashMap(double max_load = 0.75); // constructor
    
    ~HashMap(); // destructor; deletes all of the items in the hashmap
    
    int size() const; // return the number of associations in the hashmap
    
    // The insert method associates one item (key) with another (value).
    // If no association currently exists with that key, this method inserts
    // a new association into the hashmap with that key/value pair. If there is
    // already an association with that key in the hashmap, then the item
    // associated with that key is replaced by the second parameter (value).
    // Thus, the hashmap must contain no duplicate keys.
    void insert(const std::string& key, const T& value);
    
    // Defines the bracket operator for HashMap, so you can use your map like this:
    // your_map["david"] = 2.99;
    // If the key does not exist in the hashmap, this will create a new entry in
    // the hashmap and map it to the default value of type T. Then it will return a
    // reference to the newly created value in the map.
    T& operator[](const std::string& key);
    
    // If no association exists with the given key, return nullptr; otherwise,
    // return a pointer to the value associated with that key. This pointer can be
    // used to examine that value within the map.
    const T* find(const std::string& key) const;
    
    // If no association exists with the given key, return nullptr; otherwise,
    // return a pointer to the value associated with that key. This pointer can be
    // used to examine that value or modify it directly within the map.
    T* find(const std::string& key) {
    const auto& hm = *this;
    return const_cast<T*>(hm.find(key));
    }
    
    HashMap(const HashMap&) = delete;
    HashMap& operator=(const HashMap&) = delete;
private:
    struct LocationAssociations
    {
        std::string m_key;
        T m_value;
    };
    
    std::list<LocationAssociations>* m_openHashMap;
    int m_buckets;
    int m_size;
    double m_maxLoad;
    
    unsigned int getBucket(const std::string &hashMe) const;
};

template<typename T>
HashMap<T>::HashMap(double max_load)
{
    m_size = 0;
    m_buckets = 10;
    m_openHashMap = new std::list<LocationAssociations>[m_buckets];
    m_maxLoad = max_load;
}

template<typename T>
HashMap<T>::~HashMap()
{
    delete[] m_openHashMap;
}

template<typename T>
T& HashMap<T>::operator[](const std::string& key)
{
    if(find(key) != nullptr)
    {
        return *find(key);
    }
    else
    {
        T t = T();
        insert(key, t);
        return *find(key);
    }
}

template<typename T>
int HashMap<T>::size() const
{
    return m_size;
}

template<typename T>
const T* HashMap<T>::find(const std::string& key) const
{
    unsigned int bucketOfKey = getBucket(key);
    typename std::list<LocationAssociations>::iterator p;
    for(p = m_openHashMap[bucketOfKey].begin(); p != m_openHashMap[bucketOfKey].end(); p++)
    {
        if((*p).m_key == key)
        {
            return &((*p).m_value);
        }
    }
    return nullptr;
}

template<typename T>
void HashMap<T>::insert(const std::string &key, const T& value)
{
    if(find(key) != nullptr)
    {
        T* oldVal = find(key);
        *oldVal = value;
    }
    else
    {
        unsigned int insertHere = getBucket(key);
        LocationAssociations loc;
        loc.m_key = key;
        loc.m_value = value;
        m_openHashMap[insertHere].push_back(loc);
        m_size++;
    }
    
    double tempSize = m_size;
    double load = tempSize/m_buckets;
    if(load > m_maxLoad)
    {
        m_buckets *= 2;
        std::list<LocationAssociations>* reallocated = new std::list<LocationAssociations>[m_buckets];
        for(int i = 0; i < m_buckets/2; i++)
        {
            
            while(!m_openHashMap[i].empty())
            {
                typename std::list<LocationAssociations>::iterator p;
                p = m_openHashMap[i].begin();
                unsigned int insertHere = getBucket((*p).m_key);
                reallocated[insertHere].splice(reallocated[insertHere].begin(), m_openHashMap[i], p);
            }
        }
        delete[] m_openHashMap;
        m_openHashMap = reallocated;
    }
}

template<typename T>
unsigned int HashMap<T>::getBucket(const std::string &hashMe) const
{
    std::hash<std::string> str_hash;
    size_t hashValue = str_hash(hashMe);
    unsigned int bucketNum = hashValue % m_buckets;
    return bucketNum;
}

#endif // HASHMAP_H
