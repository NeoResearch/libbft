package single

import (
	"fmt"
	"github.com/NeoResearch/libbft/src/timing"
	"github.com/NeoResearch/libbft/src/util"
	"strings"
)

type Transition interface {
	// get / set
	GetActions() []Action
	GetConditions() []Condition
	GetName() string
	GetTo() State

	// methods
	AddCondition(condition Condition) Transition
	AddAction(action Action) Transition
	IsValid(timer timing.Timer, param Param, me int) (bool, error)
	Execute(timer timing.Timer, param Param, me int) State

	StringFormat(format string) string
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
		make([]Condition, 0),
		make([]Action, 0),
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

func (t *TransitionService) StringFormat(format string) string {
	var sb strings.Builder
	if format == util.GraphivizFormat {
		sb.WriteString(fmt.Sprintf(" -> %v", t.GetTo().GetName()))
		sb.WriteString(" [ label = \"")
		first := true
		for _, condition := range t.GetConditions() {
			if !first {
				sb.WriteString(" \n ")
			}
			sb.WriteString(fmt.Sprintf("%s ", condition.GetName()))
			first = false
		}
		for _, action := range t.GetActions() {
			if !first {
				sb.WriteString(" \n ")
			}
			sb.WriteString(fmt.Sprintf("%s ", action.GetName()))
			first = false
		}
		sb.WriteString("\"];")
	} else {
		sb.WriteString(fmt.Sprintf("t() => {name = '%v',", t.GetName()))
		sb.WriteString(fmt.Sprintf("to='%v',", t.GetTo().StringRecursive(false)))
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
	}

	return sb.String()
}

func (t *TransitionService) String() string {
	return t.StringFormat("")
}

func (t *TransitionService) GetTo() State {
	return t.to
}

func (t *TransitionService) GetName() string {
	return t.name
}
