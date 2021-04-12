#ifndef CROSSING_CONSOLES_CONNECTIONS_H
#define CROSSING_CONSOLES_CONNECTIONS_H

#include <thread>
#include <utility>

#include "../../src/communication/byte_layer/byte_stream/MockBidirectionalByteStream.h"
#include "../../src/communication/byte_layer/byte_stream/SocketByteServer.h"
#include "../../src/communication/connection_layer/connection/Connection.h"
#include "../../src/communication/connection_layer/connection/ConnectionManager.h"
#include "../../src/communication/message_layer/message/AcknowledgeMessage.h"
#include "../../src/communication/message_layer/message/ConnectionRequestMessage.h"
#include "../../src/communication/message_layer/message/ConnectionResetMessage.h"
#include "../../src/communication/message_layer/message/ConnectionResponseMessage.h"
#include "../../src/communication/message_layer/message/KeepAliveMessage.h"
#include "../../src/communication/message_layer/message/NotAcknowledgeMessage.h"

using namespace communication;
using namespace communication::byte_layer;
using namespace communication::connection_layer;
using namespace communication::message_layer;

class Connections : public ::testing::Test {
 public:
  Connections() = default;

  std::vector<uint8_t> empty_payload;

  std::list<std::shared_ptr<Message>> test_messages_all_types = {
      std::make_shared<KeepAliveMessage>(1000),          std::make_shared<ConnectionRequestMessage>(1200),
      std::make_shared<ConnectionResponseMessage>(1230), std::make_shared<PayloadMessage>(empty_payload, 123),
      std::make_shared<AcknowledgeMessage>(2000),        std::make_shared<ConnectionResetMessage>(3000),
      std::make_shared<NotAcknowledgeMessage>(3000),
  };

  std::shared_ptr<Connection> server_connection;
  std::shared_ptr<Connection> client_connection;

  void make_connections(std::shared_ptr<MessageInputStream> client_message_input_stream,
                        std::shared_ptr<MessageOutputStream> client_message_output_stream,
                        std::shared_ptr<MessageInputStream> server_message_input_stream,
                        std::shared_ptr<MessageOutputStream> server_message_output_stream) {
    server_connection =
        Connection::CreateServerSide(std::move(server_message_input_stream), std::move(server_message_output_stream));

    std::thread server_thread([this] { server_connection->BlockingEstablish(); });

    client_connection =
        Connection::CreateClientSide(std::move(client_message_input_stream), std::move(client_message_output_stream));

    client_connection->BlockingEstablish();
    server_thread.join();
  }

  void test_connections() {
    {
      auto original_message = std::make_shared<KeepAliveMessage>();

      client_connection->SendMessage(original_message);

      auto received_message = server_connection->ReceiveMessage();
      ASSERT_NE(received_message, nullptr);
      EXPECT_EQ(original_message->GetMessageType(), received_message->GetMessageType());

      auto ack_message = client_connection->ReceiveMessage();
      ASSERT_NE(ack_message, nullptr);
      EXPECT_EQ(ack_message->GetMessageType(), MessageType::ACKNOWLEDGE);
    }

    {
      std::vector<uint8_t> payload;
      payload.push_back(123);
      payload.push_back(45);
      auto original_message = std::make_shared<PayloadMessage>(payload);

      server_connection->SendMessage(original_message);

      auto received_message = client_connection->ReceiveMessage();
      ASSERT_NE(received_message, nullptr);
      EXPECT_EQ(original_message->GetMessageType(), received_message->GetMessageType());
      auto &received_payload_message = dynamic_cast<PayloadMessage &>(*received_message);
      EXPECT_EQ(received_payload_message.GetPayload().size(), original_message->GetPayload().size());
      EXPECT_EQ(received_payload_message.GetPayload().at(0), original_message->GetPayload().at(0));

      auto ack_message = server_connection->ReceiveMessage();
      ASSERT_NE(ack_message, nullptr);
      EXPECT_EQ(ack_message->GetMessageType(), MessageType::ACKNOWLEDGE);
    }
  }

  static void close_connection(const std::shared_ptr<Connection> &first, const std::shared_ptr<Connection> &second) {
    // first sends Reset Message to second, connection will be closed
    auto message = std::make_shared<ConnectionResetMessage>(1234);

    first->SendMessage(message);
    auto received_message = second->ReceiveMessage();
    ASSERT_NE(received_message, nullptr);
    ASSERT_EQ(received_message->GetMessageType(), message_layer::MessageType::CONNECTION_RESET);

    received_message = first->ReceiveMessage();
    ASSERT_NE(received_message, nullptr);
    ASSERT_EQ(received_message->GetMessageType(), message_layer::MessageType::ACKNOWLEDGE);

    // Connection is ready to be closed, Connection Reset has been acknowledged
    ASSERT_TRUE(first->ConnectionClosed());

    // no messages should be sent or received
    auto new_message = std::make_shared<KeepAliveMessage>(1400);
    first->SendMessage(message);
    received_message = second->ReceiveMessage();
    ASSERT_EQ(received_message, nullptr);
    received_message = first->ReceiveMessage();
    ASSERT_EQ(received_message, nullptr);
  }
};

#endif  // CROSSING_CONSOLES_CONNECTIONS_H
