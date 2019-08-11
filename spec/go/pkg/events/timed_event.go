package events

import (
	"fmt"
	"github.com/NeoResearch/libbft/pkg/timing"
	"strings"
)

type TimedEvent interface {
	// superclass
	getEvent() Event
	// get / set
	GetName() string
	GetFrom() int
	GetParameters() []string
	// methods
	IsActivated(name string, parameters []string) bool

	// get / set
	GetTimer() timing.Timer

	String() string
}

type TimedEventService struct {
	event Event
	timer timing.Timer
}

func NewTimedEvent(name string, from int, parameters []string, countdown float64) TimedEvent {
	resp := TimedEventService{
		event: NewEvent(name, from, parameters),
		timer: timing.NewDefaultTimer(),
	}
	resp.timer.Init(countdown)
	return &resp
}

func (t *TimedEventService) GetName() string {
	return t.getEvent().GetName()
}

func (t *TimedEventService) GetFrom() int {
	return t.getEvent().GetFrom()
}

func (t *TimedEventService) GetParameters() []string {
	return t.getEvent().GetParameters()
}

func (t *TimedEventService) IsActivated(name string, parameters []string) bool {
	return t.getEvent().IsActivated(name, parameters) && t.GetTimer().Expired()
}

func (t *TimedEventService) GetTimer() timing.Timer {
	return t.timer
}

func (t *TimedEventService) String() string {
	var sb strings.Builder

	sb.WriteString(fmt.Sprintf("TimedEvent %v", t.GetName()))
	for i, v := range t.GetParameters() {
		if i != len(t.GetParameters()) {
			sb.WriteString(",")
		}
		sb.WriteString(v)
	}
	if t.GetTimer().Expired() {
		sb.WriteString(") expired")
	} else {
		sb.WriteString(") notexpired")
	}
	return sb.String()
}

func (t *TimedEventService) getEvent() Event {
	return t.event
}
