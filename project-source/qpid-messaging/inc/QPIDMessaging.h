#ifndef _QPID_MESSAGING_H_
#define _QPID_MESSAGING_H_

#include <iostream>
#include <string>
#include <unordered_map>
#include <QPIDCommon.h>
#include <QPIDReceiver.h>
#include <QPIDSender.h>

namespace qpidMessaging {

class QPIDMessaging {
 public:
   QPIDMessaging(std::string broker = "localhost:5672");
   virtual ~QPIDMessaging();

   QPIDStatus qpidSendMessage(std::string topic, std::string message, std::string replyAddress = "");
   QPIDStatus qpidSendMessageAndWait(std::string topic, std::string message, std::string replyAddress, uint16_t timeout, qpidSendCallback callback);
   QPIDStatus qpidSendInvalidMessage(std::string topic);
   QPIDStatus qpidReceiveMessages(std::string topic, qpidReceiveCallback callback);
   QPIDStatus qpidReceiveStop(std::string topic);

 private:
   std::string mBroker;
   std::unordered_map<std::string, std::shared_ptr<QPIDReceiver>> activeReceivers;
};

}

#endif  /* _QPID_MESSAGING_H_ */