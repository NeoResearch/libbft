package replicated

type ScheduledEvent interface {
	GetName() string
	GetEventParams() []string
	GetCountdown() float64
	GetMachine() int32
}

type ScheduledEventService struct {
	name        string
	eventParams []string
	countdown   float64
	machine     int32
}

func NewScheduledEvent(name string, eventParams []string, countdown float64, machine int32) ScheduledEvent {
	return &ScheduledEventService{
		name,
		eventParams,
		countdown,
		machine,
	}
}

func (s *ScheduledEventService) GetName() string {
	return s.name
}

func (s *ScheduledEventService) GetEventParams() []string {
	return s.eventParams
}

func (s *ScheduledEventService) GetCountdown() float64 {
	return s.countdown
}

func (s *ScheduledEventService) GetMachine() int32 {
	return s.machine
}
