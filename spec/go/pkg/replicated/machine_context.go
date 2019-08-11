package replicated

import (
	"errors"
	"github.com/NeoResearch/libbft/pkg/events"
	"github.com/NeoResearch/libbft/pkg/machine"
	"github.com/NeoResearch/libbft/pkg/single"
)

type MachineContext interface {
	// get / set
	GetParams() []single.Param
	GetMachine() machine.SingleTimerStateMachine
	GetEvents() []events.Event
	// methods
	AddEvent(event events.Event)
	RemoveEvent(index int) error
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

func (m *MachineContextService) GetMachine() machine.SingleTimerStateMachine {
	return m.singleTimerStateMachine
}

func (m *MachineContextService) GetEvents() []events.Event {
	return m.events
}

func (m *MachineContextService) AddEvent(event events.Event) {
	m.events = append(m.events, event)
}

func (m *MachineContextService) RemoveEvent(index int) error {
	if index < 0 || index > len(m.events) {
		return errors.New("invalid index")
	}
	m.events = append(m.events[:index], m.events[index+1:]...)
}
