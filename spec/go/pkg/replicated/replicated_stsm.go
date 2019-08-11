package replicated

import (
	"fmt"
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
	LaunchScheduleEvents(param MultiContext)
	InitializeMulti(current MultiState, param MultiContext) MultiState
	IsFinalMulti(current MultiState, param MultiContext) bool
	UpdateStateMulti(current MultiState, param MultiContext) (single.State, bool)
	OnEnterStateMulti(current MultiState, param MultiContext)
	BeforeUpdateStateMulti(current MultiState, param MultiContext) bool

	StringFormat(format string) string
	String() string
}

type ReplicatedSTSMService struct {
	timedStateMachine machine.TimedStateMachine
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
	r.getTimedStateMachine().OnFinished(current, param)
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
	panic("implement me")
}

func (r *ReplicatedSTSMService) GetScheduledEvents() []ScheduledEvent {
	panic("implement me")
}

func (r *ReplicatedSTSMService) GetWatchdog() timing.Timer {
	panic("implement me")
}

func (r *ReplicatedSTSMService) SetWatchdog(timer timing.Timer) {
	panic("implement me")
}

func (r *ReplicatedSTSMService) AddScheduleEvent(countdown float64, machine int, name string, eventParams []string) {
	panic("implement me")
}

func (r *ReplicatedSTSMService) RegisterMachine(machine machine.SingleTimerStateMachine) {
	panic("implement me")
}

func (r *ReplicatedSTSMService) LaunchScheduleEvents(param MultiContext) {
	panic("implement me")
}

func (r *ReplicatedSTSMService) InitializeMulti(current MultiState, param MultiContext) MultiState {
	panic("implement me")
}

func (r *ReplicatedSTSMService) IsFinalMulti(current MultiState, param MultiContext) bool {
	panic("implement me")
}

func (r *ReplicatedSTSMService) UpdateStateMulti(current MultiState, param MultiContext) (single.State, bool) {
	panic("implement me")
}

func (r *ReplicatedSTSMService) OnEnterStateMulti(current MultiState, param MultiContext) {
	panic("implement me")
}

func (r *ReplicatedSTSMService) BeforeUpdateStateMulti(current MultiState, param MultiContext) bool {
	panic("implement me")
}
