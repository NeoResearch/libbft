package timing

import (
	"fmt"
	"time"
)

type Clock interface {
	GetTime() float64
	String() string
}

type ClockService struct {
	name string
}

func NewClock(name string) Clock {
	return &ClockService{
		name,
	}
}

func (c *ClockService) String() string {
	return fmt.Sprintf("Clock {name='%v'}", c.name)
}

func (c *ClockService) GetTime() float64 {
	return float64(time.Now().Nanosecond()) / 1e9
}
