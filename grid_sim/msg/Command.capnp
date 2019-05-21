@0xcbaac9bc054f0dc5;
using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("srgsim");

using IDMsg = import "ID.capnp";

struct Command {
  senderId @0 :IDMsg.ID;
  action @1 :Action;
  x @2: UInt32;
  y @3: UInt32;

  enum Action {
      spawn @0;
      goleft @1;
      goright @2;
      goup @3;
      godown @4;
      pickup @5;
      putdown @6;
      open @7;
      close @8;
  }
}