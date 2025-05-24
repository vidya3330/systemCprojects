 #include "systemc.h"
 #include "simple_bus_test.h"
 int sc_main(int, char **)
{
     sc_trace_file *vcdfile;
     vcdfile = sc_create_vcd_trace_file("waveform");
 
     simple_bus_test top("top");
  
      sc_trace(vcdfile, top.ssc0->masterTx, "masterTx");
      sc_trace(vcdfile, top.ssc1->slaveRx, "slaveRx");
      sc_trace(vcdfile, top.ssc1->slaveTx, "slaveTx");
      sc_trace(vcdfile, top.ssc0->masterRx, "masterRx");
      sc_trace(vcdfile, top.ssc0->tir, "tir0");
      sc_trace(vcdfile, top.ssc1->tir, "tir1");
      sc_trace(vcdfile, top.ssc1->rir, "rir1");
      sc_trace(vcdfile, top.ssc0->rir, "rir0");
      //sc_trace(vcdfile, top.fAscClockEvent, "AscClockIn");
     sc_start(100000, SC_NS);
  
      sc_close_vcd_trace_file(vcdfile);
  
    return 0;
  }

