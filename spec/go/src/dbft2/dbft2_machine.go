package dbft2

import (
	"errors"
	"fmt"
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
	UpdateStateMulti(current replicated.MultiState, param replicated.MultiContext) (replicated.MultiState, bool, error)
	OnEnterStateMulti(current replicated.MultiState, param replicated.MultiContext)
	BeforeUpdateStateMulti(current replicated.MultiState, param replicated.MultiContext) bool

	// get / set
	GetFaultyNodes() int
	// methods
	FillStatesForMachine(me int) error
	FillSimpleCycle(me int) error

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
	if numberOfMachines < 1 {
		return nil, errors.New("invalid number of machines")
	}
	machines := make([]machine.SingleTimerStateMachine, numberOfMachines)
	for i := 0; i < numberOfMachines; i++ {
		machines[i] = machine.NewSingleTimerStateMachine(clock, i, "dBFT", timing.NewTimer("C", clock, -1))
	}

	resp, err := NewDBFT2MachineMachines(faultyNodes, machines, clock, me, name)
	if err != nil {
		return nil, err
	}
	for i := 0; i < numberOfMachines; i++ {
		err = resp.FillStatesForMachine(i)
		if err != nil {
			return nil, err
		}
	}
	return resp, err
}

func NewDBFT2MachineMachines(faultyNodes int, machines []machine.SingleTimerStateMachine, clock timing.Clock, me int, name string) (DBFT2Machine, error) {
	if faultyNodes < 0 {
		return nil, errors.New("invalid number of faulty nodes")
	}
	if len(machines) < 1 {
		return nil, errors.New("invalid number of machines")
	}
	if faultyNodes > len(machines) {
		return nil, errors.New("number of faulty nodes can not be greater than the number of machines")
	}
	replicatedSTSM := replicated.NewReplicatedSTSMSizes(clock, me, name, len(machines), 0)
	replicatedSTSM.SetMachines(machines)
	resp := DBFT2MachineService{
		replicatedSTSM,
		faultyNodes,
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

func (d *DBFT2MachineService) UpdateStateMulti(current replicated.MultiState, param replicated.MultiContext) (replicated.MultiState, bool, error) {
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

func (d *DBFT2MachineService) FillStatesForMachine(me int) error {
	return d.FillSimpleCycle(me)
}

func (d *DBFT2MachineService) FillSimpleCycle(me int) error {
	machine := d.GetMachines()[me]
	// ---------------------
	// declaring dBFT states
	// ---------------------
	preInitial := single.NewState(false, "PreInitial")
	err := machine.RegisterState(preInitial)
	if err != nil {
		return err
	}
	started := single.NewState(false, "Started")
	err = machine.RegisterState(started)
	if err != nil {
		return err
	}
	backup := single.NewState(false, "Backup")
	err = machine.RegisterState(backup)
	if err != nil {
		return err
	}
	primary := single.NewState(false, "Primary")
	err = machine.RegisterState(primary)
	if err != nil {
		return err
	}
	requestSentOrReceived := single.NewState(false, "RequestSentOrReceived")
	err = machine.RegisterState(requestSentOrReceived)
	if err != nil {
		return err
	}
	commitSent := single.NewState(false, "CommitSent")
	err = machine.RegisterState(commitSent)
	if err != nil {
		return err
	}
	viewChanging := single.NewState(false, "ViewChanging")
	err = machine.RegisterState(viewChanging)
	if err != nil {
		return err
	}
	blockSent := single.NewState(true, "BlockSent")
	err = machine.RegisterState(blockSent)
	if err != nil {
		return err
	}

	// -------------------------
	// creating dBFT transitions
	// -------------------------
	// preinitial -> started
	preInitial.AddTransition(single.NewTransactionState(started).AddCondition(single.NewCondition("OnStart()", func(t timing.Timer, d single.Param, me int) (bool, error) {
		fmt.Println("Waiting for OnStart...")
		dMc := d.(replicated.MultiContext)
		return dMc.HasEvent("OnStart", me, make([]string, 0))
	})))

	// initial -> backup
	started.AddTransition(single.NewTransactionState(backup).AddCondition(single.NewCondition("not (H+v) mod R = i", func(t timing.Timer, d single.Param, me int) (bool, error) {
		fmt.Println("lambda1")
		dMc := d.(replicated.MultiContext)
		dbfContext := dMc.GetVm()[me].GetParams().(DBFT2Context)
		//(d->vm[me].params->H + d->vm[me].params->v) % d->vm[me].params->R != me
		return (dbfContext.GetHeight()+dbfContext.GetView())%dbfContext.GetNumberOfNodes() != me, nil
	})))
	// initial -> primary
	started.AddTransition(single.NewTransactionState(primary).AddCondition(single.NewCondition("(H+v) mod R = i", func(t timing.Timer, d single.Param, me int) (bool, error) {
		dMc := d.(replicated.MultiContext)
		dbfContext := dMc.GetVm()[me].GetParams().(DBFT2Context)
		fmt.Printf("lambda2 H=%v V=%v me=%v\n", dbfContext.GetHeight(), dbfContext.GetView(), me)
		return (dbfContext.GetHeight()+dbfContext.GetView())%dbfContext.GetNumberOfNodes() == me, nil
	})))

	// backup -> reqSentOrRecv
	backup.AddTransition(single.NewTransactionState(requestSentOrReceived).AddCondition(single.NewCondition("OnPrepareRequest(v)", func(t timing.Timer, d single.Param, me int) (bool, error) {
		dMc := d.(replicated.MultiContext)
		dbfContext := dMc.GetVm()[me].GetParams().(DBFT2Context)
		fmt.Printf("waiting for event OnPrepareRequest at %v for view %v\n", me, dbfContext.GetView())
		params := make([]string, 1)
		params[0] = string(dbfContext.GetView())
		return dMc.HasEvent("OnPrepareRequest", me, params)
	})))

	// reqSentOrRecv -> commitSent
	requestSentOrReceived.AddTransition(single.NewTransactionState(commitSent).AddCondition(single.NewCondition("OnStart()", func(t timing.Timer, d single.Param, me int) (bool, error) {
		fmt.Println("nothing to do... assuming all preparations were received!")
		return true, nil
	})))

	// commitSent -> blockSent
	commitSent.AddTransition(single.NewTransactionState(blockSent).AddCondition(single.NewCondition("OnStart()", func(t timing.Timer, d single.Param, me int) (bool, error) {
		fmt.Println("nothing to do... assuming all commits were received!")
		return true, nil
	})))

	return nil
}
