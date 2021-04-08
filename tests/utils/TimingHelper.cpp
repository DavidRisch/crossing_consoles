#include "TimingHelper.h"

#include <cassert>
#include <thread>

#include "../../src/communication/byte_layer/byte_stream/SocketByteServer.h"
#include "../../src/communication/connection_layer/connection/Connection.h"
#include "../../src/communication/message_layer/message/KeepAliveMessage.h"
#include "../../src/communication/message_layer/message/PayloadMessage.h"

using namespace communication::byte_layer;
using namespace communication::message_layer;
using namespace communication::connection_layer;

std::shared_ptr<TimingHelper> TimingHelper::instance;

TimingHelper::TimingHelper() {
  CalculateReferenceTime();
}

const TimingHelper& TimingHelper::GetInstance() {
  if (instance == nullptr) {
    instance.reset(new TimingHelper());
  }
  return *instance;
}

void TimingHelper::CalculateReferenceTime() {
  auto start = std::chrono::steady_clock::now();

  DummyActions();

  reference_duration = std::chrono::steady_clock::now() - start;
}

void TimingHelper::DummyActions() {
  SocketByteServer byte_server;

  auto client_byte_stream = SocketByteStream::CreateClientSide();
  auto client_message_input_stream = std::make_shared<MessageInputStream>(client_byte_stream);
  auto client_message_output_stream = std::make_shared<MessageOutputStream>(client_byte_stream);

  auto new_client = byte_server.GetNewClient();
  assert(new_client != nullptr);
  auto server_message_input_stream = std::make_shared<MessageInputStream>(new_client);
  auto server_message_output_stream = std::make_shared<MessageOutputStream>(new_client);

  auto server_connection =
      Connection::CreateServerSide(std::move(server_message_input_stream), std::move(server_message_output_stream));

  std::thread server_thread([&server_connection] { server_connection->BlockingEstablish(); });

  auto client_connection =
      Connection::CreateClientSide(std::move(client_message_input_stream), std::move(client_message_output_stream));

  client_connection->BlockingEstablish();
  server_thread.join();

  for (int i = 0; i < 150; ++i) {
    {
      auto original_message = std::make_shared<KeepAliveMessage>();

      client_connection->SendMessage(original_message);

      server_connection->ReceiveMessage();  // KeepAliveMessage
      client_connection->ReceiveMessage();  // ACK
    }

    {
      std::vector<uint8_t> payload{123, 45};
      auto original_message = std::make_shared<PayloadMessage>(payload);

      server_connection->SendMessage(original_message);

      client_connection->ReceiveMessage();  // PayloadMessage
      server_connection->ReceiveMessage();  // ACK
    }
  }
}
