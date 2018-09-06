#include "patterntester.h"

#include "Strategy/duck.h"
#include "Strategy/behaviors.h"

#include "Observer/weatherstation.h"
#include "Observer/weatherobservers.h"

#include "Decorator/beverage.h"

#include "Factory/SimpleFactory/simplepizzastore.h"
#include "Factory/chicagopizzastore.h"
#include "Factory/nypizzastore.h"

#include "Singleton/singelton.h"

#include "Command/remotecontrol.h"
#include "Command/devices.h"
#include "Command/commands.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>

void PatternTester::testPattern(Pattern pattern)
{
    prinPreInfo(pattern);

    switch (pattern)
    {
    case Pattern::Strategy:
        testStrategy();
        break;

    case Pattern::Observer:
        testObserver();
        break;

    case Pattern::Decorator:
        testDecorator();
        break;

    case Pattern::Factory:
        testFactory();
        break;

    case Pattern::Singleton:
        testSingleton();
        break;

    case Pattern::Command:
        testCommand();
        break;
    }

    prinPostInfo(pattern);
}

void PatternTester::testStrategy()
{
    std::vector<std::shared_ptr<Duck>> ducks;

    const auto mallarDuck = std::make_shared<MallardDuck>();
    const auto modelDuck = std::make_shared<ModelDuck>();
    const auto redheadDuck = std::make_shared<RedheadDuck>();
    const auto decoyDuck = std::make_shared<DecoyDuck>();
    decoyDuck->setFlyBehavior(std::make_shared<FlyRocketPowered>());

    ducks.push_back(mallarDuck);
    ducks.push_back(modelDuck);
    ducks.push_back(redheadDuck);
    ducks.push_back(decoyDuck);

    for (const auto &duck : ducks)
    {
        duck->display();
        duck->swim();
        duck->performFly();
        duck->performQuack();
        std::cout << std::endl;
    }
}

void PatternTester::testObserver()
{
    const auto station = std::make_shared<WeatherStation>();
    BaseObserver::create<CurrentConditionsDisplay>(station);
    BaseObserver::create<StatisticsDisplay>(station);
    BaseObserver::create<ForecastDisplay>(station);

    station->setMeasurements(80.f, 65.f, 30.4f);
    station->setMeasurements(82.f, 70.f, 29.2f);
    station->setMeasurements(78.f, 90.f, 29.2f);
    station->setMeasurements(78.2f, 90.f, 29.2f); // <-- probably will not be notified
}

void PatternTester::testDecorator()
{
    auto espresso = std::make_shared<Espresso>();
    std::cout << espresso.get() << std::endl;

    auto darkRoast = std::make_shared<DarkRoast>();
    auto darkRoastMocha = std::make_shared<Mocha>(std::move(darkRoast));
    auto darkRoastMochaMocha = std::make_shared<Mocha>(std::move(darkRoastMocha));
    auto darkRoastMochaMochaWhip = std::make_shared<Whip>(std::move(darkRoastMochaMocha));
    std::cout << darkRoastMochaMochaWhip.get() << std::endl;

    auto houseBlend = std::make_shared<HouseBlend>();
    auto houseBlendSoy = std::make_shared<Soy>(std::move(houseBlend));
    auto houseBlendSoyMocha = std::make_shared<Mocha>(std::move(houseBlendSoy));
    auto houseBlendSoyMochaWhip = std::make_shared<Whip>(std::move(houseBlendSoyMocha));
    std::cout << houseBlendSoyMochaWhip.get() << std::endl;
}

void PatternTester::testFactory()
{
    std::cout << decorateMessage("Simple Factory", '-') << std::endl;
    std::vector<std::unique_ptr<SimplePizza>> pizzas;
    pizzas.push_back(SimplePizzaStore::orderPizza("cheese"));
    pizzas.push_back(SimplePizzaStore::orderPizza("veggie"));
    std::cout << "\nWe ordered: \n\n";
    for (const auto &pizza : pizzas)
        std::cout << pizza.get() << std::endl;
    std::cout << decorateMessage("Simple Factory end", '-') << std::endl << std::endl;

    std::cout << decorateMessage("Abstract Factory", '-') << std::endl;
    auto nyStore = std::make_shared<NYPizzaStore>();
    auto chicagoStore = std::make_shared<ChicagoPizzaStore>();

    std::shared_ptr<Pizza> pizza = nyStore->orderPizza("cheese");
    std::cout << "Ethan ordered a \'" + pizza->name() + "\'\n\n";

    pizza = chicagoStore->orderPizza("cheese");
    std::cout << "Joel ordered a \'" + pizza->name() + "\'\n\n";

    pizza = nyStore->orderPizza("clam");
    std::cout << "Ethan ordered a \'" + pizza->name() + "\'\n\n";

    pizza = chicagoStore->orderPizza("clam");
    std::cout << "Joel ordered a \'" + pizza->name() + "\'\n\n";

    pizza = nyStore->orderPizza("pepperoni");
    std::cout << "Ethan ordered a \'" + pizza->name() + "\'\n\n";

    pizza = chicagoStore->orderPizza("pepperoni");
    std::cout << "Joel ordered a \'" + pizza->name() + "\'\n\n";

    pizza = nyStore->orderPizza("veggie");
    std::cout << "Ethan ordered a \'" + pizza->name() + "\'\n";
    std::cout << pizza.get() << "\n";

    pizza = chicagoStore->orderPizza("veggie");
    std::cout << "Joel ordered a \'" + pizza->name() + "\'\n";
    std::cout << pizza.get() << std::endl;
    std::cout << decorateMessage("Abstract Factory end", '-') << std::endl << std::endl;
}

