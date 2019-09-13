package rpc_replicated

import (
	"fmt"
	"github.com/NeoResearch/libbft/src/bftevent"
	"github.com/NeoResearch/libbft/src/events"
	"github.com/NeoResearch/libbft/src/machine"
	"github.com/NeoResearch/libbft/src/single"
)

type RPCMachineContext interface {
	// get / set
	GetParams() []single.Param
	GetMe() int
	GetWorld() []bftevent.BFTEventClient
	GetEvents() []events.Event
	// methods
	TestSetRegularDelay(testRegularDelayMS int)
	TestSetDropRate(dropRate float64)
	HasEvent(name string, eventArgs []string) bool
	RegisterEvent(event events.Event)
	AddEvents(events []events.Event)
	Broadcast(event string, eventArgs []string)
	AddEventFromRPC(name string, from machine.MachineId, parameters []string, delay int)

	String() string
}

type RPCMachineContextService struct {
	params             []single.Param
	me                 int
	world              []bftevent.BFTEventClient
	events             []events.Event
	testRegularDelayMS int
	testDropRate       float64
}

func NewRPCMachineContext(params []single.Param, me int, world []bftevent.BFTEventClient, seed int) RPCMachineContext {
	return &RPCMachineContextService{
		events:             nil,
		testRegularDelayMS: 0,
		testDropRate:       0.0,
	}
}

func (r *RPCMachineContextService) GetParams() []single.Param {
	return r.params
}

func (r *RPCMachineContextService) GetMe() int {
	return r.me
}

func (r *RPCMachineContextService) GetWorld() []bftevent.BFTEventClient {
	return r.world
}

func (r *RPCMachineContextService) GetEvents() []events.Event {
	return r.events
}

func (r *RPCMachineContextService) TestSetRegularDelay(testRegularDelayMS int) {
	r.testRegularDelayMS = testRegularDelayMS
}

func (r *RPCMachineContextService) TestSetDropRate(dropRate float64) {
	r.testDropRate = dropRate
}

func (r *RPCMachineContextService) HasEvent(name string, eventArgs []string) bool {
	for _, v := range r.GetEvents() {
		if v.IsActivated(name, eventArgs) {
			return true
		}
	}
	return false
}

func (r *RPCMachineContextService) RegisterEvent(event events.Event) {
	fmt.Printf("RPCMachineContext registering event '{}'", event)
	r.events = append(r.events, event)
}

func (r *RPCMachineContextService) AddEvents(events []events.Event) {
	for _, event := range r.GetEvents() {
		r.RegisterEvent(event)
	}
}

func (r *RPCMachineContextService) Broadcast(event string, eventArgs []string) {
	panic("implement me")
}

func (r *RPCMachineContextService) AddEventFromRPC(name string, from machine.MachineId, parameters []string, delay int) {
	if delay == 0 {
		r.RegisterEvent(events.NewEvent(name, from, parameters))
	} else {
		r.RegisterEvent(events.NewTimedEvent(delay / 1000.0, name, from, parameters))
	}
}

func (r *RPCMachineContextService) String() string {
	panic("implement me")
}
