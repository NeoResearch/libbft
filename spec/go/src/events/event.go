package events

import (
	"fmt"
	"strings"
)

type Event interface {
	// get / set
	GetName() string
	GetFrom() int
	GetParameters() []string
	// methods
	IsActivated(name string, parameters []string) bool

	String() string
}

type EventService struct {
	name       string
	from       int
	parameters []string
}

func NewEvent(name string, from int, parameters []string) Event {
	return &EventService{
		name,
		from,
		parameters,
	}
}

func (e *EventService) GetName() string {
	return e.name
}

func (e *EventService) GetFrom() int {
	return e.from
}

func (e *EventService) String() string {
	var sb strings.Builder
	sb.WriteString(fmt.Sprintf("Event %v(", e.GetName()))
	for i, param := range e.GetParameters() {
		sb.WriteString(fmt.Sprintf("%v", param))
		if i != len(e.GetParameters())-1 {
			sb.WriteString(",")
		}
	}
	sb.WriteString(")")
	return sb.String()
}

func (e *EventService) IsActivated(name string, parameters []string) bool {
	if e.GetName() == name && len(e.GetParameters()) == len(parameters) {
		for i, v := range e.GetParameters() {
			if parameters[i] != v {
				return false
			}
		}
		return true
	}
	return false
}

func (e *EventService) GetParameters() []string {
	return e.parameters
}
