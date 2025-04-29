#pragma once

#include <iostream>
#include <string>

#include "unitree/robot/channel/channel_publisher.hpp"
#include "unitree/robot/channel/channel_subscriber.hpp"
#include <unitree/common/thread/thread.hpp>
#include <unitree/idl/hg/LowState_.hpp>
#include <unitree/idl/hg/MotorCmd_.hpp>

class HumanoidPub {
public:
    HumanoidPub(const std::string &networkInterface = "") {
        unitree::robot::ChannelFactory::Instance()->Init(0, networkInterface);
        std::cout << "Initialize channel factory." << std::endl;

        low_state_publisher.reset(new unitree::robot::ChannelPublisher<unitree_hg::msg::dds_::LowState_>("rt/lowstate"));
        // low_state_publisher.reset(new unitree::robot::ChannelPublisher<unitree_hg::msg::dds_::MotorCmd_>("rt/motorcmd2"));
        low_state_publisher->InitChannel();

        state_writer_ptr = unitree::common::CreateRecurrentThreadEx("state_writer", UT_CPU_ID_NONE, 200000, &HumanoidPub::LowStateWriter, this);
    }

    ~HumanoidPub() = default;

    void LowStateWriter() {
        std::cout << "Publishing state..." << std::endl;

        unitree_hg::msg::dds_::LowState_ dds_low_state{};
        dds_low_state.mode_pr(77);
        // unitree_hg::msg::dds_::MotorCmd_ dds_low_state{};

        low_state_publisher->Write(dds_low_state);
    }

    unitree::robot::ChannelPublisherPtr<unitree_hg::msg::dds_::LowState_> low_state_publisher;
    // unitree::robot::ChannelPublisherPtr<unitree_hg::msg::dds_::MotorCmd_> low_state_publisher;
    unitree::common::ThreadPtr state_writer_ptr;
};
