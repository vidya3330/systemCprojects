#ifndef __simple_bus_test_h
#define __simple_bus_test_h

#include <systemc.h>

//#include "simple_bus_master_blocking.h"
#include "simple_bus_master_non_blocking.h"
//#include "simple_bus_master_direct.h"
//#include "simple_bus_slow_mem.h"
#include "simple_bus.h"
//#include "simple_bus_fast_mem.h"
#include "simple_bus_arbiter.h"
#include "ssc.h"

SC_MODULE(simple_bus_test)
{
  // channels
  sc_clock C1;

  sc_signal< SSC_TxRx_Tlm > mas0Tsla1R;
  sc_signal< SSC_TxRx_Tlm > mas0Rsla1T;
  sc_signal< SSC_TxRx_Tlm > sla0Tmas1R;
  sc_signal< SSC_TxRx_Tlm > sla0Rmas1T;
  
  sc_signal< sc_time > fssc0Clock;
  sc_signal< sc_time > fclc0Clock;

  sc_signal< sc_time > fssc1Clock;
  sc_signal< sc_time > fclc1Clock;

  sc_signal< bool > tir0Wire;
  sc_signal< bool > rir0Wire;
  sc_signal< bool > tir1Wire;
  sc_signal< bool > rir1Wire;

  // module instances
  //simple_bus_master_blocking     *master_b;
  simple_bus_master_non_blocking *master_nb;
  //simple_bus_master_direct       *master_d;
  //simple_bus_slow_mem            *mem_slow;
  simple_bus                     *bus;
  //simple_bus_fast_mem            *mem_fast;
  simple_bus_arbiter             *arbiter;
  ssc							 *ssc0;
  ssc							 *ssc1;

  // constructor
  SC_CTOR(simple_bus_test)
    : C1("C1")
  {
	fssc0Clock.write(sc_time(10,SC_NS));
	fclc0Clock.write(sc_time(100,SC_NS));

	fssc1Clock.write(sc_time(10,SC_NS));
	fclc1Clock.write(sc_time(100,SC_NS));

    // create instances
    //master_b = new simple_bus_master_blocking("master_b", 4, 0x4c, false, 300);
    master_nb = new simple_bus_master_non_blocking("master_nb", 3, 0xF03100F4, false, 20);
    //master_d = new simple_bus_master_direct("master_d", 0x78, 100);
    //mem_fast = new simple_bus_fast_mem("mem_fast", 0xF0310000, 0xF03100FF);
    ssc0 = new ssc("ssc0", 0xF0310000, 0xF03100FF);
	ssc1 = new ssc("ssc1", 0xF0310100, 0xF03101FF);
    //mem_slow = new simple_bus_slow_mem("mem_slow", 0x80, 0xff, 1);
    // bus = new simple_bus("bus",true); // verbose output
    bus = new simple_bus("bus");
    // arbiter = new simple_bus_arbiter("arbiter",true); // verbose output
    arbiter = new simple_bus_arbiter("arbiter");

    // connect instances
    //master_d->clock(C1);
    bus->clock(C1);
    //master_b->clock(C1);
    master_nb->clock(C1);
    //mem_slow->clock(C1);

	ssc0->masterTx(mas0Tsla1R);
	ssc0->masterRx(mas0Rsla1T);
	ssc0->slaveTx(sla0Tmas1R);
	ssc0->slaveRx(sla0Rmas1T);

	ssc0->tir(tir0Wire);
	ssc0->rir(rir0Wire);
	
	ssc1->masterTx(sla0Rmas1T);
	ssc1->masterRx(sla0Tmas1R);
	ssc1->slaveTx(mas0Rsla1T);
	ssc1->slaveRx(mas0Tsla1R);

	ssc1->tir(tir1Wire);
	ssc1->rir(rir1Wire);
	
	master_nb->ssc0Tir(tir0Wire);
	master_nb->ssc0Rir(rir0Wire);
	master_nb->ssc1Tir(tir1Wire);
	master_nb->ssc1Rir(rir1Wire);

	ssc0->fsscClockInputPort(fssc0Clock);
	ssc0->fclcClockInputPort(fclc0Clock);

	ssc1->fsscClockInputPort(fssc1Clock);
	ssc1->fclcClockInputPort(fclc1Clock);

    //master_d->bus_port(*bus);
    //master_b->bus_port(*bus);
    master_nb->bus_port(*bus);
    bus->arbiter_port(*arbiter);
    //bus->slave_port(*mem_slow);
    //bus->slave_port(*mem_fast);
	bus->slave_port(*ssc0);
	bus->slave_port(*ssc1);

  }

  // destructor
  ~simple_bus_test()
  {
    //if (master_b) {delete master_b; master_b = 0;}
    if (master_nb) {delete master_nb; master_nb = 0;}
    //if (master_d) {delete master_d; master_d = 0;}
    //if (mem_slow) {delete mem_slow; mem_slow = 0;}
    if (bus) {delete bus; bus = 0;}
    //if (mem_fast) {delete mem_fast; mem_fast = 0;}
    if (arbiter) {delete arbiter; arbiter = 0;}
	if (ssc0) {delete ssc0; ssc0 = 0;}
	if (ssc1) {delete ssc1; ssc1 = 0;}
  }

}; // end class simple_bus_test

#endif
