#include "servicereply.h"

ServiceReply::ServiceReply(const std::shared_ptr<google::protobuf::Message>  &protoMsg) : m_replyPtr(protoMsg)
{

}

const std::shared_ptr<google::protobuf::Message> ServiceReply::replyPtr()
{
    return m_replyPtr;
}
