#pragma once

namespace srgsim
{
namespace container
{
struct Command
{
};
} // namespace container
} // namespace srgsim

//
//#include <string>
//#include <tuple>
//#include <vector>
//
//#include "engine/AgentIDConstPtr.h"
//
// namespace srgsim
//{
// typedef std::tuple<AgentIDConstPtr, std::string, std::string, std::string, std::string, std::string, std::vector<AgentIDConstPtr>> stdAlicaEngineInfo;
// struct Command
//{
//    AlicaEngineInfo()
//            : senderID(nullptr)
//    {
//    }
//    AgentIDConstPtr senderID;
//    std::vector<AgentIDConstPtr> robotIDsWithMe;
//
//    AlicaEngineInfo(stdAlicaEngineInfo&& s)
//    {
//        senderID = std::get<0>(s);
//        masterPlan = std::move(std::get<1>(s));
//        currentPlan = std::move(std::get<2>(s));
//        currentState = std::move(std::get<3>(s));
//        currentRole = std::move(std::get<4>(s));
//        currentTask = std::move(std::get<5>(s));
//        robotIDsWithMe = std::move(std::get<6>(s));
//    }
//
//    stdAlicaEngineInfo toStandard() const { return std::make_tuple(senderID, masterPlan, currentPlan, currentState, currentRole, currentTask, robotIDsWithMe);
//    }
//};
//} // namespace alica