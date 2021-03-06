package machine

import (
	"errors"
	"fmt"
	"github.com/NeoResearch/libbft/src/single"
	"github.com/NeoResearch/libbft/src/timing"
	"github.com/NeoResearch/libbft/src/util"
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
	UpdateState(current single.State, param single.Param) (single.State, bool, error)
	IsFinal(current single.State, param single.Param) bool
	Initialize(current single.State, param single.Param) single.State
	OnFinished(current single.State, param single.Param)
	RunDefault() (single.State, error)
	Run(current single.State, param single.Param) (single.State, error)

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
	FindGlobalTransition(param single.Param) (single.Transition, error)

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

func NewSingleTimerStateMachineClock(timer timing.Timer) SingleTimerStateMachine {
	return NewSingleTimerStateMachineSizes(nil, 0, "STSM", timer, 0, 0)
}

func NewSingleTimerStateMachine(clock timing.Clock, me int, name string, timer timing.Timer) SingleTimerStateMachine {
	return NewSingleTimerStateMachineSizes(clock, me, name, timer, 0, 0)
}

func NewSingleTimerStateMachineSizes(clock timing.Clock, me int, name string, timer timing.Timer,
	numberOfStates int, numberOfGlobalTransitions int) SingleTimerStateMachine {
	return &SingleTimerStateMachineService{
		NewTimedStateMachine(clock, me, name),
		timer,
		make([]single.State, numberOfStates),
		make([]single.Transition, numberOfGlobalTransitions),
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

func (s *SingleTimerStateMachineService) UpdateState(current single.State, param single.Param) (single.State, bool, error) {
	r := false
	{
		gt, err := s.FindGlobalTransition(param)
		if err != nil {
			return nil, false, err
		}
		if gt != nil {
			fmt.Printf("-> found valid global transition! %v\n", gt)
			current = gt.Execute(s.GetTimer(), param, s.GetMe())
			r = true
		}
	}

	if current != nil {
		got, err := current.TryGetTransition(s.GetTimer(), param, s.GetMe())
		if err != nil {
			return nil, false, err
		}
		if got != nil {
			fmt.Printf("-> found valid transition! %v\n", got)
			current = got.Execute(s.GetTimer(), param, s.GetMe())
			r = true
		}
	}
	return current, r, nil
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
	s.GetTimer().InitDefault()

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

	if format == util.GraphivizFormat {
		sb.WriteString(fmt.Sprintf("digraph %v {\n", s.GetName()))
		sb.WriteString("//graph [bgcolor=lightgoldenrodyellow]\n")
		sb.WriteString("//rankdir=LR;\n")
		sb.WriteString("size=\"11\"\n")
		// add states
		sb.WriteString("Empty [ label=\"\", width=0, height=0, style = invis ];\n")
		for _, state := range s.GetStates() {
			sb.WriteString("node [shape = ")
			if state.IsFinal() {
				sb.WriteString("doublecircle")
			} else {
				sb.WriteString("circle")
			}
			sb.WriteString(fmt.Sprintf("]; %v;\n", state.GetName()))
		}
		// default initial state transition
		defaultState := s.GetDefaultState()
		// will happen in an empty transition
		sb.WriteString(fmt.Sprintf("Empty -> %v [label = \"\"];\n", defaultState.GetName()))
		// begin regular transitions
		//Initial -> Primary [ label = "(H + v) mod R = i" ];
		for _, state := range s.GetStates() {
			for _, transition := range state.GetTransitions() {
				sb.WriteString(fmt.Sprintf("%v ", state.GetName()))
				stringFormat := transition.StringFormat(format)
				sb.WriteString(fmt.Sprintf("%v\n", stringFormat))
			}
		}
		sb.WriteString("}\n")
	} else {
		// standard text
		sb.WriteString("STSM {")
		sb.WriteString(fmt.Sprintf("#id = %v;", s.GetMe()))
		sb.WriteString(fmt.Sprintf("Timer='%v';", s.GetTimer()))
		sb.WriteString("States=[")
		for _, state := range s.GetStates() {
			sb.WriteString(fmt.Sprintf("%v;", state))
		}
		sb.WriteString("]}")
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
	return nil
}

func (s *SingleTimerStateMachineService) RegisterGlobal(transition single.Transition) error {
	if transition == nil {
		return errors.New("the transition can not be nil")
	}
	s.globalTransitions = append(s.globalTransitions, transition)
	return nil
}

func (s *SingleTimerStateMachineService) FindGlobalTransition(param single.Param) (single.Transition, error) {
	for _, transition := range s.GetGlobalTransitions() {
		resp, err := transition.IsValid(s.GetTimer(), param, s.GetMe())
		if err != nil {
			return nil, err
		}
		if resp {
			return transition, nil
		}
	}
	return nil, nil
}

func (s *SingleTimerStateMachineService) GetName() string {
	return s.getTimedStateMachine().GetName()
}

func (s *SingleTimerStateMachineService) Run(current single.State, param single.Param) (single.State, error) {
	return RunMachine(s, current, param)
}

func (s *SingleTimerStateMachineService) getTimedStateMachine() TimedStateMachine {
	return s.timedStateMachine
}

func (s *SingleTimerStateMachineService) RunDefault() (single.State, error) {
	return s.Run(nil, nil)
}
