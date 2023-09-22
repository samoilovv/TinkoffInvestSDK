#include "operationsservice.h"

Operations::Operations(std::shared_ptr<grpc::Channel> channel, const std::string &token) :
    CustomService(token),
    m_operationsService(OperationsService::NewStub(channel))
{

}

Operations::~Operations()
{

}

ServiceReply Operations::GetOperations(const std::string  &accountId, int64_t fromseconds, int32_t fromnanos, int64_t toseconds, int32_t tonanos, OperationState state, const std::string  &figi)
{
    OperationsRequest request;
    request.set_figi(accountId);
    google::protobuf::Timestamp * from = new google::protobuf::Timestamp();
    from->set_seconds(fromseconds);
    from->set_nanos(fromnanos);
    request.set_allocated_from(from);
    google::protobuf::Timestamp * to = new google::protobuf::Timestamp();
    to->set_seconds(toseconds);
    to->set_nanos(tonanos);
    request.set_allocated_to(to);
    request.set_figi(figi);
    request.set_state(state);
    OperationsResponse reply;
    Status status = m_operationsService->GetOperations(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<OperationsResponse>(status, reply);
}

ServiceReply Operations::GetPortfolio(const std::string &accountId, PortfolioRequest_CurrencyRequest currency)
{
    PortfolioRequest request;
    request.set_account_id(accountId);
    request.set_currency(currency);
    PortfolioResponse reply;
    Status status = m_operationsService->GetPortfolio(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<PortfolioResponse>(status, reply);
}

ServiceReply Operations::GetPositions(const std::string &accountId)
{
    PositionsRequest request;
    request.set_account_id(accountId);
    PositionsResponse reply;
    Status status = m_operationsService->GetPositions(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<PositionsResponse>(status, reply);
}

ServiceReply Operations::GetWithdrawLimits(const std::string &accountId)
{
    WithdrawLimitsRequest request;
    request.set_account_id(accountId);
    WithdrawLimitsResponse reply;
    Status status = m_operationsService->GetWithdrawLimits(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<WithdrawLimitsResponse>(status, reply);
}

ServiceReply Operations::GetBrokerReport(const std::string  &accountId, int64_t fromseconds, int32_t fromnanos, int64_t toseconds, int32_t tonanos)
{
    BrokerReportRequest request;
    auto gbrr = new GenerateBrokerReportRequest();
    gbrr->set_account_id(accountId);
    google::protobuf::Timestamp * from = new google::protobuf::Timestamp();
    from->set_seconds(fromseconds);
    from->set_nanos(fromnanos);
    gbrr->set_allocated_from(from);
    google::protobuf::Timestamp * to = new google::protobuf::Timestamp();
    to->set_seconds(toseconds);
    to->set_nanos(tonanos);
    gbrr->set_allocated_to(to);
    request.set_allocated_generate_broker_report_request(gbrr);
    BrokerReportResponse reply;
    Status status = m_operationsService->GetBrokerReport(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<BrokerReportResponse>(status, reply);
}

ServiceReply Operations::GetBrokerReport(const std::string &taskId, int32_t page)
{
    BrokerReportRequest request;
    auto gbrr = new GetBrokerReportRequest();
    gbrr->set_task_id(taskId);
    gbrr->set_page(page);
    request.set_allocated_get_broker_report_request(gbrr);
    BrokerReportResponse reply;
    Status status = m_operationsService->GetBrokerReport(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<BrokerReportResponse>(status, reply);
}

ServiceReply Operations::GetDividendsForeignIssuer(const std::string  &accountId, int64_t fromseconds, int32_t fromnanos, int64_t toseconds, int32_t tonanos)
{
    GetDividendsForeignIssuerRequest request;
    auto gdfirr = new GenerateDividendsForeignIssuerReportRequest();
    gdfirr->set_account_id(accountId);
    google::protobuf::Timestamp * from = new google::protobuf::Timestamp();
    from->set_seconds(fromseconds);
    from->set_nanos(fromnanos);
    gdfirr->set_allocated_from(from);
    google::protobuf::Timestamp * to = new google::protobuf::Timestamp();
    to->set_seconds(toseconds);
    to->set_nanos(tonanos);
    gdfirr->set_allocated_to(to);
    request.set_allocated_generate_div_foreign_issuer_report(gdfirr);
    GetDividendsForeignIssuerResponse reply;
    Status status = m_operationsService->GetDividendsForeignIssuer(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<GetDividendsForeignIssuerResponse>(status, reply);
}

ServiceReply Operations::GetDividendsForeignIssuer(const std::string &taskId, int32_t page)
{
    GetDividendsForeignIssuerRequest request;
    auto gdfirr = new GetDividendsForeignIssuerReportRequest();
    gdfirr->set_task_id(taskId);
    gdfirr->set_page(page);
    request.set_allocated_get_div_foreign_issuer_report(gdfirr);
    GetDividendsForeignIssuerResponse reply;
    Status status = m_operationsService->GetDividendsForeignIssuer(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<GetDividendsForeignIssuerResponse>(status, reply);
}

