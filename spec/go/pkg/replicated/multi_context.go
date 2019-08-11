package replicated

import "github.com/NeoResearch/libbft/pkg/events"

type MultiContext interface {
	GetVm() []MachineContext
	BroadcastParams(event string, from int32, eventParams []string)
	Broadcast(event events.Event, from int32)
	SendToParams(event string, from int32, eventParams []string)
	SendTo(event events.Event, to int32)
	HasEvent(name string, at int32, eventParams []string) bool
	ConsumeEvent(name string, at int32, eventParams []string)
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
	panic("implement me")
}

func (m *MultiContextService) BroadcastParams(event string, from int32, eventParams []string) {
	panic("implement me")
}

func (m *MultiContextService) Broadcast(event events.Event, from int32) {
	panic("implement me")
}

func (m *MultiContextService) SendToParams(event string, from int32, eventParams []string) {
	panic("implement me")
}

func (m *MultiContextService) SendTo(event events.Event, to int32) {
	panic("implement me")
}

func (m *MultiContextService) HasEvent(name string, at int32, eventParams []string) bool {
	panic("implement me")
}

func (m *MultiContextService) ConsumeEvent(name string, at int32, eventParams []string) {
	panic("implement me")
}