void PatternTester::testSingleton()
{
    StaticSingleton &staticSingleton = StaticSingleton::instance();
    auto &dynamicSingleton = DynamicSingleton::instance();

    staticSingleton.setData("Data changed");
    dynamicSingleton->setData("Data changed");

//    not allowed operations:
//    StaticSingleton ss; // calling a private constructor of class 'StaticSingleton'
//    DynamicSingleton ds; // calling a private constructor of class 'DynamicSingleton'
//    StaticSingleton ss(staticSingleton); // use of deleted function
//    DynamicSingleton ds(*dynamicSingleton); // use of deleted function


    StaticSingleton &staticSingleton2 = StaticSingleton::instance();
    auto &dynamicSingleton2 = DynamicSingleton::instance();

    std::cout << "&staticSingleton address: [" << &staticSingleton
              << "] == &staticSingleton2 address: [" << &staticSingleton2 << "]"
              << std::endl;

    std::cout << "&dynamicSingleton address: [" << &dynamicSingleton
              << "] == &dynamicSingleton2 address: [" << &dynamicSingleton2 << "]\n"
              << std::endl;

    std::cout << "staticSingleton2 data: " << staticSingleton2.data() << std::endl;
    std::cout << "dynamicSingleton2 data: " << dynamicSingleton2->data() << std::endl;

    DynamicSingleton::resetInstance(); // all references and pointers for 'DynamicSingleton' now
                                       // are invalid.

//    dynamicSingleton->data(); // Undefined Behaviour !!!

    auto &dynamicSingleton3 = DynamicSingleton::instance();
    std::cout << "dynamicSingleton3 has default data: " << dynamicSingleton3->data() << std::endl;
}

void PatternTester::testCommand()
{
    auto remoteControl = std::make_unique<RemoteControl<7>>();

    auto livingRoomLight = std::make_shared<Light>("Living Room");
    auto kitchenLight = std::make_shared<Light>("Kitchen");
    auto ceilingFan= std::make_shared<CeilingFan>("Living Room");
    auto garageDoor = std::make_shared<GarageDoor>("");
    auto stereo = std::make_shared<Stereo>("Living Room");

    auto livingRoomLightOn = std::make_shared<LightOnCommand>(livingRoomLight);
    auto livingRoomLightOff = std::make_shared<LightOffCommand>(livingRoomLight);
    auto kitchenLightOn = std::make_shared<LightOnCommand>(kitchenLight);
    auto kitchenLightOff = std::make_shared<LightOffCommand>(kitchenLight);

    auto ceilingFanOn = std::make_shared<CeilingFanOnCommand>(ceilingFan);
    auto ceilingFanOff = std::make_shared<CeilingFanOffCommand>(ceilingFan);

    auto garageDoorUp = std::make_shared<GarageDoorUpCommand>(garageDoor);
    auto garageDoorDown = std::make_shared<GarageDoorDownCommand>(garageDoor);

    auto stereoOnWithCD = std::make_shared<StereoOnWithCDCommand>(stereo);
    auto stereoOff = std::make_shared<StereoOffCommand>(stereo);

    remoteControl->setCommand(0, livingRoomLightOn, livingRoomLightOff);
    remoteControl->setCommand(1, kitchenLightOn, kitchenLightOff);
    remoteControl->setCommand(2, ceilingFanOn, ceilingFanOff);
    remoteControl->setCommand(3, stereoOnWithCD, stereoOff);

    remoteControl->onButtonWasPushed(0);
    remoteControl->offButtonWasPushed(0);
    remoteControl->onButtonWasPushed(1);
    remoteControl->offButtonWasPushed(1);
    remoteControl->onButtonWasPushed(2);
    remoteControl->offButtonWasPushed(2);
    remoteControl->onButtonWasPushed(3);
    remoteControl->offButtonWasPushed(3);
}

void PatternTester::prinPreInfo(Pattern pattern)
{
    std::string message{patternName(pattern) + " pattern test start:"};
    std::cout << decorateMessage(message) << std::endl;
}

void PatternTester::prinPostInfo(Pattern pattern)
{
    std::string message{patternName(pattern) + " pattern test finished"};
    std::cout << decorateMessage(message) << std::endl << std::endl;
}

std::string PatternTester::decorateMessage(std::string message, char decorChar)
{
    message.insert(0, " ");
    message.push_back(' ');
    const size_t maxLength = 100;
    const size_t messageLength = message.size();
    const size_t decorSideLength = maxLength > messageLength ? (maxLength - messageLength) / 2 : 0;
    const std::string decorStr(decorSideLength, decorChar);
    message.insert(0, decorStr);
    message += decorStr;

    if (message.size() < maxLength)
        message.push_back(decorChar);

    return message;
}

std::string PatternTester::patternName(Pattern pattern)
{
    switch (pattern)
    {
    case Pattern::Strategy:
        return "Strategy";

    case Pattern::Observer:
        return "Observer";

    case Pattern::Decorator:
        return "Decorator";

    case Pattern::Factory:
        return "Factory";

    case Pattern::Singleton:
        return "Singleton";

    case Pattern::Command:
        return "Command";
    }

    return "No name";
}
