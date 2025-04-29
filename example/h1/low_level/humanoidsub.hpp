#pragma once

#include <iostream>
#include <string>

#include "unitree/robot/channel/channel_publisher.hpp"
#include "unitree/robot/channel/channel_subscriber.hpp"
#include <unitree/common/thread/thread.hpp>
#include <unitree/idl/hg/LowState_.hpp>

class HumanoidSub {
public:
    unitree::robot::ChannelSubscriberPtr<unitree_hg::msg::dds_::LowState_>
    lowstate_subscriber_;

    HumanoidSub(const std::string &networkInterface = "") {
        unitree::robot::ChannelFactory::Instance()->Init(0, networkInterface);
        std::cout << "Initialize channel factory." << std::endl;

        lowstate_subscriber_.reset(
            new unitree::robot::ChannelSubscriber<unitree_hg::msg::dds_::LowState_>(
                "rt/lowstate"));
        lowstate_subscriber_->InitChannel(
            std::bind(&HumanoidSub::LowStateHandler, this,
                      std::placeholders::_1),
            1);
    }

    ~HumanoidSub() = default;

    void LowStateHandler(const void *message) {
        unitree_hg::msg::dds_::LowState_ low_state =
                *(unitree_hg::msg::dds_::LowState_ *) message;

        std::cout << "LowStateHandler" << std::endl;
    }
};
