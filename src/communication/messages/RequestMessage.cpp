#include "RequestMessage.h"

RequestMessage::RequestMessage(ip_address_t& source, ip_address_t& destination, double timestamp)
    : Message(source, destination, timestamp) {
}
