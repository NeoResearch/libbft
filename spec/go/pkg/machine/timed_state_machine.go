package machine

import (
	"fmt"
	"github.com/NeoResearch/libbft/pkg/single"
	"github.com/NeoResearch/libbft/pkg/timing"
	"strings"
	"time"
)

type StateType interface{}

type TimedStateMachine interface {
	GetClock() timing.Clock
	GetMe() int

	OnEnterState(current StateType, param single.Param)
	BeforeUpdateState(current StateType, param single.Param) bool
	AfterUpdateState(current StateType, param single.Param, updated bool) bool
	UpdateState(current StateType, param single.Param) bool
	IsFinal(current StateType, param single.Param) bool
	Initialize(current StateType, param single.Param) StateType
	OnFinished(current StateType, param single.Param)
	Run(current StateType, param single.Param)

	String() string
}

type TimedStateMachineService struct {
	clock timing.Clock
	me    int
}

func NewTimedStateMachine(clock timing.Clock, me int) TimedStateMachine {
	return &TimedStateMachineService{
		clock,
		me,
	}
}

func (t *TimedStateMachineService) GetClock() timing.Clock {
	return t.clock
}

func (t *TimedStateMachineService) GetMe() int {
	return t.me
}

func (t *TimedStateMachineService) BeforeUpdateState(current StateType, param single.Param) bool {
	panic("implement me")
}

func (t *TimedStateMachineService) UpdateState(current StateType, param single.Param) bool {
	panic("implement me")
}

func (t *TimedStateMachineService) IsFinal(current StateType, param single.Param) bool {
	panic("implement me")
}

func (t *TimedStateMachineService) Initialize(current StateType, param single.Param) StateType {
	panic("implement me")
}

func (t *TimedStateMachineService) OnEnterState(current StateType, param single.Param) {}

func (t *TimedStateMachineService) AfterUpdateState(current StateType, param single.Param, updated bool) bool {
	if !updated {
		time.Sleep(100 * time.Millisecond)
	}
	return false
}

func (t *TimedStateMachineService) OnFinished(current StateType, param single.Param) {}

func (t *TimedStateMachineService) Run(current StateType, param single.Param) {
	panic("implement me")
}

func (t *TimedStateMachineService) String() string {
	var sb strings.Builder
	sb.WriteString("TSM {")
	sb.WriteString(fmt.Sprintf("#id = %v;", t.GetMe()))
	sb.WriteString(fmt.Sprintf("clock = %v;", t.GetClock()))
	sb.WriteString("TSM }")
	return sb.String()
}
