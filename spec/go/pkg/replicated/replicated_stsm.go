package replicated

import (
	"errors"
	"fmt"
	"github.com/NeoResearch/libbft/pkg/events"
	"github.com/NeoResearch/libbft/pkg/machine"
	"github.com/NeoResearch/libbft/pkg/single"
	"github.com/NeoResearch/libbft/pkg/timing"
	"strings"
)

type MultiState []single.State

type ReplicatedSTSM interface {
	// superclass
	getTimedStateMachine() machine.TimedStateMachine
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
	GetMachines() []machine.SingleTimerStateMachine
	GetScheduledEvents() []ScheduledEvent
	GetWatchdog() timing.Timer
	SetWatchdog(timer timing.Timer)
	AddScheduleEvent(countdown float64, machine int, name string, eventParams []string)
	RegisterMachine(machine machine.SingleTimerStateMachine)
	LaunchScheduleEvents(param MultiContext) error
	InitializeMulti(current MultiState, param MultiContext) (MultiState, error)
	IsFinalMulti(current MultiState, param MultiContext) bool
	UpdateStateMulti(current MultiState, param MultiContext) (MultiState, bool)
	OnEnterStateMulti(current MultiState, param MultiContext)
	BeforeUpdateStateMulti(current MultiState, param MultiContext) bool

	StringFormat(format string) string
	String() string
}

type ReplicatedSTSMService struct {
	timedStateMachine machine.TimedStateMachine
	machines          []machine.SingleTimerStateMachine
	scheduledEvents   []ScheduledEvent
	watchdog          timing.Timer
}

func NewReplicatedSTSM(clock timing.Clock, me int, name string) ReplicatedSTSM {
	return &ReplicatedSTSMService{
		timedStateMachine: machine.NewTimedStateMachine(clock, me, name),
	}
}

func (r *ReplicatedSTSMService) getTimedStateMachine() machine.TimedStateMachine {
	return r.timedStateMachine
}

func (r *ReplicatedSTSMService) GetClock() timing.Clock {
	return r.getTimedStateMachine().GetClock()
}

func (r *ReplicatedSTSMService) GetMe() int {
	return r.getTimedStateMachine().GetMe()
}

func (r *ReplicatedSTSMService) GetName() string {
	return r.getTimedStateMachine().GetName()
}

func (r *ReplicatedSTSMService) OnEnterState(current single.State, param single.Param) {
	r.getTimedStateMachine().OnEnterState(current, param)
}

func (r *ReplicatedSTSMService) BeforeUpdateState(current single.State, param single.Param) bool {
	return r.getTimedStateMachine().BeforeUpdateState(current, param)
}

func (r *ReplicatedSTSMService) AfterUpdateState(current single.State, param single.Param, updated bool) bool {
	return r.getTimedStateMachine().AfterUpdateState(current, param, updated)
}

func (r *ReplicatedSTSMService) UpdateState(current single.State, param single.Param) (single.State, bool) {
	return r.getTimedStateMachine().UpdateState(current, param)
}

func (r *ReplicatedSTSMService) IsFinal(current single.State, param single.Param) bool {
	return r.getTimedStateMachine().IsFinal(current, param)
}

func (r *ReplicatedSTSMService) Initialize(current single.State, param single.Param) single.State {
	return r.getTimedStateMachine().Initialize(current, param)
}

func (r *ReplicatedSTSMService) OnFinished(current single.State, param single.Param) {
	fmt.Println()
	fmt.Println("=================")
	fmt.Println("finished machine!")
	fmt.Println("=================")
}

func (r *ReplicatedSTSMService) Run(current single.State, param single.Param) single.State {
	return r.getTimedStateMachine().Run(current, param)
}

func (r *ReplicatedSTSMService) StringFormat(format string) string {
	var sb strings.Builder
	if format == "graphviz" {
	} else {
		sb.WriteString("ReplicatedSTSM [")
		for _, machine := range r.GetMachines() {
			sb.WriteString(fmt.Sprintf("%v;", machine))
		}
		sb.WriteString("]")

	}
	return sb.String()
}

func (r *ReplicatedSTSMService) String() string {
	return r.StringFormat("")
}

func (r *ReplicatedSTSMService) GetMachines() []machine.SingleTimerStateMachine {
	return r.machines
}

func (r *ReplicatedSTSMService) GetScheduledEvents() []ScheduledEvent {
	return r.scheduledEvents
}

func (r *ReplicatedSTSMService) GetWatchdog() timing.Timer {
	return r.watchdog
}

func (r *ReplicatedSTSMService) SetWatchdog(timer timing.Timer) {
	r.watchdog = timer
}

func (r *ReplicatedSTSMService) AddScheduleEvent(countdown float64, machine int, name string, eventParams []string) {
	r.scheduledEvents = append(r.scheduledEvents, NewScheduledEvent(name, eventParams, countdown, machine))
}

func (r *ReplicatedSTSMService) RegisterMachine(machine machine.SingleTimerStateMachine) {
	r.machines = append(r.machines, machine)
}

func (r *ReplicatedSTSMService) LaunchScheduleEvents(param MultiContext) error {
	fmt.Println("launching scheduled events!")
	for _, event := range r.GetScheduledEvents() {
		if event.GetMachine() == -1 {
			return param.BroadcastEvent(events.NewTimedEvent(event.GetName(), -1, event.GetEventParams(), event.GetCountdown()), -1)
		} else {
			return param.SendToVmEvent(events.NewTimedEvent(event.GetName(), -1, event.GetEventParams(), event.GetCountdown()), event.GetMachine())
		}
	}
	return nil
}

func (r *ReplicatedSTSMService) InitializeMulti(current MultiState, param MultiContext) (MultiState, error) {
	if current == nil {
		//current = new MultiState<Param>(machines.size(), nullptr);
	}
	if len(current) != len(r.GetMachines()) {
		return nil, errors.New("invalid number of states")
	}

	fmt.Println()
	fmt.Println("===========")
	fmt.Println("begin run()")
	fmt.Println("===========")

	fmt.Println("initializing multimachine")
	if r.GetWatchdog() != nil {
		r.GetWatchdog().Reset()
	} else {
		fmt.Println("No watchdog configured")
	}

	for i, machine := range r.GetMachines() {
		machine.Initialize(current[i], param)
	}
	err := r.LaunchScheduleEvents(param)

	return current, err
}

func (r *ReplicatedSTSMService) IsFinalMulti(current MultiState, param MultiContext) bool {
	for _, state := range current {
		if state == nil || !state.IsFinal() {
			return false
		}
	}
	return true
}

func (r *ReplicatedSTSMService) UpdateStateMulti(current MultiState, param MultiContext) (MultiState, bool) {
	resp := false
	temp := false
	for i, machine := range r.GetMachines() {
		if  current[i], temp = machine.UpdateState(current[i], param); temp {
			resp = true
		}
	}

	return current, resp
}

func (r *ReplicatedSTSMService) OnEnterStateMulti(current MultiState, param MultiContext) {
	fmt.Println("updating multi state! STATES:")
	for i, state := range current {
		fmt.Printf("Machine %v => %v\n", i, state)
	}
	if r.GetWatchdog() != nil {
		r.GetWatchdog().Reset()
	}
}

func (r *ReplicatedSTSMService) BeforeUpdateStateMulti(current MultiState, param MultiContext) bool {
	if r.GetWatchdog() != nil && r.GetWatchdog().Expired() {
		fmt.Printf("StateMachine FAILED MAXTIME %v\n", r.GetWatchdog().GetCountdown())
		return true
	}
	return false
}
