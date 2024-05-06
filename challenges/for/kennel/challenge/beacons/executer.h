#ifndef EXECUTER_H
#define EXECUTER_H
#include <queue>

#include "beacon.h"


class executer_t {
public:
    executer_t();
    bool execute_beacon();
    void add_task(beacon_t& beacon);
private:
    std::queue<beacon_t> beacon_loop_;
};



#endif //EXECUTER_H
