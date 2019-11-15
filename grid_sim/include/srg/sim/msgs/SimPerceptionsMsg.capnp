@0xf338162d11fbd927;
using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("srg::sim");
using IDMsg = import "/capnzero/ID.capnp";

struct SimPerceptionsMsg {
  receiverID @0 :IDMsg.ID;
  cellPerceptions @1 :List(CellPerceptions);

  struct CellPerceptions {
      x @0: UInt32;
      y @1: UInt32;
      perceptions @2 :List(Perception);
  }

  struct Perception {
      type @0: Type;
      state @1: State;
      objectID @2 :IDMsg.ID;
      robotID @3 :IDMsg.ID;
      x @4: UInt32;
      y @5: UInt32;

    enum Type {
      door @0;
      cupred @1;
      cupblue @2;
      cupyellow @3;
      robot @4;
    }

    enum State {
      open @0;
      closed @1;
      carried @2;
      undefined @3;
    }
  }
}