package machine

import (
	"errors"
	"fmt"
	"github.com/NeoResearch/libbft/pkg/single"
	"github.com/NeoResearch/libbft/pkg/timing"
	"strings"
)

type SingleTimerStateMachine interface {
	// Superclass
	getTimedStateMachine() TimedStateMachine
	// get / set
	GetClock() timing.Clock
	GetMe() int
	GetName() string
	// methods
	OnEnterState(current single.State, param single.Param)
	BeforeUpdateState(current single.State, param single.Param) bool
	AfterUpdateState(current single.State, param single.Param, updated bool) bool
	UpdateState(current single.State, param single.Param) (single.State, bool)
	IsFinal(current single.State, param single.Param) bool
	Initialize(current single.State, param single.Param) single.State
	OnFinished(current single.State, param single.Param)
	Run(current single.State, param single.Param) single.State

	// get / set
	GetTimer() timing.Timer
	GetStates() []single.State
	GetGlobalTransitions() []single.Transition
	SetWatchdog(maxTime float64)
	// methods
	GetStateByName(name string) single.State
	GetDefaultState() single.State
	RegisterState(state single.State) error
	RegisterGlobal(transition single.Transition) error
	FindGlobalTransition(param single.Param) single.Transition

	StringFormat(format string) string
	String() string
}

type SingleTimerStateMachineService struct {
	timedStateMachine TimedStateMachine
	timer             timing.Timer
	states            []single.State
	globalTransitions []single.Transition
	watchdog          timing.Timer
}

func NewSingleTimerStateMachine(
	timedStateMachine TimedStateMachine,
	timer timing.Timer,
	states []single.State,
	globalTransitions []single.Transition) SingleTimerStateMachine {
	return &SingleTimerStateMachineService{
		timedStateMachine,
		timer,
		states,
		globalTransitions,
		nil,
	}
}

func (s *SingleTimerStateMachineService) GetClock() timing.Clock {
	return s.getTimedStateMachine().GetClock()
}

func (s *SingleTimerStateMachineService) GetMe() int {
	return s.getTimedStateMachine().GetMe()
}

func (s *SingleTimerStateMachineService) OnEnterState(current single.State, param single.Param) {
	fmt.Printf("entering state: %v\n", current)

	if s.watchdog != nil {
		s.watchdog.Reset()
	}
}

func (s *SingleTimerStateMachineService) BeforeUpdateState(current single.State, param single.Param) bool {
	if s.watchdog != nil && s.watchdog.Expired() {
		fmt.Printf("StateMachine FAILED MAXTIME %v\n", s.watchdog.GetCountdown())
		return true
	}

	return false
}

func (s *SingleTimerStateMachineService) AfterUpdateState(current single.State, param single.Param, updated bool) bool {
	return s.getTimedStateMachine().AfterUpdateState(current, param, updated)
}

func (s *SingleTimerStateMachineService) UpdateState(current single.State, param single.Param) (single.State, bool) {
	r := false

	gt := s.FindGlobalTransition(param)
	if gt != nil {
		fmt.Printf("-> found valid global transition! %v\n", gt)
		current = gt.Execute(s.GetTimer(), param, s.GetMe())
		r = true
	}

	if current != nil {
		got := current.TryGetTransition(s.GetTimer(), param, s.GetMe())
		if got != nil {
			fmt.Printf("-> found valid global transition! %v\n", gt)
			current = got.Execute(s.GetTimer(), param, s.GetMe())
			r = true
		}
	}
	return current, r
}

func (s *SingleTimerStateMachineService) IsFinal(current single.State, param single.Param) bool {
	return current.IsFinal()
}

func (s *SingleTimerStateMachineService) Initialize(current single.State, param single.Param) single.State {
	if current == nil && len(s.GetStates()) == 0 {
		return nil
	}

	fmt.Println()
	fmt.Println("===========")
	fmt.Println("begin run()")
	fmt.Println("===========")

	fmt.Println("OnInitialize() Single MACHINE!")
	if s.watchdog != nil {
		s.watchdog.Reset()
	} else {
		fmt.Println("No watchdog configured!")
	}

	fmt.Println("will initialize timer")
	s.GetTimer().Init()

	fmt.Println("will reset timer")
	s.GetTimer().Reset()

	if current == nil {
		current = s.GetDefaultState()
	}
	return current
}

func (s *SingleTimerStateMachineService) OnFinished(current single.State, param single.Param) {
	fmt.Println()
	fmt.Println("=================")
	fmt.Println("finished machine!")
	fmt.Println("=================")
}

func (s *SingleTimerStateMachineService) String() string {
	return s.StringFormat("")
}

func (s *SingleTimerStateMachineService) StringFormat(format string) string {
	var sb strings.Builder

	if format == "graphviz" {
	} else {
		// standard text
		sb.WriteString("STSM {")
		sb.WriteString(fmt.Sprintf("#id = %v;',", s.GetMe()))
		sb.WriteString(fmt.Sprintf("Timer='%v';", s.GetTimer()))
		sb.WriteString("States=[")
		for _, state := range s.GetStates() {
			sb.WriteString(fmt.Sprintf("%v;", state))
		}
		sb.WriteString("]")
		sb.WriteString("}")
	}
	return sb.String()
}

func (s *SingleTimerStateMachineService) GetTimer() timing.Timer {
	return s.timer
}

func (s *SingleTimerStateMachineService) GetStates() []single.State {
	return s.states
}

func (s *SingleTimerStateMachineService) GetGlobalTransitions() []single.Transition {
	return s.globalTransitions
}

func (s *SingleTimerStateMachineService) SetWatchdog(maxTime float64) {
	s.watchdog = timing.NewDefaultTimer().Init(maxTime)
}

func (s *SingleTimerStateMachineService) GetStateByName(name string) single.State {
	for _, state := range s.GetStates() {
		if state.GetName() == name {
			return state
		}
	}
	return nil
}

func (s *SingleTimerStateMachineService) GetDefaultState() single.State {
	if len(s.GetStates()) == 0 {
		return nil
	}
	return s.GetStates()[0]
}

func (s *SingleTimerStateMachineService) RegisterState(state single.State) error {
	if state == nil {
		return errors.New("the state can not be nil")
	}
	s.states = append(s.states, state)
}

func (s *SingleTimerStateMachineService) RegisterGlobal(transition single.Transition) error {
	if transition == nil {
		return errors.New("the transition can not be nil")
	}
	s.globalTransitions = append(s.globalTransitions, transition)
}

func (s *SingleTimerStateMachineService) FindGlobalTransition(param single.Param) single.Transition {
	for _, transition := range s.GetGlobalTransitions() {
		if transition.IsValid(s.GetTimer(), param, s.GetMe()) {
			return transition
		}
	}
	return nil
}

func (s *SingleTimerStateMachineService) GetName() string {
	return s.getTimedStateMachine().GetName()
}

func (s *SingleTimerStateMachineService) Run(current single.State, param single.Param) single.State {
	return s.getTimedStateMachine().Run(current, param)
}

func (s *SingleTimerStateMachineService) getTimedStateMachine() TimedStateMachine {
	return s.timedStateMachine
}