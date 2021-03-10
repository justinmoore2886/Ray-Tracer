#include "HitList.h"
#include <fstream>
#include <iostream>

using namespace std;


HitList::HitList() {
}

Hit HitList::getMin() {
    double dist;
    int minI;

    if (isEmpty()) {
        cerr << "HitList empty! Can't getMin." << endl;
        exit(EXIT_FAILURE);
    }

    double minDist = 999999;
    int i = 0;
    for (auto hit : hits) {
        dist = hit.getDistance();
        if (dist < minDist) {
            minDist = dist;
            minI = i;
        }
        i++;
    }

    return hits[minI];
}

void HitList::add(const Hit& hit) {
    hits.push_back(hit);
}
