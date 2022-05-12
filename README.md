# TinkoffInvestSDK
C++ client for Tinkoff invest API

# OpenAPI SDK для Тинькофф Инвестиций

Данный проект представляет собой инструментарий на языке C++ для работы с grpc-интерфейсом торговой
платформы [Тинькофф Инвестиции](https://www.tinkoff.ru/invest/), который можно
использовать для создания торговых роботов.

## Начало работы

### Сборка

Перейдите в директорию проекта и выполните следующие команды:

```bash
mkdir build && cd build
cmake ..
make
``` 

## Примеры

Примеры доступны [здесь](https://github.com/samoilovv/TinkoffInvestSDK/tree/main/samples). Для запуска примеров необходимо добавить токен в переменную окружения.

<!-- termynal -->

```
$ export TOKEN=YOUR_TOKEN
```

Пример использования унарных запросов: открытие счета в песочнице и получение информации о нем.

```cpp

InvestApiClient сlient("invest-public-api.tinkoff.ru:443", getenv("TOKEN"));

//get reference to sandbox service
auto sandbox = std::dynamic_pointer_cast<Sandbox>(сlient.service("sandbox"));

//open account
sandbox->OpenSandboxAccount();

//print info about your account
auto accounts = sandbox->GetSandboxAccounts();
auto portfolio = sandbox->GetSandboxPortfolio(accounts.accountID(0));
std::cout << portfolio.ptr()->DebugString() << std::endl;

//close account
sandbox->CloseSandboxAccount(accountId);

```


Пример использования потокового асинхронного запроса: подписка на получение последних цен.

```cpp

void tradesStreamCallBack(ServiceReply reply)
{
    std::cout << reply.ptr()->DebugString() << std::endl;
}

int main()
{
    InvestApiClient сlient("invest-public-api.tinkoff.ru:443", getenv("TOKEN"));

    //get references to Sandbox and OrdersStream service
    auto marketdata = std::dynamic_pointer_cast<MarketDataStream>(сlient.service("makretdatastream"));

    //Start MarketData stream
    std::thread thread = std::thread(&MarketDataStream::AsyncCompleteRpc, marketdata.get());

    //Subscribe on British American Tobacco and Visa Inc. prices and start streaming
    marketdata->SubscribeLastPriceAsync({"BBG000BWPXQ8", "BBG00844BD08"}, tradesStreamCallBack);

    thread.join();

    return 0;
}

```


Пример использования потокового блокирующего вызова: подписка на получение последних цен.

```cpp

void tradesStreamCallBack(ServiceReply reply)
{
    std::cout << reply.ptr()->DebugString() << std::endl;
}

int main()
{
    InvestApiClient сlient("invest-public-api.tinkoff.ru:443", getenv("TOKEN"));

    //get references to Sandbox and OrdersStream service
    auto marketdata = std::dynamic_pointer_cast<MarketDataStream>(tinkoffInvestClient.service("makretdatastream"));

    //Subscribe on British American Tobacco and Visa Inc. prices and start streaming
    marketdata->SubscribeLastPrice({"BBG000BWPXQ8", "BBG00844BD08"}, tradesStreamCallBack);

    return 0;
}

```


Вывод:

<!-- termynal -->

```
subscribe_last_price_response {
  tracking_id: "8442ce626f5a6e6"
  last_price_subscriptions {
    figi: "BBG000BWPXQ8"
    subscription_status: SUBSCRIPTION_STATUS_SUCCESS
  }
  last_price_subscriptions {
    figi: "BBG00844BD08"
    subscription_status: SUBSCRIPTION_STATUS_SUCCESS
  }
}

last_price {
  figi: "BBG000BWPXQ8"
  price {
    units: 41
    nano: 500000000
  }
  time {
    seconds: 1651270899
    nanos: 146644771
  }
}

last_price {
  figi: "BBG00844BD08"
  price {
    units: 100
    nano: 120000000
  }
  time {
    seconds: 1585063374
    nanos: 334361000
  }
}
```

## Документация

Подробную документацию можно найти по [ссылке](https://samoilovv.github.io/TinkoffInvestSDK/).
