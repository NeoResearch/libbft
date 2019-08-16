package replicated

import (
	"errors"
	"github.com/NeoResearch/libbft/src/events"
)

type MultiContext interface {
	GetVm() []MachineContext
	Broadcast(event string, from int, eventParams []string) error
	BroadcastEvent(event events.Event, from int) error
	SendToVm(event string, from int, to int, eventParams []string) error
	SendToVmEvent(event events.Event, to int) error
	HasEvent(name string, at int, eventParams []string) (bool, error)
	ConsumeEvent(name string, at int, eventParams []string) error
}

type MultiContextService struct {
	vm []MachineContext
}

func NewMultiContext(vm []MachineContext) MultiContext {
	return &MultiContextService{
		vm,
	}
}

func (m *MultiContextService) GetVm() []MachineContext {
	return m.vm
}

func (m *MultiContextService) Broadcast(event string, from int, eventParams []string) error {
	return m.BroadcastEvent(events.NewEvent(event, from, eventParams), from)
}

func (m *MultiContextService) BroadcastEvent(event events.Event, from int) error {
	for i := 0; i < len(m.GetVm()); i++ {
		if i != from {
			return m.SendToVmEvent(event, i)
		}
	}
	return nil
}

func (m *MultiContextService) SendToVm(event string, from int, to int, eventParams []string) error {
	return m.SendToVmEvent(events.NewEvent(event, from, eventParams), to)
}

func (m *MultiContextService) SendToVmEvent(event events.Event, to int) error {
	if to < 0 || to >= len(m.GetVm()) {
		return errors.New("invalid \"to\" destination")
	}
	m.GetVm()[to].AddEvent(event)
	return nil
}

func (m *MultiContextService) HasEvent(name string, at int, eventParams []string) (bool, error) {
	if at < 0 || at >= len(m.GetVm()) {
		return false, errors.New("invalid \"at\" destination")
	}
	for _, event := range m.GetVm()[at].GetEvents() {
		if event.IsActivated(name, eventParams) {
			return true, nil
		}
	}
	return false, nil
}

func (m *MultiContextService) ConsumeEvent(name string, at int, eventParams []string) error {
	if at < 0 || at >= len(m.GetVm()) {
		return errors.New("invalid \"at\" destination")
	}
	for i, event := range m.GetVm()[at].GetEvents() {
		if event.IsActivated(name, eventParams) {
			return m.GetVm()[at].RemoveEvent(i)
		}
	}
	return nil
}
