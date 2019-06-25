#ifndef __NETWORK_MESSAGE_H__
#define __NETWORK_MESSAGE_H__

#include <exception>
#include <string>
#include <vector>
#include <memory>
#include <network/exception.h>
#include <network/message/protocol/protocol.pb.h>

namespace network
{
  enum response_statuses{
    RS_SUCCESS = 100,
    RS_REJECTED = 502,
    RS_ERROR = 400
  };

  using storage_type = std::vector<unsigned char>;
  using protocol::Request;
  using protocol::Response;
}

#endif//__NETWORK_MESSAGE_H__
