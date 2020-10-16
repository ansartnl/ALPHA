#ifndef FACTORY_H
#define FACTORY_H

#include <QtCore/QMap>

// template factory class. Construct Derived class acording provided Key and return Base class pointer on it
template <class Base, class Key>
class Factory
{
    // Helper function to construct derived clases. We cant take direct pointer of class. So we have to use such trick.
    template <class T>
    static Base *standartConstructor()
    {
        return new T;
    }

    typedef Base *(*constructor)();
    QMap<Key, constructor> map; // construction map
    constructor def; // default constructor. Using when construction is not passible

public:
    Factory() : def(0) {}
    Factory(const Factory &other) : map(other.map), def(other.def) {}
    virtual ~Factory() {}

    // registre class on the factory
    template<class Deriv>
    bool reg(const Key &key)
    {
        // if we alredy have the key return if the constructor in the map the same
        if (map.contains(key))
            return constructor(standartConstructor<Deriv>) == map.value(key);

        // otherwise add the new constructor helper function
        map.insert(key, standartConstructor<Deriv>);
        return true;
    }

    // set the default constructor
    template<class Deriv>
    void setDefault()
    {
        def = standartConstructor<Deriv>;
    }

    // remove the default constructor
    void removeDefault()
    {
        def = 0;
    }

    // create derived class acording the key. Created class should be deleted by caller.
    Base *create(const Key &key) const
    {
        if (!map.contains(key))
            return ( def ? def() : 0 );
        return map.value(key)();
    }

    // create all registred classes on the factory except default
    QList<Base *> createAll() const
    {
        QList<Base *> result;
        foreach (constructor c, map)
            result.push_back(c());
        return result;
    }

    // test is key registred or free
    bool isRegistered(const Key &key) const
    {
        return map.contains(key);
    }

    // free the key
    void unRegister(const Key &key)
    {
        map.remove(key);
    }

    // test using derived class
    template<class Deriv>
    bool isRegistered() const
    {
        const constructor searchedConstructor = standartConstructor<Deriv>;
        foreach (const constructor &c, map)
            if (c == searchedConstructor)
                return true;
        return false;
    }

    // unregister using derived class
    template<class Deriv>
    int unRegister()
    {
        QList<Key> keys = map.keys(constructor(standartConstructor<Deriv>));
        foreach (const Key &c, keys)
            map.remove(c);
        return keys.size();
    }
};

#endif // FACTORY_H
