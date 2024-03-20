#include <QPIDMessaging.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <exception>

using ::testing::Return;
using ::testing::_;
using ::testing::Throw;
using ::testing::AnyNumber;

std::unique_ptr<qpidMessaging::QPIDMessaging> qpidMessagingClient;
qpidMessaging::qpidReceiveCallback receiveCallback_;
std::string message_;
std::string exchange_;
std::string replyAddress_;
std::string broker_ = "localhost:5672";
bool callbackCalled;

void receiveCallback(std::string message, std::string exchange, std::string replyAddress) {
    message_ = message;
    exchange_ = exchange;
    replyAddress_ = replyAddress;
    callbackCalled = true;
}

class MockQPIDBrokerConnector : public qpidMessaging::IQPIDBrokerConnector {
 public:
    MOCK_METHOD(qpid::messaging::Connection, createConnection, (std::string broker), (override));
    MOCK_METHOD(void, openConnection, (qpid::messaging::Connection& connection), (override));
    MOCK_METHOD(bool, connectionIsOpen, (qpid::messaging::Connection& connection), (override));
    MOCK_METHOD(void, closeConnection, (qpid::messaging::Connection& connection), (override));
    MOCK_METHOD(qpid::messaging::Session, createSession, (qpid::messaging::Connection& connection), (override));
    MOCK_METHOD(void, sessionAcknowledge, (qpid::messaging::Session& session), (override));
    MOCK_METHOD(qpid::messaging::Sender, createSender, (qpid::messaging::Session& session, std::string exchange), (override));
    MOCK_METHOD(void, senderSend, (qpid::messaging::Sender& sender, qpid::messaging::Message& message), (override));
    MOCK_METHOD(qpid::messaging::Receiver, createReceiver, (qpid::messaging::Session& session, std::string exchange), (override));
    MOCK_METHOD(qpid::messaging::Message, receiverFetch, (qpid::messaging::Receiver& receiver), (override));
};

class TestSetup : public ::testing::Test
{
 protected:
    std::shared_ptr<qpidMessaging::IQPIDBrokerConnector> brokerConnector;

 public:

    void SetUp() override {
        brokerConnector = std::make_shared<MockQPIDBrokerConnector>();
        qpidMessagingClient = std::make_unique<qpidMessaging::QPIDMessaging>();
        qpidMessagingClient->injectBrokerConnector(brokerConnector);
        receiveCallback_ = receiveCallback;
        message_ = "";
        exchange_ = "";
        replyAddress_ = "";
        callbackCalled = false;
    }

    void TearDown() override {
        qpidMessagingClient = nullptr;
        brokerConnector = nullptr;
    }
};

// START test sender
TEST_F(TestSetup, qpidMessagingSendMessage_SUCCESS){
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), createConnection(broker_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), openConnection(_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), createSession(_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), createSender(_, "send-here"))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), senderSend(_, _))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), closeConnection(_))
                                                .Times(1);
    qpidMessaging::QPIDStatus result = qpidMessagingClient->qpidSendMessage("Hi there!", "send-here", "reply-here");
    EXPECT_EQ(result, qpidMessaging::QPIDStatus::QPID_SUCCESS);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

TEST_F(TestSetup, qpidMessagingSendMessage_ERROR){
    qpid::messaging::Message message;
    message.setContent("Hi there!");
    message.setReplyTo(qpid::messaging::Address("reply-here"));
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), createConnection(broker_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), openConnection(_))
                                                .Times(1)
                                                .WillOnce(Throw(std::exception()));
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), closeConnection(_))
                                                .Times(1);
    qpidMessaging::QPIDStatus result = qpidMessagingClient->qpidSendMessage("Hi there!", "send-here", "reply-here");
    EXPECT_EQ(result, qpidMessaging::QPIDStatus::QPID_ERROR);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

TEST_F(TestSetup, qpidMessagingSendInvalidMessage_SUCCESS){
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), createConnection(broker_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), openConnection(_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), createSession(_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), createSender(_, "send-here"))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), senderSend(_, _))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), closeConnection(_))
                                                .Times(1);
    qpidMessaging::QPIDStatus result = qpidMessagingClient->qpidSendInvalidMessage("send-here");
    EXPECT_EQ(result, qpidMessaging::QPIDStatus::QPID_SUCCESS);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}
// END test sender

