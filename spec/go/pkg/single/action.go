package single

import "github.com/NeoResearch/libbft/pkg/timing"

type TimedAction func(timing.Timer, Param, int) bool

type Action interface {
	// get / set
	GetTimedAction() TimedAction

	String() string
}

type ActionService struct {
	name        string
	timedAction TimedAction
}

func NewAction(name string, action TimedAction) Action {
	return &ActionService{
		name,
		action,
	}
}

func (a *ActionService) String() string {
	return a.name
}

func (a *ActionService) GetTimedAction() TimedAction {
	return a.timedAction
}
