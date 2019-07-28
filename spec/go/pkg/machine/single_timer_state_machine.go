package machine

import (
	"github.com/NeoResearch/libbft/pkg/single"
	"github.com/NeoResearch/libbft/pkg/timing"
)

type SingleTimerStateMachine interface {
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
