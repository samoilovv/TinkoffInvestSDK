# TinkoffInvestSDK
Qt client for Tinkoff invest API

# OpenAPI SDK для Тинькофф Инвестиций

Данный проект представляет собой инструментарий на языке C++/Qt для работы с grpc-интерфейсом торговой
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

Примеры доступны [здесь](https://github.com/samoilovv/TinkoffInvestSDK/tree/main/samples). Для запуска примеров необходимо добавить токен в переменную окружения.

<!-- termynal -->

```
$ export TOKEN=YOUR_TOKEN
```

Пример использования потокового запроса: подписаться и получать последние цены двух инструментов

```cpp

    InvestApiClient greeter("invest-public-api.tinkoff.ru:443", getenv("TOKEN"));

    //get reference to marketdata service
    auto marketdataPtr = qSharedPointerCast<MarketData>(greeter.service("marketdata")).get();

    //handle replies in lambda function
    QObject::connect(marketdata.get(), &CustomService::sendData, [marketdataPtr](ServiceReply reply){
        std::cout << reply.ptr()->DebugString() << std::endl;
    });

    //start streaming and subscribe on British American Tobacco and Visa Inc. prices
    marketdataPtr->MarketDataStream({"BBG000BWPXQ8", "BBG00844BD08"});
```

Вывод:
<!-- termynal -->

```
subscribe_last_price_response {
  tracking_id: "626f561485de8897ce89f15598f5a6e6"
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

Подробную документацию по InvestApi можно найти по [ссылке](https://tinkoff.github.io/investAPI/).
