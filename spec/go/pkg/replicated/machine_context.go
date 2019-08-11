package replicated

import (
	"github.com/NeoResearch/libbft/pkg/events"
	"github.com/NeoResearch/libbft/pkg/machine"
	"github.com/NeoResearch/libbft/pkg/single"
)

type MachineContext interface {
	// get / set
	GetParams() []single.Param
	GetSingleTimerStateMachine() machine.SingleTimerStateMachine
	GetEvents() []events.Event
}

type MachineContextService struct {
	params                  []single.Param
	singleTimerStateMachine machine.SingleTimerStateMachine
	events                  []events.Event
}

func NewMachineContext(params []single.Param, singleTimerStateMachine machine.SingleTimerStateMachine,
	events []events.Event) MachineContext {
	return &MachineContextService{
		params,
		singleTimerStateMachine,
		events,
	}
}

func (m *MachineContextService) GetParams() []single.Param {
	return m.params
}

func (m *MachineContextService) GetSingleTimerStateMachine() machine.SingleTimerStateMachine {
	return m.singleTimerStateMachine
}

func (m *MachineContextService) GetEvents() []events.Event {
	return m.events
}
