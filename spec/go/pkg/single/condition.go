package single

import (
	"github.com/NeoResearch/libbft/pkg/timing"
)

type TimedFunction func(timing.Timer, Param, int) bool

type Condition interface {
	String() string
	TimedFunction() TimedFunction
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

func (c *ConditionService) TimedFunction() TimedFunction {
	return c.timedFunction
}
