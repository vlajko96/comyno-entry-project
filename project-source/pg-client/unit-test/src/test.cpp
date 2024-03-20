#include <PGClient.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <exception>

using ::testing::Return;
using ::testing::_;
using ::testing::Throw;
using ::testing::AnyNumber;

std::unique_ptr<pgClient::PGClient> postgresClient;
pgClient::pgSelectCallback selectCallback_;
pgClient::Message message_;
std::string database_ = "dbname=comynoDB user=comyno password=comyno hostaddr=127.0.0.1 port=5432";
bool callbackCalled;

void selectCallback(std::vector<pgClient::Message> messages) {
    message_ = messages[0];
    callbackCalled = true;
}

class MockPGConnector : public pgClient::IPGConnector {
public:
    MOCK_METHOD(std::shared_ptr<pqxx::connection>, openConnection, (std::string database), (override));
    MOCK_METHOD(std::shared_ptr<pqxx::work>, createTransaction, (pqxx::connection& connection), (override));
    MOCK_METHOD(pqxx::result, executeQuery, (pqxx::work& transaction, std::string query), (override));
    MOCK_METHOD(void, prepareStatement, (pqxx::connection& connection, std::string name, std::string query), (override));
    MOCK_METHOD(void, executePrepared, (pqxx::work& transaction, std::string name), (override));
    MOCK_METHOD(void, commitTransaction, (pqxx::work& transaction), (override));
    MOCK_METHOD(std::string, quoteString, (pqxx::work& transaction, std::string str), (override));
    MOCK_METHOD(bool, resultIsEmpty, (pqxx::result& result), (override));
    MOCK_METHOD(int, resultGetFirstInt, (pqxx::result& result), (override));
    MOCK_METHOD(std::vector<pgClient::Message>, resultGetAll, (pqxx::result& result), (override));
};

class TestSetup : public ::testing::Test
{
 protected:
    std::shared_ptr<pgClient::IPGConnector> pgConnector;

 public:

    void SetUp() override {
        pgConnector = std::make_shared<MockPGConnector>();
        postgresClient = std::make_unique<pgClient::PGClient>();
        postgresClient->injectPGConnector(pgConnector);
        selectCallback_ = selectCallback;
        callbackCalled = false;
    }

    void TearDown() override {
        postgresClient = nullptr;
        pgConnector = nullptr;
    }
};

// START test persist
TEST_F(TestSetup, pgClientPersistMessage_SUCCESS){
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), openConnection(database_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), createTransaction(_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), quoteString(_, "Hello my friend!"))
                                                .Times(1)
                                                .WillOnce(Return("\'Hello my friend!\'"));
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), executeQuery(_, "INSERT INTO Messages (payload) VALUES ('Hello my friend!')"))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), commitTransaction(_))
                                                .Times(1);
    pgClient::PGStatus result = postgresClient->pgMessagePersist("Hello my friend!");
    EXPECT_EQ(result, pgClient::PGStatus::PG_SUCCESS);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

TEST_F(TestSetup, pgClientPersistMessageOnExchange_SUCCESS){
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), openConnection(database_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), createTransaction(_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), quoteString(_, "send-queue"))
                                                .Times(1)
                                                .WillOnce(Return("\'send-queue\'"));
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), executeQuery(_, "SELECT ID FROM Exchanges WHERE exchange_name = 'send-queue'"))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), resultIsEmpty(_))
                                                .Times(1)
                                                .WillOnce(Return(false));
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), resultGetFirstInt(_))
                                                .Times(1)
                                                .WillOnce(Return(1));
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), quoteString(_, "Hello my friend!"))
                                                .Times(1)
                                                .WillOnce(Return("\'Hello there!\'"));
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), executeQuery(_, "INSERT INTO Messages (payload, exchange_id) VALUES ('Hello there!' , 1)"))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), commitTransaction(_))
                                                .Times(1);
    pgClient::PGStatus result = postgresClient->pgMessagePersist("Hello my friend!", "send-queue");
    EXPECT_EQ(result, pgClient::PGStatus::PG_SUCCESS);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

TEST_F(TestSetup, pgClientPersistMessageOnExchange_ERROR){
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), openConnection(database_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), createTransaction(_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), quoteString(_, "invalid"))
                                                .Times(1)
                                                .WillOnce(Return("\'invalid\'"));
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), executeQuery(_, "SELECT ID FROM Exchanges WHERE exchange_name = 'invalid'"))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), resultIsEmpty(_))
                                                .Times(1)
                                                .WillOnce(Return(true));
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), commitTransaction(_))
                                                .Times(1);
    pgClient::PGStatus result = postgresClient->pgMessagePersist("Hello my friend!", "invalid");
    EXPECT_EQ(result, pgClient::PGStatus::PG_ERROR);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

TEST_F(TestSetup, pgClientPersistMessageOnExchange_EXCEPTION){
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), openConnection(database_))
                                                .Times(1)
                                                .WillOnce(Throw(std::exception()));
    pgClient::PGStatus result = postgresClient->pgMessagePersist("Hello my friend!", "invalid");
    EXPECT_EQ(result, pgClient::PGStatus::PG_ERROR);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}
