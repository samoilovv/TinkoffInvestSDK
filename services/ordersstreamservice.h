#ifndef ORDERSSTREAMSERVICE_H
#define ORDERSSTREAMSERVICE_H

#include <QObject>
#include <thread>
#include "customservice.h"
#include <grpcpp/grpcpp.h>
#include "orders.grpc.pb.h"
#include "servicereply.h"

using grpc::ClientAsyncReader;
using grpc::Channel;
using grpc::CompletionQueue;

using namespace tinkoff::public1::invest::api::contract::v1;

class CommonAsyncClientCall
{
public:
  ClientContext context;
  TradesStreamResponse reply;
  enum CallStatus { CREATE, PROCESS, FINISH };
  CallStatus callStatus ;
  Status status;
  void printReply(const char* from)
  {
    if(!reply.DebugString().empty())
      std::cout << "[" << from << "]: reply message = " << reply.DebugString() << std::endl;
    else
      std::cout << "[" << from << "]: reply message empty" << std::endl;
  }
  explicit CommonAsyncClientCall(std::string token):callStatus(PROCESS)
  {
      std::string meta_value = "Bearer " + token;
      context.AddMetadata("authorization", meta_value);
      context.AddMetadata("x-app-name", APP_NAME);
  }
  virtual ~CommonAsyncClientCall(){}
  virtual void Proceed(bool = true) = 0;
};

class AsyncClientCall1M : public CommonAsyncClientCall
{
  std::unique_ptr<ClientAsyncReader<TradesStreamResponse> > responder;
  std::function<void(ServiceReply)> callback;
public:
  AsyncClientCall1M(const TradesStreamRequest& request, CompletionQueue& cq_, std::unique_ptr<OrdersStreamService::Stub>& stub_, std::string token, std::function<void(ServiceReply)> callback)
  :CommonAsyncClientCall(token), callback(callback)
  {
    std::cout << "[Proceed1M]: new client 1-M" << std::endl;
    responder = stub_->AsyncTradesStream(&context, request, &cq_, (void*)this);
    callStatus = PROCESS ;
  }

  virtual void Proceed(bool ok = true) override
  {
    if(callStatus == PROCESS)
    {
      if(!ok)
      {
        std::cout << "[Proceed1M]: Trying finish" << std::endl;
        responder->Finish(&status, (void*)this);
        callStatus = FINISH;
        return ;
      }
      responder->Read(&reply, (void*)this);
      auto replycopy(reply);
      auto data = ServiceReply(std::make_shared<TradesStreamResponse>(replycopy));
      if (callback)
      {
        callback(data);
      }
      printReply("Proceed1M");
    }
    else if(callStatus == FINISH)
    {
      std::cout << "[Proceed1M]: Good Bye" << std::endl;
      delete this;
    }
    return ;
  }

};

/*!
    \brief  Сервис торговых поручений в режиме стриминга

    Сервис предназначен для получения потока сделок пользователя.
*/
class OrdersStream: public CustomService
{
    Q_OBJECT
    Q_CLASSINFO("ordersstream", "OrdersStream Service")

    enum Type {
        CONNECT = 1,
        READ = 2,
        FINISH = 3
    };

public:
    OrdersStream(std::shared_ptr<Channel> channel, const std::string &token);
    ~OrdersStream();

    /// Поток сделок пользователя, блокирующий вызов
    void TradesStream(const std::vector<std::string> &accounts, std::function<void(ServiceReply)> callback);
    /// Поток сделок пользователя, асинхронный вызов
    void TradesStreamAsync(const std::vector<std::string> &accounts, std::function<void(ServiceReply)> callback);

    void GladToSeeMe(std::function<void(ServiceReply)> callback)
    {
      TradesStreamRequest request;

      new AsyncClientCall1M(request, m_cq, m_ordersStreamService, m_token, callback);
    }

    void AsyncCompleteRpc()
    {
        void* got_tag;
        bool ok = false;
        while(m_cq.Next(&got_tag, &ok))
        {
            CommonAsyncClientCall* call = static_cast<CommonAsyncClientCall*>(got_tag);
            call->Proceed(ok);
        }
        std::cout << "Completion queue is shutting down." << std::endl;
    }

private:
    std::unique_ptr<OrdersStreamService::Stub> m_ordersStreamService;
    std::unique_ptr<std::thread> m_grpc_thread_;
    std::unique_ptr<ClientAsyncReader<TradesStreamResponse>> m_reader;
    CompletionQueue m_cq;
    TradesStreamResponse m_reply;
    ClientContext m_context;
    TradesStreamRequest m_request;
    grpc::Status m_status = grpc::Status::OK;
    void GrpcThread();
};

#endif // ORDERSSTREAMSERVICE_H
