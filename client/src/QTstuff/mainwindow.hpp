#pragma once

#include <memory>

#include "Connection.hpp"
#include "Message.hpp"

#include<QWidget>
#include<QGroupBox>

// #include <qt5/QtWidgets/QMainWindow>
// #include <qt5/QtGui/QCloseEvent>

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(std::shared_ptr<Connection> connection, QWidget *parent = nullptr);

private:
    QGroupBox *createFirstExclusiveGroup();
    QGroupBox *createSecondExclusiveGroup();
    QGroupBox *createNonExclusiveGroup();
    QGroupBox *createPushButtonGroup();

    QGroupBox *createChatField();
    QGroupBox *messageInputSendGroup();

    std::shared_ptr<Connection> _connection;
};