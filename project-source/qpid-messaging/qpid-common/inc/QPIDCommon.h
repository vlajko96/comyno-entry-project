#ifndef _QPID_COMMON_H_
#define _QPID_COMMON_H_

#include <iostream>
#include <string>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <functional>

#include <qpid/messaging/Connection.h>
#include <qpid/messaging/Message.h>
#include <qpid/messaging/Receiver.h>
#include <qpid/messaging/Sender.h>
#include <qpid/messaging/Session.h>
#include <qpid/messaging/Address.h>
#include <qpid/messaging/Duration.h>

#define INVALID_MESSAGE "INVALID MESSAGE"

namespace qpidMessaging {

enum class QPIDStatus {
    QPID_SUCCESS,
    QPID_ERROR,
    QPID_ALREADY,
    QPID_INVALID_PARAM
};

using qpidSendCallback = std::function<void(std::string, std::string)>;
using qpidReceiveCallback = std::function<void(std::string, std::string, std::string)>;

}

#endif  /* _QPID_COMMON_H_ */
