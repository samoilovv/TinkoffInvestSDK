# TinkoffInvestSDK
C++ client for Tinkoff invest API

# OpenAPI SDK для Тинькофф Инвестиций

Данный проект представляет собой инструментарий на языке C++ для работы с grpc-интерфейсом торговой
платформы [Тинькофф Инвестиции](https://www.tinkoff.ru/invest/), который можно
использовать для создания торговых роботов.

## Начало работы

### Сборка

Клонируйте репозиторий:

```bash
git clone https://github.com/samoilovv/TinkoffInvestSDK.git
cd TinkoffInvestSDK
git submodule update --init --recursive
``` 

Перейдите в директорию проекта и выполните следующие команды:

```bash
mkdir build && cd build
cmake ..
cmake --build .
``` 
Первый запуск CMake вызовет скачивание зависимостей, что может занять некоторое время. 

## Примеры

Примеры доступны [здесь](https://github.com/samoilovv/TinkoffInvestSDK/tree/main/samples). Для запуска примеров необходимо добавить токен в переменную окружения.

<!-- termynal -->

```
$ export TOKEN=YOUR_TOKEN
```

Пример использования унарных запросов: открытие счета в песочнице и получение информации о нем.

```cpp

InvestApiClient сlient("invest-public-api.tinkoff.ru:443", getenv("TOKEN"));

//get pointer to sandbox service
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


Пример использования потокового блокирующего вызова: подписка на получение последних цен и на ленту обезличенных сделок. Каждый вызов необходимо помещать в отдельный поток.

```cpp

using namespace std;

void marketStreamCallBack(ServiceReply reply)
{
    cout << reply.ptr()->DebugString() << endl;
}

int main()
{    
    InvestApiClient client("invest-public-api.tinkoff.ru:443", getenv("TOKEN"));

    //get pointer to MarketDataStream service
    auto marketdata = dynamic_pointer_cast<MarketDataStream>(client.service("marketdatastream"));

    //subscribe to NVIDIA and Tesla Motors prices and start streaming
    thread th1([marketdata](){
            marketdata->SubscribeLastPrice({"BBG000BBJQV0", "BBG000N9MNX3"}, marketStreamCallBack);
        }
    );

    //subscribe to Bashneft (BANE) and Moscow Exchange (MOEX) shares transactions and start streaming
    thread th2([marketdata](){
            marketdata->SubscribeTradesAsync({"BBG004S68758", "BBG004730JJ5"}, marketStreamCallBack);
        }
    );

    th1.join();
    th2.join();

    return 0;
}

```

Пример использования потокового асинхронного запроса: подписка на получение последних цен и на ленту обезличенных сделок. Клиент обрабатывает ответы сервера в единственном потоке, что существенно повышает производительность системы при большом количестве запросов. 

```cpp

using namespace std;

void marketStreamCallBack(ServiceReply reply)
{
    cout << reply.ptr()->DebugString() << endl;
}

int main()
{
    InvestApiClient сlient("invest-public-api.tinkoff.ru:443", getenv("TOKEN"));

    //get pointer to MarketDataStream service
    auto marketdata = dynamic_pointer_cast<MarketDataStream>(сlient.service("marketdatastream"));

    //subscribe to British American Tobacco and Visa Inc. prices 
    marketdata->SubscribeLastPriceAsync({"BBG000BWPXQ8", "BBG00844BD08"}, marketStreamCallBack);
    
    //subscribe to Bashneft (BANE) and Moscow Exchange (MOEX) shares transactions
    marketdata->SubscribeTradesAsync({"BBG004S68758", "BBG004730JJ5"}, marketStreamCallBack);    

    return 0;
}

```

Вывод:

<!-- termynal -->

```
subscribe_last_price_response {
  tracking_id: "628164edf8495c0"
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
    units: 42
    nano: 80000000
  }
  time {
    seconds: 1652481859
    nanos: 657347773
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

subscribe_trades_response {
  tracking_id: "628164ed2816ca3dab68f498d02ca29b"
  trade_subscriptions {
    figi: "BBG004S68758"
    subscription_status: SUBSCRIPTION_STATUS_SUCCESS
  }
  trade_subscriptions {
    figi: "BBG004730JJ5"
    subscription_status: SUBSCRIPTION_STATUS_SUCCESS
  }
}
```

## Документация

Подробную документацию можно найти по [ссылке](https://samoilovv.github.io/TinkoffInvestSDK/). Вопросы и предложения [сюда](https://github.com/samoilovv/TinkoffInvestSDK/issues).
