#include <gtest/gtest.h>

#include <thread>

#include "../src/communication/ProtocolDefinition.h"
#include "../src/communication/byte_layer/byte_stream/MockBidirectionalByteStream.h"
#include "../src/communication/byte_layer/byte_stream/SocketByteServer.h"
#include "../src/communication/connection_layer/connection/ClientSideConnectionManager.h"
#include "../src/communication/connection_layer/connection/ServerSideConnectionManager.h"
#include "../src/communication/connection_layer/event/PayloadEvent.h"
#include "fixtures/ConnectionManagers.h"
#include "utils/detect_debugger.h"

using namespace communication;
using namespace communication::byte_layer;
using namespace communication::connection_layer;
using namespace communication::message_layer;

#ifndef _WIN32

bool CAUGHT_SIGNAL_BROKEN_PIPE = false;

static void broken_pipe_handler(int signum) {
  assert(signum == SIGPIPE);
  // Received signal of type SIGPIPE (Broken pipe)
  CAUGHT_SIGNAL_BROKEN_PIPE = true;
}

static void setup_signal_handler() {
  // setup signal handler to catch and handle signal SIGPIPE
  // see https://pubs.opengroup.org/onlinepubs/9699919799/functions/sigaction.html

  struct sigaction sa = {};  // sigaction used to handle signal
  sa.sa_handler = broken_pipe_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESETHAND;  // reset to SIG_DFL and the SA_SIGINFO flag on entry to the signal handler
  sigaction(SIGPIPE, &sa, nullptr);
}

static void reset_signal_handler() {
  // reset signal handler to default behaviour
  // needs to be called after every call of setup_signal_handler to ensure default behaviour in following tests.

  struct sigaction sa = {};
  sa.sa_handler = SIG_DFL;  // default behaviour
  sigemptyset(&sa.sa_mask);
  sigaction(SIGPIPE, &sa, nullptr);

  CAUGHT_SIGNAL_BROKEN_PIPE = false;
}

#endif

TEST_F(ConnectionManagers, ConnectClient) {
  create_server_and_client();
}

TEST_F(ConnectionManagers, ServerTimeout) {
  // Server becomes unreachable
  auto timeout = std::chrono::milliseconds(100);
  create_server_and_client(timeout);

  client_manager->HandleConnections();
  std::this_thread::sleep_for(timeout);
  client_manager->HandleConnections();
  auto event = client_manager->PopAndGetOldestEvent();

  EXPECT_NE(event, nullptr);
  ASSERT_EQ(event->GetType(), EventType::DISCONNECT);

  // all events should have been processed by now
  EXPECT_EQ(server_manager->PopAndGetOldestEvent(), nullptr);
  EXPECT_EQ(client_manager->PopAndGetOldestEvent(), nullptr);
}

TEST_F(ConnectionManagers, ClientTimeout) {
  // Client becomes unreachable
  auto timeout = std::chrono::milliseconds(100);
  create_server_and_client(timeout);

  server_manager->HandleConnections();
  std::this_thread::sleep_for(timeout);
  server_manager->HandleConnections();

  auto event = server_manager->PopAndGetOldestEvent();
  EXPECT_NE(event, nullptr);
  ASSERT_EQ(event->GetType(), EventType::DISCONNECT);
  ASSERT_EQ(server_manager->ConnectionCount(), 0);

  // all events should have been processed by now
  EXPECT_EQ(server_manager->PopAndGetOldestEvent(), nullptr);
  EXPECT_EQ(client_manager->PopAndGetOldestEvent(), nullptr);
}

TEST_F(ConnectionManagers, UnknownPartnerException) {
  create_server_and_client();
  std::vector<uint8_t> payload;
  EXPECT_THROW(server_manager->SendDataToConnection(client_id + 1, payload),
               ConnectionManager::UnknownPartnerException);
  EXPECT_THROW(client_manager->SendDataToConnection(server_id + 1, payload),
               ConnectionManager::UnknownPartnerException);
}

TEST_F(ConnectionManagers, SendMessages) {
  create_server_and_client();
  send_and_check_messages(10);
}

TEST_F(ConnectionManagers, SendManyMessages) {
  create_server_and_client();
  send_and_check_messages(1000);
}

