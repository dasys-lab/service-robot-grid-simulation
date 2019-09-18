#pragma once

#include "SimCommand.h"

#include <capnzero/CapnZero.h>

namespace essentials {
    class IDManager;
}

namespace srgsim {
    class ContainerUtils {
    public:
        static SimCommand toSimCommand(::capnp::FlatArrayMessageReader& msg, essentials::IDManager* idManager);
        static void toMsg(SimCommand simCommand, ::capnp::MallocMessageBuilder& builder);
    private:
    private:
        ContainerUtils() = delete;
    };
}