// START test receiver
TEST_F(TestSetup, qpidMessagingReceiveMessages_SUCCESS){
    qpid::messaging::Message message;
    message.setContent("Hi there!");
    message.setReplyTo(qpid::messaging::Address("reply-here"));
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), createConnection(broker_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), openConnection(_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), createSession(_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), createReceiver(_, "send-here"))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), receiverFetch(_))
                                                .Times(AnyNumber())
                                                .WillRepeatedly(Return(message));
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), sessionAcknowledge(_))
                                                .Times(AnyNumber());
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), closeConnection(_))
                                                .Times(1);
    qpidMessaging::QPIDStatus result = qpidMessagingClient->qpidReceiveMessages("send-here", receiveCallback);
    EXPECT_EQ(result, qpidMessaging::QPIDStatus::QPID_SUCCESS);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    if (!callbackCalled)
    {
        FAIL();
    }
    else
    {
        EXPECT_EQ(message_, "Hi there!");
        EXPECT_EQ(exchange_, "send-here");
        EXPECT_EQ(replyAddress_, "reply-here");
        callbackCalled = false;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

TEST_F(TestSetup, qpidMessagingReceiveMessages_ERROR){
    qpid::messaging::Message message;
    message.setContent("Hi there!");
    message.setReplyTo(qpid::messaging::Address("reply-here"));
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), createConnection(broker_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), openConnection(_))
                                                .Times(1)
                                                .WillOnce(Throw(std::exception()));
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), closeConnection(_))
                                                .Times(1);
    qpidMessaging::QPIDStatus result = qpidMessagingClient->qpidReceiveMessages("send-here", receiveCallback);
    EXPECT_EQ(result, qpidMessaging::QPIDStatus::QPID_ERROR);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

TEST_F(TestSetup, qpidMessagingReceiveMessages_ALREADY){
    qpid::messaging::Message message;
    message.setContent("Hi there!");
    message.setReplyTo(qpid::messaging::Address("reply-here"));
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), createConnection(broker_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), openConnection(_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), createSession(_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), createReceiver(_, "send-here"))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), receiverFetch(_))
                                                .Times(AnyNumber())
                                                .WillRepeatedly(Return(message));
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), sessionAcknowledge(_))
                                                .Times(AnyNumber());
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), closeConnection(_))
                                                .Times(1);
    qpidMessaging::QPIDStatus result = qpidMessagingClient->qpidReceiveMessages("send-here", receiveCallback);
    EXPECT_EQ(result, qpidMessaging::QPIDStatus::QPID_SUCCESS);
    result = qpidMessagingClient->qpidReceiveMessages("send-here", receiveCallback);
    EXPECT_EQ(result, qpidMessaging::QPIDStatus::QPID_ALREADY);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

TEST_F(TestSetup, qpidMessagingReceiveMessages_EXCEPTION){
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), createConnection(broker_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), openConnection(_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), createSession(_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), createReceiver(_, "send-here"))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), receiverFetch(_))
                                                .Times(1)
                                                .WillOnce(Throw(std::exception()));
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), closeConnection(_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), connectionIsOpen(_))
                                                .Times(1);
    qpidMessaging::QPIDStatus result = qpidMessagingClient->qpidReceiveMessages("send-here", receiveCallback);
    EXPECT_EQ(result, qpidMessaging::QPIDStatus::QPID_SUCCESS);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

TEST_F(TestSetup, qpidMessagingReceiveMessagesStop_INVALID_PARAM){
    qpidMessaging::QPIDStatus result = qpidMessagingClient->qpidReceiveStop("send-here");
    EXPECT_EQ(result, qpidMessaging::QPIDStatus::QPID_INVALID_PARAM);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

TEST_F(TestSetup, qpidMessagingReceiveMessagesStop_SUCCESS){
    qpid::messaging::Message message;
    message.setContent("Hi there!");
    message.setReplyTo(qpid::messaging::Address("reply-here"));
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), createConnection(broker_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), openConnection(_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), createSession(_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), createReceiver(_, "send-here"))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), receiverFetch(_))
                                                .Times(AnyNumber())
                                                .WillRepeatedly(Return(message));
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), sessionAcknowledge(_))
                                                .Times(AnyNumber());
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), closeConnection(_))
                                                .Times(1);
    qpidMessaging::QPIDStatus result = qpidMessagingClient->qpidReceiveMessages("send-here", receiveCallback);
    EXPECT_EQ(result, qpidMessaging::QPIDStatus::QPID_SUCCESS);
    result = qpidMessagingClient->qpidReceiveStop("send-here");
    EXPECT_EQ(result, qpidMessaging::QPIDStatus::QPID_SUCCESS);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

TEST_F(TestSetup, qpidMessagingReceiveMessagesStop_EXCEPTION){
    qpid::messaging::Message message;
    message.setContent("Hi there!");
    message.setReplyTo(qpid::messaging::Address("reply-here"));
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), createConnection(broker_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), openConnection(_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), createSession(_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), createReceiver(_, "send-here"))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), receiverFetch(_))
                                                .Times(AnyNumber())
                                                .WillRepeatedly(Return(message));
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), sessionAcknowledge(_))
                                                .Times(AnyNumber());
    EXPECT_CALL(*std::static_pointer_cast<MockQPIDBrokerConnector>(brokerConnector), closeConnection(_))
                                                .Times(1)
                                                .WillOnce(Throw(std::exception()));
    qpidMessaging::QPIDStatus result = qpidMessagingClient->qpidReceiveMessages("send-here", receiveCallback);
    EXPECT_EQ(result, qpidMessaging::QPIDStatus::QPID_SUCCESS);
    result = qpidMessagingClient->qpidReceiveStop("send-here");
    EXPECT_EQ(result, qpidMessaging::QPIDStatus::QPID_ERROR);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}
// END test receiver