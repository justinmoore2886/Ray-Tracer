#if !defined(_HITLIST_H_)

#define _HITLIST_H_

#include <vector>
#include "Hit.h"

class HitList {
public:
    HitList();
    Hit getMin();
    bool isEmpty() {return hits.size() == 0;};
    void add(const Hit& hit);
    void clear() {hits.clear();}
    int size() {return (int)hits.size(); }

private:
    vector<Hit> hits;
};

#endif
