package dbft2

import (
	"errors"
	"github.com/NeoResearch/libbft/src/machine"
	"github.com/NeoResearch/libbft/src/replicated"
	"github.com/NeoResearch/libbft/src/single"
	"github.com/NeoResearch/libbft/src/timing"
)

type DBFT2Machine interface {
	// superclass TimedStateMachine
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

	// superclass
	getReplicatedSTSM() replicated.ReplicatedSTSM
	// get / set
	GetMachines() []machine.SingleTimerStateMachine
	GetScheduledEvents() []replicated.ScheduledEvent
	GetWatchdog() timing.Timer
	SetWatchdog(timer timing.Timer)
	AddScheduleEvent(countdown float64, machine int, name string, eventParams []string)
	RegisterMachine(machine machine.SingleTimerStateMachine)
	LaunchScheduleEvents(param replicated.MultiContext) error
	InitializeMulti(current replicated.MultiState, param replicated.MultiContext) (replicated.MultiState, error)
	IsFinalMulti(current replicated.MultiState, param replicated.MultiContext) bool
	UpdateStateMulti(current replicated.MultiState, param replicated.MultiContext) (replicated.MultiState, bool)
	OnEnterStateMulti(current replicated.MultiState, param replicated.MultiContext)
	BeforeUpdateStateMulti(current replicated.MultiState, param replicated.MultiContext) bool

	// get / set
	GetFaultyNodes() int
	// methods
	FillStatesForMachine(me int)

	StringFormat(format string) string
	String() string
}

type DBFT2MachineService struct {
	replicatedSTSM replicated.ReplicatedSTSM
	faultyNodes    int
}

func NewDefaultDBFT2Machine(faultyNodes int, numberOfMachines int, clock timing.Clock, me int, name string) (DBFT2Machine, error) {
	return NewDBFT2Machine(0, 1, nil, 0, "replicated_dBFT")
}

func NewDBFT2Machine(faultyNodes int, numberOfMachines int, clock timing.Clock, me int, name string) (DBFT2Machine, error) {
	if faultyNodes < 0 {
		return nil, errors.New("invalid number of faulty nodes")
	}
	if numberOfMachines < 1 {
		return nil, errors.New("invalid number of machines")
	}
	if faultyNodes > numberOfMachines {
		return nil, errors.New("number of faulty nodes can not be greater than the number of machines")
	}
	replicatedSTSM := replicated.NewReplicatedSTSMSizes(clock, me, name, numberOfMachines, 0)
	for i := 0; i < numberOfMachines; i++ {
		replicatedSTSM.GetMachines()[i] = machine.NewSingleTimerStateMachine(clock, i, "dBFT", timing.NewTimer("C", clock, -1))
	}

	resp := DBFT2MachineService{
		replicatedSTSM,
		faultyNodes,
	}

	for i := 0; i < numberOfMachines; i++ {
		resp.FillStatesForMachine(i)
	}
	return &resp, nil
}

func (d *DBFT2MachineService) getReplicatedSTSM() replicated.ReplicatedSTSM {
	return d.replicatedSTSM
}

func (d *DBFT2MachineService) GetFaultyNodes() int {
	return d.faultyNodes
}

func (d *DBFT2MachineService) GetClock() timing.Clock {
	return d.getReplicatedSTSM().GetClock()
}

func (d *DBFT2MachineService) GetMe() int {
	return d.getReplicatedSTSM().GetMe()
}

func (d *DBFT2MachineService) GetName() string {
	return d.getReplicatedSTSM().GetName()
}

func (d *DBFT2MachineService) OnEnterState(current single.State, param single.Param) {
	d.getReplicatedSTSM().OnEnterState(current, param)
}

func (d *DBFT2MachineService) BeforeUpdateState(current single.State, param single.Param) bool {
	return d.getReplicatedSTSM().BeforeUpdateState(current, param)
}

func (d *DBFT2MachineService) AfterUpdateState(current single.State, param single.Param, updated bool) bool {
	return d.getReplicatedSTSM().AfterUpdateState(current, param, updated)
}

func (d *DBFT2MachineService) UpdateState(current single.State, param single.Param) (single.State, bool) {
	return d.getReplicatedSTSM().UpdateState(current, param)
}

func (d *DBFT2MachineService) IsFinal(current single.State, param single.Param) bool {
	return d.getReplicatedSTSM().IsFinal(current, param)
}

func (d *DBFT2MachineService) Initialize(current single.State, param single.Param) single.State {
	return d.getReplicatedSTSM().Initialize(current, param)
}

func (d *DBFT2MachineService) OnFinished(current single.State, param single.Param) {
	d.getReplicatedSTSM().OnFinished(current, param)
}

func (d *DBFT2MachineService) Run(current single.State, param single.Param) single.State {
	return d.getReplicatedSTSM().Run(current, param)
}

func (d *DBFT2MachineService) GetMachines() []machine.SingleTimerStateMachine {
	return d.getReplicatedSTSM().GetMachines()
}

func (d *DBFT2MachineService) GetScheduledEvents() []replicated.ScheduledEvent {
	return d.getReplicatedSTSM().GetScheduledEvents()
}

func (d *DBFT2MachineService) GetWatchdog() timing.Timer {
	return d.getReplicatedSTSM().GetWatchdog()
}

func (d *DBFT2MachineService) SetWatchdog(timer timing.Timer) {
	d.getReplicatedSTSM().SetWatchdog(timer)
}

func (d *DBFT2MachineService) AddScheduleEvent(countdown float64, machine int, name string, eventParams []string) {
	d.getReplicatedSTSM().AddScheduleEvent(countdown, machine, name, eventParams)
}

func (d *DBFT2MachineService) RegisterMachine(machine machine.SingleTimerStateMachine) {
	d.getReplicatedSTSM().RegisterMachine(machine)
}

func (d *DBFT2MachineService) LaunchScheduleEvents(param replicated.MultiContext) error {
	return d.getReplicatedSTSM().LaunchScheduleEvents(param)
}

func (d *DBFT2MachineService) InitializeMulti(current replicated.MultiState, param replicated.MultiContext) (replicated.MultiState, error) {
	return d.getReplicatedSTSM().InitializeMulti(current, param)
}

func (d *DBFT2MachineService) IsFinalMulti(current replicated.MultiState, param replicated.MultiContext) bool {
	return d.getReplicatedSTSM().IsFinalMulti(current, param)
}

func (d *DBFT2MachineService) UpdateStateMulti(current replicated.MultiState, param replicated.MultiContext) (replicated.MultiState, bool) {
	return d.getReplicatedSTSM().UpdateStateMulti(current, param)
}

func (d *DBFT2MachineService) OnEnterStateMulti(current replicated.MultiState, param replicated.MultiContext) {
	d.getReplicatedSTSM().OnEnterStateMulti(current, param)
}

func (d *DBFT2MachineService) BeforeUpdateStateMulti(current replicated.MultiState, param replicated.MultiContext) bool {
	return d.getReplicatedSTSM().BeforeUpdateStateMulti(current, param)
}

func (d *DBFT2MachineService) StringFormat(format string) string {
	return d.getReplicatedSTSM().StringFormat(format)
}

func (d *DBFT2MachineService) String() string {
	return d.getReplicatedSTSM().String()
}
func (d *DBFT2MachineService) FillStatesForMachine(me int) {
	panic("implement me")
}