TEST_F(ConnectionManagers, TwoClients) {
  create_server_and_client();
  create_second_client();

  for (int i = 0; i < 10; ++i) {
    std::vector<uint8_t> payload_server_to_client1{1, 2, static_cast<uint8_t>(i)};
    std::vector<uint8_t> payload_server_to_client2{3, 4, static_cast<uint8_t>(i)};
    std::vector<uint8_t> payload_client1_to_server{5, 6, static_cast<uint8_t>(i)};
    std::vector<uint8_t> payload_client2_to_server{7, 8, static_cast<uint8_t>(i)};

    server_manager->SendDataToConnection(client_id, payload_server_to_client1);
    server_manager->SendDataToConnection(second_client_id, payload_server_to_client2);
    client_manager->SendDataToServer(payload_client1_to_server);

    std::this_thread::sleep_for(std::chrono::microseconds(100));
    server_manager->HandleConnections();

    second_client_manager->SendDataToServer(payload_client2_to_server);

    std::this_thread::sleep_for(std::chrono::microseconds(100));
    server_manager->HandleConnections();
    client_manager->HandleConnections();
    second_client_manager->HandleConnections();

    assert_payload_received(server_manager->PopAndGetOldestEvent(), payload_client1_to_server);
    assert_payload_received(server_manager->PopAndGetOldestEvent(), payload_client2_to_server);
    assert_payload_received(client_manager->PopAndGetOldestEvent(), payload_server_to_client1);
    assert_payload_received(second_client_manager->PopAndGetOldestEvent(), payload_server_to_client2);

    // all events should have been processed by now
    EXPECT_EQ(server_manager->PopAndGetOldestEvent(), nullptr);
    EXPECT_EQ(client_manager->PopAndGetOldestEvent(), nullptr);
    EXPECT_EQ(second_client_manager->PopAndGetOldestEvent(), nullptr);
  }
}

TEST_F(ConnectionManagers, LongQueue) {
  create_server_and_client();
  std::vector<std::vector<uint8_t>> payloads_server_to_client;
  std::vector<std::vector<uint8_t>> payloads_client_to_server;

  int count = 10;

  for (int i = 0; i < count; ++i) {
    payloads_server_to_client.emplace_back();
    payloads_server_to_client.back().push_back(1);
    payloads_server_to_client.back().push_back(i);
    payloads_server_to_client.back().push_back(2);

    payloads_client_to_server.emplace_back();
    payloads_client_to_server.back().push_back(3);
    payloads_client_to_server.back().push_back(i);
    payloads_client_to_server.back().push_back(4);
  }

  for (int i = 0; i < count; ++i) {
    server_manager->SendDataToConnection(client_id, payloads_server_to_client.at(i));
    client_manager->SendDataToServer(payloads_client_to_server.at(i));
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    server_manager->HandleConnections();
    client_manager->HandleConnections();
  }

  for (int i = 0; i < count; ++i) {
    assert_payload_received(server_manager->PopAndGetOldestEvent(), payloads_client_to_server.at(i));
    assert_payload_received(client_manager->PopAndGetOldestEvent(), payloads_server_to_client.at(i));
  }

  // all events should have been processed by now
  EXPECT_EQ(server_manager->PopAndGetOldestEvent(), nullptr);
  EXPECT_EQ(client_manager->PopAndGetOldestEvent(), nullptr);
}

TEST_F(ConnectionManagers, ClientConnectionReset) {
  // Client sends message of type ConnectionResetMessage
  create_server_and_client();

#ifndef _WIN32
  // setup signal handler to catch signal SIGPIPE and fail test
  setup_signal_handler();
#endif

  client_manager->CloseConnection(ProtocolDefinition::server_partner_id);

  server_manager->HandleConnections();
  client_manager->HandleConnections();

  auto event = server_manager->PopAndGetOldestEvent();
  ASSERT_NE(event, nullptr);
  ASSERT_EQ(event->GetType(), EventType::DISCONNECT);
  ASSERT_EQ(server_manager->ConnectionCount(), 0);

  event = client_manager->PopAndGetOldestEvent();
  ASSERT_NE(event, nullptr);
  EXPECT_EQ(event->GetType(), EventType::DISCONNECT);

  // no broken pipe signal should be triggered
  client_manager->HandleConnections();
  server_manager->HandleConnections();

#ifndef _WIN32
  ASSERT_FALSE(CAUGHT_SIGNAL_BROKEN_PIPE);
  reset_signal_handler();
#endif

  // all events should have been processed by now
  EXPECT_EQ(server_manager->PopAndGetOldestEvent(), nullptr);
  EXPECT_EQ(client_manager->PopAndGetOldestEvent(), nullptr);
}

