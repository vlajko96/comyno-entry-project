#ifndef _PG_COMMON_H_
#define _PG_COMMON_H_

#include <iostream>
#include <string>
#include <unordered_map>
#include <thread>
#include <functional>

#include <pqxx/pqxx>
#include <CMLog.h>
#include <ThreadPool.h>

namespace pgClient {

struct Message {
    int id;
    std::string payload;
    std::string timestamp;
};

enum class PGStatus {
    PG_SUCCESS,
    PG_ERROR
};

using pgSelectCallback = std::function<void(std::vector<Message>)>;

}

#endif  /* _PG_COMMON_H_ */
