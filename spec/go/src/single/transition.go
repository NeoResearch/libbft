package single

import (
	"fmt"
	"github.com/NeoResearch/libbft/src/timing"
	"strings"
)

type Transition interface {
	// get / set
	GetConditions() []Condition
	GetActions() []Action
	// methods
	AddCondition(condition Condition) Transition
	AddAction(action Action) Transition
	IsValid(timer timing.Timer, param Param, me int) (bool, error)
	Execute(timer timing.Timer, param Param, me int) State

	String() string
}

type TransitionService struct {
	to         State
	name       string
	conditions []Condition
	actions    []Action
}

func NewTransactionState(state State) Transition {
	return NewTransaction(state, "")
}

func NewTransaction(state State, name string) Transition {
	return &TransitionService{
		state,
		name,
		make([]Condition, 1),
		make([]Action, 1),
	}
}

func (t *TransitionService) AddCondition(condition Condition) Transition {
	t.conditions = append(t.conditions, condition)
	return t
}

func (t *TransitionService) AddAction(action Action) Transition {
	t.actions = append(t.actions, action)
	return t
}

func (t *TransitionService) IsValid(timer timing.Timer, param Param, me int) (bool, error) {
	for _, condition := range t.GetConditions() {
		resp, err := condition.GetTimedFunction()(timer, param, me)
		if err != nil {
			return resp, err
		}
		if !resp {
			return false, nil
		}
	}
	return true, nil
}

func (t *TransitionService) Execute(timer timing.Timer, param Param, me int) State {
	for _, action := range t.GetActions() {
		action.GetTimedAction()(timer, param, me)
	}
	return t.to
}

func (t *TransitionService) GetConditions() []Condition {
	return t.conditions
}

func (t *TransitionService) GetActions() []Action {
	return t.actions
}

func (t *TransitionService) String() string {
	var sb strings.Builder
	sb.WriteString(fmt.Sprintf("t() => {name = '%v',", t.name))
	sb.WriteString(fmt.Sprintf("to='%v',", t.to))
	sb.WriteString("conditions=[")
	for _, condition := range t.GetConditions() {
		sb.WriteString(fmt.Sprintf("%v;", condition))
	}
	sb.WriteString("], ")
	sb.WriteString("actions=[")
	for _, action := range t.GetActions() {
		sb.WriteString(fmt.Sprintf("%v;", action))
	}
	sb.WriteString("], ")
	sb.WriteString("}")
	return sb.String()
}