// END test persist

// START test query
TEST_F(TestSetup, pgClientQueryMessages_SUCCESS){
    std::vector<pgClient::Message> messageArray;
    messageArray.emplace_back(pgClient::Message{1, "Hello there!", "20.03.2024."});
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), openConnection(database_))
                                                .Times(2);
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), createTransaction(_))
                                                .Times(2);
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), prepareStatement(_, "set_iso_level", "SET TRANSACTION ISOLATION LEVEL SERIALIZABLE"))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), executePrepared(_, "set_iso_level"))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), executeQuery(_, "SELECT ID, payload, created_at FROM Messages"))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), resultGetAll(_))
                                                .Times(1)
                                                .WillOnce(Return(messageArray));
    pgClient::PGStatus result = postgresClient->pgMessageQuery(selectCallback_);
    EXPECT_EQ(result, pgClient::PGStatus::PG_SUCCESS);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    if (!callbackCalled)
    {
        FAIL();
    }
    else
    {
        EXPECT_EQ(message_.id, 1);
        EXPECT_EQ(message_.payload, "Hello there!");
        EXPECT_EQ(message_.timestamp, "20.03.2024.");
        callbackCalled = false;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

TEST_F(TestSetup, pgClientQueryMessagesOnExchange_SUCCESS){
    std::vector<pgClient::Message> messageArray;
    messageArray.emplace_back(pgClient::Message{1, "Hello there!", "20.03.2024."});
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), openConnection(database_))
                                                .Times(2);
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), createTransaction(_))
                                                .Times(2);
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), quoteString(_, "send-queue"))
                                                .Times(1)
                                                .WillOnce(Return("\'send-queue\'"));
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), executeQuery(_, "SELECT ID FROM Exchanges WHERE exchange_name = 'send-queue'"))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), resultIsEmpty(_))
                                                .Times(1)
                                                .WillOnce(Return(false));
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), resultGetFirstInt(_))
                                                .Times(1)
                                                .WillOnce(Return(1));
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), prepareStatement(_, "set_iso_level", "SET TRANSACTION ISOLATION LEVEL SERIALIZABLE"))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), executePrepared(_, "set_iso_level"))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), executeQuery(_, "SELECT ID, payload, created_at FROM Messages WHERE exchange_id = 1"))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), resultGetAll(_))
                                                .Times(1)
                                                .WillOnce(Return(messageArray));
    pgClient::PGStatus result = postgresClient->pgMessageQuery(selectCallback_, "send-queue");
    EXPECT_EQ(result, pgClient::PGStatus::PG_SUCCESS);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    if (!callbackCalled)
    {
        FAIL();
    }
    else
    {
        EXPECT_EQ(message_.id, 1);
        EXPECT_EQ(message_.payload, "Hello there!");
        EXPECT_EQ(message_.timestamp, "20.03.2024.");
        callbackCalled = false;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

TEST_F(TestSetup, pgClientQueryMessagesOnExchange_ERROR){
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), openConnection(database_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), createTransaction(_))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), quoteString(_, "invalid"))
                                                .Times(1)
                                                .WillOnce(Return("\'invalid\'"));
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), executeQuery(_, "SELECT ID FROM Exchanges WHERE exchange_name = 'invalid'"))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), resultIsEmpty(_))
                                                .Times(1)
                                                .WillOnce(Return(true));
    pgClient::PGStatus result = postgresClient->pgMessageQuery(selectCallback_, "invalid");
    EXPECT_EQ(result, pgClient::PGStatus::PG_ERROR);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

TEST_F(TestSetup, pgClientQueryMessagesOnExchange_EXCEPTION){
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), openConnection(database_))
                                                .Times(1)
                                                .WillOnce(Throw(std::exception()));
    pgClient::PGStatus result = postgresClient->pgMessageQuery(selectCallback_, "send-queue");
    EXPECT_EQ(result, pgClient::PGStatus::PG_ERROR);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

TEST_F(TestSetup, pgClientQueryMessagesOnExchange_THREAD_EXCEPTION){
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), openConnection(database_))
                                                .Times(2);
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), createTransaction(_))
                                                .Times(2);
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), quoteString(_, "send-queue"))
                                                .Times(1)
                                                .WillOnce(Return("\'send-queue\'"));
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), executeQuery(_, "SELECT ID FROM Exchanges WHERE exchange_name = 'send-queue'"))
                                                .Times(1);
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), resultIsEmpty(_))
                                                .Times(1)
                                                .WillOnce(Return(false));
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), resultGetFirstInt(_))
                                                .Times(1)
                                                .WillOnce(Return(1));
    EXPECT_CALL(*std::static_pointer_cast<MockPGConnector>(pgConnector), prepareStatement(_, "set_iso_level", "SET TRANSACTION ISOLATION LEVEL SERIALIZABLE"))
                                                .Times(1)
                                                .WillOnce(Throw(std::exception()));
    pgClient::PGStatus result = postgresClient->pgMessageQuery(selectCallback_, "send-queue");
    EXPECT_EQ(result, pgClient::PGStatus::PG_SUCCESS);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}
// END test query