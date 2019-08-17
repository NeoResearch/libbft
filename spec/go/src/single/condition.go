package single

import (
	"github.com/NeoResearch/libbft/src/timing"
)

type TimedFunction func(timing.Timer, Param, int) (bool, error)

type Condition interface {
	// get / set
	GetName() string
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

func (c *ConditionService) GetName() string {
	return c.name
}
