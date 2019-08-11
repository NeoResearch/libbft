package single

import (
	"fmt"
	"github.com/NeoResearch/libbft/pkg/timing"
	"math/rand"
	"strings"
	"time"
)

type State interface {
	// get / set
	GetName() string
	GetTransitions() []Transition
	IsFinal() bool
	// Meethods
	AddTransition(transition Transition)
	TryGetTransition(timer timing.Timer, param Param, me int) Transition

	StringRecursive(recursive bool) string
	String() string
}

type StateService struct {
	name        string
	isFinal     bool
	transitions []Transition
}

func NewDefaultState(isFinal bool, name string) State {
	return NewState(false, "")
}

func NewState(isFinal bool, name string) State {
	return &StateService{
		name,
		isFinal,
		make([]Transition, 1),
	}
}

func (s *StateService) GetTransitions() []Transition {
	return s.transitions
}

func (s *StateService) AddTransition(transition Transition) {
	s.transitions = append(s.transitions, transition)
}

func (s *StateService) TryGetTransition(timer timing.Timer, param Param, me int) Transition {
	rand.Seed(time.Now().UnixNano())
	transitions := make([]Transition, len(s.transitions))
	copy(transitions, s.transitions)
	rand.Shuffle(len(s.transitions), func(i, j int) { s.transitions[i], s.transitions[j] = s.transitions[j], s.transitions[i] })
	for _, transition := range transitions {
		if transition.IsValid(timer, param, me) {
			return transition
		}
	}
	return nil
}

func (s *StateService) IsFinal() bool {
	return s.isFinal
}

func (s *StateService) String() string {
	return s.StringRecursive(true)
}

func (s *StateService) StringRecursive(recursive bool) string {
	var sb strings.Builder
	sb.WriteString("state:{")
	sb.WriteString(fmt.Sprintf("name='%v';", s.name))
	if s.IsFinal() {
		sb.WriteString("FINAL")
	}
	sb.WriteString(";")

	if recursive {
		sb.WriteString("transitions=[")
		for _, transition := range s.GetTransitions() {
			sb.WriteString(fmt.Sprintf("%v;", transition))
		}
		sb.WriteString("]")
	} else {
		sb.WriteString("...")
	}
	sb.WriteString("}")
	return sb.String()
}

func (s *StateService) GetName() string {
	return s.name
}
