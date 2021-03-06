package machine

import (
	"fmt"
	"github.com/NeoResearch/libbft/src/single"
	"github.com/NeoResearch/libbft/src/timing"
	"strings"
	"time"
)

type TimedStateMachine interface {
	// get / set
	GetClock() timing.Clock
	GetMe() int
	GetName() string
	// methods
	OnEnterState(current single.State, param single.Param)
	BeforeUpdateState(current single.State, param single.Param) bool
	AfterUpdateState(current single.State, param single.Param, updated bool) bool
	UpdateState(current single.State, param single.Param) (single.State, bool, error)
	IsFinal(current single.State, param single.Param) bool
	Initialize(current single.State, param single.Param) single.State
	OnFinished(current single.State, param single.Param)
	Run(current single.State, param single.Param) (single.State, error)

	String() string
}

type TimedStateMachineService struct {
	clock timing.Clock
	me    int
	name string
}

func NewDefaultTimedStateMachine() TimedStateMachine {
	return NewTimedStateMachine(nil, 0, "")
}

func NewTimedStateMachine(clock timing.Clock, me int, name string) TimedStateMachine {
	return &TimedStateMachineService{
		clock,
		me,
		name,
	}
}

func (t *TimedStateMachineService) GetClock() timing.Clock {
	return t.clock
}

func (t *TimedStateMachineService) GetMe() int {
	return t.me
}

func (t *TimedStateMachineService) BeforeUpdateState(current single.State, param single.Param) bool {
	panic("interface method")
}

func (t *TimedStateMachineService) UpdateState(current single.State, param single.Param) (single.State, bool, error) {
	panic("interface method")
}

func (t *TimedStateMachineService) IsFinal(current single.State, param single.Param) bool {
	panic("interface method")
}

func (t *TimedStateMachineService) Initialize(current single.State, param single.Param) single.State {
	panic("interface method")
}

func (t *TimedStateMachineService) OnEnterState(current single.State, param single.Param) {
	panic("interface method")
}

func (t *TimedStateMachineService) AfterUpdateState(current single.State, param single.Param, updated bool) bool {
	if !updated {
		time.Sleep(100 * time.Millisecond)
	}
	return false
}

func (t *TimedStateMachineService) OnFinished(current single.State, param single.Param) {
	panic("interface method")
}

func (t *TimedStateMachineService) Run(initial single.State, param single.Param) (single.State, error) {
	return RunMachine(t, initial, param)
}

func (t *TimedStateMachineService) String() string {
	var sb strings.Builder
	sb.WriteString("TSM {")
	sb.WriteString(fmt.Sprintf("#id = %v;", t.GetMe()))
	sb.WriteString(fmt.Sprintf("clock = %v;", t.GetClock()))
	sb.WriteString("TSM }")
	return sb.String()
}

func (t *TimedStateMachineService) GetName() string {
	return t.name
}

func RunMachine(t TimedStateMachine, initial single.State, param single.Param) (single.State, error) {
	current := t.Initialize(initial, param)
	if current == nil {
		return nil, nil
	}
	t.OnEnterState(current, param)

	for !t.IsFinal(current, param) {
		var err error
		if t.BeforeUpdateState(current, param) {
			return nil, nil
		}
		updated := false
		current, updated, err = t.UpdateState(current, param)
		if err != nil {
			return nil, nil
		}
		if updated {
			t.OnEnterState(current, param)
		}
		if t.AfterUpdateState(current, param, updated) {
			return nil, nil
		}
	}

	t.OnFinished(current, param)

	return current, nil
}
