CREATE TABLE Exchanges (
    ID SERIAL PRIMARY KEY,
    exchange_name VARCHAR(255) UNIQUE,
    exchange_type VARCHAR(255)
);

CREATE TABLE Messages (
    ID SERIAL PRIMARY KEY,
    payload VARCHAR(255),
    exchange_id INTEGER NULL,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT NOW(),
    FOREIGN KEY (exchange_id) REFERENCES Exchanges(ID)
);

INSERT INTO Exchanges(exchange_name, exchange_type) VALUES ('send-queue', 'queue');
INSERT INTO Exchanges(exchange_name, exchange_type) VALUES ('send-topic', 'topic');
INSERT INTO Exchanges(exchange_name, exchange_type) VALUES ('reply-queue', 'queue');
INSERT INTO Exchanges(exchange_name, exchange_type) VALUES ('reply-topic', 'topic');

CREATE INDEX idx_exchanges_exchange_name ON Exchanges(exchange_name);
CREATE INDEX idx_messages_exchange_id ON Messages(exchange_id);
