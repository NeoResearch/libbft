package replicated

import "fmt"

type Event interface {
	GetType() string
	GetName() string
	GetFrom() int
	String() string
}

type EventService struct {
	theType string
	name    string
	from    int
}

func NewEvent(theType string, name string, from int) Event {
	return &EventService{
		theType,
		name,
		from,
	}
}

func (e *EventService) GetType() string {
	return e.theType
}

func (e *EventService) GetName() string {
	return e.name
}

func (e *EventService) GetFrom() int {
	return e.from
}

func (e *EventService) String() string {
	return fmt.Sprintf("Event {name=%v; type=%v}", e.GetName(), e.GetType())
}
