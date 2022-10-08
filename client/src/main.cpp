#include <memory>

#include <Connection.hpp>
#include <NetModel.hpp>
#include <UI.hpp>

int main (){
    auto connection = std::make_shared<Connection>();
    auto net_model = std::make_shared<NetModel>(connection);
    auto ui = std::make_shared<UI>();

    net_model->subscribe(ui);
}