TEST_F(ConnectionManagers, ServerConnectionReset) {
  // Server sends message of type ConnectionResetMessage
  create_server_and_client();

#ifndef _WIN32
  // setup signal handler to catch signal SIGPIPE and fail test
  setup_signal_handler();
#endif

  // reset Connection
  partner_id_t client_id = 1;  // only 1 client is connected at this point
  server_manager->CloseConnection(client_id);

  client_manager->HandleConnections();
  server_manager->HandleConnections();

  auto event = client_manager->PopAndGetOldestEvent();
  ASSERT_NE(event, nullptr);
  ASSERT_EQ(event->GetType(), EventType::DISCONNECT);

  event = server_manager->PopAndGetOldestEvent();
  ASSERT_NE(event, nullptr);
  ASSERT_EQ(event->GetType(), EventType::DISCONNECT);

  // no broken pipe signal should be triggered
  client_manager->HandleConnections();
  server_manager->HandleConnections();

#ifndef _WIN32
  ASSERT_FALSE(CAUGHT_SIGNAL_BROKEN_PIPE);
  reset_signal_handler();
#endif

  // all events should have been processed by now
  EXPECT_EQ(server_manager->PopAndGetOldestEvent(), nullptr);
  EXPECT_EQ(client_manager->PopAndGetOldestEvent(), nullptr);
}

#ifndef _WIN32
TEST_F(ConnectionManagers, BrokenPipeSignal) {
  // test that setup_signal_handler sets new handler for signal SIGPIPE in tests

  if (runningUnderDebugger()) {
    // signal is not caught in debugging mode
    return;
  }

  setup_signal_handler();
  raise(SIGPIPE);
  ASSERT_TRUE(CAUGHT_SIGNAL_BROKEN_PIPE);

  reset_signal_handler();
  ASSERT_FALSE(CAUGHT_SIGNAL_BROKEN_PIPE);
}
#endif

TEST_F(ConnectionManagers, ClientKeepAlive) {
  // Client sends KeepAlive to Server
  auto timeout = std::chrono::milliseconds(100);
  auto keep_alive_interval = timeout / ProtocolDefinition::keep_alive_denominator;

  create_server_and_client(timeout);

  std::this_thread::sleep_for(keep_alive_interval);
  // send KeepAlive Message
  client_manager->HandleConnections();

  std::this_thread::sleep_for(keep_alive_interval);
  // Acknowledge KeepAlive Message, no timeout is triggered
  server_manager->HandleConnections();
  client_manager->HandleConnections();

  // no timeout events should have been triggered
  EXPECT_EQ(server_manager->PopAndGetOldestEvent(), nullptr);
  EXPECT_EQ(client_manager->PopAndGetOldestEvent(), nullptr);
}

TEST_F(ConnectionManagers, ServerKeepAlive) {
  // Server sends KeepAlive to Client
  auto timeout = std::chrono::milliseconds(100);
  auto keep_alive_interval = timeout / ProtocolDefinition::keep_alive_denominator;

  create_server_and_client(timeout);

  std::this_thread::sleep_for(keep_alive_interval);
  // sends KeepAlive Message
  server_manager->HandleConnections();

  std::this_thread::sleep_for(keep_alive_interval);
  // Acknowledge KeepAlive Message, no timeout is triggered
  client_manager->HandleConnections();
  server_manager->HandleConnections();

  // no timeout events should have been triggered
  EXPECT_EQ(server_manager->PopAndGetOldestEvent(), nullptr);
  EXPECT_EQ(client_manager->PopAndGetOldestEvent(), nullptr);
}
