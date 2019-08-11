package single

import (
	"github.com/NeoResearch/libbft/pkg/timing"
)

type TimedFunction func(timing.Timer, Param, int) bool

type Condition interface {
	// get / set
	GetTimedFunction() TimedFunction
	// methods
	String() string
}

type ConditionService struct {
	name          string
	timedFunction TimedFunction
}

func NewCondition(name string, timedFunction TimedFunction) Condition {
	return &ConditionService{
		name,
		timedFunction,
	}
}

func (c *ConditionService) String() string {
	return c.name
}

func (c *ConditionService) GetTimedFunction() TimedFunction {
	return c.timedFunction
}
